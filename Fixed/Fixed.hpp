#pragma once

#include "TypeConvert.hpp"
#include <cstddef>
#include <type_traits>

// #define EXFIXEDPOINT_PERFORMANCE_MONITORING

#ifdef EXFIXEDPOINT_PERFORMANCE_MONITORING
#include <iostream>
#define EX_PFM_OUT(format, ...) std::printf(format, __VA_ARGS__)
#else
#define EX_PFM_OUT(...)
#endif

namespace ExFixedPoint {

/**
 * @brief
 *
 * @tparam BaseType
 * @tparam fracBits
 * @tparam allowTypeElevation
 */
template <typename BaseType, unsigned int fracBits, bool allowTypeElevation = true>
class Fixed {
    static_assert(fracBits < sizeof(BaseType) * 8 - 1, "tparam fracBits must lower than BaseType bits number, and must reserve at least 1 bit for integer");
    static_assert(fracBits > 0, "tparam fracBits must greater than 0, and must reserve at least 1 bit for fraction");
    static_assert(std::is_integral<BaseType>::value, "tparam BaseType must be a integral type");

  public:
    struct ConstructFromRawValue {};
    static constexpr BaseType fixedMutli = static_cast<BaseType>(1u) << fracBits;
    inline explicit constexpr Fixed(BaseType value, ConstructFromRawValue) noexcept : value(value) {}

    BaseType value;

    inline constexpr Fixed(void) noexcept : value(0) {
        EX_PFM_OUT("Construct with zero Initilize\n");
    };

#ifdef EXFIXEDPOINT_PERFORMANCE_MONITORING

    inline constexpr Fixed(const Fixed &src) noexcept : value(src.value) {
        EX_PFM_OUT("Copy Construct\n");
    }

    inline constexpr Fixed(const Fixed &&src) noexcept : value(src.value) {
        EX_PFM_OUT("Move Construct\n");
    }

#endif

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed(Integer number) noexcept : value(static_cast<BaseType>(number << fracBits)) {
        EX_PFM_OUT("Construct From Integer %lld\n", (long long)number);
    }

    template <typename FloatingPoint, typename std::enable_if<std::is_floating_point<FloatingPoint>::value>::type * = nullptr>
    inline constexpr Fixed(FloatingPoint number) noexcept : value(static_cast<BaseType>(number * fixedMutli)) {
        EX_PFM_OUT("Construct From FloatingPoint %f\n", (double)number);
    }

    template <typename OtherBaseType, unsigned int otherFracBits, bool unused>
    inline constexpr Fixed(const Fixed<OtherBaseType, otherFracBits, unused> &other) noexcept : value(
                                                                                                    static_cast<BaseType>((sizeof(BaseType) > sizeof(OtherBaseType)) ? ((fracBits > otherFracBits) ? static_cast<BaseType>(other.value) << (fracBits - otherFracBits) : static_cast<BaseType>(other.value) >> (otherFracBits - fracBits)) : ((fracBits > otherFracBits) ? static_cast<OtherBaseType>(other.value) << (fracBits - otherFracBits) : static_cast<OtherBaseType>(other.value) >> (otherFracBits - fracBits)))) {
        EX_PFM_OUT("Construct From Fixed<%s, %d, %d>\n", typeid(OtherBaseType).name(), otherFracBits, unused);
    }

    inline constexpr Fixed floor(void) const noexcept { return Fixed(value & ~(fixedMutli - 1), ConstructFromRawValue{}); }

    inline constexpr Fixed ceil(void) const noexcept { return Fixed((value & ~(fixedMutli - 1)) + fixedMutli, ConstructFromRawValue{}); }
    inline constexpr Fixed round(void) const noexcept { return ((value & (fixedMutli - 1)) < (static_cast<BaseType>(1) << (fracBits - 1))) ? floor() : ceil(); }

    template <typename Any>
    inline static constexpr Fixed from(const Any &number) noexcept { return Fixed(number); }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Integer as(void) const noexcept { return static_cast<Integer>(round().value >> fracBits); }

    template <typename FloatingPoint, typename std::enable_if<std::is_floating_point<FloatingPoint>::value>::type * = nullptr>
    inline constexpr FloatingPoint as(void) const noexcept { return static_cast<FloatingPoint>(value) / fixedMutli; }

    template <typename Any>
    inline explicit constexpr operator Any(void) const noexcept { return as<Any>(); }

    constexpr inline Fixed &operator-(void) const noexcept {
        return Fixed(-this->value, ConstructFromRawValue{});
    }

    inline Fixed &operator+=(const Fixed &other) noexcept {
        this->value += other.value;
        return *this;
    }

    inline Fixed &operator-=(const Fixed &other) noexcept {
        this->value -= other.value;
        return *this;
    }

    template <bool condition = allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    inline Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> operator*=(const Fixed &other) noexcept {
        static_assert(std::is_integral<LargerInteger_t<BaseType>>::value, "No larger integer can contain the result of multiplication");
        EX_PFM_OUT("Accurate multiplication\n");
        Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> result;
        result.value = static_cast<LargerInteger_t<BaseType>>(this->value) * other.value;
        this->value = static_cast<BaseType>(result.value >> fracBits);
        return result;
    }

    template <bool condition = not allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    inline Fixed &operator*=(const Fixed &other) noexcept {
        EX_PFM_OUT("Fast multiplication\n");
        this->value = (this->value >> (fracBits / 2)) * (other.value >> (fracBits / 2 + fracBits % 2));
        return *this;
    }

    template <bool condition = allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    inline Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> operator/=(const Fixed &other) noexcept {
        static_assert(std::is_integral<LargerInteger_t<BaseType>>::value, "No larger integer can contain the result of division");
        EX_PFM_OUT("Accurate division\n");
        Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> result;
        result.value = ((LargerInteger_t<BaseType>)this->value << fracBits) / ((LargerInteger_t<BaseType>)other.value) << fracBits;
        this->value = result.value;
        return result;
    }

    template <bool condition = not allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    inline Fixed &operator/=(const Fixed &other) noexcept {
        EX_PFM_OUT("Fast division\n");
        this->value = (this->value << (fracBits / 2)) / (other.value << (fracBits / 2 + fracBits % 2));
        return *this;
    }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline Fixed &operator+=(Integer number) noexcept {
        EX_PFM_OUT("Integer addition\n");
        this->value += number << fracBits;
        return *this;
    }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline Fixed &operator-=(Integer number) noexcept {
        EX_PFM_OUT("Integer subtraction\n");
        this->value -= number << fracBits;
        return *this;
    }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline Fixed &operator*=(Integer number) noexcept {
        EX_PFM_OUT("Integer multiplication\n");
        this->value *= number;
        return *this;
    }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline Fixed &operator/=(Integer number) noexcept {
        EX_PFM_OUT("Integer division\n");
        this->value /= number;
        return *this;
    }

    constexpr inline Fixed operator+(const Fixed &other) const noexcept { return Fixed(*this) += other; }

    constexpr inline Fixed operator-(const Fixed &other) const noexcept { return Fixed(*this) -= other; }

    template <bool condition = allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    constexpr inline Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> operator*(const Fixed &other) const noexcept { return Fixed(*this) *= other; }

    template <bool condition = not allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    constexpr inline Fixed operator*(const Fixed &other) const noexcept { return Fixed(*this) *= other; }

    template <bool condition = allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    constexpr inline Fixed<LargerInteger_t<BaseType>, fracBits * 2, allowTypeElevation> operator/(const Fixed &other) const noexcept { return Fixed(*this) /= other; }

    template <bool condition = not allowTypeElevation, typename std::enable_if<condition>::type * = nullptr>
    constexpr inline Fixed operator/(const Fixed &other) const noexcept { return Fixed(*this) /= other; }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed operator+(Integer number) const noexcept { return Fixed(*this) += number; }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed operator-(Integer number) const noexcept { return Fixed(*this) -= number; }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed operator*(Integer number) const noexcept { return Fixed(*this) *= number; }

    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed operator/(Integer number) const noexcept { return Fixed(*this) /= number; }

    inline constexpr bool operator==(const Fixed &other) const noexcept { return this->value == other.value; }
    inline constexpr bool operator!=(const Fixed &other) const noexcept { return this->value != other.value; }
    inline constexpr bool operator<(const Fixed &other) const noexcept { return this->value < other.value; }
    inline constexpr bool operator>(const Fixed &other) const noexcept { return this->value > other.value; }
    inline constexpr bool operator<=(const Fixed &other) const noexcept { return this->value <= other.value; }
    inline constexpr bool operator>=(const Fixed &other) const noexcept { return this->value >= other.value; }

    inline Fixed &operator=(const Fixed &other) noexcept {
        EX_PFM_OUT("Copy Assignment Operator\n");
        this->value = other.value;
        return *this;
    }
};

template <typename Number, typename B, unsigned int F, bool A, typename std::enable_if<std::is_arithmetic<Number>::value>::type * = nullptr>
constexpr Fixed<B, F, A> operator+(Number x, const Fixed<B, F, A> &y) noexcept {
    return Fixed<B, F, A>(x) + y;
}

template <typename Number, typename B, unsigned int F, bool A, typename std::enable_if<std::is_arithmetic<Number>::value>::type * = nullptr>
constexpr Fixed<B, F, A> operator-(Number x, const Fixed<B, F, A> &y) noexcept {
    return Fixed<B, F, A>(x) - y;
}

template <typename Number, typename B, unsigned int F, bool A, typename std::enable_if<std::is_arithmetic<Number>::value>::type * = nullptr>
constexpr Fixed<B, F, A> operator*(Number x, const Fixed<B, F, A> &y) noexcept {
    return Fixed<B, F, A>(x) * y;
}

template <typename Number, typename B, unsigned int F, bool A, typename std::enable_if<std::is_arithmetic<Number>::value>::type * = nullptr>
constexpr Fixed<B, F, A> operator/(Number x, const Fixed<B, F, A> &y) noexcept {
    return Fixed<B, F, A>(x) / y;
}

} // namespace ExFixedPoint
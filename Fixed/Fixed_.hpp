#pragma once

#include "TypeConvert.hpp"
#include <cstddef>
#include <type_traits>

namespace ExFixedPoint {

/**
 * @brief FixedPoint Number class
 *
 * @tparam IntType Int type for storage FixedPoint Number, share the same symbolism.
 * @tparam fracBits fraction occupies in a fixed-point number
 */
template <typename IntType, int fracBits, bool allowTypeElevation>
class Fixed {
    static_assert(
        fracBits < sizeof(IntType) * 8 || fracBits > 0,
        "fracBits must between IntType bits and 0 (not included).");

    static_assert(
        std::is_integral<IntType>::value,
        "IntType must be integral");

  protected:
    static constexpr std::make_unsigned_t<IntType> midFrac = 1 << (fracBits - 1);

    /**
     * @brief value of fixed point
     *
     */

  public:
    struct Constant {
        static constexpr Fixed<IntType, fracBits> PI = (double)3.14159265358979323846;
        static constexpr Fixed<IntType, fracBits> EXP = (double)2.71828182845904523536;
    };

    struct Value {
        std::make_unsigned_t<IntType> frac : fracBits;
        IntType integer : sizeof(IntType) * 8 - fracBits;
    };

    union Combined {
        IntType combined;
        Value separated;
    };

    Combined value;

    /**
     * @brief Default constructor
     *
     */

    Fixed(void) = default;

    /**
     * @brief Construct from IntegerType
     *
     * @tparam Integer IntegerType
     * @tparam std::enable_if<std::is_integral<Integer>::value>::type
     */
    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Fixed(Integer integer)
        : value{static_cast<IntType>(integer) << fracBits} {
    }
    /**
     * @brief Construct from FloatingPointType
     *
     * @tparam Floating FloatingPointType
     * @tparam std::enable_if<std::is_floating_point<Floating>::value>::type
     */
    template <typename Floating, typename std::enable_if<std::is_floating_point<Floating>::value>::type * = nullptr>
    inline constexpr Fixed(Floating floating)
        : value{static_cast<IntType>(floating * (1u << fracBits))} {
    }

    /**
     * @brief Construct from other FixedNumber
     *
     * @tparam OtherIntType don't care, compiler automatically populates this
     * @tparam otherFracBits don't care compiler automatically populates this
     */
    template <typename OtherIntType, int otherFracBits>
    inline constexpr Fixed(const Fixed<OtherIntType, otherFracBits> &other)
        : value{(otherFracBits > fracBits) ? (other.value.separated.integer << fracBits) | (other.value.separated.frac >> (otherFracBits - fracBits))
                                           : (other.value.separated.integer << fracBits) | (other.value.separated.frac << (fracBits - otherFracBits))} {
    }

    /**
     * @brief convert to IntegerType
     * will round up to the nearest integer
     * @tparam Integer IntegerType
     * @tparam std::enable_if<std::is_integral<Integer>::value>::type
     * @return constexpr Integer converted value
     */
    template <typename Integer, typename std::enable_if<std::is_integral<Integer>::value>::type * = nullptr>
    inline constexpr Integer as(void) const { return (Integer)(round().value.separated.integer); }

    /**
     * @brief convert to FloatingPointType
     *
     * @tparam Floating FloatingPointType
     * @tparam std::enable_if<std::is_floating_point<Floating>::value>::type
     * @return constexpr Floating converted value
     */
    template <typename Floating, typename std::enable_if<std::is_floating_point<Floating>::value>::type * = nullptr>
    inline constexpr Floating as(void) const { return (Floating)value.combined / (Floating)((IntType)1 << fracBits); }

    /**
     * @brief implicit conversion for arithmetic types
     *
     * @tparam Number
     * @tparam std::enable_if<std::is_arithmetic<Number>::value>::type
     * @return Number
     */
    template <typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type * = nullptr>
    inline explicit constexpr operator Number() const { return as<Number>(); }

    /**
     * @brief operator+
     *
     * @param other
     * @return constexpr Fixed<IntType, fracBits>
     */
    inline constexpr Fixed<IntType, fracBits> operator+(const Fixed<IntType, fracBits> &other) const { return {{this->value.combined + other.value.combined}}; }

    /**
     * @brief operator-
     *
     * @param other
     * @return constexpr Fixed<IntType, fracBits>
     */
    inline constexpr Fixed<IntType, fracBits> operator-(const Fixed<IntType, fracBits> &other) const { return {{this->value.combined - other.value.combined}}; }

    /**
     * @brief  operator*
     *
     * @param other
     * @return constexpr Fixed<LargerInteger_t<IntType>, fracBits * 2>
     */
    template <typename std::enable_if<allowTypeElevation>::type * = nullptr>
    inline constexpr Fixed<LargerInteger_t<IntType>, fracBits * 2> operator*(const Fixed<IntType, fracBits> &other) const { return {{(LargerInteger_t<IntType>)this->value.combined * (LargerInteger_t<IntType>)other.value.combined}}; }

    template <typename std::enable_if<!allowTypeElevation>::type * = nullptr>
    inline constexpr Fixed<LargerInteger_t<IntType>, fracBits * 2> operator*(const Fixed<IntType, fracBits> &other) const {
    }

    /**
     * @brief operator/
     *
     * @param other
     * @return constexpr Fixed<IntType, fracBits>
     */
    inline constexpr Fixed<IntType, fracBits> operator/(const Fixed<IntType, fracBits> &other) const { return {{((LargerInteger_t<IntType>)this->value.combined << fracBits) / ((LargerInteger_t<IntType>)other.value.combined)}}; }

    /**
     * @brief operator+=
     *
     * @param other
     * @return Fixed<IntType, fracBits>& reference to this.
     */
    inline Fixed<IntType, fracBits> &operator+=(const Fixed<IntType, fracBits> &other) {
        this->value.combined += other.value.combined;
        return *this;
    }

    /**
     * @brief operator-=
     *
     * @param other
     * @return Fixed<IntType, fracBits>& reference to this.
     */
    inline Fixed<IntType, fracBits> &operator-=(const Fixed<IntType, fracBits> &other) {
        this->value.combined -= other.value.combined;
        return *this;
    }

    /**
     * @brief
     *
     * @param other operator*=
     * @return Fixed<LargerInteger_t<IntType>, fracBits * 2>&
     */
    inline Fixed<LargerInteger_t<IntType>, fracBits * 2> operator*=(const Fixed<IntType, fracBits> &other) {
        Fixed<LargerInteger_t<IntType>, fracBits * 2> result = operator*(other);
        *this = result;
        return result;
    }

    /**
     * @brief operator/=
     *
     * @param other
     * @return Fixed<IntType, fracBits>&
     */
    inline Fixed<IntType, fracBits> &operator/=(const Fixed<IntType, fracBits> &other) {
        this->value.combined = ((LargerInteger_t<IntType>)this->value.combined << fracBits) / ((LargerInteger_t<IntType>)other.value.combined);
        return *this;
    }

    /**
     * @brief operator>
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator>(const Fixed &other) const { return this->value.combined > other.value.combined; }

    /**
     * @brief operator<
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator<(const Fixed &other) const { return this->value.combined < other.value.combined; }

    /**
     * @brief operator>=
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator>=(const Fixed &other) const { return this->value.combined >= other.value.combined; }

    /**
     * @brief operator<=
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator<=(const Fixed &other) const { return this->value.combined <= other.value.combined; }

    /**
     * @brief operator==
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator==(const Fixed &other) const { return this->value.combined == other.value.combined; }

    /**
     * @brief operator!=
     *
     * @param other
     * @return true
     * @return false
     */
    inline constexpr bool operator!=(const Fixed &other) const { return this->value.combined != other.value.combined; }

    /**
     * @brief round down
     *
     * @return Fixed
     */
    inline constexpr Fixed floor(void) const {
        Fixed ret;
        ret.value.separated.integer = this->value.separated.integer;
        ret.value.separated.frac = 0;
        return ret;
    }

    /**
     * @brief round up
     *
     * @return Fixed
     */
    inline constexpr Fixed ceil(void) const {
        Fixed ret;
        ret.value.separated.integer = this->value.separated.integer + 1;
        ret.value.separated.frac = 0;
        return ret;
    }

    /**
     * @brief round
     *
     * @return Fixed
     */
    inline constexpr Fixed round(void) const {
        if (this->value.separated.frac >= midFrac)
            return ceil();
        else
            return floor();
    }
};

} // namespace ExFixedPoint

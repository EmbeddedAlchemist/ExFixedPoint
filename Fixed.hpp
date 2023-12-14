#pragma once

#include "TypeConvert.hpp"
#include <cstddef>
#include <type_traits>

namespace ExFixedPoint {

template <typename IntType, std::size_t fracBits>
class Fixed {

  protected:
    struct Value {
        std::make_unsigned_t<IntType> frac : fracBits;
        IntType integer : sizeof(IntType) * 8 - fracBits;
    };

    union Combined {
        IntType combined;
        Value separated;
    };

    static constexpr std::make_unsigned_t<IntType> midFrac = 1 << (fracBits - 1);

    Combined value;

  public:
    /**
     * @brief Constructor with zero initialized;
     *
     */
    Fixed(void) = default;

    /**
     * @brief COnstruct from IntegerType
     *
     * @tparam Integer IntegerType
     * @tparam std::enable_if<std::is_integral<Integer>::value>::type
     */

    template <typename Integer, typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
    constexpr Fixed(Integer integer);

    /**
     * @brief Construct from FloatingType
     *
     * @tparam Floating FloatingType
     * @tparam std::enable_if<std::is_floating_point<Floating>::value>::type
     */
    template <typename Floating, typename = typename std::enable_if<std::is_floating_point<Floating>::value>::type>
    constexpr Fixed(Floating floating);

    /**
     * @brief Construct from other type
     * 
     * @tparam OtherIntType 
     * @tparam otherFracBits 
     */
    template <typename OtherIntType, std::size_t otherFracBits>
    constexpr Fixed(const Fixed<OtherIntType, otherFracBits> &other);

    /**
     * @brief Explicit conversion to integer
     *
     * @tparam Integer IntegerType
     * @tparam std::enable_if<std::is_integral<Integer>::value>::type
     * @return constexpr Integer
     */
    template <typename Integer, typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
    constexpr Integer as(void) const;
    /**
     * @brief Explicit conversion to floating
     *
     * @tparam Floating FloatingType
     * @tparam std::enable_if<std::is_floating_point<Floating>::value>::type
     * @return constexpr Floating
     */
    template <typename Floating, typename = typename std::enable_if<std::is_floating_point<Floating>::value>::type>
    constexpr Floating as(void) const;

    /**
     * @brief Implicit conversion to integer
     *
     * @tparam Integer
     * @tparam std::enable_if<std::is_integral<Integer>::value>::type
     */
    template <typename Integer, typename = typename std::enable_if<std::is_integral<Integer>::value>::type>
    operator Integer(void) const;

    /**
     * @brief Construct a new operator Floating object
     *
     * @tparam Floating
     * @tparam std::enable_if<std::is_floating_point<Floating>::value>::type
     */
    template <typename Floating, typename = typename std::enable_if<std::is_floating_point<Floating>::value>::type>
    operator Floating(void) const;

    constexpr Fixed<IntType, fracBits> operator+(const Fixed<IntType, fracBits> &other) const;
    constexpr Fixed<IntType, fracBits> operator-(const Fixed<IntType, fracBits> &other) const;
    constexpr Fixed<LargerInteger_t<IntType>, fracBits * 2> operator*(const Fixed<IntType, fracBits> &other) const;
    constexpr Fixed<IntType, fracBits> operator/(const Fixed<IntType, fracBits> &other) const;

    Fixed<IntType, fracBits> &operator+=(const Fixed<IntType, fracBits> &other);
    Fixed<IntType, fracBits> &operator-=(const Fixed<IntType, fracBits> &other);
    Fixed<LargerInteger_t<IntType>, fracBits * 2> &operator*=(const Fixed<IntType, fracBits> &other);
    Fixed<IntType, fracBits> &operator/=(const Fixed<IntType, fracBits> &other);

    bool operator>(const Fixed<IntType, fracBits> &other) const;
    bool operator<(const Fixed<IntType, fracBits> &other) const;
    bool operator>=(const Fixed<IntType, fracBits> &other) const;
    bool operator<=(const Fixed<IntType, fracBits> &other) const;
    bool operator==(const Fixed<IntType, fracBits> &other) const;
    bool operator!=(const Fixed<IntType, fracBits> &other) const;

    Fixed<IntType, fracBits> floor(void) const;
    Fixed<IntType, fracBits> ceil(void) const;
    Fixed<IntType, fracBits> round(void) const;
};

} // namespace ExFixedPoint

#include "Fixed.ipp"
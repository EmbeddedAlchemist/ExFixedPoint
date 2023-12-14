#pragma once

#include "Fixed.hpp"

namespace ExFixedPoint {
template <typename IntType, std::size_t fracBits>
template <typename OtherIntType, std::size_t otherFracBits>
inline constexpr Fixed<IntType, fracBits>::Fixed(const Fixed<OtherIntType, otherFracBits> &other) {
    this->value.separated.integer = other.value.separated.integer;
    if (otherFracBits >= fracBits)
        this->value.separated.frac = other.value.separated.frav >> (otherFracBits - fracBits);
    else
        this->value.separated.frac = other.value.separated.frav >> (fracBits - otherFracBits);
}

template <typename IntType, std::size_t fracBits>
template <typename Integer, typename>
inline constexpr Fixed<IntType, fracBits>::Fixed(Integer integer) {
    this->value.separated.integer = integer;
    this->value.separated.frac = 0;
}

template <typename IntType, std::size_t fracBits>
inline constexpr Fixed<IntType, fracBits> Fixed<IntType, fracBits>::operator+(const Fixed<IntType, fracBits> &other) const {
    Fixed<IntType, fracBits> result;
    result.value.combined = this->value.combined + other.value.combined;
    return result;
}

template <typename IntType, std::size_t fracBits>
inline constexpr Fixed<IntType, fracBits> Fixed<IntType, fracBits>::operator-(const Fixed<IntType, fracBits> &other) const {
    Fixed<IntType, fracBits> result;
    result.value.combined = this->value.combined - other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline constexpr Fixed<LargerInteger_t<IntType>, fracBits * 2> Fixed<IntType, fracBits>::operator*(const Fixed<IntType, fracBits> &other) const {
    Fixed<LargerInteger_t<IntType>, fracBits * 2> result;
    result.value.combined = (LargerInteger_t<IntType>)this->value.combined * (LargerInteger_t<IntType>)other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline constexpr Fixed<IntType, fracBits> Fixed<IntType, fracBits>::operator/(const Fixed<IntType, fracBits> &other) const {
    Fixed<IntType, fracBits> ret;
    ret.value.combined = ((LargerInteger_t<IntType>)this->value.combined << fracBits) / ((LargerInteger_t<IntType>)other.value.combined << fracBits);
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> &Fixed<IntType, fracBits>::operator+=(const Fixed<IntType, fracBits> &other) {
    this->value.combined += other.value.combined;
    return *this;
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> &Fixed<IntType, fracBits>::operator-=(const Fixed<IntType, fracBits> &other) {
    this->value.combined -= other.value.combined;
    return *this;
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> &Fixed<IntType, fracBits>::operator/=(const Fixed<IntType, fracBits> &other) {
    this->value.combined = ((LargerInteger_t<IntType>)this->value.combined << fracBits) / ((LargerInteger_t<IntType>)other.value.combined << fracBits);
    return *this;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator>(const Fixed<IntType, fracBits> &other) const {
    return this->value.combined > other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator<(const Fixed<IntType, fracBits> &other) const {
    return this->value.combined < other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator>=(const Fixed<IntType, fracBits> &other) const {
    return this->value.combined >= other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator<=(const Fixed<IntType, fracBits> &other) const {
    return this->value.combined <= other.value.combined;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator==(const Fixed<IntType, fracBits> &other) const {
    return return this->value.combined == other.value.combined;
    ;
}

template <typename IntType, std::size_t fracBits>
inline bool Fixed<IntType, fracBits>::operator!=(const Fixed<IntType, fracBits> &other) const {
    return return this->value.combined != other.value.combined;
    ;
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> Fixed<IntType, fracBits>::floor(void) const {
    Fixed<IntType, fracBits> ret;
    ret.value.separated.integer = this->value.separated.integer;
    ret.value.separated.frac = 0;
    return ret;
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> Fixed<IntType, fracBits>::ceil(void) const {
    Fixed<IntType, fracBits> ret;
    ret.value.separated.integer = this->value.separated.integer + 1;
    ret.value.separated.frac = 0;
    return ret;
}

template <typename IntType, std::size_t fracBits>
inline Fixed<IntType, fracBits> Fixed<IntType, fracBits>::round(void) const {
    if (this->value.separated.frac >= midFrac)
        return ceil();
    else
        return floor();
}

} // namespace ExFixedPoint
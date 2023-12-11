#pragma once

#include <type_traits>

namespace ExFixedPoint {
template <typename IntType, typename FracType, typename FullType>
class FixedSym {
    static_assert(std::is_integral<IntType>::value, "IntType must be integral");
    static_assert(std::is_integral<FracType>::value, "FracType must be integral");
    static_assert(std::is_integral<FullType>::value, "FullType must be integral");
    static_assert(std::is_unsigned<FracType>::value, "FracType must be unsigned");
    static_assert(sizeof(IntType) == sizeof(FracType), "IntType and FracType must have same size");
    static_assert(sizeof(IntType) + sizeof(FracType) == sizeof(FullType), "FullType size must be the sum of IntType and FracType");
    static_assert(std::is_signed<IntType>::value == std::is_signed<FullType>::value, "IntType and FullType must both are signed or unsigned");

    struct FullStruct {
        FracType farc;
        IntType integer;
    };

    union CombineType {
        FullStruct separated;
        FullType combined;
    };

    CombineType data;

  public:
    constexpr FixedSym(IntType integer);

    constexpr FixedSym operator+(const FixedSym &other);
    constexpr FixedSym operator-(const FixedSym &other);
    constexpr FixedSym operator*(const FixedSym &other);
    constexpr FixedSym operator/(const FixedSym &pther);

    FixedSym &operator+=(const FixedSym &other);
    FixedSym &operator-=(const FixedSym &other);
    FixedSym &operator*=(const FixedSym &other);
    FixedSym &operator/=(const FixedSym &other);

    bool operator>(const FixedSym &other);
    bool operator<(const FixedSym &other);
    bool operator>=(const FixedSym &other);
    bool operator<=(const FixedSym &other);
    bool operator==(const FixedSym &other);
    bool operator!=(const FixedSym &other);
};
} // namespace ExFixedPoint

#include "FixedSym.ipp"
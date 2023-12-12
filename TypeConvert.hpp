#pragma once

#include <cstdint>
#include <type_traits>

namespace ExFixedPoint {

// template <typename Integer>
// struct LargerInteger {
//     static_assert(std::is_integral<Integer>::value, "Integer must be an integral type");

//     using type = typename std::conditional<std::is_signed<Integer>::value, typename std::conditional<sizeof(Integer) == 1, std::int16_t, typename std::conditional<sizeof(Integer) == 2, std::int32_t, typename std::conditional<sizeof(Integer) == 4, std::int64_t, void>::type>::type>::type,
//                                            typename std::conditional<sizeof(Integer) == 1, std::uint16_t, typename std::conditional<sizeof(Integer) == 2, std::uint32_t, typename std::conditional<sizeof(Integer) == 4, std::uint64_t, void>::type>::type>::type>::type;
// };

template <typename Integer>
struct LargerInteger {
    using type =
        std::conditional_t<
        std::is_signed<Integer>::value,
        std::conditional_t<sizeof(Integer) == 1, std::int16_t,
        std::conditional_t<sizeof(Integer) == 2, std::int32_t,
        std::conditional_t<sizeof(Integer) == 4, std::int64_t,
        void>>>,
        std::conditional_t<sizeof(Integer) == 1, std::uint16_t,
        std::conditional_t<sizeof(Integer) == 2, std::uint32_t,
        std::conditional_t<sizeof(Integer) == 4, std::uint64_t,
        void>>>>;
};


template<typename Integer>
using LargerInteger_t = typename LargerInteger<Integer>::type;

template <typename Integer>
struct SmallerInteger {
    using type =
        std::conditional_t<
        std::is_signed<Integer>::value,
        std::conditional_t<sizeof(Integer) == 1, void,
        std::conditional_t<sizeof(Integer) == 2, std::int8_t,
        std::conditional_t<sizeof(Integer) == 4, std::int16_t,
        std::int32_t>>>,
        std::conditional_t<sizeof(Integer) == 1, void,
        std::conditional_t<sizeof(Integer) == 2, std::uint8_t,
        std::conditional_t<sizeof(Integer) == 4, std::uint16_t,
        std::uint32_t>>>>;
};
template<typename Integer>
using SmallerInteger_t = typename SmallerInteger<Integer>::type;


} // namespace ExFixedPoint

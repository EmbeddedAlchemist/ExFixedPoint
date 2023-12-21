#pragma once

#include <cstdint>
#include <type_traits>

namespace ExFixedPoint {

template <typename Integer>
struct LargerInteger {
    using type =
        typename std::conditional<
            std::is_signed<Integer>::value,
                typename std::conditional<sizeof(Integer) == 1, std::int16_t,
                typename std::conditional<sizeof(Integer) == 2, std::int32_t,
                typename std::conditional<sizeof(Integer) == 4, std::int64_t,
#if defined(__SIZEOF_INT128__)
                typename std::conditional<sizeof(Integer) == 8, __int128>,
#endif
                void
#if defined(__SIZEOF_INT128__)
                >::type
#endif

            >::type>::type>::type,
                typename std::conditional < sizeof(Integer) == 1, std::uint16_t,
                typename std::conditional < sizeof(Integer) == 2, std::uint32_t,
                typename std::conditional < sizeof(Integer) == 4, std::uint64_t,
#if defined(__SIZEOF_UINT128__)
                typename std::conditional<sizeof(Integer) == 8, __uint128>,
#endif
                void
#if defined(__SIZEOF_UINT128__)
                >::type
#endif
            >::type>::type>::type
        >::type;
};

/**
 * @brief convert Integer to larger IntegerType
 *
 * @tparam Integer
 */
template <typename Integer>
using LargerInteger_t = typename LargerInteger<Integer>::type;

template <typename Integer>
struct SmallerInteger {
    using type =
        typename std::conditional<
            std::is_signed<Integer>::value,
            typename std::conditional<sizeof(Integer) == 1, void,
            typename std::conditional<sizeof(Integer) == 2, std::int8_t,
            typename std::conditional<sizeof(Integer) == 4, std::int16_t,
            typename std::conditional<sizeof(Integer) == 8, std::int32_t,
                                                     std::int64_t>::type>::type>::type>::type,
            typename std::conditional<sizeof(Integer) == 1, void,
            typename std::conditional<sizeof(Integer) == 2, std::uint8_t,
            typename std::conditional<sizeof(Integer) == 4, std::uint16_t,
            typename std::conditional<sizeof(Integer) == 8, std::uint32_t,
                                                     std::uint64_t>::type>::type>::type >::type>::type;
};

/**
 * @brief convert integer to smaller integer type;
 *
 * @tparam Integer
 */
template <typename Integer>
using SmallerInteger_t = typename SmallerInteger<Integer>::type;

} // namespace ExFixedPoint

#pragma once

#include "Fixed/Fixed.hpp"
#include <cstdint>

namespace ExFixedPoint {
template <typename BaseType, unsigned int fracBits>
using FastFixed = Fixed<BaseType, fracBits, false>;

template <unsigned int fracBits>
using Fixed8 = Fixed<std::int8_t, fracBits>;

template <unsigned int fracBits>
using Fixed16 = Fixed<std::int16_t, fracBits>;

template <unsigned int fracBits>
using Fixed32 = Fixed<std::int32_t, fracBits>;

template <unsigned int fracBits>
using FastFixed8 = FastFixed<std::int8_t, fracBits>;

template <unsigned int fracBits>
using FastFixed16 = FastFixed<std::int16_t, fracBits>;

template <unsigned int fracBits>
using FastFixed32 = FastFixed<std::int32_t, fracBits>;

template <unsigned int fracBits>
using FastFixed64 = FastFixed<std::int64_t, fracBits>;

using Fixed_8_8 = Fixed16<8>;
using Fixed_8_24 = Fixed32<24>;
using Fixed_16_16 = Fixed32<16>;
using Fixed_24_8 = Fixed32<8>;

using FastFixed_8_8 = FastFixed16<8>;
using FastFixed_8_24 = FastFixed32<24>;
using FastFixed_16_16 = FastFixed32<16>;
using FastFixed_24_8 = FastFixed32<8>;
using FastFixed_8_56 = FastFixed64<56>;
using FastFixed_16_48 = FastFixed64<48>;
using FastFixed_24_40 = FastFixed64<40>;
using FastFixed_32_32 = FastFixed64<32>;
using FastFixed_40_24 = FastFixed64<24>;
using FastFixed_48_16 = FastFixed64<16>;
using FastFixed_56_8 = FastFixed64<8>;

template <unsigned int fracBits>
using Fixed8u = Fixed<std::uint8_t, fracBits>;

template <unsigned int fracBits>
using Fixed16u = Fixed<std::uint16_t, fracBits>;

template <unsigned int fracBits>
using Fixed32u = Fixed<std::uint32_t, fracBits>;

template <unsigned int fracBits>
using FastFixed8u = FastFixed<std::uint8_t, fracBits>;

template <unsigned int fracBits>
using FastFixed16u = FastFixed<std::uint16_t, fracBits>;

template <unsigned int fracBits>
using FastFixed32u = FastFixed<std::uint32_t, fracBits>;

template <unsigned int fracBits>
using FastFixed64u = FastFixed<std::uint64_t, fracBits>;

using Fixed_8_8u = Fixed16u<8>;
using Fixed_8_24u = Fixed32u<24>;
using Fixed_16_16u = Fixed32u<16>;
using Fixed_24_8u = Fixed32u<8>;

using FastFixed_8_8u = FastFixed16u<8>;
using FastFixed_8_24u = FastFixed32u<24>;
using FastFixed_16_16u = FastFixed32u<16>;
using FastFixed_24_8u = FastFixed32u<8>;
using FastFixed_8_56u = FastFixed64<56>;
using FastFixed_16_48u = FastFixed64u<48>;
using FastFixed_24_40u = FastFixed64u<40>;
using FastFixed_32_32u = FastFixed64u<32>;
using FastFixed_40_24u = FastFixed64u<24>;
using FastFixed_48_16u = FastFixed64u<16>;
using FastFixed_56_8u = FastFixed64u<8>;


#if defined(__SIZEOF_INT128__)

template <unsigned int fracBits>
using Fixed64 = Fixed<std::int64_t, fracBits>;

template <unsigned int fracBits>
using FastFixed128 = FastFixed<__int128_t, fracBits>;

#endif

#if defined(__SIZEOF_UINT128__)

template <unsigned int fracBits>
using Fixed64u = Fixed<std::uint64_t, fracBits>;

template <unsigned int fracBits>
using FastFixed128u = FastFixed<__uint128_t, fracBits>;

#endif



} // namespace ExFixedPoint

#pragma once

#include <cstdint>
#include "Fixed.hpp"

namespace ExFixedPoint {

template <std::size_t fracBits>
using Fixed8 = Fixed<std::int8_t, fracBits>;

template <std::size_t fracBits>
using Fixed16 = Fixed<std::int16_t, fracBits>;

template <std::size_t fracBits>
using Fixed32 = Fixed<std::int32_t, fracBits>;

template <std::size_t fracBits>
using Fixed64 = Fixed<std::int64_t, fracBits>;


} // namespace ExFixedPoint

# ExFixedPoint
[中文版本](./README_CN.md)

ExFixedPoint is a C++ Fixed Point Arithmetic Library. Supported C++11 or higher.



## To Do

- [x] Four Operations
- [ ] More math function in `math.h`



## Benchmark

It can be seen that the arithmetic overhead of fixed-point numbers is similar to that of integers

STM32F103, 72 MHz, AC6, -Ofast, C++11

| Operate (100,000 times) \\ type |   double   |   float    | FastFixed32 | Fixed16  |   Fixed32    | int32_t  |
| :-----------------------------: | :--------: | :--------: | :---------: | :------: | :----------: | :------: |
|       type = type + type;       | 1534.96 ms | 1190.26 ms |  10.62 ms   | 10.63 ms |   10.62 ms   | 10.62 ms |
|       type = type - type;       | 1536.18 ms | 1270.24 ms |  10.89 ms   | 10.75 ms |   10.76 ms   | 10.62 ms |
|      type = type \* type;       | 1352.03 ms | 1202.88 ms |  22.01 ms   | 29.03 ms | 72.55 ms \*  | 10.76 ms |
|       type = type / type;       | 1191.88 ms | 1136.28 ms |  36.77 ms   | 53.53 ms | 455.30 ms \* | 20.76 ms |

\* Fixed32 types allow type elevation during multiplication and division operations, which reduces the loss of precision due to the operation, but can lead to performance degradation, especially if the machine does not support 64-bit integer operations.

## Quick Start!

### Setup

1. To use ExFixedPoint, you may add the directory containing` ExFixedPoint.hpp` to the Include Path.
2. Then, include `ExFixedPoint.hpp` in your source file. ExFixedPoint is fully implemented by the template, no source file need to add.
3. Fixed-point class `Fixed` are contained in namespace `ExFixedPoint`
4. You can also use the preset types provided in `ExFixedPoint.hpp`.

### Construct a Fixed-Point

Class `Fixed` has 3 template parameters:

- ` typename BaseType` Specify an integer type for storing the entire fixed-point number, fixed-point number will share same sign as the integer type
  - Must be a integer type
- `unsigned int fracBits` Specifies the number of fractional digits of the fixed point. 
  - This value must be greater than 0 and less than the number of digits of the `BaseType`
  - integers and fractions must be preserved to at least one place.  
- `bool allowTypeElevation` This value controls whether or not the result is allowed to be lifted to a larger integer type during multiplication and division operations, which can improve the precision of the operation, but can degrade performance.
  - Default is true

An error in this value will cause the static_assert in the class to fail.

### Use Preset Type

For ease of use, ExFixedPoint offers a number of preset types

|        Type         |   `BaseType`    | `fracBits` | `allowTypeElevation` |
| :-----------------: | :-------------: | :--------: | :------------------: |
| `Fixed`(class self) |        -        |     -      |    true (Default)    |
|     `FastFixed`     |        -        |     -      |        false         |
|      `Fixed8`       |  `std::int8_t`  |     -      |         true         |
|      `Fixed16`      | `std::int16_t`  |     -      |         true         |
|      `Fixed32`      | `std::int32_t`  |     -      |         true         |
|    `Fixed64` \*     |  `std::int8_t`  |     -      |         true         |
|      `Fixed8u`      | `std::uint8_t`  |     -      |         true         |
|     `Fixed16u`      | `std::uint16_t` |     -      |         true         |
|     `Fixed32u`      | `std::uint32_t` |     -      |         true         |
|    `Fixed64u` \*    | `std::uint8_t`  |     -      |         true         |
|    `FastFixed8`     |  `std::int8_t`  |     -      |        false         |
|    `FastFixed16`    | `std::int16_t`  |     -      |        false         |
|    `FastFixed32`    | `std::int32_t`  |     -      |        false         |
|    `FastFixed64`    | `std::int64_t`  |     -      |        false         |
|  `FastFixed128` \*  |  `__int128_t`   |     -      |        false         |
|    `FastFixed8u`    | `std::uint8_t`  |     -      |        false         |
|   `FastFixed16u`    | `std::uint16_t` |     -      |        false         |
|   `FastFixed32u`    | `std::uint32_t` |     -      |        false         |
|   `FastFixed64u`    | `std::uint64_t` |     -      |        false         |
| `FastFixed128u` \*  |  `__uint128_t`  |     -      |        false         |
|     `Fixed_8_8`     | `std::int16_t`  |     8      |         true         |
|    `Fixed_8_24`     | `std::int32_t`  |     24     |         true         |
|    `Fixed_16_16`    | `std::int32_t`  |     16     |         true         |
|    `Fixed_24_8`     | `std::int32_t`  |     8      |         true         |
|    `Fixed_8_8u`     | `std::uint16_t` |     8      |         true         |
|    `Fixed_8_24u`    | `std::uint32_t` |     24     |         true         |
|   `Fixed_16_16u`    | `std::uint32_t` |     16     |         true         |
|    `Fixed_24_8u`    | `std::uint32_t` |     8      |         true         |
|   `FastFixed_8_8`   | `std::int16_t`  |     8      |        false         |
|  `FastFixed_8_24`   | `std::int32_t`  |     24     |        false         |
|  `FastFixed_16_16`  | `std::int32_t`  |     16     |        false         |
|  `FastFixed_8_56`   | `std::int64_t`  |     56     |        false         |
|  `FastFixed_16_48`  | `std::int64_t`  |     48     |        false         |
|  `FastFixed_24_40`  | `std::int64_t`  |     40     |        false         |
|  `FastFixed_32_32`  | `std::int64_t`  |     32     |        false         |
|  `FastFixed_40_24`  | `std::int64_t`  |     24     |        false         |
|  `FastFixed_48_16`  | `std::int64_t`  |     16     |        false         |
|  `FastFixed_56_8`   | `std::int64_t`  |     8      |        false         |
|  `FastFixed_8_8u`   | `std::uint16_t` |     8      |        false         |
|  `FastFixed_8_24u`  | `std::uint32_t` |     24     |        false         |
| `FastFixed_16_16u`  | `std::uint32_t` |     16     |        false         |
|  `FastFixed_8_56u`  | `std::uint64_t` |     56     |        false         |
| `FastFixed_16_48u`  | `std::uint64_t` |     48     |        false         |
| `FastFixed_24_40u`  | `std::uint64_t` |     40     |        false         |
| `FastFixed_32_32u`  | `std::uint64_t` |     32     |        false         |
| `FastFixed_40_24u`  | `std::uint64_t` |     24     |        false         |
| `FastFixed_48_16u`  | `std::uint64_t` |     16     |        false         |
|  `FastFixed_56_8u`  | `std::uint64_t` |     8      |        false         |

`-` : Need to specify in template parameter.

`*` : Only available in machine that support `__int128_t`and`__uint128_t`

### Mathematical Operation

ExFixedPoint allow mathematical operation just like native arithmetic type, you can use operator `+` `-` `*` `/` `+=` `-=` `*=` `/=` between same type fixed-point, different type fixed-point or  native arithmetic type.

Example:

~~~Cpp
Fixed_8_8 pi = 3.14159;                 //Construct from floating-point
FastFixed_8_8 r = 5;                    //Construct from integer
auto /*Fixed_8_8*/ two_pi = pi * 2;     //operating with integer
auto /*Fixed_16_16*/ area = two_pi * r; //operating with other fixed-point, type elevation
~~~




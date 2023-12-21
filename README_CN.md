# ExFixedPoint
[English Edition](./README.md)

ExFixedPoint 是一个 C++ 定点计算库，支持C++11或更高的版本。



## 计划清单

- [x] 四则运算
- [ ] `math.h`中的数学函数



## 性能测试

定点数的运算开销与整数相近

STM32F103, 72 MHz, AC6, -Ofast, C++11

| 100,000次运算 \\ 类型 |   double   |   float    | FastFixed32 | Fixed16  |   Fixed32    | int32_t  |
| :-------------------: | :--------: | :--------: | :---------: | :------: | :----------: | :------: |
|  type = type + type;  | 1534.96 ms | 1190.26 ms |  10.62 ms   | 10.63 ms |   10.62 ms   | 10.62 ms |
|  type = type - type;  | 1536.18 ms | 1270.24 ms |  10.89 ms   | 10.75 ms |   10.76 ms   | 10.62 ms |
| type = type \* type;  | 1352.03 ms | 1202.88 ms |  22.01 ms   | 29.03 ms | 72.55 ms \*  | 10.76 ms |
|  type = type / type;  | 1191.88 ms | 1136.28 ms |  36.77 ms   | 53.53 ms | 455.30 ms \* | 20.76 ms |

\* Fixed32 允许在乘除运算时的类型提升， 可以避免损失精度，但会增大性能开销，特别是在不支持64位运算的机器上。

## 原神，启动！

### 使用前说明

1. 将包含` ExFixedPoint.hpp` 的目录添加到 Include Path 中
2. 在源文件中引入 `ExFixedPoint.hpp` 。 ExFixedPoint 完全由模板实现，不需要添加源文件。
3. 定点数类 `Fixed` 包含在命名空间 `ExFixedPoint`中。
4. 你也可以直接使用 `ExFixedPoint.hpp`提供的预设类型.

### 构造定点数

类 `Fixed` 有3个模板参数:

- ` typename BaseType` 指定一个整数类型用于存储定点数。定点数的符号性和这个整数类型相同。
  - 必须是一个整数类型。
- `unsigned int fracBits` 指定定点数中小数所占的位数. 
  - 必须大于0，小于 `BaseType`的位数
  - 整数和小数部分均至少占一位.  
- `bool allowTypeElevation` 该值控制是否允许在乘除运算的时候进行类型提升。可以提升精度，但会降低性能。
  - 默认为 true

若上述参数有误，类中的static_assert会大声哭闹来提示你的。

可以直接从原生数字类型构造定点数，或显示调用构造方法或From方法来构造。

~~~cpp
Fixed<std::int32_t, 16. true> a = 3.14; //从原生数字类型隐式构造
Fixed<std::int32_t, 16. true> b(3.14); //显式构造
auto b = Fixed<std::int32_t, 16. true>::from(3.14); //from方法
~~~



### 预设类型

为方便使用， ExFixedPoint 提供了一些预设类型

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

`-` : 需要手动在模板参数中指定。

`*` : 仅在支持 `__int128_t`、`__uint128_t`的机器上生效。

### 数学运算

ExFixedPoint 可以像操作原生数字类型一样操作定点数, 可以在相同或不同类型的定点数和原生数字类型之间使用 `+` `-` `*` `/` `+=` `-=` `*=` `/=` 运算符.

允许隐式从原生数字类型转为定点数，但不允许隐式从定点数转为原生数字类型。 你可以使用`as`方法或显式类型转换来转换为原生类型。

~~~Cpp
Fixed_8_8 pi = 3.14159;                 //Construct from floating-point
FastFixed_8_8 r = 5;                    //Construct from integer
auto /*Fixed_8_8*/ two_pi = pi * 2;     //operating with integer
auto /*Fixed_16_16*/ area = two_pi * r; //operating with other fixed-point, type elevation
~~~




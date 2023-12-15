# FL-M32处理器软件运行环境
## 功能简介
FL-M32_runtime是FL-M32 SOC的运行时环境，提供一些基本C开发环境，用于简化用户开发C程序的难度。

## 文件描述
Runtime_TLS文件结构如下所示：

| 文件夹             | 功能描述                                          |
|-------------------|--------------------------------------------------|
| demo              | c程序示例，如[数独计算](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/demos/sudoku)，[源目的MAC替换](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/demos/singleCore_Replace_MAC_Addr)  |
| Firmware_Runtime  | 核心软件库，提供一些基本C开发环境，用于简化用户开发C程序的难度    |
| tests             | 提供最小的测试程序，如[isa](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/tests/isa)，[coremark](https://github.com/JunnanLi/FL-M32_Runtime/tree/main/Runtime_TLS/tests/coremark)       |

## 使用说明
1) 你可以直接进入demo文件夹，选择其中的示例程序，运行`make`命令即可，会自动在**hex**文件夹中生成二进制文件**firmware.hex**。然后将其拷贝至**FL-M32_Remote_Controller**文件下，运行`./t_send`进行远程加载
2) 你也可以自己编写c程序，并调用**Firmware_Runtime**提供的系统函数，实现报文处理、中断处理、打印等功能。需要注意的是，编写完软件程序后，需要相应的修改**Makefile**，以能够正确链接到Firmware_Runtime的**Makefile.sys**

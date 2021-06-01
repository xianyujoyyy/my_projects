# ch1 计算机系统漫游

编译过程：

预处理阶段。xx.i

编译阶段。xx.s。低级机器语言指令。

汇编阶段。xx.o。机器语言指令，可重定位目标程序。

链接阶段。和库函数合并成可执行文件。

运行hello程序：

键盘输入->寄存器->内存

hello文件中的代码和数据磁盘->主存

处理器执行main程序中的机器语言指令:主存->寄存器->显示设备

cache memory：L0寄存器 L1，L2，L3 SRAM

主存 DRAM

上一层的存储器作为低一层存储器的高速缓存。

文件是对I/O设备的抽象表示，虚拟内存是对主存和磁盘I/O设备的抽象表示，进程则是对处理器、主存和I/O设备的抽象表示。

进程是操作系统对一个正在运行的程序的一种抽象。在一个系统上可以同时运行多个进程，而每个进程都好像在独占地使用硬件。并发运行，则是说一个进程的指令和另一个进程的指令是交错执行的。

操作系统保持跟踪进程运行所需的所有状态信息，即上下文。

线程共享同样的代码和全局数据。

虚拟内存是一个抽象概念，它为每个进程提供了一个假象，即每个进程都在独占地使用主存。虚拟地址空间。

从低到高：程序代码和数据。堆。共享库。栈。内核虚拟内存（用户不可见）。

并发：指一个同时具有多个活动的系统。

并行：用并发来使一个系统运行得更快。

抽象：

（（（I/O设备：文件 + 主存）：虚拟内存 + 处理器：指令集架构）：进程 + 操作系统）：虚拟机

# ch2 信息的表示和处理

字节：8位2进制数，最小的可寻址内存单元。

c指针：值+类型 值表示位置，类型表示那个位置上所存储对象的类型。

十六进制：0～9 A～F 每一位对应4位二进制数。0x，0X开头。

字长：指针数据的标称大小。最重要的系统参数就是虚拟地址空间的最大大小。0～2^w - 1

char 1 short 2 int 4 long 32:4 64:8 char* 32:4 64:8 float 4 double 8

大端：数字高位在内存低位。小端：数字低位在内存低位。

sizeof(T) 确定对象字节数。

c移位操作：右移，算术右移，补最高位，逻辑右移，补0，通常默认为算术右移，无符号数为逻辑补位。移动位数多于数字本身位数时，通常会取mod。>>,<<优先级低于+,-。

C数据类型范围：signed 负值比正值范围大1，unsigned 是 signed正值范围的两倍多1.

补码：最高位为符号位，负数为1，正数为0，-(1/0) * 2^(w-1)， 其他位都为正的权值。

即最小的负数为10...00 最大的正数为01...11,unsigned 无符号位。

可移植性：

<limit.h> INT_MAX,INT_MIN...

<stdint.h> int64_t,uint64_t,INT64_MAX...

printf("x = %" PRId32 ", y = %" PRIu64 "\n", x, y) 打印int32_t, uint64_t

反码：最高有效位权是-(2^(w-1) - 1)而不是-2^(w-1),其他同补码。

原码：最高有效位是符号位，用来确定剩下的位应该取负权还是正权。

强制类型转换的结果是保持位值不变，只是改变了解释这些位的方式。

```c
#include <stdio.h>
typedef unsigned char* byte_pointer;
void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    printf("%.2x", start[i]);
  }
  printf("\n");
}
void show_int(int x) { show_bytes((byte_pointer) &x, sizeof(int)); }
```

如果一个运算数是有符号的，另一个是无符号的，C会隐式的将有符号的参数强制类型转换为无符号数。

扩展一个数字的位表示：无符号数转换为一个更大的数据类型，零扩展。

补码数字，符号扩展。

截断数字：丢弃高位。

注意**unsigned**的减法，signed和unsigned的转换造成的安全漏洞。

无符号数溢出判断：s=x+(y-2^w)<x

补码（signed）溢出判断：x>0,y>0,而s<=0，正溢出。负溢出反之。

补码非的位级表示：-x=~x+1

x*2^w = x<<w 将乘法转换为位移和加法/减法。除法>>，只能用于除以2的幂,+偏置量，正数0，负数7。

**补码和无符号乘法具有相同的位级行为，而且它们是可交换的。**

浮点数：0.111 = 2^-1+2^-2+2^-3

IEEE浮点数：V=(-1)^s*M\*2^E

float:s,exp,frac字段分别为1位，8位，23位。

double:s,exp,frac字段分别为1位，11位，52位。

阶码：2^(k-1) -1

规格化：exp位不全为0，也不全为1，E=e-Bias,M=f+1。靠近0，稠密。

非规格化：exp位全为0，E=1-Bias,M=f。远离0，稀疏。

特殊值：exp全为1，frac全为0，∞。frac不全为0，NaN。

偶数舍入：形如XXXX.YYY100...的二进制位模式，最后一个Y是近似到的位，舍入方式为倾向于使Y为0.

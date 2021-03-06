# 内核开发的特点

* 内核编程时既不能访问C库也不能访问标准的C头文件。
* 内核编程时必须使用GUN C（gcc）。inline、asm()插入汇编、likely()、unlikely()分支选择。
* 内核编程时缺乏用户空间的内存保护机制。内核态可以访问所有资源。内核中的内存不分页。
* 内核编程时难以执行浮点运算。
* 内核给每个进程只有一个很小的定长堆栈。
* 由于内核支持异步中断、抢占和SMP，因此必须时刻注意同步和并发。自旋锁、信号量。
* 要考虑可移植性的重要性。

# 进程

进程是正在执行的程序代码的实时结果。

线程拥有一个独立的程序计数器、进程栈和一组进程寄存器。内核调度的对象是线程，而不是进程。Linux中线程和进程并不特别区分。同一个进程的线程之间共享虚拟内存，但每个都拥有各自的虚拟处理器。

进程描述符task_struct,包含一个具体进程的所有信息。task_struct链表task list。

struct thread_info;位于进程内核栈低内存地址,通过current从thread_info中快速得到task_struct的地址。

进程状态：TASK_RUNNING,可执行或正在执行或等待执行。

TASK_INTERRUPTIBLE,TASK_UNINTERRUPTIBLE,睡眠等待条件达成，信号可唤醒\不可唤醒。

\_\_TASK_TRACED,\_\_TASK_STOPPED.

系统调用，异常处理程序是进程陷入内核的接口，在内核执行时，仍处于进程上下文中，current宏有效。

fork()拷贝当前进程创建一个子进程+exec()读取可执行文件并将其载入地址空间开始运行。copy on write，fork()后立刻执行exec()可避免拷贝。

thread(linux):

参数决定了新创建进程的行为方式和父子进程之间共享资源种类。

clone(CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND, 0);

父子进程共享地址空间，父子进程共享文件系统信息，父子进程共享打开的文件，父子进程共享信号处理函数和被阻断的信号。

普通fork()进程:clone(SIGCHLD, 0);

**内核线程**:内核的后台操作，没有独立的地址空间，mm指针为NULL。可以被调度和抢占。

EXIT_ZOMBIE:仍保留内核栈，thread_info和task_struct（可能有父进程需要的数据）。父进程退出时，会给子进程找新的父进程，可为进程中的其他线程或init进程。否则会变为僵尸进程浪费资源。

# 进程调度

调度程序负责决定将哪个进程投入运行，何时运行以及运行多长时间。有效分配资源。

CFS（完全公平调度算法），针对普通进程的调度类。允许每个进程运行一段时间、循环轮转、选择运行最少vruntime的进程作为下一个运行进程，而不再采用给每个进程时间片的做法，CFS在所有可运行进程总数基础上计算出一个进程应该运行多久，而不是依靠nice值来计算时间片，nice值在CFS中被作为进程获得的处理器运行比的权重。时间片有底线，避免频繁切换进程。

用红黑树组织可运行进程队列，找到vruntime最小值的进程。且红黑树最左侧叶子节点已缓存。没有任何进程时，CFS调度器选择idle任务运行。

进程变为可运行状态或fork()创建进程时插入红黑树，进程阻塞或终止时remove。

调度器入口schedule()，需要和一个具体的调度类关联，找到最高优先级的调度类然后找到该类中的进程。pick_next_task()。

休眠和唤醒：进程把自己标记为休眠状态，从红黑树中移出，放入**等待队列**，然后调用schedule()选择和执行一个其他进程。唤醒将进程设置为可执行状态，然后再从等待队列中移到可执行红黑树中。

用户抢占：task_struct中的need_resched标志被设置，内核返回用户空间时会导致schedule()被调用，此时就会发生用户抢占。用户抢占发生在从系统调用返回用户空间时，从中断处理程序返回用户空间时。

内核抢占：thread_info中引入preempt_count计数器。该计数器初始值为0，每当使用锁的时候数值加1，释放锁的时候数值减1，当数值为0时，可以执行抢占。从中断返回内核空间时，need_resched被设置且preempt_count为0的话，会调度进行抢占。进程阻塞或显式调用schedule()也会抢占。

 # 中断和中断处理

中断值通常被称为中断请求(IRQ)线。异步，通常与特定设备关联。在响应一个特定中断的时候，内核会执行一个函数，中断处理程序(interrupt handler)或中断服务例程(interrupt service routine,ISR)。

异常：同步中断，处理器执行到错误时，靠内核处理。

中断上下文：不可阻塞，不可调度，不可使用会阻塞的函数。

中断处理程序：分为两个部分，top half(上半部，*中断禁止，无须重入，尽可能迅速、简洁*)接受一个中断，立即开始执行，但只做有严格时限的工作。能够允许稍后完成的工作推迟到bottom half(下半部，*开中断*)完成。

在禁止中断之前保存中断系统的状态会更加安全一些，在准备激活中断时，只需把中断恢复到它们原来的状态。

禁止指定中断线：disable_irq,disable_irq_nosync,enable_irq,synchronize_irq.

是否处于中断中：in_interrupt,in_irq.

## bottom half

* 如果一个任务对时间非常敏感，将其放在中断处理程序中执行。
* 如果一个任务和硬件相关，将其放在中断处理程序中执行。
* 如果一个任务要保证不被其他中断打断，将其放在中断处理程序中执行。
* 其他所有任务，考虑放置在下半部执行。

下半部执行的关键在于当它们运行的时候，允许响应所有的中断。

软中断（softirqs）：一组静态定义的下半部接口，有32个，编译期间静态分配。可以在所有处理器上同时执行，即使类型相同也可以。需要锁保护，保留给系统中对时间要求最严格的下半部。中断处理程序可以抢占软中断，但软中断不能休眠。do_softirq()循环32次，执行所有软中断。 System_call：异常128。系统调用对应号放在eax中。

tasklet（tasklet_struct）：是一种基于**软中断**实现的灵活性强，动态创建的下半部实现机制。接口更简单，锁保护要求较低。同一时间，相同的tasklet里只有一个在运行。

执行步骤：检查tasklet的状态是否为TASKLET_STATE_SCHED，若已被调度过，函数立即返回。调用_tasklet_schedule()。保存中断状态，然后禁止本地中断。把需要调度的tasklet加到每个处理器一个的tasklet_vec链表或taklet_hi_vec链表的表头上去。唤起TASKLET_SOFTIRQ或HI_SOFTIRQ软中断，在下次调用do_softirq()时会执行tasklet。在执行tasklet时通过TASKLET_STATE_RUN检查是否有相同tasklet在执行，若有就不执行。

工作队列：内核线程执行event/n，进程上下文。允许重新调度和睡眠。造成的开销最大，牵扯到内核线程甚至是上下文切换。

Cpu_workqueue_struct(工作者线程) workqueue_struct（每个工作者线程有一个)work_struct（每个处理函数有一个)

Ksoftirqd:辅助处理软中断的内核线程。当大量软中断出现时，内核会唤醒一组内核线程来处理，这些线程在低优先级上运行，避免跟其他重要任务抢资源，但最终肯定会被执行。

如果进程上下文和一个下半部共享数据，在访问这些数据之前，需要禁止下半部的处理并得到锁的使用权。为了本地和SMP保护并防止死锁。中断上下文和一个下半部共享数据，在访问数据之前，要禁止中断并得到锁。

C传递结构体为输入参数，方便结构体修改。内核线程mm为NULL不能操作用户空间内存，无映射。

# 内核同步

临界区：访问和操作共享数据的代码段。避免并发和防止竞争条件成为同步。需要保护的数据而非代码。

内核中并发执行的原因：中断/软中断（tasklet）/内核抢占/睡眠及与用户空间的同步/对称多处理

dead lock条件：一个或多个执行线程和一个或多个资源，每个线程都在等待其中的一个资源，但所有的资源都已经被占用，线程循环等待，永远不会释放已经占有的资源。

加锁粒度小，可扩展性更好，但加大开销。尽量简单。

## 原子操作

整数操作、位操作。

atomic_t,volatile

## 自旋锁spin lock

自旋锁最多只能被一个可执行线程持有。如果一个执行线程试图获得一个已经持有的自旋锁，那么该线程就会一直进行busy waiting。

**短时期内进行轻量级加锁**

等待时间小于进程上下文切换，使用自旋锁，而不睡眠。

不可递归，单处理器时不编译。

读锁可以递归。

## 信号量

Linux中信号量是一种睡眠锁。如果一个任务试图获得一个当前不可用的信号量时，信号量会将其推进一个等待队列，然后让其睡眠。

开销大于自旋锁。使用于等待时间比较长的情况，只能用于进程上下文（可睡眠），需要睡眠所以不能与自旋锁同时使用。

区分读-写机制使用是有条件的，只有在可以自然界定读-写时才有价值。

## 互斥体

信号量为1，mutex。

加锁解锁在同一个进程上下文，持有锁时进程不可退出，不可递归。

## 完成变量

内核中一个任务需要发出信号通知另一任务发生了某个特定事件，利用完成变量是使两个任务得到同步的简单方法。

## 顺序锁 seq锁

读写共享数据，实现依靠一个序列计数器。当加锁数据被写入时，序列值会增加，在读取数据之前和之后，序列号都会被读取，如果读取的序列号值相同，说明在读操作中没有被写操作打断过。

适用于：读者多，写者少，但希望写优先于读。

```c++
do {
  seq = read_seqbegin(&lock)
  get_val();
						}while(read_seqretry(&lock, seq))
```

## 禁止抢占

一个数据能被一个处理器访问，没有持有自旋锁，内核又是抢占式的，那么一个新调度的任务就可能访问同一个变量。

Preempt_disable()

Preempt_enable()

## 顺序和屏障

rmb()读屏障，之前的载入调用不会排到之后，之后的载入调用不会排到之前。

wmb()写屏障，存储。

mb()读写屏障。

# 内存管理（内核管理物理内存）

MMU:管理内存并把虚拟地址转换为物理地址的硬件，以页为单位（4KB）。struct page;

page与物理页相关，仅用于描述当前时刻相关物理页中存放的东西。描述物理内存本身。

由于硬件的限制，将页划分为不同的区，一些体系结构的内存的物理寻址范围比虚拟寻址范围大得多，就有一些内存不能永久的映射到内核空间上，ZONE_DMA,ZONE_NORMAL,ZONE_HIGHMEM。

GFP：GFP_ATOMIC(不可睡眠),GFP_KERNEL（可睡眠）

kmalloc(),kfree()

vmalloc()：虚拟地址连续，而物理地址无须连续。性能不如kmalloc,需要建立页表象，TLB抖动更大。vfree()

slab层：高速缓存组里放不同的对象（常用数据结构），避免频繁分配和释放页。

物理地址在内核上的映射，逻辑地址。

内核栈，大小有限，注意不要溢出。

高端内存的映射：永久映射kmap，可睡眠，只能用于进程上下文。kunmap。

临时映射：不阻塞，atomic，可用在中断上下文和不能重新调度的地方。kmap_atomic,kunmap_atomic.

每个CPU的数据：存放在一个数组中，每一项对应一个处理器。get_cpu()...put_cpu()禁止了内核抢占。使用时禁止抢占，不要睡眠。

如果想从高端内存进行分配，使用alloc_pages(),返回一个指向struct page的指针，而不是逻辑地址。因为高端内存可能并没有被映射，为了获得真正的地址，调用kmap()，把高端内存映射到内核的逻辑地址空间。

# 虚拟文件系统（VFS）

VFS中有四个主要的对象类型：

superblock，代表一个具体的已安装的文件系统。

inode，代表一个具体文件。仅当文件被访问时，才在内存中创建。路径中的每个组成部分都由一个索引节点对象表示。

dentry，代表一个目录项，是路径的一个组成部分。目录是一个文件，目录项不是。解析dentry，结果放在struct nameidata中。没有对应的磁盘数据结构，VFS根据字符串形式的路径名现场创建。快速查找到文件。目录项缓存（dcache），不需要每次解析，hashtable，lru。

file，代表由进程打开的文件。task_struct -> file -> f_dentry -> inode.

索引式文件系统，inode与数据区块固定后就不再变动，inode包含12个直接，1个间接，1个双间接，1个三间接。

# 块I/O层

系统中能够随机访问固定大小数据片的硬件设备称作块设备，这些固定大小的数据片就称作块。

字符设备按照字符流的方式被有序访问，像串口和键盘。

块设备中最小的可寻址单元是扇区。

# 进程地址空间（内核管理用户空间中进程的内存）

虚拟内存技术：地址空间远远大于系统物理内存。

进程只能访问有效内存区域内的内存地址。每个内存区域也具有相关权限如对相关进程有可读、可写、可执行属性。如果一个进程访问了不在有效范围中的内存区域，或以不正确的方式访问了有效地址，那么内核就会终止该进程，并返回“段错误”信息。

Struct mm_struct; vma 代码段，数据段，bss，堆，栈，命令行参数，环境变量。

Mmap(list),mm_rb(rbtree).遍历，查找。mmap_cache。

clone()时设置CLONE_VM，共享地址空间，线程。

内核线程没有进程地址空间，也没有mm_struct,task_struct中的mm为空。没有用户上下文。

内核线程在需要的时候，使用前一个进程的内存描述符，即使用前一个进程的页表。

内存区域vm_area_struct（virtual memory areas,VMAs）,一个vma拥有一致的属性。

共享不可写内存节约内存空间。bss段copy on write。

虚拟地址到物理地址：多级页表。PGD，PMD，PTE。检索时要加锁，page_table_lock。依赖硬件。

TLB缓存。

# 页高速缓存和页回写

页高速缓存(cache)是Linux内核实现磁盘缓存，减少磁盘I/O。

写缓存：“回写”，写操作后将页标记为脏，一个进程周期性将脏页写回到磁盘，并清楚脏页标志。使磁盘中的数据和内存中保持一致。

缓存回收：LRU/n，按照访问频繁程度分为n个LRU链。

Struct address_space;管理缓存项和页I/O操作。（page_cache_entity）。每个文件对应一个address_space,对应多个vm_area_struct.

# 可移植性

可移植性是指代码从一种体系结构移植到另外一种不同的体系结构上的方便程度。

所有的接口和核心代码都是独立于硬件体系结构的C语言代码，对于性能要求很严格的部分，内核的特性会根据不同的硬件体系进行调整。

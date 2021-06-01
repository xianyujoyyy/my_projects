* Linux多线程服务端编程

* 深入理解Nginx

  muduo:one loop per thread,acceptor+eventloop(rr；另一种方式，随机开始（防止潮涌）遍历服务器，分配给最快响应的服务器)，acceptor接受连接。event driven。eventloop，不要fork（）。

  Nginx:one loop per process,

  master+worker("惊群"，“负载均衡”：ngx_try_lock_accept_mutex+ngx_accept_mutex+ngx_accept_disabled)，master：接受连接，平滑升级（fork()+exec并通过环境变量在新旧进程之间传递信息），worker的配置和重启。event driven。ngx_cycle_t有ngx_init_cycle配置。

  通信机制：

  muduo:shared memory + 进程间socket

  Nginx:shared memory+socket+signal

  垃圾回收：

  muduo：智能指针+RAII

  Nginx：memory pool（per request），需根据生命周期决定放在哪个pool里。减少CPU资源消耗，减少内存碎片。大块内存直接分配，和pool list连接。

  事件分解：会阻塞的事件分解为两个阶段；不能被当作事件（epoll）的按照时间分解，用timer；如果无法划分，用独立的进程执行，并在执行后通知。

  三个半事件：连接，消息到达，写回完成，断开连接。+timer。

  timer：

  muduo：timefd（可被epoll）+set（底层rbtree）**only for linux**

  Nginx：自己实现的rbtree且不能被epoll，需要单独检查 **跨平台**

  callback：

  muduo：channel存放fd及其callback，epoll结束后，需通过fd查找到相应channel并执行callback（set：O（log n））。

  Nginx：连接池，三个数组，connection_n和其对应的write events，read events数组序号是一致的。

  Epoll：

  struct eventloop,每个对象（struct epitem）挂在红黑树上，方便高效识别，所有epoll中的事件都会与设备建立回调关系，将事件放在rdllink（双向链表）中，复制到用户态中。使用完要close。默认LT。

  muduo默认使用LT触发，Nignx默认使用ET触发。

  connection是否是同一个：

  muduo：将fd事件设置为不关注任何事件，暂时并不关闭connection。

  Nginx：connection指针在第一次初始化时取反，根据指针最后一位必为0，取反后为1，判断是否是一个新连接，防止信息串掉。

  TCP half-close：

  shutdown write，并不关闭read，等待对方关闭。
  
  

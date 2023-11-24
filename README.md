### StudentManager
实训作业

1. studentManager 使用 magic static 单例模式，对于一个单独的管理系统来说写起来简单清楚
2. 封装了 send, recv 函数，先读长度再按照长度读取字节流
3. 对于每一个客户端连接，新建线程的同时 thread_loacl 储存该客户端的套接字文件描述符，减少参数传递
4. 对于访问 mysql 操作加了读写共享锁 shared_mutex

todo:
1. student builder 构造起

#### dependency
1. libmysqlclient-dev
2. mysql-server
3. gcc 9.50 及以上 -std=c++17
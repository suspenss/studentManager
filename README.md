### StudentManager
实训作业

1. studentManager 使用 magic static 单例模式，对于一个单独的管理系统来说写起来简单清楚
2. 封装了 send, recv 函数，先读长度再按照长度读取字节流
3. 对于每一个客户端连接，新建线程的同时 thread_loacl 储存该客户端的套接字文件描述符，减少参数传递
4. 对于访问 mysql 操作加了读写共享锁 shared_mutex
5. 用 gpt 编写了 Makefile，方便编译运行 `make` 编译, `make run_client` 运行客户端, `make run_server` 运行服务端
6. 通过 ostringstream 进行 Student 对象的序列化，方便在服务器之间传递
todo:
1. student builder 构造器

#### dependency
1. libmysqlclient-dev
2. mysql-server
3. gcc 9.50 及以上(shared_mutex) -std=c++17
4. 可能在需要链接 pthread

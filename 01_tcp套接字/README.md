## 第1章 理解网络编程和套接字  

### 1.1 理解网络编程和套接字

#### 什么叫网络编程

网络编程就是编写程序使两台主机之间进行通信  

#### 如何实现网络编程

分为硬件和软件，硬件不是我们需要考虑的事情。因此注重软件即可，也就是专注套接字编程  

#### 什么叫套接字

套接字由操作系统提供，使网络数据传输的专用设备  

#### tcp套接字

套接字分为两种，我们先考虑tcp套接字。  

对于套接字而言，接听和拨打是有区别的，我们先讨论接听套接字。  

我们用电话机的安装到拨通的流程来模拟tcp接听套接字，也就是服务端的套接字。
  
>一共4步  

- 1  

> 问：接电话需要什么
> 答：电话机
> 有了电话机才能安装电话  socket创建

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
成功时返回文件描述符，失败返回-1
```

- 2

> 问：有了电话机之后需要什么
> 答：需要电话号码用来通信
> 如何创建电话号码  bind创建

```c
#include <sys/socket.h>
int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
成功时返回文件描述符，失败返回-1
```

- 3

> 问：有了电话号码就能通信了吗
> 答：还需要电话线，只有接入链路，才能相互通信
> 电话线由  listen负责，一连接就转为可接听模式

```c
#include <sys/socket.h>
int listen(int sockfd, int backlog);
成功时返回文件描述符，失败返回-1
```

- 4
  
> 问：这样电话打进来就能接听吗
> 答: 不够，还需要拿起电话机，接听
> 接听由accept函数来完成

```c
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
成功时返回文件描述符，失败返回-1 
```

ok，我们已经讲完了tcp套接字的打电话套接字，也就是客户端套接字  

客户端套接字比服务端套接字简单许多，只有两个步骤  

> 1. socket创建套接字
> 2. connect向服务端发送连接请求

### 1.2 基于Linux的文件操作

在Linux中socket操作与文件没有区别，因此在数据传输中可以使用I/O相关的函数  

如果想理解文件I/O函数，就先要知道文件描述符  

> 什么叫文件描述符?
> 就是系统分配给文件或者套接字的整数，简单来说，就是函数成功调用时的返回值

例如在Linux中的标准输入，输出及错误的文件描述符

|文件描述符   |对象   |
|---|---|
|0   |标准输入： Standard Input  |
|1   |标准输出： Standard Output |
|2   |标准错误： Standard Error  |

> **文件和套接字一般都需要创建才会被分配文件描述符，但是上述的三种在程序开始之后就会被自动分配**

简单来说文件描述符其实就是对相关函数的简称  

例如，一本说名叫《thiswv虽然是世界上最帅的人，但是他的代码却写得非常差》

如果每次提起这本书都写全名，将会十分的繁琐，因此可以简称他为**5**  

这样的话，下此提起这本书就直接叫5即可  

打开文件函数open

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *path, int flag);
```

> 其中path传递路径，flag传递打开模式信息

|打开模式   |含义   |
|---|---|
|O_CREAT   |必要时创建文件   |
|O_TRUNC   |删除全部现有数据   |
|O_APPEND  |维持现有数据，保存到其后面   |
|O_RDONLY  |只读打开   |
|O_WRONLY  |只写打开   |
|O_RDWR    |读写打开   |

文件打开后必须关闭

```c
#include <unistd.h>

int close(int fd);
```

> fd为文件描述符参数，不管文件还是套接字都可以关闭

将数据写入文件

```c
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t nbytes);
```

> fd 文件描述符，buf 保存要传输数据的缓冲地址值，nbytes 要传输的字节数
> 成功时返回写入的字节数，失败时返回-1

ssize_t 一般时通过typedef声明的  

read 函数用来接受数据

```c
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes);
```

> 成功时返回接受的字节数但遇到文件尾则返回0
> 失败时返回-1
> fd 文件描述符， buf 保存数据的缓冲地址值， nbytes 要接受数据的最大字节值

### 1.5 习题

1. 套接字在网络编程中的作用是什么？为何称它为套接字？  

  > 答：套接字是网络传输的软件设备。 在编程中“套接字”就是用来连接该网络的工具，本身就具有连接的意思。
  > 详细见书本第3页

2. 在服务端创建套接字后，会依次调用listen函数和accept函数。请比较说明二者作用。

  > 答：listen函数起监听作用转换为可接受请求状态，accpet函数是接受连接请求。  

3. 略  

4. 创建套接字后一般会给它分配地址，为什么？为了完成地址分配需要调用哪个函数？

  > 答：通信需要确定目标地址和端口号，套接字用于标识网络中的进程，使数据能够路由到相应的目标。 bind函数

5. 略  

6. 底层文件I/O函数与ANSI标准定义的文件I/O函数之间有何区别？
  
  > 答：  
  > 底层文件I/O函数有对应的文件描述符，如open， read...函数
  > ANSI标准的文件I/O函数 fopen对应的是文件指针 FILE *

7. 略  

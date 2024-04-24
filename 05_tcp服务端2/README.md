### 第五章

回声客户端存在问题

write(sock, message, sizeof(message));
str_len = read(sock, message, BUF_SIZE - 1);
message[str_len] = 0;
printf("Message from server: %s", message);  

上述代码有个小问题，我们发现，客户端关于数据的发送和接收每次都是一个字符串

但第二章讲过，tcp的传输是没有数据边界的。

那么你现在多次调用了write来传递字符串，可能在实际中它一次就传完了。

此时，客户端有可能从服务器端收到多个字符串，这不是我们希望看到的结果。

服务器端也还需考虑如下情况。

字符串太长，需要两个或以上的数据包发送。

服务器端希望一次write写完全部数据，但如果数据太大，操作系统就有可能把数据分成多个数据包发送到客户端。

客户端可能在尚未收到全部数据包时就调用read函数。

#### 回声服务器端没有问题，只有回声客户端有问题？

问题不在服务器端，而在客户端。

分别来看看两者的代码

echo_server.c

```c
while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
  write(clnt_sock, message, str_len);
}
```

echo_client.c

```c
write(sock, message, sizeof(message));
str_len = read(sock, message, BUF_SIZE - 1);
```

两者都在循环利用read和write函数。

实际上客户端传输没有问题，问题在于接受，在看看完整的代码

```c
while (1) {
    fputs("Input message(Q to quit): ", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "Q\n") || !strcmp(message, "q\n")) {
      break;
    }

    write(sock, message, sizeof(message));
    str_len = read(sock, message, BUF_SIZE - 1);
    message[str_len] = 0;
    printf("Message from server: %s", message);
  }
```

可以发现，write是一次性，全部发送完数据，但read接收却不是这样的。

因此，是不是等待一会儿，就可以一次性read所有数据呢？

那么问题又来了，等多久合适呢？1秒，1分钟，10分钟？

#### 回声客户端的解决方法


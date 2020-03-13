# A Simple HTTP WebServer
## Introduction
本项目为基于 epoll 的多线程Web服务器，解析了 Get 请求，可处理静态资源请求，支持 HTTP 长连接及超时断开。  
测试页：
## Envoirment
* 操作系统：Ubuntu 16.04
* 编译器：gcc 5.4.0
* 工程构建：make
## Build
```
git clone https://github.com/desiress/WebServer.git
cd Webserver
make
```
## Usage
```
webserver [port] [file_path(should begin with '/')]

例：webserver 80 /root/Docu
表示开启80端口 浏览器所请求资源的根目录为/root/Docu
```
## Technical points
* 基于 epoll 的 IO 复用机制，采用边缘触发（ET）模式，和非阻塞模式。
* 使用多线程充分利用多核 CPU，并使用线程池避免线程频繁创建销毁的开销。
* version1 基于单线程实现，version2 利用线程池实现多线程，Version3 实现超时断开及HTTP长链接。


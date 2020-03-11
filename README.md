# A Simple HTTP WevServer
## Introduction
本项目为基于 epoll 的Web服务器，解析了 Get 请求，可处理静态资源请求。
## Envoirment
* 操作系统：Ubuntu 16.04
* 编译器：gcc 5.4.0
* 工程构建： make
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
* 由于采用 ET 模式，read、write 和 accept 的时候必须采用循环的方式，直到 error==EAGAIN 为止，防止漏读等清况，这样的效率会比 LT 模式高很多，减少了触发次数

## Timeline 

# A Simple HTTP WebServer
## Introduction
本项目为基于 epoll 的Web服务器，解析了 Get 请求，可处理静态资源请求，支持超时断开。  
测试页：http://114.116.240.106/demo.html
## Envoirment
* 操作系统：Ubuntu 16.04
* 编译器：gcc 5.4.0
* 工程构建：make
* 压测工具：webbench
## Usage
```
cd Webserver
make
webserver [port] [file_path(should begin with '/')]

例：webserver 80 /root/Docu
表示开启80端口 浏览器所请求资源的根目录为/root/Docu
```
## Technical points
* 基于 epoll 的 IO 复用机制，采用边缘触发（ET）模式，和非阻塞模式。
* 由于采用 ET 模式，read 的时候必须采用循环的方式，直到 error==EAGAIN 为止，这样减少了触发次数，提高效率。   
* version1 基于单线程实现，version2 添加超时断开功能。  



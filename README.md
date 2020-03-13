# A Simple HTTP WebServer
## Introduction
本项目为基于 epoll 的Web服务器，解析了 Get 请求，可处理静态资源请求，支持超时断开。  
测试页：http://114.116.240.106/demo.html

## Purpose
本项目是个人的网络编程项目，在开始做这个项目之前，花时间学习了一些 C++ 后台方向的基础知识，但纸上得来终觉浅，学了一段时间后觉得知识点很破碎很抽象，于是想要做一个练手项目，不仅希望能通过这个项目能把所学知识串成一个整体，而且还希望在开发过程中熟练掌握Linux、git、make等开发工具的使用。

## Envoirment
* 操作系统：Ubuntu 16.04
* 编译器：gcc 5.4.0
* 工程构建：make
* 压测工具：webbench

## Usage
```
git clone https://github.com/desiress/WebServer.git
cd Webserver
make

webserver [port] [file_path(should begin with '/')]

例：webserver 80 /root/Docu
表示开启80端口 浏览器所请求资源的根目录为/root/Docu
```
## Technical Points
* 基于 epoll 的 IO 复用机制
* 采用epoll的边沿触发（ET）模式
* 非阻塞IO

## Performance Test
采用 HTTP 压力测试工具 WebBench 进行测试
模拟 1000 个客户端进程，测试时间为 60s
![](https://github.com/desiress/WebServer/blob/master/docs/webbench%20test.png)

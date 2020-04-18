# A Simple HTTP WebServer
## 项目简介
本项目为 C++ 编写的基于 epoll 的多线程 web 服务器，实现了 HTTP 的解析和 Get 方法请求，支持静态资源访问。
测试页：http://114.116.240.106/demo.html

## 项目目的
本项目是个人的网络编程项目，在开始做这个项目之前，花时间学习了一些 C++ 后台方向的基础知识，但纸上得来终觉浅，学了一段时间后觉得知识点很破碎很抽象，于是想要做一个练手项目，不仅希望能通过这个项目能把所学知识串成一个整体，而且还希望在开发过程中熟练掌握Linux、git、make等开发工具的使用。

## 开发环境
* 操作系统：Ubuntu 16.04
* 编译器：g++ 4.8
* 工程构建：make
* 压测工具：webbench

## 使用方法
```
git clone https://github.com/desiress/WebServer.git
cd Webserver
make

webserver [port] [file_path(should begin with '/')]

例：webserver 80 /root/Docu
表示开启80端口 浏览器所请求资源的根目录为/root/Docu
```
## 技术要点
* 基于 epoll 的 IO 复用机制，采用边缘触发（ET）模式，配合非阻塞I/O，解决了高并发下的 socket 处理问题。
* 使用多线程充分利用多核 CPU，并使用线程池避免线程频繁创建销毁的开销。
* 采用同步 I/O 模拟 proactor 模式处理事件，主线程接收请求报文，然后将任务插入请求队列，由工作线程从请求队列中获取任务。

## 性能测试
* 采用 HTTP 压力测试工具 WebBench 进行测试  
* 模拟 1000 个客户端进程，测试时间为 60s  
    
![](https://github.com/desiress/WebServer/blob/master/docs/webbench%20test.png)

## 改进提升
* 实现定时器功能，定时剔除不活跃连接。
* 为减少内存泄漏的可能，应使用智能指针等 RAII 机制。
* 添加日志系统。

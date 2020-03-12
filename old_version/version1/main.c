#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>

#include "socket.h"
#include "http.h"

#define MAXSIZE 1024

int main(int argc, const char* argv[])
{
    if(argc < 3)
    {
        printf("eg: ./a.out port path\n");
        exit(1);
    }

    // 端口
    int port = atoi(argv[1]);
    // 修改进程的工作目录, 方便后续操作
    int ret = chdir(argv[2]);
    if(ret == -1)
    {
        perror("chdir error");
        exit(1);
    }
    
    int epfd = epoll_create(MAXSIZE);
	if(epfd == -1)
    {
        perror("epoll_create error");
        exit(1);
    }
    
    int lfd = lfd_init(port, epfd);
    
    struct epoll_event all[MAXSIZE];
    while(1)
    {
    	int ret = epoll_wait(epfd, all, MAXSIZE, -1);
        if(ret == -1)
        {
            perror("epoll_wait error");
            exit(1);
        }
      
         // 遍历发生变化的节点
        for(int i=0; i<ret; ++i)
        {
            // 只处理读事件, 其他事件默认不处理
            struct epoll_event *pev = &all[i];
            if(!(pev->events & EPOLLIN))
            {
                // 不是读事件
                continue;
            }

            if(pev->data.fd == lfd)
            {
                // 接受连接请求
                accept_connection(lfd, epfd);
            }
            else
            {
                // 读数据
                read_and_respond(pev->data.fd, epfd);
            }
        }
    }
 
    return 0;
}

#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "http.h"
#include "epoll.h"




int main(int argc, char *argv[])
{
    unsigned short port = SERV_PORT;

    if (argc == 3)
        port = atoi(argv[1]);                           //使用用户指定端口.如未指定,用默认端口
        
    // 修改进程的工作目录, 方便后续操作
    if(chdir(argv[2]) < 0) 
    {
        perror("chdir error");
        exit(1);
    }
	
	run(port);
    /* 退出前释放所有资源 */
    return 0;
}


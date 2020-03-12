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

    // �˿�
    int port = atoi(argv[1]);
    // �޸Ľ��̵Ĺ���Ŀ¼, �����������
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
      
         // ���������仯�Ľڵ�
        for(int i=0; i<ret; ++i)
        {
            // ֻ������¼�, �����¼�Ĭ�ϲ�����
            struct epoll_event *pev = &all[i];
            if(!(pev->events & EPOLLIN))
            {
                // ���Ƕ��¼�
                continue;
            }

            if(pev->data.fd == lfd)
            {
                // ������������
                accept_connection(lfd, epfd);
            }
            else
            {
                // ������
                read_and_respond(pev->data.fd, epfd);
            }
        }
    }
 
    return 0;
}

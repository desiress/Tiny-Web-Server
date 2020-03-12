#include <sys/types.h>          
#include <sys/socket.h>
#include <errno.h>
#include <sys/epoll.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "socket.h"


int lfd_init(int port, int epfd)
{
    //�������������׽���
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket error");
        exit(1);
    }

    // lfd�󶨱���IP��port
    struct sockaddr_in serv;
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    // �˿ڸ���
    int flag = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    
    int ret = bind(lfd, (struct sockaddr*)&serv, sizeof(serv));
    if(ret == -1)
    {
        perror("bind error");
        exit(1);
    }

    // ���ü���
    ret = listen(lfd, 64);
    if(ret == -1)
    {
        perror("listen error");
        exit(1);
    }

    // lfd��ӵ�epoll����
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    if(ret == -1)
    {
        perror("epoll_ctl add lfd error");
        exit(1);
    }

    return lfd;
}

void accept_connection(int lfd, int epfd)
{
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    int cfd = accept(lfd, (struct sockaddr*)&client, &len);
    if(cfd == -1)
    {
        perror("accept error");
        exit(1);
    }

    // ��ӡ�ͻ�����Ϣ
    /*char ip[64] = {0};
    printf("New Client IP: %s, Port: %d, cfd = %d\n",
           inet_ntop(AF_INET, &client.sin_addr.s_addr, ip, sizeof(ip)),
           ntohs(client.sin_port), cfd);*/

    // ����cfdΪ������
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    // �õ����½ڵ�ҵ�epoll����
    struct epoll_event ev;
    ev.data.fd = cfd;
    // ���ط�����ģʽ
    ev.events = EPOLLIN | EPOLLET;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
    if(ret == -1)
    {
        perror("epoll_ctl add cfd error");
        exit(1);
    }  
}

void disconnect(int cfd, int epfd)
{
	int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, 0);
	if(ret == -1)
	{
	        perror("epoll_ctl del cfd error");
	        exit(1);
	}
	close(cfd);
}



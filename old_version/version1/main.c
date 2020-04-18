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
        port = atoi(argv[1]);                           //ʹ���û�ָ���˿�.��δָ��,��Ĭ�϶˿�
        
    // �޸Ľ��̵Ĺ���Ŀ¼, �����������
    if(chdir(argv[2]) < 0) 
    {
        perror("chdir error");
        exit(1);
    }
	
	run(port);
    /* �˳�ǰ�ͷ�������Դ */
    return 0;
}


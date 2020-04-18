#ifndef _EPOLL_H
#define _EPOLL_H

#define MAX_EVENTS  1024                                    //监听上限数
#define BUFLEN      4096
#define SERV_PORT   8080

/* 描述就绪文件描述符相关信息 */
struct myevent_s {
    int fd;                                                 //要监听的文件描述符
    int events;                                             //对应的监听事件
    void *arg;                                              //泛型参数
    void (*call_back)(int fd, int events, void *arg);       //回调函数
    int status;                                             //是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    char buf[BUFLEN];
    int len;
    long last_active;                                       //记录每次加入红黑树 g_efd 的时间值
};

void run(unsigned short port);

void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);
void eventadd(int efd, int events, struct myevent_s *ev);
void eventdel(int efd, struct myevent_s *ev);

void do_request(int fd, int events, void *arg);

void initlistensocket(int efd, short port);
void acceptconn(int lfd, int events, void *arg);




//void recvdata(int fd, int events, void *arg);
//void senddata(int fd, int events, void *arg);

#endif
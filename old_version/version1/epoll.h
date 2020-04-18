#ifndef _EPOLL_H
#define _EPOLL_H

#define MAX_EVENTS  1024                                    //����������
#define BUFLEN      4096
#define SERV_PORT   8080

/* ���������ļ������������Ϣ */
struct myevent_s {
    int fd;                                                 //Ҫ�������ļ�������
    int events;                                             //��Ӧ�ļ����¼�
    void *arg;                                              //���Ͳ���
    void (*call_back)(int fd, int events, void *arg);       //�ص�����
    int status;                                             //�Ƿ��ڼ���:1->�ں������(����), 0->����(������)
    char buf[BUFLEN];
    int len;
    long last_active;                                       //��¼ÿ�μ������� g_efd ��ʱ��ֵ
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
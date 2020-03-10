#ifndef _SOCKET_H
#define _SOCKET_H

int lfd_init(int port, int epfd);
void accept_connection(int lfd, int epfd)
void disconnect(int cfd, int epfd)

#endif
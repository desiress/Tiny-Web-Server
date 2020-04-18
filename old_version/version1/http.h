#ifndef _HTTP_H
#define _HTTP_H

void read_and_respond(void *arg);
void handle_request(const char* request, int cfd);
void send_head(int cfd, int no, const char* desp, const char* type, long len);
void send_file(int cfd, const char* filename);
void send_dir(int cfd, const char* dirname);

#endif
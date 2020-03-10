#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#include "http.h"
#include "socket.h"
#include "data.h"

void read_and_respond(int cfd, int epfd)
{
	char line[1024] = {0};
    // ��������
    int len = get_line(cfd, line, sizeof(line));
    if(len == 0)
    {
        printf("�ͻ��˶Ͽ�������...\n");
        // �ر��׽���, cfd��epoll��del
        disconnect(cfd, epfd);         
    }
    else
    {
        printf("����������: %s", line);
        printf("============= ����ͷ ============\n");
        // ��������û����
        // ������
        while(len)
        {
            char buf[1024] = {0};
            len = get_line(cfd, buf, sizeof(buf));
            printf("-----: %s", buf);
        }
        printf("============= The End ============\n");
    }
    // ������: get /xxx http/1.1
    // �ж��ǲ���get����
    if(strncasecmp("get", line, 3) == 0)
    {
        // ����http����
        handle_request(line, cfd);
        // �ر��׽���, cfd��epoll��del
        disconnect(cfd, epfd);         
    }
    
}

void handle_request(const char* request, int cfd)
{
    // ���http������
    // get /xxx http/1.1
    char method[12], path[1024], protocol[12];
    sscanf(request, "%[^ ] %[^ ] %[^ ]", method, path, protocol);

    printf("method = %s, path = %s, protocol = %s\n", method, path, protocol);

    // ת�� ������ʶ����������� - > ����
    // ���� %23 %34 %5f
    decode_str(path, path);
        // ����path  /xx
        // ȥ��path�е�/
    char* file = path+1;
    // ���û��ָ�����ʵ���Դ, Ĭ����ʾ��ԴĿ¼�е�����
    if(strcmp(path, "/") == 0)
    {
        // file��ֵ, ��ԴĿ¼�ĵ�ǰλ��
        file = "./";
    }

    // ��ȡ�ļ�����
    struct stat st;
    int ret = stat(file, &st);
    if(ret == -1)
    {
        // show 404
        send_head(cfd, 404, "File Not Found", ".html", -1);
        send_file(cfd, "404.html");
    }

    // �ж���Ŀ¼�����ļ�
    // �����Ŀ¼
    if(S_ISDIR(st.st_mode))
    {
        // ����ͷ��Ϣ
        send_head(cfd, 200, "OK", get_file_type(".html"), -1);
        // ����Ŀ¼��Ϣ
        send_dir(cfd, file);
    }
    else if(S_ISREG(st.st_mode))
    {
        // �ļ�
        // ������Ϣ��ͷ
        send__head(cfd, 200, "OK", get_file_type(file), st.st_size);
        // �����ļ�����
        send_file(cfd, file);
    }
}

// ������Ӧͷ
void send_head(int cfd, int no, const char* desp, const char* type, long len)
{
    char buf[1024] = {0};
    // ״̬��
    sprintf(buf, "http/1.1 %d %s\r\n", no, desp);
    send(cfd, buf, strlen(buf), 0);
    // ��Ϣ��ͷ
    sprintf(buf, "Content-Type:%s\r\n", type);
    sprintf(buf+strlen(buf), "Content-Length:%ld\r\n", len);
    send(cfd, buf, strlen(buf), 0);
    // ����
    send(cfd, "\r\n", 2, 0);
}

void send_file(int cfd, const char* filename)
{
    // ���ļ�
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
    {
        // show 404
        return;
    }

    // ѭ�����ļ�
    char buf[4096] = {0};
    int len = 0;
    while( (len = read(fd, buf, sizeof(buf))) > 0 )
    {
        // ���Ͷ���������
        send(cfd, buf, len, 0);
    }
    if(len == -1)
    {
        perror("read file error");
        exit(1);
    }

    close(fd);
}

void send_dir(int cfd, const char* dirname)
{
    // ƴһ��htmlҳ��<table></table>
    char buf[4094] = {0};

    sprintf(buf, "<html><head><title>Ŀ¼��: %s</title></head>", dirname);
    sprintf(buf+strlen(buf), "<body><h1>��ǰĿ¼: %s</h1><table>", dirname);

    char enstr[1024] = {0};
    char path[1024] = {0};
    // Ŀ¼�����ָ��
    struct dirent** ptr;
    int num = scandir(dirname, &ptr, NULL, alphasort);
    // ����
    for(int i=0; i<num; ++i)
    {
        char* name = ptr[i]->d_name;

        // ƴ���ļ�������·��
        sprintf(path, "%s/%s", dirname, name);
        printf("path = %s ===================\n", path);
        struct stat st;
        stat(path, &st);

        encode_str(enstr, sizeof(enstr), name);
        // ������ļ�
        if(S_ISREG(st.st_mode))
        {
            sprintf(buf+strlen(buf), 
                    "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>",
                    enstr, name, (long)st.st_size);
        }
        // �����Ŀ¼
        else if(S_ISDIR(st.st_mode))
        {
            sprintf(buf+strlen(buf), 
                    "<tr><td><a href=\"%s/\">%s/</a></td><td>%ld</td></tr>",
                    enstr, name, (long)st.st_size);
        }
        send(cfd, buf, strlen(buf), 0);
        memset(buf, 0, sizeof(buf));
        // �ַ���ƴ��
    }

    sprintf(buf+strlen(buf), "</table></body></html>");
    send(cfd, buf, strlen(buf), 0);

    printf("dir message send OK!!!!\n");
#if 0
    // ��Ŀ¼
    DIR* dir = opendir(dirname);
    if(dir == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    // ��Ŀ¼
    struct dirent* ptr = NULL;
    while( (ptr = readdir(dir)) != NULL )
    {
        char* name = ptr->d_name;
    }
    closedir(dir);
#endif
}
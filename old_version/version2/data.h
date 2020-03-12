#ifndef _DATA_H
#define _DATA_H

int get_line(int sock, char *buf, int size);
const char *get_file_type(const char *name);
void encode_str(char* to, int tosize, const char* from);
void decode_str(char *to, char *from);
int hexit(char c);

#endif
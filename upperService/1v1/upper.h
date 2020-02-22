#ifndef __UPPER__H
#define __UPPER__H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <arpa/inet.h>

#define SERVPORT 23456

void perrorExit(const char* errinfo);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Socket(int domain, int type, int protocol);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void* ptr, size_t nbytes);
int Close(int fd);
ssize_t WriteN(int fd, const void* vptr, size_t n);
ssize_t ReadN(int fd, void *vptr, ssize_t n);




#endif //__UPPER__H

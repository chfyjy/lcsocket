#include <sys/wait.h>
#include <pthread.h>
#include "upper.h"

struct sockInfo
{
	struct sockaddr_in clientAddr;
	int clientFd;
};

void* doUpper(void *arg)
{
	struct sockInfo* sinfo = (struct sockInfo*)arg;
	char buf[BUFSIZ];
	char ipbuf[16];
	printf("recv from:%s:%d\n",
		inet_ntop(AF_INET, &(*sinfo).clientAddr.sin_addr, ipbuf, sizeof(ipbuf)),
		ntohs((*sinfo).clientAddr.sin_port));

	int n, i;
	while(1)
	{
		n = Read(sinfo->clientFd, buf, BUFSIZ);
		if(n == 0)
		{
			printf("%s:%d closed\n",
			inet_ntop(AF_INET, &(*sinfo).clientAddr.sin_addr, ipbuf, sizeof(ipbuf)),
			ntohs((*sinfo).clientAddr.sin_port));
			return NULL;
		}
		for(i = 0; i < n; i++)
			buf[i] = toupper(buf[i]);
		Write(sinfo->clientFd, buf, n);
	}
	Close(sinfo->clientFd);
	printf("%s:%d closed\n",
		inet_ntop(AF_INET, &(*sinfo).clientAddr.sin_addr, ipbuf, sizeof(ipbuf)),
		ntohs((*sinfo).clientAddr.sin_port));

	return NULL;
}

int main(void)
{
	int sfd, cfd;
	struct sockaddr_in serv_addr;
	sfd = Socket(AF_INET, SOCK_STREAM, 0);//创建套接字

	//初始化地址结构 man 7 ip 可以查看结构体信息
	bzero(&serv_addr, sizeof(serv_addr));//清零结构体

	serv_addr.sin_family = AF_INET;//初始化协议族为ipv4
	serv_addr.sin_port = htons(SERVPORT);//绑定端口号
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);//设置监听ip为任意有效ip
	//inet_pton(AF_INET, "192.168.12.12", &serv_addr.sin_addr.sin_addr)

	//绑定地址结构
	Bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	//设定连接发起上限 此处不阻塞
	Listen(sfd, 128);

	pthread_t tid;
	//建立连接
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	
	while(1)
	{
		client_addr_len = sizeof(client_addr);
		cfd = Accept(sfd, (struct sockaddr*)&client_addr, &client_addr_len);
		struct sockInfo sinfo;
		sinfo.clientFd = cfd;
		sinfo.clientAddr = client_addr;
		pthread_create(&tid, NULL, doUpper, (void*)& sinfo);//创建线程
		pthread_detach(tid);//分离线程

	}


	//关闭连接
	Close(sfd);

	return 0;
}

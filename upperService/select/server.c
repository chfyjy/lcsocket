#include "upper.h"
#include <ctype.h>
#include <sys/select.h>

int main(void)
{
	int maxfd, listenfd, connfd, sockfd;

	struct sockaddr_in client_addr, server_addr;
	socklen_t client_addr_len;

	//创建用于监听的描述符
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	//初始化服务器地址信息结构体
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = SERVPORT;
	//绑定监听描述符与地址信息结构体
	Bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	//监听
	Listen(listenfd, 128);

	maxfd = listenfd;
	int i, maxi, j;
	ssize_t n;
	maxi = -1;
	int nready, clientfds[FD_SETSIZE];
	for(int i = 0; i < FD_SETSIZE; i++)
		clientfds[i] = -1;

	fd_set readset, allset;
	//初始化select监听集合
	FD_ZERO(&allset);
	//设置请求描述符到监听集合
	FD_SET(listenfd, &allset);

	char buff[BUFSIZ], str[INET_ADDRSTRLEN];

	while(1)
	{
		readset = allset;
		printf("sleect \n");
		nready = select(maxfd+1, &readset, NULL, NULL, NULL);
		printf("sleect returned:%d\n", nready);
		if(nready < 0)
			perrorExit("select error");
		//判断是否有新的连接到服务器
		if(FD_ISSET(listenfd, &readset))
		{
			client_addr_len = sizeof(client_addr);
			//接受新连接请求 此处可立即执行 而不阻塞
			connfd = 
				Accept(listenfd, (struct sockaddr*)&client_addr, &client_addr_len);
			printf("%s:%d connected.\n", 
				inet_ntop(AF_INET, &client_addr, str, INET_ADDRSTRLEN),
				ntohs(client_addr.sin_port));
			//将连接后的描述符放置到描述符数组
			for(int i = 0; i < FD_SETSIZE; i++)
			{
				if(clientfds[i] < 0)
				{
					clientfds[i] = connfd;
					break;
				}
			}
			if(i == FD_SETSIZE)
			{
				fputs("too many clients\n", stderr);
				exit(1);
			}
			//设置连接后的描述符到监听集合
			FD_SET(connfd, &allset);
			//更新最大的描述符
			if(connfd > maxfd)
				maxfd = connfd;
			//更新描述符数量/在数组的最大下标
			if(i > maxi)
				maxi = i;
			if(--nready == 0)
				continue;
		}
		for(i = 0; i < maxi; i++)
		{
			if((sockfd = clientfds[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &readset))
			{
				if((n = Read(sockfd, buff, BUFSIZ)) == 0)
				{
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					clientfds[i] = -1;
				}
				else if(n > 0)
				{
					for(j = 0; j < n; j++)
						buff[j] = toupper(buff[j]);
					Write(sockfd, buff, n);
				}
				if(--nready == 0)
					break;
			}
		}
	}

	Close(listenfd);

	return 0;
}

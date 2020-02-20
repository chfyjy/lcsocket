#include "upper.h"

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

	//建立连接
	struct sockaddr_in client_addr;
	pid_t pid;
	
	while(1)
	{
		socklen_t client_addr_len = sizeof(client_addr);
		cfd = Accept(sfd, (struct sockaddr*)&client_addr, &client_addr_len);

		pid = fork();
		if(pid < 0)
			perrorExit("fork error");
		else if(pid == 0)//子进程
		{
			Close(sfd);
			break;
		}
		else//未回收子进程 客户端退出后 服务程序未结束 将会产生大量僵尸进程
			Close(cfd);

	}
	if(pid == 0)
	{
		int n, i;
		//打印连接的客户端信息
		char ipbuff[16];                	
		printf("client addr:%s:%d\n", 
			inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuff, sizeof(ipbuff)), 
     			ntohs(client_addr.sin_port));    
		char buff[BUFSIZ];
		while(1)
		{
			n = Read(cfd, buff, BUFSIZ);
			if( n == 0)
			{
				Close(cfd);
				return 0;
			}
			else if(n == -1)
			{
				perrorExit("Read error");
			}
			else
			{
				for(i = 0; i < n; i++)
				{
					buff[i] = toupper(buff[i]);
				}
				Write(cfd, buff, n);
			}
		}

	}


	//关闭连接
	Close(sfd);

	return 0;
}

#include "upper.h"

int main(void)
{
	int sfd, cfd;
	struct sockaddr_in serv_addr;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("create error");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVPORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret == -1)
	{
		perror("bind error");
		return -1;
	}
	
	ret = listen(sfd, 128);
	if(ret == -1)
	{
		perror("listen error");
		return -1;
	}

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	cfd = accept(sfd, (struct sockaddr*)&client_addr, &client_addr_len);
	if(cfd < 0)
	{
		perror("accept error");
		return -1;
	}
	else	     
	{
		char ipbuff[16];                	
		printf("client addr:%s:%d\n", 
				inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuff, sizeof(ipbuff)), 
     				ntohs(client_addr.sin_port));
	}    

	int n, i;
	char buff[BUFSIZ];
	while(1)
	{
		n = read(cfd, buff, BUFSIZ);
		for(i = 0; i < n; i++)
		{
			buff[i] = toupper(buff[i]);
		}
		write(cfd, buff, n);
	}

	close(cfd);
	close(sfd);

	return 0;
}

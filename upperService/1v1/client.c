#include "upper.h"

int main(void)
{
	int cfd;
	struct sockaddr_in serv_addr;
	cfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVPORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	Connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	int n;
	while(1)
	{
		char buff[BUFSIZ] = {'\0'};
		fgets(buff, sizeof(buff), stdin);
		n = strlen(buff);
		write(cfd, buff, n);
		
 		n = Read(cfd, buff,sizeof(buff));
		write(STDOUT_FILENO, buff, n);
		
	}
	Close(cfd);

	return 0;
}

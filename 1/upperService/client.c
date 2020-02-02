#include "upper.h"

int main(void)
{
	int cfd;
	struct sockaddr_in serv_addr;
	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd == -1)
	{
		printf("%s\n", "create error");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVPORT);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int ret = connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret == -1)
	{
		printf("%s\n", "connect error");
		return -1;
	}
	
	int n, m, i;
	while(1)
	{
		char buff[BUFSIZ] = {'\0'};
		//scanf("%s", buff);
		fgets(buff, sizeof(buff), stdin);
		n = strlen(buff);
		write(cfd, buff, n);
		
 		n = read(cfd, buff,sizeof(buff));
		write(STDOUT_FILENO, buff, n);
		
	}
	close(cfd);

	return 0;
}

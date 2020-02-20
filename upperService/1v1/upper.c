#include "upper.h"
//错误退出
void perrorExit(const char* errinfo)
{
	perror(errinfo);//打印错误信息
	exit(-1);//退出当前进程
}
//接受连接
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int n;
again:
	if((n = accept(sockfd, addr, addrlen)) < 0)
	{	//正常的失败情况
		if((errno == ECONNABORTED) || (errno == EINTR))
			goto again;//重新尝试
		else//异常的失败情况
			perrorExit("accept error");//退出
	}
	return n;
}
//绑定
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	  int n;
	  //绑定并判断绑定结果
	  if((n = bind(sockfd, addr, addrlen)) < 0)
		  perrorExit("bind error");//绑定失败退出
	  return n;
}
//连接
int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int n;
	n = connect(sockfd, addr, addrlen);//连接
	if(n < 0)//失败退出
		perrorExit("connect error");
	return n;
}
//监听
int Listen(int sockfd, int backlog)
{
	int n;
	//监听 并判断
	if((n = listen(sockfd, backlog)) < 0)
		perrorExit("listen error");//失败退出
	return n;
}
//创建套接字
int Socket(int domain, int type, int protocol)
{
	int n;
	//创建套接字并判断结果
	if((n = socket(domain, type, protocol)) < 0)
		perrorExit("socket error");//失败退出
	return n;
}
//读
ssize_t Read(int fd, void *buf, size_t count)
{
	ssize_t n;
again:
	//读取并判断结果
	if((n = read(fd, buf, count)) == -1)
	{
		if(errno == EINTR)//正常的失败情况
			goto again;//重试
		else//异常的失败情况
			return -1;//返回错误
	}
	return n;
}
//写
ssize_t Write(int fd, const void* ptr, size_t nbytes)
{
	ssize_t n;
again:
	//写并判断结果
	if((n = write(fd, ptr, nbytes)) == -1)
	{
		if(errno == EINTR)//正常的失败情况
			goto again;//重试
		else//异常的失败情况
			return -1;//返回错误
	}
	return n;
}
//关闭套接字
int Close(int fd)
{
	int ret;
	//关闭套接字 并判断结果
	if((ret = close(fd)) == -1)
		perrorExit("close error");//错误退出
	return ret;
}
//读取n字节
ssize_t ReadN(int fd, void *vptr, ssize_t n)
{
	size_t nleft = n;//初始化剩余需要读取的字节数
	ssize_t nread = 0;//初始化本次读取的字节数
	char *ptr = vptr;//初始化读取后存放目标缓存指针

	while(nleft > 0)//剩余需要读取的字节数大于0时 继续读取
	{	//读取并判断结果
		if((nread = read(fd, ptr, nleft)) < 0)
		{	//正常的失败返回 表示本次读取的字节数为0
			if(errno == EINTR)
				nread = 0;
			else//异常的失败返回
				return -1;
		}
		else if(nread == 0)
			break;//读取失败时退出
		nleft -= nread;//重新计算剩余需要读取的字节数
		ptr += nread;//目标缓存指针前移读取字节数个位置
	}
	//返回实际读取的字节数
	return n-nleft;
}
//写入n字节
ssize_t WriteN(int fd, const void* vptr, size_t n)
{
	size_t nleft = n;//初始化剩余需要写入的字节数
	ssize_t nwritten = 0;//初始化本次写入的字节数
	const char *ptr = vptr;	//初始化指向需写入的源数据的指针
	//剩余需要写入的字节数大于0时继续写入
	while(nleft > 0)
	{	//写入并判断写入结果
		if((nwritten = write(fd, ptr, nleft)) <= 0)
		{//正常的失败返回
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else//异常的失败返回
				return -1;
		}
		nleft -= nwritten;//重新计算剩余需要写入的字节数
		ptr += nwritten;//源数据指针前移已写入字节数个位置
	}

	return n;
}


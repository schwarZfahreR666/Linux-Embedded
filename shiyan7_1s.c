#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define port 3333
int main(int argc,char *argv[])
{
	int sockfd,fd;
	struct sockaddr_in ser_addr;
	struct sockaddr_in cli_addr;
	int size;
	char s[]="Hello!\n";
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		printf("Create socket error\n");
	bzero(&ser_addr,sizeof(struct sockaddr_in));
	ser_addr.sin_family=AF_INET;
	
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	ser_addr.sin_port=htons(port);
	if(bind(sockfd,(struct sockaddr *)(&ser_addr),sizeof(struct sockaddr))==-1)
		printf("bind error");
	if(listen(sockfd,5)==-1)
		printf("set listen error\n");
	while(1)
	{
		size=sizeof(struct sockaddr_in);
		fd=accept(sockfd,(struct sockaddr *)(&cli_addr),&size);
		write(fd,s,strlen(s));
		close(fd);
		
	}
	close(sockfd);
	exit(0);
}

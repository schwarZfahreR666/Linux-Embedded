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
	int sockfd;
	char buffer[1024];
	struct sockaddr_in ser_addr;
	struct hostent *host;
	int n;
	host=gethostbyname(argv[1]);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&ser_addr,sizeof(struct sockaddr_in));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(port);
	ser_addr.sin_addr=*((struct in_addr *)host->h_addr);
	
	if(connect(sockfd,(struct sockaddr *)(&ser_addr),sizeof(struct sockaddr))==-1)
		printf("error\n");
	n=read(sockfd,buffer,1024);
	buffer[n]='\0';
	printf("recive: %s\n",buffer);
	close(sockfd);
	exit(0);
	
}

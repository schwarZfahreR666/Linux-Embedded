/*udp_client.c*/
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<arpa/inet.h>
#define SERVER_PORT 8888
#define MAX_BUF_SIZE 1024

void udpc_requ(int sockfd,const struct sockaddr_in *addr,int len)
{
	char buffer[MAX_BUF_SIZE];
	int n;
	while(1)
	{	
		printf("Please input char:\n");
		fgets(buffer,MAX_BUF_SIZE,stdin);
		sendto(sockfd,buffer,strlen(buffer),0,addr,len);
		bzero(buffer,MAX_BUF_SIZE);
	}
}

int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in addr;

	
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	

	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(SERVER_PORT);
	inet_aton(argv[1],&addr.sin_addr);
	

	udpc_requ(sockfd,&addr,sizeof(struct sockaddr_in));
	close(sockfd);

}

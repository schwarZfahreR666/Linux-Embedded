#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#define BUFFER_SIZE 1024
int main(int argc,char **argv)
{
	int from_fd,to_fd;
	int bytes_read,bytes_write;
	int fd_length;
	char buffer[BUFFER_SIZE];
	char *ptr;
	if(argc!=3)
	{
		printf("The number of arguments is wrong!\n");
		return 0;
	
	}
	if((from_fd=open(argv[1],O_RDONLY))==-1)
	{
		printf("Open original file fail! \n");
		return 0;
	}
	if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
	{
		printf("Create objective file fail! \n");
		return 0;
	}
	while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
	{
		if((bytes_read==-1)&&(errno!=EINTR)) break;
		else if(bytes_read>0)
		{
			ptr=buffer;
			while(bytes_write=write(to_fd,ptr,bytes_read))
			{
				if((bytes_write==-1)&&(errno!=EINTR)) break;
				else if(bytes_write==bytes_read) break;
				else if(bytes_write>0)
				{
					ptr+=bytes_write;
					bytes_read-=bytes_write;
				}
				
			}
			if(bytes_write==-1) break;
		}
	}
	fd_length=lseek(from_fd,0,SEEK_END);
	close(from_fd);
	close(to_fd);
	printf("The length of file is %d \n",fd_length);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#define MEM S_IRUSR|S_IWUSR
int main()
{
int shmid;
pid_t pid;
char *p1_MEMORY,*p2_MEMORY;
if((shmid=shmget(IPC_PRIVATE,1024,MEM))==-1)
{
printf("Create memory Error\n");
return 0;
}
pid=vfork();
if(pid==0)
{
p1_MEMORY=shmat(shmid,0,0);
memset(p1_MEMORY,'\0',1024);
strncpy(p1_MEMORY,"Hello!",1024);
printf("I'm A:%s\n",p1_MEMORY);
wait(NULL);
exit(0);
}
if(pid>0)
{
sleep(1);

p2_MEMORY=shmat(shmid,0,0);
printf("I'm B:%s\n",p2_MEMORY);
exit(0);

}
}

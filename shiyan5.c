#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    pid_t pid_1,pid_2;
    int n;
    pid_1=vfork();
    if(pid_1==0)
    {
    
    for(n=1;n<6;n++)
    {
    printf("I am %d,i am running %d s\n",getpid(),n);
    sleep(1);
    }
    _exit(0);
    }
    else if(pid_1>0)
    {
    pid_2=wait(NULL);
    printf("%d has finished after %d s\n",pid_2,n-1);
    
    }
    return 0;


}

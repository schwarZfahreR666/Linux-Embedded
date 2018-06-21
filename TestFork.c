#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main()
{
    pid_t pid_1;
    char s[100]="I am 1";
    pid_1=vfork();
    if(pid_1==0)
    {
    s[5]='2';
    printf("%s\n",s);
    _exit(0);
    }
    else if(pid_1>0)
    {
    printf("%s\n",s);
    }
    return 0;


}

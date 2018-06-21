#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
int g_Flag=0;
int preflag=0,aftflag=0;
sem_t sem;
void Pthread1()
{
sem_wait(&sem);
printf("This is thread1\n");


g_Flag=1;
//sleep(1);
pthread_exit(0);


}
void Pthread2()
{

printf("This is thread2\n");


g_Flag=2;
sem_post(&sem);
//sleep(1);
pthread_exit(0);


}
int main()
{
pthread_t t1,t2;
sem_init(&sem,0,0);
while(1){
pthread_create(&t1,NULL,(void*)Pthread1,NULL);
preflag=g_Flag;
if((preflag==1&&aftflag==2)||(preflag==2&&aftflag==1))
{
printf("main pthread exit\n");
exit(0);
}
pthread_create(&t2,NULL,(void*)Pthread2,NULL);
aftflag=g_Flag;
if((preflag==1&&aftflag==2)||(preflag==2&&aftflag==1))
{
printf("main pthread exit\n");
exit(0);
}
pthread_join(t2,NULL);
}


}

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/ipc.h>
union semun
{
int val;
struct semid_ds *buf;
unsigned short *array;
};
int sem_p(int sem_id)
{
struct sembuf sem_buf;
sem_buf.sem_num=0;
sem_buf.sem_op=-1;
sem_buf.sem_flg=SEM_UNDO;
semop(sem_id,&sem_buf,1);
return 0;
}
int sem_v(int sem_id)
{
struct sembuf sem_buf;
sem_buf.sem_num=0;
sem_buf.sem_op=1;
sem_buf.sem_flg=SEM_UNDO;
semop(sem_id,&sem_buf,1);
return 0;
}
int main()
{
pid_t pid;
int sem_id;
key_t sem_key;
union semun sem_union;
sem_union.val=1;
sem_key=ftok(".",'a');
sem_id=semget(sem_key,1,0666|IPC_CREAT);
semctl(sem_id,0,SETVAL,sem_union);
pid=fork();
if(pid==0)
{
sem_p(sem_id);
printf("child running!\n");
sem_v(sem_id);
sleep(2);
sem_p(sem_id);
printf("child exit\n");
exit(0);
}
else if(pid>0)
{
sem_p(sem_id);
printf("A running!\n");
sem_v(sem_id);
sleep(2);
sem_p(sem_id);
printf("A wake up and exit\n");
sem_v(sem_id);
exit(0);
}


}

#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main(void)
{
	struct timeval time1,time2;
	int i,m,n;
	long time3; 
	if(gettimeofday(&time1,NULL)==-1) printf("Wrong! \n");
	printf("The starting time is %ld\n",time1.tv_sec*1000000+time1.tv_usec);
	for(i=0;i<1000;i++)
	{
		for(m=0;m<1000;m++)
		{
			for(n=0;n<1000;n++)
			{
				
			}
		}
	}
	if(gettimeofday(&time2,NULL)==-1) printf("Wrong! \n");
	printf("The ending time is %ld\n",time2.tv_sec*1000000+time2.tv_usec);
	time3=(time2.tv_sec-time1.tv_sec)*1000000+(time2.tv_usec-time1.tv_usec);
	printf("The total time is %ld us\n",time3);
	return 0;
}

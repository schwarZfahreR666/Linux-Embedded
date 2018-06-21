#include <stdio.h>
void main(void)
{
	int a[20];
	int b[3]={0,0,0};
	int *p;
	int n,i;
	printf("Please enter 20 students' score:");
	for(n=0;n<20;n++)
	{
		scanf("%d",&a[n]);
		printf("%d ",n+1);
	}
	for(i=0;i<20;i++)
		        {
				                if(a[i]>=80&&a[i]<=100)
						       	b[0]++;
						if(a[i]>=60&&a[i]<=79)
						       	b[1]++;
						if(a[i]<=59&&a[i]>=0)
						       	b[2]++;
			}
	printf("The number of A is:%d,The number of B is:%d,The number of C is:%d\n",b[0],b[1],b[2]);


}

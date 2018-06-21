#include <stdio.h>
void main(void)
{
	int a[10];
	int c;
	int i=0,n=0;
	for(i=0;i<10;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=0;i<10;i++)
	{
		for(n=0;n<9;n++)
		{
			if(a[n]>a[n+1])
			{
				c=a[n];
				a[n]=a[n+1];
				a[n+1]=c;
			}

		}
	}
	for(i=0;i<10;i++)
	{
		printf("%d ",a[i]);

	}
	printf("\n");

}

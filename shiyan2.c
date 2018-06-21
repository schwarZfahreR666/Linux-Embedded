#include <stdio.h>
int main(int argc,char **argv)
{
	FILE *fd;
	char ch;
	if(argc!=2)
	{
		printf("The number of arguments is wrong!\n");
		return 0;
	
	}
	if((fd=fopen(argv[1],"a+"))==NULL)
	{
		printf("Open or create file fail! \n");
		return 0;
	}
	printf("Please input string:\n");
	ch=getchar();
	while(ch!='\n')
	{
		if((fputc(ch,fd)==-1)) printf("Input fail!\n");
		printf("%c",ch);
		ch=getchar();
	}
	printf("\n");
	fclose(fd);
	return 0;
	
}

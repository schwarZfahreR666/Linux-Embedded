#include "chat.h"
int socketfd;  //套接字描述符
//菜单提示的数据结构   
typedef struct {
	char cmd[40];			//format
	char explain[100];//description
	int  admin;				//authority
}usage;
int global_admin_flag=NORMAL_USER;   //用户的权限标记，默认为0
int globel_is_shutup;   						//用户是否被禁言的标记 ,默认为0
  
//0为普通用户具有的执行权限 ，1为管理员具有的执行权限 。
 usage help_menu[] = {
	{"format", 			        "\tdescription",0},
	{">信息", 			      "\t\t与所有在线用户聊天",0},
	{":用户名>信息",		    "\t选择用户聊天",0},
	{"--online",            "\t显示在线用户",0},
	{"--help" ,              "\t\t显示帮助信息",0},
	{"exit",		            "\t\t退出",0},
	{0,0,0}
}; 

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc ,char *argv[])
{
  signal(SIGINT, signHandler); 
	inits();
	close(socketfd);
	return 0;
}
//初始化链接
void  inits()
{
	struct sockaddr_in   server;
	if((socketfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
         perror("creat socket error\n");
         exit(1);
    }
	bzero(&server, sizeof(server));
	//memset(&server,0,sizeof(server));
	// bzero(&(server.sin_zero),8);  
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	inet_pton(AF_INET,IP,&server.sin_addr);
	if(( connect(socketfd,(struct sockaddr *)&server,sizeof(struct sockaddr)))==-1)
	{
         perror("connect error\n");
         exit(1);
    } 
	reg_log(); //    login or register
	pthread_t tid;//线程标识符 pthread_t 
	if((pthread_create(&tid,NULL,pthread_fun,&socketfd))==-1)
	print_err("client pthread_create() error");		
   	writedata();
}
//显示菜单项
void show_menu()
{
	int i = 0;
	printf("*********************************************\n");
	for(; help_menu[i].cmd[0] != 0; i++)
	{
		 
	if(help_menu[i].admin==NORMAL_USER)//普通用户
		printf("*\t%s\t\t%s\n", help_menu[i].cmd, help_menu[i].explain);
	}
	printf("********************************************\n");
}
//进行选择注册或登陆 。
void reg_log()
{
	char ch;
	printf("1：注册;\t2：登录\n");
	printf("请选择:");
	while(1)
	{
		ch=getchar();
		if(ch=='2')
		{
			int get_ret=0;
  		while(1)
			{ 
				login();
				read(socketfd,&get_ret,sizeof(get_ret));
				if(get_ret==NORMAL_USER_LOGIN_SUCCESS)//1
				{
					printf("用户您已成功登录。欢迎您！\n\n");
					global_admin_flag =NORMAL_USER;//0普通用户
			    show_menu();//显示菜单
					break;
				}
				
				else if(get_ret==NORMAL_USER_LOGIN_FAILED_ONLINE)//2
                {
					printf("登录失败，该用户在线!\n");
                    exit(EXIT_SUCCESS);
					break;
                }
                else if(get_ret==NORMAL_USER_LOGIN_PASSWD_ERROR) //4
          	        printf("密码错误，请重新登录！\n");
                else//0
					printf("用户名错误，请重新登录！\n");
			}
			break;
		}
		else if(ch=='1')
		{
			 int get_ret=0;
			 while(1)
			{
				register_client();
				read(socketfd,&get_ret,sizeof(int));
			  if(get_ret==REGIST_EXITED)//账号已经存在 1
           printf("该用户已存在，请重新输入！\n ");
 				else if(get_ret==REGIST_FALIED)//注册失败 0
					 printf("注册失败，请重新输入!\n ");
				else 
				{
					printf("注册成功,帐号ID为：%d ,请登陆。\n ",get_ret);
					exit(EXIT_SUCCESS);	
					break;
				}
			}
			break;
		}
		else
		{
			printf("输入错误，请重新选择.\n");
		}
		//清空输入流.
		for(  ;   (ch=getchar())!='\n'  && ch !=EOF; )
			continue;
	}      
}

//判断用户名是否输入了非法字符eg:空格，标点或特殊符号
void isvalid (char   *str)
{
   while(1)
   { 
         scanf("%s",str);
         int  i=0 ,flag=-1;
 			   for(i=0;str[i]!=0;i++)
			   { 
			         if(ispunct(str[i]))
                {
  		          	 flag=i;
  		         		 break;		   
  		         }
		     }
		      if (flag!=-1)
		      {
		        printf("对不起，您输入了非法字符，请重新输入！\n");
		    	  bzero(str,sizeof(str));
		    	}
		    	else break;
 	}
}
//注册新用户
void register_client()
{
	pthread_mutex_lock(&mutex);
	send_info  send;
	printf("用户名：\n");
	isvalid (send.name);
 	printf("密码  ：\n");
	isvalid (send.passwd);
	send.type=CLIENT_REGISTER;
	write(socketfd,&send,sizeof(send));
	pthread_mutex_unlock(&mutex);
} 
//登陆
void  login()
{
	send_info  send;
	printf("用户名："); 
  isvalid (send.name);
 	printf("密码  ：");
  isvalid (send.passwd);
  send.type=CLIENT_LOGIN;
	write(socketfd,&send,sizeof(send));

}
// 接收数据
void *pthread_fun(void *arg)
{
	char buf[BUF_SIZE]={0};
	int length=0;
	while(1)
	{
		length=read(socketfd,buf,BUF_SIZE);
		if(length<=0)
		{
			printf("服务器已关闭!\n");
			exit(EXIT_SUCCESS);	
			break;
		}
		
  		printf("%s\n",buf);
		memset(buf,0,sizeof(buf)); // clear  buf
	}
	close(socketfd);
	pthread_exit(NULL);
	exit(EXIT_SUCCESS);	
}

//打印错误信息
void  print_err(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

//解析字符串，包装填充数据结构
void parse_input_buf(char *src,  send_info  *send)
{
  switch(src[0]){
	case '>' : 
		 send->type=PUBLIC_CHAT;
		 strcpy(send->buf,src+1);
		 break;
	case ':' :
	    strcpy(send->id,strtok(src+1,">"));
		  send->type=PRIVATE_CHAT; 
			strcpy(send->buf,strtok(NULL,">"));
		 break;
	case '-' :
       if(strcmp(src,"--online")==0)
          send->type=CLIENT_ONLINE;  
       else if(strcmp(src,"--help")==0)
          show_menu();
  		 break;
  
	default :
		send->type=0;
		strcpy(send->buf,src); 
		 break;
	}
} 
//发送数据
void writedata()
{
  char buf[BUF_SIZE]={'\0'};
  send_info  send; 
 	while(1)
	{
		gets(buf); 
		parse_input_buf(buf, &send); 
		if(strcmp("--help",buf)==0) continue;	
		if(!globel_is_shutup)
    	write(socketfd, &send, sizeof(send));
	  if(strcmp("exit",buf)==0) 
	  	{   
	  		close(socketfd);
	  	  exit(EXIT_SUCCESS);
	  	}
		memset(buf,0,sizeof(buf));
	}
}

//忽略ctrl +c 键的处理函数
void signHandler(int signNo)
{
		DEBUG("singal:%d \n",signNo);
    printf("退出聊天室请按输入 exit ，谢谢.\n");
}


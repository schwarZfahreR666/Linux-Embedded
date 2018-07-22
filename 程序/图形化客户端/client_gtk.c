#include <gtk/gtk.h>
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
char g_usr[100];
char g_password[100];
GtkTextBuffer *get_text_buffer;
GtkTextIter start;
GtkTextIter end;
  
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
#include "stdlib.h"
pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;


//初始化链接
void  inits(GtkWidget *widget, gpointer entry)
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
	login(); //    login or register
	pthread_t tid;//线程标识符 pthread_t 
	if((pthread_create(&tid,NULL,pthread_fun,&socketfd))==-1)
	print_err("client pthread_create() error");		
   //	writedata();
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


//判断用户名是否输入了非法字符eg:空格，标点或特殊符号
void isvalid (char   *str,int n)
{
   
 	if(n==1)
 	{
 	strcpy(str,g_usr);
 	}
 	else if(n==2)
 	{
 	strcpy(str,g_password);
 	}
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
		    
 	
}

//登陆
void  login()
{

	char *s1,*s2;
	int get_ret=0;
	send_info  send;
	isvalid (send.name,1);
	isvalid (send.passwd,2);
	send.type=CLIENT_LOGIN;
	
	
	
	write(socketfd,&send,sizeof(send));
	
	read(socketfd,&get_ret,sizeof(get_ret));
				if(get_ret==NORMAL_USER_LOGIN_SUCCESS)//1
				{
					printf("用户您已成功登录。欢迎您！\n\n");
					gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		gtk_text_buffer_insert(get_text_buffer, &end, "用户您已成功登录。欢迎您！\n\n" ,-1);
					global_admin_flag =NORMAL_USER;//0普通用户
					
				}
				
				else if(get_ret==NORMAL_USER_LOGIN_FAILED_ONLINE)//2
                {
                	gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		            gtk_text_buffer_insert(get_text_buffer, &end, "登录失败，该用户在线!\n" ,-1);
					printf("登录失败，该用户在线!\n");
                    
					
                }
                else if(get_ret==NORMAL_USER_LOGIN_PASSWD_ERROR)
                { //4
          	        printf("密码错误，请重新登录！\n");
          	        gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		gtk_text_buffer_insert(get_text_buffer, &end, "密码错误，请重新登录！\n" ,-1);
		}
                else//0
					{printf("用户名错误，请重新登录！\n");
					gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		gtk_text_buffer_insert(get_text_buffer, &end, "用户名错误，请重新登录！\n" ,-1);
		}
		
					

}

// 接收数据
void *pthread_fun(void *arg)
{
	char buf[BUF_SIZE]={0};
	int length=0;
	gchar  *str=(gchar *)malloc(BUF_SIZE);
	

	while(1)
	{
		length=read(socketfd,buf,BUF_SIZE);
		if(length<=0)
		{
			//printf("服务器已关闭!\n");
			gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		gtk_text_buffer_insert(get_text_buffer, &end, "服务器已关闭!\n" ,-1);
			exit(EXIT_SUCCESS);	
			break;
		}
		
		strcpy(str,(gchar *)buf);
		gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
		gtk_text_buffer_insert(get_text_buffer, &end, str ,strlen(str));
		gtk_text_buffer_insert(get_text_buffer,&end,"\n",1);/*插入文本到缓冲区*/
		//gtk_text_buffer_set_text (get_text_buffer,str,-1);
  		printf("%s\n",str);
  		system("play -q 3424.mp3 ");
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
void writedata(GtkWidget *widget, gpointer entry)
{
  
	const gchar *entry_text;
	send_info  send; 
	char buf[BUF_SIZE]={'\0'};
	gchar  *str=(gchar *)malloc(BUF_SIZE);
	gchar  *times=(gchar *)malloc(50);
	time_t timep;
	
	// 获得文本内容
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry)); 
	strcpy(buf,entry_text);
	strcpy(str,entry_text);
	time(&timep);
	strcpy(times,ctime(&timep));
	parse_input_buf(buf, &send); 	
	gtk_entry_set_text( entry, "" );
	gtk_text_buffer_get_bounds(get_text_buffer, &start, &end);
	gtk_text_buffer_insert(get_text_buffer, &end, "yourself       " ,-1);
	gtk_text_buffer_insert(get_text_buffer, &end, times  ,-1);
	gtk_text_buffer_insert(get_text_buffer,&end,"\n",1);/*插入文本到缓冲区*/	
	gtk_text_buffer_insert(get_text_buffer,&end,"\n",1);/*插入文本到缓冲区*/	
	gtk_text_buffer_insert(get_text_buffer, &end, str ,-1);
	gtk_text_buffer_insert(get_text_buffer,&end,"\n",1);/*插入文本到缓冲区*/	
	
    	write(socketfd, &send, sizeof(send));
	  if(strcmp("exit",buf)==0) 
	  	{   
	  		close(socketfd);
	  	  exit(EXIT_SUCCESS);
	  	}
		

}

//忽略ctrl +c 键的处理函数
void signHandler(int signNo)
{
		DEBUG("singal:%d \n",signNo);
    printf("退出聊天室请按输入 exit ，谢谢.\n");
}


void enter_get_usr( GtkWidget *widget, gpointer entry ) 
{ 
	const gchar *entry_text;
	// 获得文本内容
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry)); 
	strcpy(g_usr,entry_text);
	
	
}
void enter_get_password( GtkWidget *widget, gpointer entry ) 
{ 
	const gchar *entry_text; 
	
	// 获得文本内容
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry)); 
	strcpy(g_password,entry_text);
	
	
	
}


int main(int argc, char *argv[])
{
	 
  //GtkWidget *scrolledWindow;
 // GtkWidget *gettext;

	 GError **error;
	 GdkPixbuf *logobuf;
	 GdkPixbuf *namebuf;
	 gtk_init (&argc, &argv);
	 GtkBuilder *builder1=gtk_builder_new();
	 gtk_builder_add_from_file(builder1,"./window4.glade",NULL);
	 GtkWidget *window1 = GTK_WIDGET(gtk_builder_get_object(builder1,"window1"));
	 gtk_window_set_position (GTK_WINDOW (window1), GTK_WIN_POS_CENTER);
	 GtkImage *logo=GTK_IMAGE(gtk_builder_get_object(builder1, "logo"));
	 logobuf=gdk_pixbuf_new_from_file("/home/zr/Desktop/qt_chat/333.png",NULL );
	 GdkPixbuf *dst = gdk_pixbuf_scale_simple(logobuf,80,80,GDK_INTERP_BILINEAR);
	 gtk_image_set_from_pixbuf(logo, dst );
     g_object_unref(logobuf);
     g_object_unref(dst);
     GtkImage *name=GTK_IMAGE(gtk_builder_get_object(builder1, "name"));
	 namebuf=gdk_pixbuf_new_from_file("/home/zr/Desktop/qt_chat/222.png",NULL );
	 GdkPixbuf *sst = gdk_pixbuf_scale_simple(namebuf,80,80,GDK_INTERP_BILINEAR);
	 gtk_image_set_from_pixbuf(name, sst );
     g_object_unref(namebuf);
     g_object_unref(sst);
	 
	 GtkButton *button1 = GTK_BUTTON(gtk_builder_get_object(builder1, "Log in"));
	 GtkButton *button2 = GTK_BUTTON(gtk_builder_get_object(builder1, "send"));
	 GtkScrolledWindow *scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder1, "scroll"));
	//scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
	 GtkEntry *usr_text = GTK_ENTRY(gtk_builder_get_object(builder1,"usr_text"));
	 GtkEntry *password_text = GTK_ENTRY(gtk_builder_get_object(builder1,"password_text"));
	 
	 GtkEntry *message = GTK_ENTRY(gtk_builder_get_object(builder1,"message"));
	 GtkTextView *gettext=GTK_TEXT_VIEW(gtk_builder_get_object(builder1, "gettext"));
	 gtk_text_view_set_wrap_mode(gettext,GTK_WRAP_WORD);/*处理多行显示的模式*/
     gtk_text_view_set_justification(gettext,GTK_JUSTIFY_LEFT);/*控制文字显示方向的,对齐方式*/
     gtk_text_view_set_editable(gettext,TRUE);/*允许text view内容修改*/
     gtk_text_view_set_editable(gettext,FALSE);/*设置接收文字区不可被编辑*/
	 //gettext = gtk_text_view_new();
    get_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (gettext));
     //get_text_buffer=gtk_text_view_get_buffer(gettext);
	 
	 gtk_widget_realize (window1);
	 g_signal_connect (window1, "destroy", gtk_main_quit, NULL);
	 g_signal_connect (G_OBJECT (button1), "clicked", G_CALLBACK (enter_get_usr), usr_text);
	 g_signal_connect (G_OBJECT (button1), "clicked", G_CALLBACK (enter_get_password), password_text);
	 g_signal_connect (G_OBJECT (button1), "clicked", G_CALLBACK (inits), NULL);
	 g_signal_connect (G_OBJECT (button2), "clicked", G_CALLBACK (writedata), message);
	 
 
 // gtk_container_add(GTK_CONTAINER(scrolledWindow), gettext);
  //gtk_container_add(GTK_CONTAINER(window1),scrolledWindow);
 	gtk_widget_show_all(window1);
	gtk_main();
 	
	return 0;
}


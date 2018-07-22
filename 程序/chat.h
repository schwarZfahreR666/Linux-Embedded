#ifndef CHAT_H
#define CHAT_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
//#include<linux/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include<time.h>		 //for asctime() and time()
#include<ctype.h>    //for ispunct()
 

#define QUEUELEN     100  //max of  client
#define BUF_SIZE     256  
#define IP        "192.168.8.154"
#define PORT         1234             // port
#define FILENAME     "userinfo"       // file name for save users info£¨name:passwd:id:authority£© 
#define CLIENT_ID    1000    // client id 
//========================functions================================================
#define PUBLIC_CHAT        			1  
#define PRIVATE_CHAT       			2  
#define CLIENT_LOGIN       			3  
#define CLIENT_REGISTER    			4  
#define CLIENT_ONLINE      			5  
#define CLIENT_EXIT        			6  
#define ADMIN_KICK_CLIENT       7  
#define ADMIN_SHUTUP_CLIENT     8  
#define ADVANCED_CLIENT_TO_ADMIN   9  
#define DROP_CLIENT_TO_NORMAL    10  

//====================================================================
#define NORMAL_USER_LOGIN_FAILED  			 0 
#define NORMAL_USER_LOGIN_SUCCESS 			 1 
#define NORMAL_USER_LOGIN_FAILED_ONLINE  2 
#define ADMIN_LOGIN_SUCCESS              3 
#define NORMAL_USER_LOGIN_PASSWD_ERROR   4 

#define REGIST_FALIED   0   
#define REGIST_EXITED   1   
#define NORMAL_USER     0   
#define ADMIN_USER      1  

// ===========client attr==========================================
typedef struct{
	pthread_t tid;					
	int  sockfd;  					
	char client_name[25]; 	
	char client_passwd[25]; 
	int client_id;					
  int is_online;					
  int admin;              
}client_info;
client_info clients[QUEUELEN];

//===========send struct=====================================
 typedef struct send_info{
	int  type;						
	char id[25];  				
	char buf[BUF_SIZE]; 	
	char name[25];			
	char passwd[25];			
	}send_info;
//================function struction====================================
typedef struct {
     int  fun_flag; //function flag
     void (*fun)();// function pointer variable
}proto;
//====================debug============================

#define CHAT_DEBUG
#ifdef  CHAT_DEBUG
#define DEBUG(message...) fprintf(stderr, message)
#else
#define DEBUG(message...)
#endif

// ========fun=========client.c====================
void  print_err(char *msg);
void  reg_log();
void  register_client();
void  login();
void  inits();
void  writedata();
void  show_menu();
void  signHandler(int signNo);
void *pthread_fun(void *arg);
void parse_input_buf(char *src,  send_info  *send);
void isvalid (char   *str);

//  ======fun=======server.c======================
int 	 system_init();//
int check_online(char  *tok_file[]);//
void   connect_to_client(int socketfd );//
void   err(char *err_msg);//
int    init_clents(char  *tok_file[]);//
void   register_new_client(send_info *send,int newfd);//
void   server_check_login(send_info *send,int newfd);//
void   client_exit(send_info *send,int newfd);//
int    get_sockfd(char dest[]);
void   private_chat ( send_info *send,int newfd);//
void   public_chat (send_info *send ,int newfd);//
void   get_all_online_clients (send_info *send ,int newfd);//
void *pthreads(void *arg);//
void format_buf(char *dest,char *content,int newfd);//
void fun();//
void parse_buf(send_info *send,int newfd);//



#endif 

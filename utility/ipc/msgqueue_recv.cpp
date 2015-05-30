#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 

#define  BUFFER_SIZE        512 
 
struct message 
{ 
     long msg_type; 
     char msg_text[BUFFER_SIZE]; 
}; 
 
int main() 
{ 
     int qid; 
     key_t key; 
     struct message msg; 
     
     /*根据不同的路径和关键字产生标准的key*/ 
     if ((key = ftok(".", 'a')) == -1) 
     { 
        perror("ftok"); 
        exit(1); 
     } 
     
     /*创建消息队列*/ 
     if ((qid = msgget(key, IPC_CREAT|0666)) == -1) 
     { 
        perror("msgget"); 
        exit(1); 
     } 
     printf("Open queue %d\n", qid); 
     
     do 
     { 
        /*读取消息队列*/ 
        memset(msg.msg_text, 0, BUFFER_SIZE); 
        if (msgrcv(qid, (void*)&msg, BUFFER_SIZE, 0, 0) < 0) 
        { 
            perror("msgrcv"); 
			            exit(1); 
        } 
        printf("The message from process %d : %s", msg.msg_type, msg.msg_text);         
         
     } while(strncmp(msg.msg_text, "quit", 4)); 
         
     /*从系统内核中移走消息队列 */ 
     if ((msgctl(qid, IPC_RMID, NULL)) < 0) 
     { 
        perror("msgctl"); 
        exit(1); 
     } 
     
     exit(0); 
} 
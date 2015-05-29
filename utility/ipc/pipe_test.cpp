#include <unistd.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <assert.h>
#include <sys/wait.h>
#define MAX_DATA_LEN   256 
 
int main() 
{ 
    int pipe_fd[2]; 
    const char data[] = "Pipe Test Program"; 
     
    if (::pipe(pipe_fd) < 0)     /* 创建管道 */ 
    { 
        printf("pipe create error\n"); 
        exit(1); 
    } 
     
	pid_t pid = fork();   /* 创建子进程 */ 
	assert(pid != -1);
	int real_read, real_write; 
	if (pid == 0) 
    { 
        /* 子进程关闭写描述符，并通过使子进程暂停2s等待父进程已关闭相应的读描述符 */ 
        close(pipe_fd[1]); 
        sleep(2); 

	    char buf[MAX_DATA_LEN] = { 0 }; 
        /* 子进程读取管道内容 */ 
        if (real_read = read(pipe_fd[0], buf, MAX_DATA_LEN) > 0) 
        { 
            printf("%d bytes read from the pipe is '%s'\n", real_read, buf); 
        } 
         
        /* 关闭子进程读描述符 */ 
        close(pipe_fd[0]); 
        exit(0); 
    } 
    else if (pid > 0) 
    { 
        /* 父进程关闭读描述符，并通过使父进程暂停1s等待子进程已关闭相应的写描述符 */ 
        close(pipe_fd[0]); 
        sleep(1); 
 
        if(real_write = write(pipe_fd[1], data, strlen(data)) !=  -1) 
        { 
            printf("Parent wrote %d bytes : '%s'\n", real_write, data); 
        } 
     
        /*关闭父进程写描述符*/ 
        close(pipe_fd[1]); 
     
        /*收集子进程退出信息*/ 
        waitpid(pid, NULL, 0); 
        exit(0); 
    } 
}
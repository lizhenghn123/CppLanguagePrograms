#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define DELAY_TIME     3        /* 为了突出演示效果，等待几秒钟，*/

 /* 我们必须自己定义 semun 联合类型。 */
union semun { int val; struct semid_ds *buf; unsigned short int *array; struct seminfo *__buf; };

/* 信号量初始化（赋值）函数*/
int init_sem(int sem_id, int init_value)
{
    union semun sem_union;
    sem_union.val = init_value;     /* init_value为初始值 */
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        perror("Initialize semaphore");
        return -1;
    }
    return 0;
}
/* 从系统中删除信号量的函数 */
int del_sem(int sem_id)
{
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
    {
        perror("Delete semaphore");
        return -1;
    }
}
/* P操作函数 */
int sem_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;  /* 单个信号量的编号应该为0 */
    sem_b.sem_op = -1;  /* 表示P操作 */
    sem_b.sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        perror("P operation");
        return -1;
    }
    return 0;
}
/* V操作函数*/
int sem_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0; /* 单个信号量的编号应该为0 */
    sem_b.sem_op = 1; /* 表示V操作 */
    sem_b.sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        perror("V operation");
        return -1;
    }
    return 0;
}


int main(void)
{
    pid_t result;
    int sem_id;
    sem_id = semget(ftok(".", 'a'),  1, 0666 | IPC_CREAT); /* 创建一个信号量*/
    init_sem(sem_id, 0);
    /*调用fork()函数*/
    result = fork();
    if(result ==  -1)
    {
        perror("Fork\n");
    }
    else if (result == 0) /*返回值为0代表子进程*/
    {
        printf("Child process will wait for some seconds...\n");
        sleep(DELAY_TIME);
        printf("The returned value is %d in the child process(PID = %d)\n",
               result, getpid());
        sem_v(sem_id);
    }
    else /*返回值大于0 代表父进程*/
    {
        sem_p(sem_id);
        printf("The returned value is %d in the father process(PID = %d)\n",
               result, getpid());
        sem_v(sem_id);
        del_sem(sem_id);
    }
    exit(0);
}
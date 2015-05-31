#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "ShareMemory.h"

const char *msg = "hello, shared memory!";

int main(int argc, char *argv[])
{
    SysShareMemory sm;
    sm.create(".", 0604);
    
    //char *shms = (char*)sm.attach();

    pid_t pid = ::fork();
    assert(pid >= 0);
    if (pid > 0)   // parent
    {
        char *shms = (char*)sm.attach();
        ::memcpy(shms, msg, strlen(msg) + 1);
        printf("parent set share memory : [%s]\n", msg);
        //sm.detach();
        sleep(5);  // wait child get value
    }
    else           // child
    {
        sm.attach();
        char *shms = (char*)sm.address();
        sleep(2);   // wait parent set value
        printf("child get share memory : [%s]\n", shms);
        sm.detach();
    }

    printf("###### GAME OVER ######\n");
    
    return 0;
}

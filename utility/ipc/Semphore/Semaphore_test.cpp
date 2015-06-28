#include <stdio.h>
#include <assert.h>
#include "Semaphore.h"

void test_Semaphore()
{
    Semaphore sem(2);
    sem.acquire();
    sem.acquire();

    //sem.Acquire(); // block
    assert(!sem.tryAcquire());
    assert(!sem.timedAcquire(1000));

    sem.release();
    sem.acquire();
}
void test_NamedSemaphore()
{
    const char* sem_name = "test_NamedSemaphore";

    NamedSemaphore::unlink(sem_name);
    NamedSemaphore semaphore;
    assert(semaphore.open(sem_name) == false);
    assert(semaphore.create(sem_name, 1) == true);
    assert(semaphore.close() == true);
    assert(semaphore.openOrCreate(sem_name, 1) == true);
}

int main()
{
    test_Semaphore();
    printf("#############################\n");

    test_NamedSemaphore();
    printf("###### GAME OVER ######\n");
    return 0;
}
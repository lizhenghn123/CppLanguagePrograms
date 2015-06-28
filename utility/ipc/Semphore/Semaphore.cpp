#include "Semaphore.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

namespace internal
{
bool SemaphoreOps::tryAcquire(sem_t* sem)
{
    if (sem_trywait(sem) == 0)
        return true;
    
    return false;
}

bool SemaphoreOps::timedAcquire(sem_t* sem, int64_t timeoutMs)
{
    struct timespec ts;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t usec = tv.tv_usec + timeoutMs * 1000LL;
    ts.tv_sec = tv.tv_sec + usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    return sem_timedwait(sem, &ts) == 0;
}
} // namespace internal


NamedSemaphore::NamedSemaphore()
    : sem_(NULL)
{
}

NamedSemaphore::NamedSemaphore(const char* name, unsigned int value/* = 0*/, mode_t mode/* = 0666*/)
    : sem_(NULL)
{
    if (!openOrCreate(name, value, mode))
    {
        fprintf(stderr, "NamedSemaphore::NamedSemaphore:sem_open failure\n");
        abort();
    }
}

NamedSemaphore::~NamedSemaphore()
{
    close();
}

bool NamedSemaphore::open(const char* name)
{
    sem_t* sem = sem_open(name, 0, 0666, 0);
    if (!sem)
        return false;
    sem_ = sem;
    return true;
}

bool NamedSemaphore::create(const char* name, unsigned int value, mode_t mode)
{
    sem_t* sem = sem_open(name, O_CREAT | O_EXCL, mode, value);
    if (!sem)
        return false;
    sem_ = sem;
    return true;
}

bool NamedSemaphore::openOrCreate(const char* name, unsigned int value, mode_t mode)
{
    sem_t* sem = sem_open(name, O_CREAT, mode, value);
    if (!sem)
        return false;
    sem_ = sem;
    return true;
}

bool NamedSemaphore::close()
{
    if (sem_)
    {
        sem_close(sem_);
        sem_ = NULL;
        return true;
    }
    return false;
}

bool NamedSemaphore::unlink(const char* name)
{
    if (sem_unlink(name) == 0)
        return true;
    return false;
}

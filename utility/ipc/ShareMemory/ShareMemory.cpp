#include "ShareMemory.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>


SysShareMemory::SysShareMemory()
    : sgmId_(-1), shmAddr_(NULL)
{
}

SysShareMemory::~SysShareMemory()
{
    close();
}

bool SysShareMemory::open(const char *path, int size/* = 1*/, int mode/* = 0604*/)
{
    assert(path);

    key_t key = ::ftok(path, ::getpid());
    if (-1 == key)
    {
        perror("ftok error");
        return false;
    }

    sgmId_ = ::shmget(key, size, IPC_CREAT | mode);
    if (-1 == sgmId_)
    {
        perror("shmget error");
        return false;
    }

    return true;
}

bool SysShareMemory::create(const char *path, int size/* = 1*/, int mode/* = 0604*/)
{
    key_t key = IPC_PRIVATE;

    // 得到IPC键
    if (path != NULL)
    {
        key = ::ftok(path, ::getpid());
        if (-1 == key)
        {
            perror("SysShareMemory::create ftok error");
            return false;
        }
    }

    // 创建共享内存
    sgmId_ = ::shmget(key, size, IPC_CREAT | mode);
    if (sgmId_ == -1)
    {
        perror("SysShareMemory::create shmget error");
        return false;
    }

    return true;
}

void SysShareMemory::close()
{
    if (sgmId_ != -1)
    {
        if (::shmctl(sgmId_, IPC_RMID, NULL) == -1)
        {
            perror("SysShareMemory::close() shmctl error");
        }
        sgmId_ = -1;
    }
}

void* SysShareMemory::attach(int shmflag/* = 0*/)
{
    if (shmAddr_ == NULL)
    {
        shmAddr_ = ::shmat(sgmId_, 0, 0);
        if ((void *)-1 == shmAddr_)
        {
            perror("SysShareMemory::attach shmat error");
        }
    }

    return shmAddr_;
}

void SysShareMemory::detach()
{
    if (shmAddr_ != NULL)
    {
        ::shmdt(shmAddr_);
        shmAddr_ = NULL;
    }
}
#ifndef ZL_NAMED_SEMPHORE
#define ZL_NAMED_SEMPHORE
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>

namespace internal
{
class SemaphoreOps
{
public:
    static void acquire(sem_t* sem)
    {
        sem_wait(sem);
    }

    static bool tryAcquire(sem_t* sem);
    static bool timedAcquire(sem_t* sem, int64_t timeout);

    static void release(sem_t* sem)
    {
        sem_post(sem);
    }

    static unsigned int getValue(const sem_t* sem)
    {
        int value;
        sem_getvalue(const_cast<sem_t*>(sem), &value);
        return value;
    }
};
} // namespace internal

class Semaphore
{
public:
    explicit Semaphore(unsigned int value)
    {
        sem_init(&sem_, false, value);
    }

    ~Semaphore()
    {
        sem_destroy(&sem_);
    }

    void acquire()
    {
        internal::SemaphoreOps::acquire(&sem_);
    }

    bool tryAcquire()
    {
        return internal::SemaphoreOps::tryAcquire(&sem_);
    }

    bool timedAcquire(int64_t timeoutMs)
    {
        return internal::SemaphoreOps::timedAcquire(&sem_, timeoutMs);
    }

    void release()
    {
        return internal::SemaphoreOps::release(&sem_);
    }

    // Usually get value is only used for debug propose,
    // be careful your design if you need it.
    unsigned int getValue() const
    {
        return internal::SemaphoreOps::getValue(&sem_);
    }

private:
    //Semaphore(const Semaphore&);
    //const Semaphore& opearator= (const Semaphore&);

private:
    sem_t sem_;
};



/// named semaphone can be used for cross process sync
class NamedSemaphore
{
public:
    explicit NamedSemaphore();
    explicit NamedSemaphore(const char* name, unsigned int value = 0, mode_t mode = 0666);
    ~NamedSemaphore();

    bool open(const char* name);
    bool create(const char* name, unsigned int value, mode_t mode = 0666);
    bool openOrCreate(const char* name, unsigned int value, mode_t mode = 0666);

    bool close();

    void acquire()
    {
        internal::SemaphoreOps::acquire(sem_);
    }

    bool tryAcquire()
    {
        return internal::SemaphoreOps::tryAcquire(sem_);
    }

    bool timedAcquire(int timeoutMs) // in ms
    {
        return internal::SemaphoreOps::timedAcquire(sem_, timeoutMs);
    }

    void release()
    {
        return internal::SemaphoreOps::release(sem_);
    }

    // Usually get value is only used for debug propose,
    // be careful your design if you need it.
    unsigned int getValue() const
    {
        return internal::SemaphoreOps::getValue(sem_);
    }

    /// delete a named semaphone
    static bool unlink(const char* name);

private:
    //NamedSemaphore(const NamedSemaphore&);
    //NamedSemaphore& opearator=(const NamedSemaphore&);

private:
    sem_t* sem_;
};

#endif  /* ZL_NAMED_SEMPHORE */

#include "net/TimerQueue.h"
#include "net/Timer.h"
#include "base/StopWatch.h"

NAMESPACE_ZL_NET_START

TimerQueue::TimerQueue() : thread_(std::bind(&TimerQueue::processThread, this))
{
    running_ = true;
    timers_.clear();
}

void TimerQueue::start()
{
    if(!running_)
    {
        running_ = true;
        //pthread_t pid;
        //pthread_create(&pid, 0, ProcessThread, this);
    }
}

void TimerQueue::stop()
{
    running_ = false;
}

void TimerQueue::addTimer(Timer * timer)
{
    m_mutex.lock();
    addTimer_(timer);
    m_mutex.unlock();
}

void TimerQueue::deleteTimer(Timer * timer)
{
    m_mutex.lock();
    deleteTimer_(timer);
    m_mutex.unlock();
}

void TimerQueue::addTimer_(Timer * timer)
{
    timers_.insert(timer);
}

void TimerQueue::deleteTimer_(Timer * timer)
{
    timers_.erase(timer);
}

/*定时器延迟时间线程*/
void TimerQueue::processThread()
{
    timeval start,end;
    unsigned int delay;

    timeval tm;
    end = zl::base::StopWatch::now(); ////gettimeofday(&end,0);

    const static int DEFULT_INTERVAL = 10;
    while(running_)
    {
        tm.tv_sec=0;
        tm.tv_usec=DEFULT_INTERVAL*1000;

        while(select(0,0,0,0,&tm) < 0 && errno == EINTR);
        //int ret = select(0,0,0,0,&tm);
        
        Timestamp now(Timestamp::now());
        m_mutex.lock();
        for(std::set<Timer*>::iterator iter = timers_.begin(); iter!=timers_.end(); )
        {
            Timer *timer = *iter;
            if(timer->expires_at() < now)
            {
                timer->trigger();
                iter = timers_.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
       m_mutex.unlock();
    } // while

}

NAMESPACE_ZL_NET_END

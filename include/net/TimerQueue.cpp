#include "net/TimerQueue.h"
#include "net/Timer.h"
#include "base/StopWatch.h"

NAMESPACE_ZL_NET_START

TimerQueue::TimerQueue() : thread_(std::bind(&TimerQueue::processThread, this))
{
    running_ = true;
    timers_.clear();
}

TimerQueue::~TimerQueue()
{
    running_ = false;
    zl::thread::MutexLocker lock(m_mutex);
    timers_.clear();
}

void TimerQueue::stop()
{
    running_ = false;
}

void TimerQueue::addTimer(Timer * timer)
{
    zl::thread::MutexLocker lock(m_mutex);
    addTimerWithHold(timer);
}

void TimerQueue::deleteTimer(Timer * timer)
{
    zl::thread::MutexLocker lock(m_mutex);
    deleteTimerWithHold(timer);
}

void TimerQueue::addTimerWithHold(Timer * timer)
{
    timers_.insert(timer);
}

void TimerQueue::deleteTimerWithHold(Timer * timer)
{
    timers_.erase(timer);
}

/*定时器延迟时间线程*/
void TimerQueue::processThread()
{
    timeval tm;

    const static int DEFULT_INTERVAL = 10 * 1000;  //10ms
    while(running_)
    {
        tm.tv_sec = 0;
        tm.tv_usec = DEFULT_INTERVAL;

        while(select(0, 0, 0, 0, &tm) < 0 && errno == EINTR);
        
        Timestamp now(Timestamp::now());
        {
            zl::thread::MutexLocker lock(m_mutex);
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
        }
    }
}

NAMESPACE_ZL_NET_END

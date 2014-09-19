#ifndef ZL_LRUCACHE_H
#define ZL_LRUCACHE_H
#include <list>
#include <map>
#include "thread/Mutex.h"

template<typename Key, typename Value, typename LockType = ZL::Mutex>
class LruCache
{
public:
    explicit LruCache(size_t capacity): capacity_(capacity)
    {
    }

    ~LruCache()
    {
        Clear();
    }

    void Reset()
    {
        Clear();
    }

    bool Get(const Key& key, Value& value)
    {
        ZL::MutexLocker locker(mutex_);
        typename MAP::iterator iter = keyIndex_.find(key);
        if(iter != keyIndex_.end())
        {
            valueList_.splice(valueList_.begin(), valueList_, iter->second);
            //更新索引
            iter->second = valueList_.begin();
            value = iter->second->second;
            return true;
        }
        return false;
    }

    //同上，不过如果不存在则直接返回默认值。 TODO：可以直接将默认值插入到缓存中
    Value GetOrDefault(const Key& key, const Value& default_value = Value())
    {
        Value value;
        if(Get(key, value))
            return value;
        return default_value;
    }

    //更新cache， 如果存在则更新，否则直接存入
    bool Put(const Key& key, const Value& value)
    {
        ZL::MutexLocker locker(mutex_);
        typename MAP::iterator miter = keyIndex_.find(key);
        if(miter != keyIndex_.end()) //存在
        {
            if(miter->second->second == value)  //且相等，直接返回
                return true;
            RemoveWithHolder(key);   //先移除
        }
        //更新缓存
        valueList_.push_front(std::make_pair(key, value));
        //更新访问索引
        typename LIST::iterator liter = valueList_.begin();
        keyIndex_[key] = liter;
        //是否超载
        if(keyIndex_.size() > capacity_)
        {
            liter = valueList_.end();
            --liter;
            RemoveWithHolder(liter->first);
        }
        return true;
    }

    bool Remove(const Key& key)
    {
        ZL::MutexLocker locker(mutex_);
        return RemoveWithHolder(key);
    }

    bool HasKey(const Key& key) const
    {
        ZL::MutexLocker locker(mutex_);
        return keyIndex_.find(key) != keyIndex_.end();
    }

    size_t Size() const
    {
        ZL::MutexLocker locker(mutex_);
        return valueList_.size();
    }

    size_t Capacity() const
    {
        return capacity_;
    }

    bool IsEmpty() const
    {
        ZL::MutexLocker locker(mutex_);
        return valueList_.empty();
    }

    bool IsFull() const
    {
        ZL::MutexLocker locker(mutex_);
        return keyIndex_.size() == capacity_;
    }

private:
    void Clear()
    {
        ZL::MutexLocker locker(mutex_);
        valueList_.clear();
        keyIndex_.clear();
    }

    bool RemoveWithHolder(const Key& key)  //注意此处，以后可以改为返回remove后的value值
    {
        typename MAP::iterator iter = keyIndex_.find(key);
        if(iter == keyIndex_.end())
            return false;
        valueList_.erase(iter->second);
        keyIndex_.erase(iter);
        return true;
    }

private:
    typedef std::list<std::pair<Key, Value> >       LIST;
    typedef std::map<Key, typename LIST::iterator>  MAP;

    LIST                   valueList_;
    MAP                    keyIndex_;
    size_t                 capacity_;
    mutable LockType       mutex_;
};

#endif /* ZL_LRUCACHE_H */
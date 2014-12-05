#ifndef ZL_LRUCACHE_H
#define ZL_LRUCACHE_H
#include <list>
#include <map>
#include <hash_map>
#include "thread/Mutex.h"

template<typename Key, typename Value, typename LockType = zl::thread::Mutex>
class LruCache
{
public:
    explicit LruCache(size_t capacity): capacity_(capacity)
    {
    }

    ~LruCache()
    {
        clear();
    }

public:
    void reset()
    {
        clear();
    }

    bool get(const Key& key, Value& value)
    {
        zl::thread::MutexLocker locker(mutex_);
        typename MAP::iterator iter = keyIndex_.find(key);
        if(iter != keyIndex_.end())
        {
            valueList_.splice(valueList_.begin(), valueList_, iter->second);
            iter->second = valueList_.begin();    //更新索引

            value = iter->second->second;
            return true;
        }
        return false;
    }

    //同上，不过如果不存在则直接返回默认值。 TODO：可以直接将默认值插入到缓存中
    Value getOrDefault(const Key& key, const Value& default_value = Value())
    {
        Value value;
        if(get(key, value))
            return value;
        return default_value;
    }

    //更新cache， 如果存在则更新，否则直接存入
    bool put(const Key& key, const Value& value)
    {
        zl::thread::MutexLocker locker(mutex_);
        typename MAP::iterator miter = keyIndex_.find(key);
        if(miter != keyIndex_.end()) //存在
        {
            if(miter->second->second == value)  //且相等，直接返回
                return true;
            removeWithHolder(key);   //先移除
        }

        valueList_.push_front(std::make_pair(key, value));    //更新缓存

        typename LIST::iterator liter = valueList_.begin();
        keyIndex_[key] = liter;            //更新访问索引

        if(keyIndex_.size() > capacity_)   //是否超载
        {
            liter = valueList_.end();
            --liter;
            removeWithHolder(liter->first);
        }
        return true;
    }

    bool remove(const Key& key)
    {
        zl::thread::MutexLocker locker(mutex_);
        return removeWithHolder(key);
    }

    bool hasKey(const Key& key) const
    {
        zl::thread::MutexLocker locker(mutex_);
        return keyIndex_.find(key) != keyIndex_.end();
    }

    size_t size() const
    {
        zl::thread::MutexLocker locker(mutex_);
        return valueList_.size();
    }

    size_t capacity() const
    {
        return capacity_;
    }

    bool isEmpty() const
    {
        zl::thread::MutexLocker locker(mutex_);
        return valueList_.empty();
    }

    bool isFull() const
    {
        zl::thread::MutexLocker locker(mutex_);
        return keyIndex_.size() == capacity_;
    }

private:
    void clear()
    {
        zl::thread::MutexLocker locker(mutex_);
        valueList_.clear();
        keyIndex_.clear();
    }

    bool removeWithHolder(const Key& key)  //注意此处，以后可以改为返回remove后的value值
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
    typedef std::hash_map<Key, typename LIST::iterator>  MAP;

    LIST                   valueList_;
    MAP                    keyIndex_;
    size_t                 capacity_;
    mutable LockType       mutex_;
};

#endif /* ZL_LRUCACHE_H */
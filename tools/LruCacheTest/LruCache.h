#ifndef ZL_LRUCACHE_H
#define ZL_LRUCACHE_H
#include <list>
#include <map>
#include <hash_map>
#include "thread/Mutex.h"

template<typename Key, typename Value, class LockType = zl::thread::NullMutex>
class LruCache
{
public:
    explicit LruCache(size_t capacity) : capacity_(capacity), size_(0)
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
        zl::thread::LockGuard<LockType> lock(mutex_);
        typename MAP::iterator iter = keyIndex_.find(key);
        if(iter != keyIndex_.end())
        {
            valueList_.splice(valueList_.begin(), valueList_, iter->second);
            //iter->second = valueList_.begin();    //更新索引, 非必须
            assert(iter->second == valueList_.begin());

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
        zl::thread::LockGuard<LockType> lock(mutex_);
        typename MAP::iterator miter = keyIndex_.find(key);
        if(miter != keyIndex_.end()) //存在
        {
            // 更新了该key的访问顺序（是否需要更新应按需求来确定）
            valueList_.splice(valueList_.begin(), valueList_, miter->second);
            miter->second->second = value;
        }
        else
        {
            if (size_ < capacity_)     // insert new one
            {
                valueList_.push_front(std::make_pair(key, value));
                keyIndex_[key] = valueList_.begin();
                size_++;
            }
            else      // 删除最少访问的元素
            {
                //对双向链表list的修改绝对不可以使原有的迭代器失效，否则hash里存储的内容就失效了。
                //这里使用的是splice方法来将中间的某个结点移动到首位置。
                int oldkey = valueList_.back().first;
                LIST::iterator oldone = valueList_.end();
                --oldone;
                valueList_.splice(valueList_.begin(), valueList_, oldone);
                keyIndex_.erase(oldkey);

                // 再插入新元素到列表头
                valueList_.begin()->first = key;
                valueList_.begin()->second = value;
                keyIndex_[key] = valueList_.begin();
            }
        }

        return true;
    }

    bool remove(const Key& key)
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        return removeWithHolder(key);
    }

    bool hasKey(const Key& key) const
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        return keyIndex_.find(key) != keyIndex_.end();
    }

    size_t size() const
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        return valueList_.size();
    }

    size_t capacity() const
    {
        return capacity_;
    }

    bool isEmpty() const
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        return valueList_.empty();
    }

    bool isFull() const
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
        return keyIndex_.size() == capacity_;
    }

private:
    void clear()
    {
        zl::thread::LockGuard<LockType> lock(mutex_);
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
    size_t                 size_;
    mutable LockType       mutex_;
};

#endif /* ZL_LRUCACHE_H */
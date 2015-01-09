// ***********************************************************************
// Filename         : Profiler.h
// Author           : LIZHENG
// Created          : 2015-01-07
// Description      :
//
// Last Modified By : LIZHENG
// Last Modified On : 2015-01-07
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_PEOFILING_H
#define ZL_PEOFILING_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>
#include "base/Timestamp.h"

class Profiler;

#define PROFILER_PROFILING(s)         Profiler::ProfilerUnit entry(s)
#define PROFILER_PRODUMP              Profiler::getInstance().dump();
#define PROFILER_SET_THRESHOLD(time)  Profiler::getInstance().setTimeThreshold(time)
#define PROFILER_SET_MAXCOUNT(count)  Profiler::getInstance().setMaxCount(count)
//#define PROFILER_SET_THREADSAFE(safe) Profiler::getInstance().setThreadSafe(safe)

class Profiler
{
    class Entry;
    typedef std::multimap< int64_t, Entry, std::greater<int64_t> > EntryList;

public:
    struct Entry
    {
        explicit Entry(const std::string& token)
            : duration_(0), token_(token), start_(zl::base::Timestamp::now())
        {

        }
        int64_t duration_;
        std::string token_;
        zl::base::Timestamp start_;
    };

    class ProfilerUnit
    {
    public:
        explicit ProfilerUnit(const std::string& token)
            : entry_(token)
        {
        }

        ~ProfilerUnit()
        {
            finish();
            std::cout << "----" << entry_.token_ << "-----\n";
        }

        void finish()
        {
            if(entry_.duration_ == 0)
            {
                entry_.duration_ = zl::base::Timestamp::timediffMs(zl::base::Timestamp::now(), entry_.start_);
                Profiler::getInstance().addEntry(entry_);
            }
        }
    private:
        Entry entry_;
    };

public:
    static Profiler& getInstance()
    {
        static Profiler pro("default_profiler.log");
        return pro;
    }

    Profiler(const char* filename)
        : slowerThanMs_(10), maxListCount_(500)
    {
        assert(filename != NULL);
        ofs.open(filename);
    }

public:
    void setTimeThreshold(int64_t maxDuration)
    {
        slowerThanMs_ = maxDuration;
    }

    void setMaxCount(int64_t counts)
    {
         maxListCount_ = counts;
    }
    void addEntry(const Entry& entry)
    {
        if(entry.duration_ < slowerThanMs_)
            return;
        entryList_.insert(std::make_pair(entry.duration_, entry));
        if(entryList_.size() > maxListCount_)
        {
            dump();
            entryList_.clear();
        }
    }

    void dump()
    {
        EntryList entries;
        entries.swap(entryList_);
        dump(entries);
    }

private:
    void dump(const EntryList& entries)
    {
        for(auto iter  = entries.begin(); iter != entries.end(); ++iter)
        {
            std::cout << iter->first << "\t" << iter->second.token_ << "\t" << (iter->second).start_.toString() << "\n";
            ofs << iter->first << "\t" << iter->second.token_ << "\t" << (iter->second).start_.toString() << "\n";
            ofs.flush();
        }
        std::cout << "---------------\n";
    }

private:
    EntryList entryList_;

    int64_t slowerThanMs_;
    int64_t maxListCount_;
    std::ofstream ofs;
};

#endif  /* ZL_PEOFILING_H */
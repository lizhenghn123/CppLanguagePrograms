// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef STORAGE_LEVELDB_UTIL_HISTOGRAM_H_
#define STORAGE_LEVELDB_UTIL_HISTOGRAM_H_
#include <string>

namespace zl
{
    namespace base
    {

        class Histogram
        {
        public:
            Histogram() { clear(); }
            ~Histogram() { }

            void clear();
            void add(double value);
            void merge(const Histogram& other);

            std::string toString() const;

        private:
            double median() const;
            double percentile(double p) const;
            double average() const;
            double standardDeviation() const;

        private:
            double min_;
            double max_;
            double num_;
            double sum_;
            double sum_squares_;

            enum { kNumBuckets = 154 };
            static const double kBucketLimit[kNumBuckets];
            double buckets_[kNumBuckets];
        };

    }
}

#endif  // STORAGE_LEVELDB_UTIL_HISTOGRAM_H_

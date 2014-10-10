// ***********************************************************************
// Filename         : scoped_ptr.h
// Author           : LIZHENG
// Created          : 2014-10-10
// Description      :
// Last Modified By : LIZHENG
// Last Modified On : 2014-10-10
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_SCOPED_PTR_H
#define ZL_SCOPED_PTR_H
namespace zl
{
    /************************************************
    template<typename T> class default_deleter
    ************************************************/
    template <typename T>
    struct default_deleter
    {
        void operator()(T *p) const
        {
            delete p;
            p = NULL;
        }
    };

    template <typename T>
    struct default_deleter<T[]>
    {
        void operator()(T *p) const
        {
            delete [] p;
            p = NULL;
        }
    };


    /************************************************
    template<typename T> class scoped_ptr
    ************************************************/
    template<typename T, typename deleter = default_deleter<T> >
    class scoped_ptr
    {
    public:
        typedef T element_type;
        typedef deleter deleter_type;
    public:
        explicit scoped_ptr(T *p = NULL): ptr_(p)
        {

        }

        ~scoped_ptr()
        {
            deleter_type del;
            del(ptr_);
        }

        T *get() const
        {
            return ptr_;
        }

        void reset(T *p = NULL)
        {
            scoped_ptr<T>(p).swap(*this);
        }

        T *release()
        {
            T *p = ptr_;
            ptr_ = NULL;
            return p;
        }

        T& operator*() const
        {
            return *ptr_;
        }

        T *operator->() const
        {
            return ptr_;
        }

        operator bool() const
        {
            return (ptr_ != NULL);
        }

        void swap(scoped_ptr& sp)
        {
            T *tmp = sp.ptr_;
            sp.ptr_ = ptr_;
            ptr_ = tmp;
        }

    private:
        scoped_ptr(const scoped_ptr&);
        scoped_ptr& operator=(const scoped_ptr&);
        void operator==(const scoped_ptr&) const;
        void operator!=(const scoped_ptr&) const;

        template <typename OtherT> bool operator==(const scoped_ptr<OtherT>&) const;
        template <typename OtherT> bool operator!=(const scoped_ptr<OtherT>&) const;

    private:
        T *ptr_;
    };

    template<typename T>
    inline void swap(scoped_ptr<T>& lhs, scoped_ptr<T>& rhs)
    {
        lhs.swap(rhs);
    }

    template<typename T>
    inline T *get_pointer(scoped_ptr<T> const& sp)
    {
        return sp.get();
    }

    template <typename T>
    inline bool operator==(const T *lhs, const scoped_ptr<T>& rhs)
    {
        return lhs == rhs.get();
    }

    template <typename T>
    inline bool operator==(const scoped_ptr<T>& lhs, const T *rhs)
    {
        return rhs == lhs.get();
    }

    template <typename T>
    inline bool operator!=(const T *lhs, const scoped_ptr<T>& rhs)
    {
        return lhs != rhs.get();
    }

    template <typename T>
    inline bool operator!=(const scoped_ptr<T>& lhs, const T *rhs)
    {
        return rhs != lhs.get();
    }

    /************************************************
    template<typename T> class scoped_arr
    ************************************************/
    template<typename T, typename deleter = default_deleter<T[]> >
    class scoped_arr
    {
    public:
        typedef T element_type;
        typedef deleter deleter_type;
    public:
        explicit scoped_arr(T *p = NULL) : array_(p)
        {
        }

        ~scoped_arr()
        {
            deleter_type del;
            del(array_);
        }

        T *get() const
        {
            return array_;
        }

        T& operator[](size_t i) const
        {
            return array_[i];
        }

        void reset(T *p = NULL)
        {
            scoped_arr<T>(p).swap(*this);
        }

        T *release()
        {
            T *p = array_;
            array_ = NULL;
            return p;
        }

        operator bool() const
        {
            return (array_ != NULL);
        }

        void swap(scoped_arr& rhs)
        {
            if(this == &rhs)
                return;
            T *tmp = rhs.array_;
            rhs.array_ = array_;
            array_ = tmp;
        }

    private:
        scoped_arr(const scoped_arr&);
        scoped_arr& operator=(const scoped_arr&);
        void operator==(const scoped_arr&) const;
        void operator!=(const scoped_arr&) const;

        template <typename OtherT> bool operator==(const scoped_arr<OtherT>&) const;
        template <typename OtherT> bool operator!=(const scoped_arr<OtherT>&) const;

    private:
        T *array_;
    };

    template<typename T>
    inline void swap(scoped_arr<T>& a, scoped_arr<T>& b)
    {
        a.swap(b);
    }

    template <typename T>
    inline bool operator==(const T *lhs, const scoped_arr<T>& rhs)
    {
        return lhs == rhs.get();
    }

    template <typename T>
    inline bool operator==(const scoped_arr<T>& lhs, const T *rhs)
    {
        return rhs == lhs.get();
    }

    template <typename T>
    inline bool operator!=(const T *lhs, const scoped_arr<T>& rhs)
    {
        return lhs != rhs.get();
    }

    template <typename T>
    inline bool operator!=(const scoped_arr<T>& lhs, const T *rhs)
    {
        return rhs != lhs.get();
    }

} /* namespace zl */
#endif  /* ZL_SCOPED_PTR_H */
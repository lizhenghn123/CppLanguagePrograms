#ifndef ZL_STL_ITERATOR_H
#define ZL_STL_ITERATOR_H

namespace zl
{
    namespace stl
    {
        /// iterator category
        struct input_iterator_tag                                           {};
        struct output_iterator_tag                                          {};
        struct forward_iterator_tag :public input_iterator_tag              {};
        struct bidirectional_iterator_tag :public forward_iterator_tag      {};
        struct random_access_iterator_tag :public bidirectional_iterator_tag{};

        /// iterator
        template <class Category, class Value, class Distance = std::ptrdiff_t, class Pointer = Value*, class Reference = Value&>
        struct iterator
        {
            typedef Category                              iterator_category;
            typedef Value                                 value_type;
            typedef Distance                              difference_type;
            typedef Pointer                               pointer;
            typedef Reference                             reference;
        };

        template<class Iterator>
        struct iterator_traits
        {
            typedef typename Iterator::iterator_category  iterator_category;
            typedef typename Iterator::value_type         value_type;
            typedef typename Iterator::difference_type    difference_type;
            typedef typename Iterator::pointer            pointer;
            typedef typename Iterator::reference          reference;
        };

        template<class T>
        struct iterator_traits < T* >
        {
            typedef random_access_iterator_tag            iterator_category;
            typedef T                                     value_type;
            typedef ptrdiff_t                             difference_type;
            typedef T*                                    pointer;
            typedef T&                                    reference;
        };

        template<class T>
        struct iterator_traits < const T* >
        {
            typedef random_access_iterator_tag            iterator_category;
            typedef T                                     value_type;
            typedef ptrdiff_t                             difference_type;
            typedef const T*                              pointer;
            typedef const T&                              reference;
        };

        /// iterator adapter
        /// front_insert_iterator
        template<typename FrontInsertionSequence>
        class front_insert_iterator : public iterator<output_iterator_tag, void, void, void, void>
        {
        public:
            front_insert_iterator(FrontInsertionSequence &S):s(&S){ }
            front_insert_iterator(front_insert_iterator const&fi):s((fi.s)){}
            ~front_insert_iterator(){}
            front_insert_iterator& operator=(front_insert_iterator<FrontInsertionSequence> const& o){
                if(this == &o) return *this;
                this->s = o.s;
                return *this;
            }

            front_insert_iterator& operator*(){ return *this; }
            front_insert_iterator& operator++(){ return *this; }
            front_insert_iterator& operator++(int){ return *this; }
            front_insert_iterator& operator=(typename FrontInsertionSequence::value_type const& value){ 
                s->push_front(value);
                return *this;
            }

        private:
            FrontInsertionSequence *s;
        };

        template<typename FrontInsertionSequence>
        front_insert_iterator<FrontInsertionSequence> front_inserter(FrontInsertionSequence &S)
        {
            return front_insert_iterator<FrontInsertionSequence>(S);
        }

        /// back_insert_iterator
        template<typename BackInsertionSequence>
        class back_insert_iterator:public iterator<output_iterator_tag, void, void, void, void>
        {
		public:
			typedef back_insert_iterator<BackInsertionSequence> this_base;
			typedef typename BackInsertionSequence::value_type value_type;
        public:
            explicit back_insert_iterator(BackInsertionSequence &S):s(&S){ }
            ~back_insert_iterator(){}
			//this_base& operator=(const this_base& o)
			//{
   //             this->s = o.s;
   //             return *this;
   //         }

			this_base& operator*()      { return *this; }
			this_base& operator++()     { return *this; }
			this_base& operator++(int)  { return *this; }

			this_base& operator=(const value_type& value)
			{
                s->push_back(value);
                return *this;
            }
			this_base& operator=(value_type&& value)
			{
				s->push_back(value);
				return *this;
			}
        private:
            BackInsertionSequence *s;
        };

        template<typename BackInsertionSequence>
        back_insert_iterator<BackInsertionSequence> back_inserter(BackInsertionSequence &S)
        {
            return back_insert_iterator<BackInsertionSequence>(S);
        }

        /// insert_iterator
        template<typename Container>
        class insert_iterator: public iterator<output_iterator_tag, void, void, void, void>{
        public:
            insert_iterator(Container &C,typename Container::iterator i):s(&C),iterator(i){ }
            insert_iterator(insert_iterator const& o):s(o.s),iterator(o.iterator){}
            ~insert_iterator(){}

            insert_iterator& operator*(){ return *this; }
            insert_iterator& operator++(){ return *this; }
            insert_iterator& operator++(int){ return *this; }
            insert_iterator& operator=(typename Container::value_type const&value){ 
                s->insert(iterator,value);
                ++iterator; 
                return *this;
            }
        private:
            Container* s;
            typename Container::iterator iterator;
        };

        template<typename Container,typename Iterator>
        insert_iterator<Container> inserter(Container& C,Iterator p)
        {
            return insert_iterator<Container>(C,p);
        }

        /// reverse_iterator
        template<typename Iterator>
        class reverse_iterator:public iterator<
            typename iterator_traits<Iterator>::iterator_category,
            typename iterator_traits<Iterator>::value_type,
            typename iterator_traits<Iterator>::difference_type,
            typename iterator_traits<Iterator>::pointer,
            typename iterator_traits<Iterator>::reference
        >
        {
        public:
            typedef typename iterator_traits<Iterator>::iterator_category  iterator_category;
            typedef typename iterator_traits<Iterator>::value_type         value_type;
            typedef typename iterator_traits<Iterator>::difference_type    difference_type;
            typedef typename iterator_traits<Iterator>::pointer            pointer;
            typedef typename iterator_traits<Iterator>::reference          reference;
            typedef Iterator                                               iterator_type;
            typedef reverse_iterator<Iterator>                             self;

            explicit reverse_iterator(Iterator i):base_iterator(i){ }
            reverse_iterator():base_iterator(){}
            ~reverse_iterator(){}
            reverse_iterator(reverse_iterator<Iterator> const& o):base_iterator(o.base_iterator){}
            template<typename I>
            reverse_iterator(reverse_iterator<I>& o):base_iterator(o.base_iterator){}//主要是应用于像,iterator=>const_iterator的转换
            reverse_iterator& operator=(reverse_iterator<Iterator> const& o){ base_iterator = o.base_iterator; return *this; }
            Iterator base()const{
                return base_iterator;
            }
            reference operator*() const { 
                iterator_type tmp = base_iterator;
                return *(--tmp);
            } 

            self& operator++(){--base_iterator; return *(this); } 
            self& operator--(){++base_iterator; return *(this); } 

            self operator++(int)
            { 
                iterator_type tmp = base_iterator;
                --base_iterator;
                return self(tmp); 
            } 
            self operator--(int)
            { 
                iterator_type tmp = base_iterator;
                ++base_iterator;
                return self(tmp); 
            } 

            self operator+(difference_type n){ return self(base_iterator-n); }
            self operator-(difference_type n){ return self(base_iterator+n); }

            self& operator+=(difference_type n)
            {
                base_iterator -= n;
                return *this;
            }
            self& operator-=(difference_type n)
            {
                base_iterator += n;
                return *this;
            }

            reference operator[](difference_type n)
            {
                return *(*this+n); 
            }

            pointer operator->()
            {
                return &(operator*());  /// ?
            }
        private:
            Iterator base_iterator;
        };

        template<typename Iterator>
        bool operator==(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return (l.base() == r.base());
        }

        template<typename Iterator>
        bool operator!=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return (l.base() != r.base());
        }
        template<typename Iterator>
        bool operator<(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return (r.base() < l.base());
        }

        template<typename Iterator>
        bool operator>(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return (r<l);
        }
        template<typename Iterator>
        bool operator>=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return !(l<r);
        }
        template<typename Iterator>
        bool operator<=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r)
        {
            return !(r<l);
        }

        /// istream_iterator
        template<typename T, typename Distance = std::ptrdiff_t>
        class istream_iterator: public iterator<input_iterator_tag, T, Distance, const T *, const T & >
        {
        public:
            typedef input_iterator_tag iterator_category;
            typedef T value_type;
            typedef Distance difference_type;
            typedef const T *pointer;
            typedef const T &reference;

            istream_iterator(): stream(NULL), end_mark(false) { }
            istream_iterator(std::istream &s): stream(&s)
            {
                read();
            }

            reference operator*()
            {
                return value;
            }

            pointer operator->()
            {
                return &(operator*());
            }

            istream_iterator<T, Distance> &operator++()
            {
                this->read();
                return *this;
            }
            istream_iterator<T, Distance> operator++(int)
            {
                istream_iterator<T, Distance> tmp(*this);
                read();
                return tmp;
            }

            bool operator==(istream_iterator<T, Distance> const &o)
            {
                return ( (o.stream == this->stream && o.end_mark == this->end_mark) || (o.end_mark == false && this->end_mark == false));
            }

            bool operator!=(istream_iterator<T, Distance> const &o)
            {
                return !(*this == o);
            }

        private:
            inline void read()
            {
                end_mark = ( (*stream) ? true : false );
                if(end_mark)
                {
                    *stream >> value;
                }
                end_mark = ( (*stream) ? true : false );
            }

            std::istream *stream;
            T            value;
            bool         end_mark;
        };

        /// ostream_iterator
        template<typename T>
        class ostream_iterator: public iterator<output_iterator_tag, void, void, void, void>
        {
        public:
            typedef input_iterator_tag iterator_category;
            ostream_iterator(std::ostream &s): stream(&s), delim(NULL) {}
            ostream_iterator(std::ostream &s, const char *c): stream(&s), delim(c) {}
            ostream_iterator<T> &operator=(T const &value)
            {
                *stream << value;
                if(delim)
                    *stream << delim;
                return *this;
            }

            ostream_iterator<T> &operator*()     { return *this; }
            ostream_iterator<T> &operator++()    { return *this; }
            ostream_iterator<T> &operator++(int) { return *this; }

        private:
            std::ostream  *stream;
            const char    *delim; //分隔符
        };

        /// iterator algorithm
        //template<typename InputIterator>
        //typename iterator_traits<InputIterator>::difference_type
        //	distance(InputIterator first, InputIterator last, typename iterator_traits<InputIterator>::iterator_category input_iterator_tag)
        //{
        //	typename zl::stl::iterator_traits<InputIterator>::difference_type  x = 0;
        //	for (; first != last; ++first) ++x;
        //	return x;
        //}
        template <typename InputIterator, typename Difftype>
        void distance(InputIterator first, InputIterator last, Difftype& offlen, input_iterator_tag)
        {
            for (; first != last; ++first) ++offlen;
        }

        template <typename RandomAccessIterator, typename Difftype>
        void distance(RandomAccessIterator first, RandomAccessIterator last, Difftype& offlen, random_access_iterator_tag)
        {
            offlen += (last - first);
        }

        template<typename InputIterator>
        typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
        {
            typedef typename zl::stl::iterator_traits<InputIterator>::iterator_category category;
            typename zl::stl::iterator_traits<InputIterator>::difference_type  offlen = 0;
            zl::stl::distance<InputIterator>(first, last, offlen, category());
            return offlen;
        }

        template <class InputIterator, class Distance>
        void advance(InputIterator& i, Distance n, input_iterator_tag)
        {
            for (; n > 0; --n, ++i){}
        }

        template <class ForwardIterator, class Distance>
        inline void advance(ForwardIterator& i, Distance n, forward_iterator_tag)
        {
            advance(i, n, input_iterator_tag());
        }

        template <class BidirectionalIterator, class Distance>
        void advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
        {
            if (n >= 0)
                for (; n > 0; --n, ++i){}
            else
                for (; n < 0; ++n, ++i){}
        }

        template<class RandomAccessIterator, class Distance>
        void advance(RandomAccessIterator* i, Distance n, random_access_iterator_tag)
        {
            i += n;
        }

        template <class InputIter, class Distance>
        inline void advance(InputIter& i, Distance n)
        {
            zl::stl::advance(i, n, typename zl::stl::iterator_traits<InputIter>::iterator_category());
        }

        template <class ForwardIterator1, class ForwardIterator2>
        void iter_swap(ForwardIterator1 p, ForwardIterator2 q)
        {
            typename zl::stl::iterator_traits<ForwardIterator1>::value_type temp = *p;
            *p = *q;
            *q = temp;
        }

    }
}

#endif  /* ZL_STL_ITERATOR_H */
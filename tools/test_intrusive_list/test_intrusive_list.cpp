#include <iostream>
#include <assert.h>
#include <time.h>
#include <Windows.h>
#include "base/IntrusiveList.h"

/******************************************************************************
*
*   List tests
*
***/

namespace ListTest
{

    //=============================================================================
    struct Data
    {
        LIST_LINK(Data) forward;
        LIST_LINK(Data) reverse;
        unsigned        value;
    };

    //=============================================================================
    static void TestList()
    {
        LIST_DECLARE(Data, forward) forward;
        LIST_DECLARE(Data, reverse) reverse;

        for(unsigned j = 0; j < 100; ++j)
        {

            const unsigned COUNT = 10;
            assert(forward.Empty());
            assert(reverse.Empty());
            Data *last = NULL;
            for(unsigned i = 0; i < COUNT; ++i)
            {
                Data *data = new Data;
                data->value = i;
                forward.InsertTail(data);
                reverse.InsertHead(data);
                assert(forward.Prev(data) == last);
                assert(reverse.Next(data) == last);
                last = data;
            }
            assert(!forward.Empty());
            assert(!reverse.Empty());

            Data *f = forward.Head();
            Data *r = reverse.Head();
            for(unsigned i = 0; i < COUNT; ++i)
            {
                assert(f->value == i);
                assert(r->value == COUNT - 1 - i);
                assert(f->forward.Next() == forward.Next(f));
                assert(r->reverse.Next() == reverse.Next(r));
                f = f->forward.Next();
                r = r->reverse.Next();
            }
            assert(!f);
            assert(!r);

            f = forward.Tail();
            r = reverse.Tail();
            for(unsigned i = 0; i < COUNT; ++i)
            {
                assert(f->value == COUNT - 1 - i);
                assert(r->value == i);
                assert(f->forward.Prev() == forward.Prev(f));
                assert(r->reverse.Prev() == reverse.Prev(r));
                f = f->forward.Prev();
                r = r->reverse.Prev();
            }
            assert(!f);
            assert(!r);

            forward.UnlinkAll();
            assert(forward.Empty());

            assert(!reverse.Empty());
            reverse.DeleteAll();
            assert(reverse.Empty());
        }
    }

    //=============================================================================
    static void InsertIntoBefore(LIST_PTR(Data) list, Data *data)
    {
        Data *before = list->Head();
        while(before)
        {
            if(before->value >= data->value)
                break;
            before = list->Next(before);
        }
        list->InsertBefore(data, before);
    }

    //=============================================================================
    static void InsertIntoAfter(LIST_PTR(Data) list, Data *data)
    {
        Data *after = list->Tail();
        while(after)
        {
            if(after->value <= data->value)
                break;
            after = list->Prev(after);
        }
        list->InsertAfter(data, after);
    }

    //=============================================================================
    static void TestRandom()
    {
        srand(GetTickCount());
        for(unsigned j = 0; j < 10000; ++j)
        {
            LIST_DECLARE(Data, forward) forward;
            LIST_DECLARE(Data, reverse) reverse;

            // Insert random items sequentially
            for(unsigned i = 0; i < 20; ++i)
            {
                Data *data = new Data;
                data->value = (unsigned) rand();
                InsertIntoBefore(&forward, data);
                InsertIntoAfter(&reverse, data);
            }

            // Ensure all items were inserted in order
            for(const Data *f = forward.Head(); const Data *next = forward.Next(f); f = next)
                assert(f->value <= next->value);
            for(const Data *r = reverse.Tail(); const Data *prev = reverse.Prev(r); r = prev)
                assert(r->value >= prev->value);

            // Cleanup
            forward.DeleteAll();
            assert(reverse.Empty());
        }
    }
}   // namespace ListTest

int main()
{
    ListTest::TestList();
    ListTest::TestRandom();

    system("pause");
    return 0;
}
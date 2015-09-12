#include <iostream>
#include <set>
#include <string>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include "stl/SkipList.h"
using namespace std;
using namespace zl;
using namespace zl::stl;

// http://imtinx.iteye.com/blog/1291165
// http://blog.csdn.net/daniel_ustc/article/details/20218489?utm_source=tuicool
// http://www.cnblogs.com/xuqiang/archive/2011/05/22/2053516.html
// http://blog.csdn.net/haidao2009/article/details/8206856

#define ASSERT_TRUE(a)  assert(a)
#define ASSERT_EQ(a, b)  assert(a==b)

typedef uint64_t Key;

struct Comparator
{
    int operator()(const Key& a, const Key& b) const {
        if (a < b) {
            return -1;
        }
        else if (a > b) {
            return +1;
        }
        else {
            return 0;
        }
    }
};

class SkipTest{ };


void test_Empty()
{
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp);
    ASSERT_TRUE(!list.contains(10));

    SkipList<Key, Comparator>::Iterator iter(&list);
    ASSERT_TRUE(!iter.valid());
    iter.seekToFirst();
    ASSERT_TRUE(!iter.valid());
    iter.Seek(100);
    ASSERT_TRUE(!iter.valid());
    iter.seekToLast();
    ASSERT_TRUE(!iter.valid());
}

void test_InsertAndLookup()
{
    const int N = 2000;
    const int R = 5000;
    srand((unsigned)time(0));
    std::set<Key> keys;
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp);
    for (int i = 0; i < N; i++)
    {
        Key key = rand() % R;
        if (keys.insert(key).second)
        {
            list.insert(key);
        }
    }

    for (int i = 0; i < R; i++)
    {
        if (list.contains(i))
        {
            ASSERT_EQ(keys.count(i), 1);
        }
        else
        {
            ASSERT_EQ(keys.count(i), 0);
        }
    }

    // Simple iterator tests
  {
      SkipList<Key, Comparator>::Iterator iter(&list);
      ASSERT_TRUE(!iter.valid());

      iter.Seek(0);
      ASSERT_TRUE(iter.valid());
      ASSERT_EQ(*(keys.begin()), iter.key());

      iter.seekToFirst();
      ASSERT_TRUE(iter.valid());
      ASSERT_EQ(*(keys.begin()), iter.key());

      iter.seekToLast();
      ASSERT_TRUE(iter.valid());
      ASSERT_EQ(*(keys.rbegin()), iter.key());
  }

    // Forward iteration test
    for (int i = 0; i < R; i++)
    {
        SkipList<Key, Comparator>::Iterator iter(&list);
        iter.Seek(i);

        // Compare against model iterator
        std::set<Key>::iterator model_iter = keys.lower_bound(i);
        for (int j = 0; j < 3; j++) {
            if (model_iter == keys.end()) {
                ASSERT_TRUE(!iter.valid());
                break;
            }
            else {
                ASSERT_TRUE(iter.valid());
                ASSERT_EQ(*model_iter, iter.key());
                ++model_iter;
                iter.next();
            }
        }
    }

    // Backward iteration test
  {
      SkipList<Key, Comparator>::Iterator iter(&list);
      iter.seekToLast();

      // Compare against model iterator
      for (std::set<Key>::reverse_iterator model_iter = keys.rbegin();
          model_iter != keys.rend();
          ++model_iter) {
          ASSERT_TRUE(iter.valid());
          ASSERT_EQ(*model_iter, iter.key());
          iter.prev();
      }
      ASSERT_TRUE(!iter.valid());
  }
}

void test_DeleteNode()
{
    srand((unsigned)time(0));
    const int count = 20;
    Comparator cmp;
    SkipList<Key, Comparator> list(cmp);

    printf("### Function Test ###\n");

    printf("=== Init Skip List ===\n");
    for (int i = 0; i < count; i++)
        list.insert(i);
    
    printf("=== Print Skip List ===\n");
    cout << list.dump() << "\n";

    printf("=== Search Skip List ===\n");
    for (int i = 0; i < count; i++)
    {
        int value = rand() % (count + 10);
        cout << "found [" << value << "] \tin the list? \t" << (list.contains(value) ? "YES" : "NO") << "\n";
    }

    printf("=== Delete Skip List ===\n");
    for (int i = 0; i < count + 10; i += 2)
    {
        list.remove(i);
        cout << "delete [" << i << "], found it in the list? \t" << (list.contains(i) ? "YES" : "NO") << "\n";
    }

    std::string s = list.dump();
    cout << "\n" << s << "\n";
}

int main()
{
    test_Empty();
    test_InsertAndLookup();
    test_DeleteNode();
    system("pause");
}
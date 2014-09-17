#include <iostream>
#include <assert.h>
#include "LruCache.h"
using namespace std;

int main()
{
	{
		LruCache<int, int> cache(5);
		cache.Put(2, 3);
		cache.Put(1, 11);
		cache.Put(3, 56);
		cache.Put(4, 13);
		assert(11 == cache.GetOrDefault(1));
		assert(56 == cache.GetOrDefault(3));
		assert(3 == cache.GetOrDefault(2));
		assert(!cache.HasKey(5));
		assert(111 == cache.GetOrDefault(9, 111));
		int val;
		cache.Get(4, val);
		assert(13 == val);
	}
	{
		LruCache<int, int> cache(3);
		cache.Put(1, 1);
		cache.Put(2, 2);
		cache.Put(2, 1);
		cache.Put(3, 3);
		cache.Put(6, 4);
		assert(1 == cache.GetOrDefault(2));
		assert(3 == cache.GetOrDefault(3));
		assert(0 == cache.GetOrDefault(1));
		assert(4 == cache.GetOrDefault(6));
	}
	system("pause");
	return 0;
}
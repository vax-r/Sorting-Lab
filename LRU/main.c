#include <assert.h>
#include <stdio.h>

#include "LRU.h"

/* Check whether the first entry is the most recently used node */
bool checkLRU(LRUCache *obj, int key, int value)
{
    int hash = key % obj->capacity;
    struct hlist_node *ptr = (&obj->hhead[hash])->first;
    LRUNode *cache = list_entry(ptr, LRUNode, node);
    if (cache->key != key || cache->value != value)
        return false;

    cache = list_first_entry(&obj->dhead, LRUNode, link);
    if (cache->key != key || cache->value != value)
        return false;

    return true;
}

int main(void)
{
    int size = 5;
    LRUCache *cache = lRUCacheCreate(size);

    int test_times = 100;
    for (int i = 0; i < test_times; i++) {
        int key = rand() % 10;
        int value = rand() % 100;

        if (rand() % 2) {
            printf("put(%d, %d)\n", key, value);
            lRUCachePut(cache, key, value);
        } else {
            printf("get(%d), ", key);
            value = lRUCacheGet(cache, key);
            printf("Result: %d\n", value);
        }

        if (value >= 0)
            assert(checkLRU(cache, key, value));
    }


    return 0;
}
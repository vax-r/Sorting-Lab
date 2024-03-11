#include <stdbool.h>
#include <stdlib.h>

#include "list.h"

typedef struct {
    int capacity;
    int count;
    struct list_head dhead;
    struct hlist_head hhead[];
} LRUCache;

typedef struct {
    int key;
    int value;
    struct hlist_node node;
    struct list_head link;
} LRUNode;

static struct hlist_head *lRU_hash_slot(LRUCache *obj, int key)
{
    return obj->hhead + (key % obj->capacity);
}

LRUCache *lRUCacheCreate(int capacity)
{
    LRUCache *cache = malloc(2 * sizeof(int) + sizeof(struct list_head) +
                             capacity * sizeof(struct hlist_head));
    cache->capacity = capacity;
    cache->count = 0;
    INIT_LIST_HEAD(&cache->dhead);
    for (int i = 0; i < capacity; i++)
        INIT_HLIST_HEAD(&cache->hhead[i]);
    return cache;
}

void lRUCacheFree(LRUCache *obj)
{
    struct list_head *pos, *n;
    list_for_each_safe (pos, n, &obj->dhead) {
        LRUNode *cache = list_entry(pos, LRUNode, link);
        list_del(&cache->link);
        free(cache);
    }
    free(obj);
}

int lRUCacheGet(LRUCache *obj, int key)
{
    struct hlist_node *pos;
    hlist_for_each(pos, lRU_hash_slot(obj, key))
    {
        LRUNode *cache = list_entry(pos, LRUNode, node);
        if (cache->key == key) {
            list_move(&cache->link, &obj->dhead);
            hlist_del(&cache->node);
            hlist_add_head(&cache->node, lRU_hash_slot(obj, key));
            return cache->value;
        }
    }
    return -1;
}

void lRUCachePut(LRUCache *obj, int key, int value)
{
    LRUNode *cache = NULL;
    struct hlist_node *pos;
    hlist_for_each(pos, lRU_hash_slot(obj, key))
    {
        LRUNode *c = list_entry(pos, LRUNode, node);
        if (c->key == key) {
            hlist_del(&c->node);
            hlist_add_head(&c->node, lRU_hash_slot(obj, key));
            list_move(&c->link, &obj->dhead);
            cache = c;
        }
    }

    if (!cache) {
        if (obj->count == obj->capacity) {
            cache = list_last_entry(&obj->dhead, LRUNode, link);
            list_move(&cache->link, &obj->dhead);
            hlist_del(&cache->node);
            hlist_add_head(&cache->node, lRU_hash_slot(obj, key));
        } else {
            cache = malloc(sizeof(LRUNode));
            hlist_add_head(&cache->node, lRU_hash_slot(obj, key));
            list_add(&cache->link, &obj->dhead);
            obj->count++;
        }
        cache->key = key;
    }
    cache->value = value;
}

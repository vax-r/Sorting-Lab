#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

typedef struct __node {
    long value;
    struct list_head list;
} node_t;


/* List manipulation */
int list_length(struct list_head *head)
{
    if (!head)
        return 0;

    int size = 0;
    struct list_head *cur;
    list_for_each (cur, head)
        size++;
    return size;
}

/* Create an empty list */
struct list_head *list_new()
{
    struct list_head *new_head = malloc(sizeof(struct list_head));
    if (!new_head)
        return NULL;
    INIT_LIST_HEAD(new_head);
    return new_head;
}

bool list_insert_head(struct list_head *head, int n)
{
    if (!head)
        return false;
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return false;
    INIT_LIST_HEAD(&new_node->list);
    new_node->value = n;
    list_add(&new_node->list, head);
    return true;
}

node_t *list_remove_head(struct list_head *head)
{
    if (!head || list_empty(head))
        return NULL;

    node_t *f = list_first_entry(head, node_t, list);
    list_del(&f->list);
    return f;
}

void list_free(struct list_head *head)
{
    if (!head)
        return;
    node_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        free(entry);
    }
    free(head);
    head = NULL;
    return;
}

/* End of list manipulation */

int count_level(int n)
{
    int i = 0;
    while (n /= 10)
        i++;
    return i ? (i + 1) * 10 : 10;
}

static inline void list_replace(struct list_head *old, struct list_head *new)
{
    new->next = old->next;
    new->next->prev = new;
    new->prev = old->prev;
    new->prev->next = new;
}

static inline void list_swap(struct list_head *entry1, struct list_head *entry2)
{
    struct list_head *pos = entry2->prev;

    list_del(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1)
        pos = entry2;
    list_add(entry1, pos);
}

void random_pivot(struct list_head *head)
{
    if (!head || list_is_singular(head))
        return;
    int r = rand() % list_length(head);
    struct list_head *cur;
    list_for_each (cur, head) {
        if (r == 0)
            break;
        r--;
    }
    if (head->next != cur)
        list_swap(head->next, cur);
}

/* Quick Sort */
void quick_sort(struct list_head **head)
{
    clock_t time = clock();

    if (!(*head) || list_empty(*head))
        return;

    int n = list_length(*head);
    int i = 0;
    int max_level = count_level(n);

    struct list_head *begin[max_level];
    begin[0] = *head;
    for (int i = 1; i < max_level; i++)
        begin[i] = list_new();
    struct list_head *result = list_new();
    struct list_head *left = list_new(), *right = list_new();

    int deepest_level = i;
    while (i >= 0) {
        struct list_head *L = begin[i]->next, *R = begin[i]->prev;
        if (L != R) {
            random_pivot(begin[i]);
            node_t *pivot = list_remove_head(begin[i]);

            node_t *entry, *safe;
            list_for_each_entry_safe (entry, safe, begin[i], list) {
                list_del(&entry->list);
                if (entry->value > pivot->value) {
                    list_add(&entry->list, right);
                } else {
                    list_add(&entry->list, left);
                }
            }

            list_splice_init(left, begin[i]);
            list_add(&pivot->list, begin[i + 1]);
            list_splice_init(right, begin[i + 2]);

            i += 2;
            deepest_level = deepest_level < i ? i : deepest_level;
        } else {
            if (list_is_singular(begin[i]))
                list_splice_init(begin[i], result);
            i--;
        }
    }

    for (int i = 0; i < max_level; i++)
        list_free(begin[i]);
    list_free(left);
    list_free(right);

    *head = result;

    time = clock() - time;
    printf("size : %d, max_level : %d, deepest level : %d, cpu clock : %ld\n",
           n, max_level, deepest_level, time);
}

/* End of quick sort */


/* Verify if list is order */
static bool list_is_ordered(struct list_head *head)
{
    if (!head || list_empty(head))
        return true;

    struct list_head *cur;
    list_for_each (cur, head) {
        if (cur->next == head)
            break;
        node_t *a = list_entry(cur, node_t, list);
        node_t *b = list_entry(cur->next, node_t, list);
        if (a->value > b->value) {
            printf("%ld, %ld\n", a->value, b->value);
            return false;
        }
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void show_list(struct list_head *head)
{
    node_t *cur;
    list_for_each_entry (cur, head, list)
        printf("%ld ", cur->value);
    printf("\n");
}

int main(int argc, char **argv)
{
    for (int i = 10; i <= 10000000; i *= 10) {
        struct list_head *list = list_new();
        size_t count = i;

        int *test_arr = malloc(sizeof(int) * count);

        for (int i = 0; i < count; i++)
            test_arr[i] = i;
        shuffle(test_arr, count);
        while (count--)
            list_insert_head(list, test_arr[count]);

        quick_sort(&list);
        assert(list_is_ordered(list));
        list_free(list);

        free(test_arr);
    }

    return 0;
}

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct __node {
    struct __node *left, *right;
    struct __node *next;
    long value;
} node_t;


/* List manipulation */
void list_add(node_t **list, node_t *node_t)
{
    node_t->next = *list;
    *list = node_t;
}

node_t *list_tail(node_t **left)
{
    while ((*left) && (*left)->next)
        left = &((*left)->next);
    return *left;
}

int list_length(node_t **left)
{
    int n = 0;
    while (*left) {
        ++n;
        left = &((*left)->next);
    }
    return n;
}

node_t *list_construct(node_t *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->next = list;
    node->value = n;
    return node;
}

void list_free(node_t **list)
{
    node_t *node = (*list)->next;
    while (*list) {
        free(*list);
        *list = node;
        if (node)
            node = node->next;
    }
}

/* End of list manipulation */

int count_level(int n)
{
    int i = 0;
    while (n /= 10)
        i++;
    return i ? (i + 1) * 10 : 10;
}

void random_pivot(node_t **head)
{
    int r = rand() % list_length(head);
    node_t **cur = head;
    for (int i = 0; i < r - 1 && (*cur)->next; i++)
        cur = &((*cur)->next);

    node_t *tmp = (*cur)->next;
    (*cur)->next = (*head);
    (*head) = tmp;
}

/* Quick Sort */
void quick_sort(node_t **list)
{
    clock_t time = clock();

    int n = list_length(list);
    int value;
    int i = 0;
    int max_level = count_level(n);

    node_t *begin[max_level], *end[max_level];
    node_t *result = NULL, *left = NULL, *right = NULL;

    begin[0] = *list;
    end[0] = list_tail(list);

    int reach_i = i;
    while (i >= 0) {
        node_t *L = begin[i], *R = end[i];
        if (L != R) {
            random_pivot(&L);
            node_t *pivot = L;
            value = pivot->value;
            node_t *p = pivot->next;
            pivot->next = NULL;

            while (p) {
                node_t *n = p;
                p = p->next;
                list_add(n->value > value ? &right : &left, n);
            }

            begin[i] = left;
            end[i] = list_tail(&left);
            begin[i + 1] = pivot;
            end[i + 1] = pivot;
            begin[i + 2] = right;
            end[i + 2] = list_tail(&right);

            left = right = NULL;
            i += 2;
            reach_i = reach_i < i ? i : reach_i;
        } else {
            if (L)
                list_add(&result, L);
            i--;
        }
    }
    *list = result;

    time = clock() - time;
    printf("size : %d, max_level : %d, deepest level : %d, cpu clock : %ld\n",
           n, max_level, reach_i, time);
}

/* End of quick sort */


/* Verify if list is order */
static bool list_is_ordered(node_t *list)
{
    bool first = true;
    int value;
    while (list) {
        if (first) {
            value = list->value;
            first = false;
        } else {
            if (list->value < value)
                return false;
            value = list->value;
        }
        list = list->next;
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

void show_list(node_t **head)
{
    while (*head) {
        printf("%ld ", (*head)->value);
        head = &((*head)->next);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    node_t *list = NULL;

    for (int i = 10; i <= 10000000; i *= 10) {
        size_t count = i;

        int *test_arr = malloc(sizeof(int) * count);

        for (int i = 0; i < count; i++)
            test_arr[i] = (count - i);
        // shuffle(test_arr, count);
        while (count--)
            list = list_construct(list, test_arr[count]);

        quick_sort(&list);
        assert(list_is_ordered(list));
        list_free(&list);

        free(test_arr);
    }

    return 0;
}

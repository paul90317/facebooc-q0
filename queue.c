#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "queue.h"

size_t lg(int x)
{
    size_t cnt = -1;

    while (x) {
        ++cnt;
        x >>= 1;
    }

    return cnt;
}
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *ret = malloc(sizeof(queue_t));
    ret->head = NULL;
    ret->tail = NULL;
    ret->size = 0;
    return ret;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    element_t *now = q->head, *next;

    while (now) {
        next = now->next;
        free(now->value);
        free(now);
        now = next;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    element_t *t = malloc(sizeof(element_t));

    if (!t)
        return false;

    t->value = strdup(s);

    if (!t->value)
        return false;

    t->next = q->head;

    if (!q->head)
        q->tail = t;

    q->head = t;
    ++q->size;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    element_t *t = malloc(sizeof(element_t));

    if (!t)
        return false;

    t->value = strdup(s);

    if (!t->value)
        return false;

    t->next = NULL;

    if (q->tail) {
        q->tail->next = t;
        q->tail = t;
    } else {
        q->tail = t;
        q->head = t;
    }

    ++q->size;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    element_t *t = q->head;

    if (sp)
        strncpy(sp, t->value, bufsize - 1);

    free(t->value);
    q->head = t->next;
    free(t);
    --q->size;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
size_t q_size(queue_t *q)
{
    return q ? q->size : 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    element_t *stk = NULL, *next, *head = q->head;
    q->tail = head;

    while (head) {
        next = head->next;
        head->next = stk;
        stk = head;
        head = next;
    }

    q->head = stk;
}

element_t *better_merge(element_t *a, element_t *b)
{
    element_t *ret, **next = &ret; //一開始，下一個要修改的是 ret

    while (a && b) {
        if (strcmp(a->value, b->value) <= 0) {
            *next = a;
            a = a->next;
        } else {
            *next = b;
            b = b->next;
        }

        next = &(*next)->next; //紀錄下一個要修改的位置
    }

    *next = a ? a : b;//接上剩下的
    return ret;
}

bool can_merge(list_t *stack, size_t stack_size)
{
    return stack_size >= 2 && stack[stack_size - 2].size == stack[stack_size - 1].size;
}

void merge_sort(element_t **qhead)
{
    if (!(*qhead) || !(*qhead)->next)
        return;

    //initialize stack
    size_t stack_size = 0, qsize = 0;
    element_t *head = *qhead, *temp = head;
    list_t *stack;

    while (temp) {
        ++qsize;
        temp = temp->next;
    }

    stack = malloc((lg(qsize) + 1) * sizeof(list_t));

    //input and merge
    while (head) {
        //extract one element
        temp = head;
        head = head->next;
        temp->next = NULL;

        //input one element
        stack[stack_size].head = temp;
        stack[stack_size].size = 1;
        ++stack_size;

        //merge until can't merge anymore
        while (can_merge(stack, stack_size)) {
            stack[stack_size - 2].size += stack[stack_size - 1].size;
            stack[stack_size - 2].head = better_merge(stack[stack_size - 2].head, stack[stack_size - 1].head);
            --stack_size;
        }
    }

    //merge until remained one element
    while (stack_size >= 2) {
        stack[stack_size - 2].size += stack[stack_size - 1].size;
        stack[stack_size - 2].head = better_merge(stack[stack_size - 2].head, stack[stack_size - 1].head);
        --stack_size;
    }

    *qhead = stack[0].head;
    free(stack);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);
    element_t *walk = q->head;

    while (walk->next)
        walk = walk->next;

    q->tail = walk;
}

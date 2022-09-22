#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t* ret=malloc(sizeof(queue_t));
    ret->head=NULL;
    ret->tail=NULL;
    ret->size=0;
    return ret;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    element_t* now=q->head,*next;
    while(now){
        next=now->next;
        free(now->value);
        free(now);
        now=next;
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
    if(!q)
        return false;
    element_t* t=malloc(sizeof(element_t));
    t->next=q->head;
    t->value=strdup(s);
    if(!q->head)
        q->tail=t;
    q->head=t;
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
    if(!q)
        return false;
    element_t* t=malloc(sizeof(element_t));
    t->value=strdup(s);
    t->next=NULL;
    if(q->tail){
        q->tail->next=t;
        q->tail=t;
    }else{
        q->tail=t;
        q->head=t;
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
    if(!q||!q->head)
        return false;
    element_t* t=q->head;
    if(sp)
        strncpy(sp,t->value,bufsize-1);
    free(t->value);
    q->head=t->next;
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
    if(!q)
        return 0;
    return q->size;
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
    element_t* stk=NULL,*next,*head=q->head;
    q->tail=head;
    while(head){
        next=head->next;
        head->next=stk;
        stk=head;
        head=next;
    }
    q->head=stk;
}

//by paul90317
element_t *l_mid_last(element_t*h){
    if(!h || !h->next)
        return NULL;
    element_t* t=h;
    h=h->next->next;
    while(h&&h->next){
        t=t->next;
        h=h->next->next;
    }
    return t;
}

//by paul90317
element_t *l_tail(element_t*h){
    if(!h)
        return NULL;
    while(h->next){
        h=h->next;
    }
    return h;
}

/*
 * The function's sorting algorithm should be merge sort.
 */
void merge_sort(element_t **head)
{
    if (!(*head) || !(*head)->next)
        return;
    element_t* hh=*head;
    element_t* half=l_mid_last(hh);
    element_t* temp=half;
    half=half->next;
    temp->next=NULL;
    merge_sort(&hh);
    merge_sort(&half);
    queue_t newq={0};
    while(hh&&half){
        if(strcmp(hh->value,half->value)<=0){
            q_insert_tail(&newq,hh->value);
            temp=hh;
            hh=hh->next;
            free(temp->value);
            free(temp);
        }else{
            q_insert_tail(&newq,half->value);
            temp=half;
            half=half->next;
            free(temp->value);
            free(temp);
        }
    }
    while(hh){
        q_insert_tail(&newq,hh->value);
        temp=hh;
        hh=hh->next;
        free(temp->value);
        free(temp);
    }
    while(half){
        q_insert_tail(&newq,half->value);
        temp=half;
        half=half->next;
        free(temp->value);
        free(temp);
    }
    *head=newq.head;
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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*static void q_show(queue_t *q)
{
    for (element_t *e = q->head; e; e = e->next)
        printf("%s", e->value);
}*/

static bool validate(queue_t *q)
{
    size_t size = q->size;
    q_sort(q);
    size_t cnt = 1;

    for (element_t *e = q->head; e->next; e = e->next) {
        ++cnt;

        if (strcmp(e->value, e->next->value) > 0)
            return false;
    }

    if (cnt != size)
        return false;

    q_reverse(q);
    cnt = 1;

    for (element_t *e = q->head; e->next; e = e->next) {
        ++cnt;

        if (strcmp(e->value, e->next->value) < 0)
            return false;
    }

    if (cnt != size)
        return false;

    return true;
}


int main(int argc, char**argv)
{
    queue_t *q = q_new();
    char buf[256];

    FILE *fp = fopen("cities.txt", "r");

    if (!fp) {
        perror("failed to open cities.txt");
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, 256, fp))
        q_insert_head(q, buf);

    fclose(fp);

    element_t *curr = q->head;
    size_t test_size = argc >= 2 ? strtoull(argv[1], NULL, 10) : 0;

    for (size_t i = 0; i < test_size; curr = curr->next, ++i) {
        q_insert_tail(q, curr->value);
    }

    puts("Start!");
    assert(validate(q));
    puts("Finish!");
    q_free(q);

    return 0;
}

/* This priority queue implementation from:
 * http://rosettacode.org/wiki/Priority_queue#C, released under the GNU Free
 * Documentation License. In the Descent project, this implementation is used to
 * make monsters move in INCREASING order of speed
 */
#include "priority.h"
#include <stdio.h>
#include <stdlib.h>

/* first element in array not used to simplify indices */
pri_queue priq_new(int size) {
    pri_queue q = malloc(sizeof(pri_queue_t));

    if (size < 4) {
        size = 4;
    }

    q->buf = malloc(sizeof(q_elem_t) * (size_t) size);
    q->alloc = size;
    q->n = 1;

    return q;
}

void priq_push(pri_queue q, void *data, int pri) {
    q_elem_t *b;
    int n;
    int m;

    if (q->n >= q->alloc) {
        q->alloc *= 2;
        b = q->buf = realloc(q->buf, sizeof(q_elem_t) * (size_t) q->alloc);
    } else {
        b = q->buf;
    }

    n = q->n++;

    /* append at end, then up heap */
    while ((m = n / 2) && pri < b[m].pri) {
        b[n] = b[m];
        n = m;
    }

    b[n].data = data;
    b[n].pri = pri;
}

/* Remove top item. Returns 0 if empty. *pri can be null. */
void *priq_pop(pri_queue q, int *pri) {
    void *out;
    q_elem_t *b = q->buf;
    int n = 1;
    int m;

    if (q->n == 1) {
        return 0;
    }

    out = b[1].data;
    if (pri) {
        *pri = b[1].pri;
    }

    /* pull last item to top, then down heap. */
    --q->n;

    while ((m = n * 2) < q->n) {
        if (m + 1 < q->n && b[m].pri > b[m + 1].pri) {
            m++;
        }

        if (b[q->n].pri <= b[m].pri) {
            break;
        }

        b[n] = b[m];
        n = m;
    }

    b[n] = b[q->n];
    if (q->n < q->alloc / 2 && q->n >= 16) {
        q->buf = realloc(q->buf, (size_t)(q->alloc /= 2) * sizeof(b[0]));
    }

    return out;
}

/* get the top element without removing it from queue */
void *priq_top(pri_queue q, int *pri) {
    if (q->n == 1) {
        return 0;
    }

    if (pri) {
        *pri = q->buf[1].pri;
    }

    return q->buf[1].data;
}

/* this is O(n log n), but probably not the best */
void priq_combine(pri_queue q, pri_queue q2) {
    int i;
    q_elem_t *e = q2->buf + 1;

    for (i = q2->n - 1; i >= 1; i--, e++) {
        priq_push(q, e->data, e->pri);
    }

    priq_purge(q2);
}

#ifndef PRIORITY_H
#define PRIORITY_H

typedef struct {
    void *data;
    int pri;
} q_elem_t;
typedef struct {
    q_elem_t *buf;
    int n, alloc;
} pri_queue_t, *pri_queue;

#define priq_purge(q) (q)->n = 1
#define priq_size(q) ((q)->n - 1)

pri_queue priq_new(int size);
void priq_push(pri_queue q, void *data, int pri);
void *priq_pop(pri_queue q, int *pri);
void *priq_top(pri_queue q, int *pri);
void priq_combine(pri_queue q, pri_queue q2);
#endif

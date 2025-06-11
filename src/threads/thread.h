#ifndef MY_THREAD_H
#define MY_THREAD_H

typedef struct my_thread_s my_thread_t;

typedef void (*my_thread_fn_t)(void *params);

my_thread_t *thread_create(my_thread_fn_t *thread_fn, void *arg);

void        thread_join(my_thread_t *thread);

#endif
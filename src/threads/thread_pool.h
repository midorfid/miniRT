#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "sync.h"

typedef struct thread_pool_s thread_pool_t;

typedef void     (*thread_pool_work_t)(void *params);
typedef void     (*thread_pool_work_completion_sb_t)(int status, void *params);

thread_pool_t *     thread_pool_init(size_t num_of_threads);

void                thread_pool_destroy(thread_pool_t *tp);

int                 schedule_work(thread_pool_t *tp, thread_pool_work_t work, void *work_args, thread_pool_work_completion_sb_t completion_sb);

#endif
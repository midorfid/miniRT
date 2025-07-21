#include "thread_pool.h"
#include <sys/queue.h>
#include "thread.h"
#include "sync.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum thread_work_type_e {
    THREAD_WORK_TYPE_REGULAR = 0,
    THREAD_WORK_TYPE_STOP,
} thread_work_type_t;


typedef struct tp_work_item_s {
    thread_work_type_t                 type;

    thread_pool_work_t                  work;
    void                                *work_args;
    thread_pool_work_completion_sb_t    completion_sb;

    TAILQ_ENTRY(tp_work_item_s)         next;
} tp_work_item_t;

typedef TAILQ_HEAD(tp_work_item_queue_s, tp_work_item_s) tp_work_item_queue_t;

struct thread_pool_s {
    pthread_t                   *threads;
    size_t                      num_of_threads;

    tp_work_item_queue_t        work_queue;
    pthread_mutex_t             work_mutex;
    pthread_cond_t              work_signal;
};

static void     *thread_func(void *params);
static int      enqueue_work(thread_pool_t *tp, thread_work_type_t type, thread_pool_work_t work,
                        void *work_args, thread_pool_work_completion_sb_t completion_sb);

thread_pool_t           *thread_pool_init(size_t num_of_threads) {
    thread_pool_t       *tp = calloc(1, sizeof(thread_pool_t));
    if (tp == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    tp->threads = calloc(num_of_threads, sizeof(pthread_t));
    if (tp->threads == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    tp->num_of_threads = num_of_threads;
    if (pthread_cond_init(&tp->work_signal, NULL) != 0) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    if (pthread_mutex_init(&tp->work_mutex, NULL) != 0) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    TAILQ_INIT(&tp->work_queue);
    for (int i = 0; i < num_of_threads; ++i) { 
        if (pthread_create(tp->threads + i, NULL, thread_func, tp) != 0) {
            printf("thread_pool_init() failed");
            return NULL;
        }
    }
    return tp;
}

void                thread_pool_destroy(thread_pool_t *tp) {
    for (int i = 0; i < tp->num_of_threads; ++i) {
        enqueue_work(tp, THREAD_WORK_TYPE_STOP, NULL, NULL, NULL);
    }
    for (int i = 0; i < tp->num_of_threads; ++i) {
        pthread_join(tp->threads[i], NULL);
    }
    while (!TAILQ_EMPTY(&tp->work_queue)) {
        tp_work_item_t *item = TAILQ_FIRST(&tp->work_queue);
        // if (NULL != item->completion_sb) {
        //     item->completion_sb(1, item->work_args);
        // }
        TAILQ_REMOVE(&tp->work_queue, item, next);
        free(item);
    }
    pthread_mutex_destroy(&tp->work_mutex);
    pthread_cond_destroy(&tp->work_signal);

    free(tp);
}

int                 schedule_work(thread_pool_t *tp, thread_pool_work_t work, void *work_args, thread_pool_work_completion_sb_t completion_sb) {
    return enqueue_work(tp, THREAD_WORK_TYPE_REGULAR, work, work_args, completion_sb);
}

static void         *thread_func(void *params) {
    thread_pool_t *pool = (thread_pool_t *)params;
    if (pool == NULL) {
        printf("thread_func() failed");
        return NULL;
    }
    while (true) {
        pthread_mutex_lock(&pool->work_mutex);

        while (TAILQ_EMPTY(&pool->work_queue)) {
            int rc = pthread_cond_wait(&pool->work_signal, &pool->work_mutex);
            // if (rc == 0) {
            //     printf("thread_func() failed");
            //     return;
            // }
        }
        tp_work_item_t *new_work_item = TAILQ_FIRST(&pool->work_queue);
        
        TAILQ_REMOVE(&pool->work_queue, new_work_item, next);

        pthread_mutex_unlock(&pool->work_mutex);

        tp_work_item_t copy = *new_work_item;
        free(new_work_item);

        if (copy.type == THREAD_WORK_TYPE_STOP) {
            return NULL;
        }
        copy.work(copy.work_args);
        copy.completion_sb(0, copy.work_args);
    }
    return NULL;
}

static int          enqueue_work(thread_pool_t *tp, thread_work_type_t type, thread_pool_work_t work,
                                void *work_args, thread_pool_work_completion_sb_t completion_sb)
{
    tp_work_item_t      *new_item = calloc(1, sizeof(tp_work_item_t));
    if (new_item == NULL) {
        printf("enqueue_work() failed");
        return -1;
    }

    pthread_mutex_lock(&tp->work_mutex);

    new_item->type = type;
    new_item->work = work;
    new_item->work_args = work_args;
    new_item->completion_sb = completion_sb;

    TAILQ_INSERT_TAIL(&tp->work_queue, new_item, next);
    
    pthread_cond_signal(&tp->work_signal);

    pthread_mutex_unlock(&tp->work_mutex);

    return 0;
}

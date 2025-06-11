#include "thread_pool.h"
#include "queue.h"

typedef enum thread_work_type_s {
    THREAD_WORK_TYPE_REGULAR = 0,
    THREAD_WORK_TYPE_STOP,
} thread_work_type_t;


typedef struct tp_work_item_s {
    thread_work_type_t                 type;

    thread_pool_work_t                  work;
    void                                *work_args;
    thread_pool_work_completion_sb_t    completion_sb;

    MY_QUEUE_ENTRY(tp_work_item_s)         next;
} tp_work_item_t;

typedef MY_QUEUE_HEAD(tp_work_item_queue_s, tp_work_item_s) tp_work_item_queue_t;

typedef struct thread_pool_s {
    my_thread_t           **threads;
    size_t              num_of_threads;

    tp_work_item_queue_t    *work_queue;
    my_mutex_t              *work_mutex;
    my_cond_t               *work_signal;
};

static void     thread_func(void *params);
static int      enqueue_work(thread_pool_t *tp, thread_work_type_t type, thread_pool_work_t work,
                        void *work_args, thread_pool_work_completion_sb_t completion_sb);

thread_pool_t           *thread_pool_init(size_t num_of_threads) {
    thread_pool_t       *tp = calloc(1, sizeof(thread_pool_t));
    if (tp == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    tp->threads = calloc(num_of_threads, sizeof(my_thread_t *));
    if (tp->threads == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    tp->num_of_threads = num_of_threads;
    tp->work_signal = cond_init();
    if (tp->work_signal == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    tp->work_mutex = mutex_init();
    if (tp->work_mutex == NULL) {
        printf("thread_pool_init() failed");
        return NULL;
    }
    MY_QUEUE_INIT(&tp->work_queue);
    for (int i = 0; i < num_of_threads; ++i) {
        tp->threads[i] = thread_create();
        if (tp->threads[i] == NULL) {
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
    while (!MY_QUEUE_EMPTY(&tp->work_queue)) {
        tp_work_item_t *item = MY_QUEUE_FIRST(&tp->work_queue);
        if (NULL != item->completion_sb) {
            item->completion_sb(1, item->work_args);
        }
        MY_QUEUE_DEQUEUE(&tp->work_queue, next);
        free(item);
    }
    mutex_destroy(tp->work_mutex);
    cond_destroy(tp->work_signal);

    free(tp);
}

int                 schedule_work(thread_pool_t *tp, thread_pool_work_t work, void *work_args, thread_pool_work_completion_sb_t completion_sb) {
    return enqueue_work(tp, THREAD_WORK_TYPE_REGULAR, work, work_args, completion_sb);
}

static void         thread_func(void *params) {
    thread_pool_t *pool = (thread_pool_t *)params;
    if (pool == NULL) {
        printf("thread_func() failed");
        return;
    }
    while (true) {
        mutex_lock(pool->work_mutex);

        while (MY_QUEUE_EMPTY(&pool->work_queue)) {
            int rc = cond_wait(pool->work_signal, pool->work_mutex);
            if (rc == 0) {
                printf("thread_func() failed");
                return;
            }
        }
        tp_work_item_t *new_work_item = MY_QUEUE_FIRST(&pool->work_queue);
        
        MY_QUEUE_DEQUEUE(&pool->work_queue, next);

        mutex_unlock(pool->work_mutex);

        tp_work_item_t copy = *new_work_item;
        free(new_work_item);

        if (copy.type == THREAD_WORK_TYPE_STOP) {
            return;
        }
        copy.work(copy.work_args);
        copy.copletion_sb(0, copy.work_args);
    }
}

static int          enqueue_work(thread_pool_t *tp, thread_work_type_t type, thread_pool_work_t work,
                                void *work_args, thread_pool_work_completion_sb_t completion_sb)
{
    mutex_lock(tp->work_mutex);

    tp_work_item_t      *new_item = calloc(1, sizeof(tp_work_item_t));
    if (new_item == NULL) {
        printf("enqueue_work() failed");
        return -1;
    }
    new_item->type = type;
    new_item->work = work;
    new_item->work_args = work_args;
    new_item->completion_sb = completion_sb;

    MY_QUEUE_ENQUEUE(&tp->work_queue, new_item, next);
    
    cond_signal(tp->work_signal);

    mutex_unlock(&tp->work_mutex);

    return 0;
}

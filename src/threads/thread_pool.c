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

    QUEUE_ENTRY(tp_work_item_s)         next;
} tp_work_item_t;

typedef MY_QUEUE_HEAD(tp_work_item_queue_s, tp_work_item_s) tp_work_item_queue_t;

typedef struct thread_pool_s {
    my_thread_t           **threads;
    size_t              num_of_threads;

    tp_work_item_queue_t    *work_queue;
    my_mutex_t              *work_mutex;
    my_cond_t               *work_signal;
};

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

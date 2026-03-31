#include "thread.h"
#include "pthread.h"

struct my_thread_s {
    pthread_t   thread_handle;
};

my_thread_t     *thread_create(my_thread_fn_t *thread_fn, void *arg) {
    my_thread_t *my_thread = malloc(sizeof(my_thread_t));
    if (my_thread == NULL) {
        printf("thread_create() failed");
        return NULL;
    } 
    union {
        void (*in)(void *);
        void *(*out)(void *);
    } fn = {
        .in = thread_fn,
    };

    int rc = pthread_create(&my_thread->thread_handle, NULL, fn.out, arg);
    if (rc == 0) {
        printf("thread_create() failed");
        return NULL;
    }

    return my_thread;
}

void        thread_join(my_thread_t *thread) {
    if (thread != NULL) {
        pthread_join(&thread->thread_handle, NULL);
        free(thread);
    }
}

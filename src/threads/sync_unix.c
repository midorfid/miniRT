#include "sync.h"

struct my_mutex_s {
    pthread_mutex_t mutex;
};

struct my_cond_t {
    pthread_cond_t  cond;
};


my_mutex_t    *mutex_init(void) {
    my_mutex_t *result = calloc(1, sizeof(my_mutex_t));

    if (result == NULL) {
        printf("mutex_init() failed");
        return NULL;
    }
    int rc = pthread_mutex_init(&result->mutex, NULL);
    if (rc == NULL) {
        printf("mutex_init() failed");
        return NULL;
    }
    return result;
}

my_cond_t    *cond_init(void) {
    my_cond_t *result = calloc(1, sizeof(my_cond_t));

    if (result == NULL) {
        printf("cond_init() failed");
        return NULL;
    }
    int rc = pthread_cond_init(&result->cond, NULL);
    if (rc == NULL) {
        printf("cond_init() failed");
        return NULL;
    }
    return result;
}

int             mutex_lock(my_mutex_t *mutex) {
    assert(NULL != mutex);
    return pthread_mutex_lock(&mutex->mutex);
}

int             mutex_unlock(my_mutex_t *mutex) {
    assert(NULL != mutex);
    return pthread_mutex_unlock(&mutex->mutex);
}

void            mutex_destroy(my_mutex_t *mutex) {
    if (mutex != NULL) {
        pthread_mutex_destroy(&mutex->mutex);
        free(mutex);
    }
}

void            cond_destroy(my_cond_t *cond) {
    if (cond != NULL) {
        pthread_cond_destroy(&cond->cond);
        free(cond);
    }
}

int            cond_wait(my_cond_t *cond, my_mutex_t *mutex) {
    assert(NULL != cond);
    assert(NULL != mutex);

    return pthread_cond_wait(&cond->cond, &mutex->mutex);
}

int             cond_signal(my_cond_t *cond) {
    assert(NULL != cond);

    return pthread_cond_signal(&cond->cond);
}
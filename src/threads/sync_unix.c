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

#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

typedef struct my_mutex_s my_mutex_t;

typedef struct my_cond_s my_cond_t;

my_cond_t       *cond_init(void);

void            cond_destroy(my_cond_t *cond);

int            cond_wait(my_cond_t *cond, my_mutex_t *mutex);

int             cond_signal(my_cond_t *cond);


my_mutex_t      *mutex_init(void);

int             mutex_lock(my_mutex_t *mutex);

int             mutex_unlock(my_mutex_t *mutex);

void            mutex_destroy(my_mutex_t *mutex);

#endif
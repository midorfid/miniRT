#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>

typedef struct my_mutex_s my_mutex_t;

typedef struct my_cond_s my_cond_t;

my_cond_t    *cond_init(void);

my_mutex_t    *mutex_init(void);


#endif
#ifndef MY_THREAD_H
#define MY_THREAD_H

#if defined(__GNUC__) || defined(__clang__)
#define THREAD_LOCAL __thread
#elif defined(_MSX_VER)
#define THREAD_LOCAL __declspec(thread)
#else
#warning "Thread local storage is not supported for this compiler, there might be artifacts in the rendered image in case of multi-threaded rendering"
#endif

typedef struct my_thread_s my_thread_t;

typedef void (*my_thread_fn_t)(void *params);

my_thread_t *thread_create(my_thread_fn_t *thread_fn, void *arg);

void        thread_join(my_thread_t *thread);

#endif
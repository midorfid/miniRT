#ifndef MY_QUEUE_H
#define MY_QUEUE_H


#define MY_QUEUE_HEAD(name, type)                                                                               \
    struct name                                                                                                 \
    {                                                                                                           \
        struct type *first;                                                                                     \
        struct type **last;                                                                                     \
    }
    
#define MY_QUEUE_ENTRY(type)                                                                                    \
    struct                                                                                                      \
    {                                                                                                           \
        struct type *next; // next element                                                                      \
    }

#define MY_QUEUE_FIRST(head) ((head)->first)
#define MY_QUEUE_EMPTY(head) (MY_QUEUE_FIRST(head) == NULL)
#define MY_QUEUE_NEXT(elm, field) ((elm)->field.next)

#define MY_QUEUE_INIT(head)                                                                                     \
    do                                                                                                          \
    {                                                                                                           \
        MY_QUEUE_FIRST(head) = NULL;                                                                            \
        (head)->last = &MY_QUEUE_FIRST(head);                                                                   \
    } while (0)


#define MY_QUEUE_ENQUEUE(head, elm, field)                                                                      \
    do                                                                                                          \
    {                                                                                                           \
        MY_QUEUE_NEXT((elm), field) = NULL;                                                                     \
        *(head)->last = (elm);                                                                                  \
        (head)->last = &MY_QUEUE_NEXT((elm), field);                                                            \
    } while (0)

#define MY_QUEUE_DEQUEUE(head, field)                                                                           \
    do                                                                                                          \
    {                                                                                                           \
        if ((MY_QUEUE_FIRST((head)) = MY_QUEUE_NEXT(MY_QUEUE_FIRST((head)), field)) == NULL) {                  \
            (head)->last = &MY_QUEUE_FIRST((head));                                                             \
        }                                                                                                       \
    } while (0)

#endif
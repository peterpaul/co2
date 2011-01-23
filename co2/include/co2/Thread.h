#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include "co2/Object.h"

O_METHOD_DEF(Thread, int, run, (void *_self));
O_METHOD_DEF(Thread, void *, main, (void *_self));
O_METHOD_DEF(Thread, int, join, (void *_self));

#define ThreadClass_Attr				\
    ObjectClass_Attr;                                  \
    O_METHOD(Thread, run);                              \
    O_METHOD(Thread, main);                             \
    O_METHOD(Thread, join)


#define Thread_Attr				\
    Object_Attr;				\
    pthread_t thread

O_CLASS(Thread, Object);

#endif				/* _THREAD_H */

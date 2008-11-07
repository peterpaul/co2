#ifndef LIST_H
#define LIST_H

#include "PObject.h"
P_METHOD_DEF(PList, void *, resize, (void *_self, unsigned size));
P_METHOD_DEF(PList, void *, append, (void *_self, void *item));
P_METHOD_DEF(PList, void *, append_list, (void *_self, void *_list));
P_METHOD_DEF(PList, void *, merge, (void *_self, void *_other));
P_METHOD_DEF(PList, void *, remove, (void *self));
P_METHOD_DEF(PList, void *, get, (void *_self, unsigned index));
P_METHOD_DEF(PList, void *, set, (void *_self, unsigned index, void *item));
P_METHOD_DEF(PList, void *, map, (void *_self, void (*fun) (void *)));
P_METHOD_DEF(PList, void *, map_args, (void *_self, void (*fun) (void *, va_list *), ...));
P_METHOD_DEF(PList, void *, filter, (void *_self, int (*filter) (void *)));
P_METHOD_DEF(PList, void *, filter_args, (void *_self, int (*filter) (void *, va_list *), ...));
P_METHOD_DEF(PList, void *, destroy, (void *_self));

#define PList_Class \
PObject_Class; \
P_METHOD (PList, resize); \
P_METHOD (PList, append); \
P_METHOD (PList, append_list); \
P_METHOD (PList, merge); \
P_METHOD (PList, remove); \
P_METHOD (PList, get); \
P_METHOD (PList, set); \
P_METHOD (PList, map); \
P_METHOD (PList, map_args); \
P_METHOD (PList, filter); \
P_METHOD (PList, filter_args); \
P_METHOD (PList, destroy)

#define PList_Attr \
PObject_Attr; \
struct PObjectClass * type; \
void **data; \
unsigned length, max

P_CLASS(PList,PObject);

#endif				/* not LIST_H */

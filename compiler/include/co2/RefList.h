#ifndef REFLIST_H
#define REFLIST_H

#include "co2/RefObject.h"

O_METHOD_DEF (RefList, void *, resize, (void *_self, unsigned size));
O_METHOD_DEF (RefList, void *, append, (void *_self, void *item));
O_METHOD_DEF (RefList, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF (RefList, void *, merge, (void *_self, void *_other));
O_METHOD_DEF (RefList, void *, remove, (void *self));
O_METHOD_DEF (RefList, void *, get, (void *_self, unsigned index));
O_METHOD_DEF (RefList, void *, set,
	      (void *_self, unsigned index, void *item));
O_METHOD_DEF (RefList, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF (RefList, void *, map_args,
	      (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF (RefList, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF (RefList, void *, filter_args,
	      (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF (RefList, void *, destroy, (void *_self));
O_METHOD_DEF (RefList, void *, getIterator, (void *_self));

#define RefListClass_Attr						\
  RefObjectClass_Attr;							\
  O_METHOD (RefList, resize);						\
  O_METHOD (RefList, append);						\
  O_METHOD (RefList, append_list);					\
  O_METHOD (RefList, merge);						\
  O_METHOD (RefList, remove);						\
  O_METHOD (RefList, get);						\
  O_METHOD (RefList, set);						\
  O_METHOD (RefList, map);						\
  O_METHOD (RefList, map_args);						\
  O_METHOD (RefList, filter);						\
  O_METHOD (RefList, filter_args);					\
  O_METHOD (RefList, destroy);						\
  O_METHOD (RefList, getIterator)

#define RefList_Attr							\
  RefObject_Attr;							\
  struct Class * type;							\
  struct RefObject **data;						\
  unsigned length, max

O_CLASS (RefList, RefObject);

#endif /* not REFLIST_H */

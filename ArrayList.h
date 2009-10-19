#ifndef ArrayList_H
#define ArrayList_H

#include "Array.h"

O_METHOD_DEF(ArrayList, void *, append, (void *_self, void *item));
O_METHOD_DEF(ArrayList, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF(ArrayList, void *, merge, (void *_self, void *_other));
O_METHOD_DEF(ArrayList, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF(ArrayList, void *, map_args,
	     (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF(ArrayList, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF(ArrayList, void *, filter_args,
	     (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF(ArrayList, void *, getIterator, (void *_self));

#define ArrayListClass_Attr			\
  ArrayClass_Attr;				\
  O_METHOD(ArrayList, append);			\
  O_METHOD(ArrayList, append_list);		\
  O_METHOD(ArrayList, merge);			\
  O_METHOD(ArrayList, map);			\
  O_METHOD(ArrayList, map_args);		\
  O_METHOD(ArrayList, filter);			\
  O_METHOD(ArrayList, filter_args);		\
  O_METHOD(ArrayList, getIterator)


#define ArrayList_Attr				\
  Array_Attr;					\
  unsigned length

O_CLASS(ArrayList, Array);

#endif /* ArrayList_H */

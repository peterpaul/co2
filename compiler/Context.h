#ifndef Context_H
#define Context_H

#include "RefList.h"

O_METHOD_DEF(Context, void, add, (void *_self, void *_item));
O_METHOD_DEF(Context, void *, find, (void *_self, void *_class));
O_METHOD_DEF(Context, void, remove_last, (void *_self));

#define ContextClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(Context, add);			\
  O_METHOD(Context, find);			\
  O_METHOD(Context, remove_last)

#define Context_Attr				\
  RefObject_Attr;				\
  struct RefList * stack

O_CLASS(Context, RefObject);

extern struct Context * current_context;

#endif /* Context_H */

#ifndef RELEASEPOOLITEM_H
#define RELEASEPOOLITEM_H

#include "Object.h"

O_METHOD_DEF(ReleasePoolItem, void *, add, (void *_self, void *_item));
O_METHOD_DEF(ReleasePoolItem, void *, remove, (void *_self, void *_item));
O_METHOD_DEF(ReleasePoolItem, void, clear_list, (void *_self));

#define ReleasePoolItemClass_Attr					\
  ObjectClass_Attr;							\
  O_METHOD (ReleasePoolItem, add);					\
  O_METHOD (ReleasePoolItem, remove);					\
  O_METHOD (ReleasePoolItem, clear_list)

#define ReleasePoolItem_Attr						\
  Object_Attr;								\
  struct ReleasePoolItem *next;						\
  struct RefObject *item

O_CLASS(ReleasePoolItem, Object);

#endif				/* not RELEASEPOOLITEM_H */

#ifndef RELEASEPOOL_H
#define RELEASEPOOL_H

#include "Object.h"

O_METHOD_DEF(ReleasePool, void *, add, (void *_self, void *_item));
O_METHOD_DEF(ReleasePool, void *, remove, (void *_self, void *_item));

#define ReleasePoolClass_Attr						\
  ObjectClass_Attr;							\
  O_METHOD (ReleasePool, add);						\
  O_METHOD (ReleasePool, remove)

#define ReleasePool_Attr						\
  Object_Attr;								\
  struct ReleasePool *parent;						\
  struct ReleasePoolItem **hashmap

O_CLASS(ReleasePool, Object);

extern struct ReleasePool * current_release_pool;

#endif				/* not RELEASEPOOL_H */

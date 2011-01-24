#ifndef $1_H
#define $1_H

#include "$2.h"

//O_METHOD_DEF($1, void *, getNext, (void *_self));
//O_METHOD_DEF($1, bool, hasNext, (void *_self));

#define $1Class_Attr				\
  $2Class_Attr

#define $1_Attr					\
  $2_Attr
//  O_METHOD($1, getNext);			\
//  O_METHOD($1, hasNext)

O_CLASS($1, $2);

#endif /* $1_H */

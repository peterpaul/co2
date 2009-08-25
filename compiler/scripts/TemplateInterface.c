#include "$1.h"

#define O_SUPER $2()

//O_METHOD_IF($1, void *, getNext, (void *_self), (_self));
//O_METHOD_IF($1, bool, hasNext, (void *_self), (_self));

O_OBJECT($1, $2);
O_END_OBJECT

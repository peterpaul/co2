#include "co2/Iterator.h"

#define O_SUPER Interface()

O_METHOD_IF(Iterator, void *, getNext, (void *_self), (_self));
O_METHOD_IF(Iterator, bool, hasNext, (void *_self), (_self));

O_OBJECT(Iterator, Interface);
O_END_OBJECT

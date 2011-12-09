#include "co2/IfA.h"

#define O_SUPER Interface()

O_METHOD_IF(IfA, int, getValue, (void *_self), (_self));
O_METHOD_IF(IfA, void, setValue, (void *_self, int _value), (_self, _value));

O_OBJECT(IfA, Interface);
O_END_OBJECT

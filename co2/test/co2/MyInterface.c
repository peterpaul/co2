#include "co2/MyInterface.h"

#define O_SUPER Interface()

O_METHOD_IF(MyInterface, int, getValue, (void *_self), (_self));

O_OBJECT(MyInterface, Interface);
O_END_OBJECT

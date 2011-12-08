#ifndef IfA_H
#define IfA_H

#include "co2/Interface.h"

O_METHOD_DEF(IfA, int, getValue, (void *_self));
O_METHOD_DEF(IfA, void, setValue, (void *_self, int _value));

#define IfAClass_Attr				\
  InterfaceClass_Attr

#define IfA_Attr				\
  Interface_Attr;				\
  O_METHOD(IfA, getValue);			\
  O_METHOD(IfA, setValue)

O_CLASS(IfA, Interface);

#endif /* IfA_H */

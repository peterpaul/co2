#ifndef IfB_H
#define IfB_H

#include "co2/Interface.h"

//O_METHOD_DEF(IfB, void *, getNext, (void *_self));
//O_METHOD_DEF(IfB, bool, hasNext, (void *_self));

#define IfBClass_Attr				\
  InterfaceClass_Attr

#define IfB_Attr					\
  Interface_Attr
//  O_METHOD(IfB, getNext);			\
//  O_METHOD(IfB, hasNext)

O_CLASS(IfB, Interface);

#endif /* IfB_H */

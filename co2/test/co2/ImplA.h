#ifndef ImplA_H
#define ImplA_H

#include "co2/Object.h"
#include "co2/IfA.h"

#define ImplAClass_Attr				\
  ObjectClass_Attr;				\
  O_METHOD (IfA, getValue);			\
  O_METHOD (IfA, setValue)

#define ImplA_Attr				\
  Object_Attr;					\
  int value

O_CLASS(ImplA, Object);

#endif /* ImplA_H */

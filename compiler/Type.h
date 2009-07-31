#ifndef TYPE_H
#define TYPE_H

#include "RefObject.h"
#include "Token.h"

#define TypeClass_Attr				\
  RefObjectClass_Attr

#define Type_Attr				\
  RefObject_Attr

O_CLASS(Type, RefObject);

#endif /* TYPE_H */

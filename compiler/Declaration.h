#ifndef DECLARATION_H
#define DECLARATION_H

#include "RefObject.h"
#include "Token.h"

#define DeclarationClass_Attr			\
  RefObjectClass_Attr

#define Declaration_Attr			\
  RefObject_Attr;				\
  struct Token * token

O_CLASS(Declaration, RefObject);

#endif /* DECLARATION_H */

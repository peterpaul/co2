#ifndef CLASSDECLARATION_H
#define CLASSDECLARATION_H

#include "co2/ObjectTypeDeclaration.h"

O_METHOD_DEF (ClassDeclaration, void *, find_common_base, (void *_self, void *_other));

#define ClassDeclarationClass_Attr		\
  ObjectTypeDeclarationClass_Attr;		\
  O_METHOD (ClassDeclaration, find_common_base)

#define ClassDeclaration_Attr			\
  ObjectTypeDeclaration_Attr;			\
  struct Token * superclass;			\
  struct RefList * interfaces

O_CLASS (ClassDeclaration, ObjectTypeDeclaration);

#endif /* CLASSDECLARATION_H */

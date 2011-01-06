#ifndef TypeDeclaration_H
#define TypeDeclaration_H

#include "ObjectTypeDeclaration.h"

#define TypeDeclarationClass_Attr			\
  ObjectTypeDeclarationClass_Attr

#define TypeDeclaration_Attr				\
  ObjectTypeDeclaration_Attr;				\
  bool is_struct

O_CLASS(TypeDeclaration, ObjectTypeDeclaration);

#endif /* TypeDeclaration_H */

#ifndef GenerateSourceVisitor_H
#define GenerateSourceVisitor_H

#include "BaseCompileObjectVisitor.h"

#define GenerateSourceVisitorClass_Attr		\
  BaseCompileObjectVisitorClass_Attr

#define GenerateSourceVisitor_Attr		\
  BaseCompileObjectVisitor_Attr;		\
  FILE *fp

O_CLASS(GenerateSourceVisitor, BaseCompileObjectVisitor);

#endif /* GenerateSourceVisitor_H */

#ifndef GenerateHeaderVisitor_H
#define GenerateHeaderVisitor_H

#include "BaseCompileObjectVisitor.h"

#define GenerateHeaderVisitorClass_Attr		\
  BaseCompileObjectVisitorClass_Attr

#define GenerateHeaderVisitor_Attr		\
  BaseCompileObjectVisitor_Attr;		\
  FILE *fp

O_CLASS(GenerateHeaderVisitor, BaseCompileObjectVisitor);

#endif /* GenerateHeaderVisitor_H */

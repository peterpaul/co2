#ifndef GenerateSourceIncludesVisitor_H
#define GenerateSourceIncludesVisitor_H

#include "co2/BaseCompileObjectVisitor.h"

#define GenerateSourceIncludesVisitorClass_Attr		\
  BaseCompileObjectVisitorClass_Attr

#define GenerateSourceIncludesVisitor_Attr	\
  BaseCompileObjectVisitor_Attr;		\
  FILE *fp;					\
  struct Hash *map

O_CLASS(GenerateSourceIncludesVisitor, BaseCompileObjectVisitor);

#endif /* GenerateSourceIncludesVisitor_H */

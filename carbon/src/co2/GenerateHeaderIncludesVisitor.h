#ifndef GenerateHeaderIncludesVisitor_H
#define GenerateHeaderIncludesVisitor_H

#include "co2/BaseCompileObjectVisitor.h"

#define GenerateHeaderIncludesVisitorClass_Attr		\
  BaseCompileObjectVisitorClass_Attr

#define GenerateHeaderIncludesVisitor_Attr	\
  BaseCompileObjectVisitor_Attr;		\
  FILE *fp;					\
  struct Hash *map

O_CLASS(GenerateHeaderIncludesVisitor, BaseCompileObjectVisitor);

#endif /* GenerateHeaderIncludesVisitor_H */

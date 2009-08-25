#ifndef TOKEN_H
#define TOKEN_H

#include "CompileObject.h"
#include "String.h"

#define TokenClass_Attr				\
  CompileObjectClass_Attr

#define Token_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  int type;					\
  struct String * file;				\
  int line

O_CLASS(Token, CompileObject);

#endif /* TOKEN_H */

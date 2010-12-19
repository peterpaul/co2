#ifndef TOKEN_H
#define TOKEN_H

#include "CompileObject.h"
#include "String.h"

O_METHOD_DEF (Token, void *, ctor_from_token,
	      (void *_self, va_list * app));

#define TokenClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Token, ctor_from_token)
  

#define Token_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  int type;					\
  struct String * file;				\
  int line

O_CLASS (Token, CompileObject);

#endif /* TOKEN_H */

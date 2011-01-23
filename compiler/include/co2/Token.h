#ifndef TOKEN_H
#define TOKEN_H

#include "co2/CompileObject.h"
#include "co2/File.h"
#include "co2/String.h"
#include "co2/error.h"

O_METHOD_DEF (Token, void *, ctor_from_token,
	      (void *_self, va_list * app));

#define TokenClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Token, ctor_from_token)
  

#define Token_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  int type;					\
  struct File * file;				\
  int line

O_CLASS (Token, CompileObject);

#endif /* TOKEN_H */

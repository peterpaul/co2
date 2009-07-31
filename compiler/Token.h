#ifndef TOKEN_H
#define TOKEN_H

#include "RefObject.h"

#define TokenClass_Attr				\
  RefObjectClass_Attr

#define Token_Attr				\
  RefObject_Attr;				\
  struct String * name;				\
  int type;					\
  struct String * file;				\
  int line

#endif /* TOKEN_H */

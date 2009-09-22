#ifndef TokenGenerator_H
#define TokenGenerator_H

#include "Singleton.h"
#include "Token.h"

O_METHOD_DEF(TokenGenerator, struct Token *, create, (void *_self, struct Token * token));

#define TokenGeneratorClass_Attr			\
  SingletonClass_Attr;					\
  O_METHOD(TokenGenerator, create)

#define TokenGenerator_Attr				\
  Singleton_Attr;					\
  int value

O_CLASS(TokenGenerator, Singleton);

#endif /* TokenGenerator_H */

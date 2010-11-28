#ifndef STATEMENT_H
#define STATEMENT_H

#include "CompileObject.h"
#include "Token.h"

#define StatementClass_Attr			\
  CompileObjectClass_Attr

#define Statement_Attr				\
  CompileObject_Attr

O_CLASS (Statement, CompileObject);

#endif /* STATEMENT_H */

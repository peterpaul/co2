#ifndef STATEMENT_H
#define STATEMENT_H

#include "co2/CompileObject.h"
#include "co2/Token.h"

#define StatementClass_Attr			\
  CompileObjectClass_Attr

#define Statement_Attr				\
  CompileObject_Attr

O_CLASS (Statement, CompileObject);

#endif /* STATEMENT_H */

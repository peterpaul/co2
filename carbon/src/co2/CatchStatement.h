#ifndef CatchStatement_H
#define CatchStatement_H

#include "co2/Statement.h"

#define CatchStatementClass_Attr			\
  StatementClass_Attr

#define CatchStatement_Attr				\
  Statement_Attr;					\
  struct Scope * scope;					\
  struct ArgumentDeclaration * argument;		\
  struct Statement * body

O_CLASS(CatchStatement, Statement);

#endif /* CatchStatement_H */

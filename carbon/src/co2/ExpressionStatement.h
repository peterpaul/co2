#ifndef ExpressionStatement_H
#define ExpressionStatement_H

#include "co2/Statement.h"
#include "co2/Expression.h"

#define ExpressionStatementClass_Attr		\
  StatementClass_Attr

#define ExpressionStatement_Attr		\
  Statement_Attr;				\
  struct Expression * expr

O_CLASS (ExpressionStatement, Statement);

#endif /* ExpressionStatement_H */

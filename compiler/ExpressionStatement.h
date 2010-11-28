#ifndef ExpressionStatement_H
#define ExpressionStatement_H

#include "Statement.h"
#include "Expression.h"

#define ExpressionStatementClass_Attr		\
  StatementClass_Attr

#define ExpressionStatement_Attr		\
  Statement_Attr;				\
  struct Expression * expr

O_CLASS (ExpressionStatement, Statement);

#endif /* ExpressionStatement_H */

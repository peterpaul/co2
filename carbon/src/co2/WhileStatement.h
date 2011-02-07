#ifndef WhileStatement_H
#define WhileStatement_H

#include "co2/Statement.h"
#include "co2/Expression.h"

#define WhileStatementClass_Attr			\
  StatementClass_Attr

#define WhileStatement_Attr				\
  Statement_Attr;					\
  struct Expression * condition;			\
  struct Statement * body

O_CLASS (WhileStatement, Statement);

#endif /* WhileStatement_H */

#ifndef IfStatement_H
#define IfStatement_H

#include "co2/Statement.h"

#define IfStatementClass_Attr			\
  StatementClass_Attr

#define IfStatement_Attr				\
  Statement_Attr;					\
  struct Expression * condition;			\
  struct Statement * then_clause;			\
  struct Statement * else_clause

O_CLASS (IfStatement, Statement);

#endif /* IfStatement_H */

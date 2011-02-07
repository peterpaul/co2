#ifndef DoStatement_H
#define DoStatement_H

#include "co2/Statement.h"
#include "co2/Expression.h"

#define DoStatementClass_Attr			\
  StatementClass_Attr

#define DoStatement_Attr				\
  Statement_Attr;					\
  struct Expression * condition;			\
  struct Statement * body

O_CLASS (DoStatement, Statement);

#endif /* DoStatement_H */

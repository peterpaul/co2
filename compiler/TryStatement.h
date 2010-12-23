#ifndef TryStatement_H
#define TryStatement_H

#include "Statement.h"

#define TryStatementClass_Attr			\
  StatementClass_Attr

#define TryStatement_Attr			\
  Statement_Attr;				\
  struct Statement * try_clause;		\
  struct RefList * catch_clause;		\
  struct Statement * finally_clause

O_CLASS(TryStatement, Statement);

#endif /* TryStatement_H */

#ifndef ForStatement_H
#define ForStatement_H

#include "Statement.h"

#define ForStatementClass_Attr			\
  StatementClass_Attr

#define ForStatement_Attr				\
  Statement_Attr;					\
  struct Expression * initialization;			\
  struct Expression * condition;			\
  struct Expression * iteration;			\
  struct Statement * body

O_CLASS (ForStatement, Statement);

#endif /* ForStatement_H */

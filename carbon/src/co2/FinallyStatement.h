#ifndef FinallyStatement_H
#define FinallyStatement_H

#include "Statement.h"

#define FinallyStatementClass_Attr		\
  StatementClass_Attr

#define FinallyStatement_Attr			\
  Statement_Attr;				\
  struct Statement * body

O_CLASS(FinallyStatement, Statement);

#endif /* FinallyStatement_H */

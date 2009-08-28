#ifndef ReturnStatement_H
#define ReturnStatement_H

#include "Statement.h"
#include "Expression.h"

#define ReturnStatementClass_Attr			\
  StatementClass_Attr

#define ReturnStatement_Attr				\
  Statement_Attr;					\
  struct Expression *expr

O_CLASS(ReturnStatement, Statement);

#endif /* ReturnStatement_H */

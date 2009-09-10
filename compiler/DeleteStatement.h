#ifndef DeleteStatement_H
#define DeleteStatement_H

#include "Statement.h"

#define DeleteStatementClass_Attr			\
  StatementClass_Attr

#define DeleteStatement_Attr				\
  Statement_Attr;					\
  struct Expression * expr

O_CLASS(DeleteStatement, Statement);

#endif /* DeleteStatement_H */

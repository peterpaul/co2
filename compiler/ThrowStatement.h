#ifndef ThrowStatement_H
#define ThrowStatement_H

#include "Statement.h"
#include "Expression.h"

#define ThrowStatementClass_Attr			\
  StatementClass_Attr

#define ThrowStatement_Attr				\
  Statement_Attr;					\
  struct Expression * expr;				\
  struct CatchStatement * catch_context;		\
  struct TryStatement * try_context

O_CLASS(ThrowStatement, Statement);

#endif /* ThrowStatement_H */

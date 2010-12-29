#ifndef ReturnStatement_H
#define ReturnStatement_H

#include "Statement.h"
#include "Expression.h"
#include "FunctionDeclaration.h"

#define ReturnStatementClass_Attr			\
  StatementClass_Attr

#define ReturnStatement_Attr				\
  Statement_Attr;					\
  struct Expression *expr;				\
  struct FunctionDeclaration *function_context;		\
  struct TryStatement *try_context;			\
  struct CatchStatement *catch_context

O_CLASS (ReturnStatement, Statement);

#endif /* ReturnStatement_H */

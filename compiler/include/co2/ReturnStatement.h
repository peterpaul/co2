#ifndef ReturnStatement_H
#define ReturnStatement_H

#include "co2/Statement.h"
#include "co2/Expression.h"
#include "co2/FunctionDeclaration.h"

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

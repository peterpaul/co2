#ifndef STATEMENT_H
#define STATEMENT_H

#include "RefObject.h"
#include "Token.h"

#define StatementClass_Attr			\
  RefObjectClass_Attr

#define Statement_Attr				\
  RefObject_Attr

O_CLASS(Statement, RefObject);

#endif /* STATEMENT_H */

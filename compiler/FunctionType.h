#ifndef FunctionType_H
#define FunctionType_H

#include "Type.h"
#include "RefList.h"

O_METHOD_DEF (FunctionType, void *, ctor_from_decl,
	      (void *_self, va_list * app));
O_METHOD_DEF (FunctionType, void *, ctor_from_expr,
	      (void *_self, va_list * app));

#define FunctionTypeClass_Attr			\
  TypeClass_Attr;				\
  O_METHOD(FunctionType, ctor_from_decl);	\
  O_METHOD(FunctionType, ctor_from_expr)

#define FunctionType_Attr			\
  Type_Attr;					\
  bool has_var_args;				\
  struct Type * return_type;			\
  struct RefList * parameters;			\
  struct Token * generated_name

O_CLASS (FunctionType, Type);

#endif /* FunctionType_H */

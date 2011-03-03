#ifndef FunctionType_H
#define FunctionType_H

#include "co2/Type.h"
#include "co2/RefList.h"

O_METHOD_DEF (FunctionType, void *, ctor_from_decl,
	      (void *_self, va_list * app));
O_METHOD_DEF (FunctionType, void *, ctor_from_expr,
	      (void *_self, va_list * app));
O_METHOD_DEF (FunctionType, void, generate_named, 
	      (void *_self, struct Token *name));
#define FunctionTypeClass_Attr			\
  TypeClass_Attr;				\
  O_METHOD(FunctionType, ctor_from_decl);	\
  O_METHOD(FunctionType, ctor_from_expr);	\
  O_METHOD(FunctionType, generate_named)

#define FunctionType_Attr			\
  Type_Attr;					\
  bool has_var_args;				\
  struct Type * return_type;			\
  struct RefList * parameters;			\
  struct Token * generated_name

O_CLASS (FunctionType, Type);

#endif /* FunctionType_H */

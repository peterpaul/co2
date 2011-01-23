#ifndef CompileObject_H
#define CompileObject_H

#include "co2/RefObject.h"
#include "co2/Context.h"
#include "co2/BaseCompileObjectVisitor.h"

O_METHOD_DEF (CompileObject, void, type_check, (void *_self));
O_METHOD_DEF (CompileObject, void, optimize, (void *_self));
O_METHOD_DEF (CompileObject, void, generate, (void *_self));
O_METHOD_DEF (CompileObject, struct Token *, get_token, (void *_self));
O_METHOD_DEF (CompileObject, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor));

#define CompileObjectClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(CompileObject, generate);		\
  O_METHOD(CompileObject, optimize);		\
  O_METHOD(CompileObject, type_check);		\
  O_METHOD(CompileObject, get_token);		\
  O_METHOD(CompileObject, accept)

#define CompileObject_Attr			\
  RefObject_Attr

O_CLASS (CompileObject, RefObject);

int type_filter (void *_member, va_list * app);
int not_type_filter (void *_member, va_list * app);
void accept (void *_object, va_list *app);

#endif /* CompileObject_H */

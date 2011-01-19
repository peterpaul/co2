#ifndef Declaration_H
#define Declaration_H

#include "CompileObject.h"
#include "Scope.h"

O_METHOD_DEF (Declaration, void, set_scope, (void *_self, void *scope));
O_METHOD_DEF (Declaration, void, set_class_decl,
	      (void *_self, void *_class_decl));
O_METHOD_DEF (Declaration, void, set_include_file,
	      (void *_self, void *_include_file));

#define DeclarationClass_Attr			\
  CompileObjectClass_Attr;			\
  O_METHOD(Declaration, set_scope);		\
  O_METHOD(Declaration, set_class_decl);	\
  O_METHOD(Declaration, set_include_file)

#define Declaration_Attr			\
  CompileObject_Attr;				\
  struct Token * name;				\
  struct Scope * scope;				\
  struct Type * type;				\
  struct Token * include_file;			\
  struct ClassDeclaration * class_decl;		\
  bool declared;				\
  bool defined

O_CLASS (Declaration, CompileObject);

void Declaration_list_set_include_header (void *_decl, va_list * app);
void Declaration_list_type_check (void *_decl);
int Declaration_new_member_filter (void *_member, va_list * app);

#endif /* Declaration_H */

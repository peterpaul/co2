#include "ClassDeclaration.h"
#include "FunDeclaration.h"
#include "ArgDeclaration.h"
#include "VarDeclaration.h"
#include "Type.h"
#include "Token.h"
#include "RefList.h"
#include "FunctionType.h"
#include "io.h"
#include "Statement.h"
#include "Scope.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ClassDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  self = O_SUPER->ctor(self, app);
  self->superclass = O_BRANCH_CAST(va_arg(*app, struct Type *), Token());
  O_BRANCH_CALL(self->superclass, retain);
  self->interfaces = O_BRANCH_CAST(va_arg(*app, struct RefList *), RefList());
  O_BRANCH_CALL(self->interfaces, retain);
  return self;
}

O_IMPLEMENT(ClassDeclaration, void *, dtor, (void *_self), (_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  O_BRANCH_CALL(self->superclass, release);
  O_BRANCH_CALL(self->interfaces, release);
  O_BRANCH_CALL(self->members, release);
}

static int member_filter(void *_member, va_list * app)
{
  return o_is_of(_member, va_arg(*app, void *));
}

static int new_method_filter(void *_method)
{
  struct FunDeclaration * method = O_CAST(_method, FunDeclaration());
  return O_BRANCH_CALL(method->scope->parent, lookup_in_this_scope, method->name) == NULL;
}

static void ClassDeclaration_generate_method_arguments(void *_arg)
{
  struct ArgDeclaration * arg = O_CAST(_arg, ArgDeclaration());
  fprintf(out, ", ");
  O_CALL(arg, generate);
}

static void ClassDeclaration_generate_method_argument_list(void *_arg)
{
  struct ArgDeclaration * arg = O_CAST(_arg, ArgDeclaration());
  fprintf(out, ", ");
  O_CALL(arg->name, generate);
}

static void ClassDeclaration_generate_method_definition(void *_method_decl, va_list * app)
{
  struct FunDeclaration * method_decl = O_CAST(_method_decl, FunDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  struct FunctionType * method_type = o_cast(method_decl->type, FunctionType());
  fprintf(out, "O_METHOD_DEF (");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_type->return_type, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ", (void *_self");
  O_CALL(method_decl->formal_arguments, map, ClassDeclaration_generate_method_arguments);
  fprintf(out, "));\n");
}

static void ClassDeclaration_generate_method_registration(void *_method_decl, va_list *app)
{
  struct FunDeclaration * method_decl = O_CAST(_method_decl, FunDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "; \\\n O_METHOD (");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ")");
}

static void ClassDeclaration_generate_method_registration_2(void *_method_decl, va_list *app)
{
  struct FunDeclaration * method_decl = O_CAST(_method_decl, FunDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_OBJECT_METHOD (");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ");\n");
}

static void ClassDeclaration_generate_method_implementation(void *_method_decl, va_list *app)
{
  struct FunDeclaration * method_decl = O_CAST(_method_decl, FunDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  struct FunctionType * method_type = o_cast(method_decl->type, FunctionType());
  fprintf(out, "O_IMPLEMENT(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_type->return_type, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ", (void *_self");
  O_CALL(method_decl->formal_arguments, map, ClassDeclaration_generate_method_arguments);
  fprintf(out, "), (_self");
  O_CALL(method_decl->formal_arguments, map, ClassDeclaration_generate_method_argument_list);
  fprintf(out, "))\n");
  fprintf(out, "{\n");
  fprintf(out, "struct ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "* self = O_CAST(_self, ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "());\n");
  O_CALL(method_decl->body, generate);
  fprintf(out, "}\n\n");
}

static void ClassDeclaration_generate_attribute_registration(void *_method_decl, va_list *app)
{
  struct VarDeclaration * method_decl = O_CAST(_method_decl, VarDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "; \\\n ");
  O_CALL(method_decl->type, generate);
  fprintf(out, " ");
  O_CALL(method_decl->name, generate);
}

static void generate_superclass(struct ClassDeclaration * self)
{
  if (self->superclass)
    {
      O_CALL(self->superclass, generate);
    }
  else
    {
      fprintf(out, "Object");
    }
}

O_IMPLEMENT(ClassDeclaration, void, generate, (void *_self), (_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  /* filter the members */
  struct RefList * attributes = O_CALL(self->members, filter_args, member_filter, VarDeclaration());
  O_CALL(attributes, retain);
  struct RefList * methods = O_CALL(self->members, filter_args, member_filter, FunDeclaration());
  O_CALL(methods, retain);
  struct RefList * new_methods = O_CALL(methods, filter, new_method_filter);
  O_CALL(new_methods, retain);

  /* generate the class */
  // TODO Only generate new methods, not those inherited from superclass.
  O_CALL(new_methods, map_args, ClassDeclaration_generate_method_definition, self);
  fprintf(out, "\n");

  fprintf(out, "#define ");
  O_CALL(self->name, generate);
  fprintf(out, "Class_Attr\\\n ");
  generate_superclass(self);
  fprintf(out, "Class_Attr");
  O_CALL(new_methods, map_args, ClassDeclaration_generate_method_registration, self);
  fprintf(out, "\n\n");

  fprintf(out, "#define ");
  O_CALL(self->name, generate);
  fprintf(out, "_Attr\\\n ");
  generate_superclass(self);
  fprintf(out, "_Attr");
  O_CALL(attributes, map_args, ClassDeclaration_generate_attribute_registration, self);
  fprintf(out, "\n\n");

  fprintf(out, "O_CLASS(");
  O_CALL(self->name, generate);
  fprintf(out, ", ");
  generate_superclass(self);
  fprintf(out, ");\n\n");

  fprintf(out, "#define O_SUPER ");
  generate_superclass(self);
  fprintf(out, "()\n\n");

  O_CALL(methods, map_args, ClassDeclaration_generate_method_implementation, self);

  fprintf(out, "O_OBJECT(");
  O_CALL(self->name, generate);
  fprintf(out, ", ");
  generate_superclass(self);
  fprintf(out, ");\n");

  O_CALL(methods, map_args, ClassDeclaration_generate_method_registration_2, self);

  fprintf(out, "O_END_OBJECT\n\n");

  fprintf(out, "#undef O_SUPER\n\n");

  O_CALL(attributes, release);
  O_CALL(methods, release);
}

static void ClassDeclaration_type_check_members(void *_member)
{
  struct Declaration * member = O_CAST(_member, Declaration());
  O_CALL(member, type_check);
}

O_IMPLEMENT(ClassDeclaration, void, type_check, (void *_self), (_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  if (self->superclass)
    {
      struct ClassDeclaration * super = O_CALL(self->scope, lookup, self->superclass);
      if (super)
	{
	  O_CAST(super, ClassDeclaration());
	  self->member_scope->parent = super->member_scope;
	}
    }
  O_CALL(self->members, map, ClassDeclaration_type_check_members);
}

O_OBJECT(ClassDeclaration, Declaration);
O_OBJECT_METHOD(ClassDeclaration, ctor);
O_OBJECT_METHOD(ClassDeclaration, dtor);
O_OBJECT_METHOD(ClassDeclaration, generate);
O_OBJECT_METHOD(ClassDeclaration, type_check);
O_END_OBJECT

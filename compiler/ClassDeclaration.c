#include "ClassDeclaration.h"
#include "FunctionDeclaration.h"
#include "ArgumentDeclaration.h"
#include "VariableDeclaration.h"
#include "ConstructorDeclaration.h"
#include "DestructorDeclaration.h"
#include "Type.h"
#include "ObjectType.h"
#include "Token.h"
#include "RefList.h"
#include "FunctionType.h"
#include "io.h"
#include "Statement.h"
#include "Scope.h"
#include "PrimitiveType.h"
#include "grammar.tab.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ClassDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  self = O_SUPER->ctor(self, app);
  self->superclass = O_BRANCH_CAST(va_arg(*app, struct Type *), Token());
  O_BRANCH_CALL(self->superclass, retain);
  self->interfaces = O_BRANCH_CAST(va_arg(*app, struct RefList *), RefList());
  O_BRANCH_CALL(self->interfaces, retain);
  return self;
}

O_IMPLEMENT(ClassDeclaration, void *, dtor, (void *_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  O_BRANCH_CALL(self->superclass, release);
  O_BRANCH_CALL(self->interfaces, release);
  O_BRANCH_CALL(self->members, release);
  O_CALL(self->member_scope, delete);
}

static int new_constructor_filter(void *_constructor)
{
  struct ConstructorDeclaration * constructor = O_CAST(_constructor, ConstructorDeclaration());
  if (strcmp(constructor->name->name->data, "ctor") != 0)
    {
      return O_BRANCH_CALL(constructor->scope->parent, lookup_type_in_this_scope, constructor->name, ConstructorDeclaration()) == NULL;
    }
  return false;
}

static void ClassDeclaration_generate_constructor_arguments(void *_arg)
{
  struct ArgumentDeclaration * arg = O_CAST(_arg, ArgumentDeclaration());
  O_CALL(arg, generate);
  if (o_is_a(arg->type, ObjectType())) {
    struct ObjectType * type = (struct ObjectType *) arg->type;
    fprintf(out, "=O_BRANCH_CAST(va_arg(*app, ");
    O_CALL(type, generate);
    fprintf(out, "), ");
    O_CALL(type->token, generate);
    fprintf(out, "());\n");
  } else {
    fprintf(out, "=va_arg(*app, ");
    O_CALL(arg->type, generate);
    fprintf(out, ");\n");
  }
}

static void ClassDeclaration_generate_method_arguments(void *_arg)
{
  struct ArgumentDeclaration * arg = O_CAST(_arg, ArgumentDeclaration());
  fprintf(out, ", ");
  O_CALL(arg, generate);
}

static void ClassDeclaration_generate_constructor_definition(void *_constructor_decl, va_list * app)
{
  struct ConstructorDeclaration * constructor_decl = O_CAST(_constructor_decl, ConstructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_METHOD_DEF(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", void *, ");
  if (strcmp(constructor_decl->name->name->data, "ctor") != 0) {
    fprintf(out, "ctor_");
  }
  O_CALL(constructor_decl->name, generate);
  fprintf(out, ", (void *_self, va_list *app));\n");
}

static void ClassDeclaration_generate_method_definition(void *_method_decl, va_list * app)
{
  struct FunctionDeclaration * method_decl = O_CAST(_method_decl, FunctionDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  struct FunctionType * method_type = o_cast(method_decl->type, FunctionType());
  fprintf(out, "O_METHOD_DEF(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_type->return_type, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ", (void *_self");
  O_CALL(method_decl->formal_arguments, map, ClassDeclaration_generate_method_arguments);
  fprintf(out, "));\n");
}

static void ClassDeclaration_generate_constructor_registration(void *_constructor_decl, va_list *app)
{
  struct ConstructorDeclaration * constructor_decl = O_CAST(_constructor_decl, ConstructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "; \\\n O_METHOD(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  if (strcmp(constructor_decl->name->name->data, "ctor") != 0) {
    fprintf(out, "ctor_");
  }
  O_CALL(constructor_decl->name, generate);
  fprintf(out, ")");
}

static void ClassDeclaration_generate_method_registration(void *_method_decl, va_list *app)
{
  struct FunctionDeclaration * method_decl = O_CAST(_method_decl, FunctionDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "; \\\n O_METHOD(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ")");
}

static void ClassDeclaration_generate_constructor_registration_2(void *_constructor_decl, va_list *app)
{
  struct ConstructorDeclaration * constructor_decl = O_CAST(_constructor_decl, ConstructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_OBJECT_METHOD(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  if (strcmp(constructor_decl->name->name->data, "ctor") != 0) {
    fprintf(out, "ctor_");
  }
  O_CALL(constructor_decl->name, generate);
  fprintf(out, ");\n");
}

static void ClassDeclaration_generate_destructor_registration_2(void *_destructor_decl, va_list *app)
{
  struct DestructorDeclaration * destructor_decl = O_CAST(_destructor_decl, DestructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_OBJECT_METHOD(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", dtor);\n");
}

static void ClassDeclaration_generate_method_registration_2(void *_method_decl, va_list *app)
{
  struct FunctionDeclaration * method_decl = O_CAST(_method_decl, FunctionDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_OBJECT_METHOD(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", ");
  O_CALL(method_decl->name, generate);
  fprintf(out, ");\n");
}

static void ClassDeclaration_generate_constructor_implementation(void *_constructor_decl, va_list *app)
{
  struct ConstructorDeclaration * constructor_decl = O_CAST(_constructor_decl, ConstructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_IMPLEMENT(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", void *");
  fprintf(out, ", ");
  if (strcmp(constructor_decl->name->name->data, "ctor") != 0) {
    fprintf(out, "ctor_");
  }
  O_CALL(constructor_decl->name, generate);
  fprintf(out, ", (void *_self, va_list *app))\n");
  fprintf(out, "{\n");
  fprintf(out, "struct ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "* self=O_CAST(_self, ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "());\n");
  O_CALL(constructor_decl->formal_arguments, map, ClassDeclaration_generate_constructor_arguments);
  
  O_CALL(constructor_decl->body, generate);
  fprintf(out, "return self;\n");
  fprintf(out, "}\n\n");
}

static void ClassDeclaration_generate_destructor_implementation(void *_destructor_decl, va_list *app)
{
  struct DestructorDeclaration * destructor_decl = O_CAST(_destructor_decl, DestructorDeclaration());
  struct ClassDeclaration * class_decl = o_cast(va_arg(*app, struct ClassDeclaration *), ClassDeclaration());
  fprintf(out, "O_IMPLEMENT(");
  O_CALL(class_decl->name, generate);
  fprintf(out, ", void *");
  fprintf(out, ", dtor, (void *_self))\n");
  fprintf(out, "{\n");
  fprintf(out, "struct ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "* self=O_CAST(_self, ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "());\n");
  O_CALL(destructor_decl->body, generate);
  fprintf(out, "return O_SUPER->dtor(self);\n");
  fprintf(out, "}\n\n");
}

static void ClassDeclaration_generate_method_implementation(void *_method_decl, va_list *app)
{
  struct FunctionDeclaration * method_decl = O_CAST(_method_decl, FunctionDeclaration());
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
  fprintf(out, "))\n");
  fprintf(out, "{\n");
  fprintf(out, "struct ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "* self=O_CAST(_self, ");
  O_CALL(class_decl->name, generate);
  fprintf(out, "());\n");

  if (!o_is_of(method_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(method_type->return_type))->token->type != VOID)
    {
      O_CALL(method_type->return_type, generate);
      fprintf(out, " return_value;\n");
    }

  if (method_type->has_var_args)
    {
      fprintf(out, "va_list ap;\n");
      fprintf(out, "va_start(ap, ");
      struct ArgumentDeclaration * arg_decl;
      if (method_decl->formal_arguments->length == 1)
	{
	  fprintf(out, "_self");
	}
      else
	{
	  struct ArgumentDeclaration * arg_decl = O_CALL(method_decl->formal_arguments, get, method_decl->formal_arguments->length - 2);
	  O_CALL(arg_decl->name, generate);
	}
      fprintf(out, ");\n");
    }

  O_CALL(method_decl->body, generate);

  if (!o_is_of(method_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(method_type->return_type))->token->type != VOID)
    {
      fprintf(out, "function_end:\n");
    }

  if (method_type->has_var_args)
    {
      fprintf(out, "va_end(ap);\n");
    }

  if (!o_is_of(method_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(method_type->return_type))->token->type != VOID)
    {
      fprintf(out, "return return_value;\n");
    }

  fprintf(out, "}\n\n");
}

static void ClassDeclaration_generate_attribute_registration(void *_method_decl, va_list *app)
{
  struct VariableDeclaration * method_decl = O_CAST(_method_decl, VariableDeclaration());
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

O_IMPLEMENT(ClassDeclaration, void, generate, (void *_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  /* filter the members */
  struct RefList * attributes = O_CALL(self->members, filter_args, type_filter, VariableDeclaration());
  O_CALL(attributes, retain);
  struct RefList * methods = O_CALL(self->members, filter_args, type_filter, FunctionDeclaration());
  O_CALL(methods, retain);
  struct RefList * constructors = O_CALL(self->members, filter_args, type_filter, ConstructorDeclaration());
  O_CALL(constructors, retain);
  struct RefList * destructors = O_CALL(self->members, filter_args, type_filter, DestructorDeclaration());
  O_CALL(destructors, retain);

  struct RefList * new_methods = O_CALL(methods, filter_args, Declaration_new_member_filter, FunctionDeclaration());
  O_CALL(new_methods, retain);

  struct RefList * new_constructors = O_CALL(constructors, filter, new_constructor_filter);
  O_CALL(new_constructors, retain);

  /* generate the class */
  O_CALL(new_constructors, map_args, ClassDeclaration_generate_constructor_definition, self);
  fprintf(out, "\n");

  O_CALL(new_methods, map_args, ClassDeclaration_generate_method_definition, self);
  fprintf(out, "\n");

  fprintf(out, "#define ");
  O_CALL(self->name, generate);
  fprintf(out, "Class_Attr\\\n ");
  generate_superclass(self);
  fprintf(out, "Class_Attr");
  O_CALL(new_constructors, map_args, ClassDeclaration_generate_constructor_registration, self);
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

  O_CALL(constructors, map_args, ClassDeclaration_generate_constructor_implementation, self);
  O_CALL(destructors, map_args, ClassDeclaration_generate_destructor_implementation, self);
  O_CALL(methods, map_args, ClassDeclaration_generate_method_implementation, self);

  fprintf(out, "O_OBJECT(");
  O_CALL(self->name, generate);
  fprintf(out, ", ");
  generate_superclass(self);
  fprintf(out, ");\n");

  O_CALL(constructors, map_args, ClassDeclaration_generate_constructor_registration_2, self);
  O_CALL(destructors, map_args, ClassDeclaration_generate_destructor_registration_2, self);
  O_CALL(methods, map_args, ClassDeclaration_generate_method_registration_2, self);

  fprintf(out, "O_END_OBJECT\n\n");

  fprintf(out, "#undef O_SUPER\n\n");

  O_CALL(attributes, release);
  O_CALL(methods, release);
  O_CALL(new_methods, release);
  O_CALL(constructors, release);
  O_CALL(destructors, release);
}

O_IMPLEMENT(ClassDeclaration, void, type_check, (void *_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  O_CALL(current_context, add, self);
  if (self->superclass)
    {
      struct Declaration * _super_class = O_CALL(self->scope, lookup, self->superclass);
      if (_super_class)
	{
	  struct ClassDeclaration * super_class = O_CAST(_super_class, ClassDeclaration());
	  self->member_scope->parent = super_class->member_scope;
	}
    }
  O_CALL(self->members, map, Declaration_list_type_check);
  O_CALL(current_context, remove_last);
}

O_IMPLEMENT(ClassDeclaration, bool, is_compatible, (void *_self, void *_other))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  struct ClassDeclaration * other = o_cast(_other, ClassDeclaration());
  while (self && self != other)
    {
      self = (struct ClassDeclaration *) O_CALL(self->scope, lookup, self->superclass);
    }
  return self == other;
}

O_OBJECT(ClassDeclaration, Declaration);
O_OBJECT_METHOD(ClassDeclaration, ctor);
O_OBJECT_METHOD(ClassDeclaration, dtor);
O_OBJECT_METHOD(ClassDeclaration, generate);
O_OBJECT_METHOD(ClassDeclaration, type_check);
O_END_OBJECT

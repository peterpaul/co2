#include "InterfaceDeclaration.h"
#include "ArgumentDeclaration.h"
#include "FunctionDeclaration.h"
#include "FunctionType.h"
#include "PrimitiveType.h"
#include "Statement.h"
#include "io.h"
#include "grammar.tab.h"
#include "ClassDeclaration.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT (InterfaceDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->interfaces =
    o_branch_cast (va_arg (*app, struct RefList *), RefList ());
  O_BRANCH_CALL (self->interfaces, retain);
  return self;
}

O_IMPLEMENT (InterfaceDeclaration, void *, dtor, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  O_BRANCH_CALL (self->interfaces, release);
  return O_SUPER->dtor (self);
}

static void
InterfaceDeclaration_generate_method_definition (void *_method_decl,
						 va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct InterfaceDeclaration *class_decl =
    O_CAST (va_arg (*app, struct InterfaceDeclaration *),
	    InterfaceDeclaration ());
  struct FunctionType *method_type =
    o_cast (method_decl->type, FunctionType ());
  fprintf (out, "O_METHOD_DEF (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_type->return_type, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ", (void *_self");
  O_CALL (method_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_arguments);
  fprintf (out, "));\n");
}

static void
InterfaceDeclaration_generate_method_registration (void *_method_decl,
						   va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct InterfaceDeclaration *class_decl =
    O_CAST (va_arg (*app, struct InterfaceDeclaration *),
	    InterfaceDeclaration ());
  fprintf (out, "; \\\n O_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ")");
}

static void
InterfaceDeclaration_generate_method_implementation (void *_method_decl,
						     va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct InterfaceDeclaration *class_decl =
    O_CAST (va_arg (*app, struct InterfaceDeclaration *),
	    InterfaceDeclaration ());
  struct FunctionType *method_type =
    o_cast (method_decl->type, FunctionType ());
  fprintf (out, "O_METHOD_IF (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_type->return_type, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ", (void *_self");
  O_CALL (method_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_arguments);
  fprintf (out, "), (_self");
  O_CALL (method_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_argument_names);
  fprintf (out, "));\n");
}

O_IMPLEMENT (InterfaceDeclaration, void, generate, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  /* filter the members */
  struct RefList *methods =
    O_CALL (self->members, filter_args, type_filter, FunctionDeclaration ());
  O_CALL (methods, retain);
  struct RefList *new_methods =
    O_CALL (methods, filter_args, Declaration_new_member_filter,
	    FunctionDeclaration ());
  O_CALL (new_methods, retain);

  fprintf (out, "#include \"Interface.h\"\n");

  /* generate the class */
  O_CALL (new_methods, map_args,
	  InterfaceDeclaration_generate_method_definition, self);
  fprintf (out, "\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "Class_Attr\\\n ");
  fprintf (out, "InterfaceClass_Attr");
  fprintf (out, "\n\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "_Attr\\\n ");
  fprintf (out, "Interface_Attr");
  O_CALL (new_methods, map_args,
	  InterfaceDeclaration_generate_method_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "O_CLASS (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n\n");

  fprintf (out, "#define O_SUPER Interface ()\n\n");

  O_CALL (methods, map_args,
	  InterfaceDeclaration_generate_method_implementation, self);
  fprintf (out, "\n");

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n");
  fprintf (out, "O_END_OBJECT\n\n");

  fprintf (out, "#undef O_SUPER\n\n");

  O_CALL (methods, release);
  O_CALL (new_methods, release);
}

O_IMPLEMENT (InterfaceDeclaration, void, type_check, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  O_CALL (current_context, add, self);
  O_CALL (self->members, map, Declaration_list_type_check);
  O_CALL (current_context, remove_last);
}

static void InterfaceDeclaration_is_compatible_with_class(void *_self, va_list *app)
{
  struct Token *self = O_CAST(_self, Token ());
  struct InterfaceDeclaration *interface = O_CAST(va_arg(*app, struct InterfaceDeclaration*), InterfaceDeclaration ());
  bool *found = va_arg(*app, bool*);
  if (strcmp(self->name->data, interface->name->name->data) == 0) 
    {
      *found = true;
    }
}

O_IMPLEMENT (InterfaceDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct InterfaceDeclaration *self = O_CAST (_self, InterfaceDeclaration ());
  struct ObjectTypeDeclaration *other = o_cast (_other, ObjectTypeDeclaration ());
  bool found = false;
  if (o_is_of (other, ClassDeclaration ()))
    {
      struct ClassDeclaration *class_decl = O_CAST (other, ClassDeclaration ());
      do 
	{
	  O_BRANCH_CALL (class_decl->interfaces, map_args, InterfaceDeclaration_is_compatible_with_class, self, &found);
	} 
      while (!found && class_decl->superclass);
      return found;
    }
  else
    {
      return strcmp (self->name->name->data, other->name->name->data) == 0;
    }
}

O_OBJECT (InterfaceDeclaration, Declaration);
O_OBJECT_METHOD (InterfaceDeclaration, ctor);
O_OBJECT_METHOD (InterfaceDeclaration, dtor);
O_OBJECT_METHOD (InterfaceDeclaration, type_check);
O_OBJECT_METHOD (InterfaceDeclaration, generate);
O_OBJECT_METHOD (InterfaceDeclaration, is_compatible);
O_END_OBJECT

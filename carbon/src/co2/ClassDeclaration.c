#include "co2/ClassDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/RefList.h"
#include "co2/FunctionType.h"
#include "co2/io.h"
#include "co2/Statement.h"
#include "co2/Scope.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT (ClassDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->superclass = O_BRANCH_RETAIN_ARG (Token);
  self->interfaces = O_BRANCH_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (ClassDeclaration, void *, dtor, (void *_self))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (self->superclass, release);
  O_BRANCH_CALL (self->interfaces, release);
  return O_SUPER->dtor (self);  
}

O_IMPLEMENT (ClassDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->members, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

int
new_constructor_filter (void *_constructor)
{
  struct ConstructorDeclaration *constructor =
    O_CAST (_constructor, ConstructorDeclaration ());
  if (strcmp (constructor->name->name->data, "ctor") != 0)
    {
      return O_BRANCH_CALL (constructor->scope->parent,
			    lookup_type_in_this_scope, constructor->name,
			    ConstructorDeclaration ()) == NULL;
    }
  return false;
}

void
generate_superclass (struct ClassDeclaration *self)
{
  if (self->superclass)
    {
      O_CALL (self->superclass, generate);
    }
  else
    {
      fprintf (out, "Object");
    }
}

O_IMPLEMENT (ClassDeclaration, void, type_check, (void *_self))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  if (self->superclass)
    {
      struct Declaration *_super_class =
	O_CALL (self->scope, lookup, self->superclass);
      if (_super_class)
	{
	  struct ClassDeclaration *super_class =
	    O_CAST (_super_class, ClassDeclaration ());
	  self->member_scope->parent = super_class->member_scope;
	}
    }
  /* TODO check whether:
   * 1. superclasses already implement interfaces, if so: fail
   * 2. foreach interface, methods already exists in other interface (including interfaces from superclasses), if so: fail
   * 3. class (including superclasses) implements all methods of all interfaces, if not: fail
   */
  O_CALL (self->members, map, Declaration_list_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ClassDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct ClassDeclaration *self = O_CAST (_self, ClassDeclaration ());
  if (o_is_of (_other, ClassDeclaration ()))
    {
      struct ClassDeclaration *other = O_CAST (_other, ClassDeclaration ());
      while (other && other->superclass && self != other)
	{
	  other =
	    (struct ClassDeclaration *) O_CALL (global_scope, lookup,
						other->superclass);
	}
      return self == other;
    }
  else if (o_is_of (_other, InterfaceDeclaration ()))
    {
      struct InterfaceDeclaration *other = O_CAST (_other, InterfaceDeclaration ());
      return O_CALL (other, is_compatible, self);
    }
  else
    {
      return false;
    }
}

O_OBJECT (ClassDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD (ClassDeclaration, ctor);
O_OBJECT_METHOD (ClassDeclaration, dtor);
O_OBJECT_METHOD (ClassDeclaration, accept);
O_OBJECT_METHOD (ClassDeclaration, type_check);
O_OBJECT_METHOD (ClassDeclaration, is_compatible);
O_END_OBJECT

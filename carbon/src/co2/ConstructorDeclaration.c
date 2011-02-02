#include "co2/ConstructorDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/Type.h"
#include "co2/RefList.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"

#define O_SUPER Declaration()

O_IMPLEMENT (ConstructorDeclaration, void *, ctor,
	     (void *_self, va_list * app))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->class_name = O_RETAIN_ARG (Token);
  self->formal_arguments = O_RETAIN_ARG (RefList);
  self->body = O_BRANCH_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT (ConstructorDeclaration, void *, dtor, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  O_CALL (self->class_name, release);
  O_CALL (self->formal_arguments, release);
  O_BRANCH_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ConstructorDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ConstructorDeclaration *self = O_CAST (_self, ConstructorDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->formal_arguments, map_args, accept, visitor);
  O_BRANCH_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (ConstructorDeclaration, void, type_check, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  struct Declaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  if (class_decl == NULL)
    {
      error (self->class_name,
	     "Constructors ('%s') are only allowed in class declarations\n",
	     self->class_name->name->data);
    }
  if (strcmp (class_decl->name->name->data, self->class_name->name->data) !=
      0)
    {
      error (self->class_name, "Constructor should be called '%s'\n",
	     class_decl->name->name->data);
    }

  O_CALL (self->formal_arguments, map, Declaration_list_type_check);

  self->type =
    O_CALL_CLASS (ObjectType (), new, self->class_name, class_decl);
  O_CALL (self->type, retain);

  O_CALL (self->body, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (ConstructorDeclaration, Declaration);
O_OBJECT_METHOD (ConstructorDeclaration, ctor);
O_OBJECT_METHOD (ConstructorDeclaration, dtor);
O_OBJECT_METHOD (ConstructorDeclaration, accept);
O_OBJECT_METHOD (ConstructorDeclaration, type_check);
O_END_OBJECT

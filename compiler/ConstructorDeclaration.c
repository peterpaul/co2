#include "ConstructorDeclaration.h"
#include "ClassDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Statement.h"
#include "io.h"
#include "FunctionType.h"
#include "ObjectType.h"

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

O_IMPLEMENT (ConstructorDeclaration, void, accept, (void *_self, struct CompileObjectVisitor *visitor))
{
  struct ConstructorDeclaration *self = O_CAST (_self, ConstructorDeclaration ());
  O_CALL (self->formal_arguments, map_args, accept, visitor);
  O_CALL (self->body, accept, visitor);
  O_CALL_IF (CompileObjectVisitor, visitor, visit, self);
}

O_IMPLEMENT (ConstructorDeclaration, void, type_check, (void *_self))
{
  struct ConstructorDeclaration *self =
    O_CAST (_self, ConstructorDeclaration ());
  O_CALL (current_context, add, self);
  struct Declaration *class_decl =
    O_CALL (current_context, find, ClassDeclaration ());
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

  self->type =
    O_CALL_CLASS (ObjectType (), new, self->class_name, class_decl);
  O_CALL (self->type, retain);

  O_CALL (self->body, type_check);
  O_CALL (current_context, remove_last);
}

O_OBJECT (ConstructorDeclaration, Declaration);
O_OBJECT_METHOD (ConstructorDeclaration, ctor);
O_OBJECT_METHOD (ConstructorDeclaration, dtor);
O_OBJECT_METHOD (ConstructorDeclaration, accept);
O_OBJECT_METHOD (ConstructorDeclaration, type_check);
O_END_OBJECT

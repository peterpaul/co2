#include "DestructorDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Statement.h"
#include "io.h"
#include "FunctionType.h"

#define O_SUPER Declaration()

static struct FunctionType *
get_type (struct DestructorDeclaration *self)
{
  return o_cast (self->type, FunctionType ());
}

O_IMPLEMENT (DestructorDeclaration, void *, ctor,
	     (void *_self, va_list * app))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  self = O_SUPER->ctor (self, app);
  /*
     // TODO lookup current class
     self->type = O_CAST(va_arg(*app, struct FunctionType *), FunctionType());
     O_CALL(self->type, retain);
   */
  self->class_name = O_RETAIN_ARG (Token);
  self->body = O_BRANCH_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT (DestructorDeclaration, void *, dtor, (void *_self))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  O_CALL (self->class_name, release);
  O_BRANCH_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (DestructorDeclaration, void, generate, (void *_self))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  O_CALL (get_type (self)->return_type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, "(");
  fprintf (out, ")\n");
  fprintf (out, "{\n");
  O_CALL (self->body, generate);
  fprintf (out, "}\n");
}

O_IMPLEMENT (DestructorDeclaration, void, type_check, (void *_self))
{
  struct DestructorDeclaration *self =
    O_CAST (_self, DestructorDeclaration ());
  O_CALL (current_context, add, self);
  O_CALL (self->body, type_check);
  O_CALL (current_context, remove_last);
}

O_OBJECT (DestructorDeclaration, Declaration);
O_OBJECT_METHOD (DestructorDeclaration, ctor);
O_OBJECT_METHOD (DestructorDeclaration, dtor);
O_OBJECT_METHOD (DestructorDeclaration, generate);
O_OBJECT_METHOD (DestructorDeclaration, type_check);
O_END_OBJECT

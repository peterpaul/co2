#include "lex.yy.h"
#include "IncludeStack.h"

const char * input_file_name = NULL;
int linenumber = 1;
struct IncludeStack * include_stack = NULL;

#define O_SUPER RefObject()

O_IMPLEMENT (IncludeStack, void *, ctor, (void *_self, va_list * app))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  self = O_SUPER->ctor (self, app);
  self->parent = include_stack;
  include_stack = self;
  self->current_line = va_arg (*app, unsigned);
  self->current_file = va_arg (*app, char *);
  self->buffer_state = va_arg (*app, YY_BUFFER_STATE);
  return self;
}

O_IMPLEMENT (IncludeStack, void *, dtor, (void *_self))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  include_stack = self->parent;
  return O_SUPER->dtor (self);
}

O_OBJECT (IncludeStack, RefObject);
O_OBJECT_METHOD (IncludeStack, ctor);
O_OBJECT_METHOD (IncludeStack, dtor);
O_END_OBJECT

#include "lex.yy.h"
#include "IncludeStack.h"

const char * input_file_name = NULL;
int linenumber = 1;

#define O_SUPER RefObject()

O_IMPLEMENT (IncludeStack, void *, ctor, (void *_self, va_list * app))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  self = O_SUPER->ctor (self, app);
  self->current_line = va_arg (*app, unsigned);
  self->current_file = va_arg (*app, char *);
  self->buffer_state = va_arg (*app, YY_BUFFER_STATE);
  return self;
}

O_IMPLEMENT (IncludeStack, void *, dtor, (void *_self))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (IncludeStack, void, switch_back, (void *_self))
{
  struct IncludeStack *self = O_CAST (_self, IncludeStack ());
  /* The input_file_name is only allocated for include files, and not for
     the original input file. Therefore this call to free is after the
     check on the include level. */
  free((char *) input_file_name);
  yy_switch_to_buffer(self->buffer_state);
  linenumber = self->current_line;
  input_file_name = self->current_file;
  O_CALL(self, delete);
}

O_OBJECT (IncludeStack, RefObject);
O_OBJECT_METHOD (IncludeStack, ctor);
O_OBJECT_METHOD (IncludeStack, dtor);
O_OBJECT_METHOD (IncludeStack, switch_back);
O_END_OBJECT

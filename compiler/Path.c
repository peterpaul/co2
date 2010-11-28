#include "Path.h"
#include "RefList.h"
#include "Token.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Path, void *, ctor, (void *_self, va_list * app))
{
  struct Path *self = O_CAST (_self, Path ());
  self = O_SUPER->ctor (self, app);
  self->path_name = O_CAST (va_arg (*app, struct RefList *), RefList ());
  O_CALL (self->path_name, retain);
  return self;
}

O_IMPLEMENT (Path, void *, dtor, (void *_self))
{
  struct Path *self = O_CAST (_self, Path ());
  O_CALL (self->path_name, release);
  return O_SUPER->dtor (self);
}

void
Path_convert_to_system_path (void *_token, va_list * app)
{
  struct Token *token = O_CAST (_token, Token ());
  struct String *result = O_CAST (va_arg (*app, struct String *), String ());
  O_CALL (result, append_str, "/");
  O_CALL (result, append, token->name);
}

O_IMPLEMENT (Path, struct String *, to_system_path, (void *_self))
{
  struct Path *self = O_CAST (_self, Path ());
  struct String *result = O_CALL_CLASS (String (), new, "");
  O_CALL (self->path_name, map_args, Path_convert_to_system_path, result);
  return result;
}

O_OBJECT (Path, CompileObject);
O_OBJECT_METHOD (Path, ctor);
O_OBJECT_METHOD (Path, dtor);
O_OBJECT_METHOD (Path, to_system_path);
O_END_OBJECT

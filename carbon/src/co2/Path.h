#ifndef Path_H
#define Path_H

#include "co2/CompileObject.h"

O_METHOD_DEF (Path, struct String *, to_system_path, (void *_self));

#define PathClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Path, to_system_path)

#define Path_Attr				\
  CompileObject_Attr;				\
  struct RefList * path_name

O_CLASS (Path, CompileObject);

#endif /* Path_H */

#ifndef Path_H
#define Path_H

#include "CompileObject.h"

#define PathClass_Attr				\
  CompileObjectClass_Attr

#define Path_Attr				\
  CompileObject_Attr;				\
  struct RefList * path_name

O_CLASS(Path, CompileObject);

#endif /* Path_H */

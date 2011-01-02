#ifndef File_H
#define File_H

#include "CompileObject.h"

#define FileClass_Attr				\
  CompileObjectClass_Attr

#define File_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  struct String * absolute_path;		\
  struct RefList * file_dependencies;		\
  struct RefList * declarations

O_CLASS (File, CompileObject);

#endif /* File_H */

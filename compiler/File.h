#ifndef File_H
#define File_H

#include "CompileObject.h"

O_METHOD_DEF(File, void, parse_imports, (void *_self));

#define FileClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(File, parse_imports)

#define File_Attr				\
  CompileObject_Attr;				\
  struct RefList * declarations;		\
  struct RefList * import_statements;		\
  struct RefList * imported_files;		\
  struct Path * package

O_CLASS(File, CompileObject);

#endif /* File_H */

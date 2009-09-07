#ifndef File_H
#define File_H

#include "CompileObject.h"

#define FileClass_Attr				\
  CompileObjectClass_Attr

#define File_Attr				\
  CompileObject_Attr;				\
  struct RefList * declarations;		\
  struct RefList * imports;			\
  struct Path * package

O_CLASS(File, CompileObject);

#endif /* File_H */

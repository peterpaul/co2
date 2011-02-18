#ifndef File_H
#define File_H

#include "co2/CompileObject.h"
#include "co2/Hash.h"

O_METHOD_DEF (File, void, sort, (void *_self));

#define FileClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD (File, sort)

#define File_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  struct String * absolute_path;		\
  struct RefList * file_dependencies;		\
  struct RefList * declarations

O_CLASS (File, CompileObject);

struct Hash * get_hash_map ();

#endif /* File_H */

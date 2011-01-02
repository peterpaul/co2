#ifndef INCLUDESTACK_H
#define INCLUDESTACK_H

#include "RefObject.h"

#define IncludeStackClass_Attr			\
  RefObjectClass_Attr

#define IncludeStack_Attr			\
  RefObject_Attr;				\
  struct IncludeStack * parent;			\
  unsigned current_line;			\
  const char * current_file;			\
  YY_BUFFER_STATE buffer_state

O_CLASS (IncludeStack, RefObject);

extern const char * input_file_name;
extern int linenumber;

extern struct IncludeStack * include_stack;

#endif /* INCLUDESTACK_H */

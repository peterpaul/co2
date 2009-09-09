#ifndef INCLUDESTACK_H
#define INCLUDESTACK_H

#include "RefObject.h"

#define IncludeStackClass_Attr			\
  RefObjectClass_Attr

#define IncludeStack_Attr			\
  RefObject_Attr;				\
  unsigned current_line;			\
  const char * current_file;			\
  YY_BUFFER_STATE buffer_state

O_CLASS(IncludeStack, RefObject);

#endif /* INCLUDESTACK_H */

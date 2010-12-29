#ifndef INCLUDESTACK_H
#define INCLUDESTACK_H

#include "RefObject.h"

O_METHOD_DEF (IncludeStack, void, switch_back, (void *));

#define IncludeStackClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD (IncludeStack, switch_back)

#define IncludeStack_Attr			\
  RefObject_Attr;				\
  unsigned current_line;			\
  const char * current_file;			\
  YY_BUFFER_STATE buffer_state

O_CLASS (IncludeStack, RefObject);

extern const char * input_file_name;
extern int linenumber;


#endif /* INCLUDESTACK_H */

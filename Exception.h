#ifndef Exception_H
#define Exception_H

#include "Object.h"
#include "exception.h"

#define ExceptionClass_Attr			\
  ObjectClass_Attr;				\
  unsigned long id

#define Exception_Attr				\
  Object_Attr;					\
  const char * message;				\
  const char * file;				\
  unsigned int line;				\
  const char * func

O_CLASS (Exception, Object);

void throw_ex (struct Exception *ex);

#define new_Exception(type,message,...)					\
  O_CALL_CLASS(type, new, __FILE__, __LINE__, __func__, message, ## __VA_ARGS__)
#define catch_ex(x)				\
  else if (ex_stack->ex_val == x ()->id)

#endif /* Exception_H */

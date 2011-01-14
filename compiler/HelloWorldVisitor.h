#ifndef HelloWorldVisitor_H
#define HelloWorldVisitor_H

#include "BaseCompileObjectVisitor.h"

#define HelloWorldVisitorClass_Attr			\
  BaseCompileObjectVisitorClass_Attr

#define HelloWorldVisitor_Attr				\
  BaseCompileObjectVisitor_Attr

O_CLASS(HelloWorldVisitor, BaseCompileObjectVisitor);

#endif /* HelloWorldVisitor_H */

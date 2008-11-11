#ifndef _MY_INTERFACE_H
#define _MY_INTERFACE_H

#include "Interface.h"

O_METHOD_DEF(MyInterface, int, getValue, (void *_self));

#define MyInterfaceClass_Attr					\
	InterfaceClass_Attr

#define MyInterface_Attr						\
	Interface_Attr;								\
	O_METHOD(MyInterface, getValue)

O_CLASS(MyInterface, Interface);

#endif /* _MY_INTERFACE_H */

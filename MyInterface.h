#ifndef _MY_INTERFACE_H
#define _MY_INTERFACE_H

#include "PInterface.h"

P_METHOD_DEF(MyInterface, int, getValue, (void *_self));

#define MyInterface_Class					\
	PInterface_Class;					\
	P_METHOD(MyInterface, getValue)

#define MyInterface_Attr					\
	PInterface_Attr

P_CLASS(MyInterface, PInterface);

#endif /* _MY_INTERFACE_H */

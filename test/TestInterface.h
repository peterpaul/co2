#ifndef _TESTINTERFACE_H
#define _TESTINTERFACE_H

/* This object implements MyInterface */
#include "MyInterface.h"

/* Implement interface method */
#define TestInterfaceClass_Attr \
	ObjectClass_Attr; \
	O_METHOD(MyInterface, getValue)

#define TestInterface_Attr \
	Object_Attr; \
	int value

O_CLASS(TestInterface, Object);

#endif				/* _TESTINTERFACE_H */

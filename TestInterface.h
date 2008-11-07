#ifndef _TESTINTERFACE_H
#define _TESTINTERFACE_H

/* This object implements MyInterface */
#include "MyInterface.h"

/* Implement interface method */
#define TestInterface_Class \
PObject_Class; \
P_METHOD(MyInterface, getValue)

#define TestInterface_Attr \
PObject_Attr; \
int value

P_CLASS(TestInterface, PObject);

#endif /* _TESTINTERFACE_H */

#ifndef _PINTERFACE_H
#define _PINTERFACE_H

#include "PObject.h"

/*
object->class->interfaces


*/

/* Interface */
#define PInterface_Class						\
	PObject_Class

#define PInterface_Attr							\
	PObject_Attr;							\
	char * name;							\
	struct PInterface * next

P_CLASS(PInterface, PObject);

#endif /* _PINTERFACE_H */

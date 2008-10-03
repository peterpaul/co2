#ifndef _PINTERFACE_H
#define _PINTERFACE_H

#include "PObject.h"

/* Interface */
#define PInterface_Class						\
	PObject_Class

#define PInterface_Attr							\
	PObject_Attr;							\
	char * name

P_CLASS(PInterface, PObject);

#endif /* _PINTERFACE_H */

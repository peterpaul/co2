#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "Object.h"

#define O_IMPLEMENT_IF(klass,type,name,args,act_args)	\
	O_IMPLEMENT(klass,type,name##_impl,args) {			\
		struct klass * self = O_CAST(_self, klass());	\
		return self->class->name(act_args);				\
	}													\
	O_IMPLEMENT(klass,type,name,args)					\

#define O_OBJECT_IF(IF)													\
	{																	\
	struct IF * interface = IF()->new(IF(), O_STRING(IF), self->interface_list); \
	self->interface_list = interface

#define O_OBJECT_IF_END							\
	}

/* Interface */
#define InterfaceClass_Attr						\
	ObjectClass_Attr

#define Interface_Attr							\
	Object_Attr;								\
	char * name;								\
	struct Interface * next

O_CLASS(Interface, Object);

#endif /* _INTERFACE_H */

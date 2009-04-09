#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "Object.h"

#define O_METHOD_IF(klass,type,name,args,act_args)			\
	O_FUNCTION_DEF(klass,type,name,args) {				\
		struct klass * interface =				\
			o_get_interface(_self, klass());		\
		return interface->name act_args;			\
	}

#define O_IMPLEMENT_IF(klass,type,name,args,act_args)			\
	static O_FUNCTION_DEF(klass,type,name##_impl,args) {		\
		struct klass * self = O_CAST(_self, klass());		\
		return self->class->name act_args;			\
	}								\
	static O_FUNCTION_DEF(_##klass,type,name,args)

#define O_OBJECT_IF(IF)							\
	{								\
	struct IF * interface = O_CALL_CLASS(IF(), new, O_STRING(IF), self->interface_list); \
	self->interface_list = interface

#define O_OBJECT_IF_METHOD(klass,method)		\
	interface->method = klass##_##method##_impl

#define O_OBJECT_IF_END				\
	}

/* Interface */
#define InterfaceClass_Attr			\
	ObjectClass_Attr

#define Interface_Attr							\
	Object_Attr;							\
	char * name;							\
	struct Interface * next

O_CLASS(Interface, Object);

#endif				/* _INTERFACE_H */

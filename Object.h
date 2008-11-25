/*
 * Object.h
 *
 *  Created on: Oct 22, 2008
 *      Author: epetkle
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "utils.h"

/* Macros */
#define O_STRING(x) #x

#define O_MAGIC 0xCAFEBABE

#define O_METHOD_DEF(klass,type,name,args)		\
	typedef type (*klass##_##name##_##t) args

#define O_METHOD(klass,name)			\
	klass##_##name##_##t name

#define O_FUNCTION_DEF(klass,type,name,args)	\
	type klass##_##name args

#define O_IMPLEMENT(klass,type,name,args,act_args)		\
	O_FUNCTION_DEF(klass,type,name,args) {			\
		struct klass * self = O_CAST(_self, klass());	\
		return self->class->name act_args;		\
	}							\
	static O_FUNCTION_DEF(_##klass,type,name,args)

#define O_OBJECT_DEF(name,klass)		\
	struct klass * name();			\
	struct name {				\
		struct klass * class;		\
		name##_Attr;			\
	}

#define O_CLASS_DEF(name,supper)		\
	struct Class * name();			\
	struct name {				\
		struct Class * class;		\
		struct supper##Class * super;	\
		name##_Attr;			\
	}

#define O_CLASS(klass,supper)			\
	O_OBJECT_DEF(klass,klass##Class);	\
	O_CLASS_DEF(klass##Class, supper)

#define O_ASSERT_CLASS(x)						\
	(assertTrue(x,"O_ASSERT_CLASS: Expected non NULL pointer"),	\
	 assertTrue(((struct Class *)(x))->magic == O_MAGIC, "O_ASSERT_CLASS: Expected O_MAGIC"))

#define O_IS_CLASS(x)				\
	(O_ASSERT_CLASS(x),			\
	 (x))

#define O_IS_OBJECT(x)							\
	(assertTrue(x,"O_IS_OBJECT: Expected non NULL pointer"),	\
	 O_ASSERT_CLASS(((struct Object *)(x))->class),			\
	 (x))

#define O_OBJECT(klass,supper)						\
	struct Class * klass##Class() {					\
		static struct Class _self;				\
		static struct Class * self = NULL;			\
		if (self == NULL) {					\
			self = &_self;					\
			self = Class()->init(Class(), self,		\
					     sizeof(struct klass##Class), \
					     O_STRING(klass##Class),	\
					     O_SUPER->class);		\
		}							\
		return self;						\
	}								\
	struct klass##Class * klass() {					\
	static struct klass##Class _self;				\
	static struct klass##Class * self = NULL;			\
	if (self == NULL) {						\
	self = &_self;							\
	self = klass##Class()->init(klass##Class(), self,		\
				    sizeof(struct klass),		\
				    O_STRING(klass),			\
				    O_SUPER)

#define O_OBJECT_METHOD(klass,method)		\
	self->method = _##klass##_##method

#define O_OBJECT_METHOD_VAL(klass,method,val)	\
	self->method = val

#define O_OBJECT_END				\
	}					\
		return self;			\
		}

#define O_END_OBJECT O_OBJECT_END

#define O_CAST(o,c)				\
	o_cast(o,c)

/* Functions */
void *o_cast(const void *_object, const void *_class);
int o_is_a(const void *_self, const void *_class);
int o_is_of(const void *_self, const void *_class);
void *o_get_interface(void *_self, void *_interface);

void *o_get_class(const char *class_name);
void o_add_class(void *_class);
void o_print_classes(FILE * fp);

void *o_super_ctor(void *_self, const void *_class, ...);

/* Object methods definition */
O_METHOD_DEF(Object, void *, ctor, (void *_self, va_list * argp));
O_METHOD_DEF(Object, void *, new, (void *_self, ...));
O_METHOD_DEF(Object, void *, new_ctor,
	     (void *_self, Object_ctor_t ctor, ...));
O_METHOD_DEF(Object, void *, init, (const void *_class, void *_self, ...));
O_METHOD_DEF(Object, void *, init_ctor,
	     (const void *_class, void *_self, Object_ctor_t ctor, ...));
O_METHOD_DEF(Object, void *, dtor, (void *_self));
O_METHOD_DEF(Object, void *, delete, (void *_self));
O_METHOD_DEF(Object, struct String *, toString, (void *_self));
O_METHOD_DEF(Object, void *, clone, (void *_self));

/* Object definition */
#define Object_Attr

/* ObjectClass definition */
#define ObjectClass_Attr			\
	long magic;				\
	size_t size;				\
	const char * name;			\
	void * interface_list;			\
	O_METHOD(Object, new);			\
	O_METHOD(Object, new_ctor);		\
	O_METHOD(Object, init);			\
	O_METHOD(Object, init_ctor);		\
	O_METHOD(Object, ctor);			\
	O_METHOD(Object, dtor);			\
	O_METHOD(Object, delete);		\
	O_METHOD(Object, toString);		\
	O_METHOD(Object, clone)

#define ObjectClass Class

O_CLASS(Object, Object);

#endif				/* OBJECT_H_ */

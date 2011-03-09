/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "co2/utils.h"
#include "co2/exception.h"

/* Macros */
#define O_STRING(x) #x

#define O_MAGIC 0xCAFEBABE

#define O_METHOD(klass,name)			\
	klass##_##name##_##t name

#define O_FUNCTION_DEF(klass,type,name,args)	\
	type klass##_##name args

#define O_METHOD_DEF(klass,type,name,args)		\
	typedef type (*klass##_##name##_##t) args;	\
        O_FUNCTION_DEF(_##klass, type, name, args)

#define O_IMPLEMENT(klass,type,name,args)	\
	O_FUNCTION_DEF(_##klass,type,name,args)

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
	 assertTrue(((struct Class *)(x))->_magic == O_MAGIC, "O_ASSERT_CLASS: Expected O_MAGIC"))

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
			self = O_CALL_CLASS(Class(), init, self,		\
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
	self = O_CALL_CLASS(klass##Class(), init, self,		\
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

#define O_CALL(o,msg,...)						\
	({typeof(o) _tmp = o;						\
		assertTrue(_tmp->class->msg,				\
			   "runtime error: %s at %p doesn't respond to %s.", \
			   _tmp->class->class_name, (void *)_tmp, __STRING(msg)); \
		_tmp->class->msg(_tmp,##__VA_ARGS__);})

#define O_CALL_CLASS(o,msg,...)						\
	({typeof(o) _tmp = o;						\
		assertTrue(_tmp->msg,					\
			   "runtime error: %s at %p doesn't respond to %s.", \
			   _tmp->class_name, (void *)_tmp, __STRING(msg));	\
		_tmp->msg(_tmp,##__VA_ARGS__);})

#ifdef O_DEBUG
#define O_CAST(o,c)				\
  o_cast(o,c)
#define O_BRANCH_CAST(o,c)			\
  o_branch_cast(o,c)
#else
#define O_CAST(o,c)				\
  (void *) o
#define O_BRANCH_CAST(o,c)			\
  (void *) o
#endif

#define O_GET_ARG(type)				\
  O_CAST(va_arg(*app, struct type *), type())

#define O_BRANCH_GET_ARG(type)				\
  O_BRANCH_CAST(va_arg(*app, struct type *), type())

#define O_BRANCH_CALL(o,msg,...)					\
  ({typeof(o) _tmp = o;							\
    assertTrue(_tmp == (typeof(_tmp))0 || _tmp->class->msg,		\
	       "runtime error: %s at %p doesn't respond to %s.",	\
	       _tmp->class->class_name, (void *)_tmp, __STRING(msg));		\
    _tmp ? _tmp->class->msg(_tmp,##__VA_ARGS__) : (typeof(_tmp->class->msg(_tmp,##__VA_ARGS__)))0;})
  

/* Functions */
void *o_cast(const void *_object, const void *_class);
void *o_branch_cast(const void *_object, const void *_class);
void *o_cast_interface(const void *_object, const void *_interface);
int o_is_a(const void *_self, const void *_class);
int o_is_of(const void *_self, const void *_class);
int o_implements(const void *_self, const void *_interface);
void *o_get_interface(void *_self, void *_interface);
void *o_get_class(const char *class_name);
void o_add_class(void *_class);
void o_print_classes(FILE * fp);

void *o_super_ctor(void *_self, const void *_class, ...);
void *o_super_ctor_named(void *_self, void *(*constructor)(void *, va_list *), ...);

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

/* Object definition */
#define Object_Attr

/* ObjectClass definition */
#define ObjectClass_Attr			\
	long _magic;				\
	size_t object_size;			\
	const char * class_name;		\
	void * interface_list;			\
	O_METHOD(Object, new);			\
	O_METHOD(Object, new_ctor);		\
	O_METHOD(Object, init);			\
	O_METHOD(Object, init_ctor);		\
	O_METHOD(Object, ctor);			\
	O_METHOD(Object, dtor);			\
	O_METHOD(Object, delete)

#define ObjectClass Class

O_CLASS(Object, Object);

#endif				/* OBJECT_H_ */

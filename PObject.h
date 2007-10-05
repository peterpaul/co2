#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

/**
 * @brief Magic value, used to recognize a PObjectClass
 */
#define P_CLASS_MAGIC 0xCAFECAFE

/**
 * @brief Convenience macro, used to define methods
 *
 * This macro expands to a function definition, that can also be used
 * directly, for example:
 *
 * <code>
 * P_IMPLEMENT(MyObject, int, foo, (void * _self, int bar))
 * {
 * ...
 * }
 * </code>
 *
 * expands to:
 *
 * <code>
 * int MyObject_foo(void * _self, int bar)
 * {
 * ...
 * }
 * </code>
 *
 * This macro follows the same argument conventions as P_METHOD_DEF().
 *
 * @param klass The name of the PObjectClass this method is defined for
 * @param type The return type of this method
 * @param name The name of this method
 * @param args The arguments this method needs
 */ 
#define P_IMPLEMENT(klass,type,name,args)	\
	type klass##_##name args

/**
 * @brief Convenience macro, used to define a method type
 *
 * This macro expands to a type definition for a new method.
 * Each class that adds a new method to the list of methods
 * available for that class, should first define the type
 * of the method.
 *
 * <code>
 * P_METHOD_DEF(MyObject, int, foo, (void * _self, int bar));
 * </code>
 *
 * expands to:
 *
 * <code>
 * typedef int (*MyObject_foo_t)(void * _self, int bar);
 * </code>
 *
 * This macro follows the same argument conventions as P_IMPLEMENT().
 *
 * @param klass The name of the PObjectClass this method is defined for
 * @param type The return type of this method
 * @param name The name of this method
 * @param args The arguments this method needs
 */
#define P_METHOD_DEF(klass,type,name,args)	\
	typedef type  (*klass##_##name##_##t) args

/**
 * @brief Convenience macro, used to declare a method in a PObjectClass
 *
 * @param klass The PObjectClass
 * @param name The method
 */
#define P_METHOD(klass,name)			\
	klass##_##name##_##t name

/**
 * @brief Calls a method of a PObject
 *
 * When the method isn't initialized, a runtime error is raised
 * and the application aborts.
 * @param x The PObject
 * @param msg The method to call
 * @param ... Additional parameters of the method
 * @return This is dependent on the method
 */
#define P_CALL(x,msg,...)						\
	(assertTrue((x)->class->msg,					\
		    "runtime error: %s at %p doesn't respond to %s.",	\
		    (x)->class->name, (void *)x, __STRING(msg)),	\
	 (x)->class->msg(x, ## __VA_ARGS__))

/**
 * @brief Checks if the supplied argument is a PObjectClass
 *
 * This check succeeds if <code>x != NULL</code> and <code>
 * x->magic == @ref P_CLASS_MAGIC</code>, otherwise an assertion fails
 * and the application aborts.
 * @param x The PObjectClass to check
 * @return x
 */
#define P_ISCLASS(x)							\
	(assert(x),							\
	 assert(((struct PObjectClass *)x)->magic == P_CLASS_MAGIC), x)

/**
 * @brief Checks if the supplied argument is a PObject
 *
 * This check succeeds if <code>x != NULL</code> and <code>
 * @ref P_ISCLASS(x->class)</code>, otherwise an assertion fails
 * and the application aborts.
 * @param x The PObject to check
 * @return x
 */
#define P_ISOBJECT(x)							\
	(assert(x),							\
	 assert(((struct PObject *)x)->class),				\
	 assert(((struct PObject *)x)->class->magic == P_CLASS_MAGIC),	\
	 x)

/**
 * @brief Defines a PObject
 *
 * This macro declares the following:
 * - a function with the PObject name that returns the PObjectClass,
 *   this is defined by @ref P_OBJECT.
 * - a Class structure
 * - an Object structure
 *
 * This macro depends on the declarations of &lt;classname&gt;_Class and
 * &lt;classname&gt;_Attr.
 * @param klass The Class name
 * @param supper The Class name of the super class
 */
#define P_CLASS(klass,supper)						\
	struct klass##Class *klass();					\
	struct klass##Class {						\
		struct supper##Class *super;				\
		klass##_Class;						\
	};								\
	struct klass {							\
		const struct klass##Class *class;			\
		klass##_Attr;						\
	}

#define P_OBJECT(klass,supper)						\
	struct klass##Class *klass() {					\
		static struct klass##Class _self;			\
		static struct klass##Class *self = NULL;		\
		if (!self) {						\
			struct supper##Class *super_class = P_SUPER;	\
			self = &_self;					\
			memcpy(self, super_class, sizeof(struct supper##Class)); \
			self->super = super_class;			\
			self->size = sizeof(struct klass);		\
			self->name = #klass

#define P_END_OBJECT							\
	}								\
		return self;						\
		}

/* These macros are dependent on DEBUG */
#ifdef P_DEBUG
#define P_CAST(x,y) p_cast(x,y)
#define P_CAST_CLASS(x,y) p_cast_class(x,y)
#define P_DEBUG_IMPLEMENT(klass,type,name,args)				\
	fprintf (stderr, "%s %s_%s %s: %s at %d\n", #type, #klass, #name, #args, __FILE__, __LINE__)
#else				/* #ifdef P_DEBUG */
#define P_CAST(x,y) (x)
#define P_CAST_CLASS(x,y) (x)
#define P_DEBUG_IMPLEMENT(klass,type,name,args)
#endif

/**
 * @brief Constructor
 *
 * The constructor will be called for any PObject created with p_new()
 * or initialized with p_init(). Each time the constructor is 
 * overwritten, the super constructor has to be called explicitly
 * like this:
 *
 * <code>
 * self = P_SUPER()->ctor(self, app);
 * </code>
 */
P_METHOD_DEF(PObject, void *, ctor, (void *_self, va_list * app));
P_METHOD_DEF(PObject, void *, dtor, (void *_self));
P_METHOD_DEF(PObject, void *, clone, (void *_self));
P_METHOD_DEF(PObject, struct PString *, toString, (void *_self));

#define PObject_Class							\
	long magic;							\
	size_t size;							\
	char *name;							\
	P_METHOD (PObject, ctor);					\
	P_METHOD (PObject, dtor);					\
	P_METHOD (PObject, clone);					\
	P_METHOD (PObject, toString)

#define PObject_Attr

P_CLASS(PObject,PObject);

void *p_new(const void *_class, ...);
void *p_new_with_ctor(const void *_class, PObject_ctor_t ctor, ...);
void *p_init(void *_self, const void *_class, ...);
void *p_alloc(const void *_class);
void p_delete(void *_self);
void *p_cast(void *_self, void *_class);
int p_isA(void *_self, void *_class);
int p_isOf(void *_self, void *_class);
void *p_cast_class(void *_self, void *_class);
void *p_super_ctor(void *_self, void *_super, ...);

#endif				/* OBJECT_H */

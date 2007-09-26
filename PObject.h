#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "utils.h"

#define P_CLASS_MAGIC 0xCAFECAFE

#define P_IMPLEMENT(klass,type,name,args)						\
  type klass##_##name args

#define P_METHOD_DEF(klass,type,name,args)						\
  typedef type  (*klass##_##name##_##t) args

#define P_METHOD(klass,name)								\
  klass##_##name##_##t name

#define P_CALL(x,msg,...)								\
	(assertTrue((x)->class->msg,					\
		    "runtime error: %s at %p doesn't respond to %s.",			\
		    (x)->class->name, (void *)x, __STRING(msg)),	\
	 (x)->class->msg(x, ## __VA_ARGS__))

#define P_ISCLASS(x) (assert(x),							\
		    assert(((struct PObjectClass *)x)->magic == P_CLASS_MAGIC), x)
#define P_ISOBJECT(x) (assert(x),							\
		     assert(((struct PObject *)x)->class),				\
		     assert(((struct PObject *)x)->class->magic == P_CLASS_MAGIC),	\
		     x)

#define P_CLASS(klass,supper)								\
struct klass##Class *klass();								\
struct klass##Class {									\
	struct supper##Class *super;							\
	klass##_Class;									\
};											\
struct klass {										\
	const struct klass##Class *class;						\
	klass##_Attr;									\
}

#define P_OBJECT(klass,supper)								\
struct klass##Class *klass() {								\
	static struct klass##Class _self;						\
	static struct klass##Class *self = NULL;					\
	if (!self) {									\
		struct supper##Class *super_class = P_SUPER;				\
		self = &_self;								\
		memcpy(self, super_class, sizeof(struct supper##Class));		\
		self->super = super_class;						\
		self->size = sizeof(struct klass);					\
		self->name = #klass

#define P_END_OBJECT									\
	}										\
	return self;									\
}

/* These macros are dependent on DEBUG */
#ifdef P_DEBUG
#define P_CAST(x,y) p_cast(x,y)
#define P_CAST_CLASS(x,y) p_cast_class(x,y)
#define P_DEBUG_IMPLEMENT(klass,type,name,args) \
  fprintf (stderr, "%s %s_%s %s: %s at %d\n", #type, #klass, #name, #args, __FILE__, __LINE__)
#else				/* #ifdef P_DEBUG */
#define P_CAST(x,y) (x)
#define P_CAST_CLASS(x,y) (x)
#define P_DEBUG_IMPLEMENT(klass,type,name,args)
#endif

P_METHOD_DEF(PObject, void *, ctor, (void *_self, va_list * app));
P_METHOD_DEF(PObject, void *, dtor, (void *_self));
P_METHOD_DEF(PObject, void *, clone, (void *_self));
P_METHOD_DEF(PObject, struct PString *, toString, (void *_self));

#define PObject_Class									\
  long magic;										\
  size_t size;										\
  char *name;										\
  P_METHOD (PObject, ctor);								\
  P_METHOD (PObject, dtor);								\
  P_METHOD (PObject, clone);								\
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

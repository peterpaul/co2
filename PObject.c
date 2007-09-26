#include "PObject.h"
#include "PString.h"

P_IMPLEMENT(PObject, struct PString *, toString, (void *_self))
{
	P_DEBUG_IMPLEMENT(PObject, struct PString *, toString, (void *_self));
	struct PObject *self = P_CAST(_self, PObject());
	return p_new (PString(), "%s at %p", self->class->name, _self);
}

P_IMPLEMENT(PObject, void *, ctor, (void *_self, va_list * app))
{
	P_DEBUG_IMPLEMENT(PObject, void *, ctor,
			  (void *_self, va_list * app));
	struct PObject *self = P_CAST(_self, PObject());
	return self;
}

void *p_super_ctor(void *_self, void *_super, ...)
{
	struct PObject *self = P_CAST(_self, PObject());
	struct PObjectClass *super = P_CAST_CLASS(_super, PObject());
	va_list ap;
	va_start(ap, _super);
	super->ctor(self, &ap);
	va_end(ap);
	return self;
}

P_IMPLEMENT(PObject, void *, dtor, (void *_self))
{
	P_DEBUG_IMPLEMENT(PObject, void *, dtor, (void *_self));
	struct PObject *self = P_CAST(_self, PObject());
	return self;
}

P_IMPLEMENT(PObject, void *, clone, (void *_self))
{
	P_DEBUG_IMPLEMENT(PObject, void *, dtor, (void *_self));
	struct PObject *self = P_CAST(_self, PObject());
	struct PObject *clone = p_alloc(self->class);
	memcpy (clone, self, self->class->size);
	return clone;
}

struct PObjectClass *PObject()
{
	/* return a class */
	static struct PObjectClass _self;
	static struct PObjectClass *self = NULL;
	if (!self) {
/* 		self = calloc(1, sizeof(struct PObjectClass)); */
		self = &_self;
		self->super = self;
		self->magic = P_CLASS_MAGIC;
		self->size = sizeof(struct PObject);
		self->name = "PObject";
		self->ctor = PObject_ctor;
		self->dtor = PObject_dtor;
		self->clone = PObject_clone;
		self->toString = PObject_toString;
	}
	return self;
}

void *p_new(const void *_class, ...)
{
	va_list ap;
	struct PObject *self = p_alloc(_class);

	va_start(ap, _class);
	P_CALL(self, ctor, &ap);
	va_end(ap);

	return self;
}

void *p_new_with_ctor(const void *_class, PObject_ctor_t ctor, ...)
{
	va_list ap;
	struct PObject *self = p_alloc(_class);

	va_start(ap, ctor);
	ctor (self, &ap);
	va_end(ap);

	return self;
}

void *p_init(void *_self, const void *_class, ...)
{
	va_list ap;
	struct PObject * self = _self;
	const struct PObjectClass *class = P_ISCLASS(_class);

	assert (self);
	self->class = class;
	va_start(ap, _class);
	P_CALL(self, ctor, &ap);
	va_end(ap);
	return self;
}

void *p_alloc(const void *_class)
{
	const struct PObjectClass *class = P_ISCLASS(_class);
	struct PObject *self = calloc(1, class->size);
	assert (self);
	self->class = (struct PObjectClass *) class;
	return self;
}

void p_delete(void *_self)
{
	struct PObject *self = P_ISOBJECT(_self);
	P_CALL(self, dtor);
	free(self);
}

void *p_cast_class(void *_self, void *_class)
{
	struct PObjectClass *myClass = P_ISCLASS(_self);
	struct PObjectClass *class = P_ISCLASS(_class);
	void *object = PObject();
	while (myClass != class && myClass != object)
		myClass = myClass->super;
	if (myClass != class) {
		fflush(stdout);
		fprintf(stderr, "runtime error: Couldn't cast %s to %s.\n",
			((struct PObjectClass *)_self)->name, class->name);
		fflush(stderr);
		assert(myClass == class);
	}
	return _self;
}

void *p_cast(void *_self, void *_class)
{
	struct PObject *self = P_ISOBJECT(_self);
	struct PObjectClass *class = P_ISCLASS(_class);
	const struct PObjectClass *myClass = self->class;
	void *object = PObject();
	while (myClass != class && myClass != object)
		myClass = myClass->super;
	if (myClass != class) {
		fflush(stdout);
		fprintf(stderr, "runtime error: Couldn't cast %s to %s.\n",
			self->class->name, class->name);
		fflush(stderr);
		assert(myClass == class);
	}
	return _self;
}

int p_isA(void *_self, void *_class)
{
	struct PObject *self = P_ISOBJECT(_self);
	struct PObjectClass *class = P_ISCLASS(_class);
	const struct PObjectClass *myClass = self->class;
/* 	void *object = PObject(); */
/* 	while (myClass != class && myClass != object) */
/* 		myClass = myClass->super; */
	return myClass == class;
}

int p_isOf(void *_self, void *_class)
{
	struct PObject *self = P_ISOBJECT(_self);
	struct PObjectClass *class = P_ISCLASS(_class);
	const struct PObjectClass *myClass = self->class;
	void *object = PObject();
	while (myClass != class && myClass != object)
		myClass = myClass->super;
	return myClass == class;
}

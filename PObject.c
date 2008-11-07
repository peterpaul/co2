#include "PObject.h"
#include "PString.h"
#include "PInterface.h"

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

P_IMPLEMENT(PObject, void *, getInterface, (void *_self, const char * interface))
{
	P_DEBUG_IMPLEMENT(PObject, void *, get_interface, (void *_self, const char * interface));
	struct PObject * self = P_CAST(_self, PObject());
	struct PInterface * head = self->class->interfaces;
	
	while (head && strcmp(head->name, interface) != 0) {
		head = head->next;
	}
	return head;
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
		p_add_class(self);
		self->ctor = PObject_ctor;
		self->dtor = PObject_dtor;
		self->clone = PObject_clone;
		self->toString = PObject_toString;
		self->getInterface = PObject_getInterface;
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

/* *** Struct for class hashmap *** */
struct ClassHashmapTuple {
	struct ClassHashmapTuple * next;
	struct PObjectClass * class;
};

static struct ClassHashmapTuple * p_new_class_hashmap_tuple(struct ClassHashmapTuple * next, struct PObjectClass * class)
{
	assertTrue(class != NULL, "parameter class may not be NULL");

	struct ClassHashmapTuple * tuple = calloc (1, sizeof(struct ClassHashmapTuple));
	assertTrue(tuple != NULL, "failed allocating memory for struct ClassHashmapTuple");

	tuple->next = next;
	tuple->class = class;
	return tuple;
}

static struct ClassHashmapTuple * p_find_class_hashmap_tuple(struct ClassHashmapTuple * head, const char * class_name)
{
	while (head != NULL
	       && strcmp(class_name, head->class->name) != 0) {
		head = head->next;
	}
	return head;
}

/* *** Functions for class registration *** */
#define CLASS_HASHMAP_SIZE 1024
struct ClassHashmapTuple ** class_hashmap = NULL;

void p_add_class(void * _class)
{
	struct PObjectClass * class = P_ISCLASS(_class);

	if (!class_hashmap) {
		/* create class_hashmap */
		class_hashmap = calloc (CLASS_HASHMAP_SIZE, sizeof (struct ClassHashmapTuple *));
	}

	unsigned long index = hash_function((unsigned char *)class->name) % CLASS_HASHMAP_SIZE;

	class_hashmap[index] = p_new_class_hashmap_tuple(class_hashmap[index], class);
}

void * p_get_class(const char * class_name)
{
	if (!class_hashmap) {
		return NULL;
	}

	unsigned long index = hash_function((unsigned char *)class_name) % CLASS_HASHMAP_SIZE;

	struct ClassHashmapTuple * tuple = p_find_class_hashmap_tuple(class_hashmap[index], class_name);

	return tuple->class;;
}

void p_print_classes(FILE * fp)
{
	int i;

	for (i = 0; i < CLASS_HASHMAP_SIZE; i++)
	{
		if (class_hashmap[i]) {
			fprintf(fp, "%s\n", class_hashmap[i]->class->name);
		}
	}
}

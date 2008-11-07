#include "PList.h"
#include "PString.h"

#define P_SUPER PObject()

/**
 * This implements a list where the entries may be empty.
 * Also, a type (class) may be provided, so that entries 
 * entered into the list can be cast to this type.
 */

P_IMPLEMENT(PList, void *, ctor, (void *_self, va_list * app))
{
	P_DEBUG_IMPLEMENT(PList, void *, ctor, (void *_self, va_list * app));
	struct PList *self = P_CAST(_self, PList());
	self = P_SUPER->ctor(self, app);
	self->max = va_arg(*app, unsigned);
	self->type = va_arg(*app, struct PObjectClass *);
	self->data = calloc(self->max, sizeof(void *));
	self->length = 0;
	return self;
}

P_IMPLEMENT(PList, void *, resize, (void *_self, unsigned size))
{
	P_DEBUG_IMPLEMENT(PList, void *, resize, (void *_self, unsigned size));
	struct PList *self = P_CAST(_self, PList());
	assert(size >= self->length);
	self->data = realloc(self->data, sizeof(void *[size]));
	self->max = size;
	return self;
}

P_IMPLEMENT(PList, void *, append, (void *_self, void *item))
{
	P_DEBUG_IMPLEMENT(PList, void *, append, (void *_self, void *item));
	struct PList *self = P_CAST(_self, PList());
	/* if item and type, cast item to type */
	if (item && self->type)
		p_cast (item, self->type);
	/* check if need to be resized */
	if (self->length == self->max)
		self = P_CALL(self, resize, self->max << 1);
	/* append item */
	self->data[self->length] = item;
	self->length++;
	return item;
}

static void PList_append_item (void *_item, va_list * ap)
{
	struct PList * list = p_cast (va_arg (*ap, struct PList *), PList ());
	P_CALL(list, append, _item);
}

P_IMPLEMENT(PList, void *, append_list, (void *_self, void *_list))
{
	P_DEBUG_IMPLEMENT(PList, void *, append_list, (void *_self, void *_list));
	struct PList *self = P_CAST(_self, PList());
	struct PList *list = p_cast(_list, PList());
	P_CALL(list, map_args, PList_append_item, self);
	return self;
}

P_IMPLEMENT(PList, void *, merge, (void *_self, void *_other))
{
	P_DEBUG_IMPLEMENT(PList, void *, merge, (void *_self, void *_other));
	struct PList *self = P_CAST(_self, PList());
	struct PList *other = P_CAST(_other, PList());
	P_CALL(self, append_list, other);
	/* delete other list */
	other->length = 0;
	p_delete (other);
	/* return result */
	return self;
}

P_IMPLEMENT(PList, void *, remove, (void *_self))
{
	P_DEBUG_IMPLEMENT(PList, void *, remove, (void *_self));
	struct PList *self = P_CAST(_self, PList());
	self->length --;
	return self->data[self->length];
}

P_IMPLEMENT(PList, void *, get, (void *_self, unsigned index))
{
	P_DEBUG_IMPLEMENT(PList, void *, get, (void *_self, unsigned index));
	struct PList *self = P_CAST(_self, PList());
	assert(index < self->length);
	return self->data[index];
}

P_IMPLEMENT(PList, void *, set, (void *_self, unsigned index, void *item))
{
	P_DEBUG_IMPLEMENT(PList, void *, set, (void *_self, unsigned index, void *item));
	struct PList *self = P_CAST(_self, PList());
	assert(index < self->length && item);
	return self->data[index] = self->type ? p_cast (item, self->type) : item;
}

P_IMPLEMENT(PList, void *, map, (void *_self, void (*fun) (void *)))
{
	struct PList *self = P_CAST(_self, PList());
	int i;
	const unsigned length = self->length;
	for (i = 0; i < length; i++) {
		fun(self->data[i]);
	}
	return self;
}

P_IMPLEMENT(PList, void *, map_args, (void *_self, void (*fun) (void *, va_list *), ...))
{
	struct PList *self = P_CAST(_self, PList());
	int i;
	va_list ap;
	va_start(ap, fun);
	const unsigned length = self->length;
	for (i = 0; i < length; i++) {
		va_list aq;
		va_copy (aq, ap);
		fun(self->data[i], &aq);
		va_end(aq);
	}
	va_end(ap);
	return self;
}

P_IMPLEMENT(PList, void *, filter, (void *_self, int (*filter) (void *)))
{
	P_DEBUG_IMPLEMENT(PList, void *, filter, (void *_self, int (*filter) (void *)));
	struct PList *self = P_CAST(_self, PList());
	int i;
	const unsigned length = self->length;
	struct PList *result = p_new(PList(), self->length / 2);
	for (i = 0; i < length; i++) {
		if (filter(self->data[i]))
			P_CALL(result, append, self->data[i]);
	}
	return self;
}

P_IMPLEMENT(PList, void *, filter_args, (void *_self, int (*filter) (void *, va_list *), ...))
{
	P_DEBUG_IMPLEMENT(PList, void *, filter_args, (void *_self, int (*filter) (void *, va_list), ...));
	struct PList *self = P_CAST(_self, PList());
	int i;
	va_list ap;
	va_start (ap, filter);
	const unsigned length = self->length;
	struct PList *result = p_new(PList(), self->length / 2);
	for (i = 0; i < length; i++) {
		va_list aq;
		va_copy(aq, ap);
		if (filter(self->data[i], &aq))
			P_CALL(result, append, self->data[i]);
		va_end(aq);
	}
	va_end(ap);
	return self;
}

/* hook for deletion of members */
static void PList_delete_items(void * item)
{
	if (p_isOf (item, PObject())) {
		p_delete (item);
	}
}

P_IMPLEMENT(PList, void *, dtor, (void *_self))
{
	P_DEBUG_IMPLEMENT(PList, void *, dtor, (void *_self));
	struct PList *self = P_CAST(_self, PList());
	P_CALL(self, map, PList_delete_items);
	free(self->data);
	self->data = NULL;
	return P_SUPER->dtor(self);
}

P_IMPLEMENT(PList, struct PString *, toString, (void *_self))
{
	P_DEBUG_IMPLEMENT(PList, struct PString *, toString, (void *_self));
	struct PList *self = P_CAST(_self, PList());
	struct PString *str = P_SUPER->toString(self);
	
	struct PString *enter = p_new (PString (), "\n");
	struct PString *item_enter = p_new (PString (), "\n\t");

	int i;
	const unsigned length = self->length;
	P_CALL(str, append_str, " [\n");
	for (i = 0; i < length; i++) {
		struct PObject * item = P_CALL(self, get, i);
		struct PString * item_str = P_CALL(item, toString);
		P_CALL(item_str, replace, enter, item_enter);
		P_CALL(str, append_str, "\t");
		P_CALL(str, append, item_str);
		p_delete (item_str);
		P_CALL(str, append_str, ";\n");
	}
	P_CALL(str, append_str, "]");

	p_delete (enter);
	p_delete (item_enter);

	return str;
}

P_OBJECT(PList,PObject);
self->ctor = PList_ctor;
self->dtor = PList_dtor;
self->resize = PList_resize;
self->append = PList_append;
self->append_list = PList_append_list;
self->merge = PList_merge;
self->remove = PList_remove;
self->get = PList_get;
self->set = PList_set;
self->map = PList_map;
self->map_args = PList_map_args;
self->filter = PList_filter;
self->filter_args = PList_filter_args;
self->toString = PList_toString;
P_END_OBJECT

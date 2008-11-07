#include "Hash.h"

#define P_SUPER PObject()

P_IMPLEMENT(PTuple, void *, ctor, (void *_self, va_list * app))
{
	P_DEBUG_IMPLEMENT(PTuple, void *, ctor, (void *_self, va_list * app));
	struct PTuple * self = P_CAST(_self, PTuple ());
	self = P_SUPER->ctor(self, app);
	self->key = (char *) strdup(va_arg(*app, char *));
	self->value = va_arg(*app, void *);
	p_cast(self->value, PObject ());
	return self;
}

P_IMPLEMENT(PTuple, void *, dtor, (void *_self))
{
	P_DEBUG_IMPLEMENT(PTuple, void *, dtor, (void *_self));
	struct PTuple * self = P_CAST(_self, PTuple ());
	free (self->key);
	return P_SUPER->dtor(self);
}

P_OBJECT(PTuple, PObject);
self->ctor = PTuple_ctor;
self->dtor = PTuple_dtor;
P_END_OBJECT

#undef P_SUPER
#define P_SUPER PObject()

static inline struct PTuple *search(const struct PHash *hash,
				   const char *key, unsigned long index)
{
	struct PTuple *head = hash->map[index];
	while (head && strcmp(head->key, key) != 0) {
		head = head->next;
	}
	return head;
}

static struct PTuple *remove_tuple(struct PTuple *head, const char *key)
{
	if (head) {
		if (strcmp(head->key, key) != 0) {
			/* not found */
			head->next = remove_tuple(head->next, key);
			return head;
		} else {
			/* found */
			struct PTuple *tuple = head->next;
			p_delete(head);
			return tuple;
		}
	} else
		return NULL;
}

P_IMPLEMENT(PHash, void *, add, (void *_self, char * key, void *_item))
{
	P_DEBUG_IMPLEMENT(PHash, void *, add, (void *_self, char * key, void *_item));
	struct PHash * self = P_CAST(_self, PHash());
	struct PObject * item = p_cast(_item, PObject());
	unsigned long index = hash_function((unsigned char *)key) % HASH_SIZE;
	struct PTuple * tuple;

	if (search (self, key, index)) {
		fprintf (stderr, "%s:%d: Adding item with duplicate key '%s'\n",
			 __FILE__,
			 __LINE__,
			 key);
		exit(EXIT_FAILURE);
	}

	tuple = p_new (PTuple(), key, item);
	tuple->next = self->map[index];
	self->map[index] = tuple;
	return item;
}

/**
 * Set the value of a Hash Tuple to a certain value or add it if it doesn't
 * exist in the Hash yet.
 */
P_IMPLEMENT(PHash, void *, set, (void *_self, char * key, void *_item))
{
	P_DEBUG_IMPLEMENT(PHash, void *, set, (void *_self, char * key, void *_item));
	struct PHash * self = P_CAST(_self, PHash());
	struct PObject * item = p_cast(_item, PObject());
	unsigned long index = hash_function((unsigned char *)key) % HASH_SIZE;
	struct PTuple * tuple;

	tuple = search (self, key, index);
	if (tuple) {
		tuple->value = item;
	} else {
		tuple = p_new (PTuple(), key, item);
		tuple->next = self->map[index];
		self->map[index] = tuple;
	}
	return item;
}

P_IMPLEMENT(PHash, void *, get, (void *_self, char * key))
{
	P_DEBUG_IMPLEMENT(PHash, void *, get, (void *_self, char * key));
	struct PHash * self = P_CAST(_self, PHash());
	unsigned long index = hash_function((unsigned char *)key) % HASH_SIZE;
	const struct PTuple *tuple = search(self, key, index);
	if (tuple) {
		return tuple->value;
	} else {
		return NULL;
	}
}

P_IMPLEMENT(PHash, void *, del, (void *_self, char * key))
{
	struct PHash * self = P_CAST (_self, PHash ());
	unsigned long index = hash_function((unsigned char *)key) % HASH_SIZE;
	self->map[index] = remove_tuple (self->map[index], key);
	return self;
}

P_IMPLEMENT(PHash, void *, dtor, (void *_self))
{
	P_DEBUG_IMPLEMENT(PHash, void *, dtor, (void *_self));
	struct PHash * self = P_CAST(_self, PHash());
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		struct PTuple * tuple = self->map[i];
		while (tuple) {
			struct PTuple * next = tuple->next;
			p_delete (tuple);
			tuple = next;
		}
	}
	return P_SUPER->dtor(self);
}

P_OBJECT(PHash, PObject);
self->add = PHash_add;
self->del = PHash_del;
self->set = PHash_set;
self->get = PHash_get;
self->dtor = PHash_dtor;
P_END_OBJECT


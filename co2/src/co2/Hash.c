#include "co2/Hash.h"

#define O_SUPER Object()

O_IMPLEMENT(Tuple, void *, ctor, (void *_self, va_list * app))
{
	struct Tuple *self = O_CAST(_self, Tuple());
	self = O_SUPER->ctor(self, app);
	self->key = (char *) strdup(va_arg(*app, char *));
	self->value = va_arg(*app, void *);
	o_cast(self->value, Object());
	return self;
}

O_IMPLEMENT(Tuple, void *, dtor, (void *_self))
{
	struct Tuple *self = O_CAST(_self, Tuple());
	free(self->key);
	return O_SUPER->dtor(self);
}

O_OBJECT(Tuple, Object);
O_OBJECT_METHOD(Tuple, ctor);
O_OBJECT_METHOD(Tuple, dtor);
O_END_OBJECT
#undef O_SUPER
#define O_SUPER Object()
    static
/* inline */
struct Tuple *search(const struct Hash *hash, const char *key,
		     unsigned long index)
{
	struct Tuple *head = hash->map[index];
	while (head && strcmp(head->key, key) != 0) {
		head = head->next;
	}
	return head;
}

static struct Tuple *remove_tuple(struct Tuple *head, const char *key)
{
	if (head) {
		if (strcmp(head->key, key) != 0) {
			/* not found */
			head->next = remove_tuple(head->next, key);
			return head;
		} else {
			/* found */
			struct Tuple *tuple = head->next;
			O_CALL(head, delete);
			return tuple;
		}
	} else
		return NULL;
}

O_IMPLEMENT(Hash, void *, add, (void *_self, const char *key, const void *_item))
{
	struct Hash *self = O_CAST(_self, Hash());
	struct Object *item = o_cast(_item, Object());
	unsigned long index = hash_function((unsigned char *) key) % HASH_SIZE;
	struct Tuple *tuple;

	if (search(self, key, index)) {
		O_CALL(self, error_already_declared, key, item);
	}

	tuple = O_CALL_CLASS(Tuple(), new, key, item);
	tuple->next = self->map[index];
	self->map[index] = tuple;
	return item;
}

/**
 * Set the value of a Hash Tuple to a certain value or add it if it doesn't
 * exist in the Hash yet.
 */
O_IMPLEMENT(Hash, void *, set, (void *_self, const char *key, const void *_item))
{
	struct Hash *self = O_CAST(_self, Hash());
	struct Object *item = o_cast(_item, Object());
	unsigned long index = hash_function((unsigned char *) key) % HASH_SIZE;
	struct Tuple *tuple;

	tuple = search(self, key, index);
	if (tuple) {
		tuple->value = item;
	} else {
		tuple = O_CALL_CLASS(Tuple(), new, key, item);
		tuple->next = self->map[index];
		self->map[index] = tuple;
	}
	return item;
}

O_IMPLEMENT(Hash, void *, get, (void *_self, const char *key))
{
	struct Hash *self = O_CAST(_self, Hash());
	unsigned long index = hash_function((unsigned char *) key) % HASH_SIZE;
	const struct Tuple *tuple = search(self, key, index);
	if (tuple) {
		return tuple->value;
	} else {
		return NULL;
	}
}

O_IMPLEMENT(Hash, void *, del, (void *_self, const char *key))
{
	struct Hash *self = O_CAST(_self, Hash());
	unsigned long index = hash_function((unsigned char *) key) % HASH_SIZE;
	self->map[index] = remove_tuple(self->map[index], key);
	return self;
}

O_IMPLEMENT(Hash, void *, dtor, (void *_self))
{
	struct Hash *self = O_CAST(_self, Hash());
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		struct Tuple *tuple = self->map[i];
		while (tuple) {
			struct Tuple *next = tuple->next;
			O_CALL(tuple, delete);
			tuple = next;
		}
	}
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(Hash, void, error_already_declared, (void *_self, const char * key, const void *_item))
{
	struct Hash *self = O_CAST(_self, Hash());
	fprintf(stderr, "Item with key '%s' already exists.\n", key);
	exit(EXIT_FAILURE);
}

O_OBJECT(Hash, Object);
O_OBJECT_METHOD(Hash, add);
O_OBJECT_METHOD(Hash, del);
O_OBJECT_METHOD(Hash, set);
O_OBJECT_METHOD(Hash, get);
O_OBJECT_METHOD(Hash, dtor);
O_OBJECT_METHOD(Hash, error_already_declared);
O_END_OBJECT

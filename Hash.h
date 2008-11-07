#ifndef Hash_H
#define Hash_H

#include "Object.h"

/* The Tuple is used to store key value pairs
 * and manage a list of Tuples.
 */
#define TupleClass_Attr							\
	ObjectClass_Attr

#define Tuple_Attr								\
	Object_Attr;								\
	char * key;									\
	struct Object * value;						\
	struct Tuple * next

O_CLASS(Tuple, Object);

/* The Hash */
#ifndef HASH_SIZE
#define HASH_SIZE 32
#endif

O_METHOD_DEF(Hash, void *, add, (void *_self, char * key, void * _item));
O_METHOD_DEF(Hash, void *, get, (void *_self, char * key));
O_METHOD_DEF(Hash, void *, set, (void *_self, char * key, void * _item));
O_METHOD_DEF(Hash, void *, del, (void *_self, char * key));

#define HashClass_Attr							\
	ObjectClass_Attr;							\
	O_METHOD (Hash, add);						\
	O_METHOD (Hash, get);						\
	O_METHOD (Hash, set);						\
	O_METHOD (Hash, del)

#define Hash_Attr								\
	Object_Attr;								\
	struct Tuple *map[HASH_SIZE]

O_CLASS(Hash, Object);

#endif /* Hash_H */

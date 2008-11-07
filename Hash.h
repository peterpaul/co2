#ifndef PHASH_H
#define PHASH_H

#include "PObject.h"

/* The Tuple is used to store key value pairs 
 * and manage a list of Tuples.
 */
#define PTuple_Class \
PObject_Class

#define PTuple_Attr \
PObject_Attr; \
char * key; \
struct PObject * value; \
struct PTuple * next

P_CLASS(PTuple, PObject);

/* The Hash */
#ifndef HASH_SIZE
#define HASH_SIZE 32
#endif

P_METHOD_DEF(PHash, void *, add, (void *_self, char * key, void * _item));
P_METHOD_DEF(PHash, void *, get, (void *_self, char * key));
P_METHOD_DEF(PHash, void *, set, (void *_self, char * key, void * _item));
P_METHOD_DEF(PHash, void *, del, (void *_self, char * key));

#define PHash_Class \
PObject_Class; \
P_METHOD (PHash, add); \
P_METHOD (PHash, get); \
P_METHOD (PHash, set); \
P_METHOD (PHash, del)

#define PHash_Attr \
PObject_Attr; \
struct PTuple *map[HASH_SIZE]

P_CLASS(PHash, PObject);

#endif /* PHASH_H */

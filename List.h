#ifndef LIST_H
#define LIST_H

#include "Object.h"
O_METHOD_DEF(List, void *, resize, (void *_self, unsigned size));
O_METHOD_DEF(List, void *, append, (void *_self, void *item));
O_METHOD_DEF(List, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF(List, void *, merge, (void *_self, void *_other));
O_METHOD_DEF(List, void *, remove, (void *self));
O_METHOD_DEF(List, void *, get, (void *_self, unsigned index));
O_METHOD_DEF(List, void *, set, (void *_self, unsigned index, void *item));
O_METHOD_DEF(List, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF(List, void *, map_args, (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF(List, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF(List, void *, filter_args, (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF(List, void *, destroy, (void *_self));

#define ListClass_Attr							\
	ObjectClass_Attr;							\
	O_METHOD (List, resize);					\
	O_METHOD (List, append);					\
	O_METHOD (List, append_list);				\
	O_METHOD (List, merge);					\
	O_METHOD (List, remove);					\
	O_METHOD (List, get);						\
	O_METHOD (List, set);						\
	O_METHOD (List, map);						\
	O_METHOD (List, map_args);					\
	O_METHOD (List, filter);					\
	O_METHOD (List, filter_args);				\
	O_METHOD (List, destroy)

#define List_Attr								\
	Object_Attr;								\
	struct Class * type;						\
	void **data;								\
	unsigned length, max

O_CLASS(List,Object);

#endif				/* not LIST_H */

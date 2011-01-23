#include "co2/Iterator.h"

#define O_SUPER Interface()

O_METHOD_IF(IList, void *, prepend, (void *_self, void *item), (_self, item));
O_METHOD_IF(IList, void *, append, (void *_self, void *item), (_self, item));
O_METHOD_IF(IList, void *, prepend_list, (void *_self, void *_other), (_self, _other));
O_METHOD_IF(IList, void *, append_list, (void *_self, void *_other), (_self, _other));
O_METHOD_IF(IList, void *, merge, (void *_self, void *_other), (_self, _other));
O_METHOD_IF(IList, void *, map, (void *_self, void (*fun) (void *)), (_self, fun));
O_METHOD_IF(IList, void *, map_args,
	    (void *_self, void (*fun) (void *, va_list *), ...), (_self, fun));
O_METHOD_IF(IList, void *, filter, (void *_self, int (*filter) (void *)), (_self, filter));
O_METHOD_IF(IList, void *, filter_args,
	    (void *_self, int (*filter) (void *, va_list *), ...), (_self, filter));
O_METHOD_IF(IList, void *, get_iterator, (void *_self), (_self));
O_METHOD_IF(IList, void *, get_first, (void *_self), (_self));
O_METHOD_IF(IList, void *, remove_first, (void *_self), (_self));
O_METHOD_IF(IList, void *, get_last, (void *_self), (_self));
O_METHOD_IF(IList, void *, remove_last, (void *_self), (_self));


O_OBJECT(Iterator, Interface);
O_END_OBJECT

#ifndef ILIST_H
#define ILIST_H

#include "co2/Interface.h"

/**
 * This method should add item to the beginning of the list.
 * When calling get_first it should return item.
 * @param item The item to prepend to the list
 * @return The prepended item
 */
O_METHOD_DEF(IList, void *, prepend, (void *_self, void *item));
/**
 * This method should add item at the end of the list.
 * When calling get_last it should return item.
 * @param item The item to append to the list
 * @return The appended item
 */
O_METHOD_DEF(IList, void *, append, (void *_self, void *item));
/**
 * This method should prepend a complete list to the list.
 * @param _other The list to prepend to the list
 * @return self
 */
O_METHOD_DEF(IList, void *, prepend_list, (void *_self, void *_other));
/**
 * This method should append a complete list at the end of the list.
 * @param _other The list to append to the list
 * @return self
 */
O_METHOD_DEF(IList, void *, append_list, (void *_self, void *_other));
/**
 * This method merges two lists, by appending _other to self.
 * List _other is deleted.
 * @param _other The list to merge with this list.
 * @return self
 */
O_METHOD_DEF(IList, void *, merge, (void *_self, void *_other));
/**
 * This method invokes a function on all the elements of the list.
 * Do not use this to delete items from the list.
 * @param fun The function to invoke for all elements of this list
 * @return self
 */
O_METHOD_DEF(IList, void *, map, (void *_self, void (*fun) (void *)));
/**
 * This method invokes a function on all the elements of the list, with the supplied extra arguments.
 * Do not use this to delete items from the list.
 * @param fun The function to invoke for all elements of this list
 * @param ... The arguments to pass to the function
 * @return self
 */
O_METHOD_DEF(IList, void *, map_args,
	     (void *_self, void (*fun) (void *, va_list *), ...));
/**
 * This method creates a new list with filtered elements.
 * The filter function is a function that returns FALSE (0) when the
 * given element should be added, and TRUE (!0) when the element should
 * not be added to the new list.
 * @param filter The filter function
 * @param The filtered list
 */
O_METHOD_DEF(IList, void *, filter, (void *_self, int (*filter) (void *)));
/**
 * This method creates a new list with filtered elements (@see "filter").
 * The filter function accepts more arguments.
 * @param filter The filter function
 * @return The filtered list
 */
O_METHOD_DEF(IList, void *, filter_args,
	     (void *_self, int (*filter) (void *, va_list *), ...));
/**
 * This method returns an implementation of the Iterator interface, to iterate this list.
 * @return Iterator for this list
 */
O_METHOD_DEF(IList, void *, get_iterator, (void *_self));
/**
 * This method returns the first element of this list.
 * @return The first element
 */
O_METHOD_DEF(IList, void *, get_first, (void *_self));
/**
 * This method removes the first element of the list, and returns it.
 * The element is not deleted.
 * @return The first element
 */
O_METHOD_DEF(IList, void *, remove_first, (void *_self));
/**
 * This method returns the last element of this list.
 * @return The last element
 */
O_METHOD_DEF(IList, void *, get_last, (void *_self));
/**
 * This method removes the last element of the list, and returns it.
 * The element is not deleted.
 * @return The last element.
 */
O_METHOD_DEF(IList, void *, remove_last, (void *_self));


#define IListClass_Attr			\
	InterfaceClass_Attr

#define IList_Attr				\
	Interface_Attr;				\
	O_METHOD(IList, prepend);		\
	O_METHOD(IList, append);		\
	O_METHOD(IList, prepend_list);		\
	O_METHOD(IList, append_list);		\
	O_METHOD(IList, merge);			\
	O_METHOD(IList, map);			\
	O_METHOD(IList, map_args);		\
	O_METHOD(IList, filter);		\
	O_METHOD(IList, filter_args);		\
	O_METHOD(IList, get_iterator);		\
	O_METHOD(IList, get_first);		\
	O_METHOD(IList, remove_first);		\
	O_METHOD(IList, get_last);		\
	O_METHOD(IList, remove_last)

O_CLASS(IList, Interface);

#endif /* ILIST_H */

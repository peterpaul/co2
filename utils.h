#ifndef UTILS_H
#define UTILS_H

#define message(x,...)									\
	__write_message(x, __FILE__, __LINE__,				\
					__ASSERT_FUNCTION, ## __VA_ARGS__)
#define assertFalse(p,msg,...)						\
	((p)											\
	 ? (message(msg, ## __VA_ARGS__),				\
	    __assert_fail("!" __STRING(p), __FILE__,	\
					  __LINE__, __ASSERT_FUNCTION))	\
	 : __ASSERT_VOID_CAST(0))
#define assertTrue(p,msg,...)							\
	((p)												\
	 ? __ASSERT_VOID_CAST(0)							\
	 : (message(msg, ## __VA_ARGS__),					\
	    __assert_fail(__STRING(p), __FILE__,			\
					  __LINE__, __ASSERT_FUNCTION)))

void __write_message(const char *fmt,
		     const char *file,
		     unsigned int line, const char *function, ...)
    __attribute__ ((format(printf, 1, 5)));

unsigned long hash_function(const unsigned char *str);
int ipow(int base, int exp);
unsigned upow(unsigned base, int exp);

#ifndef false
typedef int bool;
#define false 0
#define true !false
#endif

#define MAX_FUN(type) type max_##type(type a, type b){return (a > b ? a : b);}
#define MIN_FUN(type) type min_##type(type a, type b){return (a < b ? a : b);}

#endif				/* UTILS_H */

#ifndef PSTRING_H
#define PSTRING_H

#include "PObject.h"

P_METHOD_DEF(PString, void *, resize, (void *_self, int size));
P_METHOD_DEF(PString, void *, ensure, (void *_self, int size));
P_METHOD_DEF(PString, void *, append, (void *_self, struct PString *str));
P_METHOD_DEF(PString, void *, append_str, (void *_self, char *str, ...));
P_METHOD_DEF(PString, void *, append_str_n, (void *_self, char *str, int n));
P_METHOD_DEF(PString, void *, replace, (void *_self, struct PString *that, struct PString *this));
P_METHOD_DEF(PString, void *, ctor_from_file, (void *_self, va_list *app));
/**
 * @brief Prints this PString to a file.
 *
 * Upon  successful  return,  returns the number of characters printed (not 
 * including the '\\0' used to end output to strings).
 * @param _self The PString to print
 * @param fp The FILE to write to
 * @return The number of characters written to the FILE
 */
P_METHOD_DEF(PString, int, fprint, (void *_self, FILE *fp));
/**
 * @brief Prints this PString to a string.
 *
 * Upon  successful  return,  returns the number of characters printed (not 
 * including the '\\0' used to end output to strings). Does not write more than 
 * size bytes (including the '\\0').  If the output was truncated due to this 
 * limit then the return value is the number of characters (not including  the 
 * '\\0') which would have been written to the final string if enough space had 
 * been available. Thus, a return value of size or more means that the output 
 * was truncated.
 * @param _self The PString to print
 * @param str The string to write to
 * @param size The size of the target string
 * @return The number of characters written to the string
 */
P_METHOD_DEF(PString, int, snprint, (void *_self, char *str, int size));

#define PString_Class \
PObject_Class; \
P_METHOD (PString, resize); \
P_METHOD (PString, ensure); \
P_METHOD (PString, append); \
P_METHOD (PString, append_str); \
P_METHOD (PString, append_str_n); \
P_METHOD (PString, replace); \
P_METHOD (PString, fprint); \
P_METHOD (PString, snprint); \
P_METHOD (PString, ctor_from_file)

#define PString_Attr \
PObject_Attr; \
char * data; \
int length, max

P_CLASS (PString,PObject);

#endif

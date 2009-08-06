#ifndef _ERROR_H
#define _ERROR_H

#include "Token.h"

extern int errors;
extern int warnings;

int warning (const struct Token *, const char *, ...);
int error (const struct Token *, const char *, ...);

#endif /* _ERROR_H */

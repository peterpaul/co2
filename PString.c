#include "PString.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define P_SUPER PObject()

static size_t strnlen(const char * str, size_t n)
{
	size_t i;
	for (i = 0; str[i] && i < n; i++);
	return i;
}

P_IMPLEMENT(PString, void *, ctor_from_file, (void *_self, va_list *app))
{
	struct PString *self = P_CAST(_self, PString());
	self = P_SUPER->ctor(self, app);
	const char * filename = va_arg (*app, const char *);
	struct stat buffer;
	FILE *fp;

	if (stat (filename, &buffer) == -1) {
		P_SUPER->dtor(self);
		free (self);
		return NULL;
	}
	/* allocate buffer large enough to hold file contents */
	self->data = malloc (buffer.st_size + 1);
	self->max = buffer.st_size + 1;
	/* open the file */
	fp = fopen (filename, "r");
	if (fp == 0) {
		p_delete(self);
		return NULL;
	}
	/* read contents of file, and terminate string */
	self->length = fread (self->data, 1, buffer.st_size, fp);
	self->data[self->length] = '\0';
	/* close the file */
	fclose (fp);
	
	return self;
}

P_IMPLEMENT(PString, void *, ctor, (void *_self, va_list *app))
{
	P_DEBUG_IMPLEMENT(PString, void *, ctor, (void *_self, va_list *app));
	struct PString *self = P_CAST(_self, PString());
	self = P_SUPER->ctor(self, app);
	char * format = va_arg(*app, char *);
	int n, size = strlen (format) + 1;

	self->data = malloc (size);
	self->max = size;

	while (1) {
		va_list ap;
		va_copy(ap, *app);
		/* Try to print in the allocated space. */
		n = vsnprintf(self->data, size, format, ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)	/* glibc 2.1 */
			size = n + 1;	/* precisely what is needed */
		else		/* glibc 2.0 */
			size *= 2;	/* twice the old size */

		P_CALL(self, ensure, size);
		va_end(ap);
	}

	if (n > -1)
		self->length = n;
	else
		self->length = strlen (self->data);

	return self;
}

P_IMPLEMENT(PString, void *, resize, (void *_self, int size))
{
	P_DEBUG_IMPLEMENT(PString, void *, resize, (void *_self, int size));
	struct PString *self = P_CAST(_self, PString());
	self->data = realloc(self->data, size);
	self->max = size;
	return self;
}

P_IMPLEMENT(PString, void *, ensure, (void *_self, int size))
{
	P_DEBUG_IMPLEMENT(PString, void *, ensure, (void *_self, int size));
	struct PString *self = P_CAST(_self, PString());
	if (size > self->max)
		return P_CALL(self, resize, size);
	return self;
}

P_IMPLEMENT(PString, void *, append, (void *_self, struct PString *str))
{
	P_DEBUG_IMPLEMENT(PString, void *, append, (void *_self, struct PString *str));
	struct PString *self = P_CAST(_self, PString());
	P_CALL(self, ensure, self->length + str->length + 1);
	strcpy(self->data + self->length, str->data);
	self->length += str->length;
	return self;
}

P_IMPLEMENT(PString, void *, append_str, (void *_self, char *str, ...))
{
	P_DEBUG_IMPLEMENT(PString, void *, append_str, (void *_self, char *str));
	struct PString *self = P_CAST(_self, PString());
	int n, nn = strlen(str);
	va_list ap;
	P_CALL(self, ensure, self->length + nn + 1);	
	va_start(ap, str);
	while (1) {
		va_list ap2;
		va_copy(ap2, ap);
		P_CALL(self, ensure, nn + self->length);
		n = vsnprintf(self->data + self->length, nn, str, ap2);
		if (n > -1 && n < nn)
			break;
		if (n > -1)
			nn = n + 1;
		else
			nn *= 2;
		va_end(ap2);
	}
	va_end(ap);
	
	if (n > -1)
		self->length += n;
	else
		self->length = strlen (self->data);
/* 	strcpy(self->data + self->length, str); */
/* 	self->length += nn; */
	return self;
}

P_IMPLEMENT(PString, void *, append_str_n, (void *_self, char *str, int n))
{
	P_DEBUG_IMPLEMENT(PString, void *, append_str_n, (void *_self, char *str, int n));
	struct PString *self = P_CAST(_self, PString());
	if (n == 0)
		return self;
	int nn = strnlen(str, n);
/* 	if (nn > n) */
/* 		nn = n; */
	P_CALL(self, ensure, self->length + nn + 1);	
	strncpy(self->data + self->length, str, nn);
	self->length += nn;
	return self;
}

/**
 * This method replaces all occurrences of that with this.
 */
P_IMPLEMENT(PString, void *, replace, (void *_self, struct PString *that, struct PString *this))
{
	P_DEBUG_IMPLEMENT(PString, void *, replace, (void *_self, struct PString *that, struct PString *this));
	struct PString *self = P_CAST(_self, PString());
	/* backup original string data, and truncate self */
	struct PString *orig = p_new (PString(), self->data);
	self->length = 0;

	char *curr = orig->data;
	char *prev = orig->data;
	while ((curr = strstr (curr, that->data))) {
		P_CALL(self, append_str_n, prev, curr - prev);
		P_CALL(self, append, this);
		curr = curr + that->length;
		prev = curr;
	}
	P_CALL(self, append_str, prev);
	/* delete backup */
	p_delete(orig);
	return self;
}

P_IMPLEMENT(PString, int, fprint, (void *_self, FILE *fp))
{
	struct PString *self = P_CAST(_self, PString());
	return fprintf(fp, "%s", self->data);
}

P_IMPLEMENT(PString, int, snprint, (void *_self, char *str, int size))
{
	struct PString *self = P_CAST(_self, PString());
	return snprintf(str, size, "%s", self->data);
}

P_IMPLEMENT(PString, void *, clone, (void *_self))
{
	struct PString *self = P_CAST(_self, PString());
	struct PString *clone = P_SUPER->clone(self);
	clone->data = strdup(self->data);
	clone->max = self->length + 1;
	clone->length = self->length;
	return clone;
}

P_IMPLEMENT(PString, void *, dtor, (void *_self))
{
	P_DEBUG_IMPLEMENT(PString, void *, dtor, (void *_self));
	struct PString *self = P_CAST(_self, PString());
	free (self->data);
	self->data = NULL;
	return P_SUPER->dtor(self);
}

P_OBJECT(PString,PObject);
self->ctor = PString_ctor;
self->dtor = PString_dtor;
self->clone = PString_clone;
self->resize = PString_resize;
self->ensure = PString_ensure;
self->append = PString_append;
self->append_str = PString_append_str;
self->append_str_n = PString_append_str_n;
self->replace = PString_replace;
self->fprint = PString_fprint;
self->snprint = PString_snprint;
self->ctor_from_file = PString_ctor_from_file;
P_END_OBJECT

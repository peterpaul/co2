#include "String.h"
#include "List.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define O_SUPER Object()

static size_t strnlen(const char *str, size_t n)
{
	size_t i;
	for (i = 0; str[i] && i < n; i++);
	return i;
}

O_IMPLEMENT(String, void *, ctor_from_file, (void *_self, va_list * app),
	    (_self, app))
{
	struct String *self = O_CAST(_self, String());
	self = O_SUPER->ctor(self, app);
	const char *filename = va_arg(*app, const char *);
	struct stat buffer;
	FILE *fp;

	if (stat(filename, &buffer) == -1) {
		O_SUPER->dtor(self);
		free(self);
		return NULL;
	}
	/* allocate buffer large enough to hold file contents */
	self->data = malloc(buffer.st_size + 1);
	self->max = buffer.st_size + 1;
	/* open the file */
	fp = fopen(filename, "r");
	if (fp == 0) {
		self->class->delete(self);
		return NULL;
	}
	/* read contents of file, and terminate string */
	self->length = fread(self->data, 1, buffer.st_size, fp);
	self->data[self->length] = '\0';
	/* close the file */
	fclose(fp);

	return self;
}

O_IMPLEMENT(String, void *, ctor, (void *_self, va_list * app),
	    (_self, app))
{
	struct String *self = O_CAST(_self, String());
	self = O_SUPER->ctor(self, app);
	char *format = va_arg(*app, char *);
	int n, size = strlen(format) + 1;

	self->data = malloc(size);
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

		self->class->ensure(self, size);
		va_end(ap);
	}

	if (n > -1)
		self->length = n;
	else
		self->length = strlen(self->data);

	return self;
}

O_IMPLEMENT(String, void *, resize, (void *_self, int size), (_self, size))
{
	struct String *self = O_CAST(_self, String());
	self->data = realloc(self->data, size);
	self->max = size;
	return self;
}

O_IMPLEMENT(String, void *, ensure, (void *_self, int size), (_self, size))
{
	struct String *self = O_CAST(_self, String());
	if (size > self->max)
		return self->class->resize(self, size);
	return self;
}

O_IMPLEMENT(String, void *, append, (void *_self, struct String * str),
	    (_self, str))
{
	struct String *self = O_CAST(_self, String());
	self->class->ensure(self, self->length + str->length + 1);
	strcpy(self->data + self->length, str->data);
	self->length += str->length;
	return self;
}

O_IMPLEMENT(String, void *, append_str, (void *_self, char *str,...),
	    (_self, str))
{
	struct String *self = O_CAST(_self, String());
	int n, nn = strlen(str);
	va_list ap;
	self->class->ensure(self, self->length + nn + 1);
	va_start(ap, str);
	while (1) {
		va_list ap2;
		va_copy(ap2, ap);
		self->class->ensure(self, nn + self->length);
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
		self->length = strlen(self->data);
/* 	strcpy(self->data + self->length, str); */
/* 	self->length += nn; */
	return self;
}

O_IMPLEMENT(String, void *, append_str_n, (void *_self, char *str, int n),
	    (_self, str, n))
{
	struct String *self = O_CAST(_self, String());
	if (n == 0)
		return self;
	int nn = strnlen(str, n);
/* 	if (nn > n) */
/* 		nn = n; */
	self->class->ensure(self, self->length + nn + 1);
	strncpy(self->data + self->length, str, nn);
	self->length += nn;
	return self;
}

/**
 * This method replaces all occurrences of that with this.
 */
O_IMPLEMENT(String, void *, replace,
	    (void *_self, struct String * that, struct String * this),
	    (_self, that, this))
{
	struct String *self = O_CAST(_self, String());
	/* backup original string data, and truncate self */
	struct String *orig = String()->new(String(), self->data);
	self->length = 0;

	char *curr = orig->data;
	char *prev = orig->data;
	while ((curr = strstr(curr, that->data))) {
		self->class->append_str_n(self, prev, curr - prev);
		self->class->append(self, this);
		curr = curr + that->length;
		prev = curr;
	}
	self->class->append_str(self, prev);
	/* delete backup */
	orig->class->delete(orig);
	return self;
}

O_IMPLEMENT(String, int, fprint, (void *_self, FILE * fp), (_self, fp))
{
	struct String *self = O_CAST(_self, String());
	return fprintf(fp, "%s", self->data);
}

O_IMPLEMENT(String, int, snprint, (void *_self, char *str, int size),
	    (_self, str, size))
{
	struct String *self = O_CAST(_self, String());
	return snprintf(str, size, "%s", self->data);
}

O_IMPLEMENT(String, void *, clone, (void *_self), (_self))
{
	struct String *self = O_CAST(_self, String());
	struct String *clone = O_SUPER->clone(self);
	clone->data = strdup(self->data);
	clone->max = self->length + 1;
	clone->length = self->length;
	return clone;
}

O_IMPLEMENT(String, void *, dtor, (void *_self), (_self))
{
	struct String *self = O_CAST(_self, String());
	free(self->data);
	self->data = NULL;
	return O_SUPER->dtor(self);
}

O_IMPLEMENT(String, struct List *, split, (void *_self, const char * delim), (_self, delim))
{
	struct String *self = O_CAST(_self, String());
	struct List *list = List()->new(List(), 8, String());
	char * saveptr;
	char * str, * token, * data;
	data = strdup(self->data);
	for (str = data; ;str = NULL) {
		token = strtok_r(str, delim, &saveptr);
		if (token == NULL)
			break;

		list->class->append(list, String()->new(String, "%s", token));
	}
	free (data);
	return list;			    
}

O_OBJECT(String, Object);
O_OBJECT_METHOD(String, ctor);
O_OBJECT_METHOD(String, dtor);
O_OBJECT_METHOD(String, clone);
O_OBJECT_METHOD(String, resize);
O_OBJECT_METHOD(String, ensure);
O_OBJECT_METHOD(String, append);
O_OBJECT_METHOD(String, append_str);
O_OBJECT_METHOD(String, append_str_n);
O_OBJECT_METHOD(String, replace);
O_OBJECT_METHOD(String, fprint);
O_OBJECT_METHOD(String, snprint);
O_OBJECT_METHOD(String, ctor_from_file);
O_OBJECT_METHOD(String, split);
O_END_OBJECT

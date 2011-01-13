#include "CompileObjectVisitor.h"

#define O_SUPER Interface()

//O_METHOD_IF(CompileObjectVisitor, void *, getNext, (void *_self), (_self));
//O_METHOD_IF(CompileObjectVisitor, bool, hasNext, (void *_self), (_self));

O_METHOD_IF(CompileObjectVisitor, void, visit, (void *_self, struct CompileObject *object), (_self, object));

O_OBJECT(CompileObjectVisitor, Interface);
O_END_OBJECT

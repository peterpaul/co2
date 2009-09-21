#include "FunctionType.h"
#include "FunDeclaration.h"
#include "ArgDeclaration.h"
#include "FunctionCallExpression.h"
#include "io.h"

#define O_SUPER Type()

O_IMPLEMENT(FunctionType, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct FunctionType * self = O_CAST(_self, FunctionType());
  self = O_SUPER->ctor(self, app);
	self->return_type = o_cast(va_arg(*app, struct Type *), Type());
	O_CALL(self->return_type, retain);
	self->parameters = o_cast(va_arg(*app, struct RefList *), RefList());
	O_CALL(self->parameters, retain);
  return self;
}

static void FunctionType_ctor_get_parameter_type_from_decl(void *_decl, va_list *app)
{
	struct ArgDeclaration * decl = O_CAST(_decl, ArgDeclaration());
	struct RefList * list = o_cast(va_arg(*app, struct RefList *), RefList());
	O_CALL(list, append, decl->type);
}

O_IMPLEMENT(FunctionType, void *, ctor_from_decl, (void *_self, va_list *app), (_self, app))
{
  struct FunctionType * self = O_CAST(_self, FunctionType());
  self = O_SUPER->ctor(self, app);
  struct FunDeclaration * decl = o_cast(va_arg(*app, struct FunDeclaration *), FunDeclaration());
  self->return_type = O_CALL(decl->type, retain);
  self->parameters = O_CALL_CLASS(RefList(), new, decl->formal_arguments->length, Type());
  O_CALL(decl->formal_arguments, map_args, FunctionType_ctor_get_parameter_type_from_decl, self->parameters);
  return self;
}

static void FunctionType_ctor_get_parameter_type_from_expr(void *_decl, va_list *app)
{
	struct Expression * expr = O_CAST(_decl, Expression());
	struct RefList * list = o_cast(va_arg(*app, struct RefList *), RefList());
	O_CALL(list, append, expr->type);
}

O_IMPLEMENT(FunctionType, void *, ctor_from_expr, (void *_self, va_list *app), (_self, app))
{
  struct FunctionType * self = O_CAST(_self, FunctionType());
  self = O_SUPER->ctor(self, app);
  struct FunctionCallExpression * decl = o_cast(va_arg(*app, struct FunctionCallExpression *), FunctionCallExpression());
  self->return_type = O_CALL(decl->type, retain);
  self->parameters = O_CALL_CLASS(RefList(), new, decl->actual_arguments->length, Type());
  O_CALL(decl->actual_arguments, map_args, FunctionType_ctor_get_parameter_type_from_expr, self->parameters);
  return self;
}

O_IMPLEMENT(FunctionType, void *, dtor, (void *_self), (_self))
{
  struct FunctionType *self = O_CAST(_self, FunctionType());
	O_CALL(self->return_type, release);
	O_CALL(self->parameters, release);
  return O_SUPER->dtor(self);
}

static void FunctionType_generate_parameter(void *_parameter)
{
	struct Type * parameter = O_CAST(_parameter, Type());
	O_CALL(parameter, generate);
}

O_IMPLEMENT(FunctionType, void, generate, (void *_self), (_self))
{
	struct FunctionType *self = O_CAST(_self, FunctionType());
	O_CALL(self->return_type, generate);
	fprintf(out, "(*)(");
	O_CALL(self->parameters, map, FunctionType_generate_parameter);
	fprintf(out, ")");
}

#define SET_RESULT(x,p) (x = p ? x : p)

O_IMPLEMENT(FunctionType, bool, is_compatible, (void *_self, void *_other), (_self,_other))
{
	struct FunctionType *self = O_CAST(_self, FunctionType());
	if (O_SUPER->is_compatible(self, _other))
	{
		struct FunctionType *other = O_CAST(_other, FunctionType());
		bool result = true;
		bool tmp_result = O_CALL(self->return_type, is_compatible, other->return_type);
		SET_RESULT(result, tmp_result);
		if (self->parameters->length != other->parameters->length)
		  {
		    return false;
		  }
		int i;
		for (i = 0; i < self->parameters->length; i ++)
		  {
		    struct Type * parameter_self = O_CALL(self->parameters, get, i);
		    struct Type * parameter_other = O_CALL(other->parameters, get, i);
		    tmp_result = O_CALL(parameter_self, is_compatible, parameter_other);
		    SET_RESULT(result, tmp_result);
		  }
		return result;
	}
	return false;
}

O_OBJECT(FunctionType, Type);
O_OBJECT_METHOD(FunctionType, ctor);
O_OBJECT_METHOD(FunctionType, ctor_from_decl);
O_OBJECT_METHOD(FunctionType, ctor_from_expr);
O_OBJECT_METHOD(FunctionType, dtor);
O_END_OBJECT

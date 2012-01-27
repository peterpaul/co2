/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "co2/FixScopeVisitor.h"
  /* Declarations */
#include "co2/ClassDeclaration.h"

#define O_SUPER BaseCompileObjectVisitor ()

O_IMPLEMENT (FixScopeVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct FixScopeVisitor * self = O_CAST (_self, FixScopeVisitor ());
  self = O_SUPER->ctor (self, app);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);
  return self;
}

O_IMPLEMENT (FixScopeVisitor, void *, dtor, (void *_self))
{
  struct FixScopeVisitor *self = O_CAST (_self, FixScopeVisitor ());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  return O_SUPER->dtor (self);
}

O_IMPLEMENT_IF (FixScopeVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct ClassDeclaration *self = O_CAST (_object, ClassDeclaration ());
  if (self->superclass)
    {
      struct Declaration *_super_class =
	O_CALL_IF (IScope, self->scope, lookup, self->superclass);
      if (_super_class)
	{
	  /* TODO raise error when decl != ClassDeclaration () */
	  struct ClassDeclaration *super_class =
	    O_CAST (_super_class, ClassDeclaration ());
	  O_CALL_IF (IScope, self->member_scope, set_parent, super_class->member_scope);
	  fprintf (stderr, "fix scope: %s -> %s\n", self->name->name->data, super_class->name->name->data);
	}
      else
	{
	  error (self->name, "Could not find super_class: %s\n", self->superclass->name->data);
	}
    }
}

O_OBJECT (FixScopeVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD (FixScopeVisitor, ctor);
O_OBJECT_METHOD (FixScopeVisitor, dtor);
O_OBJECT_METHOD (FixScopeVisitor, visitClassDeclaration);
O_END_OBJECT

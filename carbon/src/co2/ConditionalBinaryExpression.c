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
#include "co2/ConditionalBinaryExpression.h"
#include "co2/Token.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Declaration.h"
#include "co2/ArrayType.h"
#include "co2/ObjectTypeDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/io.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER BinaryExpression()

O_IMPLEMENT (ConditionalBinaryExpression, void, generate, (void *_self))
{
  struct ConditionalBinaryExpression *self = O_CAST (_self, ConditionalBinaryExpression ());
  fprintf (out, "({");
  O_CALL (self->operand[0]->type, generate);
  fprintf (out, " __tmp__ = ");
  O_CALL (self->operand[0], generate);
  fprintf (out, "; __tmp__ ?");
  switch (self->operator->type)
    {
    case '.':
      {
	struct ObjectType * type = o_cast (self->operand[0]->type, ObjectType ());
	fprintf (out, "__tmp__");
	if (o_is_of (type->decl, StructDeclaration ()))
	  {
	    fprintf (out, ".");
	  }
	else
	  {
	    fprintf (out, "->");
	  }
	O_CALL (self->operand[1], generate_left, false);
      }
      break;
    case '[':
      fprintf (out, "__tmp__");
      O_CALL (self->operator, generate);
      O_CALL (self->operand[1], generate);
      fprintf (out, "]");
      break;
    default:
      error (self->operator, "Unsupported operator '%s' for ConditionalBinraryExpression\n", self->operator->name->data);
      break;
    }
  fprintf (out, " : (");
  O_CALL (self->type, generate);
  fprintf (out, ") 0;})");
}

O_OBJECT (ConditionalBinaryExpression, BinaryExpression);
O_OBJECT_METHOD (ConditionalBinaryExpression, generate);
O_END_OBJECT

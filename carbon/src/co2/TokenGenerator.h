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
#ifndef TokenGenerator_H
#define TokenGenerator_H

#include "co2/Singleton.h"
#include "co2/Token.h"

O_METHOD_DEF (TokenGenerator, struct Token *, create,
	      (void *_self, struct Token * token));

#define TokenGeneratorClass_Attr			\
  SingletonClass_Attr;					\
  O_METHOD(TokenGenerator, create)

#define TokenGenerator_Attr				\
  Singleton_Attr;					\
  int value

O_CLASS (TokenGenerator, Singleton);

#endif /* TokenGenerator_H */

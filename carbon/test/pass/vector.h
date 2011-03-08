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
#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <math.h>

#define infinity 1.0/0.0
/* #define epsilon 1e-12 */
#define epsilon 1e-3
typedef float real;
#define POW pow
#define SQRT sqrt
#define ABS fabs
#define TAN tan
#define SIN sin
#define COS cos
#define PI 3.14159

/***************************************************************/
/*** Vec object definition *************************************/
/***************************************************************/
typedef struct {
	real x, y, z;
} Vec;

static inline Vec vec(real x, real y, real z)
{
	Vec self;
	self.x = x;
	self.y = y;
	self.z = z;
	return self;
}
static inline Vec plus_v_v(Vec a, Vec b)
{
	return vec(a.x + b.x, a.y + b.y, a.z + b.z);
}
static inline Vec minus_v_v(Vec a, Vec b)
{
	return vec(a.x - b.x, a.y - b.y, a.z - b.z);
}
static inline Vec negate_v(Vec a)
{
	return vec(-a.x, -a.y, -a.z);
}
static inline Vec product_s_v(real a, Vec b)
{
	return vec(a * b.x, a * b.y, a * b.z);
}
static inline Vec product_v_v(Vec a, Vec b)
{
	return vec(a.x * b.x, a.y * b.y, a.z * b.z);
}
static inline Vec cross_v_v(Vec a, Vec b)
{
	return vec(a.y * b.z - a.z * b.y,
		   a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
static inline real dot_v_v(Vec a, Vec b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline real length_v(Vec a)
{
	return SQRT(dot_v_v(a, a));
}
static inline Vec norm_v(Vec a)
{
	return product_s_v(1. / length_v(a), a);
}
static inline Vec abs_v(Vec a)
{
	return vec(ABS(a.x), ABS(a.y), ABS(a.z));
}
static inline Vec chop_v(Vec a, real b)
{
	return vec((a.x < b ? a.x : b - epsilon),
		   (a.y < b ? a.y : b - epsilon),
		   (a.z < b ? a.z : b - epsilon));
}
static inline int print_v(Vec a)
{
	return printf("vec(%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
}
#endif				/* not VECTOR_H */

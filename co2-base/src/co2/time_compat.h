/*
 * libco2-base Library with basic datatypes in CO2/carbon
 *
 * Copyright (C) 2011,2012 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TIME_COMPAT_H
#define TIME_COMPAT_H

#include <time.h>

/*
 * carbon has no `long`/`time_t`-sized type -- LogRecord's Timeval.tv_sec is
 * declared (and generated) as `int`. On most platforms that's exactly
 * time_t's width, so `&tv_sec` can be handed straight to localtime(). MinGW-
 * w64 is the exception: its own <sys/time.h> keeps tv_sec as a 32-bit long
 * for BSD compatibility while its time_t is 64-bit, so reinterpreting a
 * tv_sec's address as `time_t *` reads 4 bytes of adjacent memory that
 * aren't part of the value, producing garbage (or a NULL-returning
 * localtime() call that then crashes strftime()). Converting the value
 * explicitly here -- an ordinary int-to-time_t promotion, not a pointer
 * reinterpretation -- sidesteps the width mismatch on every platform.
 */
struct tm *co2_localtime(int seconds);

#endif				/* TIME_COMPAT_H */

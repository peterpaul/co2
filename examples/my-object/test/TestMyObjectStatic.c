/*
 * my-object Example project for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
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
#include "MyObject.h"

int main (int argc, char ** argv)
{
	struct MyObject _object;
	struct MyObject *object = O_CALL_CLASS(MyObject(), init, &_object, 5);
	printf("object.value = %d\n", object->value);
	O_CALL(object, setValue, 13);
	printf("object.value = %d\n", O_CALL(object, getValue));
	O_CALL(object, dtor);
	return 0;
}

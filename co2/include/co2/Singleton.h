/*
 * Singleton.h
 *
 *  Created on: Oct 23, 2008
 *      Author: epetkle
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "co2/Object.h"

#define Singleton_Attr							\
	Object_Attr

#define SingletonClass_Attr						\
	ObjectClass_Attr;							\
	void * singleton

O_CLASS(Singleton, Object);

#endif				/* SINGLETON_H_ */

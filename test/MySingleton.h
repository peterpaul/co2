/*
 * MySingleton.h
 *
 *  Created on: Oct 20, 2008
 *      Author: epetkle
 */

#ifndef MYSINGLETON_H_
#define MYSINGLETON_H_

#include "Singleton.h"

#define MySingletonClass_Attr			\
	SingletonClass_Attr

#define MySingleton_Attr				\
	Singleton_Attr;						\
	int value

O_CLASS(MySingleton, Singleton);


#endif				/* MYSINGLETON_H_ */

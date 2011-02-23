/*
 * Singleton.h
 *
 *  Created on: Oct 23, 2008
 *      Author: epetkle
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "co2/Object.h"

O_METHOD_DEF (Singleton, void *, instance, (void *_self));

#define SingletonClass_Attr			\
  ObjectClass_Attr;				\
  O_METHOD (Singleton, instance);		\
  void * singleton

#define Singleton_Attr				\
  Object_Attr

O_CLASS(Singleton, Object);

#endif				/* SINGLETON_H_ */

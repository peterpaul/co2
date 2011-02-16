#!/bin/bash
set -x

LIBSOURCES="Array.c
ArrayIterator.c
ArrayList.c
Collection.c
Comparable.c
ConsoleHandler.c
Exception.c
Hash.c
IndexOutOfBoundsException.c
Iterable.c
Iterator.c
Level.c
List.c
ListIterator.c
LogFormatter.c
Logger.c
LogHandler.c
LogRecord.c
Object.c
OutOfMemoryException.c
RefObject.c
ReleasePool.c
ReleasePoolItem.c
SimpleFormatter.c
Singleton.c
String.c"

if [[ ($# > 0) && ("$1" == "clean") ]]
then
    rm *.[ch]
    rm a.out
    make clean
else
    make -k
    make -k
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0 $LIBSOURCES TestLogger.c -o TestLogger.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0 $LIBSOURCES TestRefObject.c -o TestRefObject.bin
fi

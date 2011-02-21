#!/bin/bash
set -x

LIBSOURCES="Array.c
ArrayIterator.c
ArrayList.c
Collection.c
Comparable.c
ConsoleHandler.c
Enum.c
Exception.c
HashMap.c
HashMapIterator.c
HashSet.c
IndexOutOfBoundsException.c
Iterable.c
Iterator.c
KeyValuePair.c
Level.c
List.c
ListIterator.c
LogFormatter.c
Logger.c
LogHandler.c
LogRecord.c
Map.c
Object.c
OutOfMemoryException.c
RefObject.c
ReleasePool.c
ReleasePoolItem.c
Set.c
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
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES TestLogger.c -o TestLogger.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES TestRefObject.c -o TestRefObject.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES TestMap.c -o TestMap.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES TestSet.c -o TestSet.bin
fi

#!/bin/bash
set -x

LIBSOURCES="co2/Array.c
co2/ArrayIterator.c
co2/ArrayList.c
co2/Collection.c
co2/Comparable.c
co2/ConsoleHandler.c
co2/Enum.c
co2/Exception.c
co2/HashMap.c
co2/HashMapIterator.c
co2/HashSet.c
co2/HashSetIterator.c
co2/IndexOutOfBoundsException.c
co2/Iterable.c
co2/Iterator.c
co2/KeyValuePair.c
co2/Level.c
co2/List.c
co2/ListIterator.c
co2/LogFormatter.c
co2/Logger.c
co2/LogHandler.c
co2/LogRecord.c
co2/Map.c
co2/Object.c
co2/OutOfMemoryException.c
co2/RefObject.c
co2/ReleasePool.c
co2/ReleasePoolItem.c
co2/Set.c
co2/SimpleFormatter.c
co2/String.c
co2/Throwable.c"

if [[ ($# > 0) && ("$1" == "clean") ]]
then
    make clean
    rm co2/*.[ch]
    rm TestLogger.bin TestRefObject.bin TestMap.bin TestSet.bin
else
    make -k
    make -k
    rm TestLogger.bin TestRefObject.bin TestMap.bin TestSet.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES co2/TestLogger.c -o TestLogger.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES co2/TestRefObject.c -o TestRefObject.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES co2/TestMap.c -o TestMap.bin
    gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.2/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.2/lib -lco2-1.0 $LIBSOURCES co2/TestSet.c -o TestSet.bin
fi

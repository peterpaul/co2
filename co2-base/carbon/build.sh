#!/bin/bash
set -x

LIBSOURCES="Array.c ArrayList.c Exception.c IndexOutOfBoundsException.c\
	Iterator.c List.c ListIterator.c Object.c OutOfMemoryException.c\
	RefObject.c ReleasePool.c ReleasePoolItem.c Singleton.c String.c\
	Level.c Logger.c LogFormatter.c LogHandler.c LogRecord.c\
	Comparable.c ConsoleHandler.c SimpleFormatter.c"

rm *.[ch]
rm a.out
make clean
make -k
make -k
gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0 $LIBSOURCES TestLogger.c -o TestLogger.bin
gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0 $LIBSOURCES TestRefObject.c -o TestRefObject.bin

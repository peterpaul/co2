rm *.[ch]
make -k -j3
make -k -j3
gcc -O0 -g3 -I/home/peterpaul/local/co2-0.0.1/include/co2-1.0 -L/home/peterpaul/local/co2-0.0.1/lib -lco2-1.0 *.c

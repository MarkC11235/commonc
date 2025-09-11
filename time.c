#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct timespec start, end; \
double time_taken_sec;      \
double time_taken_nsec;     
#define TIME_START clock_gettime(CLOCK_MONOTONIC, &start); 
#define TIME_END                                           \
   clock_gettime(CLOCK_MONOTONIC, &end);                  \
   time_taken_sec = (end.tv_sec - start.tv_sec);          \
   time_taken_nsec = (end.tv_nsec - start.tv_nsec) / 1e9; \
   printf("elapsed time: %f seconds\n", time_taken_sec + time_taken_nsec);
#define field(type, name, def) type name;
#define default_arg(type, name, def) .name = def,
#define unpack(type, name, def) type name = ops.name;
int bar(){
    TIME_START
    for(int i = 0; i < 100000000; i++){}
    TIME_END
}

int main(){
    bar();

    return 0;
}

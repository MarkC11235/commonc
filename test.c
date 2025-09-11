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
#include <stdio.h>
#include "common.h"

def_list(int);

struct {
    list(int) x;
    list(int) sum;
} sum_cache;

int sum(int x){
    int sum = 0;
    foreach(x_i, sum_cache.x,
        if(x_i == x){ 
            lget(sum_cache.sum, _iter, sum);
            return sum;
        }
    );
    for(int i = 1; i <= x; i++) sum += i;
    lpush(sum_cache.x, x); 
    lpush(sum_cache.sum, sum); 
    return sum;
}

int sum_nocache(int x){
    int sum = 0;
    for(int i = 1; i <= x; i++) sum += i;
    return sum;
}

void test_sum(int n, int runs, bool cache){
    TIME_START
    if(cache){
        for(int i = 0; i < runs; i++) {
            int s = sum(n);
            //printf("sum %d: %d\n", n, s);
        }
    }
    else{
        for(int i = 0; i < runs; i++) {
            int s = sum_nocache(n);
            //printf("sum %d: %d\n", n, s);
        }
    }
    TIME_END
}


int main(){
    int n = 1000, runs = 1000;
    printf("without cache\n");
    test_sum(n, runs, false);
    printf("with cache\n");
    test_sum(n, runs, true);
    
    return 0;
}

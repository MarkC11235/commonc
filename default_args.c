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

#define out_fields(_X) _X(int, n, 1) _X(char*, s, "") _X(int, indent, 0) 
typedef struct { out_fields(field) } out_options;
#define out(...) out_((out_options) { out_fields(default_arg) __VA_ARGS__ })
void out_(out_options ops){
    out_fields(unpack)
    for(int i = 0; i < n; i++){
        for(int j = 0; j < indent; j++){
            printf(" ");
        }
        printf("%s\n", s);
    }
}


#define foo_fields(_X) _X(int, a, 5) _X(int, b, 10) 
typedef struct { foo_fields(field) } foo_options;
#define foo(x, ...) foo_((foo_options) { foo_fields(default_arg) __VA_ARGS__ }, x)
int foo_(foo_options ops, int x){
    foo_fields(unpack)
    return x + a + b;
}


int main(){
    printf("%d\n", foo(7));
    printf("%d\n", foo(7, .a = 1));
    printf("%d\n", foo(7, .b = 45));

    out(.s = "Hello");

    return 0;
}

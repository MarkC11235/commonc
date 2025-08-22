#define FIELD(type, name, def) type name;
#define DEFAULT(type, name, def) .name = def,
#define UNPACK(type, name, def) type name = ops.name;
#include <stdio.h>

#define out_FIELDS(X) X(int, n, 1) X(char*, s, "") X(int, indent, 0) 
typedef struct { out_FIELDS(FIELD) } out_Options;
#define out(...) out_((out_Options) { out_FIELDS(DEFAULT) __VA_ARGS__ })
void out_(out_Options ops){
    out_FIELDS(UNPACK)
    for(int i = 0; i < n; i++){
        for(int j = 0; j < indent; j++){
            printf(" ");
        }
        printf("%s\n", s);
    }
}


#define foo_FIELDS(X) X(int, a, 5) X(int, b, 10) 
typedef struct { foo_FIELDS(FIELD) } foo_Options;
#define foo(x, ...) foo_((foo_Options) { foo_FIELDS(DEFAULT) __VA_ARGS__ }, x)
int foo_(foo_Options ops, int x){
    foo_FIELDS(UNPACK)
    return x + a + b;
}

int main(){
    printf("%d\n", foo(7));
    printf("%d\n", foo(7, .a = 1));
    printf("%d\n", foo(7, .b = 45));

    out(.s = "Hello");

    return 0;
}

#define field(type, name, def) type name;
#define default(type, name, def) .name = def,
#define unpack(type, name, def) type name = ops.name;
#include <stdio.h>

#define out_fields(x) x(int, n, 1) x(char*, s, "") x(int, indent, 0) 
typedef struct { out_fields(field) } out_options;
#define out(...) out_((out_options) { out_fields(default) __VA_ARGS__ })
void out_(out_options ops){
    out_fields(unpack)
    for(int i = 0; i < n; i++){
        for(int j = 0; j < indent; j++){
            printf(" ");
        }
        printf("%s\n", s);
    }
}


#define foo_fields(x) x(int, a, 5) x(int, b, 10) 
typedef struct { foo_fields(field) } foo_options;
#define foo(x, ...) foo_((foo_options) { foo_fields(default) __VA_ARGS__ }, x)
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

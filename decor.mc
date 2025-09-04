#include <stdio.h>

@default_args[(int, n, 1), (char*, s, ""), (int, indent, 0)]
void out(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < indent; j++){
            printf(" ");
        }
        printf("%s\n", s);
    }
}


@default_args[(int, a, 5), (int, b, 10)]
int foo(int x){
    return x + a + b;
}

int main(){
    printf("%d\n", foo(7));
    printf("%d\n", foo(7, .a = 1));
    printf("%d\n", foo(7, .b = 45));

    out(.s = "Hello");

    return 0;
}

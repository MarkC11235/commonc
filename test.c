#include "common.h"

def_list(int);
def_string;

void foo(list(int) *l){
    lset(*l, 5, 757575);
}

int main(){

    INFO("Welcome to wacky C stuff");
    ERROR("ABORT");

    


/*
    list(int) l = lcreate(int, 10);
    for(int i = 0; i < 30; i++){
        lpush(l, i);
    }

    foo(&l);

    lprint(l, itostr);

    foreach(num, l, 
        printf("%d\n", num);
    );

    int num; lget(l, 10, num);

    INFO("%d", num);

    ldestroy(l);


    string str = screate(10);
    sappend(str, "Mark ");
    sappend(str, "Castle"); 
    sprint(str);

    foreach(c, str, 
        printf("%c\n", c);
    );

    sdestroy(str);


    TODO("Add more macros like: slice, sorting, etc.");

    INFO("%d", num);
*/

    return 0;
}

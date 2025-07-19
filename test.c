#include "common.h"

def_list(int);
def_string;

int main(){
    
    list(int) l = lcreate(int, 10);
    for(int i = 0; i < 30; i++){
        lpush(l, i);
    }
    lprint(l, itostr);

    ldestroy(l);


    string str = screate(10);
    sappend(str, "Mark ");
    sappend(str, "Castle"); 
    sprint(str);

    sdestroy(str);

    return 0;
}

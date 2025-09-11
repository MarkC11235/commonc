@time
int bar(){
    for(int i = 0; i < 100000000; i++){}
}

int main(){
    bar();

    return 0;
}

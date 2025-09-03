#include <iostream>

void div2(int a, int b, int c, int d, int *e , int *f){
    *e = a * d;
    *f = b * c;
}

int main(){
    int *nom, *div;
    div2(1,2,3,4,nom,div);
    printf("%d,%d\n",*nom,*div);
}
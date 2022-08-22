#include <stdio.h>

int main(){
    int a[] = {0 ,1 ,2 ,1 ,4, 5};
    int b[] = {2 ,3 ,4 ,5};
    int l_a = 6;
    int l_b = 4;
    int i,j;
    int m,n;
    int length;
    int max = 0;

    for (i = 0; i < l_a; i++) {
        for (j = 0; j < l_b; j++) {
            m = i;
            n = j;
            length = 0;
            while (a[m] == b[n]) {
                m++;
                n++;
                length++;
            }
            if (length > max){
                max = length;
            }
        }
    }
    printf("max:%d\n",max);
    return 0;
}
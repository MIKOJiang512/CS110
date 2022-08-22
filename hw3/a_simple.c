#include <stdio.h>

int main(){
    int a[] = {0 ,1 ,2 ,3 ,4, 5};
    int b[] = {2 ,3 ,1 ,5};
    int l_a = 6;
    int l_b = 4;
    int dp[7][5] = {0};//l_a + 1
    int dpl[35] = {0};
    int max = 0;
    int i = 0,j=0;
    int p0, p1;
    
    loop1:
        j = 0;
        loop2:
            p0 = i*(l_b+1) + j;//[i][j]
            p1 = (i+1)*(l_b+1) + j+1;//[i+1][j+1]
            if (a[i] == b[j]){
                dpl[p1] = dpl[p0] + 1;
                //dp[i+1][j+1] = dp[i][j] + 1;
            }
            if (a[i] != b[j]){
                dpl[p1] = 0;
                //dp[i+1][j+1] = 0;
            }
            if(max < dpl[p1]){
                max = dpl[p1];
                //max = dp[i+1][j+1];
            }
            j = j + 1;
            if (j < l_b) {
                goto loop2;
            }
    /////////    
        i = i + 1;
        if (i < l_a) {
         goto loop1;
        }
    

    printf("max:%d",max);
    return 0;
}
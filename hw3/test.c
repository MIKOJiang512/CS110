#include <stdio.h>

int main(){
    int a[] = {0 ,1 ,2 ,0 ,4, 5};
    int b[] = {2 ,3 ,4 ,5};
    int l_a = 6;
    int l_b = 4;
    int dp[7][5] = {0};//l_a + 1
    int max = 0;
    for (int i = 0 ;i < l_a;i++){
        for (int j = 0; j < l_b; j++) {
            if (a[i] == b[j]) {
                dp[i+1][j+1] = dp[i][j] + 1;
            }
            else {
                dp[i+1][j+1] = 0;
            }
            if(max < dp[i+1][j+1]){
                max = dp[i+1][j+1];
            }
        }
    }

    printf("max:%d",max);





    return 0;
}
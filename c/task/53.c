#include<stdio.h>
int main(){
    int n;
    printf("enter value raw:");
    scanf("%d",&n);
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
          if (j <= n - i) {
                printf(" ");
            } else {
                printf("%d", n+1-j);
            }
           
           
        }
        printf("\n");
    }
    
return 0;
}
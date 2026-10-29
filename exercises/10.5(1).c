#include <stdio.h>
//f(n) = 1 + 1/2 + 1/3 +...+1/n
int  main(){
    int n;
    double sum = 0;
    scanf("%d",&n);
    for (int i = 1; i <= n; i++)
    {
        if (i%2 != 0)
        {
            sum += 1.0/i;
        }
        else
        {
            sum -= 1.0/i;
        }
        
    }
    printf("f(n) = %f",sum);
/*    int n;
    float count = 0;
    scanf("%d",&n);
    for (float i = 1.0; i <= n; i++)
    {
        count += 1/i;
    }
    printf("f(n) = %f",count);
*/
}

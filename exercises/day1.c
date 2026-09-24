#include <stdio.h>

int main()
{
    double y = 0,x = 0;
    scanf("%lf",&x);
    switch(x<0){
    case 1: y = -1;break;
    case 0: y = x * 2;
    break;
    }
    printf("%d",y);
    
    return 0;
}
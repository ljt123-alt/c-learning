#include <stdio.h>

int main(){
    int x = 1;
    int i ;
    while (x <= 100)
    {
        for ( i = 2; i < x; i++)
        {
            if (x%i == 0)
            {
                break;
            }
            
        }
        if (i == x)
        {
            printf("%d ",x);
        }
        x++;
    }
    
}
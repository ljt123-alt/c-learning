#include <stdio.h>

int main(){
    int number = 0;
    int i = 0;
    scanf("%d",&number);

    for (i = 2; i < number; i++)
    {
        if (number%i == 0)
        {
            printf("%d不是素数\n",number);
            break;
        }
    }
    if (i == number)
    {
        printf("%d是素数\n",number);
    }
    else if(i > number)
    {
        printf("%d不是素数\n",number);
    }
}

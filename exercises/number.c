#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(0));
    int number = rand()%100+1;
    int count = 0;
    int a = 0;
    printf("我已经想好了一个1到100之间的数。\n");
    do
    {
        printf("请输入猜测的数字：");
        scanf("%d",&a);
        count++;
        if (a > number)
        {
            printf("猜大了\n");
        }
        else if(a < number)
        {
            printf("猜小了\n");
        }
    } while (a != number);
    printf("恭喜你只用了%d次就猜出了答案\n",count);
    return 0;
}
#include <stdio.h>

int main(){
    int number = 0;
    double average = 0;
    int sum =0;
    int n = 0;
    do
    {
        printf("请输入数字：");
        scanf("%d",&number);
        if (number > 0)
        {
            sum += number;
            n++;
        }
        
    } while (number != -1);
    average = 1.0*sum/n;
    printf("一共输入%d个正整数,平均数为%f",n,average);
    return 0;
}
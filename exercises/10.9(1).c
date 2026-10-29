#include <stdio.h>

int main(){
    int n,i,a[5];
    int sum = 0;
    int max;
    int min;
    double average;
    printf("请输入5个整数:");
    scanf("%d",&n);
    a[0] = n;
    max = a[0];
    min = a[0];
    sum += a[0];
    for ( i = 1; i < 5; i++)
    {
        scanf("%d",&n);
        a[i] = n;
        sum += a[i];
        if (a[i] > max)
        {
            max = a[i];
        }
        if (a[i] < min)
        {
            min = a[i];
        }
    }
    average = sum/5.0;
    printf("数组元素：");
    for ( i = 0; i < 5; i++)
    {
        if (i < 4)
        {
            printf("%d ",a[i]);
        }
        else{
            printf("%d\n",a[i]);
        }
    }
    printf("总和：%d\n",sum);
    printf("平均值：%.2f\n",average);
    printf("最大值：%d\n",max);
    printf("最小值：%d\n",min);
    return 0;
}
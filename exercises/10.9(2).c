#include <stdio.h>
/*题目要求
用户先输入一个整数 n，表示数组长度，再输入n个整数。
要求：
- 1 ≤ n ≤ 20，输入其他长度时给出错误提示并结束程序。
- 输出原始数组。
- 将数组元素原地逆序。
- 输出逆序后的数组。
- 不能创建第二个数组来保存逆序结果。*/
int main(){
    int n,t;
    scanf("%d",&n);
    int a[20];
    if (n <= 0 || n > 20)
    {
        printf("输入错误");
        return 0;
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            scanf("%d",&a[i]);
        }
        for (int i = 0; i < n; i++)
        {
            if (i < n-1)
            {
                printf("%d ",a[i]);
            }
            else{
                printf("%d\n",a[i]);
            }
        }
        for (int i = 0; i < n/2; i++)
        {
            t = a[i];
            a[i] = a[n-1-i];
            a[n-1-i] = t;
        }
        for (int i = 0; i < n; i++)
        {
            if (i < n-1)
            {
                printf("%d ",a[i]);
            }
            else{
                printf("%d\n",a[i]);
            }
        }
    }
    return 0;
}
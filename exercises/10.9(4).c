/*编写一个C语言程序，让用户输入8个整数，将它们存入数组，并使用冒泡排序将数组中的数字按照从小到大的顺序排列。
要求：
1. 使用数组存储8个整数。
2. 使用嵌套for循环完成排序。
3. 不允许调用现成的排序函数。
4. 直接修改原数组，不创建第二个数组。
5. 输出排序前和排序后的数组。*/
#include <stdio.h>

int main(){
    int a[8],t;
    int swapped;
    printf("请输入8个整数：\n");
    for (int i = 0; i < 8; i++)
    {
        scanf("%d",&a[i]);
    }
    printf("排序前的数组为：\n");
    for(int i = 0; i < 8; i++)
    {
        if (i < 7)
        {
            printf("%d ",a[i]);
        }
        else{
            printf("%d\n",a[i]);
        }
    }
    for(int i = 0;i < 7; i++)
    {
        swapped = 0;
        for(int j =0; j< 7-i;j++)
        {
            if(a[j] > a[j+1])
            {
                t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
                swapped = 1;
            }
        }
        if (swapped == 0)
        {
            break;
        }
    }
    printf("排序后的数组为：\n");
    for(int i = 0; i < 8; i++)  
    {
        if (i < 7)
        {
            printf("%d ",a[i]);
        }
        else{
            printf("%d\n",a[i]);
        }
    }
    return 0;
}
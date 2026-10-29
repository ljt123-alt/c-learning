#include <stdio.h>
/*第三题：寻找出现次数最多的数字
一、题目要求
编写一个C语言程序：
1. 用户输入10个整数，并存入数组。
2. 统计每个数字出现的次数。
3. 找出出现次数最多的数字。
4. 输出这个数字以及出现次数。*/
int main(){
    int a[10];
    int count,max=0;
    int x;
    for (int i = 0; i < 10; i++)
    {
        scanf("%d",&a[i]);
    }
    for (int i = 0; i < 10; i++)
    {
        count = 1;
        for (int j = i + 1; j < 10; j++)
        {
            if (a[j] == a[i])
            {
                count++;
            }
        }
        if (count >= max)
        {
            x=a[i];
            max = count;
        }
    }
    printf("%d, 出现%d次\n",x,max);
    return 0;
}
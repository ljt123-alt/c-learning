#include <stdio.h>
void swap(int *a,int *b){
    int t;
    t = *a;
    *a =*b;
    *b = t;
    printf("交换后：%d %d\n",*a,*b);
    return;
}
int main(){
    int a,b;
    printf("请输入两个整数：");
    scanf("%d %d",&a,&b);
    printf("交换前：%d %d\n",a,b);
    swap(&a,&b);
    return 0;
}
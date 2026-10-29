#include <stdio.h>

int main(){
    int a[5];
    int *p = a;
    int max,min;
    int sum = 0;
    for(int i=0;i<5;i++){
        scanf("%d",&a[i]);
    }
    max = *p;
    min = *p;
    printf("数组元素：");
    for(int i = 0;i < 5;i++){
        if(i != 4){
            printf("%d ",*p);
        }else{
            printf("%d\n",*p);
        }
        sum += *p;
        if(*p > max){
            max = *p;
        }
        if(*p < min){
            min = *p;
        }
        p++;
    }
    printf("总和：%d\n",sum);
    printf("最大值：%d\n",max);
    printf("最小值：%d\n",min);
    return 0;
}
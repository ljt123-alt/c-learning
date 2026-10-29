#include <stdio.h>

void analyze(int *arr,int n,int *sum,int *max,int *min){
    *max = *arr;
    *min = *arr;
    *sum = 0;
    for(int i =0;i<n;i++){
        *sum += *arr;
        if(*arr > *max){
            *max = *arr;
        }
        if(*arr < *min){
            *min = *arr;
        }
        arr++;
    }
    return;
}
int main(){
    int a[5];
    int sum = 0;
    int max,min;
    for(int i=0;i<5;i++){
        if(scanf("%d",&a[i]) != 1){
            printf("输入错误，请输入整数。\n");
            return 1;
        }
    }
    analyze(a,5,&sum,&max,&min);
    printf("数组元素：");
    for(int i = 0;i < 5;i++){
        if(i != 4){
            printf("%d ",a[i]);
        }else{
            printf("%d\n",a[i]);
        }
    }
    printf("总和：%d\n",sum);
    printf("最大值：%d\n",max);
    printf("最小值：%d\n",min);
    return 0;
}
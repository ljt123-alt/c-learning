#include <stdio.h>

void reverse(int *arr,int n){
    for(int i = 0;i < n/2;i++){
        int temp = *arr;
        *arr = *(arr+n-1-2*i);
        *(arr+n-1-2*i) = temp;
        arr++;
    }
    return;
}
int main(){
    int a[6];
    for(int i=0;i<6;i++){
        if(scanf("%d",&a[i]) != 1){
            printf("输入错误，请输入整数。\n");
            return 1;
        }
    }
    printf("原数组元素：");
    for(int i = 0;i < 6;i++){
        if(i != 5){
            printf("%d ",a[i]);
        }else{
            printf("%d\n",a[i]);
        }
    }
    reverse(a,6);
    printf("逆序数组元素：");
    for(int i = 0;i < 6;i++){
        if(i != 5){
            printf("%d ",a[i]);
        }else{
            printf("%d\n",a[i]);
        }
    }

}
#include <stdio.h>
#include <string.h>
void reverse(char *str){
    char *left = str;
    char *right;
    for(right = str; *right != '\0'; right++);
    if (right == str)
    {
        return; // 空字符串，直接返回
    }
    
    right--;
    while(left < right){
        char temp = *left;
        *left = *right;
        *right = temp;
        right--;
        left++;
    }

}
int main(){
    char str[101];
    if(scanf("%100s", str) != 1){
        printf("输入错误\n");
        return 1;
    }
    printf("逆序前字符串: %s\n", str);
    reverse(str);
    printf("逆序后字符串: %s\n", str);
    return 0;
}
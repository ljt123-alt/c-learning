#include <stdio.h>

int isPalindrome(const char *str){
    const char *left = str;
    const char *right;
    for(right = str; *right != '\0';right++);
    if (right == left)
    {
        return 1; // 空字符串，认为是回文
    }
    right--;
    while(*left ==*right && left < right){
        left++;
        right--;
    }
    if(left >= right){
        return 1; // 是回文
    } else {
        return 0; // 不是回文
    }
}
int main(){
    char str[101];
    if(scanf("%100s", str) != 1){
        printf("输入错误\n");
        return 1;
    }
    if(isPalindrome(str)){
        printf("字符串是回文\n");
    } else {
        printf("字符串不是回文\n");
    }
    return 0;
}
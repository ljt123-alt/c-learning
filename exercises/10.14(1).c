#include <stdio.h>

void removeChar(char *str,char ch){
    char *r = str;
    char *w = str;
    while(*r != '\0'){
        if(*r != ch){
            *w++ = *r;
        }
        r++;
    }
    *w = '\0';
    return;
}
int main() {
    char str[101];
    char ch;
    if (scanf("%100s %c", str, &ch) != 2) {
        printf("输入错误\n");
        return 1;
    }
    removeChar(str, ch);
    printf("删除字符后的字符串: %s\n", str);
    return 0;
}
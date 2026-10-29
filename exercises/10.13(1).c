#include <stdio.h>

int main(){
    char str[101];
    char *p = str;
    int count = 0;
    int a = 0,b = 0,c = 0,d = 0;
    scanf("%s",str);
    while(*p != '\0'){
        count++;
        if(*p >= 'a' && *p <= 'z'){
            a++;
        }
        else if(*p >= 'A' && *p <= 'Z'){
            b++;
        }
        else if(*p >= '0' && *p <= '9'){
            c++;
        }
        else{
            d++;
        }
        p++;
    }
    printf("字符串长度: %d\n", count);
    printf("小写字母: %d\n", a);
    printf("大写字母: %d\n", b);
    printf("数字字符: %d\n", c);
    printf("其他字符: %d\n", d);
    return 0;
}
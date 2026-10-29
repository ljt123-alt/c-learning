#include <stdio.h>

int my_strcmp(const char *s1,const char *s2){
    while(*s1 && (*s1 == *s2)){
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;

}
char *my_strcpy(char *dest,const char *src){
    const char *s = src;
    char *d = dest;
    while((*d++ = *s++) != '\0');
    return dest;
}
char *my_strcat(char *dest,const char *src){
    char *d = dest;
    while (*d != '\0')
    {
        d++;
    }
    while ((*d++ = *src++) != '\0'){
        // 复制 src 到 dest 的末尾
    };
    return dest;
}
int main(){
    char str1[101],str2[101];
    if(scanf("%100s %100s", str1,str2) != 2){
        printf("输入错误\n");
        return 1;
    }
    int cmp_result = my_strcmp(str1,str2);
    if(cmp_result < 0){
        printf("\"%s\" 小于 \"%s\"\n", str1,str2);
    } else if(cmp_result > 0){
        printf("\"%s\" 大于 \"%s\"\n", str1,str2);
    } else {
        printf("\"%s\" 等于 \"%s\"\n", str1,str2);
    }
    char copy[201];
    my_strcpy(copy,str1);
    printf("复制后的字符串: %s\n", copy);
    my_strcat(copy,str2);
    printf("连接后的字符串: %s\n", copy);
    return 0;
}
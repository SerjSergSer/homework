#include <stdio.h>
#include <string.h>

#define MAX_LEN 10001

int main() 
{
    char s1[MAX_LEN], s2[MAX_LEN];
    int len1, len2;
    int prefix_suffix = 0;
    int suffix_prefix = 0;
    
    scanf("%s", s1);
    scanf("%s", s2);
    
    len1 = strlen(s1);
    len2 = strlen(s2);
    
    // Ищем префикс s1, являющийся суффиксом s2
    // Идем от меньшего к большему, но сохраняем максимум
    int max_len = (len1 < len2) ? len1 : len2;
    
    for (int l = 1; l <= max_len; l++) {
        int match = 1;
        for (int i = 0; i < l; i++) {
            if (s1[i] != s2[len2 - l + i]) {
                match = 0;
                break;
            }
        }
        if (match) {
            prefix_suffix = l;  // сохраняем, если нашли
        }
    }
    
    // Ищем суффикс s1, являющийся префиксом s2
    for (int l = 1; l <= max_len; l++) {
        int match = 1;
        for (int i = 0; i < l; i++) {
            if (s1[len1 - l + i] != s2[i]) {
                match = 0;
                break;
            }
        }
        if (match) {
            suffix_prefix = l;  // сохраняем, если нашли
        }
    }
    
    printf("%d %d\n", prefix_suffix, suffix_prefix);
    
    return 0;
}

#include "../include/libft.h"

char    *ft_strchr_multi(char *str, const char *set) {
    size_t  i;
    size_t  j;

    i = 0;
    while (str[i]) {
        j = 0;
        while (set[j]) {
            if (str[i] == set[j])
                return (str + i);
            j++;
        }
        i++;
    }
    return (NULL);
}

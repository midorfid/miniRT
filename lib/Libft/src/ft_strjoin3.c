#include "../include/libft.h"

char    *ft_strjoin3(const char   *start, const char    *mid, const char *end) {
    char    *new;
    size_t  len_start;
    size_t  len_mid;
    size_t  len_end;
    size_t  i;

    len_start = ft_strlen(start);
    len_mid = ft_strlen(mid);
    len_end = ft_strlen(end);
    new = malloc(sizeof(char) * (len_start + len_mid + len_end + 1));
    if(new == NULL)
        return(NULL);
    ft_strcpy(new, start);
    ft_strcpy(new + len_start, mid);
    ft_strcpy(new + len_start + len_mid, end);
    new[len_start + len_mid + len_end] = '\0';
    return(new);
}

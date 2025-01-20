#include "../include/libft.h"

//return NULL
void    *free_double_array(char **src) {
    size_t  i;

    i = 0;
    while(src[i]) {
        free(src[i]);
        i++;
    }
    free(src);
    return(NULL);
}

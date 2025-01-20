#include "../include/libft.h"

void    *ft_realloc(void *old, size_t oldsize, size_t newsize){
    void    *res;
    
    res = malloc(newsize);
    if(!res){
        free(old);
        return(res);
    }
    ft_memcpy(res, old, oldsize);
    free(old);
    return(res);
}

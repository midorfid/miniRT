#include "../include/libft.h"

size_t  double_array_len(char **input) {
    size_t  i;

    i = 0;
    while (input[i]) {
        i++;
    }
    return (i);
}

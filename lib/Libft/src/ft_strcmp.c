#include "../include/libft.h"

int	ft_strcmp(const	char	*s1, const	char	*s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
	// size_t	i;

	// i = 0;
	// if (s1[i] == '\0' && s2[i] == '\0')
	// 	return (0);
	// while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	// {
	// 	i++;
	// }
	// if (s1[i] == '\0' || s2[i] == '\0')
	// 	return (0);
	// return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

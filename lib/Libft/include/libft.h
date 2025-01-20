/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ashevchu <ashevchu@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/20 15:19:04 by ashevchu      #+#    #+#                 */
/*   Updated: 2023/04/03 21:15:15 by ashevchu      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
// # include "./ft_printf/include/ft_printf.h"
# include "../get_next_line/include/get_next_line.h"

static inline void	ft_ignore_result(long long int unused_result){
	(void)unused_result;
}

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

unsigned long	ft_strlen(const char *s);

size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t			ft_strnlen(char *str, size_t maxlen);
size_t  		double_array_len(char **input);

int				ft_atoi(const char *str);
int				ft_isalnum(int a);
int				ft_isalpha(int c);
int				ft_isascii(int a);
int				ft_isdigit(int a);
int				ft_isprint(int a);
int				ft_tolower(int c);
int				ft_toupper(int c);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
int				ft_strcmp(const	char	*s1, const	char	*s2);
int				ft_strncmp(const char *s1, const char *s2, size_t x);
int				ft_lstsize(t_list *lst);

void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
void			*ft_memset(void *b, int c, size_t len);
void			*ft_calloc(size_t count, size_t size);
void    		*ft_realloc(void *old, size_t oldsize, size_t newsize);
void    		*free_double_array(char **src);

char    		*ft_strchr_multi(char *str, const char *set);
char			*ft_strchr(const char	*str, int s);
char			*ft_strdup(const char *s1);
char			*ft_strnstr(const	char *haystack, const	char *needle,
					size_t len);
char			*ft_strrchr(const char *s, int c);
char			*ft_strjoin(char const *s1, char const *s2);
char    		*ft_strjoin3(const char   *start, const char    *mid, const char *end);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_itoa(int n);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strcpy(char *dest, const char *src);

char			**ft_split(char	const *str, char c);
char			**ft_splitset(char const *s, const char *set);
char    		**ft_dstrdup(char **src);

void			ft_striteri(char *s, void (*f)(unsigned int, char *));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
void			ft_bzero(void	*s, size_t n);
void			ft_lstadd_front(t_list **lst, t_list *new);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void*));
void			ft_lstclear(t_list **lst, void (*del)(void*));
void			ft_lstiter(t_list *lst, void (*f)(void *));

t_list			*ft_lstnew(void *content);
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *),
					void (*del)(void *));
#endif
#include "libft.h"
#include "pipex.h"
#include <stdlib.h>


t_list *get_alloc_list(void)
{
	static t_list *head;

	return (head);
}

void *ft_malloc(size_t size)
{
	t_list	*head;
	void	*p;

	head = get_alloc_list();
	p = malloc(size);
	ft_lstadd_back(&head, ft_lstnew(p));
	return (p);
}

void *ft_clear_list(void)
{
	t_list *head;
	head = get_alloc_list();

	ft_lstclear(&head, free);
}

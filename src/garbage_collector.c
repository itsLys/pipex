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
	t_list	node;
	void	*p;

	head = get_alloc_list();
	p = malloc(size);
	node.content = p;
	node.next = NULL;
	ft_lstadd_back(&head, &node);
	return (p);
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
	enum { ADD, MULTI, VAL} type;
	int val;
	struct node *l;
	struct node *r;
} node;

node *new_node(node n)
{
	node *ret = calloc(1, sizeof(node));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}
void	destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free(n);
}


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node
{
	enum {
		ADD,
		MUL,
		VAL
	} type;
	int val;
	struct node *l;
	struct node *r;
} node;

node *new_node(node n);

node *new_node(node n)
{
	node *res = calloc(1, sizeof(node));
	if (!res)
		return (NULL);
	*res = n;
	return (res);
}

void destroy_tree(node *n)
{
	if (!n)
		return ;
	if (n->type != VAL)
	{
		destroy_tree(n->l);
		destroy_tree(n->r);
	}
	free (n);
	return ;
}

void unexpected(char c)
{
	if (c)
		printf();
	else
		printf();
}
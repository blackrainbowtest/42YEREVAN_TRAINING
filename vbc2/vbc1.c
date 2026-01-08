#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
	enum { 
		MUL,
		ADD,
		VAL
	} type;
	struct node *l;
	struct node *r;
	int val;
} node;

node *new_node(node n);
void destroy_tree(node *n);
void unexpected(char c);
int accept(char **s, char c);

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
	free(n);
}

void unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'", c);
	else
		printf("Unexpected end of input\n");
}

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}
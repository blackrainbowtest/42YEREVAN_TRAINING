#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
	enum { ADD, MUL, VAL} type;
	int val;
	struct node *l;
	struct node *r;
} node;

node	*new_node(node n);
void	destroy_tree(node *n);
void	unexpected(char c);
int		accept(char **s, char c);
int		eval_tree(node *tree);

node	*new_node(node n)
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

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

int		accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int	eval_tree(node *tree)
{
	if (!tree)
		return (0);
	switch(tree->type)
	{
		case ADD:
			return eval_tree(tree->l) + eval_tree(tree->r);
		case MUL:
			return eval_tree(tree->l) * eval_tree(tree->r);
		case VAL:
			return tree->val;
	}
	return (0);
}
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
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input\n");
}

node *parce_primary(char **s)
{
	node *res;
	node  tmp;

	if (**s == '(')
	{
		(*s)++;
		res = parse_addition(s);
		if (!res || **s != ')')
		{
			destroy_tree(res);
			unexpected(**s);
			return (NULL);
		}
		(*s)++;
		return (res);
	}
	if (isdigit(**s))
	{
		tmp.type = VAL;
		tmp.val = **s - '0';
		tmp.l = NULL;
		tmp.r = NULL;
		res = new_node(tmp);
		if (!res)
			return (NULL);
		(*s)++;
		return (res);
	}
	unexpected(**s);
	return (NULL);
}

node *parse_multiplication(char **s)
{
	node *res;
	node *left;
	node *right;
	node *new;
	node tmp;

	left = parce_primary(s);
	if (!left)
		return (NULL);
	while (**s == '*')
	{
		(*s)++;
		right = parce_primary(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		tmp.type = MUL;
		tmp.val = 0;
		tmp.l = left;
		tmp.r = right;
		new = new_node(tmp);
		if (!new)
		{
			destroy_tree(left);
			destroy_tree(right);
			return (NULL);
		}
		left = new;
	}
	return (left);
}

node *parse_addition(char **s)
{
	node *res;
	node *left;
	node *right;
	node *new;
	node tmp;

	left = parse_multiplication(s);
	if (!left)
		return (NULL);
	while (**s == '+')
	{
		(*s)++;
		right = parse_multiplication(s);
		if (!right)
		{
			destroy_tree(left);
			return (NULL);
		}
		tmp.type = ADD;
		tmp.val = 0;
		tmp.l = left;
		tmp.r = right;
		new = new_node(tmp);
		if (!new)
		{
			destroy_tree(left);
			destroy_tree(right);
			return (NULL);
		}
		left = new;
	}
	return (left);
}

node *parse_expression(char **s)
{
	node *res = parse_addition(s);
	if (!ret)
		return (NULL);
	if (**s)
	{
		destroy_tree(res);
		unexpected(**s);
		return (NULL);
	}
	return (res);
}

int	eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return eval_tree(tree->l) + eval_tree(tree->r);
		case MUL:
			return	eval_tree(tree->l) * eval_tree(tree->r);
		case VAL:
			return tree->val;
	}
	return (0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	char *input = argv[1];
	node *tree = parse_expression(&input);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return (0);
}
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node
{
	eval {
		ADD,
		MUL,
		VAL
	}
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
		return (NULL);
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

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	unexpected(**s);
	return (0);
}

node *parse_primary(char **s)
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
	node  tmp;

	left = parse_primary(s);
	if (!left)
		return (NULL);
	while (**s == '*')
	{
		(*s)++;
		right = parse_primary(s);
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
	node  tmp;

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
	if (!res)
		return (NULL);
	if (**s)
	{
		destroy_tree(res);
		unexpected(**s);
		return (NULL);
	}
	return (res);
}

int eval_tree(node *tree)
{
	switch (tree->type)
	{
		case ADD:
			return eval_tree(tree.r) + eval_tree(tree.r);
		case MUL:
			return eval_tree(tree.l) * eval_tree(tree.r);
		case VAL:
			return tree->val;
	}
	return 0;
}
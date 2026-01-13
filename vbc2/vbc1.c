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
void destroy_tree(node *n);
void unexpected(char c);
int accept(char **s, char c);
node *parse_primary(char **s, char c);

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

int accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

node *parse_primary(char **s, char c)
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
		tmp.val = **s + '0';
		tmp.l = NULL;
		tmp.r = NULL;
		res = new_node(tmp);
		(*s)++;
		return (res);
	}
	unexpected(**s);
	return (NULL);
}

node *parse_multiplication(char **s)
{
	node *left;
	node *right;
	node *new;
	node tmp;

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
		tmp.l = left;
		tmp.r = right;
		tmp.val = 0;
		new = new_node(tmp);
		if (!new)
		{
			destroy_tree(left);
			destroy_tree(right);
			return (NULL);
		}
		left = new;
	}
	return left;
}
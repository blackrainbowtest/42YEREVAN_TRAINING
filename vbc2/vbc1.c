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

}

node *parse_addition(char **s)
{

}
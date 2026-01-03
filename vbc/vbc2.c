#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef struct node
{
	enum {
		MUL, ADD, VAL
	} type;
} node;

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
	return res;
}

int eval_tree(node *n)
{
	if (n->type == MUL)
		return eval_tree(n->l) * eval_tree(n->r);
	else if (n->type == ADD)
		return eval_tree(n->l) + eval_tree(n->r);
	else if (n->type == VAL)
		return n->val;

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
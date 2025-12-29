#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node *parse_1(char **str);
node *parse_2(char **str);
node *parse_3(char **str);

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
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

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
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

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

/* ---------------------- */

node *parse_1(char **str)
{
    node *res;
    node tmp;

    if (**str == '(')
    {
        (*str)++;
        res = parse_3(str);
        if (!res || **str != ')')
        {
            destroy_tree(res);
            unexpected(**str);
            return NULL;
        }
        (*str)++;
        return res;
    }
    if (isdigit(**str))
    {
        tmp.type = VAL;
        tmp.val = **str - '0';
        tmp.l = NULL;
        tmp.r = NULL;
        res = new_node(tmp);
        (*str)++;
        return res;
    }
    unexpected(**str);
    return NULL;
}

node *parse_2(char **str)
{
    node *left;
    node *right;
    node tmp;

    left = parse_1(str);
    if (!left)
        return NULL;
    while(**str == '*')
    {
        (*str)++;
        right = parse_1((str));
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;
        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}

node *parse_3(char **str)
{
    node *left;
    node *right;
    node tmp;

    left = parse_2(str);
    if (!left)
        return NULL;
    while (**str == '+')
    {
        (*str)++;
        right = parse_2(str);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;

        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    return left;
}

/* ---------------------- */

node    *parse_expr(char *s)
{
    node *ret;

    ret = parse_3(&s);
    if (!ret)
        return (NULL);
    if (*s) 
    {
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
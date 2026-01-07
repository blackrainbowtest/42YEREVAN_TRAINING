#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,      // Nodo de suma
        MULTI,    // Nodo de multiplicación  
        VAL       // Nodo de valor (hoja)
    } type;
    int val;           // Valor para nodos VAL
    struct node *l;    // Hijo izquierdo
    struct node *r;    // Hijo derecho
} node;

node *parse_expression(char **s);
node *parse_addition(char **s);
node *parse_multiplication(char **s);
node *parse_primary(char **s);
node *new_node(node n);
void destroy_tree(node *n);
int eval_tree(node *tree);
void unexpected(char c);
int accept(char **s, char c);
int expect(char **s, char c);

node *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return NULL;
    *ret = n;
    return ret;
}

void destroy_tree(node *n)
{
    if (!n)
        return;
    
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
        return 1;
    }
    return 0;
}

int expect(char **s, char c) // ls function not need
{
    if (accept(s, c))
        return 1;
    unexpected(**s);
    return 0;
}

node *parse_primary(char **s)
{
    node *res;
    node tmp;
    
    if (**s == '(')
    {
        (*s)++;  // Consumir '('
        res = parse_addition(s);  // Parsear expresión interna
        if (!res || **s != ')')
        {
            destroy_tree(res);
            unexpected(**s);
            return NULL;
        }
        
        (*s)++;  // Consumir ')'
        return res;
    }
    if (isdigit(**s))
    {
        tmp.type = VAL;
        tmp.val = **s - '0';  // Convertir '0'-'9' a 0-9
        tmp.l = NULL;
        tmp.r = NULL;
        
        res = new_node(tmp);
        (*s)++;  // Consumir dígito
        return res;
    }
    unexpected(**s);
    return NULL;
}

node *parse_multiplication(char **s)
{
    node *left;
    node *right;
    node tmp;
    
    left = parse_primary(s);
    if (!left)
        return NULL;
    while (**s == '*')
    {
        (*s)++;  // Consumir '*'
        
        right = parse_primary(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;  // No usado en nodos operadores
        
        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

node *parse_addition(char **s)
{
    node *left;
    node *right;
    node tmp;
    
    left = parse_multiplication(s);
    if (!left)
        return NULL;
    
    while (**s == '+')
    {
        (*s)++;  // Consumir '+'
        
        right = parse_multiplication(s);
        if (!right)
        {
            destroy_tree(left);
            return NULL;
        }
        
        // Crear nodo de suma
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        tmp.val = 0;  // No usado en nodos operadores
        
        node *old_left = left;
        left = new_node(tmp);
        if (!left)
        {
            destroy_tree(old_left);
            destroy_tree(right);
            return NULL;
        }
    }
    
    return left;
}

node *parse_expression(char **s)
{
    node *ret = parse_addition(s);
    
    if (!ret)
        return NULL;
    
    // Verificar que se consumió toda la entrada
    if (**s)
    {
        destroy_tree(ret);
        unexpected(**s);
        return NULL;
    }
    
    return ret;
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return eval_tree(tree->l) + eval_tree(tree->r);
            
        case MULTI:
            return eval_tree(tree->l) * eval_tree(tree->r);
            
        case VAL:
            return tree->val;
    }
    
    return 0;  // No debería llegar aquí
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return 1;
    
    char *input = argv[1];
    node *tree = parse_expression(&input);
    
    if (!tree)
        return 1;
    
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return 0;
}

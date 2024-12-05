#include "diff.h"

Tree_Node* Copy_Subtree (Tree_Node* node) {
    if (node == NULL) {
        return NULL;
    }

    Tree_Node* new_node = (Tree_Node*) calloc (1, sizeof (Tree_Node));
    if (new_node == NULL) {
        fprintf (stderr, "Ошибка выделения памяти для копии поддерева.\n");
        exit (EXIT_FAILURE);
    }

    new_node->type = node->type;
    new_node->num_value = node->num_value;
    new_node->var_name = node->var_name;
    new_node->operation = node->operation;

    if (node->left != NULL) {
        new_node->left = Copy_Subtree (node->left);
    }
    if (node->right != NULL) {
        new_node->right = Copy_Subtree (node->right);
    }

    return new_node;
}



Tree_Node* Diff_Add (Tree_Node* node)
{
    if (node == NULL || node->left == NULL || node->right == NULL) {
        return NULL;
    }
    return New_Node (NODE_OPERATION , ADD , Diff (node->left) , Diff (node->right));
}


Tree_Node* Diff_Sub (Tree_Node* node)
{
    if (node == NULL || node->left == NULL || node->right == NULL) {
        return NULL;
    }
    return New_Node (NODE_OPERATION , SUB , Diff (node->left) , Diff (node->right));
}


Tree_Node* Diff_Mul (Tree_Node* node)
{
    if (node == NULL || node->left == NULL || node->right == NULL) {
        return NULL;
    }
    return New_Node (NODE_OPERATION , ADD , New_Node (NODE_OPERATION , MUL , Diff (node->left) , Copy_Subtree (node->right)) ,
           New_Node (NODE_OPERATION , MUL , Copy_Subtree (node->left) , Diff (node->right)));
}


Tree_Node* Diff_Div (Tree_Node* node)
{
    if (node == NULL || node->left == NULL || node->right == NULL) {
        return NULL;
    }
    Tree_Node* numerator = New_Node (
        NODE_OPERATION , SUB ,
        New_Node (NODE_OPERATION , MUL , Diff (node->left) , Copy_Subtree (node->right)) ,
        New_Node (NODE_OPERATION , MUL , Copy_Subtree (node->left) , Diff (node->right))
    );

    Tree_Node* denominator = New_Node (
        NODE_OPERATION , MUL , Copy_Subtree (node->right) , Copy_Subtree (node->right));

    return New_Node (NODE_OPERATION , DIV , numerator , denominator);
}


Tree_Node* Diff_Sin (Tree_Node* node)
{
    if (node == NULL || node->left == NULL) {
        return NULL;
    }
    return New_Node (NODE_OPERATION , MUL , New_Node (NODE_OPERATION , 'c' , node->left , NULL) , Diff (node->left));
}


Tree_Node* Diff_Cos (Tree_Node* node)
{
    if (node == NULL || node->left == NULL) {
        return NULL;
    }
    Tree_Node* diff_cos = New_Node (NODE_OPERATION , MUL , New_Node (NODE_NUMBER , -1 , NULL , NULL) , New_Node (NODE_OPERATION , 's' , node->left , NULL));
    return New_Node (NODE_OPERATION , MUL , diff_cos , Diff (node->left));
}


Tree_Node* Diff_Exp (Tree_Node* node)
{
    if (node == NULL || node->left == NULL || node->right == NULL) {
        return NULL;
    }

    // Первый случай: g (x)^n, где g (x) — это функция, а n — число
    if (node->right->type == NODE_NUMBER) {
        double n = node->right->num_value;

        // d/dx (g (x)^n) = n * g (x)^ (n-1) * g' (x)
        Tree_Node* g_prime = Diff (node->left);  // g' (x)

        return New_Node (NODE_OPERATION, '*',
            New_Node (NODE_NUMBER, n, NULL, NULL),  // n
            New_Node (NODE_OPERATION, '*',
                New_Node (NODE_OPERATION, '^', Copy_Subtree (node->left), New_Node (NODE_NUMBER, n - 1, NULL, NULL)),  // g (x)^ (n-1)
                g_prime  // g' (x)
            )
        );
    }

    // Второй случай: n^g (x),
    if (node->left->type == NODE_NUMBER) {
        double n = node->left->num_value;

        // d/dx (n^g (x)) = n^g (x) * ln (n) * g' (x)
        Tree_Node* g_prime = Diff (node->right);  // g' (x)

        Tree_Node* part_no_diff =  New_Node (NODE_OPERATION, '*',
            New_Node (NODE_OPERATION, '^', Copy_Subtree (node->left), Copy_Subtree (node->right)),  // n^g (x)
            New_Node (NODE_NUMBER, log (n), NULL, NULL));  // ln (n)

        return New_Node (NODE_OPERATION , MUL , part_no_diff , g_prime);
    }

    // Третий случай: f (x)^g (x)
    // d/dx (f (x)^g (x)) = f (x)^g (x) * (g' (x) * ln (f (x)) + g (x) * f' (x) / f (x))
    if (node->left->type == NODE_VARIABLE || node->left->type == NODE_OPERATION) {
        Tree_Node* f_prime = Diff (node->left);  // f' (x)
        Tree_Node* g_prime = Diff (node->right);  // g' (x)

        return New_Node (NODE_OPERATION, '*',
            New_Node (NODE_OPERATION, '^', Copy_Subtree (node->left), Copy_Subtree (node->right)),  // f (x)^g (x)
            New_Node (NODE_OPERATION, '+',
                New_Node (NODE_OPERATION, '*', Copy_Subtree (g_prime), New_Node (NODE_OPERATION, 'L', Copy_Subtree (node->left), NULL)),  // g' (x) * ln (f (x))
                New_Node (NODE_OPERATION, '*', Copy_Subtree (node->right), New_Node (NODE_OPERATION, '/', Copy_Subtree (f_prime), Copy_Subtree (node->left)))  // g (x) * f' (x) / f (x)
            )
        );
    }

    fprintf (stderr, "Ошибка: неизвестный узел для дифференцирования.\n");
    exit (EXIT_FAILURE);
}

Tree_Node* Diff_Ln (Tree_Node* node)
{
    if (node == NULL || node->left == NULL) {
        return NULL;
    }
    return New_Node (NODE_OPERATION, DIV, Diff (node->left), Copy_Subtree (node->left));
}


Tree_Node* Diff_Sqrt (Tree_Node* node)
{
    if (node == NULL || node->left == NULL) {
        return NULL;
    }
    // g (x) - это выражение под корнем
    Tree_Node* g = Copy_Subtree (node->left);

    // Дифференцируем g (x) по x
    Tree_Node* g_diff = Diff (g);

    // Производная от sqrt (g (x)): 1 / (2 * sqrt (g (x))) * g' (x)
    Tree_Node* two = New_Node (NODE_NUMBER, 2, NULL, NULL);
    Tree_Node* denominator = New_Node (NODE_OPERATION, MUL, two, Copy_Subtree (node->left));
    Tree_Node* result = New_Node (NODE_OPERATION, DIV, g_diff, denominator);

    return result;
}

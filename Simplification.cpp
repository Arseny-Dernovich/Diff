#include "diff.h"

double Evaluate (Tree_Node* node)
{
    if (node == NULL) return 0;

    switch (node->operation) {

        case ADD: return node->left->num_value + node->right->num_value;
        case SUB: return node->left->num_value - node->right->num_value;
        case MUL: return node->left->num_value * node->right->num_value;
        case DIV:
            if (node->right->num_value != 0) {
                return node->left->num_value / node->right->num_value;
            } else {
                fprintf (stderr, "Ошибка: деление на ноль.\n");
                exit (EXIT_FAILURE); // assert
            }
        default:
                fprintf (stderr, "Ошибка: неизвестная операция для свёртки.\n");
                exit (EXIT_FAILURE);
    }
}

// Функция свёртки констант
Tree_Node* Fold_Constants (Tree_Node* node)
{
    if (node == NULL) return NULL;

    node->left  = Fold_Constants (node->left);
    node->right = Fold_Constants (node->right);

    if (node->type == NODE_OPERATION &&
        node->left && node->right &&
        node->left->type == NODE_NUMBER && node->right->type == NODE_NUMBER) {

        double result = Evaluate (node);
        return New_Node (NODE_NUMBER, result, NULL, NULL);
    }

    return node;
}

Tree_Node* Remove_Neutral_Elements (Tree_Node* node)
{
    if (node == NULL) return NULL;

    node->left = Remove_Neutral_Elements (node->left);
    node->right = Remove_Neutral_Elements (node->right);

    // Удаление нейтральных элементов для сложения
    if (node->operation == '+' && node->left->type == NODE_NUMBER && node->left->num_value == 0) {
        return node->right;  // x + 0 = x
    }
    if (node->operation == '+' && node->right->type == NODE_NUMBER && node->right->num_value == 0) {
        return node->left;  // 0 + x = x
    }

    // Удаление нейтральных элементов для умножения
    if (node->operation == '*' && node->left->type == NODE_NUMBER && node->left->num_value == 1) {
        return node->right;  // x * 1 = x
    }
    if (node->operation == '*' && node->right->type == NODE_NUMBER && node->right->num_value == 1) {
        return node->left;  // 1 * x = x
    }

    // Обработка случаев умножения на 0
    if (node->operation == '*' && ( (node->left->type == NODE_NUMBER && node->left->num_value == 0) ||
                                   (node->right->type == NODE_NUMBER && node->right->num_value == 0))) {
        return New_Node (NODE_NUMBER, 0, NULL, NULL);  // x * 0 = 0
    }

    // Упрощение для степени: f (x)^1 = f (x)
    if (node->operation == '^' && node->right->type == NODE_NUMBER && node->right->num_value == 1) {
        return node->left;  // f (x)^1 = f (x)
    }

    // Упрощение для степени: f (x)^0 = 1
    if (node->operation == '^' && node->right->type == NODE_NUMBER && node->right->num_value == 0) {
        return New_Node (NODE_NUMBER, 1, NULL, NULL);  // f (x)^0 = 1
    }

    return node;
}


Tree_Node* Simplify_Tree (Tree_Node* node)
{
    if (node == NULL) return NULL;

    node = Fold_Constants (node);

    node = Remove_Neutral_Elements (node);

    return node;
}

void Free_Tree (Tree_Node *node)
{
    if (!node) return;
    Free_Tree (node->left);
    Free_Tree (node->right);
    free (node);
}

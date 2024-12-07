#include "diff.h"


void Substitute_x_with_zero (Tree_Node* node)
{
    if (node == NULL) return;

    if (node->type == NODE_VARIABLE && node->var_name == 'x') {
        node->type = NODE_NUMBER;
        node->num_value = 0;

    } else if (node->type == NODE_OPERATION) {

        Substitute_x_with_zero (node->left);
        Substitute_x_with_zero (node->right);
    }
}


double Factorial (int n)
{
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }

    return result;
}

void Taylor(Tree_Node* root, int n, FILE* latex_file)
{
    if (root == NULL) {
        fprintf(stderr, "Ошибка: дерево функции пусто.\n");
        return;
    }

    if (latex_file == NULL) {
        fprintf(stderr, "Ошибка: файл LaTeX недоступен.\n");
        return;
    }

    fprintf(latex_file, "\\section*{Разложение функции по Тейлору}\n");
    fprintf(latex_file, "Разложение функции в ряд Тейлора до порядка $n = %d$:\n", n);
    fprintf(latex_file, "\\begin{equation}\n");
    fprintf(latex_file, "f(x) \\approx ");

    // printf("Разложение по Тейлору до порядка %d:\n", n);

    Tree_Node* current_term = Copy_Subtree(root);
    // Free_Tree (root);
    double coefficient = 0;

    for (int i = 0; i <= n; i++) {
    // fprintf(stderr, "i = %d\n", i);

    // Create_Graph (current_term);
        Tree_Node* term_copy = Copy_Subtree (current_term);
        // Create_Graph (term_copy);
        Substitute_x_with_zero (term_copy);
        term_copy = Simplify_Tree (term_copy);
        Create_Graph (term_copy);
        // PrintTree (term_copy , 0);


        coefficient = term_copy->num_value / Factorial(i);

        printf("Член %d: %lg * x^%d\n", i, coefficient, i);
        if (coefficient > 0) {

            if (i > 0) {

                fprintf(latex_file, " + ");
                fprintf(latex_file, "%lg x^{%d}", coefficient, i);
            }

            else

                fprintf (latex_file, "%lg ", coefficient);
        }

        else if (coefficient < 0) {
        if (i > 0) {
                fprintf(latex_file, " - ");
            }
            fprintf(latex_file, "%lg x^{%d}", abs(coefficient), i);
        }

        Free_Tree(term_copy);

        if (i < n) {

            Tree_Node* next_term = Diff(current_term);

            // PrintTree (next_term , 0);
            Free_Tree(current_term);
    // fprintf(stderr, "HUIIIIIIIIII");
            current_term = next_term;
    // Create_Graph (current_term);
        }
    }

    fprintf(latex_file, "\n\\end{equation}\n");
    fprintf(latex_file, "\\noindent Конец разложения.\n");


    Free_Tree(current_term);
}



// for (int i = 0; i <= n; i++) {
//     fprintf(stderr, "i = %d\n", i);
//
//     // Create_Graph (current_term);
//         Tree_Node* term_copy = Copy_Subtree (current_term);
//         // Create_Graph (term_copy);
//         Substitute_x_with_zero (term_copy);
//         Fold_Constants (term_copy);
//         Create_Graph (term_copy);
//         PrintTree (term_copy , 0);
//
//
//         coefficient = term_copy->num_value / Factorial(i);
//         printf ("\ncoeff = %lg\n" , term_copy->num_value);
//
//         printf("Член %d: %lg * x^%d\n", i, coefficient, i);
//
//         if (i > 0) {
//             fprintf(latex_file, " + ");
//         }
//         fprintf(latex_file, "%lg x^{%d}", coefficient, i);
//
//         Free_Tree(term_copy);
//
//         if (i < n) {
//
//             Tree_Node* next_term = Diff(current_term);
//
//             // PrintTree (next_term , 0);
//             Free_Tree(current_term);
//     // fprintf(stderr, "HUIIIIIIIIII");
//             current_term = next_term;
//     // Create_Graph (current_term);
//         }
//     }
//
//     fprintf(latex_file, "\n\\end{equation}\n");
//     fprintf(latex_file, "\\noindent Конец разложения.\n");
//
//
//     Free_Tree(current_term);
// }

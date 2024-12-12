#include "diff.h"

int main (int argc , char** argv)
{
    const char* filename = "input.txt";
    if (argc == 2) {
        filename = argv[1];
    }


    char* expr = Read_File_To_Buffer (filename);

    const char* latex_filename = "expres";

    Tree_Node* root = Parse_Expression (expr);
    // Create_Graph (root);
    root = Simplify_Tree (root);
    Tree_Node* copy_root = Copy_Subtree (root);
    Generate_Gnuplot_File(root, "plot_script.gp");

    Tree_Node* copy_diff_root = Copy_Subtree (root);


    copy_diff_root = Diff (copy_diff_root);
    copy_diff_root = Simplify_Tree (copy_diff_root);
    PrintTree (copy_diff_root, 0);
    fprintf (stderr , "HUI\n");
    Generate_Latex_File (copy_root , copy_diff_root , latex_filename);
    // printf ("Parsed expression:\n");
    // printf ("\n");

    // Generate_Diff_Latex_File (root , latex_filename);
    printf ("Differentiated expression:\n");
    // PrintTree (copy_diff_root, 0);
    // printf ("\n");

    printf ("Simplified expression:\n");
    // PrintTree (copy_diff_root, 0);
    printf ("\n");


    // Generate_Latex_File (root, latex_filename);

    Compile_Latex_To_Pdf (latex_filename);
// fprintf(stderr, "PORNO!\n");
    free (expr);
    Free_Tree (root);

    return 0;
}


// enum
// {
// op_+ = 2
// op_- = 1

Function* Find_Function (char op)
{
    for (int i = 0; i < functions_count; i++) {
        if (functions[i].name_in_file == op) {
            return &functions[i];
            //return &functions[enum]
        }
    }
    return NULL;
}


Tree_Node* New_Node (enum Node_Type type , double value , Tree_Node* left , Tree_Node* right)
{
    Tree_Node* node = (Tree_Node*) calloc (1 , sizeof (Tree_Node));
    node->type = type;
    node->left = left;
    node->right = right;

    if (type == NODE_NUMBER) {
        node->num_value = value;

    } else if (type == NODE_VARIABLE) {
        node->var_name = (char)value;

    } else if (type == NODE_OPERATION) {
        node->operation = (char)value;
    }

    return node;
}


Tree_Node* Diff (Tree_Node* node)
{
    if (node == NULL) {
        return NULL;
    }

    if (node->type == NODE_NUMBER) {
        return New_Node (NODE_NUMBER, 0, NULL, NULL);
    }

    if (node->type == NODE_VARIABLE) {
        return New_Node (NODE_NUMBER, 1, NULL, NULL);
    }

    if (node->type == NODE_OPERATION) {
        Function* func = Find_Function(node->operation);
        if (func != NULL) {
            return func->differentiate(node);
        } else {
            fprintf(stderr, "Ошибка: операция '%c' не поддерживается для дифференцирования.\n", node->operation);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stderr, "Ошибка: операция \'%c\' , %d  не поддерживается для дифференцирования.\n", node->operation , node->type);
    fprintf (stderr, "Ошибка: неизвестный узел для дифференцирования.\n");
    exit (EXIT_FAILURE);
}


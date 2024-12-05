#include "diff.h"

int main ()
{
    const char* filename = "input.txt";

    char* expr = Read_File_To_Buffer (filename);

    const char* latex_filename = "expres";

    Tree_Node* root = Parse_Expression (expr);
    Tree_Node* copy_root = Copy_Subtree (root);
    Generate_Gnuplot_File(root, "plot_script.gp");
    root = Diff (root);
    root = Simplify_Tree (root);
    Generate_Latex_File (copy_root , root , latex_filename);
    // printf ("Parsed expression:\n");
    // PrintTree (root, 0);
    // printf ("\n");

    // Generate_Diff_Latex_File (root , latex_filename);
    printf ("Differentiated expression:\n");
    PrintTree (root, 0);
    // printf ("\n");

    printf ("Simplified expression:\n");
    PrintTree (root, 0);
    printf ("\n");

    Create_Graph (root);

    // Generate_Latex_File (root, latex_filename);

    Compile_Latex_To_Pdf (latex_filename);

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

        Function* func = Find_Function (node->operation);
        return func->differentiate (node);

    }

    fprintf (stderr, "Ошибка: неизвестный узел для дифференцирования.\n");
    exit (EXIT_FAILURE);
}


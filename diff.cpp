#include "diff.h"

int main () {
    const char* filename = "input.txt";
    expr = Read_File_To_Buffer (filename);

    Tree_Node* root = Parse_Expression (expr);
    printf ("Parsed expression:\n");
    PrintTree (root , 0);
    printf ("\n");
    Create_Graph (root);

    const char* latex_filename = "express";
    Generate_Latex_File (root , latex_filename);

    Compile_Latex_To_Pdf (latex_filename);

    free ( (char*)expr);
    return 0;
}




Function* Find_Function (char op)
{
    for (int i = 0; i < functions_count; i++) {
        if (functions[i].name_in_file == op) {
            return &functions[i];
        }
    }
    return NULL;
}


Tree_Node* New_Node  (enum Node_Type type , double value , Tree_Node* left , Tree_Node* right)
{
    Tree_Node* node =  (Tree_Node*)malloc (sizeof (Tree_Node));
    node->type = type;
    node->left = left;
    node->right = right;

    if  (type == NODE_NUMBER) {
        node->num_value = value;

    } else if  (type == NODE_VARIABLE) {
        node->var_name =  (char)value;

    } else if  (type == NODE_OPERATION) {
        node->operation =  (char)value;
    }

    return node;
}

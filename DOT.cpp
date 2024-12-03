#include "diff.h"

void GenerateDOT (Tree_Node* node , FILE* file , int* node_count , int is_root)
{
    if  (!node) return;

    int current_node =  (*node_count)++;
    const char* color = "";

    if  (is_root) {
        color = "red";  // Корень
    } else if  (node->type == NODE_NUMBER) {
        color = "lightblue";  // Число
    } else if  (node->type == NODE_VARIABLE) {
        color = "green";  // Переменная
    } else {
        color = "yellow";  // Оператор
    }

    if  (node->type == NODE_OPERATION) {

        Function* func = Find_Function (node->operation);
        if  (func != NULL && func->type_operation == UNARY) {
            fprintf (file , "node%d [label=\"%s\" , style=filled , fillcolor=%s];\n" , current_node , func->real_name , color);

        } else {
            fprintf (file , "node%d [label=\"%c\" , style=filled , fillcolor=%s];\n" , current_node , node->operation , color);
        }

    } else if  (node->type == NODE_NUMBER) {
        fprintf (file , "node%d [label=\"%.2lf\" , style=filled , fillcolor=%s];\n" , current_node , node->num_value , color);

    } else if  (node->type == NODE_VARIABLE) {
        fprintf (file , "node%d [label=\"%c\" , style=filled , fillcolor=%s];\n" , current_node , node->var_name , color);
    }

    if  (node->left) {

        int left_node = *node_count;
        GenerateDOT (node->left , file , node_count , 0);
        fprintf (file , "node%d -> node%d;\n" , current_node , left_node);
    }

    if  (node->right) {

        int right_node = *node_count;
        GenerateDOT (node->right , file , node_count , 0);
        fprintf (file , "node%d -> node%d;\n" , current_node , right_node);
    }
}

void Create_Graph (Tree_Node* root)
{
    FILE* file = fopen ("tree.dot" , "w");
    if  (!file) {
        perror ("Failed to create DOT file");
        exit (EXIT_FAILURE);
    }

    fprintf (file , "digraph G {\n");
    fprintf (file , "node [fontname=\"Arial\"];\n");
    int node_count = 0;
    GenerateDOT (root , file , &node_count , true);
    fprintf (file , "}\n");

    fclose (file);

    system ("dot -Tpng tree.dot -o tree.png");
    printf ("Tree graph generated as 'tree.png'.\n");
}

void PrintTree (Tree_Node* node , int depth)
{
    if  (!node) return;

    PrintTree (node->left , depth + 1);
    for  (int i = 0; i < depth; i++) printf ("  ");

    if  (node->type == NODE_NUMBER) {
        printf ("Number: %.2lf\n" , node->num_value);
    } else if  (node->type == NODE_VARIABLE) {
        printf ("Variable: %c\n" , node->var_name);
    } else if  (node->type == NODE_OPERATION) {
        printf ("Operation: %c\n" , node->operation);
        PrintTree (node->right , depth + 1);
    }
}

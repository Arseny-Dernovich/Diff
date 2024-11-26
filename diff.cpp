#include "diff.h"




TreeNode* New_Node(enum NodeType type, void* value, TreeNode* left, TreeNode* right)
{
    my_assert (value == NULL);
    TreeNode* node = (TreeNode*) calloc (1, sizeof(TreeNode));
    node->type = type;
    node->left = left;
    node->right = right;

    switch (type) {

        case OPERATION:
            node->operation = *(char*)value;
            break;

        case CONSTANT:
            node->constant = *(double*)value;
            break;

        case VARIABLE:
            node->variable = strdup ((char*)value);
            break;
    }

    return node;
}


void Skip_Whitespace(FILE* file)
{
    char c = 0;
    while ((c = fgetc(file)) != EOF && isspace(c)) {

    }
    ungetc(c, file);
}


TreeNode* Parse_Expression(FILE* file)
{
    Skip_Whitespace(file);

    char c = fgetc(file);
    if (c == EOF) return NULL;

    if (c == '(') {
        TreeNode* node = Parse_Expression(file);
        Skip_Whitespace(file);
        fgetc(file);
        return node;
    }

    if (c == '+' || c == '-' || c == '*' || c == '/') {
        char op = c;

        TreeNode* left = Parse_Expression(file);
        TreeNode* right = Parse_Expression(file);

        return New_Node(OPERATION, &op, left, right);
    }

    if (isdigit(c) || c == '.') {
        ungetc(c, file);
        double value;
        fscanf(file, "%lf", &value);
        return New_Node(CONSTANT, &value, NULL, NULL);
    }

    if (isalpha(c)) {
        char var[2] = {c, '\0'};
        return New_Node(VARIABLE, var, NULL, NULL);
    }


    return NULL;
}




int main()
{
    FILE* file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл.\n");
        return 1;
    }

    TreeNode* root = Parse_Expression(file);
    fclose(file);

    Display_Tree_With_Graphviz(root);

    return 0;
}

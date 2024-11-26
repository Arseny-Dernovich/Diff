#include "My_features.h"

enum NodeType {
    OPERATION,
    CONSTANT,
    VARIABLE
};

struct TreeNode {
    enum NodeType type;
    char operation;
    double constant;
    char* variable;
    struct TreeNode* left;
    struct TreeNode* right;
};

typedef struct TreeNode TreeNode;

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


void Generate_Dot(TreeNode* node, FILE* file)
{
    if (node == NULL) return;

    char label[256]="";
    switch (node->type) {

        case OPERATION:
            snprintf(label, sizeof(label), " %c", node->operation);
            break;

        case CONSTANT:
            snprintf(label, sizeof(label), " %.2f", node->constant);
            break;

        case VARIABLE:
            snprintf(label, sizeof(label), " %s", node->variable);
            break;
    }
    fprintf(file, "    \"%p\" [label=\" %s \"];\n", node, label);

    if (node->left) {
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"left\", color=green];\n", node, node->left);
        Generate_Dot(node->left, file);
    }

    if (node->right) {
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"right\", color=red];\n", node, node->right);
        Generate_Dot(node->right, file);
    }
}

void Create_Dot_File(TreeNode* root, const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл для записи.\n");
        return;
    }

    fprintf(file, "digraph Tree {\n");
    fprintf(file, "node [shape=oval, style=filled, fillcolor=lightblue fontname=\"Arial\"];\n");

    Generate_Dot(root, file);

    fprintf(file, "}\n");

    fclose(file);
}

void Display_Tree_With_Graphviz(TreeNode* root)
{
    const char* dotFilename = "tree.dot";
    const char* imageFilename = "tree.png";

    Create_Dot_File(root, dotFilename);


    char command[512] = "";
    snprintf(command, sizeof(command), "dot -Tpng tree.dot -o %s", imageFilename);
    system(command);

    printf("Граф дерева сохранён в файл %s.\n", imageFilename);
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

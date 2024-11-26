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


TreeNode* Parse_Expression(FILE* file);
TreeNode* New_Node(enum NodeType type, void* value, TreeNode* left, TreeNode* right);
void Generate_Dot(TreeNode* node, FILE* file);
void Create_Dot_File(TreeNode* root, const char* filename);
void Display_Tree_With_Graphviz(TreeNode* root);

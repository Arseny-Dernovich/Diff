#include "My_features.h"

enum Node_Type {
    NODE_NUMBER ,
    NODE_VARIABLE ,
    NODE_OPERATION
};

enum Operation_Type {
    BINARY ,
    UNARY
};

struct Tree_Node {
    enum Node_Type type;
    double num_value;
    char var_name;
    char operation;
    Tree_Node* left;
    Tree_Node* right;
};

struct Function {
    char name_in_file;
    const char* real_name;
    const char* latex_name;
    Operation_Type type_operation;
};


static Function functions[] = {
    {'s' , "sin" , "\\sin" , UNARY} ,
    {'S' , "sqrt" , "\\sqrt" , UNARY} ,
    {'c' , "cos" , "\\cos" , UNARY} ,
    {'L' , "ln" , "\\ln" , UNARY} ,
    {'+' , "+" , "+" , BINARY} ,
    {'-' , "-" , "-" , BINARY} ,
    {'*' , "*" , "\\cdot" , BINARY} ,
    {'/' , "/" , "\\div" , BINARY} ,
    {'^' , "^" , "\\wedge"  , BINARY}
};

const int functions_count = sizeof (functions) / sizeof (Function);

Tree_Node* Parse_G ();  // Главное правило
Tree_Node* Parse_E ();  // Основное выражение
Tree_Node* Parse_T ();  // Обработка * , /
Tree_Node* Parse_P ();  // Обработка скобок , чисел , переменных и функций
Tree_Node* Parse_F ();
Tree_Node* Parse_S ();  // Обработка функций
void PrintTree (Tree_Node* node , int depth);
void Create_Graph (Tree_Node* root);
Function* Find_Function (char op);
void  Generate_Latex (Tree_Node* node , FILE* latex_file);
void Generate_Latex_File (Tree_Node* root , const char* filename);
void Compile_Latex_To_Pdf (const char* latex_filename);
char* Read_File_To_Buffer (const char* filename);
Tree_Node* Parse_Expression (const char* input);
Tree_Node* New_Node (enum Node_Type type , double value , Tree_Node* left , Tree_Node* right);

static const char* expr = "";
static int p = 0;

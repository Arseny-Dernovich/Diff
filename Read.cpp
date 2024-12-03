#include "diff.h"

char* Read_File_To_Buffer (const char* filename)
{
    struct stat st = {};
    FILE* file = fopen (filename , "r");
    if  (!file) {
        perror ("Failed to open file");
        exit (EXIT_FAILURE);
    }

    if  (fstat (fileno (file) , &st) == -1) {
        perror ("Failed to get file size");
        fclose (file);
        exit (EXIT_FAILURE);
    }

    long size = st.st_size;

    char* buffer =  (char*) calloc (1 , size + 1);
    if  (!buffer) {
        perror ("Failed to allocate memory");
        fclose (file);
        exit (EXIT_FAILURE);
    }

    int j = 0;
    for  (int i = 0; i < size; i++) {
        char ch = fgetc (file);
        if  (ch == EOF) break;
        if  (!isspace (ch)) {
            buffer[j++] = ch;
        }
    }

    buffer[j] = '\0';

    fclose (file);
    return buffer;
}

void Syntax_Error ()
{
    printf ("Syntax Error\n");
    exit (EXIT_FAILURE);
}


Tree_Node* Parse_N ()
{
    double value = 0;
    while (isdigit (expr[p])) {

        value = value * 10 +  (expr[p] - '0');
        p++;
    }
    return New_Node (NODE_NUMBER , value , NULL , NULL);
}

// Функция обработки переменной x
Tree_Node* Parse_V ()
{
    if (expr[p] == 'x') {
        p++;
        return New_Node (NODE_VARIABLE , 'x' , NULL , NULL);

    } else {
        printf ("Ошибка: неизвестная переменная!\n");
        exit (EXIT_FAILURE);
    }
}

// Функция обработки функций
Tree_Node* Parse_F ()
{
    Function* func = Find_Function (expr[p]);
    if (func != NULL) {
        p++;
        if (expr[p] == ' (') {
            p++;
            Tree_Node* argument = Parse_S ();
            if (expr[p] == ')') {
                p++;
                return New_Node (NODE_OPERATION ,  (double)func->name_in_file , argument , NULL);
            }
        }
    }
    printf ("Ошибка синтаксиса в функции\n");
    exit (EXIT_FAILURE);
}

// Обработка скобок , чисел , переменных и функций
Tree_Node* Parse_P ()
{
    if  (expr[p] == ' (') {
        p++;
        Tree_Node* node = Parse_E ();
        if  (expr[p] == ')') {
            p++;
            return node;
        }
        printf ("Ошибка: ожидается закрывающая скобка!\n");
        exit (EXIT_FAILURE);

    } else if  (isdigit (expr[p])) {
        return Parse_N ();

    } else if  (expr[p] == 'x') {
        return Parse_V ();

    } else {
        return Parse_F ();
    }
}

Tree_Node* Parse_S ()
{
    Tree_Node* node = Parse_P ();

    while  (expr[p] == '^') {

        p++;
        Tree_Node* right = Parse_P ();
        node = New_Node (NODE_OPERATION , '^' , node , right);

    }

    return node;
}

// Обработка умножения и деления
Tree_Node* Parse_T ()
{
    Tree_Node* node = Parse_P ();
    while  (expr[p] == '*' || expr[p] == '/') {

        char op = expr[p];
        p++;
        Tree_Node* right = Parse_P ();
        node = New_Node (NODE_OPERATION , op , node , right);
    }
    return node;
}

Tree_Node* Parse_E ()
{
    Tree_Node* node = Parse_T ();
    while  (expr[p] == '+' || expr[p] == '-') {

        char op = expr[p];
        p++;
        Tree_Node* right = Parse_T ();
        node = New_Node (NODE_OPERATION , op , node , right);
    }
    return node;
}

// Главное правило G: E'$'
Tree_Node* Parse_G ()
{
    Tree_Node* node = Parse_E ();
    if  (expr[p] == '$') {
        p++;
    }
    else
        Syntax_Error ();
    return node;
}

Tree_Node* Parse_Expression (const char* input)
{
    expr = input;
    p = 0;
    Tree_Node* root = Parse_G ();
    if  (expr[p] != '\0') {
        printf ("Ошибка: неожиданный символ '%c' в конце выражения\n" , expr[p]);
        exit (EXIT_FAILURE);
    }
    return root;
}

#include "diff.h"

void Generate_Latex (Tree_Node* node, FILE* latex_file)
{
    if (node == NULL) return;

    if (node->type == NODE_NUMBER) {
        fprintf (latex_file, "%.1lf", node->num_value);

    } else if (node->type == NODE_VARIABLE) {
        fprintf (latex_file, "%c", node->var_name);

    } else if (node->type == NODE_OPERATION) {

        // Проверка на унарный минус
        if (node->operation == '-' && node->left != NULL && node->left->type == NODE_NUMBER && node->left->num_value == 0) {
            // Унарный минус, выводим только правое поддерево
            fprintf (latex_file, "-");
            Generate_Latex (node->right, latex_file);
        } else {
            Function* func = Find_Function (node->operation);
            if (func != NULL && func->type_operation == UNARY) {
                fprintf (latex_file, "%s (", func->latex_name);
                Generate_Latex (node->left, latex_file);
                fprintf (latex_file, ")");
            } else {

                if (node->operation == '^') {
                    fprintf (latex_file, "{");
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file, "}^{");
                    Generate_Latex (node->right, latex_file);
                    fprintf (latex_file, "}");
                } else {
                    // Для других бинарных операций, например x + y
                    fprintf (latex_file, " (");
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file, " %c ", node->operation);
                    Generate_Latex (node->right, latex_file);
                    fprintf (latex_file, ")");
                }
            }
        }
    }
}


void Generate_Latex_File (Tree_Node* root , const char* filename)
{
    FILE* latex_file = fopen (filename , "w");
    if  (!latex_file) {
        perror ("Failed to open LaTeX file");
        exit (EXIT_FAILURE);
    }

    fprintf (latex_file , "\\documentclass{article}\n");
    fprintf (latex_file , "\\usepackage{amsmath}\n");
    fprintf (latex_file , "\\begin{document}\n");
    fprintf (latex_file , "Expression: $");

     Generate_Latex (root , latex_file);

    fprintf (latex_file , "$\n");
    fprintf (latex_file , "\\end{document}\n");

    fclose (latex_file);
}

void Compile_Latex_To_Pdf (const char* latex_filename)
{
    char pdf_filename[256] = "";

    snprintf (pdf_filename , sizeof (pdf_filename) , "%s.pdf" , latex_filename);

    if  (remove (pdf_filename) == 0) {
        printf ("Удален старый PDF файл: %s\n" , pdf_filename);

    } else {
        printf ("Старый PDF файл не найден , продолжим компиляцию...\n");
    }

    char command[256] = "";
    snprintf (command , sizeof (command) , "pdflatex %s" , latex_filename);

    int result = system (command);
    if  (result != 0) {
        printf ("Ошибка компиляции LaTeX в PDF\n");
        exit (EXIT_FAILURE);
    }
}

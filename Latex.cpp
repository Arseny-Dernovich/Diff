#include "diff.h"

void Generate_Latex (Tree_Node* node, FILE* latex_file)
{
    if (node == NULL) return;

    if (node->type == NODE_NUMBER) {
        fprintf (latex_file, "%.0lf", node->num_value);

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
            printf ("MYYYYYYYYY %c " , func->name_in_file);
            if (func != NULL && func->type_operation == UNARY) {

                if (node->left->var_name == 'x') {

                    fprintf (latex_file, "%s ", func->latex_name);
                    Generate_Latex (node->left, latex_file);
                }

                else {

                    fprintf (latex_file, "%s (", func->latex_name);
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file, ")");
                }


            } else  {

                if (node->operation == '^') {

                    fprintf (latex_file, "{");
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file, "}^{");
                    Generate_Latex (node->right, latex_file);
                    fprintf (latex_file, "}");

                }

                else if (func->name_in_file == DIV) {

                    fprintf (latex_file , " %s{" , func->latex_name);
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file , "}{");
                    Generate_Latex (node->right, latex_file);
                    fprintf (latex_file , "}");
                }

                else {

                    fprintf (latex_file, " (");
                    Generate_Latex (node->left, latex_file);
                    fprintf (latex_file, " %s ", func->latex_name);
                    Generate_Latex (node->right, latex_file);
                    fprintf (latex_file, ")");
                }

            }
        }
    }
}


void Generate_Latex_For_Graphic (Tree_Node* node, FILE* latex_file)
{
    if (node == NULL) return;

    if (node->type == NODE_NUMBER) {
        fprintf (latex_file, "%.0lf", node->num_value);

    } else if (node->type == NODE_VARIABLE) {
        fprintf (latex_file, "%c", node->var_name);

    } else if (node->type == NODE_OPERATION) {

        // Проверка на унарный минус
        if (node->operation == '-' && node->left != NULL && node->left->type == NODE_NUMBER && node->left->num_value == 0) {
            // Унарный минус, выводим только правое поддерево
            fprintf (latex_file, "-");
            Generate_Latex_For_Graphic (node->right, latex_file);

        } else {

            Function* func = Find_Function (node->operation);
            // printf ("MYYYYYYYYY %c " , func->name_in_file);
            if (func != NULL && func->type_operation == UNARY) {

                if (func->name_in_file == 's') {

                    fprintf (latex_file, "sin (");
                    Generate_Latex_For_Graphic (node->left, latex_file);  // Рекурсивный вызов для левого поддерева
                    fprintf (latex_file, ")");
                }

                else if (func->name_in_file == 'c') {

                    fprintf (latex_file, "cos (");
                    Generate_Latex_For_Graphic (node->left, latex_file);  // Рекурсивный вызов для левого поддерева
                    fprintf (latex_file, ")");
                }

                else if (func->name_in_file == 'S') {

                    fprintf (latex_file, "sqrt (");
                    Generate_Latex_For_Graphic (node->left, latex_file);  // Рекурсивный вызов для левого поддерева
                    fprintf (latex_file, ")");
                }

                else if (func->name_in_file == 'L') {

                    fprintf (latex_file, "log (");
                    Generate_Latex_For_Graphic (node->left, latex_file);  // Рекурсивный вызов для левого поддерева
                    fprintf (latex_file, ")");
                }

            } else  {

                if (node->operation == '^') {

                    // fprintf (latex_file, "{");
                    Generate_Latex_For_Graphic (node->left, latex_file);
                    fprintf (latex_file, "**");
                    Generate_Latex_For_Graphic (node->right, latex_file);
                    // fprintf (latex_file, "}");

                }

                else if (func->name_in_file == DIV) {

                    // fprintf (latex_file , "{");
                    Generate_Latex_For_Graphic (node->left, latex_file);
                    fprintf (latex_file , "/");
                    Generate_Latex_For_Graphic (node->right, latex_file);
                    // fprintf (latex_file , "}");
                }

                else if (func->name_in_file == MUL) {

                    fprintf (latex_file , " (");
                    Generate_Latex_For_Graphic (node->left, latex_file);
                    fprintf (latex_file , " * ");
                    Generate_Latex_For_Graphic (node->right, latex_file);
                    fprintf (latex_file , ")");
                }

                else {

                    fprintf (latex_file, " (");
                    Generate_Latex_For_Graphic (node->left, latex_file);
                    fprintf (latex_file, " %s ", func->latex_name);
                    Generate_Latex_For_Graphic (node->right, latex_file);
                    fprintf (latex_file, ")");
                }

            }
        }
    }
}


void Generate_Gnuplot_Settings (FILE* gnuplot_file)
{
    fprintf (gnuplot_file, "set terminal pngcairo enhanced font \"Verdana,12\"\n");
    fprintf (gnuplot_file, "set output \"plot_graphic.png\"\n");
    fprintf (gnuplot_file, "set samples 10000\n");
    fprintf (gnuplot_file, "set title \"График функции\" font \"Verdana,14\" textcolor rgb \"blue\"\n");
    fprintf (gnuplot_file, "set xlabel \"x\" font \"Verdana,12\"\n");
    fprintf (gnuplot_file, "set ylabel \"f (x)\" font \"Verdana,12\"\n");

    fprintf (gnuplot_file, "set grid lw 1 lt 0 lc rgb \"#bbbbbb\"\n");
    fprintf (gnuplot_file, "set border lw 2\n");
    fprintf (gnuplot_file, "set tics font \"Verdana,10\"\n");

    fprintf (gnuplot_file, "set xrange [-20:20]\n");
    fprintf (gnuplot_file, "set yrange [-20:20]\n");

    fprintf (gnuplot_file, "set key top right box\n");

    fprintf (gnuplot_file, "plot ");
}


void Generate_Gnuplot_File (Tree_Node* root, const char* filename)
{
    FILE* gnuplot_file = fopen (filename, "w");
    if (!gnuplot_file) {
        perror ("Failed to open Gnuplot file");
        exit (EXIT_FAILURE);
    }

    Generate_Gnuplot_Settings (gnuplot_file);

    Generate_Latex_For_Graphic (root, gnuplot_file);

    fprintf (gnuplot_file, " with lines lw 2 lc rgb \"red\" title \"Function\"\n");

    fclose (gnuplot_file);

    printf ("Running Gnuplot with script: %s\n", filename);
    char command[256] = "";
    snprintf (command, sizeof (command), "gnuplot %s", filename);
    system (command);
}

// void Generate_Graphik (Tree_Node* root, FILE* latex_file)
// {
//     // Генерация графика в отдельном блоке figure
//     fprintf (latex_file, "\\begin{figure}[H]\n");
//     fprintf (latex_file, "    \\centering\n");  // Центрирование графика
//     fprintf (latex_file, "    \\begin{tikzpicture}\n");
//     fprintf (latex_file, "        \\begin{axis}[\n");
//     fprintf (latex_file, "            axis x line=middle,\n");
//     fprintf (latex_file, "            axis y line=middle,\n");
//     fprintf (latex_file, "            xlabel={$x$},\n");
//     fprintf (latex_file, "            ylabel={$f (x)$},\n");
//     fprintf (latex_file, "            xmin=-20, xmax=20,\n");
//     fprintf (latex_file, "            ymin=-20, ymax=20,\n");
//     fprintf (latex_file, "            samples=100,\n");
//     fprintf (latex_file, "            domain=0.1:20,\n");
//     fprintf (latex_file, "            grid=both,\n");
//     fprintf (latex_file, "            enlargelimits=true,\n");
//     fprintf (latex_file, "            axis line style={-},\n");
//     fprintf (latex_file, "        ]\n");
//
//     fprintf (latex_file, "            \\addplot[blue, thick] {");
//     Generate_Latex_For_Graphic (root, latex_file);  // Генерация графика через ваш метод
//     fprintf (latex_file, "};\n");
//
//     fprintf (latex_file, "        \\end{axis}\n");
//     fprintf (latex_file, "    \\end{tikzpicture}\n");
//     fprintf (latex_file, "\\end{figure}\n");
// }

void Generate_Latex_File (Tree_Node* root , Tree_Node* diff_root ,  const char* filename)
{
    FILE* latex_file = fopen (filename, "w");
    if (!latex_file) {
        perror ("Failed to open LaTeX file");
        exit (EXIT_FAILURE);
    }


    fprintf (latex_file, "\\documentclass{article}\n");
    fprintf (latex_file, " \\usepackage[utf8]{inputenc}\n");
     fprintf (latex_file, " \\usepackage[T2A]{fontenc}\n");
    fprintf (latex_file, "\\usepackage{amsmath}\n");
    fprintf (latex_file, "\\usepackage{amssymb}\n");
    fprintf (latex_file, "\\usepackage{graphicx}\n");
    fprintf (latex_file, "\\usepackage{float}\n");
    fprintf (latex_file, "\\begin{document}\n");


    fprintf (latex_file, "\\begin{figure}[H]\n");
    fprintf (latex_file, "  \\centering\n");
    fprintf (latex_file, "  \\includegraphics[width=0.8\\textwidth]{plot_graphic.png}\n");  // Путь к картинке
    fprintf (latex_file, "  \\caption{График функции}\n");
    // fprintf (latex_file, "  \\label{fig:graph}\n");
    fprintf (latex_file, "\\end{figure}\n");


    fprintf (latex_file, "\\vspace{1cm}\n");


    fprintf (latex_file, "Исходное уравнение: $");
    Generate_Latex (root, latex_file);
    fprintf (latex_file, "$\n\n");

    fprintf (latex_file, "\\vspace{1cm}\n");

    fprintf (latex_file, "Производная: $");
    Generate_Latex (diff_root, latex_file);
    fprintf (latex_file, "$\n\n");

    fprintf (latex_file, "\\vspace{1cm}\n");

    printf ("\n\n\n Введите порядок разложения для формулы Тейлора: ");
    int n = 0;
    scanf ("%d" , &n);

    Taylor (root , n , latex_file);





    fprintf (latex_file, "\\end{document}\n");

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




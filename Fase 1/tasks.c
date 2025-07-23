#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "tasks.h"

/**
 * Le a matriz do ficheiro, procurando o maior valor de energia
 * que se encontram a uma distancia par.k ou menor
 * 
 * @param par parametros do cabecalho
 * @param fp ficheiro de entrada
 * @return maior valor de energia encontrado
 */
int ler_matriz_1(NV_Param par, FILE* fp)
{
    int major = 0, whatever = 0;

    for(int i = 0; i < par.L; i++){
        for(int j = 0; j < par.C; j++){ 
            if(fscanf(fp, "%d ", &whatever) != 1)
                exit(0);

            if (whatever < 1)
                continue;

            int distance = abs(par.l1-1-i) + abs(par.c1-1-j);
            if (distance > par.k)
                continue;

            if (distance == 0){
                continue;
            }

            if(whatever > major)
                major = whatever;
        }
    }

    return major;
}


/**
 * Le a matriz do ficheiro, somando os valores de energia 
 * que se encontram a uma distancia par.k ou menor
 * 
 * @param par parametros do cabecalho
 * @param fp ficheiro de entrada
 * @return maior valor de energia encontrado
 */
int ler_matriz_2(NV_Param par, FILE* fp)
{
    int sum = 0, whatever = 0;

    for(int i = 0; i < par.L; i++){
        for(int j = 0; j < par.C; j++){ 
            if(fscanf(fp, "%d ", &whatever) != 1)
                exit(0);

            if (whatever < 1)
                continue;

            int distance = abs(par.l1-1-i) + abs(par.c1-1-j);
            if (distance > par.k)
                continue;


            if (distance == 0){
                continue;
            }

            sum = sum + whatever;
        }
    }
    return sum;
}


/**
 * Le a matriz do ficheiro para a memoria
 * 
 * @param par parametros do cabecalho
 * @param fp ficheiro de entrada
 * @return a matriz lida
 */
int** ler_matriz_3(NV_Param par, FILE* fp){

    int **matrix = (int **)malloc(par.L * sizeof(int *));
    for (int i = 0; i < par.L; i++) {
        matrix[i] = (int *)malloc(par.C * sizeof(int));
    }

    for(int i = 0; i < par.L; i++)
        for(int j = 0; j < par.C; j++) 
            if(fscanf(fp, "%d ", &matrix[i][j]) != 1)
                exit(0);

    return matrix;
}

// inicia e imprime o resultado da tarefa 1
void tarefa_1(NV_Param par, FILE *f_in, FILE *f_out)
{    
    par.k = -par.k;

    int major;
    major = ler_matriz_1(par,f_in);

    //faz print do cabecalho com o resultado
    fprintf(f_out,"%d %d %d %d %d %d\n", par.L, par.C, par.l1, par.c1, -par.k, major); 
}


// inicia e imprime o resultado da tarefa 2
void tarefa_2(NV_Param par, FILE *f_in, FILE *f_out)
{    
    int sum;

    sum = ler_matriz_2(par,f_in);

    //faz print do cabecalho com o resultado
    fprintf(f_out,"%d %d %d %d %d %d\n", par.L, par.C, par.l1, par.c1, par.k, sum); 
}

// percorre verticalmente e de seguida horizontalmente a matriz
// e imprime o resultado
void tarefa_3(NV_Param par, FILE *f_in, FILE *f_out){

    fprintf(f_out, "%d %d %d %d %d %d %d\n", par.L, par.C, par.l1, par.c1,  par.k, par.l2, par.c2);

    int** matrix = ler_matriz_3(par, f_in);

    // verifica se tem que descer ou subir
    if(par.l1 > par.l2){
        for (int i = par.l1-1; i > par.l2-1; i--) 
            fprintf(f_out, "%d %d %d\n", i, par.c1, matrix[i-1][par.c1-1]);
    } else {
        for (int i = par.l1+1; i < par.l2+1; i++)
            fprintf(f_out, "%d %d %d\n", i, par.c1, matrix[i-1][par.c1-1]);
    }

    // verifica se tem de ir para a esquerda ou direita
    if(par.c1 > par.c2){
        for (int i = par.c1-1; i > par.c2-1; i--)
            fprintf(f_out, "%d %d %d\n", par.l2, i, matrix[par.l2-1][i-1]);
    } else {
        for (int i = par.c1+1; i < par.c2+1; i++)
            fprintf(f_out, "%d %d %d\n", par.l2, i, matrix[par.l2-1][i-1]);
    }

    for (int i = 0; i < par.L; i++)
        free(matrix[i]);
    free(matrix);

}




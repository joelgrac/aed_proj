#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include "file.h"

int bound1(int currentsum, int** matrix, int rows, int cols, int startRow, int startcol,int steps_remaining,int** visited);
void tarefa_2(NV_Param par, FILE* f_in, FILE* f_out);
void tarefa_1(NV_Param par, FILE* f_in, FILE* f_out);
int dfs(int** mapa, int L, int C, int i, int j, int passos_restantes, int energia_atual, int** visitado, int** caminho, int* indice_caminho, int k,int* abort, int target_energy);
int dfs2(int** mapa, int L, int C, int i, int j, int passos_restantes, int energia_atual, int** visitado, int** caminho, int* indice_caminho, int k,int* abort);

#endif

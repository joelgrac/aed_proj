#include <stdlib.h>
#include <stdio.h>
#ifndef FILE_H
#define FILE_H

/*****************************************************************************
 * typedef struct NV_Param
 *
 * Campos:
 *  - L, C: dimensões do mapa (linhas e colunas)
 *  - k: parâmetro específico para as tarefas
 *  - l1, c1: coordenadas iniciais no mapa
 *  - task_energy: indicador de tarefa
 *  - e_ini: energia inicial
 ****************************************************************************/
typedef struct NV_Param {
    int L, C, k, l1, c1, task_energy, e_ini;
} NV_Param;


int leitura_cabecalho(NV_Param*, FILE*);
int verificar_cabecalho(NV_Param);
void problema_mal_definido(NV_Param, FILE*, FILE*);
#endif

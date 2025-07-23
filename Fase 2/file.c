#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"


/*****************************************************************************
 * leitura_cabecalho()
 *
 * Arguments: par - ponteiro para a estrutura NV_Param onde serão armazenados
 *                    os parâmetros do cabeçalho
 *            fp - ponteiro para o arquivo de entrada onde os dados estão armazenados
 *
 * Returns: 1 se a leitura for bem-sucedida, 0 caso contrário
 *
 * Description: Lê e armazena os valores do cabeçalho de um arquivo de entrada,
 *              preenchendo os campos da estrutura NV_Param com valores L, C, 
 *              task_energy, l1, c1, k, e e_ini. A função retorna 1 em caso de
 *              sucesso e 0 se algum valor não conseguir ser lido e o cabeçalho não tem formato com 7 números
 ****************************************************************************/
int leitura_cabecalho(NV_Param* par, FILE* fp)
{
    if(fscanf(fp, "%d %d %d %d %d %d %d\n", &par->L, &par->C, &par->task_energy, &par->l1, &par->c1,&par->k, &par->e_ini) != 7)
        return 0;

    return 1;
}

/*****************************************************************************
 * verificar_cabecalho()
 *
 * Arguments: par - estrutura NV_Param contendo os parâmetros lidos do cabeçalho
 *
 * Returns: 1 se o cabeçalho estiver correto, 0 caso contrário
 *
 * Description: Verifica se os valores do cabeçalho estão dentro dos limites 
 *              definidos para o problema. Retorna 1 se os valores são válidos
 *              e 0 caso algum valor esteja fora dos limites.
 ****************************************************************************/
int verificar_cabecalho(NV_Param par)
{
    if(par.l1 > par.L || par.c1 > par.C || par.l1 < 1 || par.c1 < 1 || par.L < 1 || par.C < 1 || (par.task_energy!=-2 && par.task_energy<=0) || par.k<=0|| par.e_ini<1) 
        return 0;

    return 1;
}

/*****************************************************************************
 * problema_mal_definido()
 *
 * Arguments: par - estrutura NV_Param com os parâmetros do cabeçalho
 *            f_in - ponteiro para o arquivo de entrada, onde os dados estão armazenados
 *            f_out - ponteiro para o arquivo de saída onde a mensagem de erro será escrita
 *
 * Returns: void
 * Side-Effects: escreve uma mensagem de erro no arquivo de saída e ignora dados que constituem um porblema mal definido
 *
 * Description: Quando o problema está mal definido, esta função escreve os parâmetros
 *              lidos no arquivo de saída e ignora o conteúdo da matriz LxC e avança pelo arquivo sem processamento adicional.
 *              `whatever` atua como um buffer temporário para ignorar os dados da matriz.
 ****************************************************************************/
void problema_mal_definido(NV_Param par, FILE *f_in, FILE *f_out){

    int whatever;
        fprintf(f_out, "%d %d %d %d %d %d %d \n\n", par.L, par.C, par.task_energy, par.l1, par.c1, par.k, par.e_ini);
        for (int i = 0; i < par.L*par.C; i++){
            if (fscanf(f_in, "%d ", &whatever) != 1)
                exit(0);
        }


}

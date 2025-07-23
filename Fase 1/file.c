#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"


// Le o cabecalho 
int leitura_cabecalho(NV_Param* par, FILE* fp)
{
    if(fscanf(fp, "%d %d %d %d %d", &par->L, &par->C, &par->l1, &par->c1, &par->k) != 5)
        return 0;

    return 1;
}

// Le os numeros restantes do cabecalho tarefa 3
void leitura_cabecalho_3(NV_Param* par, FILE* fp)
{
    if(fscanf(fp, " %d %d\n", &par->l2, &par->c2) != 2)
        exit(0);
}

// Verifica o cabecalho
// Retorna 0 se estiver incorreto, 1 se estiver correto
int verificar_cabecalho(NV_Param par)
{
    if(par.l1 > par.L || par.c1 > par.C || par.l1 < 1 || par.c1 < 1 || par.L < 1 || par.C < 1) // sera que uma grid de 1x1 seria aceite lol
        return 0;

    return 1;
}

// Verifica o cabecalho da tarefa 3
// Retorna 0 se estiver incorreto, 1 se estiver correto
int verificar_cabecalho_3(NV_Param par)
{
    if (!verificar_cabecalho(par))
        return 0;


    if (par.l2 > par.L || par.c2 > par.C || par.l2 < 1 || par.c2 < 1)
        return 0;

    return 1;

}

// Faz print do suposto quando o problema esta mal definido
// E passa a frente L*C
void problema_mal_definido(NV_Param par, FILE *f_in, FILE *f_out){

    int whatever;
    if (par.k == 0){
        fprintf(f_out, "%d %d %d %d %d %d %d\n\n", par.L, par.C, par.l1, par.c1, par.k, par.l2, par.c2);
        for (int i = 0; i < par.L*par.C; i++){
            if (fscanf(f_in, "%d ", &whatever) != 1)
                exit(0);
        }
    } else {
        fprintf(f_out, "%d %d %d %d %d\n\n", par.L, par.C, par.l1, par.c1, par.k);
        for (int i = 0; i < par.L*par.C; i++){
            if (fscanf(f_in, "%d ", &whatever) != 1)
                exit(0);
        }
    }


}
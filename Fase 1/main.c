#include <stdlib.h>
#include <stdio.h>

#include "file.h"
#include "tasks.h"

int main(int argc, char* argv[]){

    if(argc != 2){
        return 0;
    }

    const char *extensao = strrchr(argv[1], '.'); 

    if (extensao == NULL || strcmp(extensao, ".1maps") != 0) {
        return 0; // Sai do programa silenciosamente
    }

    size_t len = strlen(argv[1]);
    char *filename_out = (char*) malloc(sizeof(char) * (len - 6 + 10));
    if (filename_out == NULL) {
        return 0;
    }

    // Copia o nome do arquivo sem a extensão
    strcpy(filename_out, argv[1]);
    filename_out[len - 6] = '\0';

    // Adiciona "_out" ao final do nome do arquivo
    strcat(filename_out, ".sol1maps");
    filename_out[len+6+9]='\0';


    FILE *f_in;
    f_in = fopen(argv[1],"r");

    if (f_in == NULL){
        return 0;
    }   

    FILE *f_out;
    f_out = fopen(filename_out,"w");

    free(filename_out);


    while(1){
        NV_Param par;

        if (!leitura_cabecalho(&par,f_in)){
            break;
        }

        if (par.k < 0){
            if(!verificar_cabecalho(par)){
                problema_mal_definido(par, f_in, f_out);
                continue;
            }
            tarefa_1(par,f_in,f_out);
        }

        if (par.k > 0){
            if(!verificar_cabecalho(par)){
                problema_mal_definido(par, f_in, f_out);
                continue;
            }
            tarefa_2(par,f_in,f_out);
        }

        if(par.k == 0){
            leitura_cabecalho_3(&par, f_in);
            if(!verificar_cabecalho_3(par)){
                problema_mal_definido(par, f_in, f_out);
                continue;
            }
            tarefa_3(par,f_in,f_out);
        }
        
        fprintf(f_out,"\n");
    }

    fclose(f_in);
    fclose(f_out);

    return 0;
}
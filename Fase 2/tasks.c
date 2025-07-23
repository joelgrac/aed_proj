#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include "tasks.h"

/* estrutura para representar o estado de uma celula na pilha */
typedef struct {
    int i, j;              /* posição atual no mapa */
    int passos_restantes;  /* passos restantes */
    int energia_atual;     /* energia acumulada até o momento */
    int direcao;            /* direção atual para explorar (0 a 3: cima,baixo,esquerda,direita) */
} Estado;



/****************************************************************************** 
 * bound1 ()
 *
 * Argumentos: currentsum - soma atual de valores acumulados
 *            matrix - matriz representando o mapa (matriz de inteiros)
 *            rows - número de linhas na matriz
 *            cols - número de colunas na matriz
 *            startRow - linha inicial a partir da qual a soma deve ser calculada
 *            startcol - coluna inicial a partir da qual a soma deve ser calculada
 *            steps_remaining - número de passos restantes para explorar
 *            visited - matriz que indica quais células já foram visitadas
 *
 * Retorna: local_sum - a soma total dos valores acessíveis a partir da posição
 *          inicial, considerando os passos restantes e as células visitadas
 *
 * Side-Effects: nenhuma (não altera estados fora da função)
 *
 * Description: calcula a soma dos valores nas células acessíveis dentro do
 *              alcance determinado por steps_remaining. Apenas células que
 *              não foram visitadas e têm valores positivos são consideradas
 *              na soma.
 *
 *****************************************************************************/

int bound1(int currentsum, int** matrix, int rows, int cols, int startRow, int startcol,int steps_remaining,int** visited) {
    int local_sum=currentsum;   
    int aux=0;  /* variável auxiliar para controlar o alcance da soma nas direcoes vertical e horizontal */
    /* incrementa índices para que comecem em 1*/
    startRow++;
    startcol++;
    /* percorre as linhas e colunas dentro do alcance dos passos restantes*/
    for (int i=startRow-steps_remaining;i<=(startRow+steps_remaining);i++){
        for(int j=startcol-aux;j<=startcol+aux;j++){
            if(i>0 && i<=rows && j>0 && j<=cols){   /* verifica se a posicao esta dentro dos limites da matriz*/
                if(visited[i-1][j-1]==0 && matrix[i-1][j-1]>0){   /* verifica se a celula não foi visitada e se tem valor positivo*/
                    local_sum=local_sum+matrix[i-1][j-1];   /* adiciona o valor da celula a soma atual*/
                }
            }
        }
        /* altera a largura do alcance dependendo da posição da linha atual em relacao a startRow */
        if(i<startRow){
            aux++;
        }
        else{
            aux--;
        }
    }
    return local_sum;  /*soma total calculada*/
}

/****************************************************************************** 
 * dfs ()
 *
 * Argumentos: mapa - matriz representando o mapa de energias (matriz de inteiros)
 *            L - numero de linhas do mapa
 *            C - numero de colunas do mapa
 *            i - linha inicial da exploracao
 *            j - coluna inicial da exploracao
 *            passos_restantes - numero de passos restantes para explorar
 *            energia_atual - energia acumulada ate o momento
 *            visitado - matriz que indica quais celulas ja foram visitadas
 *            caminho_final - matriz para armazenar o caminho que atinge a energia alvo
 *            indice_caminho - indice para rastrear o caminho
 *            k - numero exato de passos que devem ser dados
 *            abort - flag para indicar se a exploracao deve ser abortada
 *            target_energy - energia alvo a ser atingida ou superada
 *
 * Retorna: max_energia - a energia maxima acumulada ao final da exploração
 *
 * Side-Effects: modifica a matriz visitado e pode alterar estados fora da função
 *
 * Description: realiza uma busca em profundidade (dfs) para explorar o mapa,
 *              tentando atingir ou superar a energia alvo em exatamente k passos.
 *              A funcao armazena o caminho que leva a essa energia e realiza
 *              backtracking quando necessário.
 *
 *****************************************************************************/

int dfs(int** mapa, int L, int C, int i, int j, int passos_restantes, int energia_atual, int** visitado, int** caminho_final, int* indice_caminho, int k,int* abort, int target_energy) {
    /* direcoes para explorar (cima, baixo, esquerda, direita) */
    int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    /* criacao da pilha de estados */
    Estado* pilha = (Estado*) malloc(L * C * sizeof(Estado));  /* alocar para tamanho máximo da pilha:L * C */
    int topo = -1,max_energia=-1;  /* inicialmente, a pilha está vazia */
    int counter=0; /* variável auxiliar de controlo para que o percurso não ultrapasse o número de passos permitidos*/

    int aux; /* contador que rastreia quantos passos no caminho atual são iguais aos do caminho inicial*/

    *abort = 1;

    
    int** caminho = (int**)malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        caminho[i] = (int*)malloc(3 * sizeof(int));  /*3 para linha, coluna e valor da célula*/
    }
    int** caminho_inicial = (int**)malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        caminho_inicial[i] = (int*)malloc(3 * sizeof(int)); /*3 para linha, coluna e valor da célula*/
    }

    /* empilha o estado inicial */
    topo++;
    pilha[topo].i = i;
    pilha[topo].j = j;
    pilha[topo].passos_restantes = passos_restantes;
    pilha[topo].energia_atual = energia_atual;
    pilha[topo].direcao = 0;  /* começa a explorar a primeira direção */
    caminho[k-1][0]=-1;       /* inicializa o último elemento do caminho */
    
    /* loop principal do DFS */
    while (topo >= 0) {
        Estado atual = pilha[topo];
        i = atual.i;
        j = atual.j;
        passos_restantes = atual.passos_restantes;
        energia_atual = atual.energia_atual;

        /* atualiza a energia máxima se as condições se verificam*/
        if (max_energia<energia_atual && passos_restantes ==0){
            max_energia=energia_atual;
            for (int i=0;i<k;i++){
                caminho_final[i][0] = caminho[i][0];  
                caminho_final[i][1] = caminho[i][1];  
                caminho_final[i][2] = caminho[i][2];
            }
        }
        /* verifica se a energia máxima atingiu a e_alvo e se o caminho e válido */
        if (max_energia>=target_energy && caminho[k-1][0]!=-1)
        {   
            
            *abort = 0; /* indica que a exploração foi bem-sucedida*/
            
            /* liberta a memoria alocada */
            for (int i = 0; i < k; i++) {
                free(caminho[i]); 
            }
            for (int i = 0; i < k; i++) {
                free(caminho_inicial[i]);
            }
            free(caminho);
            free(caminho_inicial);
            return max_energia;
        }

        /* se nao restam passos, retorna a energia acumulada */
        /* marca a celula como visitada */
        visitado[i][j] = 1;
        
        /* explora as direcoes */
        int encontrou_caminho = 0;
        while (pilha[topo].direcao < 4) {
            int novo_i = i + direcoes[pilha[topo].direcao][0];
            int novo_j = j + direcoes[pilha[topo].direcao][1];

            /* avanca a direcao para que nao seja explorada novamente */
            pilha[topo].direcao++;

            // Verifica se está dentro dos limites do mapa
            if (novo_i >= 0 && novo_i < L && novo_j >= 0 && novo_j < C ) {
                if (visitado[novo_i][novo_j] == 0){
                    int energia_celula = mapa[novo_i][novo_j];
                    int nova_energia = energia_atual + energia_celula;
                    visitado[novo_i][novo_j]=1;

                    /* se a nova energia eh valida e a celula nao foi visitada */
                    if ( passos_restantes>0 && nova_energia > 0 && bound1(nova_energia, mapa, L, C, novo_i, novo_j, passos_restantes-1, visitado)>=target_energy) {
                        /* regista o caminho se nao ultrapassar k passos */
                        if (*indice_caminho < k) {
                            caminho[*indice_caminho][0] = novo_i + 1;   /* ajusta para 1-indexed */
                            caminho[*indice_caminho][1] = novo_j + 1;  /* ajusta para 1-indexed */
                            caminho[*indice_caminho][2] = energia_celula;  /* regista o valor da celula */
                            (*indice_caminho)++;
                        }
                         /* empilha o novo estado */
                        topo++;
                        pilha[topo].i = novo_i;
                        pilha[topo].j = novo_j;
                        pilha[topo].passos_restantes = passos_restantes - 1;
                        pilha[topo].energia_atual = nova_energia;
                        pilha[topo].direcao = 0;  /* comeca a explorar a primeira direcao */
                        encontrou_caminho = 1;

                        if (counter<k){
                            caminho_inicial[counter][0] = novo_i + 1;  /* ajusta para 1-indexed */
                            caminho_inicial[counter][1] = novo_j + 1;  /* ajusta para 1-indexed */
                            caminho_inicial[counter][2] = energia_celula;
                        }
                        counter++;
                        break;  /* sai do loop de direcao para processar o novo estado */
                    }
                    else{
                        visitado[novo_i][novo_j]=0; /* marca como nao visitado se nao e valido */
                    }
                }
                
            }
        }
        aux=0;
        /* verifica se o caminho ultrapassou o limite k */
        if(counter>k){
            for (int i =0; i<k;i++){
                /* verifica se o passo atual e igual ao passo inicial */
                if (caminho[i][0] == caminho_inicial[i][0] && caminho[i][1] == caminho_inicial[i][1] && caminho[i][2] == caminho_inicial[i][2]){
                    aux++;     /*contando quantos sao iguais*/
                }
                if (aux==k){   /* se caminho alternativo e igual ao inical*/
                    for (int i = 0; i < k; i++) {
                        free(caminho[i]); 
                    }
                    for (int i = 0; i < k; i++) {
                        free(caminho_inicial[i]);
                    }
                    free(caminho);
                    free(caminho_inicial);
                    return max_energia; /* retorna energia maxima*/
                }
            }
        }
        /* se nao encontrou um caminho valido, faz o backtrack */
        if (!encontrou_caminho) {
            if (topo > 0) {
                visitado[i][j] = 0;  /* marca a celula atual como nao visitada */
                topo--;              /* desempilha o estado atual */
                energia_atual = pilha[topo].energia_atual;  /* restaura a energia do estado anterior */
                passos_restantes++;
                (*indice_caminho)--;
            } else {
                topo--;  /* desempilha o estado atual */
                passos_restantes++;
                (*indice_caminho)--;
            }
        }

    }
    /* se não conseguimos encontrar um caminho atual válido dentro do limite de passos permitido (k)*/ 
    if (counter<k)
        *abort=1;  /* exploracao deve ser abortada */
    
    /* se nao encontrou solucao, liberta memoria*/
    for (int i = 0; i < k; i++) {
        free(caminho[i]); 
    }
    for (int i = 0; i < k; i++) {
        free(caminho_inicial[i]);
    }
    free(caminho);
    free(caminho_inicial);
    return max_energia;   /*energia maxima encontrada*/
}

/****************************************************************************** 
 * dfs2 ()
 *
 * Argumentos: mapa            - matriz representando o mapa de energias (matriz de inteiros)
 *            L               - número de linhas do mapa
 *            C               - número de colunas do mapa
 *            i               - linha inicial da exploração
 *            j               - coluna inicial da exploração
 *            passos_restantes - número de passos restantes para atingir exatamente k passos
 *            energia_atual   - energia acumulada até o momento da chamada
 *            visitado        - matriz que indica as células já visitadas (para evitar loops)
 *            caminho_final   - matriz para armazenar o caminho que resulta na máxima energia
 *            indice_caminho  - índice para rastrear o caminho atual
 *            k               - número exato de passos a serem dados
 *            abort           - flag para indicar se a exploração deve ser abortada em casos específicos
 *
 * Retorna: max_energia - o valor máximo de energia acumulada ao final de exatamente k passos
 *
 * Side-Effects: Modifica a matriz de células visitadas e o caminho final, armazenando o caminho
 *               com a maior energia após k passos.
 *
 * Description: Realiza uma busca em profundidade (DFS) para explorar o mapa em busca da
 *              energia máxima acumulada ao final de exatamente k passos. A função realiza
 *              backtracking quando necessário e armazena o caminho correspondente à máxima
 *              energia alcançada dentro das restrições de passos.
 *
 *****************************************************************************/

int dfs2(int** mapa, int L, int C, int i, int j, int passos_restantes, int energia_atual, int** visitado, int** caminho_final, int* indice_caminho, int k,int* abort) {
     /* direcoes para explorar (cima, baixo, esquerda, direita) */
    int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    /* criacao da pilha de estados */
    Estado* pilha = (Estado*) malloc(L * C * sizeof(Estado));  /* alocar para tamanho maximo da pilha:L * C */
    int topo = -1,max_energia=-1,counter=0,aux;  /* inicialmente, a pilha está vazia */

    /* aloca memoria para o caminho  e caminho_inicial*/
    int** caminho = (int**)malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        caminho[i] = (int*)malloc(3 * sizeof(int)); /*3 para linha, coluna e valor da célula*/
    }
    int** caminho_inicial = (int**)malloc(k * sizeof(int*));
    for (int i = 0; i < k; i++) {
        caminho_inicial[i] = (int*)malloc(3 * sizeof(int)); /*3 para linha, coluna e valor da célula*/
    }

    /* empilha o estado inicial */
    topo++;
    pilha[topo].i = i;
    pilha[topo].j = j;
    pilha[topo].passos_restantes = passos_restantes;
    pilha[topo].energia_atual = energia_atual;
    pilha[topo].direcao = 0;  /* comeca a explorar a primeira direcao */
    while (topo >= 0) {
        Estado atual = pilha[topo];
        i = atual.i;
        j = atual.j;
        passos_restantes = atual.passos_restantes;
        energia_atual = atual.energia_atual;
        /* verifica se a energia atual é maior que a max energia e se restam passos */
        if (max_energia<energia_atual && passos_restantes ==0){
            max_energia=energia_atual;
            for (int i=0;i<k;i++){
                caminho_final[i][0] = caminho[i][0];  
                caminho_final[i][1] = caminho[i][1];  
                caminho_final[i][2] = caminho[i][2];
            }
        }



        // Se não restam passos, retorna a energia acumulada
        /* marca a celula como visitada */
        visitado[i][j] = 1;
        
        /* explora as direcoes */
        int encontrou_caminho = 0;
        while (pilha[topo].direcao < 4) {
            int novo_i = i + direcoes[pilha[topo].direcao][0];
            int novo_j = j + direcoes[pilha[topo].direcao][1];

            /* avança a direcao para que nao seja explorada novamente */
            pilha[topo].direcao++;

            /* verifica se esta dentro dos limites do mapa */
            if (novo_i >= 0 && novo_i < L && novo_j >= 0 && novo_j < C ) {
                if (visitado[novo_i][novo_j] == 0){
                    int energia_celula = mapa[novo_i][novo_j];
                    int nova_energia = energia_atual + energia_celula;
                    visitado[novo_i][novo_j]=1;
                    /* se a nova energia é valida e a celula nao foi visitada */
                    if ( passos_restantes>0 && nova_energia > 0 && bound1(nova_energia, mapa, L, C, novo_i, novo_j, passos_restantes-1, visitado)>max_energia) {
                        /* registra o caminho se nao ultrapassar k */
                        if (*indice_caminho < k) {
                            caminho[*indice_caminho][0] = novo_i + 1;    /* ajusta para 1-indexed */
                            caminho[*indice_caminho][1] = novo_j + 1;    /* ajusta para 1-indexed */
                            caminho[*indice_caminho][2] = energia_celula; /* registra o valor da celula */
                            (*indice_caminho)++;
                        }
                        /* empilha o novo estado */
                        topo++;
                        pilha[topo].i = novo_i;
                        pilha[topo].j = novo_j;
                        pilha[topo].passos_restantes = passos_restantes - 1;
                        pilha[topo].energia_atual = nova_energia;
                        pilha[topo].direcao = 0;  /* comeca a explorar a primeira direcao */
                        encontrou_caminho = 1;

                        if (counter<k){
                            caminho_inicial[counter][0] = novo_i + 1;  /* ajusta para 1-indexed */
                            caminho_inicial[counter][1] = novo_j + 1;  /* ajusta para 1-indexed */
                            caminho_inicial[counter][2] = energia_celula;
                        }
                        counter++;
                        break;   /* sai do loop de direcao para processar o novo estado */
                    }
                    else{
                        visitado[novo_i][novo_j]=0;
                    }
                }
                
            }
        }
        aux=0;
        /* verifica se o caminho ultrapassou o limite k */
        if(counter>k){
            for (int i =0; i<k;i++){
                if (caminho[i][0] == caminho_inicial[i][0] && caminho[i][1] == caminho_inicial[i][1] && caminho[i][2] == caminho_inicial[i][2]){
                    aux++;
                }
                if (aux==k){
                    for (int i = 0; i < k; i++) {
                        free(caminho[i]); 
                    }
                    for (int i = 0; i < k; i++) {
                        free(caminho_inicial[i]);
                    }
                    free(caminho);
                    free(caminho_inicial);
                    return max_energia;
                }
            }
        }
        /* se nao encontrou um caminho valido, faz o backtrack */
        if (!encontrou_caminho) {
            if (topo > 0) {
                visitado[i][j] = 0; /* marca a celula atual como nao visitada */
                topo--;             /* desempilha o estado atual */
                energia_atual = pilha[topo].energia_atual;  /* restaura a energia do estado anterior */
                passos_restantes++;
                (*indice_caminho)--;
            } else {
                topo--;  /* desempilha o estado atual */
                passos_restantes++;
                (*indice_caminho)--;
            }
        }

    }
    if (counter<k)
        *abort=1;
    /*liberta memoria*/
    for (int i = 0; i < k; i++) {
        free(caminho[i]); 
    }
    for (int i = 0; i < k; i++) {
        free(caminho_inicial[i]);
    }
    free(caminho);
    free(caminho_inicial);
    return max_energia;    /*retorna energia maxima*/
}

/****************************************************************************** 
 * tarefa_2 ()
 *
 * Argumentos: par      - estrutura contendo parâmetros de entrada, como dimensões do mapa,
 *                        posição inicial, energia inicial e o número exato de passos (k)
 *            f_in      - ponteiro para o arquivo de entrada contendo o mapa de energias
 *            f_out     - ponteiro para o arquivo de saída onde o resultado será impresso
 *
 * Retorna: void
 *
 * Side-Effects: Escreve no arquivo de saída o resultado da exploração para atingir a
 *               energia máxima em exatamente k passos. Também libera a memória alocada
 *               para as estruturas auxiliares usadas na exploração.
 *
 * Description: Inicializa e carrega o mapa a partir do arquivo de entrada, configura a
 *              matriz de visitados e chama a função dfs2 para explorar o mapa, buscando
 *              a energia máxima após exatamente k passos. O resultado (energia e caminho)
 *              é escrito no arquivo de saída. A função garante a liberação de toda a memória
 *              alocada para a tarefa.
 *
 *****************************************************************************/

void tarefa_2(NV_Param par, FILE *f_in, FILE *f_out) {    
    /* inicializa o mapa */
    int** mapa = (int **)malloc(par.L * sizeof(int *));
    for (int i = 0; i < par.L; i++) {
        mapa[i] = (int *)malloc(par.C * sizeof(int));
        for (int j = 0; j < par.C; j++) {
            if (fscanf(f_in, "%d", &mapa[i][j]) != 1) {
                exit(0);
            }
        }
    }

    
    int abort=0; /* flag para sinalizar se a busca deve ser abortada em determinados casos*/
    
    /* inicializa o array de visitado para acompanhar celulas ja visitadas */
    int** visitado = (int**) malloc(par.L * sizeof(int*));
    for (int i = 0; i < par.L; i++) {
        visitado[i] = (int*) calloc(par.C, sizeof(int)); /* inicializa com zeros para indicar "nao visitado" */
    }

     /* cria o array caminho para armazenar o caminho encontrado */
    int** caminho = (int**)malloc(par.k * sizeof(int*));  /* aloca espaco para armazenar o caminho encontrado */
    for (int i = 0; i < par.k; i++) {
        caminho[i] = (int*)malloc(3 * sizeof(int));   /* armazena 3 para linha, coluna e valor da celula */
    }
    int indice_caminho = 0;  /* indice para acompanhar o caminho atual */



    /* inicializa energia acumulada e passos restantes com os valores iniciais */
    int energia_acumulada = par.e_ini; /* comeca com a energia inicial */
    int passos_restantes = par.k;  /* define os passos restantes, que devem ser exatamente k */

    /* chama a dfs atualizada, com a celula inicial ajustada para indice 0 */
    energia_acumulada = dfs2(mapa, par.L, par.C, par.l1 - 1, par.c1 - 1, passos_restantes, energia_acumulada, visitado, caminho, &indice_caminho, par.k,&abort);
    
    /* imprime o cabecalho do resultado, incluindo energia acumulada se possivel */
    if(abort==1 || energia_acumulada <= 0){
        /* caso em que nenhum caminho valido com exatamente k passos foi encontrado */
        fprintf(f_out, "%d %d %d %d %d %d %d -1\n", par.L, par.C, par.task_energy, par.l1, par.c1, par.k, par.e_ini);
    }
    else{
        /* caso em que um caminho valido foi encontrado, atingindo a energia acumulada maxima */
        fprintf(f_out, "%d %d %d %d %d %d %d %d\n", par.L, par.C, par.task_energy, par.l1, par.c1, par.k, par.e_ini,energia_acumulada);
        /* imprime o caminho completo se foi encontrado com sucesso */
        for (int i = 0; i < par.k; i++) {
            fprintf(f_out, "%d %d %d\n", caminho[i][0], caminho[i][1], caminho[i][2]); // Linha, Coluna, Valor da célula
        }
    }

    /* liberta a memoria alocada para o mapa e a matriz de visitados */
    for (int i = 0; i < par.L; i++) {
        free(mapa[i]);
        free(visitado[i]);
    }
    free(mapa);
    free(visitado);

    /* libera a memoria alocada para o caminho encontrado */
    for (int i = 0; i < par.k; i++) {
        free(caminho[i]);
    }
    free(caminho);
}

/****************************************************************************** 
 * tarefa_1 ()
 *
 * Argumentos: par - estrutura NV_Param que contem parametros da tarefa e mapa
 *             f_in - ponteiro para o arquivo de entrada contendo o mapa
 *             f_out - ponteiro para o arquivo de saida para imprimir os resultados
 *
 * Retorna: void
 *
 * Side-Effects: grava no arquivo de saida o maior caminho encontrado com a energia 
 *               acumulada, ou -1 caso nao seja possivel
 *
 * Descricao: realiza a primeira tarefa do projeto, onde busca-se atingir ou superar 
 *            uma energia alvo utilizando exatamente k passos. A funcao utiliza 
 *            uma busca em profundidade para explorar caminhos no mapa, e imprime 
 *            o caminho valido encontrado com energia acumulada que satisfaça a tarefa.
 *            Caso nao seja encontrado, grava -1 no arquivo de saida.
 *
 *****************************************************************************/


void tarefa_1(NV_Param par, FILE *f_in, FILE *f_out) {    
    /* inicializar o mapa */
    int** mapa = (int **)malloc(par.L * sizeof(int *));
    for (int i = 0; i < par.L; i++) {
        mapa[i] = (int *)malloc(par.C * sizeof(int));
        for (int j = 0; j < par.C; j++) {
            if (fscanf(f_in, "%d", &mapa[i][j]) != 1) {   /* ler valores do mapa do arquivo de entrada */
                exit(0);
            }
        }
    }
    /* inicializar a variavel flag de controlo abort */
    int abort;

    /* inicializar a matriz de celulas visitadas */
    int** visitado = (int**) malloc(par.L * sizeof(int*));
    for (int i = 0; i < par.L; i++) {
        visitado[i] = (int*) calloc(par.C, sizeof(int));
    }

    /* criar matriz para armazenar o caminho e o indice do caminho */
    int** caminho = (int**)malloc(par.k * sizeof(int*));
    for (int i = 0; i < par.k; i++) {
        caminho[i] = (int*)malloc(3 * sizeof(int)); // 3 para linha, coluna e valor da célula
    }
    int indice_caminho = 0; /* indice para acompanhar o caminho */


     /* chamada para a funcao dfs para explorar o mapa */
    int energia_acumulada = par.e_ini; /* comeca com a energia inicial */
    int passos_restantes = par.k;  

    /* chamada para a dfs, nao contabilizando a celula inicial */
    energia_acumulada = dfs(mapa, par.L, par.C, par.l1 - 1, par.c1 - 1, passos_restantes, energia_acumulada, visitado, caminho, &indice_caminho, par.k,&abort,par.task_energy);
    /* verifica se foi encontrado um caminho valido com energia acumulada suficiente */
    if(energia_acumulada>=par.task_energy && abort==0){
        /* imprimir cabecalho com os valores relevantes e energia acumulada */
        fprintf(f_out, "%d %d %d %d %d %d %d %d\n", par.L, par.C, par.task_energy, par.l1, par.c1, par.k, par.e_ini,energia_acumulada);
        /* imprimir caminho encontrado */
        for (int i = 0; i < indice_caminho; i++) {
            fprintf(f_out, "%d %d %d\n", caminho[i][0], caminho[i][1], caminho[i][2]); // Linha, Coluna, Valor da célula
        }
    }
    else{
        /* caso onde nao foi possivel encontrar um caminho valido com k passos */
        fprintf(f_out, "%d %d %d %d %d %d %d -1\n", par.L, par.C, par.task_energy, par.l1, par.c1, par.k, par.e_ini);
    }
    

     /* liberta a memoria alocada */
    for (int i = 0; i < par.L; i++) {
        free(mapa[i]);
        free(visitado[i]);
    }
    free(mapa);
    free(visitado);

    for (int i = 0; i < par.k; i++) {
        free(caminho[i]);
    }
    free(caminho);
}
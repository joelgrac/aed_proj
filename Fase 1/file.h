
typedef struct NV_Param{
    int L, C, k, l1, c1, l2, c2;
} NV_Param;

int leitura_cabecalho(NV_Param*, FILE*);
void leitura_cabecalho_3(NV_Param*, FILE*);
int verificar_cabecalho(NV_Param);
int verificar_cabecalho_3(NV_Param);
void problema_mal_definido(NV_Param, FILE*, FILE*);


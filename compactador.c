#include <stdio.h>
#include <stdlib.h>

//IMPORTANTE
char codigos_caracteres[50][50];
char tam_cod_car[50];

typedef struct heap_nodo{
    char caractere;
    int frequencia;
    struct heap_nodo *esq;
    struct heap_nodo *dir;
}heap_nodo;

typedef struct min_heap{
    int tamanho;
    int tam_total;
    struct heap_nodo **vetor;
}min_heap;

//[01]
min_heap *criar_minheap(int tam){
    min_heap *aux = (min_heap*)malloc(sizeof(min_heap));
    aux->tamanho = 0;
    aux->tam_total = tam;
    aux->vetor = (heap_nodo**)malloc(tam * sizeof(min_heap));

    return aux;
}

//[02.A]
heap_nodo *novo_nodo(char caractere, int frequencia){
    heap_nodo *aux = (heap_nodo*)malloc(sizeof(heap_nodo));
    aux->dir = NULL;
    aux->esq = NULL;
    aux->caractere = caractere;
    aux->frequencia = frequencia;

    return aux;
}

//[02]
min_heap *construir_min_heap_vetor(min_heap *heap_min, char *caractere, int *frequencia, int tam){
    int i = 0;
    for (i = 0; i < tam; i++){
        heap_min->vetor[i] = novo_nodo(caractere[i], frequencia[i]);//[02.A]
    } 
    heap_min->tamanho = tam;

    return heap_min;
}

//[03.A]
void corrige_min_heap(min_heap *heap_min, int pos){
    int menor = pos;
    int esq = 0;
    int dir = 0;
    heap_nodo *aux;

    dir = (2 * pos) + 1;
    esq = (2 * pos) + 2;

    if (esq < heap_min->tamanho && heap_min->vetor[esq]->frequencia < heap_min->vetor[menor]->frequencia){
        menor = esq;
    }
    if (dir < heap_min->tamanho && heap_min->vetor[dir]->frequencia < heap_min->vetor[menor]->frequencia){
        menor = dir;
    }
    if (menor != pos){
        aux = heap_min->vetor[menor];
        heap_min->vetor[menor] = heap_min->vetor[pos];
        heap_min->vetor[pos] = aux;

        corrige_min_heap(heap_min, menor);
    }

}

//[03]
min_heap *construir_min_heap(min_heap *heap_min){
    int i = 0;
    int aux = 0;
    aux = heap_min->tamanho - 1;
    
    for (i = ((aux - 1) / 2); i >= 0; i--){
        corrige_min_heap(heap_min, i);//[03.A]
    }
    return heap_min;
}

//[04]
int verifica_tam(min_heap *heap_min){
    if (heap_min->tamanho == 1){
        return 1;
    }else{
        return 0;
    }
}

//[05]
heap_nodo *extrair_min(min_heap *heap_min){
    heap_nodo *aux;
    
    aux = heap_min->vetor[0];
    heap_min->vetor[0] = heap_min->vetor[heap_min->tamanho-1];
    heap_min->tamanho--;
    corrige_min_heap(heap_min, 0);//[03.A]

    return aux;
}

//[06]
void insere_min_heap_nodo(min_heap *heap_min, heap_nodo *aux){
    int i = 0;
    heap_min->tamanho++;
    i = heap_min->tamanho - 1;

    while (i && aux->frequencia < heap_min->vetor[(i-1)/2]->frequencia){
        heap_min->vetor[i] = heap_min->vetor[(i-1)/2];
        i = ((i - 1) / 2);
    }
    heap_min->vetor[i] = aux;
}

//[07]
void imprime_codigo(heap_nodo *min_heap_nodo_raiz, int *vetor, int x){

    if (min_heap_nodo_raiz->esq){
        vetor[x] = 0;
        imprime_codigo(min_heap_nodo_raiz->esq, vetor, x+1);
    }
    if (min_heap_nodo_raiz->dir){
        
        vetor[x] = 1;
        imprime_codigo(min_heap_nodo_raiz->dir, vetor, x+1);
    }
    if (!(min_heap_nodo_raiz->esq) && !(min_heap_nodo_raiz->dir)){

        if (min_heap_nodo_raiz->caractere >= 'a' && min_heap_nodo_raiz->caractere <= 'z'){//letras alfabeto
            int lugar = 0;
            int i = 0;
            lugar = min_heap_nodo_raiz->caractere - 97;
            tam_cod_car[lugar] = x;
            for (i = 0; i < x; i++){
                codigos_caracteres[lugar][i] = vetor[i];
            }
        }

        else if (min_heap_nodo_raiz->caractere == 32){//espaco [-----]
            int i = 0;
            tam_cod_car[26] = x;
            for (i = 0; i < x; i++){
                codigos_caracteres[26][i] = vetor[i];
            }
        }

        else if (min_heap_nodo_raiz->caractere == 10){// \n - quebra linha
            int i = 0;
            tam_cod_car[27] = x;
            for (i = 0; i < x; i++){
                codigos_caracteres[27][i] = vetor[i];
            }
        }

        else if (min_heap_nodo_raiz->caractere == -1){// EOF
            int i = 0;
            tam_cod_car[28] = x;
            for (i = 0; i < x; i++){
                codigos_caracteres[28][i] = vetor[i];
            }
        }
    }
} 

//PONTO DE INICIO DA CONTRUCAO DA "ARVORE" DE HUFFMAN || OS NUMEROS DOS COMENTARIOS (EX: [01]) SÃO UMA FORMA DE EU ORGANIZAR O CODIGO E ESSES NUMEROS ME AJUDARAM NA HORA DA IMPLEMENTACAO
heap_nodo *arvore_huffman(char *carctere, int *frequencia, int tam){
    heap_nodo *dir = NULL;
    heap_nodo *esq = NULL;
    heap_nodo *uniao = NULL;
    min_heap *heap_min;
    
    heap_min = criar_minheap(tam); //[01]
    heap_min = construir_min_heap_vetor(heap_min, carctere, frequencia, tam);//[02]
    heap_min = construir_min_heap(heap_min);//[03]

    while (1){
        if (verifica_tam(heap_min) == 1){//[04]
            break;
        }
        esq = extrair_min(heap_min);//[05]
        dir = extrair_min(heap_min);//[05]

        uniao = novo_nodo('X', esq->frequencia + dir->frequencia);

        uniao->dir = dir;
        uniao->esq = esq;

        insere_min_heap_nodo(heap_min, uniao);//[06]
    }

    return extrair_min(heap_min);
}

//LIBERAR MEMORIA DA "ARVORE" DE HUFFMAN
void libera_memoria_HN(heap_nodo *heap_min){
    if (heap_min == NULL){
		return;
	}
	libera_memoria_HN(heap_min->esq);
	libera_memoria_HN(heap_min->dir);
	free(heap_min);
}

int main(int argc, char *argv[]){
    FILE *arq_entrada;
    FILE *arq_saida;
    heap_nodo *heap_min;
    int vetor_aux[30];
    char tab_car[30];
    int tab_freq[30];
    char eof = EOF;
    char aux_alf = 0;
    int tot_bits_txt = 0;
    char var = 0b00000000;
    char var_aux02 = 0b00000000;
    char *colecao_bits;
    int c_itr = 0;
    int i = 0;
    int j = 0;
    int aux = 0;


//ZERAR VETOR|MATRIZ

    for (i = 0; i < 30; i++){
        tab_freq[i] = 0;
    }

    for (i = 0; i < 30; i++){
        tam_cod_car[i] = 0;
    }
    
    for (i = 0; i < 40; i++){
        for (j = 0; j < 40; j++){
            codigos_caracteres[i][j] = 0;
        }
    }


//ABRIR ARQUIVOS

    //arq_entrada = fopen("faroeste.txt", "r");
    arq_entrada = fopen(argv[1], "r");
    if (arq_entrada == NULL){
        printf("O arquivo nao abriu 1\n");
        return 0;
    }
     
    //arq_saida = fopen("faroeste.hzip", "w");
    arq_saida = fopen(argv[2], "w");
    if (arq_saida == NULL){
        printf("O arquivo nao abriu 2\n");
        return 0;
    }
    
//CRIAR VETOR DOS CARACTERES
    for (i = 0; i < 29; i++){
        if (i == 26){
            tab_car[26] = ' ';
        }
        else if (i == 27){
            tab_car[27] = 10;
        }else if (i == 28){
            tab_car[28] = eof;
        }else{
            tab_car[i] = i + 97;
        }
    }

//ENCONTRAR A TABELA DE CARACTERES DE arq_entrada
    //letras = de 97 a 122 - depende da ascii // nao muda
    //EOF = -1//pode mudar
    //Espaco[space] = 32 - depende da ascii // nao muda
    //Nova linha[\n ou NL] = 10 - depende da ascii // nao muda
    while(aux_alf != EOF){
            aux_alf = fgetc(arq_entrada);
            if (aux_alf == 32){
                tab_freq[26]++;
            }
            else if (aux_alf == 10){
                tab_freq[27]++;
            }
            else if (aux_alf == eof){
                tab_freq[28]++;
            }else{
                aux = (aux_alf - 97);
                tab_freq[aux]++;
            }
    }   
    aux_alf = 0;
    fseek(arq_entrada, 0, SEEK_SET);

//IMPRIMIR A TABELA DE FREQUENCIA DOS CARACTERES

    for (i = 0; i < 29; i++){
        if (i == 27){
            printf("\\n: ");
        }
        else if (i == 28){
            printf("EOF: ");
        }else{
            printf("%c: ", tab_car[i]);
        }
        printf("%d\n", tab_freq[i]);
    }
    printf("---");
    printf("\n");

//ENVIAR tab_car E tab_freq PARA arvore_huffman

    aux = 0;

    heap_min = arvore_huffman(tab_car, tab_freq, 29);
    imprime_codigo(heap_min, vetor_aux, aux);

    for (i = 0; i < 29; i++){
        if (tab_car[i] == 32){
            printf(" : ");
        }
        else if (tab_car[i] == 10){
            printf("\\n: ");
        }
        else if (tab_car[i] == -1){
            printf("EOF: ");
        }else{
            printf("%c: ", tab_car[i]);
        }
        for (j = 0; j < tam_cod_car[i]; j++){
            printf("%d", codigos_caracteres[i][j]);
        }
        printf("\n");
    }

//CRIAR CABECALHO DO ARQUIVO .hzip

//codigos_caracteres || matriz de codigos huffman
//tam_cod_car || e o tamanho de cada codigo huffman
//tab_car || uma lista dos caracteres 

    for (i = 0; i < 29; i++){
        fwrite(&tam_cod_car[i], sizeof(char), 1, arq_saida);
        for (j = 0; j <= 32; j++){
            if (j % 8 == 0 && j != 0){
                fwrite(&var, sizeof(char), 1, arq_saida);
                var = 0b00000000;
                c_itr = 0;
                if (j == 32){
                    continue;
                }
            }
            if (codigos_caracteres[i][j] == 1){
                var = var | (1 << 7-c_itr);
            }
            else if (codigos_caracteres[i][j] == 0){
                var = var & ~(1 << 7-c_itr);
            }
            c_itr++;
        }
        //fwrite(&var_aux02, sizeof(char), 1, arq_saida); //AQUI ERRA DIFERENTE MAS EU PERCEBI QUE FAZENDO ISSO EU LIMITAVA O TAMANHO DOS CODIGOS MAS ACABEI DEIXANDO ESSAS DUAS LINHAS
        //fwrite(&var_aux02, sizeof(char), 1, arq_saida);
    }


//CONVERTER O TEXTO EM BINARIO

//tab_freq - tabela de frequencia dos caracteres
//tam_cod_car - tamanho do codigo de cada caractere 
    int ct = 0;

    for (i = 0; i < 29; i++){
        tot_bits_txt = tot_bits_txt + (tab_freq[i] * tam_cod_car[i]);
    }

    colecao_bits = (char*)malloc(tot_bits_txt * sizeof(char));
    aux_alf = 0;

    while (aux_alf != EOF){
        aux_alf = fgetc(arq_entrada);
        if (aux_alf >= 'a' && aux_alf <= 'z'){
            for (i = 0; i < tam_cod_car[aux_alf - 97]; i++){
                colecao_bits[ct] = codigos_caracteres[aux_alf-97][i];
                ct++;
            }
        }
        if (aux_alf == 32){//espaco - 26
           for (i = 0; i < tam_cod_car[26]; i++){
               colecao_bits[ct] = codigos_caracteres[26][i];
               ct++;
           }
        }
        else if (aux_alf == 10){//quebra linha - 27
            for (i = 0; i < tam_cod_car[27]; i++){
               colecao_bits[ct] = codigos_caracteres[27][i];
               ct++;
           }
        }
        else if (aux_alf == -1){//EOF - 28
            for (i = 0; i < tam_cod_car[28]; i++){
               colecao_bits[ct] = codigos_caracteres[28][i];
               ct++;
            }
        }
    }
    aux_alf = 0;
    fseek(arq_entrada, 0, SEEK_SET);

//CONVERTER colecao_bits PARA BYTES E ENVIAR PARA O ARQUIVO DE .hzip
    char vet_final = 0b0000000;
    int aux_final = 0;

    for (i = 0; i < tot_bits_txt; i++){
        if (i % 8 == 0 && i != 0){
            fwrite(&vet_final, sizeof(char), 1, arq_saida);
            vet_final = 0b00000000;
            aux_final = 0;
            if (i == tot_bits_txt){
                continue;
            }
        }
        if (colecao_bits[i] == 1){
            vet_final = vet_final | (1 << 7-aux_final);
        }
        else if (colecao_bits[i] == 0){
            vet_final = vet_final & ~(1 << 7-aux_final);
        }
        aux_final++;
    }

//LIBERA MEMORIA E FECHA O ARQUIVO
    libera_memoria_HN(heap_min);
    free(colecao_bits);
    fclose(arq_entrada);
    fclose(arq_saida);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

typedef struct nodo{
	char chave;
	struct nodo *esq;
	struct nodo *dir;
}nodo;

nodo *novo(nodo *raiz){
	nodo *aux = (nodo*)malloc(sizeof(nodo));
	aux->esq = NULL;
	aux->dir = NULL;
	aux->chave = '\0';

	return aux;
}
//FAZ A BUSCA NA ARVORE USANDO UM FLUO DE BITS PARA ACHAR OS CARACTERES CORESPONDENTES
void busca_letras(nodo *raiz, char *vetor, int tam, FILE *arq_saida){
	nodo *aux;
	aux = raiz;
	int flag = 0;
	int i = 0;
	for (i = 0; i < tam; i++){//laco que le todo o vetor de bits do arquivo .hzip(ou outro) e subtstitui pelo caractere corespondente
		if ((aux->chave >= 'a' && aux->chave <= 'z') || aux->chave == '?' || aux->chave == '@' || aux->chave == ' '){
			if (aux->chave == '?'){
				fprintf(arq_saida, "\n");
			}
			else if (aux->chave == ' '){
				fprintf(arq_saida, " ");
			}
			else if (aux->chave == '@'){
				
			}else{
				flag = fwrite(&aux->chave, 1, sizeof(aux->chave), arq_saida);
				if (flag != 1){
					printf("ERRO\n");
				}
			}
			aux = raiz;
		}
		if (vetor[i] == 0){
			aux = aux->esq;
		}
		else if (vetor[i] == 1){
			aux = aux->dir;
		}
	}
	fclose(arq_saida);
}
//INSERE NA ARVORE
nodo *insere(nodo *raiz, int *vet, int i, char letra, char fim){
	if (raiz == NULL){
		printf("raiz nula");
		return NULL;
	}
	if (i > (fim-1)){
		raiz->chave = letra;

		raiz->esq = NULL;
		raiz->dir = NULL;

		return raiz;
	}
	if (vet[i] == 0){
		if (raiz->esq == NULL){
			raiz->esq = novo(raiz->esq);

			//return raiz;//AQUI
		}
		raiz->esq = insere(raiz->esq, vet, i+1, letra, fim);
	}
	else if (vet[i] == 1){
		if (raiz->dir == NULL){
			raiz->dir = novo(raiz->dir);

			//return raiz;//AQUI
		}
		raiz->dir = insere(raiz->dir, vet, i+1, letra, fim);
	}
	return raiz;
}
//USO DE TESTE PARA CONFERIR A ARVORE DO ALFABETO
void imprime(nodo *raiz){
	if (raiz == NULL){
		return;
	}
	printf(" %c->", raiz->chave);
	imprime(raiz->dir);
	imprime(raiz->esq);
}
//LIBERA MEMORIA DA ARVORE
void libera_memoria(nodo *raiz){
	if (raiz == NULL){
		return;
	}
	libera_memoria(raiz->esq);
	libera_memoria(raiz->dir);
	free(raiz);
}

int main(int argc, char *argv[]){
	int alfabeto[40] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '?', '@'};
	char lis_cab[40];
	int lista_alf[30][33];
	char cod[5];
	char cab;
	char aux_letras = 0;
	int num_bytes = 0;
	int tam_vet_letras = 0;
	int cont_nuns = 0;
	int aux = 0;
	int cont = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	FILE *arq_entrada;
	FILE *arq_saida;
	nodo *trie_letras;
	char *cod_texto;

//ABRE ARQUIVOS

	arq_entrada = fopen(argv[1], "r");
	//arq_entrada = fopen("faroeste.hzip", "r");
	if (arq_entrada == NULL){
		printf("O arquivo nao abriu\n");
		return 0;
	}
	arq_saida = fopen(argv[2], "w");
	//arq_saida = fopen("faroeste.orig.txt", "w");
		if (arq_saida == NULL){
		printf("O arquivo nao abriu\n");
		return 0;
	}

	trie_letras = novo(trie_letras);

//ENCONTRA OS CODIGOS DE HUFFMAN DAS LETRAS
	for (j = 0; j < 29; j++){
		fread(&cab, sizeof(char), 1, arq_entrada);
		fread(&cod, sizeof(char), 4, arq_entrada);

		lis_cab[j] = cab;

		for (i = 0; i < 4; i++){
			for (k = 0; k < 8; k++){
				aux = cod[i] & 1<< 31;
				if (aux != 0){
					lista_alf[j][l] = 1;
				}else{
					lista_alf[j][l] = 0;
				}

				cod[i] = cod[i] << 1;
				l++;
			}
		}
		l = 0;
	}
	
	//IMPRIMIR OS CODIGOS DE HUFFMAN
	for (i = 0; i <= 26; i++){
		printf("%c: ", alfabeto[i]);
		for (j = 0; j < lis_cab[i]; j++){
			printf("%d", lista_alf[i][j]);
		}
		printf("\n");
	}
	printf("\\n: ");
	for (i = 0; i < lis_cab[27]; i++){
		printf("%d", lista_alf[27][i]);
	}
	printf("\n");
	printf("EOF: ");
	for (i = 0; i < lis_cab[28]; i++){
		printf("%d", lista_alf[28][i]);
	}
	printf("\n");
	
//LE O TAMANHO DE BYTES DO ARQUIVO BINARIO
	while (fread(&aux_letras, sizeof(char), 1, arq_entrada)){
		num_bytes++;
	}
	aux_letras = 0;
	aux = 0;
	fseek(arq_entrada, 145, SEEK_SET);
	//determina o num de bits
	tam_vet_letras = (num_bytes * 8);
	cod_texto = (char*)malloc(tam_vet_letras * sizeof(char));
	if (cod_texto == NULL){
		printf("erro de malloc\n");
	}

//DETERMINA A SEQUENCIA BINARIA DO TEXTO
	for (j = 0; j < num_bytes; j++){
		fread(&aux_letras, sizeof(char), 1, arq_entrada);
		for (i = 0; i < 8; i++){
			aux = aux_letras & 1<<31;
			if (aux != 0){
				cod_texto[cont_nuns] = 1;
			}else{
				cod_texto[cont_nuns] = 0;
			}
			cont_nuns++;
			aux_letras = aux_letras << 1;
		}
		aux = 0;
	}

//INSERE OS CODIGOS DE HUFFMAN NA ARVORE
	for (i = 0; i < 29; i++){
		trie_letras = insere(trie_letras, lista_alf[i], 0, alfabeto[i], lis_cab[i]);
	}

//DESCODIFICA O TEXTO USANDO A ARVORE
	busca_letras(trie_letras, cod_texto, tam_vet_letras, arq_saida);


//LIBERA MEMORIA E FECHA OS ARQUIVO
	libera_memoria(trie_letras);
	free(cod_texto);
	fclose(arq_entrada);
	//O arq_saida e fechado na funcao busca_letras
	return 0;
}


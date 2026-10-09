#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura Base
typedef struct No {
    char nome[256];
    int ehArquivo; 
    struct No *primeiroFilho;
    struct No *proximoIrmao;
} No;

//cria novo nó na memória
void liberarArvore(No *raiz);
No* criarNo(char *nome, int ehArquivo) {
    No *novo = (No*)malloc(sizeof(No));
    if (novo != NULL) {
        strncpy(novo->nome, nome, 255);
        novo->nome[255] = '\0'; //fim da string
        novo->ehArquivo = ehArquivo;
        novo->primeiroFilho = novo; 
        novo->proximoIrmao = novo;  
    }
    return novo;
}

//coloca filho no final da lista do nó pai
void inserirFilho(No *pai, No *filho) {
    if (pai == NULL || filho == NULL) return;

    if (pai->primeiroFilho == pai) {
        pai->primeiroFilho = filho;
        filho->proximoIrmao = filho; 
    } else {
        No *atual = pai->primeiroFilho;
        while (atual->proximoIrmao != pai->primeiroFilho) {
            atual = atual->proximoIrmao;
        }
        atual->proximoIrmao = filho;
        filho->proximoIrmao = pai->primeiroFilho; 
    }
}

//busca um filho direto pelo nome
No* buscarFilho(No *pai, char *nome) {
    if (pai == NULL || pai->primeiroFilho == pai) return NULL;
    No *atual = pai->primeiroFilho;
    do {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximoIrmao;
    } while (atual != pai->primeiroFilho);

    return NULL; 
}

//desconecta um filho do pai e libera memoria
void removerFilho(No *pai, No *filho) {
    if (pai == NULL || pai->primeiroFilho == pai || filho == NULL) return;
    No *atual = pai->primeiroFilho;
    No *anterior = NULL;
    int encontrou = 0;

    do {
        if (atual->proximoIrmao == filho) {
            anterior = atual;
            encontrou = 1;
            break;
        }
        atual = atual->proximoIrmao;
    } while (atual != pai->primeiroFilho);

    if (encontrou) {
        if (filho->proximoIrmao == filho) {
            pai->primeiroFilho = pai;
        } else {

            if (pai->primeiroFilho == filho) {
                pai->primeiroFilho = filho->proximoIrmao;
            }
            anterior->proximoIrmao = filho->proximoIrmao;
        }
        filho->proximoIrmao = filho; 
        liberarArvore(filho);
    }
}

//libera toda a arvore de forma recursiva
void liberarArvore(No *raiz) {
    if (raiz == NULL) return;
    if (raiz->primeiroFilho != raiz && raiz->primeiroFilho != NULL) {
        No *atual = raiz->primeiroFilho;
        No *proximo;
        do {
            proximo = atual->proximoIrmao;
            atual->proximoIrmao = atual; 
            liberarArvore(atual);
            atual = proximo;
        } while (atual != raiz->primeiroFilho);
        raiz->primeiroFilho = raiz;
    }

    free(raiz);
}


void testar_estrutura_circular() {
    printf("\n INICIO TESTE - estrutura circular \n");
    
    // criar nós
    No *raiz = criarNo("raiz", 0);
    No *arq1 = criarNo("arquivo1.txt", 1);
    No *arq2 = criarNo("arquivo2.txt", 1);
    No *arq3 = criarNo("arquivo3.txt", 1);
    
    // inserir filhos
    inserirFilho(raiz, arq1);
    inserirFilho(raiz, arq2);
    inserirFilho(raiz, arq3);
    
    // verifica encadeamento circular 
    printf("1. Primeiro filho do pai: %s\n", raiz->primeiroFilho->nome); // arq1
    printf("2. Irmao seguinte: %s\n", raiz->primeiroFilho->proximoIrmao->nome); // arq2
    printf("3. Ultimo irmao: %s\n", raiz->primeiroFilho->proximoIrmao->proximoIrmao->nome); // arq3
    printf("4. O proximo do ultimo deve ser o primeiro: %s\n", 
           raiz->primeiroFilho->proximoIrmao->proximoIrmao->proximoIrmao->nome); // arq1
           
    // buscar filho
    No *busca = buscarFilho(raiz, "arquivo2.txt");
    if (busca) printf("\n> Busca bem sucedida: Encontrado o %s\n", busca->nome);
    
    // remover o primeiro filho para testar ligação da cauda
    printf("\nRemovendo o arquivo1.txt (Primeiro elemento)...\n");
    removerFilho(raiz, arq1);
    
    // verificar novamente o encadeamento
    printf("> Novo primeiro filho: %s\n", raiz->primeiroFilho->nome); // arq2
    printf("> O proximo do ultimo (arq3) deve apontar para o novo primeiro: %s\n", 
           raiz->primeiroFilho->proximoIrmao->proximoIrmao->nome); // arq2
           
    // liberar a árvore
    liberarArvore(raiz);
    printf("\n--- FIM: Estrutura libertada sem leaks nem loops infinitos ---\n");
}
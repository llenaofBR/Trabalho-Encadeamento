#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//estrutura
typedef struct No {
    char nome[256];
    int ehArquivo;
    struct No *primeiroFilho;
    struct No *proximoIrmao;
} No;

//cria novo nó na memória
No* criarNo(char *nome, int ehArquivo) {
    No *novo = (No*)malloc(sizeof(No));
    if (novo != NULL) {
        strncpy(novo->nome, nome, 255);
        novo->nome[255] = '\0'; //fim da string
        novo->ehArquivo = ehArquivo;
        novo->primeiroFilho = NULL;
        novo->proximoIrmao = NULL;
    }
    return novo;
}

//coloca filho no final da lista do nó pai
void inserirFilho(No *pai, No *filho) {
    if (pai == NULL || filho == NULL) return;

    if (pai->primeiroFilho == NULL) {
        pai->primeiroFilho = filho;
    } else {
        No *atual = pai->primeiroFilho;
        while (atual->proximoIrmao != NULL) {
            atual = atual->proximoIrmao;
        }
        atual->proximoIrmao = filho;
    }
}

//busca um filho direto pelo nome
No* buscarFilho(No *pai, char *nome) {
    if (pai == NULL) return NULL;

    No *atual = pai->primeiroFilho;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximoIrmao;
    }
    return NULL;
}

//desconecta um filho do pai e libera memoria
void removerFilho(No *pai, No *filho) {
    if (pai == NULL || pai->primeiroFilho == NULL || filho == NULL) return;

    No *atual = pai->primeiroFilho;
    No *anterior = NULL;

    while (atual != NULL && atual != filho) {
        anterior = atual;
        atual = atual->proximoIrmao;
    }

    //se encontrou o nó correto
    if (atual == filho) {
        if (anterior == NULL) {
            pai->primeiroFilho = atual->proximoIrmao;
        } else {
            anterior->proximoIrmao = atual->proximoIrmao;
        }

        atual->proximoIrmao = NULL; 
        liberarArvore(atual);
    }
}

//libera toda a arvore de forma recursiva
void liberarArvore(No *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->primeiroFilho);
    liberarArvore(raiz->proximoIrmao);
    free(raiz);
}
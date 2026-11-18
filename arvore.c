#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

No *criarNo(const char *nome, int ehArquivo) {
    if (nome == NULL) return NULL;
    No *novo = malloc(sizeof(No));
    if (!novo) return NULL;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->ehArquivo = ehArquivo ? 1 : 0;
    novo->primeiroFilho = NULL;
    novo->proximoIrmao = NULL;
    novo->pai = NULL;
    return novo;
}

void inserirFilho(No *pai, No *filho) {
    if (!pai || !filho || pai->ehArquivo || filho == pai) return;
    filho->pai = pai;
    filho->proximoIrmao = NULL;
    if (!pai->primeiroFilho) {
        pai->primeiroFilho = filho;
        return;
    }
    No *atual = pai->primeiroFilho;
    while (atual->proximoIrmao) atual = atual->proximoIrmao;
    atual->proximoIrmao = filho;
}

No *buscarFilho(No *pai, const char *nome) {
    if (!pai || !nome || pai->ehArquivo) return NULL;
    for (No *atual = pai->primeiroFilho; atual; atual = atual->proximoIrmao)
        if (strcmp(atual->nome, nome) == 0) return atual;
    return NULL;
}

void liberarArvore(No *raiz) {
    if (!raiz) return;
    No *filho = raiz->primeiroFilho;
    while (filho) {
        No *proximo = filho->proximoIrmao;
        filho->proximoIrmao = NULL;
        liberarArvore(filho);
        filho = proximo;
    }
    free(raiz);
}

void removerFilho(No *pai, No *filho) {
    if (!pai || !filho) return;
    No *atual = pai->primeiroFilho;
    No *anterior = NULL;
    while (atual && atual != filho) {
        anterior = atual;
        atual = atual->proximoIrmao;
    }
    if (!atual) return;
    if (anterior) anterior->proximoIrmao = atual->proximoIrmao;
    else pai->primeiroFilho = atual->proximoIrmao;
    atual->pai = NULL;
    atual->proximoIrmao = NULL;
    liberarArvore(atual);
}

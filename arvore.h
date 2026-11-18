#ifndef ARVORE_H
#define ARVORE_H

#define TAM_NOME 256

typedef struct No {
    char nome[TAM_NOME];
    int ehArquivo;                 /* 0 = diretório, 1 = arquivo */
    struct No *primeiroFilho;
    struct No *proximoIrmao;
    struct No *pai;
} No;

No *criarNo(const char *nome, int ehArquivo);
void inserirFilho(No *pai, No *filho);
No *buscarFilho(No *pai, const char *nome);
void removerFilho(No *pai, No *filho);
void liberarArvore(No *raiz);

#endif

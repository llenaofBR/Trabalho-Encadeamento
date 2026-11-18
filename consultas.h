#ifndef CONSULTAS_H
#define CONSULTAS_H
#include "arvore.h"

void carregarArquivo(No *raiz, const char *arquivo);
void processarLinha(No *raiz, char *linha);
void comandoSearch(No *raiz, const char *nome);
void buscarRecursivo(No *atual, const char *nome, int *encontrou);
void comandoList(No *atual);

#endif

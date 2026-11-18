#include <stdio.h>
#include "arvore.h"
#include "consultas.h"
#include "shell.h"

int main(void) {
    No *raiz = criarNo("/", 0);
    if (!raiz) { fprintf(stderr, "Nao foi possivel criar a raiz.\n"); return 1; }
    carregarArquivo(raiz, "in.txt");
    iniciarShell(raiz);
    liberarArvore(raiz);
    return 0;
}

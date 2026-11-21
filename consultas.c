#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consultas.h"

/* Pela especificacao, arquivos sempre possuem extensao; nomes sem extensao
   representam diretorios. A barra final tambem identifica explicitamente
   um diretorio. */
static int caminhoTerminaEmArquivo(const char *caminho, int terminaBarra) {
    if (terminaBarra) return 0;
    const char *ultimaBarra = strrchr(caminho, '/');
    const char *ultimaBarraWindows = strrchr(caminho, '\\');
    if (ultimaBarraWindows && (!ultimaBarra || ultimaBarraWindows > ultimaBarra))
        ultimaBarra = ultimaBarraWindows;
    const char *nome = ultimaBarra ? ultimaBarra + 1 : caminho;
    const char *ponto = strrchr(nome, '.');
    return ponto != NULL && ponto != nome && ponto[1] != '\0';
}

static void inserirCaminho(No *raiz, char *caminho) {
    while (*caminho == '/' || *caminho == '\\') caminho++;
    size_t tam = strlen(caminho);
    if (tam == 0) return;

    int terminaBarra = caminho[tam - 1] == '/' || caminho[tam - 1] == '\\';
    char *copia = malloc(tam + 1);
    if (!copia) return;
    strcpy(copia, caminho);

    char *partes[256];
    int qtd = 0;
    for (char *p = strtok(copia, "/\\"); p && qtd < 256; p = strtok(NULL, "/\\")) {
        if (strcmp(p, ".") != 0 && strcmp(p, "..") != 0 && *p)
            partes[qtd++] = p;
    }

    if (qtd == 0) {
        free(copia);
        return;
    }

    int ultimoEhArquivo = caminhoTerminaEmArquivo(caminho, terminaBarra);
    No *atual = raiz;
    for (int i = 0; i < qtd; i++) {
        int ultimo = (i == qtd - 1);
        int ehArquivo = ultimo && ultimoEhArquivo;
        No *filho = buscarFilho(atual, partes[i]);

        if (filho) {
            /* Se o mesmo caminho foi declarado como diretorio e arquivo,
               prevalece diretorio para preservar seus possiveis descendentes. */
            if (!ehArquivo) filho->ehArquivo = 0;
        } else {
            filho = criarNo(partes[i], ehArquivo);
            if (!filho) break;
            inserirFilho(atual, filho);
        }

        if (ehArquivo) break;
        atual = filho;
        if (atual->ehArquivo) break;
    }
    free(copia);
}

void processarLinha(No *raiz, char *linha) {
    if (!raiz || !linha) return;
    linha[strcspn(linha, "\r\n")] = '\0';
    while (*linha == ' ' || *linha == '\t') linha++;
    size_t n = strlen(linha);
    while (n && (linha[n - 1] == ' ' || linha[n - 1] == '\t'))
        linha[--n] = '\0';
    if (!n || linha[0] == '#') return;
    inserirCaminho(raiz, linha);
}

void carregarArquivo(No *raiz, const char *arquivo) {
    if (!raiz || !arquivo) return;
    FILE *fp = fopen(arquivo, "r");
    if (!fp) {
        fprintf(stderr, "Erro: nao foi possivel abrir '%s'.\n", arquivo);
        return;
    }
    char linha[2048];
    while (fgets(linha, sizeof(linha), fp))
        processarLinha(raiz, linha);
    fclose(fp);
}

static void montarCaminho(No *no, char *destino, size_t capacidade) {
    No *pilha[256];
    int quantidade = 0;
    for (No *p = no; p && p->pai && quantidade < 256; p = p->pai)
        pilha[quantidade++] = p;

    if (capacidade == 0) return;
    snprintf(destino, capacidade, "/");
    for (int i = quantidade - 1; i >= 0; i--) {
        size_t usados = strlen(destino);
        size_t necessario = strlen(pilha[i]->nome) + (usados > 1 ? 1 : 0);
        if (usados + necessario + 1 > capacidade) break;
        if (usados > 1) strcat(destino, "/");
        strcat(destino, pilha[i]->nome);
    }
}

static void buscarRecursivoComCaminho(No *atual, const char *nome, int *encontrou) {
    if (!atual || !nome || !encontrou) return;

    if (atual->pai && compararNomes(atual->nome, nome) == 0) {
        char caminho[2048];
        montarCaminho(atual, caminho, sizeof(caminho));
        printf("%s %s\n", atual->ehArquivo ? "[ARQUIVO]" : "[PASTA]", caminho);
        *encontrou = 1;
    }

    for (No *filho = atual->primeiroFilho; filho; filho = filho->proximoIrmao)
        buscarRecursivoComCaminho(filho, nome, encontrou);
}

/* Mantida como funcao publica para compatibilidade com a declaracao do .h. */
void buscarRecursivo(No *atual, const char *nome, int *encontrou) {
    buscarRecursivoComCaminho(atual, nome, encontrou);
}

void comandoSearch(No *raiz, const char *nome) {
    if (!raiz || !nome || !*nome) {
        printf("Uso: search <nome>\n");
        return;
    }
    int encontrou = 0;
    buscarRecursivoComCaminho(raiz, nome, &encontrou);
    if (!encontrou)
        printf("Nenhum resultado encontrado para '%s'.\n", nome);
}

void comandoList(No *atual) {
    if (!atual) {
        printf("Diretorio invalido.\n");
        return;
    }
    if (atual->ehArquivo) {
        printf("'%s' e um arquivo, nao um diretorio.\n", atual->nome);
        return;
    }
    if (!atual->primeiroFilho) {
        printf("Diretorio vazio.\n");
        return;
    }
    for (No *filho = atual->primeiroFilho; filho; filho = filho->proximoIrmao)
        printf("%s  %s\n", filho->ehArquivo ? "[ARQ]" : "[DIR]", filho->nome);
}

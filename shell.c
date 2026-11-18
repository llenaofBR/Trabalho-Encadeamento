#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "shell.h"
#include "consultas.h"

static void caminhoAtual(No *atual, char *destino, size_t capacidade) {
    No *pilha[256];
    int n = 0;
    for (No *p = atual; p && p->pai && n < 256; p = p->pai)
        pilha[n++] = p;

    if (capacidade == 0) return;
    snprintf(destino, capacidade, "/");
    for (int i = n - 1; i >= 0; i--) {
        size_t usados = strlen(destino);
        size_t necessario = strlen(pilha[i]->nome) + (usados > 1 ? 1 : 0);
        if (usados + necessario + 1 > capacidade) break;
        if (usados > 1) strcat(destino, "/");
        strcat(destino, pilha[i]->nome);
    }
}

/* Retira espacos externos e aspas opcionais do argumento. */
static char *normalizarArgumento(char *argumento) {
    if (!argumento) return NULL;
    while (isspace((unsigned char)*argumento)) argumento++;
    size_t tamanho = strlen(argumento);
    while (tamanho && isspace((unsigned char)argumento[tamanho - 1]))
        argumento[--tamanho] = '\0';
    if (tamanho >= 2 && ((argumento[0] == '"' && argumento[tamanho - 1] == '"') ||
                         (argumento[0] == '\'' && argumento[tamanho - 1] == '\''))) {
        argumento[tamanho - 1] = '\0';
        argumento++;
    }
    return argumento;
}

No *comandoCD(No *atual, const char *nome) {
    if (!atual || !nome || !*nome) {
        printf("Uso: cd <diretorio>\n");
        return atual;
    }
    if (strcmp(nome, ".") == 0) return atual;
    if (strcmp(nome, "..") == 0) return atual->pai ? atual->pai : atual;
    if (strcmp(nome, "/") == 0) {
        while (atual->pai) atual = atual->pai;
        return atual;
    }

    No *destino = buscarFilho(atual, nome);
    if (!destino) {
        printf("Diretorio '%s' nao encontrado.\n", nome);
        int sugestao = 0;
        size_t tamanho = strlen(nome);
        for (No *filho = atual->primeiroFilho; filho; filho = filho->proximoIrmao) {
            if (!filho->ehArquivo && strncmp(filho->nome, nome, tamanho) == 0) {
                if (!sugestao) printf("Possiveis alternativas:\n");
                printf("  %s\n", filho->nome);
                sugestao = 1;
            }
        }
        if (!sugestao) printf("Nenhuma alternativa encontrada.\n");
        return atual;
    }
    if (destino->ehArquivo) {
        printf("'%s' e um arquivo, nao um diretorio.\n", nome);
        return atual;
    }
    return destino;
}

void comandoMkdir(No *atual, const char *nome) {
    if (!atual || atual->ehArquivo || !nome || !*nome) {
        printf("Uso: mkdir <nome>\n");
        return;
    }
    if (strchr(nome, '/') || strchr(nome, '\\') || strcmp(nome, ".") == 0 || strcmp(nome, "..") == 0) {
        printf("Informe somente o nome de um diretorio.\n");
        return;
    }
    if (buscarFilho(atual, nome)) {
        printf("Ja existe um item chamado '%s'.\n", nome);
        return;
    }
    No *novo = criarNo(nome, 0);
    if (!novo) {
        printf("Erro de memoria ao criar diretorio.\n");
        return;
    }
    inserirFilho(atual, novo);
    printf("Diretorio '%s' criado.\n", nome);
}

void comandoRm(No *atual, const char *nome) {
    if (!atual || !nome || !*nome) {
        printf("Uso: rm <diretorio>\n");
        return;
    }
    No *alvo = buscarFilho(atual, nome);
    if (!alvo) {
        printf("Diretorio '%s' nao encontrado neste local.\n", nome);
        return;
    }
    if (alvo->ehArquivo) {
        printf("'%s' e um arquivo. O comando rm deste trabalho remove pastas.\n", nome);
        return;
    }
    removerFilho(atual, alvo);
    printf("Diretorio '%s' e todo o seu conteudo foram removidos.\n", nome);
}

void comandoHelp(void) {
    puts("Comandos disponiveis:");
    puts("  cd <diretorio>  Entra em uma pasta; cd .. volta uma pasta e cd / vai a raiz");
    puts("  search <nome>   Busca uma pasta ou arquivo e mostra sua localizacao");
    puts("  rm <diretorio>  Remove uma pasta e todo o seu conteudo");
    puts("  list            Lista pastas e arquivos do diretorio atual");
    puts("  mkdir <nome>    Cria uma pasta no diretorio atual");
    puts("  clear           Limpa o conteudo da tela");
    puts("  help            Exibe esta lista de comandos");
    puts("  exit            Encerra o programa e libera a arvore");
}

void iniciarShell(No *raiz) {
    if (!raiz) return;
    No *atual = raiz;
    char linha[512];

    while (1) {
        char caminho[1024];
        caminhoAtual(atual, caminho, sizeof(caminho));
        printf("%s $ ", caminho);
        fflush(stdout);

        if (!fgets(linha, sizeof(linha), stdin)) break;
        linha[strcspn(linha, "\r\n")] = '\0';

        char *inicio = linha;
        while (isspace((unsigned char)*inicio)) inicio++;
        if (!*inicio) continue;

        char *fimComando = inicio;
        while (*fimComando && !isspace((unsigned char)*fimComando)) fimComando++;
        char *argumento = NULL;
        if (*fimComando) {
            *fimComando = '\0';
            argumento = normalizarArgumento(fimComando + 1);
        }
        char *comando = inicio;

        if (strcmp(comando, "exit") == 0) break;
        else if (strcmp(comando, "help") == 0) comandoHelp();
        else if (strcmp(comando, "list") == 0) comandoList(atual);
        else if (strcmp(comando, "cd") == 0) atual = comandoCD(atual, argumento);
        else if (strcmp(comando, "mkdir") == 0) comandoMkdir(atual, argumento);
        else if (strcmp(comando, "rm") == 0) comandoRm(atual, argumento);
        else if (strcmp(comando, "search") == 0) comandoSearch(raiz, argumento);
        else if (strcmp(comando, "clear") == 0) {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        } else {
            printf("Comando desconhecido: %s. Digite 'help'.\n", comando);
        }
    }
}

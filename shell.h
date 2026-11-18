#ifndef SHELL_H
#define SHELL_H
#include "arvore.h"
No *comandoCD(No *atual, const char *nome);
void comandoMkdir(No *atual, const char *nome);
void comandoRm(No *atual, const char *nome);
void comandoHelp(void);
void iniciarShell(No *raiz);
#endif

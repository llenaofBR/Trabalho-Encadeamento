# Simulador de sistema de arquivos — árvore encadeada

Implementação em C usando a representação **primeiro filho / próximo irmão**. Cada nó mantém também um ponteiro para o pai, necessário para implementar `cd ..` e exibir o caminho atual.

## Arquivos
- `arvore.h` / `arvore.c`: estrutura e operações básicas da árvore.
- `consultas.h` / `consultas.c`: importação de caminhos, `search` e `list`.
- `shell.h` / `shell.c`: interface e comandos `cd`, `mkdir`, `rm`, `help`, `clear`, `exit`, `pwd` e `ls`.
- `main.c`: inicialização, carregamento de `in.txt`, execução do shell e liberação da memória.
- `in.txt`: exemplo de caminhos para importação.

## Formato de in.txt
Um caminho por linha. Os diretórios intermediários são criados automaticamente. Por convenção, a última parte sem barra final é considerada arquivo; para declarar um diretório explicitamente, termine a linha com `/`. Linhas vazias e linhas iniciadas por `#` são ignoradas.

Exemplos:
```
Documentos/                 # diretório
Documentos/relatorio.pdf    # arquivo
Projetos/                   # diretório
Projetos/AED3/              # diretório
```

## Compilação (GCC)
```bash
gcc -Wall -Wextra -std=c11 main.c arvore.c consultas.c shell.c -o sistema
```

No Windows PowerShell:
```powershell
gcc -Wall -Wextra -std=c11 main.c arvore.c consultas.c shell.c -o sistema.exe
.\sistema.exe
```

Execute o programa na mesma pasta de `in.txt`.

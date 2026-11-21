# Documentação da Árvore de Diretórios

Trabalho 2 de Algoritmos e Estrutura de Dados III: implementação de uma árvore genérica para representar diretórios e arquivos, permitindo sua manipulação por meio de uma interface de linha de comando desenvolvida em linguagem C.

## Sumário

1. [Organização dos arquivos](#1-organização-dos-arquivos)
2. [Como compilar e executar](#2-como-compilar-e-executar)
3. [Estrutura de dados (`struct`)](#3-estrutura-de-dados-struct)
4. [Descrição das funções](#4-descrição-das-funções)
5. [Programa principal e interface de comandos](#5-programa-principal-e-interface-de-comandos)
6. [Tratamento da redundância de código](#6-tratamento-da-redundância-de-código)
7. [Desafios e dificuldades](#7-desafios-e-dificuldades)
8. [Considerações sobre a organização](#8-considerações-sobre-a-organização)

## 1. Organização dos arquivos

O projeto foi dividido em arquivos responsáveis por diferentes partes da implementação, separando a estrutura da árvore, as operações de consulta, o processamento dos comandos e a inicialização do programa.

| Arquivo | Conteúdo |
| --- | --- |
| `arvore.h` | Definição da estrutura `No` e declaração das funções de manipulação da árvore. |
| `arvore.c` | Implementação das operações de criação, inserção, busca entre filhos, remoção e liberação dos nós. |
| `consultas.h` | Declarações das funções relacionadas à leitura e às consultas da árvore. |
| `consultas.c` | Implementação da leitura do arquivo `in.txt`, busca recursiva e listagem dos componentes. |
| `shell.h` | Declarações relacionadas à interface de linha de comando. |
| `shell.c` | Processamento e execução dos comandos digitados pelo usuário. |
| `main.c` | Inicialização da árvore, carregamento dos dados, execução da interface e liberação da memória. |
| `in.txt` | Arquivo de entrada que contém os caminhos dos diretórios e arquivos utilizados na montagem da árvore. |

## 2. Como compilar e executar

Com os arquivos `.c` e `.h` e o arquivo `in.txt` na mesma pasta, a compilação pode ser realizada utilizando o GCC:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 main.c arvore.c consultas.c shell.c -o sistema
```

Para executar:

- **Linux ou macOS:** `./sistema`
- **Windows (PowerShell):** `.\sistema`

O arquivo `in.txt` deve estar disponível no diretório de execução do programa, pois é utilizado para carregar a estrutura inicial de diretórios e arquivos.

## 3. Estrutura de dados (`struct`)

O projeto utiliza uma árvore genérica para representar a hierarquia de diretórios e arquivos. Cada elemento é armazenado em um nó, que mantém referências para seus relacionamentos dentro da estrutura.

### Estrutura `No`

A estrutura `No` representa um diretório ou arquivo. Ela contém:

- `nome`: armazena o nome do diretório ou arquivo.
- `ehArquivo`: indica se o nó representa um arquivo ou um diretório.
- `primeiroFilho`: aponta para o primeiro elemento subordinado ao nó atual.
- `proximoIrmao`: aponta para o próximo elemento que possui o mesmo diretório pai.
- `pai`: aponta para o diretório que contém o nó atual.

A combinação dos ponteiros `primeiroFilho` e `proximoIrmao` permite representar árvores com quantidade variável de filhos, sem precisar definir previamente um limite de elementos por diretório.

O ponteiro `pai` permite identificar o diretório de origem de cada elemento e auxilia na navegação pela estrutura.

## 4. Descrição das funções

### Funções de `arvore.c`

#### `criarNo()`

Aloca memória para um novo nó, atribui seu nome e identifica se ele representa um arquivo ou diretório. Também inicializa os ponteiros de relacionamento com `NULL`.

#### `inserirFilho()`

Adiciona um novo nó como filho de outro nó da árvore. Caso o diretório ainda não possua filhos, o novo elemento passa a ser o primeiro filho. Caso contrário, é inserido ao final da sequência de irmãos.

Durante a inserção, o ponteiro `pai` do novo elemento é atualizado para indicar seu diretório correspondente.

#### `buscarFilho()`

Percorre os filhos diretos de um diretório para localizar um elemento pelo nome informado. Retorna o nó encontrado ou `NULL` quando não existe correspondência.

#### `removerFilho()`

Localiza e remove um filho específico da árvore. Antes da liberação, ajusta os ponteiros da sequência de irmãos e utiliza a liberação recursiva para remover também os elementos subordinados ao diretório excluído.

#### `liberarArvore()`

Percorre recursivamente os elementos da árvore, liberando os filhos e os respectivos nós. Essa função é utilizada para evitar que a memória alocada durante a execução permaneça ocupada após o encerramento ou a remoção de diretórios.

### Funções de `consultas.c`

#### `carregarArquivo()`

Realiza a leitura do arquivo `in.txt` e utiliza os caminhos informados para construir a hierarquia de diretórios e arquivos na árvore.

Cada caminho é separado em componentes, utilizando o caractere `/` como delimitador. Os elementos são inseridos respeitando a relação entre diretórios e subdiretórios.

#### Busca recursiva

Percorre a árvore para localizar um arquivo ou diretório pelo nome. Quando encontra o elemento solicitado, apresenta sua localização dentro da estrutura.

A utilização da recursividade permite pesquisar elementos em diferentes níveis da árvore, sem limitar a busca aos filhos diretos do diretório atual.

#### Listagem dos componentes

Percorre os filhos do diretório informado e apresenta os arquivos e subdiretórios existentes naquele nível da árvore.

### Funções de `shell.c`

O módulo responsável pelo terminal interpreta os comandos digitados pelo usuário e executa a operação correspondente.

Entre suas responsabilidades estão o processamento da entrada, a identificação dos comandos, a navegação entre diretórios e a execução das operações de manipulação da árvore.

## 5. Programa principal e interface de comandos

### `main()` (`main.c`)

A função `main()` coordena a inicialização e a execução do programa:

1. Cria o nó raiz da árvore, representado por `/`.
2. Carrega a estrutura inicial a partir do arquivo `in.txt`.
3. Inicia a interface de linha de comando.
4. Ao finalizar a execução, libera a memória ocupada pela árvore.

### Comandos disponíveis

A interface permite executar operações de navegação, consulta e manipulação dos diretórios e arquivos.

| Comando | Operação | Finalidade |
| --- | --- | --- |
| `cd <diretório>` | Navegação | Acessa um diretório existente. Quando não encontra correspondência exata, apresenta possíveis alternativas por prefixo, quando disponíveis. |
| `search <nome>` | Busca | Localiza um arquivo ou diretório pelo nome e informa sua localização. |
| `rm <diretório>` | Remoção | Remove um diretório e seus elementos subordinados, realizando a liberação recursiva da memória. |
| `list` | Listagem | Apresenta os arquivos e diretórios existentes no diretório atual. |
| `mkdir <nome>` | Criação | Cria um novo diretório dentro do diretório atual. |
| `clear` | Terminal | Limpa o conteúdo exibido na tela. |
| `help` | Ajuda | Apresenta os comandos disponíveis, seus modos de utilização e suas finalidades. |
| `exit` | Encerramento | Finaliza a interface e permite que a memória alocada seja liberada. |

## 6. Tratamento da redundância de código

Durante a implementação, foram utilizadas funções auxiliares para centralizar operações recorrentes e evitar a repetição de trechos de código.

- **Operações sobre a árvore.** As funções de criação, inserção, busca, remoção e liberação foram concentradas no módulo `arvore.c`. Dessa forma, os demais módulos podem reutilizar essas operações sem precisar implementar novamente a manipulação dos ponteiros.

- **Consultas e navegação.** As operações de leitura, busca recursiva e listagem foram organizadas em `consultas.c`, separando a lógica de consulta da interpretação dos comandos.

- **Processamento dos comandos.** A interface concentra a identificação e o encaminhamento dos comandos, utilizando as funções dos demais módulos para executar as operações solicitadas.

## 7. Desafios e dificuldades

- **Modularização e organização do código.** Um dos desafios durante o desenvolvimento foi estruturar o programa de forma que suas funcionalidades permanecessem organizadas e independentes. Para atender a esse objetivo, adotou-se a separação das responsabilidades em diferentes arquivos: `arvore.h` e `arvore.c` concentram a estrutura de dados e as operações de manipulação da árvore; `consultas.h` e `consultas.c` reúnem as funcionalidades de leitura, busca e listagem; `shell.h` e `shell.c` são responsáveis pelo processamento dos comandos; e `main.c` coordena a inicialização e a execução do programa. Essa organização facilita a manutenção, a identificação de erros e a evolução do código.

- **Padronização da comparação de nomes.** Durante a implementação das operações de busca e navegação, identificou-se a necessidade de evitar que diferenças entre letras maiúsculas e minúsculas interferissem na localização de arquivos e diretórios. Para solucionar essa questão, foi implementada a função `compararNomes()`, que utiliza `tolower()`, da biblioteca `ctype.h`, para realizar comparações sem distinção entre maiúsculas e minúsculas. Com isso, diferentes variações de escrita de um mesmo nome são consideradas equivalentes nas operações que utilizam essa comparação.

- **Tratamento de caracteres de espaçamento na entrada.** Outro aspecto considerado foi o tratamento dos caracteres recebidos pela interface de linha de comando. A presença de espaços em branco ou tabulações (`Tab`) no início de uma entrada poderia comprometer o reconhecimento adequado dos comandos. Para evitar esse comportamento, utilizou-se a função `isspace()`, da biblioteca `ctype.h`, que permite identificar caracteres de espaçamento e ignorá-los durante o processamento inicial da entrada. Dessa maneira, o programa apresenta maior tolerância a variações na digitação.

## 8. Considerações sobre a organização

A separação entre os módulos permite dividir as responsabilidades do programa e manter uma estrutura mais organizada.

O arquivo `arvore.h` disponibiliza a estrutura de dados e os protótipos das operações da árvore, enquanto `arvore.c` concentra a implementação dessas operações.

Os arquivos `consultas.h` e `consultas.c` organizam as funcionalidades relacionadas à leitura dos dados, busca e listagem. Já `shell.h` e `shell.c` são responsáveis pela interface de linha de comando e pelo processamento das solicitações do usuário.

Por fim, `main.c` coordena a inicialização, a execução e o encerramento do programa.

Essa organização favorece a modularização, a reutilização das funções e a manutenção do código, além de facilitar a compreensão das responsabilidades de cada parte da implementação.

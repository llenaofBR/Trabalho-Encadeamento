# Trabalho-Encadeamento
Este repositório contém a implementação da estrutura de dados central para um simulador de sistema de ficheiros. O projeto foi desenvolvido como parte de um trabalho de equipa para a disciplina de Algoritmos e Estruturas de Dados 3.

# Módulo de Árvore - Sistema de Ficheiros
A estrutura utiliza a abordagem **"Primeiro Filho, Próximo Irmão"** em C, poupando memória ao evitar a alocação de arrays dinâmicos de ponteiros para cada nó.

## Funcionalidades da Estrutura
Este módulo foca exclusivamente na gestão da árvore e isola a lógica de alocação de memória:
- `criarNo`: Alocação e inicialização de novos diretórios e ficheiros.
- `inserirFilho`: Adição de um nó ao final da lista de filhos de um nó pai.
- `buscarFilho`: Pesquisa de um filho direto específico através do nome.
- `removerFilho`: Desconexão e libertação segura de memória de um nó e das suas sub-árvores.
- `liberarArvore`: Libertação recursiva de memória (pós-ordem) de toda a estrutura para evitar *memory leaks*.

## Estrutura do Nó
A implementação base assenta numa estrutura interligada:
- Nome do ficheiro/diretório
- Identificador de tipo (ficheiro ou diretório)
- Ponteiro para o primeiro filho
- Ponteiro para o nó irmão seguinte

## Tecnologias Utilizadas
- Linguagem C
- Visual Studio Code

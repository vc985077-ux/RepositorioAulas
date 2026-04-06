/**
 * ===================================================================
 * SISTEMA DE MOCHILA - JOGO DE SOBREVIVENCIA
 * ===================================================================
 * 
 * Este programa implementa duas versoes de um sistema de mochila:
 * 
 * 1. VETOR (lista sequencial) - acesso direto, tamanho fixo
 * 2. LISTA ENCADEADA - acesso sequencial, tamanho dinamico
 * 
 * O objetivo e demonstrar como a escolha da estrutura de dados
 * influencia no desempenho do jogo, especialmente em situacoes
 * de pressao (zona de perigo).
 * 
 * Compilar: gcc -o mochila mochila.c
 * Executar: ./mochila
 * 
 * ===================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================================================================
// CONSTANTES E DEFINICOES
// ===================================================================
#define MAX_ITENS_VETOR 100
#define NOME_TAM 30
#define TIPO_TAM 20

// ===================================================================
// ESTRUTURAS DE DADOS
// ===================================================================

/**
 * Estrutura base para um item da mochila
 * Campos:
 * - nome: identificacao do item (ex: "Pocao de Vida")
 * - tipo: categoria do item (ex: "medicina", "arma", "comida")
 * - quantidade: numero de unidades do item
 */
typedef struct {
    char nome[NOME_TAM];
    char tipo[TIPO_TAM];
    int quantidade;
} Item;

/**
 * Estrutura do no para lista encadeada
 * Campos:
 * - dados: o item armazenado neste no
 * - proximo: ponteiro para o proximo elemento da lista
 */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ===================================================================
// VARIAVEIS GLOBAIS (para contagem de operacoes)
// ===================================================================
int comparacoesSequencialVetor = 0;
int comparacoesBinariaVetor = 0;
int comparacoesSequencialLista = 0;

// ===================================================================
// PROTOTIPOS DAS FUNCOES - VETOR
// ===================================================================
void inicializarVetor(Item vetor[], int* tamanho);
int inserirItemVetor(Item vetor[], int* tamanho, Item novoItem);
int removerItemVetor(Item vetor[], int* tamanho, char nome[]);
void listarItensVetor(Item vetor[], int tamanho);
int buscarSequencialVetor(Item vetor[], int tamanho, char nome[]);
void ordenarVetorPorNome(Item vetor[], int tamanho);
int buscaBinariaVetor(Item vetor[], int tamanho, char nome[]);

// ===================================================================
// PROTOTIPOS DAS FUNCOES - LISTA ENCADEADA
// ===================================================================
No* inserirItemLista(No* lista, Item novoItem);
No* removerItemLista(No* lista, char nome[]);
void listarItensLista(No* lista);
int buscarSequencialLista(No* lista, char nome[]);
void liberarLista(No* lista);
int obterTamanhoLista(No* lista);

// ===================================================================
// FUNCOES AUXILIARES
// ===================================================================
void limparBuffer();
void exibirMenuPrincipal();
void exibirSubmenu(char estrutura[]);
void pausarTela();
void compararEstruturas();

// ===================================================================
// IMPLEMENTACAO DAS FUNCOES
// ===================================================================

/**
 * Limpa o buffer do teclado para evitar problemas de leitura.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Pausa a execucao ate o usuario pressionar ENTER.
 */
void pausarTela() {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

/**
 * Inicializa o vetor da mochila (tamanho zero).
 */
void inicializarVetor(Item vetor[], int* tamanho) {
    *tamanho = 0;
}

/**
 * INSERE um item no VETOR (lista sequencial).
 * 
 * Diferenca entre estruturas:
 * - Vetor: insercao no final e O(1) se houver espaco
 * - Lista: insercao no inicio e O(1) sempre
 * 
 * @param vetor Array de itens
 * @param tamanho Ponteiro para o tamanho atual
 * @param novoItem Item a ser inserido
 * @return 1 se sucesso, 0 se falha (mochila cheia)
 */
int inserirItemVetor(Item vetor[], int* tamanho, Item novoItem) {
    if (*tamanho >= MAX_ITENS_VETOR) {
        printf("[ERRO] MOCHILA CHEIA! Nao e possivel adicionar mais itens.\n");
        return 0;
    }
    
    vetor[*tamanho] = novoItem;
    (*tamanho)++;
    
    printf("[OK] Item adicionado ao VETOR: %s (x%d) - Tipo: %s\n", 
           novoItem.nome, novoItem.quantidade, novoItem.tipo);
    return 1;
}

/**
 * REMOVE um item do VETOR pelo nome.
 * 
 * Diferenca entre estruturas:
 * - Vetor: remocao exige deslocamento de elementos O(n)
 * - Lista: remocao exige apenas ajuste de ponteiros O(1) apos encontrar
 * 
 * @param vetor Array de itens
 * @param tamanho Ponteiro para o tamanho atual
 * @param nome Nome do item a ser removido
 * @return 1 se sucesso, 0 se nao encontrado
 */
int removerItemVetor(Item vetor[], int* tamanho, char nome[]) {
    int posicao = -1;
    
    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            posicao = i;
            break;
        }
    }
    
    if (posicao == -1) {
        printf("[ERRO] Item '%s' nao encontrado na mochila (VETOR).\n", nome);
        return 0;
    }
    
    for (int i = posicao; i < *tamanho - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    (*tamanho)--;
    
    printf("[OK] Item '%s' removido do VETOR com sucesso!\n", nome);
    return 1;
}

/**
 * LISTA todos os itens do VETOR.
 */
void listarItensVetor(Item vetor[], int tamanho) {
    if (tamanho == 0) {
        printf("[MOCHILA] MOCHILA VAZIA (VETOR)!\n");
        return;
    }
    
    printf("\n+========================================================================+\n");
    printf("|                         MOCHILA - VERSAO VETOR                          |\n");
    printf("+========================================================================+\n");
    printf("| %-3s | %-20s | %-12s | %-8s |\n", "Pos", "Nome", "Tipo", "Qtd");
    printf("+------------------------------------------------------------------------+\n");
    
    for (int i = 0; i < tamanho; i++) {
        printf("| %-3d | %-20s | %-12s | %-8d |\n", 
               i, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
    printf("+========================================================================+\n");
    printf("Total de itens: %d\n", tamanho);
}

/**
 * BUSCA SEQUENCIAL no VETOR (O(n)).
 * 
 * Percorre todos os elementos um a um ate encontrar o item.
 * Conta o numero de comparacoes para analise de desempenho.
 * 
 * @return Posicao do item ou -1 se nao encontrado
 */
int buscarSequencialVetor(Item vetor[], int tamanho, char nome[]) {
    comparacoesSequencialVetor = 0;
    
    for (int i = 0; i < tamanho; i++) {
        comparacoesSequencialVetor++;
        if (strcmp(vetor[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * ORDENA o vetor por nome usando Bubble Sort.
 * 
 * A ordenacao e necessaria para habilitar a busca binaria.
 * 
 * @param vetor Array de itens
 * @param tamanho Numero de elementos no vetor
 */
void ordenarVetorPorNome(Item vetor[], int tamanho) {
    if (tamanho <= 1) return;
    
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Item temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
    printf("[INFO] Vetor ordenado por nome (ordem alfabetica)!\n");
}

/**
 * BUSCA BINARIA no VETOR (O(log n)).
 * 
 * Diferenca entre estruturas:
 * - Vetor: permite busca binaria porque tem acesso direto por indice
 * - Lista: NAO permite busca binaria porque nao tem acesso direto ao meio
 * 
 * @param vetor Array de itens (DEVE ESTAR ORDENADO)
 * @param tamanho Numero de elementos
 * @param nome Nome do item a ser buscado
 * @return Posicao do item ou -1 se nao encontrado
 */
int buscaBinariaVetor(Item vetor[], int tamanho, char nome[]) {
    comparacoesBinariaVetor = 0;
    int inicio = 0;
    int fim = tamanho - 1;
    
    while (inicio <= fim) {
        comparacoesBinariaVetor++;
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(vetor[meio].nome, nome);
        
        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}

// ===================================================================
// FUNCOES DA LISTA ENCADEADA
// ===================================================================

/**
 * INSERE um item na LISTA ENCADEADA (no inicio - O(1)).
 * 
 * Diferenca entre estruturas:
 * - Vetor: insercao no inicio exige deslocamento O(n)
 * - Lista: insercao no inicio e O(1) (apenas ajusta ponteiros)
 * 
 * @param lista Ponteiro para o inicio da lista
 * @param novoItem Item a ser inserido
 * @return Novo ponteiro para o inicio da lista
 */
No* inserirItemLista(No* lista, Item novoItem) {
    No* novo = (No*)malloc(sizeof(No));
    
    if (novo == NULL) {
        printf("[ERRO] Falha na alocacao de memoria!\n");
        return lista;
    }
    
    novo->dados = novoItem;
    novo->proximo = lista;
    
    printf("[OK] Item adicionado a LISTA ENCADEADA: %s (x%d) - Tipo: %s\n", 
           novoItem.nome, novoItem.quantidade, novoItem.tipo);
    
    return novo;
}

/**
 * REMOVE um item da LISTA ENCADEADA pelo nome.
 * 
 * Diferenca entre estruturas:
 * - Vetor: remocao exige deslocamento O(n)
 * - Lista: remocao exige apenas ajuste de ponteiros O(1) apos encontrar
 * 
 * @param lista Ponteiro para o inicio da lista
 * @param nome Nome do item a ser removido
 * @return Novo ponteiro para o inicio da lista
 */
No* removerItemLista(No* lista, char nome[]) {
    if (lista == NULL) {
        printf("[ERRO] MOCHILA VAZIA! Nada para remover (LISTA).\n");
        return NULL;
    }
    
    if (strcmp(lista->dados.nome, nome) == 0) {
        No* temp = lista;
        lista = lista->proximo;
        printf("[OK] Item '%s' removido da LISTA ENCADEADA!\n", temp->dados.nome);
        free(temp);
        return lista;
    }
    
    No* atual = lista;
    while (atual->proximo != NULL && strcmp(atual->proximo->dados.nome, nome) != 0) {
        atual = atual->proximo;
    }
    
    if (atual->proximo == NULL) {
        printf("[ERRO] Item '%s' nao encontrado na mochila (LISTA).\n", nome);
        return lista;
    }
    
    No* temp = atual->proximo;
    atual->proximo = temp->proximo;
    printf("[OK] Item '%s' removido da LISTA ENCADEADA!\n", temp->dados.nome);
    free(temp);
    
    return lista;
}

/**
 * LISTA todos os itens da LISTA ENCADEADA.
 */
void listarItensLista(No* lista) {
    if (lista == NULL) {
        printf("[MOCHILA] MOCHILA VAZIA (LISTA ENCADEADA)!\n");
        return;
    }
    
    printf("\n+========================================================================+\n");
    printf("|                    MOCHILA - VERSAO LISTA ENCADEADA                     |\n");
    printf("+========================================================================+\n");
    printf("| %-3s | %-20s | %-12s | %-8s |\n", "Pos", "Nome", "Tipo", "Qtd");
    printf("+------------------------------------------------------------------------+\n");
    
    No* atual = lista;
    int pos = 0;
    while (atual != NULL) {
        printf("| %-3d | %-20s | %-12s | %-8d |\n", 
               pos, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
        pos++;
    }
    printf("+========================================================================+\n");
    printf("Total de itens: %d\n", pos);
}

/**
 * BUSCA SEQUENCIAL na LISTA ENCADEADA (O(n)).
 * 
 * Percorre todos os nos um a um ate encontrar o item.
 * Conta o numero de comparacoes para analise de desempenho.
 * 
 * @param lista Ponteiro para o inicio da lista
 * @param nome Nome do item a ser buscado
 * @return Posicao do item ou -1 se nao encontrado
 */
int buscarSequencialLista(No* lista, char nome[]) {
    comparacoesSequencialLista = 0;
    No* atual = lista;
    int pos = 0;
    
    while (atual != NULL) {
        comparacoesSequencialLista++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return pos;
        }
        atual = atual->proximo;
        pos++;
    }
    return -1;
}

/**
 * Retorna o tamanho da lista encadeada.
 */
int obterTamanhoLista(No* lista) {
    int tamanho = 0;
    No* atual = lista;
    while (atual != NULL) {
        tamanho++;
        atual = atual->proximo;
    }
    return tamanho;
}

/**
 * Libera toda a memoria alocada pela lista encadeada.
 */
void liberarLista(No* lista) {
    No* atual = lista;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

// ===================================================================
// FUNCOES DE COMPARACAO E ANALISE
// ===================================================================

/**
 * Exibe uma comparacao detalhada entre as duas estruturas.
 */
void compararEstruturas() {
    printf("\n");
    printf("+============================================================================+\n");
    printf("|                    COMPARACAO ENTRE ESTRUTURAS                             |\n");
    printf("+============================================================================+\n");
    printf("|                                                                            |\n");
    printf("|  CARACTERISTICA           |  VETOR                    |  LISTA ENCADEADA    |\n");
    printf("|  ------------------------ | ------------------------- | ------------------- |\n");
    printf("|  Acesso por posicao       |  Rapido (O(1))            |  Lento (O(n))       |\n");
    printf("|  Insercao no inicio       |  Lento (desloca O(n))     |  Rapido (O(1))      |\n");
    printf("|  Remocao no inicio        |  Lento (desloca O(n))     |  Rapido (O(1))      |\n");
    printf("|  Insercao no final        |  Rapido (O(1))            |  Lento (O(n))       |\n");
    printf("|  Busca por nome           |  Sequencial O(n)          |  Sequencial O(n)    |\n");
    printf("|  Busca com ordenacao      |  Binaria O(log n)         |  Nao se aplica      |\n");
    printf("|  Memoria utilizada        |  Fixa (pre-alocada)       |  Dinamica (sob demanda) |\n");
    printf("|  Tamanho maximo           |  Limitado (%d)            |  Ilimitado (RAM)    |\n", MAX_ITENS_VETOR);
    printf("|  Ponteiros                |  Nao utiliza              |  Utiliza (*proximo) |\n");
    printf("|  Alocacao dinamica        |  Nao                      |  Sim (malloc/free)  |\n");
    printf("|                                                                            |\n");
    printf("+============================================================================+\n");
}

/**
 * Exibe o menu principal.
 */
void exibirMenuPrincipal() {
    printf("\n");
    printf("+============================================================================+\n");
    printf("|                     SISTEMA DE MOCHILA - SOBREVIVENCIA                      |\n");
    printf("+============================================================================+\n");
    printf("|                                                                            |\n");
    printf("|   Escolha qual estrutura deseja testar:                                    |\n");
    printf("|                                                                            |\n");
    printf("|   1 - VETOR (lista sequencial) - Acesso rapido por posicao                 |\n");
    printf("|   2 - LISTA ENCADEADA - Insercao/remocao rapida no inicio                  |\n");
    printf("|   3 - COMPARAR estruturas (analise de desempenho)                          |\n");
    printf("|   0 - Sair                                                                 |\n");
    printf("|                                                                            |\n");
    printf("+============================================================================+\n");
    printf("Escolha uma opcao: ");
}

/**
 * Exibe o submenu para uma estrutura especifica.
 */
void exibirSubmenu(char estrutura[]) {
    printf("\n+========================================================================+\n");
    printf("|                         MOCHILA - %-30s |\n", estrutura);
    printf("+========================================================================+\n");
    printf("|  1 - Inserir item                                                      |\n");
    printf("|  2 - Remover item (por nome)                                           |\n");
    printf("|  3 - Listar todos os itens                                             |\n");
    printf("|  4 - Buscar item (busca sequencial)                                    |\n");
    printf("|  5 - Ordenar itens + Busca binaria (APENAS VETOR)                      |\n");
    printf("|  0 - Voltar                                                            |\n");
    printf("+========================================================================+\n");
    printf("Escolha uma opcao: ");
}

// ===================================================================
// FUNCAO PRINCIPAL
// ===================================================================
int main() {
    Item mochilaVetor[MAX_ITENS_VETOR];
    int tamanhoVetor = 0;
    
    No* mochilaLista = NULL;
    
    int opcaoPrincipal, opcaoSubmenu;
    char nomeBusca[NOME_TAM];
    Item novoItem;
    int posicao;
    
    inicializarVetor(mochilaVetor, &tamanhoVetor);
    
    printf("\n");
    printf("+============================================================================+\n");
    printf("|                                                                            |\n");
    printf("|                     BEM-VINDO AO SISTEMA DE MOCHILA                        |\n");
    printf("|                                                                            |\n");
    printf("|  Neste sistema voce ira comparar duas estruturas de dados:                 |\n");
    printf("|  * VETOR (lista sequencial)                                                |\n");
    printf("|  * LISTA ENCADEADA (estrutura dinamica)                                    |\n");
    printf("|                                                                            |\n");
    printf("|  O objetivo e entender como a escolha da estrutura influencia              |\n");
    printf("|  o desempenho do jogo, especialmente em situacoes de pressao.             |\n");
    printf("|                                                                            |\n");
    printf("+============================================================================+\n");
    
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcaoPrincipal);
        limparBuffer();
        
        switch(opcaoPrincipal) {
            case 1:
                do {
                    exibirSubmenu("VETOR");
                    scanf("%d", &opcaoSubmenu);
                    limparBuffer();
                    
                    switch(opcaoSubmenu) {
                        case 1:
                            printf("\n--- INSERIR ITEM NA MOCHILA (VETOR) ---\n");
                            printf("Nome do item: ");
                            fgets(novoItem.nome, NOME_TAM, stdin);
                            novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;
                            
                            printf("Tipo (arma/comida/ferramenta/medicina): ");
                            fgets(novoItem.tipo, TIPO_TAM, stdin);
                            novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;
                            
                            printf("Quantidade: ");
                            scanf("%d", &novoItem.quantidade);
                            limparBuffer();
                            
                            inserirItemVetor(mochilaVetor, &tamanhoVetor, novoItem);
                            break;
                            
                        case 2:
                            if (tamanhoVetor == 0) {
                                printf("[MOCHILA] Mochila vazia! Nada para remover.\n");
                            } else {
                                printf("\n--- REMOVER ITEM (VETOR) ---\n");
                                printf("Nome do item a remover: ");
                                fgets(nomeBusca, NOME_TAM, stdin);
                                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                                removerItemVetor(mochilaVetor, &tamanhoVetor, nomeBusca);
                            }
                            break;
                            
                        case 3:
                            listarItensVetor(mochilaVetor, tamanhoVetor);
                            break;
                            
                        case 4:
                            if (tamanhoVetor == 0) {
                                printf("[MOCHILA] Mochila vazia! Nada para buscar.\n");
                            } else {
                                printf("\n--- BUSCA SEQUENCIAL (VETOR) ---\n");
                                printf("Nome do item a buscar: ");
                                fgets(nomeBusca, NOME_TAM, stdin);
                                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                                
                                posicao = buscarSequencialVetor(mochilaVetor, tamanhoVetor, nomeBusca);
                                
                                printf("\n[RESULTADO DA BUSCA]\n");
                                if (posicao != -1) {
                                    printf("  [OK] Item '%s' ENCONTRADO na posicao %d!\n", nomeBusca, posicao);
                                    printf("  [INFO] Quantidade: %d\n", mochilaVetor[posicao].quantidade);
                                    printf("  [INFO] Tipo: %s\n", mochilaVetor[posicao].tipo);
                                } else {
                                    printf("  [ERRO] Item '%s' NAO encontrado!\n", nomeBusca);
                                }
                                printf("  [INFO] Comparacoes realizadas: %d\n", comparacoesSequencialVetor);
                            }
                            break;
                            
                        case 5:
                            if (tamanhoVetor == 0) {
                                printf("[MOCHILA] Mochila vazia! Adicione itens primeiro.\n");
                            } else {
                                printf("\n--- ORDENACAO + BUSCA BINARIA (VETOR) ---\n");
                                ordenarVetorPorNome(mochilaVetor, tamanhoVetor);
                                listarItensVetor(mochilaVetor, tamanhoVetor);
                                
                                printf("\nDigite o nome do item para BUSCA BINARIA: ");
                                fgets(nomeBusca, NOME_TAM, stdin);
                                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                                
                                posicao = buscaBinariaVetor(mochilaVetor, tamanhoVetor, nomeBusca);
                                
                                printf("\n[RESULTADO DA BUSCA BINARIA]\n");
                                if (posicao != -1) {
                                    printf("  [OK] Item '%s' ENCONTRADO na posicao %d!\n", nomeBusca, posicao);
                                    printf("  [INFO] Quantidade: %d\n", mochilaVetor[posicao].quantidade);
                                    printf("  [INFO] Tipo: %s\n", mochilaVetor[posicao].tipo);
                                } else {
                                    printf("  [ERRO] Item '%s' NAO encontrado!\n", nomeBusca);
                                }
                                printf("  [INFO] Comparacoes realizadas: %d\n", comparacoesBinariaVetor);
                                
                                buscarSequencialVetor(mochilaVetor, tamanhoVetor, nomeBusca);
                                printf("  [INFO] Para comparacao, busca sequencial faria: %d comparacoes\n", 
                                       comparacoesSequencialVetor);
                            }
                            break;
                            
                        case 0:
                            printf("Voltando ao menu principal...\n");
                            break;
                            
                        default:
                            printf("[ERRO] Opcao invalida!\n");
                    }
                    
                    if (opcaoSubmenu != 0) pausarTela();
                    
                } while (opcaoSubmenu != 0);
                break;
                
            case 2:
                do {
                    exibirSubmenu("LISTA ENCADEADA");
                    scanf("%d", &opcaoSubmenu);
                    limparBuffer();
                    
                    switch(opcaoSubmenu) {
                        case 1:
                            printf("\n--- INSERIR ITEM NA MOCHILA (LISTA) ---\n");
                            printf("Nome do item: ");
                            fgets(novoItem.nome, NOME_TAM, stdin);
                            novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;
                            
                            printf("Tipo (arma/comida/ferramenta/medicina): ");
                            fgets(novoItem.tipo, TIPO_TAM, stdin);
                            novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;
                            
                            printf("Quantidade: ");
                            scanf("%d", &novoItem.quantidade);
                            limparBuffer();
                            
                            mochilaLista = inserirItemLista(mochilaLista, novoItem);
                            break;
                            
                        case 2:
                            if (mochilaLista == NULL) {
                                printf("[MOCHILA] Mochila vazia! Nada para remover.\n");
                            } else {
                                printf("\n--- REMOVER ITEM (LISTA) ---\n");
                                printf("Nome do item a remover: ");
                                fgets(nomeBusca, NOME_TAM, stdin);
                                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                                mochilaLista = removerItemLista(mochilaLista, nomeBusca);
                            }
                            break;
                            
                        case 3:
                            listarItensLista(mochilaLista);
                            break;
                            
                        case 4:
                            if (mochilaLista == NULL) {
                                printf("[MOCHILA] Mochila vazia! Nada para buscar.\n");
                            } else {
                                printf("\n--- BUSCA SEQUENCIAL (LISTA ENCADEADA) ---\n");
                                printf("Nome do item a buscar: ");
                                fgets(nomeBusca, NOME_TAM, stdin);
                                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                                
                                posicao = buscarSequencialLista(mochilaLista, nomeBusca);
                                
                                printf("\n[RESULTADO DA BUSCA]\n");
                                if (posicao != -1) {
                                    printf("  [OK] Item '%s' ENCONTRADO na posicao %d da lista!\n", nomeBusca, posicao);
                                } else {
                                    printf("  [ERRO] Item '%s' NAO encontrado!\n", nomeBusca);
                                }
                                printf("  [INFO] Comparacoes realizadas: %d\n", comparacoesSequencialLista);
                            }
                            break;
                            
                        case 5:
                            printf("[ATENCAO] Busca binaria NAO se aplica a listas encadeadas!\n");
                            printf("  Motivo: Listas encadeadas nao permitem acesso direto ao elemento do meio.\n");
                            printf("  Para busca eficiente em listas encadeadas, utilize busca sequencial.\n");
                            break;
                            
                        case 0:
                            printf("Voltando ao menu principal...\n");
                            break;
                            
                        default:
                            printf("[ERRO] Opcao invalida!\n");
                    }
                    
                    if (opcaoSubmenu != 0) pausarTela();
                    
                } while (opcaoSubmenu != 0);
                break;
                
            case 3:
                compararEstruturas();
                pausarTela();
                break;
                
            case 0:
                printf("\n--- FINALIZANDO O SISTEMA ---\n");
                liberarLista(mochilaLista);
                printf("[OK] Memoria liberada com sucesso!\n");
                printf("[OK] Sistema encerrado.\n");
                break;
                
            default:
                printf("[ERRO] Opcao invalida! Digite 1, 2, 3 ou 0.\n");
        }
        
    } while (opcaoPrincipal != 0);
    
    return 0;
}

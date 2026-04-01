#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura do território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Protótipos das funções (declaração antes do main)
void limparBuffer();
void cadastrar(struct Territorio *t, int posicao);
void listar(struct Territorio *t, int total);
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void liberar(struct Territorio *t);

int main() {
    struct Territorio *mapa;  // Ponteiro para o vetor de territórios
    int quantidade;           // Quantos territórios o usuário quer
    int total = 0;            // Quantos já foram cadastrados
    int opcao;                // Opção do menu
    
    srand(time(NULL));  // Inicializa números aleatórios
    
    printf("=== JOGO WAR ===\n\n");
    
    // Pergunta quantos territórios o jogador quer
    printf("Quantos territórios você quer no jogo? ");
    scanf("%d", &quantidade);
    limparBuffer();
    
    // ALOCAÇÃO DINÂMICA: cria o vetor de territórios
    mapa = (struct Territorio*) malloc(quantidade * sizeof(struct Territorio));
    
    // Verifica se a memória foi alocada corretamente
    if (mapa == NULL) {
        printf("Erro! Não foi possível alocar memória.\n");
        return 1;
    }
    
    do {
        // Menu principal
        printf("\n=== MENU ===\n");
        printf("1 - Cadastrar território\n");
        printf("2 - Listar territórios\n");
        printf("3 - Atacar\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        switch(opcao) {
            case 1:  // CADASTRAR
                if (total < quantidade) {
                    cadastrar(mapa, total);  // Passa o ponteiro e a posição
                    total++;  // Aumenta o contador
                } else {
                    printf("Limite de territórios atingido!\n");
                }
                break;
                
            case 2:  // LISTAR
                listar(mapa, total);
                break;
                
            case 3:  // ATACAR
                if (total < 2) {
                    printf("Precisa de pelo menos 2 territórios para atacar!\n");
                } else {
                    int a, d;
                    
                    // Mostra os territórios disponíveis
                    listar(mapa, total);
                    
                    printf("\nEscolha o ATACANTE (1 a %d): ", total);
                    scanf("%d", &a);
                    printf("Escolha o DEFENSOR (1 a %d): ", total);
                    scanf("%d", &d);
                    limparBuffer();
                    
                    // Converte para índice do vetor (começa do 0)
                    a--;
                    d--;
                    
                    // Verifica se os números são válidos
                    if (a >= 0 && a < total && d >= 0 && d < total && a != d) {
                        // CHAMADA DA FUNÇÃO DE ATAQUE USANDO PONTEIROS
                        // Passamos o endereço dos territórios na memória
                        atacar(&mapa[a], &mapa[d]);
                    } else {
                        printf("Opção inválida!\n");
                    }
                }
                break;
                
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 0);
    
    // LIBERA A MEMÓRIA ALOCADA
    liberar(mapa);
    
    return 0;
}

// ============================================
// FUNÇÃO PARA LIMPAR O BUFFER DO TECLADO
// ============================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================
// FUNÇÃO PARA CADASTRAR UM TERRITÓRIO
// Recebe um PONTEIRO para a struct e a posição
// ============================================
void cadastrar(struct Territorio *t, int posicao) {
    printf("\n--- Cadastro de Território ---\n");
    
    printf("Nome: ");
    fgets(t[posicao].nome, 30, stdin);
    t[posicao].nome[strcspn(t[posicao].nome, "\n")] = '\0';  // Tira o Enter
    
    printf("Cor do exército: ");
    fgets(t[posicao].cor, 10, stdin);
    t[posicao].cor[strcspn(t[posicao].cor, "\n")] = '\0';
    
    printf("Número de tropas: ");
    scanf("%d", &t[posicao].tropas);
    limparBuffer();
    
    printf("Território cadastrado com sucesso!\n");
}

// ============================================
// FUNÇÃO PARA LISTAR TERRITÓRIOS
// Recebe um PONTEIRO e o total de territórios
// ============================================
void listar(struct Territorio *t, int total) {
    printf("\n--- Territórios Cadastrados ---\n");
    
    if (total == 0) {
        printf("Nenhum território cadastrado.\n");
        return;
    }
    
    for (int i = 0; i < total; i++) {
        printf("%d. %s (%s) - %d tropas\n", 
               i + 1,           // Número mostrado para o usuário
               t[i].nome,       // Acessando o campo nome
               t[i].cor,        // Acessando o campo cor
               t[i].tropas);    // Acessando o campo tropas
    }
}

// ============================================
// FUNÇÃO DE ATAQUE - A MAIS IMPORTANTE
// Recebe dois PONTEIROS para os territórios
// void = não retorna nada, mas modifica os dados
// ============================================
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    int dado_atacante, dado_defensor;
    
    printf("\n=== COMBATE ===\n");
    printf("%s (%s) vs %s (%s)\n", 
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);
    
    // Verificações antes do ataque
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("ERRO: Não pode atacar território aliado!\n");
        return;
    }
    
    if (atacante->tropas <= 1) {
        printf("ERRO: Atacante precisa de pelo menos 2 tropas!\n");
        return;
    }
    
    // ROLA OS DADOS (números aleatórios de 1 a 6)
    dado_atacante = rand() % 6 + 1;
    dado_defensor = rand() % 6 + 1;
    
    printf("Dado do atacante: %d\n", dado_atacante);
    printf("Dado do defensor: %d\n", dado_defensor);
    
    // LÓGICA DO ATAQUE
    if (dado_atacante > dado_defensor) {
        // ATACANTE VENCE - CONQUISTA O TERRITÓRIO
        printf("\nATACANTE VENCEU! Conquistou o território!\n");
        
        // Usando ponteiro -> para modificar os dados
        strcpy(defensor->cor, atacante->cor);  // Muda a cor do defensor
        defensor->tropas = atacante->tropas / 2;  // Metade das tropas do atacante
        
        printf("✓ %s agora pertence ao exército %s\n", defensor->nome, defensor->cor);
        printf("✓ %s recebeu %d tropas\n", defensor->nome, defensor->tropas);
        
    } else if (dado_atacante < dado_defensor) {
        // DEFENSOR VENCE - ATACANTE PERDE UMA TROPA
        printf("\nDEFENSOR VENCEU! Ataque repelido!\n");
        
        atacante->tropas--;  // Perde uma tropa (usa -> por ser ponteiro)
        
        printf("✗ %s perdeu 1 tropa. Agora tem %d tropas\n", 
               atacante->nome, atacante->tropas);
               
    } else {
        // EMPATE - AMBOS PERDEM UMA TROPA
        printf("\nEMPATE! Ambos perdem 1 tropa\n");
        
        atacante->tropas--;
        defensor->tropas--;
        
        printf("✗ %s agora tem %d tropas\n", atacante->nome, atacante->tropas);
        printf("✗ %s agora tem %d tropas\n", defensor->nome, defensor->tropas);
    }
    
    // Mostra a situação atualizada
    printf("\n--- Situação atual ---\n");
    printf("%s: %d tropas (%s)\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("%s: %d tropas (%s)\n", defensor->nome, defensor->tropas, defensor->cor);
}

// ============================================
// FUNÇÃO PARA LIBERAR A MEMÓRIA
// Recebe o PONTEIRO e libera o espaço alocado
// ============================================
void liberar(struct Territorio *t) {
    free(t);  // Libera a memória que foi alocada com malloc
}

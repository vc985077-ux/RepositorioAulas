#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================
// ESTRUTURA DO TERRITÓRIO
// ============================================
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// ============================================
// PROTÓTIPOS DAS FUNÇÕES
// ============================================
void limparBuffer();
void cadastrarTerritorio(struct Territorio *mapa, int posicao);
void listarTerritorios(struct Territorio *mapa, int total);
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
void exibirMissao(char *missao);
int verificarMissao(char *missao, struct Territorio *mapa, int totalTerritorios, char *corJogador);
void liberarMemoria(struct Territorio *mapa, char *missao);

// ============================================
// FUNÇÃO PARA LIMPAR O BUFFER
// ============================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ============================================
// FUNÇÃO PARA CADASTRAR TERRITÓRIO
// ============================================
void cadastrarTerritorio(struct Territorio *mapa, int posicao) {
    printf("\n--- Cadastro de Território ---\n");
    
    printf("Nome: ");
    fgets(mapa[posicao].nome, 30, stdin);
    mapa[posicao].nome[strcspn(mapa[posicao].nome, "\n")] = '\0';
    
    printf("Cor do exército (Vermelho/Azul/Verde/Amarelo): ");
    fgets(mapa[posicao].cor, 10, stdin);
    mapa[posicao].cor[strcspn(mapa[posicao].cor, "\n")] = '\0';
    
    printf("Número de tropas: ");
    scanf("%d", &mapa[posicao].tropas);
    limparBuffer();
    
    printf("✓ Território cadastrado!\n");
}

// ============================================
// FUNÇÃO PARA LISTAR TERRITÓRIOS
// ============================================
void listarTerritorios(struct Territorio *mapa, int total) {
    printf("\n--- Territórios ---\n");
    if (total == 0) {
        printf("Nenhum território cadastrado.\n");
        return;
    }
    
    for (int i = 0; i < total; i++) {
        printf("%d. %s (%s) - %d tropas\n", 
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ============================================
// FUNÇÃO DE ATAQUE
// ============================================
void atacar(struct Territorio *atacante, struct Territorio *defensor) {
    int dado_atacante, dado_defensor;
    
    printf("\n=== COMBATE ===\n");
    printf("%s (%s) vs %s (%s)\n", 
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);
    
    // Validações
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("❌ Não pode atacar território aliado!\n");
        return;
    }
    
    if (atacante->tropas <= 1) {
        printf("❌ Precisa de pelo menos 2 tropas para atacar!\n");
        return;
    }
    
    // Simula os dados
    dado_atacante = rand() % 6 + 1;
    dado_defensor = rand() % 6 + 1;
    
    printf("🎲 Atacante rolou: %d\n", dado_atacante);
    printf("🛡️ Defensor rolou: %d\n", dado_defensor);
    
    // Lógica do combate
    if (dado_atacante > dado_defensor) {
        printf("\n✨ ATACANTE VENCEU! Conquistou o território!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("✓ %s agora é %s com %d tropas\n", 
               defensor->nome, defensor->cor, defensor->tropas);
    } 
    else if (dado_atacante < dado_defensor) {
        printf("\n💔 DEFENSOR VENCEU! Ataque repelido!\n");
        atacante->tropas--;
        printf("✓ %s perdeu 1 tropa. Agora tem %d\n", 
               atacante->nome, atacante->tropas);
    } 
    else {
        printf("\n🤝 EMPATE! Ambos perdem 1 tropa\n");
        atacante->tropas--;
        defensor->tropas--;
        printf("✓ %s: %d tropas | %s: %d tropas\n", 
               atacante->nome, atacante->tropas,
               defensor->nome, defensor->tropas);
    }
}

// ============================================
// FUNÇÃO PARA ATRIBUIR MISSÃO (SORTEIO)
// ============================================
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, missoes[indiceSorteado]);
}

// ============================================
// FUNÇÃO PARA EXIBIR MISSÃO (PASSAGEM POR VALOR)
// ============================================
void exibirMissao(char *missao) {
    printf("\n╔════════════════════════════════╗\n");
    printf("║     🎯 SUA MISSÃO ESTRATÉGICA   ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║ %s\n", missao);
    printf("╚════════════════════════════════╝\n");
}

// ============================================
// FUNÇÃO PARA VERIFICAR SE A MISSÃO FOI CUMPRIDA
// ============================================
int verificarMissao(char *missao, struct Territorio *mapa, 
                    int totalTerritorios, char *corJogador) {
    
    // MISSÃO 1: Conquistar 3 territórios seguidos
    if (strstr(missao, "Conquistar 3 territórios seguidos") != NULL) {
        int consecutivos = 0;
        for (int i = 0; i < totalTerritorios - 1; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && 
                strcmp(mapa[i + 1].cor, corJogador) == 0) {
                consecutivos++;
                if (consecutivos >= 2) return 1; // 2 pares = 3 consecutivos
            } else {
                consecutivos = 0;
            }
        }
        return 0;
    }
    
    // MISSÃO 2: Eliminar todas as tropas da cor vermelha
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0) {
                return 0; // Ainda tem tropas vermelhas
            }
        }
        return 1; // Todas as tropas vermelhas foram eliminadas
    }
    
    // MISSÃO 3: Conquistar 5 territórios
    if (strstr(missao, "Conquistar 5 territórios") != NULL) {
        int count = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                count++;
            }
        }
        return count >= 5;
    }
    
    // MISSÃO 4: Acumular 20 tropas em um único território
    if (strstr(missao, "Acumular 20 tropas") != NULL) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 20) {
                return 1;
            }
        }
        return 0;
    }
    
    // MISSÃO 5: Controlar todos os territórios de uma mesma cor
    if (strstr(missao, "Controlar todos os territórios") != NULL) {
        // Verifica se existe alguma cor que o jogador controla completamente
        char cores[][10] = {"Vermelho", "Azul", "Verde", "Amarelo"};
        
        for (int c = 0; c < 4; c++) {
            int totalCor = 0;
            int controlados = 0;
            
            for (int i = 0; i < totalTerritorios; i++) {
                if (strcmp(mapa[i].cor, cores[c]) == 0) {
                    totalCor++;
                    if (strcmp(mapa[i].cor, corJogador) == 0) {
                        controlados++;
                    }
                }
            }
            
            if (totalCor > 0 && controlados == totalCor) {
                return 1;
            }
        }
        return 0;
    }
    
    return 0; // Missão não identificada ou não cumprida
}

// ============================================
// FUNÇÃO PARA LIBERAR MEMÓRIA
// ============================================
void liberarMemoria(struct Territorio *mapa, char *missao) {
    free(mapa);
    free(missao);
    printf("\n✓ Memória liberada com sucesso!\n");
}

// ============================================
// FUNÇÃO PRINCIPAL
// ============================================
int main() {
    // ========== VARIÁVEIS ==========
    struct Territorio *mapa = NULL;
    char *missaoJogador = NULL;  // Ponteiro para armazenar a missão
    int quantidadeTerritorios = 0;
    int totalCadastrados = 0;
    int opcao;
    char corJogador[10];
    int jogoAtivo = 1;
    int missaoExibida = 0;  // Controla se a missão já foi mostrada
    
    // ========== VETOR DE MISSÕES (strings pré-definidas) ==========
    char *missoes[] = {
        "Conquistar 3 territórios seguidos (em linha reta)",
        "Eliminar todas as tropas da cor vermelha",
        "Conquistar 5 territórios",
        "Acumular 20 tropas em um único território",
        "Controlar todos os territórios de uma mesma cor"
    };
    
    int totalMissoes = 5;
    
    // Inicializa gerador de números aleatórios
    srand(time(NULL));
    
    // ========== APRESENTAÇÃO ==========
    printf("╔══════════════════════════════════════════╗\n");
    printf("║     🎲 WAR - EDIÇÃO MISSÕES ESTRATÉGICAS 🎲 ║\n");
    printf("║     Desafio final - Conquiste o mundo!    ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");
    
    // ========== CONFIGURAÇÃO INICIAL ==========
    printf("Qual a cor do seu exército? ");
    fgets(corJogador, 10, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';
    
    printf("Quantos territórios terão no jogo? ");
    scanf("%d", &quantidadeTerritorios);
    limparBuffer();
    
    // ========== ALOCAÇÃO DINÂMICA DA MISSÃO ==========
    // malloc para armazenar a string da missão
    missaoJogador = (char*) malloc(200 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("❌ Erro ao alocar memória para a missão!\n");
        return 1;
    }
    
    // ========== SORTEIO DA MISSÃO ==========
    atribuirMissao(missaoJogador, missoes, totalMissoes);
    
    // ========== ALOCAÇÃO DINÂMICA DOS TERRITÓRIOS ==========
    mapa = (struct Territorio*) malloc(quantidadeTerritorios * sizeof(struct Territorio));
    if (mapa == NULL) {
        printf("❌ Erro ao alocar memória para os territórios!\n");
        free(missaoJogador);
        return 1;
    }
    
    printf("\n✓ Jogo configurado com sucesso!\n");
    printf("✓ Você joga com o exército %s\n", corJogador);
    
    // ========== LOOP PRINCIPAL DO JOGO ==========
    do {
        printf("\n╔════════════════════════════════╗\n");
        printf("║           MENU PRINCIPAL        ║\n");
        printf("╠════════════════════════════════╣\n");
        printf("║  1 - Cadastrar território       ║\n");
        printf("║  2 - Listar territórios         ║\n");
        printf("║  3 - Atacar                     ║\n");
        printf("║  4 - Verificar minha missão     ║\n");
        printf("║  0 - Sair                       ║\n");
        printf("╚════════════════════════════════╝\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer();
        
        // Exibe a missão apenas uma vez (no primeiro turno)
        if (!missaoExibida && opcao != 0) {
            exibirMissao(missaoJogador);
            missaoExibida = 1;
        }
        
        switch(opcao) {
            case 1:  // CADASTRAR
                if (totalCadastrados < quantidadeTerritorios) {
                    cadastrarTerritorio(mapa, totalCadastrados);
                    totalCadastrados++;
                } else {
                    printf("❌ Limite máximo de territórios atingido!\n");
                }
                break;
                
            case 2:  // LISTAR
                listarTerritorios(mapa, totalCadastrados);
                break;
                
            case 3:  // ATACAR
                if (totalCadastrados < 2) {
                    printf("❌ Precisa de pelo menos 2 territórios!\n");
                    break;
                }
                
                listarTerritorios(mapa, totalCadastrados);
                
                int atacante, defensor;
                printf("\nEscolha o ATACANTE (1 a %d): ", totalCadastrados);
                scanf("%d", &atacante);
                printf("Escolha o DEFENSOR (1 a %d): ", totalCadastrados);
                scanf("%d", &defensor);
                limparBuffer();
                
                atacante--;
                defensor--;
                
                if (atacante >= 0 && atacante < totalCadastrados && 
                    defensor >= 0 && defensor < totalCadastrados && 
                    atacante != defensor) {
                    
                    // Verifica se o atacante é do jogador ou de outro exército
                    if (strcmp(mapa[atacante].cor, corJogador) == 0) {
                        // Atacante é o jogador
                        atacar(&mapa[atacante], &mapa[defensor]);
                    } else {
                        printf("⚠️ Você só pode atacar com seus próprios territórios!\n");
                        printf("   Seus territórios são da cor: %s\n", corJogador);
                    }
                } else {
                    printf("❌ Opção inválida!\n");
                }
                break;
                
            case 4:  // VERIFICAR MISSÃO
                exibirMissao(missaoJogador);
                
                // Verifica se a missão foi cumprida
                if (verificarMissao(missaoJogador, mapa, totalCadastrados, corJogador)) {
                    printf("\n🎉🎉🎉 PARABÉNS! 🎉🎉🎉\n");
                    printf("╔════════════════════════════════════════╗\n");
                    printf("║     VOCÊ CUMPRIU SUA MISSÃO E VENCEU!   ║\n");
                    printf("║     🌟 WAR - VOCÊ É O GRANDE ESTRATEGISTA 🌟 ║\n");
                    printf("╚════════════════════════════════════════╝\n");
                    jogoAtivo = 0;  // Encerra o jogo
                } else {
                    printf("\n📋 Missão ainda NÃO cumprida. Continue lutando!\n");
                }
                break;
                
            case 0:  // SAIR
                printf("\nEncerrando o jogo...\n");
                break;
                
            default:
                printf("❌ Opção inválida!\n");
        }
        
        // Verificação automática ao final de cada turno (requisito)
        if (jogoAtivo && totalCadastrados >= 2) {
            if (verificarMissao(missaoJogador, mapa, totalCadastrados, corJogador)) {
                printf("\n🎉🎉🎉 VITÓRIA POR MISSÃO! 🎉🎉🎉\n");
                printf("Ao final do turno, sua missão foi cumprida!\n");
                printf("Parabéns, você venceu o jogo!\n");
                jogoAtivo = 0;
            }
        }
        
        if (opcao != 0 && jogoAtivo) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 0 && jogoAtivo);
    
    // ========== LIBERAÇÃO DA MEMÓRIA ==========
    liberarMemoria(mapa, missaoJogador);
    
    printf("\nObrigado por jogar WAR!\n");
    return 0;
}

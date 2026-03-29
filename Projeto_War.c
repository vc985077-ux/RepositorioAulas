#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes Globais
#define MAX_territ 5

//Definição STRUCT
struct Territorio{
    char nome[30];
    char cor[10];
    int tropas;
};

// Função para limpar buffer de entrada
void limparBufferEntrada(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
};

//Principal (main)
int main() {
    struct Territorio mapa[MAX_territ];
    int totalTerritorios = 0;
    int opcao;

    printf("======================\n");
    printf(" ---- PROJETO WAR ---- \n");
    printf("======================\n");
    printf("Bem-vindo ao jogo War!\n\n");
    printf("Vamos começar cadastrando os territórios do jogo.\n\n");

    //Repetição principal do Menu
    do {
        //Menu de opções
        printf("======================\n");
        printf(" --- MENU PRINCIPAL --- \n");
        printf("======================\n");
        printf("1 - Cadastrar novo território\n");
        printf("2 - Listar todos os territórios\n");
        printf("0 - Sair\n");
        printf("------------------------\n");
        printf("Escolha uma opção: ");
        
        //Lê a opção do usuário.
        scanf("%d", &opcao);
        limparBufferEntrada(); //LLimpa o '\n' deixado pelo scanf.

        //Processamento das opções
        switch (opcao){
        case 1: //cadastro território
            printf("--- Cadastro de Novo Território ---\n\n");

            if (totalTerritorios < MAX_territ){
                printf("Digite o nome do território: ");
                fgets(mapa[totalTerritorios].nome, 30, stdin);

                printf("Digite a cor do território: ");
                fgets(mapa[totalTerritorios].cor, 10, stdin);

                mapa[totalTerritorios].nome[strcspn(mapa[totalTerritorios].nome,"\n")] = '\0';
                mapa[totalTerritorios].cor[strcspn(mapa[totalTerritorios].cor,"\n")] = '\0';

                printf("Digite o número de tropas: ");
                scanf("%d", &mapa[totalTerritorios].tropas);
                limparBufferEntrada(); //Limpa o '\n' deixado pelo scanf.
                
                totalTerritorios++;
                printf("\nTerritório cadastrado com sucesso!\n\n");
            } else {
                printf("Limite de territórios atingido. Não é possível cadastrar.\n\n");
            } 
            
            printf("\nPressione Enter para continuar...");
            getchar(); //Pausa para o usuário ler a mensagem antes de voltar ao menu
            
            break;

        case 2: //listar territórios
            printf("\n--- Lista de Territórios Cadastrados ---\n\n");
            for (int i = 0; i < totalTerritorios; i++){
                printf("Território %d:\n", i + 1);
                printf("Nome: %s\n", mapa[i].nome);
                printf("Cor: %s\n", mapa[i].cor);
                printf("Tropas: %d\n", mapa[i].tropas);
                printf("------------------------\n");
            }
            if (totalTerritorios == 0){
                printf("\nNenhum território cadastrado ainda.\n\n");
            }

            printf("\nPressione Enter para continuar...");
            getchar(); //Pausa para o usuário ler a mensagem antes de voltar ao menu

            break;

        case 0:
            printf("Saindo do jogo... Obrigado por jogar!\n");
            break;
        default:
            printf("Opção inválida. Por favor, escolha uma opção válida.\n\n");
            printf("\nPressione Enter para continuar...");
            getchar(); //Pausa para o usuário ler a mensagem antes de voltar ao menu
        } 
    } while (opcao != 0);
};

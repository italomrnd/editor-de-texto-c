#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Linha{
    char conteudo[256];
    struct Linha* proxima;
}Linha;

char memoria[1001] = "";

void criar_novo_arquivo();
void abrir_arquivo();
void salvar_arquivo();
void sair();
void limpar_terminal();

void menu_principal(){
    int escolha;
    do{
        limpar_terminal();
        printf("Editor de texto\n");
        printf("=============== \n");
        printf("1 - Criar um novo arquivo\n");
        printf("2 - Abrir arquivo existente\n");
        printf("3 - Salvar arquivo\n");
        printf("4 - Sair do editor de texto\n");
        printf("Escolha a função a ser utilizada: \n");
        scanf("%d", &escolha);
        
        getchar();

        switch(escolha){
            case 1:
                criar_novo_arquivo();
                break;
            case 2:     
                abrir_arquivo();
                break;
            case 3:
                salvar_arquivo();
                break;
            case 4:
                sair();
                break;
            default:
                printf("Escolha inválida. Por favor, tente novamente\n");
                printf("Pressione Enter para continuar\n");
                getchar(); // espera a entrada do usuário e pausa o programa
        }
    } while (escolha != 4);
}

void criar_novo_arquivo(){
    printf("Digite o texto(máx. 1000 caracteres): \n");
    fgets(memoria, sizeof(memoria), stdin);
    printf("\nTexto guardado com sucesso\n");
    printf("Pressione Enter para retornar ao menu");
    getchar();
}   

void abrir_arquivo(){
    char nome_do_arquivo[101];
    FILE *arquivo;

    printf("Entre o nome do arquivo a ser aberto: \n");
    scanf("%s", nome_do_arquivo);
    getchar();

    arquivo = fopen(nome_do_arquivo, "r");
    
    if (arquivo == NULL){
        printf("O arquivo não existe\n");
        return;
    }

    char caractere;
    printf("Conteúdo do arquivo: \n");
    printf("--------------------------\n");
    while ((caractere = fgetc(arquivo)) != EOF){ // enquanto o caractere que leio não é EOF...
        putchar(caractere);
    }

    fclose(arquivo);

    printf("\n[fim do arquivo] Pressione Enter para continuar");
    getchar();
}

void salvar_arquivo(){
    char nome_do_arquivo[101];
    FILE *arquivo;

    if (strlen(memoria) == 0){
        printf("Não há nada para salvar. Crie um arquivo válido antes\n");
        return;
    }

    printf("Digite o nome do arquivo a ser salvo(ex. texto.txt): \n");
    scanf("%s", nome_do_arquivo);

    arquivo = fopen(nome_do_arquivo, "w");
    if (arquivo == NULL){
        printf("O arquivo não é válido\n");
        return;
    }
    fprintf(arquivo, "%s", memoria);
    getchar();
    fclose(arquivo);
    printf("Arquivo %s salvo com sucesso\n", nome_do_arquivo);
    printf("Pressione Enter para continuar\n");
    getchar();
}

void sair(){
    printf("Até a próxima!\n");
    exit(0);
}

void limpar_terminal(){
    #ifdef _WIN32   
        system("cls");  
    #else   
        system("clear");
    #endif
}

int main(){
    menu_principal();
}
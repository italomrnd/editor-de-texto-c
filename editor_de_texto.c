#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Linha{
    char conteudo[256];
    struct Linha* proxima;
}Linha;

Linha* inicio_do_texto = NULL;
Linha* fim_do_texto = NULL;

void criar_novo_arquivo();
void abrir_arquivo();
void salvar_arquivo();
void sair();
void limpar_terminal();
void mostrar_texto();

void menu_principal(){
    int escolha;
    do{
        limpar_terminal();
        printf("Editor de texto\n");
        printf("=============== \n");
        printf("1 - Criar um novo arquivo\n");
        printf("2 - Abrir arquivo existente\n");
        printf("3 - Salvar arquivo\n");
        printf("4 - Mostrar texto na tela\n");
        printf("5 - Sair do editor de texto\n");
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
                mostrar_texto();
                break;
            case 5:
                sair();
                break;
            default:
                printf("Escolha inválida. Por favor, tente novamente\n");
                printf("Pressione Enter para continuar\n");
                getchar(); 
        }
    } while (escolha != 5);
}

void criar_novo_arquivo(){
    Linha* nova_linha = malloc(sizeof(Linha)); 
    
    if(nova_linha == NULL){
        printf("Não há memória disponível no sistema\n");
        return;
    }
    nova_linha->proxima = NULL;

    printf("Digite o texto da nova linha (máx. 256 caracteres): \n");
    fgets(nova_linha->conteudo, sizeof(nova_linha->conteudo), stdin);
    
    if (inicio_do_texto == NULL){
        inicio_do_texto = nova_linha;
        fim_do_texto = nova_linha;
    }
    else{
        fim_do_texto->proxima = nova_linha;
        fim_do_texto = nova_linha;
    }
    printf("Linha salva com sucesso\n");
    printf("Pressione Enter para retornar ao menu\n");
    getchar();
}   

void mostrar_texto(){
    Linha* atual = inicio_do_texto;

    if (atual == NULL){
        printf("O editor está vazio\n");
    }
    else{
        printf("Seu texto atual: \n");
        int contador = 1;
        while (atual != NULL){
            printf("%d: %s", contador, atual->conteudo);
            atual = atual->proxima;
            contador++;
        }
        printf("-----------------\n");
    }
    printf("Pressione Enter para continuar\n");
    getchar();
}

void abrir_arquivo(){
    char nome_do_arquivo[101];
    char conteudo_linha[256];
    
    printf("Digite o nome do arquivo a ser aberto: \n");
    scanf("%s", nome_do_arquivo);
    getchar();

    FILE* arquivo = fopen(nome_do_arquivo, "r"); // r de read, afinal leremos o arquivo
    if (arquivo == NULL){
        printf("Arquivo nao encontrado\n");
        return;
    }

    // limpando o texto antigo da memória antes de carregar o novo usando free
    Linha* atual = inicio_do_texto;
    while (atual != NULL){
        Linha* proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
    inicio_do_texto = NULL;
    fim_do_texto = NULL;

    // lê o arquivo txt e monta a lista ligada
    while (fgets(conteudo_linha, sizeof(conteudo_linha), arquivo) != NULL){
        Linha* nova_linha = malloc(sizeof(Linha));
        if (nova_linha == NULL){
            printf("Erro ao carregar arquivo \n");
            fclose(arquivo);
            return;
        }
        strcpy(nova_linha->conteudo, conteudo_linha);
        nova_linha->proxima = NULL;

        if (inicio_do_texto == NULL){
            inicio_do_texto = nova_linha;
            fim_do_texto = nova_linha;
        }
        else{
            fim_do_texto->proxima = nova_linha;
            fim_do_texto = nova_linha;
        }
    }
    fclose(arquivo);
    printf("Arquivo carregado com sucesso\n");
    printf("Pressione Enter para continuar\n");
    getchar();
    mostrar_texto();
}

void salvar_arquivo(){
    char nome_do_arquivo[101];
    FILE *arquivo;

    printf("Digite o nome do arquivo para salvar (ex: texto.txt): \n");
    scanf("%s", nome_do_arquivo);
    getchar();

    arquivo = fopen(nome_do_arquivo, "w");
    
    if (arquivo == NULL){
        printf("Erro ao abrir/criar arquivo\n");
        return;
    }

    Linha* atual = inicio_do_texto;
    while (atual != NULL){
        fprintf(arquivo, "%s", atual->conteudo);    
        atual = atual->proxima;
    }
    fclose(arquivo);
    printf("Arquivo salvo com sucesso, aperte Enter para continuar\n");
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Linha{
    char conteudo[256];
    struct Linha* proxima;
}Linha;

Linha* inicio_do_texto = NULL;
Linha* fim_do_texto = NULL;
char arquivo_atual[256] = "";

void novo_arquivo();
void abrir_arquivo();
void salvar_arquivo();
void editar_arquivo();
void deletar_arquivo();
void sair();
void limpar_terminal();
void mostrar_texto();

void menu_principal(){
    char escolha;
    do{
        limpar_terminal();
        printf("Editor de texto\n");
        printf("=============== \n");
        printf("1 - Criar um novo arquivo\n");
        printf("2 - Abrir arquivo existente\n");
        printf("3 - Editar arquivo aberto\n");
        printf("4 - Salvar arquivo atual\n");
        printf("5 - Deletar arquivo\n");
        printf("6 - Sair do editor de texto\n");
        printf("Escolha a função a ser utilizada: \n");
        scanf(" %c", &escolha); // preferi ler assim porque ele não entendia letras como escolhas inválidas
        getchar();

        switch(escolha){
            case '1':
                novo_arquivo();
                break;
            case '2':     
                abrir_arquivo();
                break;
            case '3':
                editar_arquivo();
                break;
            case '4':
                salvar_arquivo();
                break;
            case '5':
                deletar_arquivo();
                break;
            case '6':
                sair();
                break;
            default:
                printf("Escolha inválida. Por favor, tente novamente\n");
                printf("Pressione Enter para continuar\n");
                getchar(); 
        }
    } while(escolha != '6'); 
}

void novo_arquivo(){
    limpar_terminal();

    strcpy(arquivo_atual, "");

    // limpando o buffer para um novo arquivo
    Linha* atual = inicio_do_texto;    
    while (atual != NULL){
        Linha* proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
    inicio_do_texto = NULL;
    fim_do_texto = NULL;
    printf("--- CRIANDO UM NOVO ARQUIVO DE TEXTO \n");
    printf("Digite o seu texto linha por linha. \n");
    printf("Digite FIM em uma linha isolada para encerrar o texto\n");

    // loop para digitar várias linhas seguidas
    while (1){
        char conteudo_da_linha[256];
        printf("> ");
        fgets(conteudo_da_linha, sizeof(conteudo_da_linha), stdin);

        if (strcmp(conteudo_da_linha, "FIM\n") == 0)
            break;

        // aloca memória para a nova linha digitada
        Linha* nova_linha = malloc(sizeof(Linha));
        if (nova_linha == NULL){
            printf("Erro na criação de uma nova linha\n");
            return;
        }
        // preenche os nós da lista ligada com o conteudo digitado pelo usuario no teclado
        strcpy(nova_linha->conteudo, conteudo_da_linha);
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
    // salva o que o usuário escreve
    char responder;
    printf("Texto finalizado. Deseja salvar esse arquivo? (s/n): ");
    scanf(" %c", &responder);
    getchar();
    
    if (responder == 'S' || responder =='s')
        salvar_arquivo();
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
            printf("Linha %d: %s", contador, atual->conteudo);
            atual = atual->proxima;
            contador++;
        }
        printf("-----------------\n");
    }
    printf("Pressione Enter para continuar\n");
    getchar();
}

void abrir_arquivo(){
    limpar_terminal();
    char nome_do_arquivo[101];
    char conteudo_linha[256];
    
    printf("Digite o nome do arquivo a ser aberto: \n");
    scanf("%s", nome_do_arquivo);
    getchar();

    FILE* arquivo = fopen(nome_do_arquivo, "r"); // r de read, afinal leremos o arquivo
    if (arquivo == NULL){
        printf("Arquivo nao encontrado\n");
        printf("Pressione Enter para voltar ao menu\n");
        getchar();
        return;
    }
    //guardando o nome do arquivo que foi aberto
    strcpy(arquivo_atual, nome_do_arquivo);

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
    limpar_terminal();
    mostrar_texto();
}

void salvar_arquivo(){
// se não tiver nenhum arquivo aberto na memória, peço o nome pela primeira vez
if (strlen(arquivo_atual) == 0){
    printf("Digite o nome do arquivo para salvar (ex: texto.txt): \n");
    scanf("%s", arquivo_atual); // guardando na variável local
    getchar(); 
}

FILE *arquivo = fopen(arquivo_atual, "w");

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
printf("Arquivo '%s' salvo com sucesso! Pressione Enter para continuar.\n", arquivo_atual);
getchar();
}

void deletar_arquivo(){
    limpar_terminal();
    char arquivo_a_deletar[101];
    printf("Deletar arquivo\n");
    printf("Digite o nome do arquivo que deseja apagar: \n");
    scanf("%s", arquivo_a_deletar);
    getchar();

    if (remove(arquivo_a_deletar) == 0){
        printf("O arquivo '%s' foi deletado com sucesso \n", arquivo_a_deletar);
        // se o arquivo deletado foi o que estava aberto, limpamos o editor
        if (strcmp(arquivo_atual, arquivo_a_deletar) == 0){
            Linha* atual = inicio_do_texto;
            while (atual != NULL){
                Linha *proxima = atual->proxima;
                free(atual);
                atual = proxima;
            }
            inicio_do_texto = NULL;
            fim_do_texto = NULL;
            strcpy(arquivo_atual, "");
        }
    }
    else{
        printf("Não foi possível deletar o arquivo.\n");
    }
    printf("Pressione Enter para voltar ao menu\n");
    getchar();
}   

void editar_arquivo(){
    limpar_terminal();
    if (inicio_do_texto == NULL){
        printf("Não há linhas para editar: o arquivo é vazio\n");
        printf("Pressione Enter para continuar...\n");
        getchar();
        return;
    }

    // mostra o texto para o usuário saber qual linha escolher
    mostrar_texto();
    printf("----------------------\n");

    int numero_linha;
    printf("Digite o número da linha que deseja alterar: ");
    scanf("%d", &numero_linha);
    getchar();
    
    // caminha na lista até achar a linha escolhida
    Linha* atual = inicio_do_texto;
    int contador = 1;
    while (atual != NULL && contador < numero_linha){
        atual = atual->proxima;
        contador++;
    }

    //checando se a linha de fato existe
    if (atual == NULL || numero_linha <= 0){
        printf("Linha inválida!\n");
    }
    else{
        printf("Digite o novo texto para a linha %d:\n> ", numero_linha);
        fgets(atual->conteudo, sizeof(atual->conteudo), stdin);
        printf("Linha editada com sucesso!\n");

        printf("Gravando alterações...\n ");
        salvar_arquivo(); // a funcao salvar_arquivo já tem getchar
    }   
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
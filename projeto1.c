#include <stdio.h>
#include <stdlib.h>  //usado system
#include <ctype.h>   //usado toupper
#include <string.h>  //usado strcmp
#include <time.h>

typedef char Str20[20];

typedef struct Colaborador{
    Str20 date;
    Str20 timeIn;
    Str20 timeOut;
    Str20 details;
} ColType;

ColType registroFuncionario;
Str20 currentDate;
Str20 currentTime;
Str20 id;
Str20 searchDate;
int hours;
int hoursTemp;
Str20 temp;

FILE *fileFuncionario;

char resposta;

int option;

void getDate(){ //adquirir a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(currentDate, "%02d/%02d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return;
}

void getTime(){ //adquirir a hora atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(currentTime, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    return;
}

void input(){ //input template para option
    printf("Opcao: ");
    scanf("%i",&option);
    fflush(stdin);
    return;
};

void inputId(){ //input template para o ID
    system("cls");
    printf("Inserir ID: ");
    scanf("%s",id);
    fflush(stdin);
    return;
};

void inputDate(){ //input template para a data
    system("cls");
    printf("Inserir Data (DD/MM/AAAA): ");
    scanf("%s",searchDate);
    fflush(stdin);
    return;
};

void baterPonto(){
    inputId();
    getTime();

    fileFuncionario = fopen(id, "a+b"); //abrir o arquivo

    if (strcmp(registroFuncionario.date, currentDate)==0){ //testar se a data é igual a data atual
        strcpy(registroFuncionario.timeOut, currentTime);
    }else{
        strcpy(registroFuncionario.date, currentDate);
        strcpy(registroFuncionario.timeIn, currentTime);
    }
    fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
    fclose(fileFuncionario); //fecha o arquivo 
    system("pause");
};

void calcularHoras(){
    inputId();

    system("cls");

    fileFuncionario = fopen(id, "r+b");


    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        strncpy(temp,registroFuncionario.timeIn, 2);
        sscanf(temp, "%d", &hoursTemp);
        hours = hours + hoursTemp;
        hoursTemp = 0;
        strncpy(temp,registroFuncionario.timeOut, 2);
        sscanf(temp, "%d", &hoursTemp);
        hours -= hoursTemp;
    }while (!feof(fileFuncionario));

    fclose(fileFuncionario);

    printf("Horas Trabalhadas: %i \n", hours);

    printf("\nNova Consulta? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        calcularHoras();
    }else{
        system("pause");
    }
};

void consultaTotal(){
    inputId();
    system("cls");

    fileFuncionario = fopen(id, "r+b");

    while (!feof(fileFuncionario)){
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, "NULL")!=0 && (!feof(fileFuncionario))){
            printf("***** %s *****\n\n", registroFuncionario.date);
            printf("Chegada: %s\n",registroFuncionario.timeIn);
            printf("Saida: %s\n",registroFuncionario.timeOut);
            printf("Observacoes: %s\n\n",registroFuncionario.details);
        }
    }

    fclose(fileFuncionario);

    printf("\nNova Consulta? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        consultaTotal();
    }else{
        system("pause");
    }
};

void alterar(){
    inputId();
    inputDate();

    system("cls");

    fileFuncionario = fopen(id, "r+b");

    int found = 0;
    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, searchDate) == 0){
            found = 1;
            printf("***** %s *****\n\n", registroFuncionario.date);
            printf("Chegada: %s\n",registroFuncionario.timeIn);
            printf("Saida: %s\n",registroFuncionario.timeOut);
            printf("Observacoes: %s\n\n",registroFuncionario.details);

            do
            {
            printf("Qual Campo Deseja Alterar?\n");
            printf("1 - Chegada\n");
            printf("2 - Saida\n");
            printf("3 - Observacoes\n");
            scanf(" %s",&resposta);
            system("cls");
            if (resposta=='1'){
                printf("Nova chegada: ");
                Str20 novaChegada;
                scanf(" %s", novaChegada);
                fflush(stdin);
                strcpy(registroFuncionario.timeIn, novaChegada);
            }
            
            else if (resposta == '2'){
                printf("Nova saida: ");
                Str20 novaSaida;
                scanf(" %s", novaSaida);
                fflush(stdin);
                strcpy(registroFuncionario.timeOut, novaSaida);
            }
            else if (resposta == '3'){
                printf("Observacoes: ");
                Str20 observacoes;
                scanf(" %s", observacoes);
                fflush(stdin);
                strcpy(registroFuncionario.details, observacoes);
            }
            printf("\nAlterar Outro Campo? S/N ");
            scanf(" %s",&resposta);
            fflush(stdin);
            resposta=toupper(resposta);
            }while (resposta!='N');
            fseek(fileFuncionario, -sizeof(ColType), 1); //coloca o cursor onde a leitura está
            fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
            fclose(fileFuncionario);
            break;
            
            
        }
    }while (!feof(fileFuncionario));

    if(found == 0){
        printf("Registro não encontrado!\n\n");
    }

    printf("\nNova Alteracao? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        alterar();
    }else{
        system("pause");
    }


};

void remover(){
    inputId();
    inputDate();

    fopen(id, "r+b");
    int found = 0;
    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, searchDate)==0){
            found = 1;
        }
    }while(!feof(fileFuncionario) && (found !=1));

    if(found ==1){
        strcpy(registroFuncionario.date, "NULL");
        fseek(fileFuncionario, -sizeof(ColType), 1);
        fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        fclose(fileFuncionario);
    }
    printf("\nNova Exclusão? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        remover();
    }else{
        system("pause");
    }

};

int main()
{
    getDate();
    while (option != 3){
        system("cls");
        printf("******** MAQUINA DE PONTO ********\n\n");
        printf("1 - Funcionario\n");
        printf("2 - Financeiro\n");
        printf("3 - Finalizar\n");
        input();
        switch (option)
        {
        case 1: //funcionario
            while (option != 2)
            {
                system("cls");
                printf("******** FUNCIONARIO ********\n\n");
                printf("1 - Bater Ponto\n");
                printf("2 - Voltar\n");
                input();
                switch (option)
                {
                case 1:
                    baterPonto();
                    break;
                
                case 2:
                    break;
                }
                //Bater ponto
            }
            break;
        case 2: //financeiro
            while (option != 5)
            {
                system("cls");
                printf("******** FINANCEIRO ********\n\n");
                printf("1 - Calcular Horas de Funcionario\n");
                printf("2 - Consulta Total de Funcionario\n");
                printf("3 - Alterar Ponto de Funcionario\n");
                printf("4 - Remover Ponto de Funcionario\n");
                printf("5 - Voltar\n");
                input();
                switch (option)
                {
                case 1: //calcular horas trabalhadas, a partir do id
                    calcularHoras();
                    break;
                case 2: //consulta total, a partir do id
                    consultaTotal();
                    break;
                case 3: //alterar campos, a partir do id e após achar, pesquisa uma data e realiza a alteração
                    alterar();
                    break;
                case 4: //remoção, a partir do id, segue a mesma logica da alteração mas muda todos os campos para NULL
                    remover();
                    break;
                }
                
            }
            break;
        }

    };

    return 0;
};
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

FILE *fileFuncionario;

char resposta;

int option;

void getDate(){ //adquirir a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char year = (char) (tm.tm_year + 1900);
    char month = (char) (tm.tm_mon + 1);
    char day = (char) (tm.tm_mday);
    Str20 date = {year,'-', month,'-', day};
    strcpy(currentDate, date);
    return;
}

void getTime(){ //adquirir a hora atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char hour = (char) (tm.tm_hour);
    char min = (char) (tm.tm_min);
    char sec = (char) (tm.tm_sec);
    Str20 time = {hour,':', min,':', sec};
    strcpy(currentTime, time);
    return;
}

void input(){ //input template para option
    printf("Opcao: ");
    scanf("%i",&option);
    fflush(stdin);
    return;
};

int inputId(){ //input template para o ID
    system("cls");
    int id;
    printf("Inserir ID: ");
    scanf("%i",&id);
    fflush(stdin);
    return id;
};

char inputDate(){ //input template para a data
    system("cls");
    char date;
    printf("Inserir Data (DD-MM-AAAA): ");
    scanf("%s",&date);
    fflush(stdin);
    return date;
};

void baterPonto(){
    const char id = (char) inputId();
    getDate();
    getTime();

    fileFuncionario = fopen(&id, "a+b"); //abrir o arquivo

    short found = 0;
    do{
	fread(&registroFuncionario,sizeof(ColType),1,fileFuncionario); //ler o arquivo
	if (strcmp(registroFuncionario.date, currentDate)==0){ //testar se a data é igual a data atual
	  found=1;}}
    while (!feof(fileFuncionario)); //fazer isso enquanto nao estiver no final do arquivo

    if (found == 0){ //se nao há data registrada, registra a data e hora atual no arquivo
        strcpy(registroFuncionario.date, currentDate);
        strcpy(registroFuncionario.timeIn, currentTime);
    }else{
        strcpy(registroFuncionario.timeOut, currentTime); //caso contrario registra a hora atual como horario de saída
    }

    fseek(fileFuncionario, -sizeof(ColType), 1); //coloca o cursor onde leitura esta
    fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
    fclose(fileFuncionario); //fecha o arquivo
};

void calcularHoras(){
    const char id = (char) inputId();
    system("cls");
    int hours;

    fileFuncionario = fopen(&id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        hours += (int)registroFuncionario.timeIn;
    }while (!feof(fileFuncionario));

    fclose(fileFuncionario);

    printf("Horas Trabalhadas: %i \n", hours);

    printf("\nNova Consulta? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        calcularHoras(inputId());
    }else{
        system("pause");
    }
};

void consultaTotal(){
    const char id = (char) inputId();
    system("cls");

    fileFuncionario = fopen(&id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, "NULL")){
            printf("***** %s *****\n\n", registroFuncionario.date);
            printf("Chegada: %s\n",registroFuncionario.timeIn);
            printf("Saida: %s\n",registroFuncionario.timeOut);
            printf("Observacoes: %s\n\n",registroFuncionario.details);
        }
    }while (!feof(fileFuncionario));

    fclose(fileFuncionario);

    printf("\nNova Consulta? S/N ");
    scanf(" %s",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=='S'){
        consultaTotal(inputId());
    }else{
        system("pause");
    }
};

void alterar(){
    const char id = (char)inputId();
    char date = inputDate();

    system("cls");

    fileFuncionario = fopen(&id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, &date) == 0){
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
            fflush(stdin);
            resposta=toupper(resposta);
                switch (resposta)
            {
            case 1:
                system("cls");
                printf("Nova chegada: ");
                Str20 novaChegada;
                scanf(" %s", novaChegada);
                fflush(stdin);
                strcpy(registroFuncionario.timeIn, novaChegada);
                break;
            
            case 2:
                system("cls");
                printf("Nova saida: ");
                Str20 novaSaida;
                scanf(" %s", novaSaida);
                fflush(stdin);
                strcpy(registroFuncionario.timeOut, novaSaida);
                break;
            case 3:
                system("cls");
                printf("Observacoes: ");
                Str20 observacoes;
                scanf(" %s", observacoes);
                fflush(stdin);
                strcpy(registroFuncionario.details, observacoes);
            }
            printf("\nAlterar Outro Campo? S/N ");
            scanf(" %c",&resposta);
            fflush(stdin);
            resposta=toupper(resposta);
            }while (resposta!='S');
            fseek(fileFuncionario, -sizeof(ColType), 1); //coloca o cursor onde a leitura está
            fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
            fclose(fileFuncionario);
            break;
            
            
        }
    }while (!feof(fileFuncionario));

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
    const char id = (char) inputId();
    char date = inputDate();

    fopen(&id, "a+b");

    fseek(fileFuncionario, 0, 0);
    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcmp(registroFuncionario.date, &date)==0){
            strcpy(registroFuncionario.date, "NULL");
            fseek(fileFuncionario, -sizeof(ColType), 1);
            fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
            fclose(fileFuncionario);
        }
    }while(!feof(fileFuncionario));

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
                printf("2 - Sair\n");
                input();
                //Bater ponto
                if(option == 1){
                    baterPonto();
                }
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
                printf("5 - Sair\n");
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
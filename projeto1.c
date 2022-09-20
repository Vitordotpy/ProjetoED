#include <stdio.h>
#include <stdlib.h>  //usado system
#include <ctype.h>   //usado toupper
#include <string.h>  //usado strcmp
#include <time.h>

typedef struct Colaborador{
    char date;
    char timeIn;
    char timeOut;
    char details;
} ColType;

ColType registroFuncionario;

FILE *fileFuncionario;

char currentDate;
char currentTime;

int option;

void baterPonto(int id){
    currentDate = getDate();
    currentTime = getTime();

    fileFuncionario = fopen(id, "a+b"); //abrir o arquivo

    short found = 0;
    do{
	fread(&registroFuncionario,sizeof(ColType),1,fileFuncionario); //ler o arquivo
	if (strcmp(registroFuncionario.date, currentDate)==0){ //testar se a data é igual a data atual
	  found=1;}}
    while (!feof(fileFuncionario)); //fazer isso enquanto nao estiver no final do arquivo

    if (found == 0){ //se nao há data registrada, registra a data e hora atual no arquivo
        registroFuncionario.date = currentDate;
        registroFuncionario.timeIn = currentTime;
        registroFuncionario.timeOut = NULL;
        registroFuncionario.details = NULL;
    }else{
        registroFuncionario.timeOut = currentTime; //caso contrario registra a hora atual como horario de saída
    }

    fseek(fileFuncionario, -sizeof(ColType), 1); //coloca o cursor onde leitura esta
    fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
    fclose(fileFuncionario); //fecha o arquivo
};

void calcularHoras(int id){
    system("cls");
    int hours;

    fileFuncionario = fopen(id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        hours += (int)registroFuncionario.timeIn;
    }while (!feof(fileFuncionario));

    fclose(fileFuncionario);

    printf("Horas Trabalhadas: %i \n", hours);

        char resposta;
    printf("\nNova Consulta? S/N ");
    scanf(" %c",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=="S"){
        calcularHoras(inputId());
    }else{
        system("pause");
    }
};

void consultaTotal(int id){
    system("cls");

    fileFuncionario = fopen(id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(registroFuncionario.date =! NULL){
            printf("***** %s *****\n\n", registroFuncionario.date);
            printf("Chegada: %s\n",registroFuncionario.timeIn);
            printf("Saida: %s\n",registroFuncionario.timeOut);
            printf("Observações: %s\n\n",registroFuncionario.details);
        }
    }while (!feof(fileFuncionario));

    fclose(fileFuncionario);

    char resposta;
    printf("\nNova Consulta? S/N ");
    scanf(" %c",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=="S"){
        consultaTotal(inputId());
    }else{
        system("pause");
    }
};

void alterar(){
    int id = inputId();
    char date = inputDate();
    char resposta;

    system("cls");

    fileFuncionario = fopen(id, "a+b");

    fseek(fileFuncionario, 0, 0);

    do{
        fread(&registroFuncionario, sizeof(ColType), 1, fileFuncionario);
        if(strcomp(registroFuncionario.date, date) == 0){
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
            scanf(" %c",&resposta);
            fflush(stdin);
            resposta=toupper(resposta);
                switch (resposta)
            {
            case 1:
                char novaChegada;
                system("cls");
                printf("Nova chegada: ");
                scanf(" %c", &novaChegada);
                fflush(stdin);
                registroFuncionario.timeIn = novaChegada;
                break;
            
            case 2:
                char novaSaida;
                system("cls");
                printf("Nova saida: ");
                scanf(" %c", &novaSaida);
                fflush(stdin);
                registroFuncionario.timeOut = novaSaida;
                break;
            case 3:
                char observacoes;
                system("cls");
                printf("Observacoes: ");
                scanf(" %c", &observacoes);
                fflush(stdin);
                registroFuncionario.details = observacoes;
            }
            printf("\nAlterar Outro Campo? S/N ");
            scanf(" %c",&resposta);
            fflush(stdin);
            resposta=toupper(resposta);
            }while (resposta!="N");
            fseek(fileFuncionario, -sizeof(ColType), 1); //coloca o cursor onde a leitura está
            fwrite(&registroFuncionario, sizeof(ColType), 1, fileFuncionario); //salva os dados onde o cursor está
            fclose(fileFuncionario);
            break;
            
            
        }
    }while (!feof(fileFuncionario));

    printf("\nNova Alteracao? S/N ");
    scanf(" %c",&resposta);
    fflush(stdin);
    resposta=toupper(resposta);
    if(resposta=="S"){
        alterar();
    }else{
        system("pause");
    }


};

void remover(){

};

char getDate(){ //adquirir a data atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char date[] = (char)("%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return date;
}

char getTime(){ //adquirir a hora atual
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char time[] = (char)("%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
    return time;
}

void input(){ //input template para option
    printf("Opcao: ");
    scanf("%s",&option);
    fflush(stdin);
    return;
};

int inputId(){ //input template para o ID
    system("cls");
    int id;
    printf("Inserir ID: ");
    scanf("%s",&id);
    fflush(stdin);
    return id;
};

int inputDate(){ //input template para a data
    system("cls");
    char date;
    printf("Inserir Data (DD-MM-AAAA): ");
    scanf("%s",&date);
    fflush(stdin);
    return date;
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
                    baterPonto(inputId());
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
                    calcularHoras(inputId());
                    break;
                case 2: //consulta total, a partir do id
                    consultaTotal(inputId());
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
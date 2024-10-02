#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED// 
// Structs
typedef struct data {
    int hora, minuto;
} Data;

typedef struct voo {
    char codigo[5];
    Data previsao_chegada;
    int num_passageiros;
    int check_hora;
    Data horario_chegada;
} Voo;

typedef struct no
{
    Voo voo;
    struct no *prox;
}No;

// Prototipos
void imprimirVoo(Voo voo, bool mostra_check_hora);
int compararHoras(Data hora1, Data hora2, int minutos);
void printData(Data data);
Data setHora(int hora, int minuto);
Data gerarData(int hora_minima, int minuto_minimo);
Data verificaHora(Data horario);
int check_hora(Data previsao, Data chegada);
void mensagem_erro(char *mensagem);

// Funcoes
void imprimirVoo(Voo voo, bool mostra_check_hora) {
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("\n\t\tCodigo: %s\n", voo.codigo);
    printf("Passageiros: %d\n", voo.num_passageiros);
    printf("Previsao de chegada: ");
    printData(voo.previsao_chegada);
    if(voo.horario_chegada.hora != -1 && voo.horario_chegada.minuto != -1) {
        printf("Horario de chegada: ");
        printData(voo.horario_chegada);
    }
    printf("\n");
    if(mostra_check_hora){{
            if(voo.check_hora == -1){
                vermelho();
                printf("Pouso de emergencia\n");
                resetcor();
            }else if(voo.check_hora == 1){
                verde();
                printf("Pouso sem atraso\n");
                resetcor();
            }else{
                amarelho();
                printf("Pouso atrasado\n");
                resetcor();
            }
        }  
    }

}

// int compararHoras(Data hora1, Data hora2, int minutos) {
    
// }

void printData(Data data) {
    printf("%.2d:%.2d\n", data.hora, data.minuto);
}

Data setHora(int hora, int minuto) {
    Data aux;
    aux.hora = hora;
    aux.minuto = minuto;
    return aux;
}

Data gerarData(int hora_minima, int minuto_minimo) {
    if(minuto_minimo == 60) {
        hora_minima++;
        minuto_minimo = 0;
    }
    if(hora_minima == 24) {
        hora_minima = 0;
    }

    int hora = rand() % (24 - hora_minima) + hora_minima;
    int minuto = rand() % (60 - minuto_minimo) + minuto_minimo;

    return setHora(hora, minuto);
}

Data verificaHora(Data horario){
    if(horario.minuto >= 60){
        horario.minuto -= 60;
        horario.hora++;
    }
    if(horario.hora >= 24){
        horario.hora = 0;
    }

    return horario;
}

Data adicionaMinutos(Data horaIni,int minutos){
    Data horaAdicionada ;
    horaAdicionada.minuto += horaIni.minuto + minutos;
    if (horaAdicionada.minuto > 59){
        do{
            horaAdicionada.minuto -= 60;
            horaAdicionada.hora++;
            if (horaAdicionada.hora == 24){
                horaAdicionada.hora = 0;
            }
            return horaAdicionada;
        }while(horaAdicionada.minuto > 59);
    }
     else
    return horaAdicionada;
}
int check_hora(Data previsao, Data chegada){
    int previsao_minutos = (previsao.hora * 60) + previsao.minuto;
    int chegada_minutos = (chegada.hora * 60) + chegada.minuto;

    if(chegada_minutos <= previsao_minutos +10){
        return 1; //no horario
    }else{
        return 0; //atrasado
    }
}

void mensagem_erro(char* mensagem) {
    vermelho();
    printf("%s\n", mensagem);
    resetcor();
}

void mensagem_sucesso(char*mensagem){
    verde();
    printf("%s\n", mensagem);
    resetcor();
}

void fimFuncao() {
    fflush(stdin);
    printf("Pressione Enter para continuar...\n");
    getchar();
    system("cls");
}

#endif // STRUCTS_H_INCLUDED
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
void imprimirVoo(Voo voo, int check_hora);
int compararHoras(Data hora1, Data hora2, int minutos);
void printData(Data data);
Data setHora(int hora, int minuto);
Data gerarData(int hora_minima, int minuto_minimo);

// Funcoes
void imprimirVoo(Voo voo, int check_hora) {
    printf("Codigo: %s\n", voo.codigo);
    printf("Passageiros: %d\n", voo.num_passageiros);
    printf("Previsao de chegada: ");
    printData(voo.previsao_chegada);
    if(voo.horario_chegada.hora != NULL && voo.horario_chegada.minuto != NULL) {
        printf("Horario de chegada: ");
        printData(voo.horario_chegada);
    }
    printf("\n");
    if(check_hora != 0) {
        if(voo.check_hora != NULL){
            printf("Sem check_hora");
        }else{
            if(voo.check_hora == -1){
                printf("Pouso de emergencia");
            }else if(voo.check_hora == 1){
                printf("Pouso dentro do horario previsto");
            }else{
                printf("Pouso atrasado");
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
    if(minuto_minimo ==60) {
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

#endif // STRUCTS_H_INCLUDED
#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

// Structs
typedef struct data {
    int hora, minuto;
} Data;

typedef struct voo {
    char codigo[5];
    Data previsao_chegada;
    int num_passageiros;
    int check_hora;
} Voo;

typedef struct no
{
    Voo voo;
    struct no *prox;
}No;

// Prototipos
void imprimirVoo(Voo voo);
int compararHoras(Data hora1, Data hora2, int minutos);
void printData(Data data);
Data setHora(int hora, int minuto);
Data gerarData(int hora_minima, int minuto_minimo);

// Funcoes
void imprimirVoo(Voo voo) {
    printf("Codigo: %s\n", voo.codigo);
    printf("Passageiros: %d\n", voo.num_passageiros);
    printf("Previsao de chegada: ");
    printData(voo.previsao_chegada);
    printf("\n");
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

int compararHoras(Data hora1, Data hora2, int minutos) {
    
}

void printData(Data data) {
    printf("%.2d:%.2d", data.hora, data.minuto);
}

Data setHora(int hora, int minuto) {
    Data aux;
    aux.hora = hora;
    aux.minuto = minuto;
    return aux;
}

Data gerarData(int hora_minima, int minuto_minimo) {
    if(hora_minima == 23) hora_minima = 0;
    if(minuto_minimo == 59) hora_minima = 0;

    int hora = rand() % (24 - hora_minima + 1) + hora_minima;
    int minuto = rand() % (60 - minuto_minimo + 1) + minuto_minimo;

    return setHora(hora, minuto);
}

#endif // STRUCTS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "STRUCTS.h"
#include "FILA.h"
#include "PILHA.h"

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia);

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos);

void header();

int main() {
    Fila* emergencias = CriaFila();
    Fila* esperas = CriaFila();
    Fila* pousos = CriaFila();

    srand(time(NULL));

    Data hora_atual = gerarData(0,0);
    
    imprimeFila(esperas);
    imprimeFila(emergencias);

    inserir_voo(esperas, emergencias, rand() % 10);
    inserir_voo(esperas, emergencias, rand() % 10);

    

    liberaFila(emergencias);
    liberaFila(esperas);
    liberaFila(pousos);

    return 0;
} 

void header() {
    printf("\n----------\n");
    printf("\tBlueSky\n");
    printf("----------\n");
}
void menu(){
    printf("\n[1] Inserir Voo\n[2] Autorizar Voo\n[3] Imprimir Fila para pouso\n[4] Imprimir o Próximo pouso\n[5] Imprimir Voos que já pousaram\n[6] Simulação de pouso \n");
}

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia) {
    Voo voo;
    Data data;
    
    header();
    fflush(stdin);

    printf("Insira o codigo do voo\n");
        fgets(voo.codigo, 5, stdin);
    fflush(stdin);

    printf("Insira a quantidade de passageiros\n");
        scanf("%d", &voo.num_passageiros);
    do{
        printf("Insira a previsão de chegada\n");
        printf("Hora (0-23): ");
        scanf("%d", &data.hora);
    }while(data.hora < 0 && data.hora > 24);
    
    do{
        printf("Minutos (0-59): ");
        scanf("%d", &data.minuto);
    }while(data.minuto < 0 && data.minuto > 60);

    voo.previsao_chegada = data;
    
    printf("Eh pouso de emergencia? %c\n", emergencia == 0 ? 'S' : 'N');

    if(emergencia == 0) {
        InsereFila(emergencias, voo);
    } else {
        InsereFila(esperas, voo);
    }
}

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos) {
    Voo voo_removido;
    if(VaziaFila(emergencias)) {
        if(!VaziaFila(esperas)) {
            voo_removido = RetiraFila(esperas);
            // verificar se ta atrasado ou nao

            
        } else {
            printf("Nao existem voos em espera para pousar\n");
            return;
        }
    
    } else {
        voo_removido = RetiraFila(emergencias);
        voo_removido.check_hora = -1;
    }
    
    printf("Voo removido!\n");
    imprimirVoo(voo_removido);
    InsereFila(pousos, voo_removido);
}
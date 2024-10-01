#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "STRUCTS.h"
#include "FILA.h"
#include "PILHA.h"
#include "colors.h"

/*LEMBRANDO QUE AS FUNÇOES QUE ESTAO EM VERMELHO QUANDO O CODIGO É EXECUTADO NÃO FORAM IMPLEMENTADAS AINDA E AS QUE ESTAO EM AMARELHO NECESSITAM SERES MODIFICADAS AO LONGO DO PROJETO*/ 

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia);

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data hora_atual);

void relatorio(Fila *esperas, Fila *emergencias);

void voos_pousados(Fila *pousos);

void header();

void menu();

int main() {
    Fila* emergencias = CriaFila();
    Fila* esperas = CriaFila();
    Fila* pousos = CriaFila();

    srand(time(NULL));

    Data hora_atual = gerarData(0,0);
    
    

    int op =0;

    do{
        menu(hora_atual);
        scanf("%d",&op);

        switch (op){
        //chama função inseririr voo
        case 1:
            inserir_voo(esperas, emergencias, rand() % 10);
            break;
        
        //chama função autorizar pouso
        case 2:
            autorizar_pouso(esperas,emergencias,pousos,hora_atual);
            break;

        case 3:
            relatorio(esperas,emergencias);
            break;
        case 4:
        
            break;
        
        //chama função imprimir fila para a fila pousos
        case 5:
            voos_pousados(pousos);
            break;
        case 6:   

            break;
        default:
            break;
        }
    }while (op != 7);

    liberaFila(emergencias);
    liberaFila(esperas);
    liberaFila(pousos);

    return 0;
} 

void menu(Data hora_atual){
    header();

    printf("Hora atual:");
    printData(hora_atual);

    printf("O que deseja fazer?");
    printf("\n[1]-Inserir voo");
    amarelho();
    printf("\n[2]-Autorizar voo");//completar função 
    vermelho();
    printf("\n[3]-Relatorio das aeronaves");//criar função
    printf("\n[4]-Imprimir proxima aeronave a pousar");//criar função
    resetcor();
    printf("\n[5]-Imprimir aeronaves pousadas");
    vermelho();
    printf("\n[6]-Simular o pouso dentro de um intervalo de tempo");//criar função 
    resetcor();
    printf("\n[7]-Finalizar o sistema");
    printf("\nOpcao: ");
}

void header() {
    printf("\n\t----------\n");
    printf("\tBlueSky\n");
    printf("\t----------\n");
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
    
    voo.previsao_chegada = gerarData(0,0);
    voo.horario_chegada.hora = NULL;
    voo.horario_chegada.minuto = NULL;

    printf("Previsao de chegada:");
    printData(voo.previsao_chegada);
    
    printf("Eh pouso de emergencia? %c\n", emergencia == 0 ? 'S' : 'N');

    if(emergencia == 0) {
        InsereFila(emergencias, voo);
    } else {
        InsereFila(esperas, voo);
    }
}

//completar função
void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data hora_atual) {
    Voo voo_removido;
    if(VaziaFila(emergencias)) {
        if(!VaziaFila(esperas)) {
            voo_removido = RetiraFila(esperas);
            // verificar se ta atrasado ou nao7
        } else {
            printf("Nao existem voos em espera para pousar\n");
            return;
        }

    } else {
        voo_removido = RetiraFila(emergencias);
        voo_removido.check_hora = -1;
    }

    voo_removido.horario_chegada = hora_atual;
    voo_removido.horario_chegada.minuto += 10;

    voo_removido.horario_chegada = verificaHora(voo_removido.horario_chegada);

    printf("Voo pousou!\n");
    imprimirVoo(voo_removido, false);
    InsereFila(pousos, voo_removido);
}

void relatorio(Fila *esperas, Fila *emergencias) {
    header();

    printf("Voos em estado de emergencia:\n");
    imprimeFila(emergencias, false);
    printf("Voos em lista de espera:\n");
    imprimeFila(esperas, false);
}

//void simular_voos(Fila*esperas, Fila*emergencias, Data*hora_atual){


//}
void voos_pousados(Fila *pousos) {
    header();

    printf("Voos que ja pousaram:\n");
    imprimeFila(pousos,false);
}
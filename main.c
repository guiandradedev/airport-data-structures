#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stdio.h"
#include "STRUCTS.h"
#include "FILA.h"
#include "PILHA.h"
#include "colors.h"

/*LEMBRANDO QUE AS FUNÇOES QUE ESTAO EM VERMELHO QUANDO O CODIGO É EXECUTADO NÃO FORAM IMPLEMENTADAS AINDA E AS QUE ESTAO EM AMARELHO NECESSITAM SERES MODIFICADAS AO LONGO DO PROJETO*/ 

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia);

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos);

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
        menu();
        scanf("%d",&op);

        switch (op){
        //chama função inseririr voo
        case 1:
            inserir_voo(esperas, emergencias, rand() % 10);
            break;
        
        //chama função autorizar pouso
        case 2:
            autorizar_pouso(esperas,emergencias,pousos);
            break;

        case 3:
            
            break;
        case 4:
        
            break;
        
        //chama função imprimir fila para a fila pousos
        case 5:
            imprimeFila(pousos);
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

void menu(){
    header();

    printf("O que deseja fazer?");
    printf("\n[1]-Inserir voo");
    amarelho();
    printf("\n[2]-Autorizar voo");//completar função 
    vermelho();
    printf("\n[3]-Relatorio das aeronaves");//criar função
    printf("\n[4]-Imprimir proxima aeronova a pousar");//criar função
    resetcor();
    printf("\n[5]-Imprimir aeronaves pousadas");
    vermelho();
    printf("\n[6]-Simular o pouso dentro de um intervalo de tempo");//criar função 
    resetcor();
    printf("\n[7]-Finalizar o sistema");
    printf("\nOpcao: ")
}

void header() {
    printf("\n----------\n");
    printf("\tBlueSky\n");
    printf("----------\n");
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

//completar função
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
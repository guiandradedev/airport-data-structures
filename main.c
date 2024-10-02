#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "colors.h"
#include "STRUCTS.h"
#include "FILA.h"
#include "PILHA.h"

/*LEMBRANDO QUE AS FUNÇOES QUE ESTAO EM VERMELHO QUANDO O CODIGO É EXECUTADO NÃO FORAM IMPLEMENTADAS AINDA E AS QUE ESTAO EM AMARELHO NECESSITAM SERES MODIFICADAS AO LONGO DO PROJETO*/ 

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual);

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data *hora_atual);

void relatorio(Fila *esperas, Fila *emergencias, Data *hora_atual);

void voos_pousados(Fila *pousos, Data *hora_atual);

void proximo_voo(Fila *esperas, Fila *emergencias, Data *hora_atual);

void simular_voos(Fila*esperas, Fila*emergencias,Data* hora_atual, int minutos_intervalo);

void aviao(Data *data);
void header(Data* data);

void menu();

int main() {
    Fila* emergencias = CriaFila();
    Fila* esperas = CriaFila();
    Fila* pousos = CriaFila();
    Data hora_atual = gerarData(0,0);

    int semente, aux, op = 0;
//SEMENTE
    do {
        header(&hora_atual);
        printf("Para começar, insira a semente: ");
        scanf("%d", &semente);

        if(semente < 0) {
            mensagem_erro("A semente deve ser um valor inteiro maior que 0!");
        }
    } while(semente < 0);
    srand(semente);

    do{
        menu(hora_atual);
        scanf("%d",&op);

        switch (op){
        //chama função inseririr voo
        case 1:
            inserir_voo(esperas, emergencias, rand() % 10, &hora_atual);
            break;
        
        //chama função autorizar pouso
        case 2:
            autorizar_pouso(esperas,emergencias,pousos, &hora_atual);
            break;

        case 3:
            relatorio(esperas,emergencias, &hora_atual);
            break;
        case 4:
            proximo_voo(esperas, emergencias, &hora_atual);
            break;
        
        //chama função imprimir fila para a fila pousos
        case 5:
            voos_pousados(pousos, &hora_atual);
            break;
        case 6:
            printf("Informe o Intervalo de tempo desejado para a simulação (em minutos): ");
            scanf("%d",&aux);
            simular_voos(esperas,emergencias,&hora_atual,aux);

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
    header(&hora_atual);

    // printf("Hora atual:");
    // printData(hora_atual);

    printf("O que deseja fazer?");
    printf("\n[1]-Inserir voo");
    printf("\n[2]-Autorizar pouso");
    printf("\n[3]-Relatorio das aeronaves");
    printf("\n[4]-Imprimir proxima aeronave a pousar");
    printf("\n[5]-Imprimir aeronaves pousadas");
    printf("\n[6]-Simular o pouso dentro de um intervalo de tempo");
    resetcor();
    printf("\n[7]-Finalizar o sistema");
    printf("\nOpcao: ");
}

void header(Data *data) {
    printf("\n--------------------------------------------------------------------------------\n");
    aviao(data);
    printf("--------------------------------------------------------------------------------\n");
}
void aviao(Data *data){
    printf("            ______\n");
    printf("            _\\ _~-\\___\n");
    printf("    =  = ==(____AA____D      BlueSky \n");
    printf("                \\_____\\___________________,-~~~~~~~`-.._\n");
    printf("                /     o O o o o o O O o o o o o o O o  |\\_\n");
    printf("                `~-.__        ___..----..                  )\n");
    printf("                      `---~~\\___________/------------`````\n");
    printf("                      =  ===(_________D");
    if(data != NULL) {
        printf("  Hora atual: ");
        printData(*data);
    } else {
        printf("\n");
    }
}

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual) {
    Voo voo;
    Data data;
    
    header(hora_atual);
    fflush(stdin);

    do {
        printf("Insira o codigo do voo (4 caracteres):\n");
        fgets(voo.codigo, sizeof(voo.codigo), stdin);

        if (strlen(voo.codigo) != 4) {
            mensagem_erro("O código deve ter 4 caracteres!");
        }
    } while (strlen(voo.codigo) != 4);
    fflush(stdin);

    do {
        printf("Insira a quantidade de passageiros\n");
        scanf("%d", &voo.num_passageiros);
        if(voo.num_passageiros <= 0) {
            mensagem_erro("O numero de passageiros deve ser maior que 0");
        }
    } while(voo.num_passageiros <= 0);
    
    voo.previsao_chegada = gerarData(0,0);
    voo.horario_chegada.hora = -1;
    voo.horario_chegada.minuto = -1;

    printf("Previsao de chegada:");
    printData(voo.previsao_chegada);
    
    printf("Eh pouso de emergencia? %c\n", emergencia == 0 ? 'S' : 'N');

    if(emergencia == 0) {
        InsereFila(emergencias, voo);
    } else {
        InsereFila(esperas, voo);
    }
    fimFuncao();
}

//completar função
void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data *hora_atual) {
    Voo voo_removido;
    if(VaziaFila(emergencias)) {
        if(!VaziaFila(esperas)) {
            voo_removido = RetiraFila(esperas);
            voo_removido.horario_chegada = *hora_atual;
            voo_removido.horario_chegada.minuto += 10;
            voo_removido.horario_chegada = verificaHora(voo_removido.horario_chegada);
            *hora_atual = voo_removido.horario_chegada;
            
            voo_removido.check_hora = check_hora(voo_removido.previsao_chegada, voo_removido.horario_chegada);
        } else {
            mensagem_erro("Nao existem voos em espera para pousar!");
            return;
        }

    } else {
        voo_removido = RetiraFila(emergencias);
        voo_removido.check_hora = -1;
    }

    printf("Voo pousou!\n");
    imprimirVoo(voo_removido, true);
    InsereFila(pousos, voo_removido);
    fimFuncao();
}

void relatorio(Fila *esperas, Fila *emergencias, Data *hora_atual) {
    header(hora_atual);

    mensagem_erro("Voos em estado de emergencia:");
    imprimeFila(emergencias, false);

    printf("\n");

    mensagem_sucesso("Voos em lista de espera:");
    imprimeFila(esperas, false);
    fimFuncao();
}


void simular_voos(Fila*esperas, Fila*emergencias,Data* hora_atual, int minutos_intervalo){
    int qtd_de_voos = minutos_intervalo/10;
    No *aux= emergencias->ini;
    bool existe_voo=false;

    Data hora_simulada = *hora_atual;

    if(aux != NULL && qtd_de_voos != 0) {
        existe_voo = true;
        printf("Voos em emergencia\n");
    }
    while(aux != NULL && qtd_de_voos != 0){
        imprimirVoo(aux->voo,false);

        hora_simulada.minuto += 10;
        hora_simulada = verificaHora(hora_simulada);

        printf("O voo pousaria as: ");
        printData(hora_simulada);
        vermelho();
        printf("Voo em estado de emergencia\n");
        resetcor();

        qtd_de_voos--;
        aux = aux->prox;
    }
    
    aux = esperas->ini;
    if(aux != NULL && qtd_de_voos != 0) {
        existe_voo = true;
        printf("Voos em espera\n");
    }

    while(aux != NULL && qtd_de_voos != 0){
        imprimirVoo(aux->voo,false);

        hora_simulada.minuto += 10;
        hora_simulada = verificaHora(hora_simulada);
        printf("O voo pousaria as: ");
        printData(hora_simulada);
        
        if(check_hora(aux->voo.previsao_chegada,aux->voo.horario_chegada) == 0){
            printf("O voo vai estar atrasado");
        }else{
            printf("O voo nao vai estar atrasado\n");
        }

        qtd_de_voos--;
        aux = aux->prox;
    }

    if(!existe_voo) {
        mensagem_erro("Nao existem voos para pousar!");
    }
    fimFuncao();
}

void voos_pousados(Fila *pousos, Data *hora_atual) {
    header(hora_atual);

    printf("Voos que ja pousaram:\n");
    imprimeFila(pousos,true);
    fimFuncao();
}

void proximo_voo(Fila *esperas, Fila *emergencias, Data *hora_atual) {
    if(!VaziaFila(emergencias)) {
        printf("Proximo voo a pousar (estado de emergencia)\n");
        imprimirVoo(emergencias->ini->voo, false);
        return;
    }else if(!VaziaFila(esperas)) {
        printf("Proximo voo a pousar:\n");
        imprimirVoo(esperas->ini->voo, false);
        return;
    }else{
        mensagem_erro("Nenhum voo na fila de espera!");
    }
    fimFuncao();
}
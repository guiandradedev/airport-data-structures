#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "colors.h"
#include "STRUCTS.h"
#include "FILA.h"
#include "FUNCTIONS.h"


// Protótipos
void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual, Fila *pousos, int TempoPouso);
void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data *hora_atual, int TempoPouso);
void busca_voo(Fila *esperas, Fila *emergencias, Fila *pousos);
void relatorio(Fila *esperas, Fila *emergencias, Data *hora_atual, int TempoPouso);
void voos_pousados(Fila *pousos, Data *hora_atual, int TempoPouso);
void proximo_voo(Fila *esperas, Fila *emergencias);
void simular_voos(Fila*esperas, Fila*emergencias,Data* hora_atual, int minutos_intervalo, int TempoPouso);
char* insere_codigo();
void alterar_status(Fila *esperas, Fila *emergencias);
void estatisticas(Fila *pousos);
void print_clima(int TempoPouso);
int geraTempoPouso();
void menuMudaTempo();
void MudaTempo(int *TempoPouso);
void aviao(Data *data);
void header(Data *data,int tempo);
void menu(Data hora_atual, int tempo);
void menuFiltro(Fila *esperas, Fila *emergencias, Fila* pousos, Data *hora_atual, int TempoPouso);

int main() {
    // Definição de variaveis
    Fila* emergencias = CriaFila();
    Fila* esperas = CriaFila();
    Fila* pousos = CriaFila();
    Data hora_atual;
    int semente, aux, op = 0;
    int TempoPouso = NULL;

    animacao();

    do {
        header(NULL,NULL);
        printf("Para comecar, insira a semente: ");
        scanf("%d", &semente);

        if(semente < 0) {
            mensagem_erro("A semente deve ser um valor inteiro maior que 0!");
        }
    } while(semente < 0);
    srand(semente);
    hora_atual = gerarData(0,0);
    TempoPouso = geraTempoPouso();

    do{
        menu(hora_atual,TempoPouso);
        scanf("%d",&op);

        switch (op){
        case 1:
            inserir_voo(esperas, emergencias, rand() % 6, &hora_atual, pousos, TempoPouso);
            break;
        case 2:
            autorizar_pouso(esperas,emergencias,pousos, &hora_atual, TempoPouso);
            break;
        case 3:
            relatorio(esperas,emergencias, &hora_atual, TempoPouso);
            break;
        case 4:
            proximo_voo(esperas, emergencias);
            break;
        case 5:
            voos_pousados(pousos, &hora_atual, TempoPouso);
            break;
        case 6:
            do{
                printf("Informe o Intervalo de tempo desejado para a simulacao (em minutos): ");
                scanf("%d",&aux);
                if(aux == 0){
                    mensagem_erro("Tempo para simulacao deve ser maior que 0");
                }
            }while(aux <= 0);
            simular_voos(esperas,emergencias,&hora_atual,aux, TempoPouso);
            break;
        case 7:
            busca_voo(esperas, emergencias, pousos);
            break;
        case 8:
            alterar_status(esperas,emergencias);
            break;
        case 9:
            estatisticas(pousos);
            break;
        case 10:
            MudaTempo(&TempoPouso);
            break;
        case 11:
            menuFiltro(esperas,emergencias,pousos,&hora_atual,TempoPouso);
            break;
        case 12:
            break;
        default:
            mensagem_erro("Comando incorreto.");
            fimFuncao();
            break;
        }
    }while (op != 12);

    animacao();

    header(&hora_atual, TempoPouso);
    mensagem_sucesso("Obrigado por acessar o sistema.");

    liberaFila(emergencias);
    liberaFila(esperas);
    liberaFila(pousos);

    return 0;
} 


int geraTempoPouso(){
// 0-Clima bom(10 minutos por pouso)45%
// 1-Clima decente(15 minutos por pouso)30%
// 2-Clima ruim(20 minutos por pouso)15%
// 3-Clima pessimo(30 minutos por pouso)10%
    int r = rand() % 100;

    if(r < 45) {
        return 10; // 45% de chance
    } else if (r < 75) {
        return 15; // 30% de chance
    } else if (r < 90) {
        return 20; // 15% de chance 
    } else {
        return 30; // 10% de chance 
    }
}

void print_clima(int TempoPouso){
    if(TempoPouso == NULL){
        return;
    }else if(TempoPouso == 10){
        mensagem_sucesso("Clima esta bom, com tempo medio de pouso de 10 minutos\n");
    }else if(TempoPouso == 15){
        mensagem_amarela("Clima esta decente, com tempo medio de pouso de 15 minutos\n");
    }else if(TempoPouso == 20){
        mensagem_erro("Clima esta ruim, com tempo medio de pouso de 20 minutos\n");
    }else{
        mensagem_erro("Clima esta pessimo, com tempo medio de pouso de 30 minutos\n");
    }
}

void menuMudaTempo(){
    printf("Como esta o clima?\n");
    printf("[1]-Clima esta bom\n");
    printf("[2]-Clima esta decente\n");
    printf("[3]-Clima esta ruim\n");
    printf("[4]-Clima esta pessimo\n");
    printf("opcao: ");
}

void MudaTempo(int *TempoPouso){
    menuMudaTempo();
    int op;
    do{
        scanf("%d", &op);
    } while (op < 1 || op >4);
    
    switch (op){
        case 1:
            *TempoPouso = 10;
            break;
        case 2:
            *TempoPouso = 15;
            break;
        case 3:
            *TempoPouso = 20;
            break;
        case 4:
            *TempoPouso = 30;
            break;
        default:
            break;
    }
}

void menu(Data hora_atual, int tempo){
    header(&hora_atual, tempo);

    printf("O que deseja fazer?\n");
    printf("[1]  - Inserir voo                  [7]  - Buscar voo por codigo\n");
    printf("[2]  - Autorizar pouso              [8]  - Alterar estado de um voo\n");
    printf("[3]  - Relatorio das aeronaves      [9]  - Estatisticas do aeroporto\n");
    printf("[4]  - Imprimir proxima aeronave    [10] - Alterar o clima\n");
    printf("[5]  - Imprimir aeronaves pousadas  [11] - Filtrar Voos\n");
    printf("[6]  - Simular o pouso              [12] - Sair do Sistema\n");
    printf("\nOpcao: ");
}

void header(Data *data,int tempo) {
    printf("\n--------------------------------------------------------------------------------\n");
    aviao(data);
    print_clima(tempo);
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

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual, Fila *pousos, int TempoPouso) {
    Voo voo;
    char *codigo;
    bool existe = false;

    header(hora_atual, TempoPouso);
    fflush(stdin);

    do {
        codigo = insere_codigo();

        existe = codigo_existe(esperas,emergencias,pousos,codigo);

        if(existe){
            mensagem_erro("Este codigo ja existe");
        }
    } while (strlen(codigo) != 4 || existe != false || codigo[0] != 'V');

    strcpy(voo.codigo, codigo);
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

void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data *hora_atual, int TempoPouso) {
    Voo voo_removido;
    if(VaziaFila(emergencias)) {
        if(!VaziaFila(esperas)) {
            voo_removido = RetiraFila(esperas);

            voo_removido.horario_chegada = *hora_atual;
            voo_removido.horario_chegada.minuto += TempoPouso;
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

        voo_removido.horario_chegada = *hora_atual;
        voo_removido.horario_chegada.minuto += TempoPouso;
        voo_removido.horario_chegada = verificaHora(voo_removido.horario_chegada);
        *hora_atual = voo_removido.horario_chegada;
    }

    printf("Voo pousou!\n");
    imprimirVoo(voo_removido, true);
    InsereFila(pousos, voo_removido);
    fimFuncao();
}

void relatorio(Fila *esperas, Fila *emergencias, Data *hora_atual, int TempoPouso) {
    header(hora_atual, TempoPouso);

    mensagem_erro("Voos em estado de emergencia:");
    imprimeFila(emergencias, false);

    printf("\n");

    mensagem_sucesso("Voos em lista de espera:");
    imprimeFila(esperas, false);
    fimFuncao();
}

void simular_voos(Fila*esperas, Fila*emergencias,Data* hora_atual, int minutos_intervalo, int TempoPouso){
    int qtd_de_voos = minutos_intervalo/TempoPouso;
    No *aux= emergencias->ini;
    bool existe_voo=false;

    Data hora_simulada = *hora_atual;

    if(aux != NULL && qtd_de_voos != 0) {
        existe_voo = true;
        printf("\n\n");
        printf("Voos em emergencia\n");
    }
    while(aux != NULL && qtd_de_voos != 0){
        imprimirVoo(aux->voo,false);

        hora_simulada.minuto += TempoPouso;
        hora_simulada = verificaHora(hora_simulada);

        printf("O voo pousaria as: ");
        printData(hora_simulada);
        mensagem_erro("Voo em estado de emergencia");

        qtd_de_voos--;
        aux = aux->prox;
    }
    
    aux = esperas->ini;
    if(aux != NULL && qtd_de_voos != 0) {
        existe_voo = true;
        printf("\n\n");
        printf("Voos em espera\n");
    }

    while(aux != NULL && qtd_de_voos != 0){
        imprimirVoo(aux->voo,false);

        hora_simulada.minuto += TempoPouso;
        hora_simulada = verificaHora(hora_simulada);

        printf("O voo pousaria as: ");
        printData(hora_simulada);
        
        if(check_hora(aux->voo.previsao_chegada, hora_simulada) == 0){
            mensagem_amarela("O voo vai estar atrasado");
        }else{
            mensagem_sucesso("O voo nao vai estar atrasado");
        }

        qtd_de_voos--;
        aux = aux->prox;
    }

    if(!existe_voo) {
        mensagem_erro("Nao existem voos para pousar!");
    }
    fimFuncao();
}

void voos_pousados(Fila *pousos, Data *hora_atual, int TempoPouso) {
    header(hora_atual, TempoPouso);

    printf("Voos que ja pousaram:\n");
    imprimeFila(pousos,true);
    fimFuncao();
}

void proximo_voo(Fila *esperas, Fila *emergencias) {
    if(!VaziaFila(emergencias)) {
        printf("Proximo voo a pousar (estado de emergencia)\n");
        imprimirVoo(emergencias->ini->voo, false);
    }else if(!VaziaFila(esperas)) {
        printf("Proximo voo a pousar:\n");
        imprimirVoo(esperas->ini->voo, false);
    }else{
        mensagem_erro("Nenhum voo na fila de espera!");
    }
    fimFuncao();
}

void busca_voo(Fila *esperas, Fila *emergencias, Fila *pousos) {
    Voo *voo;
    char *codigo;


    fflush(stdin);
    codigo = insere_codigo();

    voo = busca_fila(esperas,codigo);
    if(voo == NULL){
        voo = busca_fila(emergencias,codigo);
    }else{
        imprimirVoo(*voo, false);
        fimFuncao();
        return;
    }

    if(voo == NULL){
        voo = busca_fila(pousos,codigo);
    }else{
        imprimirVoo(*voo, false);
        fimFuncao();
        return;
    }

    if(voo == NULL){
        mensagem_erro("O voo nao existe\n");
    }else{
        imprimirVoo(*voo, false);
        fimFuncao();
        return;
    }
}
char* insere_codigo(){
    char *codigo = malloc(5*sizeof(char));

    do {
        fflush(stdin);
        printf("Insira o codigo do voo (4 caracteres comecando com V):\n");
        scanf("%[^\n]s",codigo);
        fflush(stdin);

        codigo[0] = toupper(codigo[0]);

        if(codigo[0] != 'V') {
            mensagem_erro("O codigo deve comecar com V!");
        }
        
        if (strlen(codigo) != 4) {
            mensagem_erro("O codigo deve ter 4 caracteres!");
        }
    } while (strlen(codigo) != 4 || codigo[0] != 'V');
    return codigo;
}

void alterar_status(Fila *esperas, Fila *emergencias){
    char *codigo;
    bool achou = false; 
    Fila *aux = CriaFila();
    codigo = insere_codigo();

    while (!VaziaFila(esperas)){
        if(strcmp(esperas->ini->voo.codigo, codigo) == 0){
            InsereFila(emergencias, RetiraFila(esperas));
            achou = true;
        }else{
            InsereFila(aux, RetiraFila(esperas));
        }
    }
    
    if(achou){
        mensagem_sucesso("Voo transferido para emergencia com sucesso");
    }else{
        mensagem_erro("Voo nao encontrado ou ja pousou");
    }
    
    if(VaziaFila(esperas)){
        esperas->ini = aux->ini;
        esperas->fim = aux->fim;
    }

    free(aux);
    fimFuncao();
}

void estatisticas(Fila *pousos){
    int p_totais = 0;
    int p_emer_totais = 0;
    int passageiros_totais = 0;
    float passageiros_por_voo = 0;

    No *aux = pousos->ini;

    if(VaziaFila(pousos)){
        mensagem_erro("Nenhum aviao pousou ainda");
    }else{
        while(aux != NULL){
            if(aux->voo.check_hora == -1){
                p_emer_totais++;
            }
            passageiros_totais +=  aux->voo.num_passageiros;
            p_totais++;
            aux = aux->prox;
        }

        passageiros_por_voo = (float)passageiros_totais/p_totais;

        printf("\nQuantidade de pousos feitos: %d\n", p_totais);
        printf("Pousos de emergencia: %d\n", p_emer_totais);
        printf("Passageiros que passaram pelo aeroporto: %d\n", passageiros_totais);
        printf("Passageiros por voo: %.2f\n",passageiros_por_voo);
    }
    fimFuncao();
}

void menuFiltro(Fila *esperas, Fila *emergencias,Fila*pousos, Data *hora_atual, int tempoPouso){
    bool isLate = false;
    bool isEmergency = false;
    bool isWaiting = false;
    bool landed = false;
    Fila *result = CriaFila();
    int select;
    do{
        printf("\n Faca a escolha dos Filtros, quando nao quiser mais filtros digite -1:");
        printf("\n[1]-Voos Atrasados");
        printf("\n[2]-Voos em estado de emergencia");
        printf("\n[3]-Voos em espera para pouso");
        printf("\n[4]-Voos pousados\n");
        scanf("%d",&select);

        switch(select){
            case 1: if(!isEmergency)
                        isLate = true;
                    else
                        mensagem_erro("Nao pode ser de emergencia e atrasado !\n");
                    break;
                    
            case 2: if(!isLate || !isWaiting)
                        isEmergency = true;
                    else
                        mensagem_erro("Nao pode ser de emergencia e atrasado ou emergencia e aguardando\n");
                    break;

            case 3: if(!isEmergency)
                        isWaiting = true;
                    else
                        mensagem_erro("Nao pode ser de emergencia e em espera!\n");
                    break;

            case 4: if(isEmergency && isLate)
                        landed = true;
                    else
                        mensagem_erro("Nao pode ser pouso com emergencia E atraso!\n");
                    break;
        }
        printf("\nBuscar por: \n");
        if(isLate)
            printf("===> Atrasados\n");
        if(isEmergency)
            printf("===> Emergencias\n");
        if(isWaiting)
            printf("===> em Espera\n");
        if(landed)
            printf("===> Pousados\n");

    }while(select != -1 || (!landed && !isWaiting && !isEmergency && !isLate));

    result = buscaFiltro(emergencias,esperas,hora_atual,pousos,isLate,isEmergency,isWaiting,landed,tempoPouso);
    imprimeFila(result,false);

}
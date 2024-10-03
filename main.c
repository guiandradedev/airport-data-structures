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
void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual, Fila *pousos);
void autorizar_pouso(Fila *esperas, Fila *emergencias, Fila *pousos, Data *hora_atual);
void busca_voo(Fila *esperas, Fila *emergencias, Fila *pousos);
void relatorio(Fila *esperas, Fila *emergencias, Data *hora_atual);
void voos_pousados(Fila *pousos, Data *hora_atual);
void proximo_voo(Fila *esperas, Fila *emergencias);
void simular_voos(Fila*esperas, Fila*emergencias,Data* hora_atual, int minutos_intervalo);
char* insere_codigo();
void alterar_status(Fila *esperas, Fila *emergencias);
void estatisticas(Fila *pousos);
void estatisticas(Fila *pousos);
void aviao(Data *data);
void header(Data* data);
void menu();

int main() {
    // Definição de variaveis
    Fila* emergencias = CriaFila();
    Fila* esperas = CriaFila();
    Fila* pousos = CriaFila();
    Data hora_atual;
    int semente, aux, op = 0;

    do {
        header(NULL);
        printf("Para comecar, insira a semente: ");
        scanf("%d", &semente);

        if(semente < 0) {
            mensagem_erro("A semente deve ser um valor inteiro maior que 0!");
        }
    } while(semente < 0);
    srand(semente);
    hora_atual = gerarData(0,0);

    do{
        menu(hora_atual);
        scanf("%d",&op);

        switch (op){
        case 1:
            inserir_voo(esperas, emergencias, rand() % 10, &hora_atual, pousos);
            break;
        case 2:
            autorizar_pouso(esperas,emergencias,pousos, &hora_atual);
            break;
        case 3:
            relatorio(esperas,emergencias, &hora_atual);
            break;
        case 4:
            proximo_voo(esperas, emergencias);
            break;
        case 5:
            voos_pousados(pousos, &hora_atual);
            break;
        case 6:
            printf("Informe o Intervalo de tempo desejado para a simulacao (em minutos): ");
            scanf("%d",&aux);
            simular_voos(esperas,emergencias,&hora_atual,aux);
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
            break;
        default:
            mensagem_erro("Comando incorreto.");
            fimFuncao();
            break;
        }
    }while (op != 10);

    header(&hora_atual);
    mensagem_sucesso("Obrigado por acessar o sistema.");

    liberaFila(emergencias);
    liberaFila(esperas);
    liberaFila(pousos);

    return 0;
} 

// Implementação das funções
void menu(Data hora_atual){
    header(&hora_atual);

    printf("O que deseja fazer?");
    printf("\n[1]- Inserir voo");
    printf("\n[2]- Autorizar pouso");
    printf("\n[3]- Relatorio das aeronaves");
    printf("\n[4]- Imprimir proxima aeronave a pousar");
    printf("\n[5]- Imprimir aeronaves pousadas");
    printf("\n[6]- Simular o pouso dentro de um intervalo de tempo");
    printf("\n[7]- Buscar voo por codigo");
    printf("\n[8]- Alterar estado de um voo (espera->emergengia)");
    printf("\n[9]- Estatisticas do aeroporto");
    printf("\n[10]- Finalizar o sistema");
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

void inserir_voo(Fila *esperas, Fila *emergencias, int emergencia, Data *hora_atual, Fila *pousos) {
    Voo voo;
    char codigo[5];
    bool existe = false;

    header(hora_atual);
    fflush(stdin);

    do {
        printf("Insira o codigo do voo (4 caracteres comecando com V):\n");
        fgets(codigo, sizeof(codigo), stdin);

        existe = codigo_existe(esperas,emergencias,pousos,codigo);

        if(existe){
            mensagem_erro("Este codigo ja existe");
        }

        if (strlen(codigo) != 4) {
            mensagem_erro("O codigo deve ter 4 caracteres!");
        }

        codigo[0] = toupper(codigo[0]);

        if(codigo[0] != 'V') {
            mensagem_erro("O codigo deve comecar com V!");
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
        printf("\n\n");
        printf("Voos em emergencia\n");
    }
    while(aux != NULL && qtd_de_voos != 0){
        imprimirVoo(aux->voo,false);

        hora_simulada.minuto += 10;
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

        hora_simulada.minuto += 10;
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

void voos_pousados(Fila *pousos, Data *hora_atual) {
    header(hora_atual);

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
    fflush(stdin);

    do {
        printf("Insira o codigo do voo (4 caracteres comecando com V):\n");
        fgets(codigo, 5, stdin);

        if (strlen(codigo) != 4) {
            mensagem_erro("O codigo deve ter 4 caracteres!");
        }

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
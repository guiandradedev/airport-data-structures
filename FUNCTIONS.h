#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED 


// Prototipos
void imprimirVoo(Voo voo, bool mostra_check_hora);
int compararHoras(Data hora1, Data hora2, int minutos);
void printData(Data data);
Data setHora(int hora, int minuto);
Data gerarData(int hora_minima, int minuto_minimo);
Data verificaHora(Data horario);
int check_hora(Data previsao, Data chegada);
void mensagem_erro(char *mensagem);
void mensagem_amarela(char* mensagem);
void mensagem_sucesso(char*mensagem);
bool codigo_existe(Fila *emergencias, Fila *esperas, Fila *pousos, char *codigo);
bool existe_fila(Fila *aux_fila, char *codigo);
Voo* busca_fila(Fila *fila, char *codigo);


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
                mensagem_erro("Pouso de emergencia");
            }else if(voo.check_hora == 1){
                mensagem_sucesso("Pouso sem atraso");
            }else{
                mensagem_amarela("Pouso atrasado");
            }
        }  
    }

}

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

int check_hora(Data previsao, Data chegada){
    int previsao_minutos = (previsao.hora * 60) + previsao.minuto;
    int chegada_minutos = (chegada.hora * 60) + chegada.minuto;

    return chegada_minutos <= previsao_minutos + 10 ? 1 : 0;
    // 1 no horario, 0 atrasado
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

void mensagem_amarela(char* mensagem){
    amarelho();
    printf("%s\n", mensagem);
    resetcor();
}

void fimFuncao() {
    fflush(stdin);
    printf("Pressione Enter para continuar...\n");
    getchar();
    system("cls");
}

bool codigo_existe(Fila *emergencias, Fila *esperas, Fila *pousos, char *codigo){
    return existe_fila(emergencias,codigo) || existe_fila(esperas,codigo) || existe_fila(pousos,codigo);
}

bool existe_fila(Fila *aux_fila, char *codigo){
    No *aux = aux_fila->ini;

    while(aux != NULL){
        if(strcmp(codigo, aux->voo.codigo) == 0){
            return true;
        }
        aux = aux->prox;
    }
    return false;
}

Voo* busca_fila(Fila *fila, char *codigo) {
    Fila* aux = CriaFila();
    Voo* voo = NULL;

    while(!VaziaFila(fila)) {
        if(strcmp(fila->ini->voo.codigo, codigo) == 0) {
            voo = &fila->ini->voo;
        }
        InsereFila(aux, RetiraFila(fila));
    }
    if(VaziaFila(fila)){
        fila->ini = aux->ini;
        fila->fim = aux->fim;
    } 
    free(aux);
    return voo;
}

#endif // STRUCTS_H_INCLUDED
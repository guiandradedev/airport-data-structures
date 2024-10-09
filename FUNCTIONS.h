#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED 

#define LARGURA 40
#define TAMANHO_AVIAO 8

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

Fila* buscaFiltro(Fila *emergencias, Fila *esperas,Data *hora_atual, Fila *pousos, bool ehAtrasado, bool ehEmergencia, bool ehEspera, bool ehPousado,int tempoPouso){
    Fila* resultado = CriaFila();
    Fila* filaAuxiliar = CriaFila();
    Data hora_simulada = *hora_atual;
    hora_simulada.minuto += tempoPouso; 
    Voo voo_retirado;

     if((!ehEmergencia && ehAtrasado && (ehPousado^ehEspera))|| (ehEmergencia && ehPousado && !ehEspera && !ehAtrasado)) //existe pelo menos 2 seletores
     {
        if(ehEspera && ehAtrasado){ 
            resultado = buscaAtrasado(esperas,filaAuxiliar,resultado,&hora_simulada);
            return resultado;
        }
        if(ehPousado && ehAtrasado){
            resultado = buscaAtrasado(pousos,filaAuxiliar,resultado,&hora_simulada);
            return resultado;
        }
        if(ehPousado && ehEmergencia){ 
            while(!VaziaFila(pousos)){
                voo_retirado = RetiraFila(pousos);
                InsereFila(filaAuxiliar,voo_retirado);
                if(voo_retirado.check_hora == -1)
                    InsereFila(resultado,voo_retirado);
            }
            pousos->ini = filaAuxiliar->ini;
            pousos->fim = filaAuxiliar->fim;
            free(filaAuxiliar);
            return resultado;
        }

    } else  //apenas 1 
     {
        if(ehEmergencia){       
           resultado->ini = emergencias->ini;
           resultado->fim = emergencias->fim;
           return resultado;
        }
        if(ehAtrasado){
            Fila* aux = CriaFila();
            resultado = concatenaFilas(buscaAtrasado(esperas,filaAuxiliar,resultado,&hora_simulada),
                                        buscaAtrasado(pousos,filaAuxiliar,aux,&hora_simulada));
            liberaFila(aux);

            return resultado;
        
        }
        if(ehEspera){
           resultado->ini = esperas->ini;
           resultado->fim = esperas->fim;
           return resultado;
        }
        if(ehPousado){      
           resultado->ini = pousos->ini;
           resultado->fim = pousos->fim;
           return resultado;
        }
     }
    // return resultado;
}

Fila* buscaAtrasado(Fila* filaDeBusca,Fila* filaAuxiliar, Fila*resultado,Data* hora_simulada){
    Voo voo_retirado;
    while(!VaziaFila(filaDeBusca)){
        voo_retirado = RetiraFila(filaDeBusca);
        if((check_hora(voo_retirado.previsao_chegada,*hora_simulada)== 0) && voo_retirado.check_hora != -1){
            InsereFila(resultado,voo_retirado);
        }
            InsereFila(filaAuxiliar,voo_retirado);
    }
    filaDeBusca->ini = filaAuxiliar->ini;
    filaDeBusca->fim = filaAuxiliar->fim;
    free(filaAuxiliar);
    return resultado;
}

void animacao(){
    system("color 1F");

    for (int i = 0; i < LARGURA; i++) {
    printf("\033[H\033[J");
    
    int linhas_visiveis = TAMANHO_AVIAO;
    if (i > LARGURA - TAMANHO_AVIAO) {
        linhas_visiveis = TAMANHO_AVIAO - (i - (LARGURA - TAMANHO_AVIAO));
    }
    
    desenharAviao(i, linhas_visiveis);
    fflush(stdout);
    usleep(100000);
    }
    system("color 07");

}

void desenharAviao(int espacos, int linhas_visiveis) {
    // Desenha o avião com um deslocamento

    if (linhas_visiveis >= 1) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("            ______\n");
    }
    if (linhas_visiveis >= 2) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("            _\\ _~-\\___\n");
    }
    if (linhas_visiveis >= 3) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("    =  = ==(____AA____D      BlueSky\n");
    }
    if (linhas_visiveis >= 4) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("                \\_____\\___________________,-~~~~~~~`-.._\n");
    }
    if (linhas_visiveis >= 5) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("                /     o O o o o o O O o o o o o o O o  |\\_\n");
    }
    if (linhas_visiveis >= 6) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("                `~-.__        ___..----..                  )\n");
    }
    if (linhas_visiveis >= 7) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("                      `---~~\\___________/------------`````\n");
    }
    if (linhas_visiveis >= 8) {
        for (int i = 0; i < espacos; i++) printf(" ");
        printf("                      =  ===(_________D\n");
    }
    
}

#endif // STRUCTS_H_INCLUDED
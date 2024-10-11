#ifndef PROTOTIPOS_H_INCLUDED
#define PROTOTIPOS_H_INCLUDED 

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
void animacao();
void desenharAviao(int espacos, int linhas_visiveis);
Fila* buscaFiltro(Fila *emergencias, Fila *esperas,Data *hora_atual, Fila *pousos, bool ehAtrasado, bool ehEmergencia, bool ehEspera, bool ehPousado,int tempoPouso);
Fila* buscaAtrasado(Fila* filaDeBusca,Data* hora_simulada);

#endif // STRUCTS_H_INCLUDED
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
    Data horario_chegada;
} Voo;

typedef struct no
{
    Voo voo;
    struct no *prox;
}No;

typedef struct fila
{
    No * ini;
    No * fim;
} Fila;

#endif // STRUCTS_H_INCLUDED
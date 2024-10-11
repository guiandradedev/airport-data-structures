#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED

/* FUN��ES DE MANIPULA��O DE PFILA

Fila* CriaFila()  CRIA A FILA

int VaziaFila (Fila* f) VERIFICA SE A FILA EST� VAIZA

void InsereFila (Fila* f, int v) INSER��O

int RetiraFila (Fila* f) REMO��O

Fila* liberaFila (Fila* f) LIBERA A FILA

void imprimeFila (Fila* f, int check_hora)IMPRIME A FILA
*/

int VaziaFila (Fila* f)
{
    if (f->ini==NULL) return 1;
    return 0;
}


Fila* CriaFila ()
{
    Fila* f = (Fila*) malloc(sizeof(Fila));
    f->ini = f->fim = NULL;
    return f;
}

No* ins_fim (No *fim, Voo A)
{
    No *p = (No*)malloc(sizeof(No));
    p->voo = A;
    p->prox = NULL;
    if (fim != NULL) /* verifica se lista n�o estava vazia */
    fim->prox = p;
    return p;
}

void InsereFila (Fila* f, Voo v)
{
    f->fim = ins_fim(f->fim,v);
    if (f->ini==NULL) /* fila antes vazia? */
    f->ini = f->fim;
}

No* retira_ini (No* ini)
{
    No* p = ini->prox;
    free(ini);
    return p;
}

Voo RetiraFila (Fila* f)
{
    Voo v;
    if (VaziaFila(f))
    {
        printf("Fila vazia.\n");
        exit(0); /* aborta programa */
    }
    v = f->ini->voo;
    f->ini = retira_ini(f->ini);
    if (f->ini == NULL) /* fila ficou vazia? */
    f->fim = NULL;
    return v;
}

void imprimeFila (Fila* f, bool mostra_check_hora)
{
    No* q;
    printf("\n");
    if(VaziaFila(f)){
        mensagem_erro("Nao existem voos nessa fila");
        
    }else{
        for (q=f->ini; q!=NULL; q=q->prox)
        {
            imprimirVoo(q->voo,mostra_check_hora);
        }
        printf("\n");
    }
}


Fila* liberaFila (Fila* f)
{
    No* q = f->ini;
    while (q!=NULL)
    {
        No* t = q->prox;
        free(q);
        q = t;
    }
    free(f);
    return NULL;
}

Fila* concatenaFilas(Fila* f1, Fila*f2){
    Fila* filaResultado = CriaFila(); 
    No *aux;
    // printf("\n\n\tTESTE1\n\n");
    // imprimeFila(f1,false);
    // imprimeFila(f2,false);
    aux=f1->ini;
    while(aux!=NULL)
    {
        InsereFila(filaResultado,aux->voo);
        aux=aux->prox;
    }
    aux=f2->ini;
    while(aux!=NULL)
    {
        InsereFila(filaResultado,aux->voo);
        aux=aux->prox;
    }
    // printf("\n\n\tTESTE\n\n");
    //imprimeFila(filaResultado,false);
    /*Fila* fila_aux = CriaFila();
    Voo voo_aux;
    while(!VaziaFila(f1)) {
        voo_aux = RetiraFila(f1);
        InsereFila(filaResultado, voo_aux);
        InsereFila(fila_aux, voo_aux);
    }

    f1->ini = fila_aux->ini;
    f1->fim = fila_aux->fim;*/

    //liberaFila(fila_aux);

   // filaResultado->fim->prox = f2->ini;
   // filaResultado->fim = f2->fim;

    return filaResultado;
}

#endif // FILA_H_INCLUDED

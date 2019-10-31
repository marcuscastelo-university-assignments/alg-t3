#include <stdlib.h>
#include "colecao.h"

struct _no {
    int valor;
    struct _no* esq;
    struct _no* dir;
    int altura;
} ;

struct _c {
    No* inicio;
    int estrutura_id;
};

Colecao* cria_colecao(int estrutura_id) 
{
    Colecao *colecao = (Colecao*) malloc(sizeof(Colecao));
    colecao -> estrutura_id = estrutura_id;
    colecao -> inicio = NULL;
    return colecao;
}

No* cria_no(int valor)
{
    No *no = (No*) malloc(sizeof(No));
    no -> valor = valor;
    no -> altura = 0;
    no -> dir = NULL;
    no -> esq = NULL;
    return no;
}

void adiciona(Colecao* c, int valor)
{
    // Implementar
}

int existe(Colecao* c, int valor)
{
    // Implementar
    return 1;
}

void _destroi_no_recursivo(No *no) {
    if (no == NULL) return;

    _destroi_no_recursivo(no -> esq);
    _destroi_no_recursivo(no -> dir);

    free(no);
}

void destroi(Colecao* c)
{
    if (c == NULL) return;

    _destroi_no_recursivo(c -> inicio);
    free(c);
}
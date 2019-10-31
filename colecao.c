#include <stdlib.h>
#include "colecao.h"

typedef enum {
    ID_LISTA_ORDENADA = 1, ID_LISTA_ULTIMA, ID_LISTA_PRIMEIRA, ID_ABB, ID_AVL
} ESTRUTURA_ID;

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

void _adiciona_ordenada(Colecao *c, int valor) {
    if (c->inicio == NULL) {
        c -> inicio = cria_no(valor);
        return;
    }

    if (c -> inicio -> valor > valor) {
        c -> inicio -> esq = cria_no(valor);
        c -> inicio -> esq -> dir = c -> inicio;
        c -> inicio = c -> inicio -> esq; 
        return;
    }

    #define noNavegadorDir (noNavegador->dir)

    No *noNavegador = c -> inicio;
    while (noNavegadorDir != NULL && noNavegadorDir -> valor < valor) noNavegador = noNavegadorDir;
    No *novoNo = cria_no(valor);

    novoNo -> dir = noNavegadorDir;
    novoNo -> esq = noNavegador;

    novoNo -> dir -> esq = novoNo;
    novoNo -> esq -> dir = novoNo;
}

void _adiciona_inicio(Colecao *c, int valor) {

}

void _adiciona_fim(Colecao *c, int valor) {

}

void adiciona(Colecao* c, int valor)
{
    switch (c -> estrutura_id)
    {
    case ID_LISTA_ORDENADA:
        _adiciona_ordenada(c, valor);
        break;
    
    default:
        break;
    }
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
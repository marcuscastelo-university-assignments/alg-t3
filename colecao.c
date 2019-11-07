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
};

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
    if (c -> inicio == NULL) {
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

    if (noNavegadorDir != NULL)
        novoNo -> dir -> esq = novoNo;
    novoNo -> esq -> dir = novoNo;
    #undef noNavegadorDir
}

void _adiciona_inicio(Colecao *c, int valor) {
    if (c->inicio == NULL) {
        c->inicio = cria_no(valor);
        return;
    }

    c -> inicio -> esq = cria_no(valor);
    c -> inicio -> esq -> dir = c -> inicio;
    c -> inicio = c -> inicio -> esq;
}

void _adiciona_fim(Colecao *c, int valor) {
    if (c -> inicio == NULL) {
        c -> inicio = cria_no(valor);
        return;
    }

    No *fim = c -> inicio;
    while (fim -> dir != NULL) fim = fim->dir;
    fim -> dir = cria_no(valor);
    fim -> dir -> esq = fim;
}

void _adiciona_abb(Colecao * c, int valor) {
    if (c->inicio == NULL) {
        c->inicio = cria_no(valor);
        return;
    }

    percorre_e_adiciona_abb(c->inicio, valor);

    return;
}

void percorre_e_adiciona_abb(No * noAtual, int valor) {
    if (valor > noAtual->valor) { // será inserido no filho a direita (ABB)!
        if (noAtual->dir == NULL) {
            noAtual->dir = cria_no(valor);
            noAtual->dir->altura = noAtual->altura + 1;
            return;
        }
        else {
            percorre_e_adiciona_abb(noAtual->dir, valor); // chama a função recursiva, "entrando" no filho à direita
        }
    }
    else { // valor <= noAtual->valor --- será inserido no filho a esquerda (ABB)!
        if (noAtual->esq == NULL) {
            noAtual->esq = cria_no(valor);
            noAtual->esq->altura = noAtual->altura + 1;
            return;
        }
        else {
            percorre_e_adiciona_abb(noAtual->esq, valor); // chama a função recursiva, "entrando" no filho à esquerda
        }
    }

    return;
}

void adiciona(Colecao* c, int valor)
{
    switch (c -> estrutura_id)
    {
        case ID_LISTA_ORDENADA:
            _adiciona_ordenada(c, valor);
            break;
        case ID_LISTA_PRIMEIRA:
            _adiciona_inicio(c, valor);
            break;
        case ID_LISTA_ULTIMA:
            _adiciona_fim(c,valor);
            break;
        case ID_ABB:
            _adiciona_abb(c, valor);
            break;
        case ID_AVL: //TODO: implementar adiciona_avl
            break;
    }
}

int _existe_ordenada(Colecao *c, int valor) {
    No *noNavegador = c->inicio;
    while (noNavegador != NULL && noNavegador -> valor < valor) noNavegador = noNavegador -> dir;
    if (noNavegador != NULL && noNavegador -> valor == valor) return 1;
    return 0; 
}

int _existe_desordenada(Colecao *c, int valor) {
    No *noNavegador = c->inicio;
    while (noNavegador != NULL) {
        if (noNavegador -> valor == valor) return 1;
        noNavegador = noNavegador -> dir;
    } 
    return 0;
}

int existe_abb(Colecao * c, int valor) {
    if (c == NULL) return 0; // não existe, pois a coleção não está definida
    if (c->inicio == NULL) return 0; // não existe, pois a raiz não está definida (não há árvore)

    return existe_recursiva_abb(c->inicio, valor);
}

int existe_recursiva_abb(No * noAtual, int valor) {
    if (noAtual == NULL) return 0; // valor não encotrado

    if (noAtual->valor == valor) return 1; // valor encontrado

    if (noAtual->valor > valor)
        return existe_recursiva_abb(noAtual->dir, valor);
    else // noAtual->valor <= valor
        return existe_recursiva_abb(noAtual->esq, valor);
}


int existe(Colecao* c, int valor)
{
    switch (c->estrutura_id)
    {
        case ID_LISTA_ORDENADA:
            return _existe_ordenada(c, valor);
        case ID_LISTA_PRIMEIRA:
        case ID_LISTA_ULTIMA:
            return _existe_desordenada(c, valor);
        case ID_ABB:
            existe_abb(c, valor);
            return 0;
        case ID_AVL:
            return 0; //TODO: implementar existe_avl
        default:
            return 0;
    }
}

void _destroi_no_recursivo_arvore(No * no) {
    if (no == NULL) return;

    _destroi_no_recursivo_arvore(no->esq);
    _destroi_no_recursivo_arvore(no->dir);

    free(no);

    return;
}


void _destroi_lista(No *no) {
    if (no == NULL) return;
    while (no -> dir != NULL) {
        no = no -> dir;
        no -> esq -> dir = NULL;
        no -> esq -> esq = NULL;
        free(no -> esq);
    }
    no -> esq = NULL;
    free (no);
}
void destroi(Colecao* c)
{
    if (c == NULL) return;

    switch (c -> estrutura_id)
    {
    case ID_LISTA_ORDENADA:
    case ID_LISTA_PRIMEIRA:
    case ID_LISTA_ULTIMA:
        _destroi_lista(c -> inicio);
        break;
    case ID_ABB:
         _destroi_no_recursivo_arvore(c->inicio);
         break;
    
    default:
        break;
    }
    free(c);
}
#include <stdlib.h>
#include "colecao.h"

//TODO: comment subroutine functions (starting by _)

/**
 * 
 *  IMPLEMENTAÇÃO DA BIBLIOTECA COLEÇÃO (Estruturas: Lista desordenada, Lista ordenada, Árvore Binária de Busca, AVL)
 * 
 *  Trabalho 3 de Algoritmos e Estruturas de Dados I (2019)
 *  USP - São Carlos - ICMC
 *  
 *  Alunos:
 *      Marcus Vinicius C. B. Martins   nUSP: 11219237
 *      Pedro Guerra Lourenço           nUSP: 11218747
 */


/**
 *  Enum privado, de uso interno.
 *  Representa os possíveis tipos de coleção.
 */
typedef enum {
    ID_LISTA_ORDENADA = 1, ID_LISTA_ULTIMA, ID_LISTA_PRIMEIRA, ID_ABB, ID_AVL
} ESTRUTURA_ID;

/**
 * Definindo uma função que retorna o maior valor dentre dois
 */
#define max(a, b) ((a > b) ? a : b)

/**
 *  Struct privada (Tipo incompleto público)
 *  Membros:
 *      int valor       -> valor a ser armazanado na estrutura selecionada (ver struct _c)
 *      struct _no *esq -> pointer do (filho/elemento) à esquerda (Árvore/Lista)
 *      struct _no *dir -> pointer do (filho/elemento) à direita  (Árvore/Lista)
 *      int altura      -> altura da árvore (ou subárvore). [No caso de listas, vale 0]
 */
struct _no {
    int valor;
    struct _no* esq;
    struct _no* dir;
    int altura;
};

/**
 *  Estrutura privada (Tipo incompleto público)
 *  Membros:
 *      No *inicio          -> primeiro nó da estrutura (Lista ou Árvore)
 *      int estrutura_id    -> correspondente ao enum ESTRUTURA_ID (determina qual estrutura de dados será usada)
 */
struct _c {
    No* inicio;
    int estrutura_id;
};

/**
 *  Função pública do TAD Coleção.
 *  Objetivo: Alocar memória e definir valores padrões
 *  de inicialização de uma nova estrutura.
 *  
 *  Parâmetros: 
 *      int estrutura_id -> representa qual das estruturas de dados será utilizada [ver enum ESTRUTURA_ID]
 * 
 *  Retorno:
 *      Colecao* -> pointer para a nova coleção criada 
 */
Colecao* cria_colecao(int estrutura_id) 
{
    Colecao *colecao = (Colecao*) malloc(sizeof(Colecao));
    colecao -> estrutura_id = estrutura_id;
    colecao -> inicio = NULL;
    return colecao;
}

/**
 *  Função pública do TAD Coleção
 *  Objetivo: Alocar memória e definir valores padrões
 *  de inicialização de um nó.
 * 
 *  Parâmetros:
 *      int valor -> valor que será atribuído ao nó
 * 
 *  Retorno:
 *      No* -> pointer para o nó criado
 */
No* cria_no(int valor)
{
    No *no = (No*) malloc(sizeof(No));

    if (no == NULL) return NULL;

    no -> valor = valor;
    no -> altura = 0;
    no -> dir = NULL;
    no -> esq = NULL;

    return no;
}


/************************************
 * FUNÇÕES DE INSERÇÃO E EXISTÊNCIA *
 ************************************/
#pragma region Lista Ordenada

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

int _existe_ordenada(Colecao *c, int valor) {
    No *noNavegador = c->inicio;
    while (noNavegador != NULL && noNavegador -> valor < valor) noNavegador = noNavegador -> dir;
    if (noNavegador != NULL && noNavegador -> valor == valor) return 1;
    return 0; 
}

#pragma endregion

#pragma region Lista Desordenada (Inicio e Fim)

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

int _existe_desordenada(Colecao *c, int valor) {
    No *noNavegador = c->inicio;
    while (noNavegador != NULL) {
        if (noNavegador -> valor == valor) return 1;
        noNavegador = noNavegador -> dir;
    } 
    return 0;
}

#pragma endregion

#pragma region Arvore Binaria de Busca (ABB)

No * _adiciona_abb_recursiva(No * noAtual, int valor) {
    if (noAtual == NULL) return cria_no(valor); 
  
    if (valor < noAtual->valor) 
        noAtual->esq  = _adiciona_abb_recursiva(noAtual->esq, valor); 
    else if (valor > noAtual->valor) 
        noAtual->dir = _adiciona_abb_recursiva(noAtual->dir, valor);    
  
    return noAtual; 
}

void _adiciona_abb(Colecao * c, int valor) {
    if (c == NULL) return;
    if (c->inicio == NULL) return;

    _adiciona_abb_recursiva(c->inicio, valor);
}

int _existe_abb_recursiva(No * noAtual, int valorProcurado) {
    if (noAtual->valor == valorProcurado) return 1; // valor encontrado (1 = verdadeiro)

    if (valorProcurado < noAtual->valor) {
        if (noAtual->esq != NULL)
            return _existe_abb_recursiva(noAtual->esq, valorProcurado);
    }
    else {
        if (noAtual->dir != NULL)
            return _existe_abb_recursiva(noAtual->dir, valorProcurado);
    }

    return 0;
}

int _existe_abb(Colecao * c, int valor) {
    if (c == NULL) return 0;
    if (c->inicio == NULL) return 0;

    _existe_abb_recursiva(c->inicio, valor);
}

#pragma endregion

#pragma region AVL

// Funções auxiliares (de rotação)
No * rotacao_direita(No *a) {
    No * b = a->esq;

    a->esq = b->dir;
    b->dir = a;

    a->altura = max(avl_altura_no(a->esq), avl_altura_no(a->dir)) + 1;
    b->altura = max(avl_altura_no(b->esq), a->altura) + 1;

    return b;
}

No * rotacao_esquerda(No * a) {
    No * b = a->dir;

    a->dir = b->esq;
    b->esq = a;

    a->altura = max(avl_altura_no(a->esq), avl_altura_no(a->dir)) + 1;
    b->altura = max(avl_altura_no(b->dir), a->altura) + 1;

    return b;
}

No * rodatacao_esquerda_direita(No * a) {
    a->esq = rodar_esquerda(a->esq);

    return rodar_direita(a);
}

No * rodatacao_direita_esquerda(No * a) {
    a->dir = rodar_direita(a->dir);
    
    return rodar_esquerda(a);
}

No * _adiciona_avl_recursiva(No * noAtual, int valor) {
    if (noAtual == NULL) return cria_no(valor);
  
    if (valor < noAtual->valor) 
        noAtual->esq  = _adiciona_avl_recursiva(noAtual->esq, valor); 
    else if (valor > noAtual->valor) 
        noAtual->dir = _adiciona_avl_recursiva(noAtual->dir, valor);    
  
    return noAtual; 
}

void _adiciona_avl(Colecao * c, int valor) {
    if (c == NULL) return;
    if (c->inicio == NULL) return;

    _adiciona_avl_recursiva(c->inicio, valor);
}

int _existe_avl_recursiva(No * noAtual, int valorProcurado) {
    if (noAtual->valor == valorProcurado) return 1; // valor encontrado (1 = verdadeiro)

    if (valorProcurado < noAtual->valor) {
        if (noAtual->esq != NULL)
            return _existe_avl_recursiva(noAtual->esq, valorProcurado);
    }
    else {
        if (noAtual->dir != NULL)
            return _existe_avl_recursiva(noAtual->dir, valorProcurado);
    }

    return 0;
}

int _existe_avl(Colecao * c, int valor) {
    if (c == NULL) return 0;
    if (c->inicio == NULL) return 0;

    _existe_avl_recursiva(c->inicio, valor);
}

#pragma endregion


/**
 *  Função pública do TAD Coleção
 *  Objetivo: inserir elementos na coleção especificada
 *  
 *  Parâmetros:
 *      Colecao *c - pointer para o TAD coleção em que a inserção será realizada
 *      int valor  - valor a ser inserido
 *  Retorno: void
 */ 
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
        // case ID_AVL:
            // _adiciona_avl(c, valor);
            break;
    }
}


/**
 *  Função pública do TAD Coleção
 *  Objetivo: checar a existência de um certo elemento 
 *  na coleção especificada.
 * 
 *  Parâmetros:
 *      Colecao *c - pointer para o TAD coleção em que a busca será realizada
 *      int valor  - valor a ser buscado
 *  Retorno: 
 *      int -> 0: não existe, 1: existe
 */
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
            return _existe_abb(c, valor);
        // case ID_AVL:
            // return _existe_avl(c, valor);
        default:
            return 0;
    }
}

/*********************************
 * FUNÇÕES DE LIMPEZA DE MEMÓRIA *
 *********************************/

#pragma region Arvores (ABB e AVL)
void _destroi_no_arvore_recursiva(No * no) {
    if (no == NULL) return;

    _destroi_no_arvore_recursiva(no->esq);
    _destroi_no_arvore_recursiva(no->dir);

    free(no);

    return;
}
#pragma endregion

#pragma region Listas (Ordenada e Desordenada)
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
#pragma endregion

/**
 *  Função pública do TAD Coleção
 *  Objetivo: liberar toda a memória ocupada pela coleção
 * 
 *  Parâmetros: 
 *      Colecao *c - pointer para o TAD que se deseja liberar
 * 
 *  Retorno: void
 */
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
    case ID_AVL:
         _destroi_no_arvore_recursiva(c->inicio);
         break;
    default:
        break;
    }
    free(c);
}
#include <stdlib.h>
#include "colecao.h"


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
        case ID_AVL: //TODO: implementar adiciona_avl
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
            existe_abb(c, valor);
            return 0;
        case ID_AVL:
            return 0; //TODO: implementar existe_avl
        default:
            return 0;
    }
}

/*********************************
 * FUNÇÕES DE LIMPEZA DE MEMÓRIA *
 *********************************/

#pragma region Arvores (ABB e AVL)
void _destroi_no_recursivo_arvore(No * no) {
    if (no == NULL) return;

    _destroi_no_recursivo_arvore(no->esq);
    _destroi_no_recursivo_arvore(no->dir);

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
        //Uso da técnica de fall-through (os três primeiros casos chamam a mesma função)
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
#include <stdlib.h>
#include <stdio.h>

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    int valor;
    int altura;
} No;

typedef struct arvore {
    struct no* raiz;
} Arvore;

// Protótipos
void balanceamento(Arvore*, No*);
int altura(No*);
int fb(No*);
No* rsd(Arvore*, No*);
No* rse(Arvore*, No*);
No* rdd(Arvore*, No*);
No* rde(Arvore*, No*);
No* criarNo(No*, int);
No* localizar(No*, int);
void percorrer(No*, void (*callback)(int));
No* encontrarMinimo(No*);
void removerNo(Arvore*, No*);
void remover(Arvore*, int);

// Funções da árvore AVL
Arvore* criar() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->raiz = NULL;
    return arvore;
}

int vazia(Arvore* arvore) {
    return arvore->raiz == NULL;
}

No* adicionarNo(No* no, int valor) {
    if (valor > no->valor) {
        if (no->direita == NULL) {
            No* novo = criarNo(no, valor);
            no->direita = novo;
            return novo;
        } else {
            return adicionarNo(no->direita, valor);
        }
    } else {
        if (no->esquerda == NULL) {
            No* novo = criarNo(no, valor);
            no->esquerda = novo;
            return novo;
        } else {
            return adicionarNo(no->esquerda, valor);
        }
    }
}

No* adicionar(Arvore* arvore, int valor) {
    if (vazia(arvore)) {
        arvore->raiz = criarNo(NULL, valor);
        return arvore->raiz;
    } else {
        No* no = adicionarNo(arvore->raiz, valor);
        balanceamento(arvore, no->pai);
        return no;
    }
}

No* criarNo(No* pai, int valor) {
    No* no = malloc(sizeof(No));
    no->valor = valor;
    no->pai = pai;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1; // Altura inicial do nó
    return no;
}

No* localizar(No* no, int valor) {
    if (no == NULL || no->valor == valor) {
        return no;
    }
    if (valor < no->valor) {
        return localizar(no->esquerda, valor);
    } else {
        return localizar(no->direita, valor);
    }
}

void percorrer(No* no, void (*callback)(int)) {
    if (no != NULL) {
        callback(no->valor); 
        percorrer(no->esquerda, callback);
        percorrer(no->direita, callback);
    }
}

void visitar(int valor) {
    printf("%d ", valor);
}

void balanceamento(Arvore* arvore, No* no) {
    while (no != NULL) {
        no->altura = (altura(no->esquerda) > altura(no->direita) ? altura(no->esquerda) : altura(no->direita)) + 1;

        int fator = fb(no);
        if (fator > 1) {
            if (fb(no->esquerda) >= 0) {
                rsd(arvore, no);
            } else {
                rdd(arvore, no);
            }
        } else if (fator < -1) {
            if (fb(no->direita) <= 0) {
                rse(arvore, no);
            } else {
                rde(arvore, no);
            }
        }
        no = no->pai;
    }
}

int altura(No* no) {
    return no == NULL ? 0 : no->altura;
}

int fb(No* no) {
    return altura(no->esquerda) - altura(no->direita);
}

No* rse(Arvore* arvore, No* no) {
    No* direita = no->direita;
    no->direita = direita->esquerda;
    if (direita->esquerda) direita->esquerda->pai = no;
    direita->pai = no->pai;
    if (no->pai == NULL) {
        arvore->raiz = direita;
    } else if (no->pai->esquerda == no) {
        no->pai->esquerda = direita;
    } else {
        no->pai->direita = direita;
    }
    direita->esquerda = no;
    no->pai = direita;

    no->altura = (altura(no->esquerda) > altura(no->direita) ? altura(no->esquerda) : altura(no->direita)) + 1;
    direita->altura = (altura(direita->esquerda) > altura(direita->direita) ? altura(direita->esquerda) : altura(direita->direita)) + 1;
    return direita;
}

No* rsd(Arvore* arvore, No* no) {
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    if (esquerda->direita) esquerda->direita->pai = no;
    esquerda->pai = no->pai;
    if (no->pai == NULL) {
        arvore->raiz = esquerda;
    } else if (no->pai->esquerda == no) {
        no->pai->esquerda = esquerda;
    } else {
        no->pai->direita = esquerda;
    }
    esquerda->direita = no;
    no->pai = esquerda;

    no->altura = (altura(no->esquerda) > altura(no->direita) ? altura(no->esquerda) : altura(no->direita)) + 1;
    esquerda->altura = (altura(esquerda->esquerda) > altura(esquerda->direita) ? altura(esquerda->esquerda) : altura(esquerda->direita)) + 1;
    return esquerda;
}

No* rdd(Arvore* arvore, No* no) {
    no->esquerda = rse(arvore, no->esquerda);
    return rsd(arvore, no);
}

No* rde(Arvore* arvore, No* no) {
    no->direita = rsd(arvore, no->direita);
    return rse(arvore, no);
}

No* encontrarMinimo(No* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

void removerNo(Arvore* arvore, No* no) {
    if (no == NULL) return;

    No* substituto;
    if (no->esquerda == NULL && no->direita == NULL) {
        substituto = NULL;
    } else if (no->esquerda == NULL) {
        substituto = no->direita;
    } else if (no->direita == NULL) {
        substituto = no->esquerda;
    } else {
        substituto = encontrarMinimo(no->direita);
        no->valor = substituto->valor;
        removerNo(arvore, substituto);
        return;
    }

    if (substituto != NULL) {
        substituto->pai = no->pai;
    }
    if (no->pai == NULL) {
        arvore->raiz = substituto;
    } else if (no->pai->esquerda == no) {
        no->pai->esquerda = substituto;
    } else {
        no->pai->direita = substituto;
    }

    free(no);
    balanceamento(arvore, substituto != NULL ? substituto->pai : NULL);
}

void remover(Arvore* arvore, int valor) {
    No* no = localizar(arvore->raiz, valor);
    if (no != NULL) {
        removerNo(arvore, no);
    }
}

int main() {
    Arvore* a = criar();

    adicionar(a, 1);
    adicionar(a, 2);
    adicionar(a, 3);
    adicionar(a, 4);
    adicionar(a, 5);
    adicionar(a, 6);

    printf("In-order antes da remoção: ");
    percorrer(a->raiz, visitar);
    printf("\n");

    remover(a, 2);
    printf("In-order depois da remoção: ");
    percorrer(a->raiz, visitar);
    printf("\n");

    return 0;
}


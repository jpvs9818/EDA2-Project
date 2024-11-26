#include <stdlib.h>
#include <stdio.h>

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    Cor cor;
    int valor;
} No;

typedef struct arvore {
    struct no* raiz;
    struct no* nulo; 
} Arvore;

No* criarNo(Arvore*, No*, int);
void balancear(Arvore*, No*);
void rotacionarEsquerda(Arvore*, No*);
void rotacionarDireita(Arvore*, No*);

Arvore* criar();
int vazia(Arvore*);
No* adicionar(Arvore*, int);
No* localizar(Arvore* arvore, int valor);

Arvore* criar() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->nulo = criarNo(arvore, NULL, 0);  // Criar um nó "nulo"
    arvore->nulo->cor = Preto;  // Definindo a cor do nó nulo como Preto
    arvore->raiz = arvore->nulo;

    return arvore;
}

int vazia(Arvore* arvore) {
    return arvore->raiz == arvore->nulo;
}

No* minimo(Arvore* arvore, No* no) {
    while (no->esquerda != arvore->nulo) {
        no = no->esquerda;
    }
    return no;
}

No* criarNo(Arvore* arvore, No* pai, int valor) {
    No* no = malloc(sizeof(No));

    no->pai = pai;
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;
    no->cor = Vermelho; // Cor padrão para novos nós

    return no;
}

No* adicionarNo(Arvore* arvore, No* no, int valor) {
    if (valor > no->valor) {
        if (no->direita == arvore->nulo) {
            no->direita = criarNo(arvore, no, valor);
            return no->direita;
        } else {
            return adicionarNo(arvore, no->direita, valor);
        }
    } else {
        if (no->esquerda == arvore->nulo) {
            no->esquerda = criarNo(arvore, no, valor);
            return no->esquerda;
        } else {
            return adicionarNo(arvore, no->esquerda, valor);
        }
    }
}

No* adicionar(Arvore* arvore, int valor) {
    if (vazia(arvore)) {
        arvore->raiz = criarNo(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto;
        return arvore->raiz;
    } else {
        No* no = adicionarNo(arvore, arvore->raiz, valor);
        balancear(arvore, no);
        return no;
    }
}

No* localizar(Arvore* arvore, int valor) {
    if (!vazia(arvore)) {
        No* no = arvore->raiz;
        while (no != arvore->nulo) {
            if (no->valor == valor) {
                return no;
            } else {
                no = valor < no->valor ? no->esquerda : no->direita;
            }
        }
    }
    return NULL;
}

void percorrerProfundidadeInOrder(Arvore* arvore, No* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        percorrerProfundidadeInOrder(arvore, no->esquerda, callback);
        callback(no->valor);
        percorrerProfundidadeInOrder(arvore, no->direita, callback);
    }
}

void percorrerProfundidadePreOrder(Arvore* arvore, No* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        callback(no->valor);
        percorrerProfundidadePreOrder(arvore, no->esquerda, callback);
        percorrerProfundidadePreOrder(arvore, no->direita, callback);
    }
}

void percorrerProfundidadePosOrder(Arvore* arvore, No* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        percorrerProfundidadePosOrder(arvore, no->esquerda, callback);
        percorrerProfundidadePosOrder(arvore, no->direita, callback);
        callback(no->valor);
    }
}

void visitar(int valor) {
    printf("%d ", valor);
}

void balancear(Arvore* arvore, No* no) {
    while (no->pai->cor == Vermelho) {
        if (no->pai == no->pai->pai->esquerda) {
            No *tio = no->pai->pai->direita;

            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    rotacionarEsquerda(arvore, no);
                } else {
                    no->pai->cor = Preto;
                    no->pai->pai->cor = Vermelho;
                    rotacionarDireita(arvore, no->pai->pai);
                }
            }
        } else {
            No *tio = no->pai->pai->esquerda;

            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    rotacionarDireita(arvore, no);
                } else {
                    no->pai->cor = Preto;
                    no->pai->pai->cor = Vermelho;
                    rotacionarEsquerda(arvore, no->pai->pai);
                }
            }
        }
    }
    arvore->raiz->cor = Preto;
}

void rotacionarEsquerda(Arvore* arvore, No* no) {
    No* direita = no->direita;
    no->direita = direita->esquerda;
    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = no;
    }
    direita->pai = no->pai;

    if (no->pai == arvore->nulo) {
        arvore->raiz = direita;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = direita;
    } else {
        no->pai->direita = direita;
    }

    direita->esquerda = no;
    no->pai = direita;
}

void rotacionarDireita(Arvore* arvore, No* no) {
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;

    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = no;
    }

    esquerda->pai = no->pai;

    if (no->pai == arvore->nulo) {
        arvore->raiz = esquerda;
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = esquerda;
    } else {
        no->pai->direita = esquerda;
    }

    esquerda->direita = no;
    no->pai = esquerda;
}

void removerBalancear(Arvore* arvore, No* no);
void substituirNo(Arvore* arvore, No* antigo, No* novo);

void remover(Arvore* arvore, int valor) {
    No* no = localizar(arvore, valor);
    if (!no || no == arvore->nulo) return; // Node not found

    No* y = no; // Node to be removed or moved
    No* x;      // Node that replaces y
    Cor corOriginal = y->cor;

    if (no->esquerda == arvore->nulo) {
        x = no->direita;
        substituirNo(arvore, no, no->direita);
    } else if (no->direita == arvore->nulo) {
        x = no->esquerda;
        substituirNo(arvore, no, no->esquerda);
    } else {
        y = minimo(arvore, no->direita);
        corOriginal = y->cor;
        x = y->direita;

        if (y->pai == no) {
            x->pai = y;
        } else {
            substituirNo(arvore, y, y->direita);
            y->direita = no->direita;
            y->direita->pai = y;
        }

        substituirNo(arvore, no, y);
        y->esquerda = no->esquerda;
        y->esquerda->pai = y;
        y->cor = no->cor;
    }

    free(no); // Free the removed node
    if (corOriginal == Preto) {
        removerBalancear(arvore, x);
    }
}

void substituirNo(Arvore* arvore, No* antigo, No* novo) {
    if (antigo->pai == arvore->nulo) {
        arvore->raiz = novo;
    } else if (antigo == antigo->pai->esquerda) {
        antigo->pai->esquerda = novo;
    } else {
        antigo->pai->direita = novo;
    }
    novo->pai = antigo->pai;
}

void removerBalancear(Arvore* arvore, No* no) {
    while (no != arvore->raiz && no->cor == Preto) {
        if (no == no->pai->esquerda) {
            No* irmao = no->pai->direita;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai);
                irmao = no->pai->direita;
            }

            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
            } else {
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireita(arvore, irmao);
                    irmao = no->pai->direita;
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerda(arvore, no->pai);
                no = arvore->raiz;
            }
        } else {
            No* irmao = no->pai->esquerda;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai);
                irmao = no->pai->esquerda;
            }

            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
            } else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerda(arvore, irmao);
                    irmao = no->pai->esquerda;
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireita(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }
    no->cor = Preto;
}

int main() {
    Arvore* arvore = criar();

    adicionar(arvore, 50);
    adicionar(arvore, 30);
    adicionar(arvore, 70);
    adicionar(arvore, 10);
    adicionar(arvore, 40);
    adicionar(arvore, 60);
    adicionar(arvore, 80);

    printf("Em ordem: ");
    percorrerProfundidadeInOrder(arvore, arvore->raiz, visitar);
    printf("\n");

    remover(arvore, 30);
    printf("Após remoção: ");
    percorrerProfundidadeInOrder(arvore, arvore->raiz, visitar);
    printf("\n");

    return 0;
}

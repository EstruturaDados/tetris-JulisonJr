
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_Fila 5
#define TAM_Pilha 3

// estruturas

typedef struct {
    char tipo; 
    int id;     
} Peca;

typedef struct {
    Peca dados[TAM_Fila];
    int inicio, fim, qtd;
} Fila;

typedef struct {
    Peca dados[TAM_Pilha];
    int topo;
} Pilha;

//variáveis globais
int proximoID = 1;

//  geração automática

char gerarTipoAleatorio() {
    char tipos[] = {'I', 'O', 'T', 'L', 'Z'};
    return tipos[rand() % 5];
}

Peca gerarPeca() {
    Peca p;
    p.tipo = gerarTipoAleatorio();
    p.id = proximoID++;
    return p;
}

// fila

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->qtd = 0;
}

int filaVazia(Fila *f) {
    return f->qtd == 0;
}

int filaCheia(Fila *f) {
    return f->qtd == TAM_Fila;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->dados[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_Fila;
    f->qtd++;
}

Peca dequeue(Fila *f) {
    Peca removida = {'?', -1};
    if (filaVazia(f)) return removida;
    removida = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_Fila;
    f->qtd--;
    return removida;
}

void mostrarFila(Fila *f) {
    printf("FILA (Peças Futuras): ");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }
    int i = f->inicio;
    for (int c = 0; c < f->qtd; c++) {
        printf("[%c#%d] ", f->dados[i].tipo, f->dados[i].id);
        i = (i + 1) % TAM_Fila;
    }
    printf("\n");
}

//Pilha

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_Pilha- 1;
}

void push(Pilha *p, Peca pc) {
    if (pilhaCheia(p)) return;
    p->dados[++p->topo] = pc;
}

Peca pop(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) return removida;
    return p->dados[p->topo--];
}

void mostrarPilha(Pilha *p) {
    printf("Pilha (Reserva): ");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c#%d] ", p->dados[i].tipo, p->dados[i].id);
    }
    printf("\n");
}

//Trocas Avançadas

void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Não a peças suficientes para trocar!!! (fila/pilha vazia).\n");
        return;
    }

    int idxFrente = f->inicio;
    Peca temp = f->dados[idxFrente];
    f->dados[idxFrente] = p->dados[p->topo];
    p->dados[p->topo] = temp;

    printf("🔄 Peça da frente da fila trocada com topo da pilha!\n");
}

void trocarTresPrimeiros(Fila *f, Pilha *p) {
    if (p->topo != 2 || f->qtd < 3) {
        printf("E necessário 3 peças na pilha e ao menos 3 na fila.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->inicio + i) % TAM_Fila;
        Peca temp = f->dados[idxFila];
        f->dados[idxFila] = p->dados[i];
        p->dados[i] = temp;
    }

    printf("As 3 primeiras peças da fila foram trocadas com as 3 da pilha!\n");
}

//Exibição geral

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n====================================\n");
    mostrarFila(f);
    mostrarPilha(p);
    printf("====================================\n");
}

//Principal

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_Fila; i++) enqueue(&fila, gerarPeca());

    int opcao = -1;
    while (opcao != 0) {
        exibirEstado(&fila, &pilha);
        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da fila)\n");
        printf("2 - Enviar peça da fila para reserva (pilha)\n");
        printf("3 - Usar peça da reserva (pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) printf("Fila vazia!\n");
                else {
                    Peca jogada = dequeue(&fila);
                    printf("🎮 Peça jogada: [%c#%d]\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca()); //recoloca
                }
                break;
            }
            case 2: {
                if (filaVazia(&fila)) printf("Fila vazia!\n");
                else if (pilhaCheia(&pilha)) printf("Pilha cheia!\n");
                else {
                    Peca enviada = dequeue(&fila);
                    push(&pilha, enviada);
                    printf("Peça [%c#%d] enviada à reserva!\n", enviada.tipo, enviada.id);
                    enqueue(&fila, gerarPeca());
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) printf("Pilha vazia!\n");
                else {
                    Peca usada = pop(&pilha);
                    printf("Peça da reserva usada: [%c#%d]\n", usada.tipo, usada.id);
                }
                break;
            }
            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;
            case 5:
                trocarTresPrimeiros(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    }

    return 0;
}

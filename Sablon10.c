#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct NodStack {
    Masina data;
    struct NodStack* next;
};
typedef struct NodStack NodStack;

struct Stack {
    NodStack* top;
    int size;
};
typedef struct Stack Stack;

struct NodQueue {
    Masina data;
    struct NodQueue* next;
    struct NodQueue* prev;
};
typedef struct NodQueue NodQueue;

struct Queue {
    NodQueue* front;
    NodQueue* rear;
    int size;
};
typedef struct Queue Queue;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy_s(m1.model, strlen(aux) + 1, aux);
    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

Stack* createStack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void pushStack(Stack* stack, Masina masina) {
    NodStack* newNode = malloc(sizeof(NodStack));
    newNode->data = masina;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

Masina popStack(Stack* stack) {
    if (stack->top == NULL) {
        Masina empty = {0};
        return empty;
    }
    NodStack* temp = stack->top;
    Masina masina = temp->data;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    return masina;
}

int emptyStack(Stack* stack) {
    return stack->top == NULL;
}

Stack* citireStackMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }
    
    Stack* stack = createStack();
    while (!feof(file)) {
        Masina masina = citireMasinaDinFisier(file);
        if (!feof(file)) {
            pushStack(stack, masina);
        }
    }
    
    fclose(file);
    return stack;
}

void dezalocareStivaDeMasini(Stack* stack) {
    while (!emptyStack(stack)) {
        Masina masina = popStack(stack);
        free(masina.model);
        free(masina.numeSofer);
    }
    free(stack);
}

int sizeStack(Stack* stack) {
    return stack->size;
}

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue* queue, Masina masina) {
    NodQueue* newNode = malloc(sizeof(NodQueue));
    newNode->data = masina;
    newNode->next = NULL;
    newNode->prev = queue->rear;
    
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

Masina dequeue(Queue* queue) {
    if (queue->front == NULL) {
        Masina empty = {0};
        return empty;
    }
    
    NodQueue* temp = queue->front;
    Masina masina = temp->data;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    } else {
        queue->front->prev = NULL;
    }
    
    free(temp);
    queue->size--;
    return masina;
}

Queue* citireCoadaDeMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }
    
    Queue* queue = createQueue();
    while (!feof(file)) {
        Masina masina = citireMasinaDinFisier(file);
        if (!feof(file)) {
            enqueue(queue, masina);
        }
    }
    
    fclose(file);
    return queue;
}

void dezalocareCoadaDeMasini(Queue* queue) {
    while (queue->front != NULL) {
        Masina masina = dequeue(queue);
        free(masina.model);
        free(masina.numeSofer);
    }
    free(queue);
}

Masina getMasinaByIDStack(Stack* stack, int id) {
    Stack* tempStack = createStack();
    Masina foundMasina = {0};
    int found = 0;
    
    while (!emptyStack(stack)) {
        Masina masina = popStack(stack);
        if (masina.id == id && !found) {
            foundMasina = masina;
            found = 1;
        }
        pushStack(tempStack, masina);
    }
    
    while (!emptyStack(tempStack)) {
        pushStack(stack, popStack(tempStack));
    }
    
    free(tempStack);
    return foundMasina;
}

Masina getMasinaByIDQueue(Queue* queue, int id) {
    NodQueue* current = queue->front;
    while (current != NULL) {
        if (current->data.id == id) {
            return current->data;
        }
        current = current->next;
    }
    Masina empty = {0};
    return empty;
}

float calculeazaPretTotalStack(Stack* stack) {
    Stack* tempStack = createStack();
    float total = 0.0;
    
    while (!emptyStack(stack)) {
        Masina masina = popStack(stack);
        total += masina.pret;
        pushStack(tempStack, masina);
    }
    
    while (!emptyStack(tempStack)) {
        pushStack(stack, popStack(tempStack));
    }
    
    free(tempStack);
    return total;
}

float calculeazaPretTotalQueue(Queue* queue) {
    NodQueue* current = queue->front;
    float total = 0.0;
    
    while (current != NULL) {
        total += current->data.pret;
        current = current->next;
    }
    
    return total;
}

int main() {
    printf("Testare Stack:\n");
    Stack* stack = citireStackMasiniDinFisier("masini.txt");
    if (stack != NULL) {
        printf("Numarul de masini in stack: %d\n", sizeStack(stack));
        printf("Pretul total al masinilor: %.2f\n", calculeazaPretTotalStack(stack));
        
        printf("\nAfisare masini din stack:\n");
        while (!emptyStack(stack)) {
            Masina masina = popStack(stack);
            afisareMasina(masina);
            free(masina.model);
            free(masina.numeSofer);
        }
        free(stack);
    }
    
    printf("\n\nTestare Queue:\n");
    Queue* queue = citireCoadaDeMasiniDinFisier("masini.txt");
    if (queue != NULL) {
        printf("Numarul de masini in queue: %d\n", queue->size);
        printf("Pretul total al masinilor: %.2f\n", calculeazaPretTotalQueue(queue));
        
        printf("\nAfisare masini din queue:\n");
        while (queue->front != NULL) {
            Masina masina = dequeue(queue);
            afisareMasina(masina);
            free(masina.model);
            free(masina.numeSofer);
        }
        free(queue);
    }
    
    return 0;
}
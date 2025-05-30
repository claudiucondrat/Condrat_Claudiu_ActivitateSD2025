#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct NodInt {
    int data;
    struct NodInt* next;
};
typedef struct NodInt NodInt;

struct Stack {
    NodInt* top;
    int size;
};
typedef struct Stack Stack;

struct Queue {
    NodInt* front;
    NodInt* rear;
    int size;
};
typedef struct Queue Queue;

Stack* createStack() {
    Stack* stack = malloc(sizeof(Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void push(Stack* stack, int value) {
    NodInt* newNode = malloc(sizeof(NodInt));
    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

int pop(Stack* stack) {
    if (stack->top == NULL) return -1;
    NodInt* temp = stack->top;
    int value = temp->data;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    return value;
}

bool isEmpty(Stack* stack) {
    return stack->top == NULL;
}

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue* queue, int value) {
    NodInt* newNode = malloc(sizeof(NodInt));
    newNode->data = value;
    newNode->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    queue->size++;
}

int dequeue(Queue* queue) {
    if (queue->front == NULL) return -1;
    NodInt* temp = queue->front;
    int value = temp->data;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return value;
}

void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

void freeQueue(Queue* queue) {
    while (queue->front != NULL) {
        dequeue(queue);
    }
    free(queue);
}

void afiseazaStack(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack-ul este gol\n");
        return;
    }
    
    Stack* tempStack = createStack();
    printf("Stack: ");
    
    while (!isEmpty(stack)) {
        int value = pop(stack);
        printf("%d ", value);
        push(tempStack, value);
    }
    
    while (!isEmpty(tempStack)) {
        push(stack, pop(tempStack));
    }
    
    freeStack(tempStack);
    printf("\n");
}

void afiseazaQueue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue-ul este gol\n");
        return;
    }
    
    NodInt* current = queue->front;
    printf("Queue: ");
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void inverseazaNumere() {
    Stack* stack = createStack();
    
    printf("Introdu 5 numere: ");
    for (int i = 0; i < 5; i++) {
        int num;
        scanf("%d", &num);
        push(stack, num);
    }
    
    printf("Numerele in ordine inversa: ");
    while (!isEmpty(stack)) {
        printf("%d ", pop(stack));
    }
    printf("\n\n");
    
    freeStack(stack);
}

void contorizeazaElemente() {
    Queue* queue = createQueue();
    
    printf("Adaug numerele 10, 20, 30, 40 in queue\n");
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    enqueue(queue, 40);
    
    printf("Numarul de elemente in queue: %d\n", queue->size);
    afiseazaQueue(queue);
    
    printf("Scot 2 elemente: %d si %d\n", dequeue(queue), dequeue(queue));
    printf("Numarul de elemente ramase: %d\n", queue->size);
    afiseazaQueue(queue);
    printf("\n");
    
    freeQueue(queue);
}

void sumaElementelor() {
    Stack* stack = createStack();
    
    int numere[] = {5, 10, 15, 20, 25};
    printf("Adaug in stack: ");
    for (int i = 0; i < 5; i++) {
        push(stack, numere[i]);
        printf("%d ", numere[i]);
    }
    printf("\n");
    
    Stack* tempStack = createStack();
    int suma = 0;
    
    while (!isEmpty(stack)) {
        int value = pop(stack);
        suma += value;
        push(tempStack, value);
    }
    
    while (!isEmpty(tempStack)) {
        push(stack, pop(tempStack));
    }
    
    printf("Suma elementelor: %d\n", suma);
    afiseazaStack(stack);
    printf("\n");
    
    freeStack(stack);
    freeStack(tempStack);
}

void verificaParitate() {
    Queue* queuePare = createQueue();
    Queue* queueImpare = createQueue();
    
    int numere[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    for (int i = 0; i < 10; i++) {
        if (numere[i] % 2 == 0) {
            enqueue(queuePare, numere[i]);
        } else {
            enqueue(queueImpare, numere[i]);
        }
    }
    
    printf("Numere pare: ");
    afiseazaQueue(queuePare);
    
    printf("Numere impare: ");
    afiseazaQueue(queueImpare);
    printf("\n");
    
    freeQueue(queuePare);
    freeQueue(queueImpare);
}

void gasesteMximul() {
    Stack* stack = createStack();
    
    int numere[] = {3, 7, 2, 9, 1, 5};
    printf("Adaug in stack: ");
    for (int i = 0; i < 6; i++) {
        push(stack, numere[i]);
        printf("%d ", numere[i]);
    }
    printf("\n");
    
    Stack* tempStack = createStack();
    int maxim = -1;
    
    while (!isEmpty(stack)) {
        int value = pop(stack);
        if (maxim == -1 || value > maxim) {
            maxim = value;
        }
        push(tempStack, value);
    }
    
    while (!isEmpty(tempStack)) {
        push(stack, pop(tempStack));
    }
    
    printf("Maximul din stack: %d\n", maxim);
    afiseazaStack(stack);
    printf("\n");
    
    freeStack(stack);
    freeStack(tempStack);
}

int main() {
    
    inverseazaNumere();
    contorizeazaElemente();
    sumaElementelor();
    verificaParitate();
    gasesteMximul();
    
    return 0;
}
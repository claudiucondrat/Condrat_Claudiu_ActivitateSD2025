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

struct Heap {
    Masina* vector;
    int lungime;
    int nrElemente;
    int* vizibil;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
    Heap h;
    h.lungime = lungime;
    h.nrElemente = 0;
    h.vector = (Masina*)malloc(lungime * sizeof(Masina));
    h.vizibil = (int*)malloc(lungime * sizeof(int));
    for (int i = 0; i < lungime; i++) {
        h.vizibil[i] = 1;
    }
    return h;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
    int pozitieMaxim = pozitieNod;
    int pozitieStanga = 2 * pozitieNod + 1;
    int pozitieDreapta = 2 * pozitieNod + 2;

    if (pozitieStanga < heap.nrElemente &&
        heap.vizibil[pozitieStanga] == 1 &&
        heap.vector[pozitieStanga].pret > heap.vector[pozitieMaxim].pret) {
        pozitieMaxim = pozitieStanga;
    }

    if (pozitieDreapta < heap.nrElemente &&
        heap.vizibil[pozitieDreapta] == 1 &&
        heap.vector[pozitieDreapta].pret > heap.vector[pozitieMaxim].pret) {
        pozitieMaxim = pozitieDreapta;
    }

    if (pozitieMaxim != pozitieNod) {
        Masina temp = heap.vector[pozitieNod];
        heap.vector[pozitieNod] = heap.vector[pozitieMaxim];
        heap.vector[pozitieMaxim] = temp;

        filtreazaHeap(heap, pozitieMaxim);
    }
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        Heap h = initializareHeap(1);
        return h;
    }

    int numarMasini = 0;
    char buffer[100];
    while (fgets(buffer, 100, f) != NULL) {
        numarMasini++;
        fseek(f, -strlen(buffer), SEEK_CUR);
        fseek(f, strlen(buffer), SEEK_CUR);
    }
    fseek(f, 0, SEEK_SET);

    Heap heap = initializareHeap(numarMasini);

    for (int i = 0; i < numarMasini; i++) {
        heap.vector[i] = citireMasinaDinFisier(f);
        heap.nrElemente++;
    }

    fclose(f);

    for (int i = heap.nrElemente / 2 - 1; i >= 0; i--) {
        filtreazaHeap(heap, i);
    }

    return heap;
}

void afisareHeap(Heap heap) {
    printf("Elemente vizibile din heap:\n");
    for (int i = 0; i < heap.nrElemente; i++) {
        if (heap.vizibil[i] == 1) {
            printf("Pozitia %d:\n", i);
            afisareMasina(heap.vector[i]);
        }
    }
}

void afiseazaHeapAscuns(Heap heap) {
    printf("Elemente ascunse din heap:\n");
    for (int i = 0; i < heap.nrElemente; i++) {
        if (heap.vizibil[i] == 0) {
            printf("Pozitia %d:\n", i);
            afisareMasina(heap.vector[i]);
        }
    }
}

Masina extrageMasina(void* heapParam) {
    Heap* heap = (Heap*)heapParam;

    if (heap->nrElemente == 0) {
        printf("Heap-ul este gol!\n");
        Masina m;
        m.id = -1;
        m.model = NULL;
        m.numeSofer = NULL;
        return m;
    }

    Masina rezultat = heap->vector[0];

    heap->vizibil[0] = 0;

    filtreazaHeap(*heap, 0);

    return rezultat;
}

void dezalocareHeap(Heap* heap) {
    if (heap->vector != NULL) {
        for (int i = 0; i < heap->nrElemente; i++) {
            free(heap->vector[i].model);
            free(heap->vector[i].numeSofer);
        }
        free(heap->vector);
        free(heap->vizibil);

        heap->vector = NULL;
        heap->vizibil = NULL;
        heap->nrElemente = 0;
        heap->lungime = 0;
    }
}

int main() {
    Heap heap = citireHeapDeMasiniDinFisier("masini.txt");

    printf("Heap original:\n");
    afisareHeap(heap);

    printf("\nExtragere element root:\n");
    Masina extrasa = extrageMasina(&heap);
    afisareMasina(extrasa);

    printf("\nHeap dupa extragere:\n");
    afisareHeap(heap);

    printf("\nElemente ascunse din heap:\n");
    afiseazaHeapAscuns(heap);

    dezalocareHeap(&heap);

    return 0;
}
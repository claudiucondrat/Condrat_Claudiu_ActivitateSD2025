#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Eveniment {
    int id;
    char* nume;
    char* locatie;
    char* data;
    int nrParticipanti;
    float pretBilet;
};
typedef struct Eveniment Eveniment;

struct MinHeap {
    Eveniment* vector;
    int dimensiune;
    int capacitate;
};
typedef struct MinHeap MinHeap;

Eveniment initializareEveniment(int id, const char* nume, const char* locatie, const char* data, int nrParticipanti, float pretBilet) {
    Eveniment e;
    e.id = id;

    e.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(e.nume, nume);

    e.locatie = (char*)malloc(strlen(locatie) + 1);
    strcpy(e.locatie, locatie);

    e.data = (char*)malloc(strlen(data) + 1);
    strcpy(e.data, data);

    e.nrParticipanti = nrParticipanti;
    e.pretBilet = pretBilet;

    return e;
}

void afisareEveniment(Eveniment e) {
    printf("ID: %d\n", e.id);
    printf("Nume: %s\n", e.nume);
    printf("Locatie: %s\n", e.locatie);
    printf("Data: %s\n", e.data);
    printf("Nr. participanti: %d\n", e.nrParticipanti);
    printf("Pret bilet: %.2f\n\n", e.pretBilet);
}

// Comparator pentru evenimente după data (YYYY-MM-DD)
int comparaData(const char* data1, const char* data2) {
    int an1, luna1, zi1;
    int an2, luna2, zi2;

    sscanf(data1, "%d-%d-%d", &an1, &luna1, &zi1);
    sscanf(data2, "%d-%d-%d", &an2, &luna2, &zi2);

    if (an1 != an2) return an1 - an2;
    if (luna1 != luna2) return luna1 - luna2;
    return zi1 - zi2;
}

MinHeap initializareMinHeap(int capacitate) {
    MinHeap heap;
    heap.dimensiune = 0;
    heap.capacitate = capacitate;
    heap.vector = (Eveniment*)malloc(capacitate * sizeof(Eveniment));
    return heap;
}

void swap(Eveniment* a, Eveniment* b) {
    Eveniment temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->dimensiune &&
        comparaData(heap->vector[left].data, heap->vector[smallest].data) < 0)
        smallest = left;

    if (right < heap->dimensiune &&
        comparaData(heap->vector[right].data, heap->vector[smallest].data) < 0)
        smallest = right;

    if (smallest != idx) {
        swap(&heap->vector[idx], &heap->vector[smallest]);
        minHeapify(heap, smallest);
    }
}

void inserareEveniment(MinHeap* heap, Eveniment e) {
    if (heap->dimensiune == heap->capacitate) {
        printf("Heap-ul este plin!\n");
        return;
    }

    int i = heap->dimensiune;
    heap->vector[i] = e;
    heap->dimensiune++;

    while (i > 0 && comparaData(heap->vector[(i - 1) / 2].data, heap->vector[i].data) > 0) {
        swap(&heap->vector[i], &heap->vector[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Eveniment extrageMinim(MinHeap* heap) {
    if (heap->dimensiune <= 0) {
        printf("Heap-ul este gol!\n");
        Eveniment e;
        e.id = -1;
        e.nume = NULL;
        e.locatie = NULL;
        e.data = NULL;
        return e;
    }

    if (heap->dimensiune == 1) {
        heap->dimensiune--;
        return heap->vector[0];
    }

    Eveniment root = heap->vector[0];
    heap->vector[0] = heap->vector[heap->dimensiune - 1];
    heap->dimensiune--;
    minHeapify(heap, 0);

    return root;
}

void afisareHeap(MinHeap heap) {
    printf("Evenimentele din heap (ordonate dupa data):\n");
    for (int i = 0; i < heap.dimensiune; i++) {
        printf("Pozitia %d:\n", i);
        afisareEveniment(heap.vector[i]);
    }
}

Eveniment* cautareEvenimentDupaID(MinHeap heap, int id) {
    for (int i = 0; i < heap.dimensiune; i++) {
        if (heap.vector[i].id == id) {
            return &heap.vector[i];
        }
    }
    return NULL;
}

void dezalocareHeap(MinHeap* heap) {
    for (int i = 0; i < heap->dimensiune; i++) {
        free(heap->vector[i].nume);
        free(heap->vector[i].locatie);
        free(heap->vector[i].data);
    }

    free(heap->vector);
    heap->vector = NULL;
    heap->dimensiune = 0;
    heap->capacitate = 0;
}

void afisareEvenimenteViitoare(MinHeap heap, const char* dataReferinta) {
    printf("Evenimente viitoare dupa data %s (in ordine cronologica):\n", dataReferinta);

    MinHeap tempHeap = initializareMinHeap(heap.capacitate);

    for (int i = 0; i < heap.dimensiune; i++) {
        if (comparaData(heap.vector[i].data, dataReferinta) > 0) {
            inserareEveniment(&tempHeap, heap.vector[i]);
        }
    }

    while (tempHeap.dimensiune > 0) {
        Eveniment e = extrageMinim(&tempHeap);
        afisareEveniment(e);
    }

    free(tempHeap.vector);
}

float calculeazaVenitTotal(MinHeap heap) {
    float venitTotal = 0;

    for (int i = 0; i < heap.dimensiune; i++) {
        venitTotal += heap.vector[i].nrParticipanti * heap.vector[i].pretBilet;
    }

    return venitTotal;
}

int main() {
    MinHeap heap = initializareMinHeap(10);

    inserareEveniment(&heap, initializareEveniment(101, "Concert Horia Brenciu", "Sala Palatului", "2025-06-15", 500, 150.0));
    inserareEveniment(&heap, initializareEveniment(202, "Festival Folk", "Parcul Herastrau", "2025-07-20", 2000, 80.0));
    inserareEveniment(&heap, initializareEveniment(153, "Expozitie de Arta", "Muzeul National", "2025-05-10", 300, 45.0));
    inserareEveniment(&heap, initializareEveniment(304, "Maraton", "Parcul Tineretului", "2025-09-05", 1500, 100.0));
    inserareEveniment(&heap, initializareEveniment(205, "Teatru in Aer Liber", "Gradina Botanica", "2025-08-12", 400, 120.0));

    printf("===== AFISARE TOATE EVENIMENTELE =====\n");
    afisareHeap(heap);

    printf("\n===== EXTRAGERE EVENIMENT CU CEA MAI APROPIATA DATA =====\n");
    Eveniment evenimentMinim = extrageMinim(&heap);
    printf("Evenimentul cu cea mai apropiata data:\n");
    afisareEveniment(evenimentMinim);

    printf("\n===== HEAP DUPA EXTRAGERE =====\n");
    afisareHeap(heap);

    printf("\n===== CAUTARE EVENIMENT DUPA ID =====\n");
    int idCautat = 202;
    Eveniment* evenimentGasit = cautareEvenimentDupaID(heap, idCautat);

    if (evenimentGasit != NULL) {
        printf("Evenimentul cu ID-ul %d a fost gasit:\n", idCautat);
        afisareEveniment(*evenimentGasit);
    }
    else {
        printf("Evenimentul cu ID-ul %d nu a fost gasit!\n", idCautat);
    }

    printf("\n===== AFISARE EVENIMENTE VIITOARE DUPA 2025-07-01 =====\n");
    afisareEvenimenteViitoare(heap, "2025-07-01");

    printf("\n===== CALCUL VENIT TOTAL =====\n");
    printf("Venitul total din vanzarea biletelor: %.2f lei\n", calculeazaVenitTotal(heap));

    free(evenimentMinim.nume);
    free(evenimentMinim.locatie);
    free(evenimentMinim.data);

    dezalocareHeap(&heap);

    return 0;
}
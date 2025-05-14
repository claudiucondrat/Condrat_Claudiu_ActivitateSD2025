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

struct Nod {
    Masina info;
    struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
    Nod** vector;
    int dim;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* cap) {
    Nod* temp = cap;
    while (temp != NULL) {
        afisareMasina(temp->info);
        temp = temp->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;

    if (*cap == NULL) {
        *cap = nou;
        return;
    }

    Nod* temp = *cap;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = nou;
}

HashTable initializareHashTable(int dimensiune) {
    HashTable ht;
    ht.dim = dimensiune;
    ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
    for (int i = 0; i < dimensiune; i++) {
        ht.vector[i] = NULL;
    }
    return ht;
}

int calculeazaHash(int id, int dimensiune) {
    return id % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
    int pozitie = calculeazaHash(masina.id, hash.dim);
    adaugaMasinaInLista(&hash.vector[pozitie], masina);
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        HashTable ht = initializareHashTable(1);
        return ht;
    }

    HashTable ht = initializareHashTable(10);
    char buffer[100];
    while (fgets(buffer, 100, f) != NULL) {
        fseek(f, -strlen(buffer), SEEK_CUR);
        Masina m = citireMasinaDinFisier(f);
        inserareMasinaInTabela(ht, m);
    }
    fclose(f);
    return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            printf("\n===== Cluster %d =====\n", i);
            afisareListaMasini(ht.vector[i]);
        }
    }
}

void dezalocareMasina(Masina* m) {
    free(m->model);
    free(m->numeSofer);
}

void dezalocareListaMasini(Nod** cap) {
    Nod* temp = *cap;
    while (temp != NULL) {
        Nod* aux = temp;
        temp = temp->next;
        dezalocareMasina(&aux->info);
        free(aux);
    }
    *cap = NULL;
}

void dezalocareTabelaDeMasini(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        dezalocareListaMasini(&ht->vector[i]);
    }
    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
    int count = 0;
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            count++;
        }
    }

    if (count == 0) {
        *nrClustere = 0;
        return NULL;
    }

    float* preturiMedii = (float*)malloc(count * sizeof(float));
    *nrClustere = count;

    int index = 0;
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) {
            float suma = 0;
            int nrMasini = 0;
            Nod* temp = ht.vector[i];

            while (temp != NULL) {
                suma += temp->info.pret;
                nrMasini++;
                temp = temp->next;
            }

            preturiMedii[index++] = suma / nrMasini;
        }
    }

    return preturiMedii;
}

Masina getMasinaDupaId(HashTable ht, int id) {
    Masina m;
    m.id = -1;
    m.model = NULL;
    m.numeSofer = NULL;

    int pozitie = calculeazaHash(id, ht.dim);

    Nod* temp = ht.vector[pozitie];
    while (temp != NULL) {
        if (temp->info.id == id) {
            return temp->info;
        }
        temp = temp->next;
    }

    return m;
}

int main() {
    HashTable ht = citireMasiniDinFisier("masini.txt");

    afisareTabelaDeMasini(ht);

    int nrClustere = 0;
    float* preturiMedii = calculeazaPreturiMediiPerClustere(ht, &nrClustere);

    for (int i = 0; i < nrClustere; i++) {
        printf("Cluster %d: %.2f\n", i, preturiMedii[i]);
    }

    int idCautat = 2;
    Masina masina = getMasinaDupaId(ht, idCautat);

    if (masina.id != -1) {
        afisareMasina(masina);
    }

    free(preturiMedii);
    dezalocareTabelaDeMasini(&ht);

    return 0;
}
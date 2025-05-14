#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produs {
    int cod;
    char* denumire;
    float pret;
    int stoc;
    char* categorie;
};
typedef struct Produs Produs;

struct Nod {
    Produs info;
    struct Nod* next;
};
typedef struct Nod Nod;

struct HashTable {
    Nod** vector;
    int dimensiune;
};
typedef struct HashTable HashTable;

Produs initializareProdus(int cod, const char* denumire, float pret, int stoc, const char* categorie) {
    Produs p;
    p.cod = cod;

    p.denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(p.denumire, denumire);

    p.pret = pret;
    p.stoc = stoc;

    p.categorie = (char*)malloc(strlen(categorie) + 1);
    strcpy(p.categorie, categorie);

    return p;
}

void afisareProdus(Produs p) {
    printf("Cod: %d\n", p.cod);
    printf("Denumire: %s\n", p.denumire);
    printf("Pret: %.2f\n", p.pret);
    printf("Stoc: %d\n", p.stoc);
    printf("Categorie: %s\n\n", p.categorie);
}

int hashFunction(int key, int dimensiune) {
    return key % dimensiune;
}

HashTable initializareHashTable(int dimensiune) {
    HashTable ht;
    ht.dimensiune = dimensiune;
    ht.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));

    for (int i = 0; i < dimensiune; i++) {
        ht.vector[i] = NULL;
    }

    return ht;
}

void inserareProdus(HashTable* ht, Produs p) {
    int pozitie = hashFunction(p.cod, ht->dimensiune);

    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = p;

    if (ht->vector[pozitie] == NULL) {
        nou->next = NULL;
        ht->vector[pozitie] = nou;
    }
    else {
        Nod* temp = ht->vector[pozitie];
        while (temp != NULL) {
            if (temp->info.cod == p.cod) {
                printf("Produsul cu codul %d exista deja!\n", p.cod);
                free(nou);
                return;
            }
            temp = temp->next;
        }

        nou->next = ht->vector[pozitie];
        ht->vector[pozitie] = nou;
    }
}

Produs* cautareProdus(HashTable ht, int cod) {
    int pozitie = hashFunction(cod, ht.dimensiune);

    Nod* temp = ht.vector[pozitie];
    while (temp != NULL) {
        if (temp->info.cod == cod) {
            return &(temp->info);
        }
        temp = temp->next;
    }

    return NULL;
}

void afisareProduse(HashTable ht) {
    printf("Lista produselor:\n");

    for (int i = 0; i < ht.dimensiune; i++) {
        Nod* temp = ht.vector[i];

        if (temp != NULL) {
            printf("Bucket %d:\n", i);

            while (temp != NULL) {
                afisareProdus(temp->info);
                temp = temp->next;
            }
        }
    }
}

void afisareProduseCategorie(HashTable ht, const char* categorie) {
    printf("Produse din categoria %s:\n", categorie);

    for (int i = 0; i < ht.dimensiune; i++) {
        Nod* temp = ht.vector[i];

        while (temp != NULL) {
            if (strcmp(temp->info.categorie, categorie) == 0) {
                afisareProdus(temp->info);
            }
            temp = temp->next;
        }
    }
}

float calculValoareTotalaStoc(HashTable ht) {
    float valoareTotala = 0;

    for (int i = 0; i < ht.dimensiune; i++) {
        Nod* temp = ht.vector[i];

        while (temp != NULL) {
            valoareTotala += temp->info.pret * temp->info.stoc;
            temp = temp->next;
        }
    }

    return valoareTotala;
}

void actualizareStoc(HashTable ht, int cod, int cantitate) {
    Produs* p = cautareProdus(ht, cod);

    if (p != NULL) {
        p->stoc += cantitate;
        printf("Stocul produsului %s a fost actualizat la %d.\n", p->denumire, p->stoc);
    }
    else {
        printf("Produsul cu codul %d nu a fost gasit!\n", cod);
    }
}

void stergereProdus(HashTable* ht, int cod) {
    int pozitie = hashFunction(cod, ht->dimensiune);

    Nod* temp = ht->vector[pozitie];
    Nod* prev = NULL;

    while (temp != NULL && temp->info.cod != cod) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Produsul cu codul %d nu a fost gasit!\n", cod);
        return;
    }

    if (prev == NULL) {
        ht->vector[pozitie] = temp->next;
    }
    else {
        prev->next = temp->next;
    }

    free(temp->info.denumire);
    free(temp->info.categorie);
    free(temp);

    printf("Produsul cu codul %d a fost sters!\n", cod);
}

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < ht->dimensiune; i++) {
        Nod* temp = ht->vector[i];

        while (temp != NULL) {
            Nod* aux = temp;
            temp = temp->next;

            free(aux->info.denumire);
            free(aux->info.categorie);
            free(aux);
        }

        ht->vector[i] = NULL;
    }

    free(ht->vector);
    ht->vector = NULL;
    ht->dimensiune = 0;
}

int main() {
    HashTable ht = initializareHashTable(10);

    inserareProdus(&ht, initializareProdus(1001, "Laptop Dell XPS 13", 4999.99, 15, "Electronice"));
    inserareProdus(&ht, initializareProdus(2002, "iPhone 14 Pro", 5499.99, 20, "Telefoane"));
    inserareProdus(&ht, initializareProdus(1003, "Monitor Samsung 27'", 1299.99, 30, "Electronice"));
    inserareProdus(&ht, initializareProdus(3001, "Canapea extensibila", 2499.99, 5, "Mobilier"));
    inserareProdus(&ht, initializareProdus(2005, "Samsung Galaxy S23", 4299.99, 25, "Telefoane"));

    printf("====== AFISARE TOATE PRODUSELE ======\n");
    afisareProduse(ht);

    printf("\n====== CAUTARE PRODUS ======\n");
    int codCautat = 1003;
    Produs* produsGasit = cautareProdus(ht, codCautat);

    if (produsGasit != NULL) {
        printf("Produsul cu codul %d a fost gasit:\n", codCautat);
        afisareProdus(*produsGasit);
    }
    else {
        printf("Produsul cu codul %d nu a fost gasit!\n", codCautat);
    }

    printf("\n====== AFISARE PRODUSE DIN CATEGORIA TELEFOANE ======\n");
    afisareProduseCategorie(ht, "Telefoane");

    printf("\n====== ACTUALIZARE STOC ======\n");
    actualizareStoc(ht, 1001, 5);

    printf("\n====== CALCULARE VALOARE TOTALA STOC ======\n");
    printf("Valoarea totala a stocului: %.2f lei\n", calculValoareTotalaStoc(ht));

    printf("\n====== STERGERE PRODUS ======\n");
    stergereProdus(&ht, 3001);

    printf("\n====== AFISARE DUPA STERGERE ======\n");
    afisareProduse(ht);

    dezalocareHashTable(&ht);

    return 0;
}
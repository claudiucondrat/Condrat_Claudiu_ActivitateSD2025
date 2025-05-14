#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
    int isbn;
    char* titlu;
    char* autor;
    int anPublicare;
    char* editura;
    int nrPagini;
    float pret;
};
typedef struct Carte Carte;

struct NodBST {
    Carte info;
    struct NodBST* stanga;
    struct NodBST* dreapta;
};
typedef struct NodBST NodBST;

Carte initializareCarte(int isbn, const char* titlu, const char* autor, int anPublicare, const char* editura, int nrPagini, float pret) {
    Carte c;
    c.isbn = isbn;

    c.titlu = (char*)malloc(strlen(titlu) + 1);
    strcpy(c.titlu, titlu);

    c.autor = (char*)malloc(strlen(autor) + 1);
    strcpy(c.autor, autor);

    c.anPublicare = anPublicare;

    c.editura = (char*)malloc(strlen(editura) + 1);
    strcpy(c.editura, editura);

    c.nrPagini = nrPagini;
    c.pret = pret;

    return c;
}

void afisareCarte(Carte c) {
    printf("ISBN: %d\n", c.isbn);
    printf("Titlu: %s\n", c.titlu);
    printf("Autor: %s\n", c.autor);
    printf("An publicare: %d\n", c.anPublicare);
    printf("Editura: %s\n", c.editura);
    printf("Nr. pagini: %d\n", c.nrPagini);
    printf("Pret: %.2f\n\n", c.pret);
}

NodBST* creareNod(Carte c) {
    NodBST* nod = (NodBST*)malloc(sizeof(NodBST));
    nod->info = c;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    return nod;
}

NodBST* inserareCarte(NodBST* radacina, Carte c) {
    if (radacina == NULL) {
        return creareNod(c);
    }

    if (c.isbn < radacina->info.isbn) {
        radacina->stanga = inserareCarte(radacina->stanga, c);
    }
    else if (c.isbn > radacina->info.isbn) {
        radacina->dreapta = inserareCarte(radacina->dreapta, c);
    }
    else {
        printf("Cartea cu ISBN %d exista deja!\n", c.isbn);
    }

    return radacina;
}

void afisareInordine(NodBST* radacina) {
    if (radacina != NULL) {
        afisareInordine(radacina->stanga);
        afisareCarte(radacina->info);
        afisareInordine(radacina->dreapta);
    }
}

void afisarePreordine(NodBST* radacina) {
    if (radacina != NULL) {
        afisareCarte(radacina->info);
        afisarePreordine(radacina->stanga);
        afisarePreordine(radacina->dreapta);
    }
}

void afisarePostordine(NodBST* radacina) {
    if (radacina != NULL) {
        afisarePostordine(radacina->stanga);
        afisarePostordine(radacina->dreapta);
        afisareCarte(radacina->info);
    }
}

Carte* cautareCarteISBN(NodBST* radacina, int isbn) {
    if (radacina == NULL) {
        return NULL;
    }

    if (radacina->info.isbn == isbn) {
        return &(radacina->info);
    }

    if (isbn < radacina->info.isbn) {
        return cautareCarteISBN(radacina->stanga, isbn);
    }
    else {
        return cautareCarteISBN(radacina->dreapta, isbn);
    }
}

void afisareCartiAutor(NodBST* radacina, const char* autor) {
    if (radacina != NULL) {
        afisareCartiAutor(radacina->stanga, autor);

        if (strcmp(radacina->info.autor, autor) == 0) {
            afisareCarte(radacina->info);
        }

        afisareCartiAutor(radacina->dreapta, autor);
    }
}

void afisareCartiEditura(NodBST* radacina, const char* editura) {
    if (radacina != NULL) {
        afisareCartiEditura(radacina->stanga, editura);

        if (strcmp(radacina->info.editura, editura) == 0) {
            afisareCarte(radacina->info);
        }

        afisareCartiEditura(radacina->dreapta, editura);
    }
}

float calculPretTotalCarti(NodBST* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    return radacina->info.pret + calculPretTotalCarti(radacina->stanga) + calculPretTotalCarti(radacina->dreapta);
}

int numarCarti(NodBST* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    return 1 + numarCarti(radacina->stanga) + numarCarti(radacina->dreapta);
}

float calculPretMediu(NodBST* radacina) {
    int numar = numarCarti(radacina);
    if (numar == 0) {
        return 0;
    }

    return calculPretTotalCarti(radacina) / numar;
}

NodBST* gasesteMinim(NodBST* nod) {
    NodBST* current = nod;

    while (current && current->stanga != NULL) {
        current = current->stanga;
    }

    return current;
}

NodBST* stergereCarte(NodBST* radacina, int isbn) {
    if (radacina == NULL) {
        return radacina;
    }

    if (isbn < radacina->info.isbn) {
        radacina->stanga = stergereCarte(radacina->stanga, isbn);
    }
    else if (isbn > radacina->info.isbn) {
        radacina->dreapta = stergereCarte(radacina->dreapta, isbn);
    }
    else {
        if (radacina->stanga == NULL) {
            NodBST* temp = radacina->dreapta;
            free(radacina->info.titlu);
            free(radacina->info.autor);
            free(radacina->info.editura);
            free(radacina);
            return temp;
        }
        else if (radacina->dreapta == NULL) {
            NodBST* temp = radacina->stanga;
            free(radacina->info.titlu);
            free(radacina->info.autor);
            free(radacina->info.editura);
            free(radacina);
            return temp;
        }

        NodBST* temp = gasesteMinim(radacina->dreapta);

        free(radacina->info.titlu);
        free(radacina->info.autor);
        free(radacina->info.editura);

        radacina->info = temp->info;

        radacina->info.titlu = (char*)malloc(strlen(temp->info.titlu) + 1);
        strcpy(radacina->info.titlu, temp->info.titlu);

        radacina->info.autor = (char*)malloc(strlen(temp->info.autor) + 1);
        strcpy(radacina->info.autor, temp->info.autor);

        radacina->info.editura = (char*)malloc(strlen(temp->info.editura) + 1);
        strcpy(radacina->info.editura, temp->info.editura);

        radacina->dreapta = stergereCarte(radacina->dreapta, temp->info.isbn);
    }

    return radacina;
}

void dezalocareArbore(NodBST** radacina) {
    if (*radacina != NULL) {
        dezalocareArbore(&((*radacina)->stanga));
        dezalocareArbore(&((*radacina)->dreapta));

        free((*radacina)->info.titlu);
        free((*radacina)->info.autor);
        free((*radacina)->info.editura);
        free(*radacina);
        *radacina = NULL;
    }
}

int main() {
    NodBST* radacina = NULL;

    radacina = inserareCarte(radacina, initializareCarte(9780316769488, "The Catcher in the Rye", "J.D. Salinger", 1951, "Little, Brown and Company", 224, 45.99));
    radacina = inserareCarte(radacina, initializareCarte(9780679783268, "Crime and Punishment", "Fyodor Dostoevsky", 1866, "Vintage Classics", 565, 52.50));
    radacina = inserareCarte(radacina, initializareCarte(9780061120084, "To Kill a Mockingbird", "Harper Lee", 1960, "Harper Perennial", 336, 38.75));
    radacina = inserareCarte(radacina, initializareCarte(9780140283334, "1984", "George Orwell", 1949, "Penguin Books", 328, 42.25));
    radacina = inserareCarte(radacina, initializareCarte(9780142437230, "Pride and Prejudice", "Jane Austen", 1813, "Penguin Classics", 435, 35.99));

    printf("===== AFISARE CARTI IN ORDINE CRESCATOARE DUPA ISBN =====\n");
    afisareInordine(radacina);

    printf("\n===== CAUTARE CARTE DUPA ISBN =====\n");
    int isbnCautat = 9780140283334;
    Carte* carteGasita = cautareCarteISBN(radacina, isbnCautat);

    if (carteGasita != NULL) {
        printf("Cartea cu ISBN-ul %d a fost gasita:\n", isbnCautat);
        afisareCarte(*carteGasita);
    }
    else {
        printf("Cartea cu ISBN-ul %d nu a fost gasita!\n", isbnCautat);
    }

    printf("\n===== AFISARE CARTI DE LA EDITURA PENGUIN CLASSICS =====\n");
    afisareCartiEditura(radacina, "Penguin Classics");

    printf("\n===== CALCUL PRET MEDIU =====\n");
    printf("Pretul mediu al cartilor: %.2f lei\n", calculPretMediu(radacina));

    printf("\n===== STERGERE CARTE =====\n");
    radacina = stergereCarte(radacina, 9780061120084);
    printf("Dupa stergerea cartii cu ISBN 9780061120084:\n");
    afisareInordine(radacina);

    dezalocareArbore(&radacina);

    return 0;
}
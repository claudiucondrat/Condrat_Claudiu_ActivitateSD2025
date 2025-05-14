#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Film {
    int id;
    char* titlu;
    char* regizor;
    int anAparitie;
    float rating;
    char* gen;
};
typedef struct Film Film;

struct Nod {
    Film info;
    struct Nod* next;
    struct Nod* prev;
};
typedef struct Nod Nod;

Film initializareFilm(int id, const char* titlu, const char* regizor, int anAparitie, float rating, const char* gen) {
    Film f;
    f.id = id;

    f.titlu = (char*)malloc(strlen(titlu) + 1);
    strcpy(f.titlu, titlu);

    f.regizor = (char*)malloc(strlen(regizor) + 1);
    strcpy(f.regizor, regizor);

    f.anAparitie = anAparitie;
    f.rating = rating;

    f.gen = (char*)malloc(strlen(gen) + 1);
    strcpy(f.gen, gen);

    return f;
}

void afisareFilm(Film f) {
    printf("ID: %d\n", f.id);
    printf("Titlu: %s\n", f.titlu);
    printf("Regizor: %s\n", f.regizor);
    printf("An aparitie: %d\n", f.anAparitie);
    printf("Rating: %.1f\n", f.rating);
    printf("Gen: %s\n\n", f.gen);
}

void adaugaLaInceput(Nod** cap, Nod** coada, Film f) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = f;
    nou->prev = NULL;

    if (*cap == NULL) {
        nou->next = NULL;
        *cap = nou;
        *coada = nou;
    }
    else {
        nou->next = *cap;
        (*cap)->prev = nou;
        *cap = nou;
    }
}

void adaugaLaSfarsit(Nod** cap, Nod** coada, Film f) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = f;
    nou->next = NULL;

    if (*coada == NULL) {
        nou->prev = NULL;
        *cap = nou;
        *coada = nou;
    }
    else {
        nou->prev = *coada;
        (*coada)->next = nou;
        *coada = nou;
    }
}

void afisareListaFata(Nod* cap) {
    printf("Lista filmelor (de la inceput la sfarsit):\n");
    Nod* current = cap;

    while (current != NULL) {
        afisareFilm(current->info);
        current = current->next;
    }
}

void afisareListaSpate(Nod* coada) {
    printf("Lista filmelor (de la sfarsit la inceput):\n");
    Nod* current = coada;

    while (current != NULL) {
        afisareFilm(current->info);
        current = current->prev;
    }
}

Film* cautareFilmDupaID(Nod* cap, int id) {
    Nod* current = cap;

    while (current != NULL) {
        if (current->info.id == id) {
            return &(current->info);
        }
        current = current->next;
    }

    return NULL;
}

void afisareFilmeDupaGen(Nod* cap, const char* gen) {
    printf("Filme din genul %s:\n", gen);
    Nod* current = cap;

    while (current != NULL) {
        if (strcmp(current->info.gen, gen) == 0) {
            afisareFilm(current->info);
        }
        current = current->next;
    }
}

void filtreazaFilmeDupaAn(Nod* cap, int anMinim, int anMaxim) {
    printf("Filme aparute intre anii %d si %d:\n", anMinim, anMaxim);
    Nod* current = cap;

    while (current != NULL) {
        if (current->info.anAparitie >= anMinim && current->info.anAparitie <= anMaxim) {
            afisareFilm(current->info);
        }
        current = current->next;
    }
}

void filtreazaFilmeDupaRating(Nod* cap, float ratingMinim) {
    printf("Filme cu rating de cel putin %.1f:\n", ratingMinim);
    Nod* current = cap;

    while (current != NULL) {
        if (current->info.rating >= ratingMinim) {
            afisareFilm(current->info);
        }
        current = current->next;
    }
}

void stergereFilmDupaID(Nod** cap, Nod** coada, int id) {
    Nod* current = *cap;

    while (current != NULL && current->info.id != id) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Filmul cu ID-ul %d nu a fost gasit!\n", id);
        return;
    }

    if (current->prev != NULL) {
        current->prev->next = current->next;
    }
    else {
        *cap = current->next;
    }

    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    else {
        *coada = current->prev;
    }

    free(current->info.titlu);
    free(current->info.regizor);
    free(current->info.gen);
    free(current);

    printf("Filmul cu ID-ul %d a fost sters!\n", id);
}

void actualizareRating(Nod* cap, int id, float nouRating) {
    Film* film = cautareFilmDupaID(cap, id);

    if (film != NULL) {
        film->rating = nouRating;
        printf("Rating-ul filmului %s a fost actualizat la %.1f!\n", film->titlu, nouRating);
    }
    else {
        printf("Filmul cu ID-ul %d nu a fost gasit!\n", id);
    }
}

float calculRatingMediu(Nod* cap) {
    float suma = 0;
    int count = 0;
    Nod* current = cap;

    while (current != NULL) {
        suma += current->info.rating;
        count++;
        current = current->next;
    }

    if (count == 0) return 0;

    return suma / count;
}

void dezalocareListaFilme(Nod** cap, Nod** coada) {
    Nod* current = *cap;
    Nod* temp;

    while (current != NULL) {
        temp = current;
        current = current->next;

        free(temp->info.titlu);
        free(temp->info.regizor);
        free(temp->info.gen);
        free(temp);
    }

    *cap = NULL;
    *coada = NULL;
}

int main() {
    Nod* cap = NULL;
    Nod* coada = NULL;

    adaugaLaInceput(&cap, &coada, initializareFilm(1, "Inception", "Christopher Nolan", 2010, 8.8, "SF"));
    adaugaLaSfarsit(&cap, &coada, initializareFilm(2, "The Godfather", "Francis Ford Coppola", 1972, 9.2, "Drama"));
    adaugaLaInceput(&cap, &coada, initializareFilm(3, "The Dark Knight", "Christopher Nolan", 2008, 9.0, "Actiune"));
    adaugaLaSfarsit(&cap, &coada, initializareFilm(4, "Pulp Fiction", "Quentin Tarantino", 1994, 8.9, "Drama"));
    adaugaLaSfarsit(&cap, &coada, initializareFilm(5, "Interstellar", "Christopher Nolan", 2014, 8.6, "SF"));

    printf("===== AFISARE TOATE FILMELE =====\n");
    afisareListaFata(cap);

    printf("\n===== CAUTARE FILM DUPA ID =====\n");
    int idCautat = 3;
    Film* filmGasit = cautareFilmDupaID(cap, idCautat);

    if (filmGasit != NULL) {
        printf("Filmul cu ID-ul %d a fost gasit:\n", idCautat);
        afisareFilm(*filmGasit);
    }
    else {
        printf("Filmul cu ID-ul %d nu a fost gasit!\n", idCautat);
    }

    printf("\n===== AFISARE FILME DE GEN SF =====\n");
    afisareFilmeDupaGen(cap, "SF");

    printf("\n===== AFISARE FILME DIN PERIOADA 1990-2010 =====\n");
    filtreazaFilmeDupaAn(cap, 1990, 2010);

    printf("\n===== AFISARE FILME CU RATING PESTE 8.9 =====\n");
    filtreazaFilmeDupaRating(cap, 8.9);

    printf("\n===== CALCULARE RATING MEDIU =====\n");
    printf("Rating-ul mediu al filmelor din lista: %.1f\n", calculRatingMediu(cap));

    printf("\n===== ACTUALIZARE RATING =====\n");
    actualizareRating(cap, 5, 9.1);

    printf("\n===== STERGERE FILM =====\n");
    stergereFilmDupaID(&cap, &coada, 4);

    printf("\n===== LISTA DUPA MODIFICARI =====\n");
    afisareListaFata(cap);

    printf("\n===== LISTA IN SENS INVERS =====\n");
    afisareListaSpate(coada);

    dezalocareListaFilme(&cap, &coada);

    return 0;
}
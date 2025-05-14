#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char* nume;
    char* facultate;
    float medie;
    int anStudiu;
};
typedef struct Student Student;

struct NodAVL {
    Student info;
    struct NodAVL* stanga;
    struct NodAVL* dreapta;
    int inaltime;
};
typedef struct NodAVL NodAVL;

Student initializareStudent(int id, const char* nume, const char* facultate, float medie, int anStudiu) {
    Student s;
    s.id = id;

    s.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(s.nume, nume);

    s.facultate = (char*)malloc(strlen(facultate) + 1);
    strcpy(s.facultate, facultate);

    s.medie = medie;
    s.anStudiu = anStudiu;

    return s;
}

void afisareStudent(Student s) {
    printf("ID: %d\n", s.id);
    printf("Nume: %s\n", s.nume);
    printf("Facultate: %s\n", s.facultate);
    printf("Medie: %.2f\n", s.medie);
    printf("An de studiu: %d\n\n", s.anStudiu);
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getInaltime(NodAVL* nod) {
    if (nod == NULL)
        return 0;
    return nod->inaltime;
}

int getFactorEchilibru(NodAVL* nod) {
    if (nod == NULL)
        return 0;
    return getInaltime(nod->stanga) - getInaltime(nod->dreapta);
}

NodAVL* creareNod(Student s) {
    NodAVL* nod = (NodAVL*)malloc(sizeof(NodAVL));
    nod->info = s;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    nod->inaltime = 1;
    return nod;
}

NodAVL* rotatieDreapta(NodAVL* y) {
    NodAVL* x = y->stanga;
    NodAVL* T2 = x->dreapta;

    x->dreapta = y;
    y->stanga = T2;

    y->inaltime = max(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;
    x->inaltime = max(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;

    return x;
}

NodAVL* rotatieStanga(NodAVL* x) {
    NodAVL* y = x->dreapta;
    NodAVL* T2 = y->stanga;

    y->stanga = x;
    x->dreapta = T2;

    x->inaltime = max(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;
    y->inaltime = max(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;

    return y;
}

NodAVL* inserareStudent(NodAVL* nod, Student s) {
    if (nod == NULL)
        return creareNod(s);

    if (s.id < nod->info.id)
        nod->stanga = inserareStudent(nod->stanga, s);
    else if (s.id > nod->info.id)
        nod->dreapta = inserareStudent(nod->dreapta, s);
    else {
        printf("Studentul cu ID-ul %d exista deja!\n", s.id);
        return nod;
    }

    nod->inaltime = 1 + max(getInaltime(nod->stanga), getInaltime(nod->dreapta));

    int factorEchilibru = getFactorEchilibru(nod);

    if (factorEchilibru > 1 && s.id < nod->stanga->info.id)
        return rotatieDreapta(nod);

    if (factorEchilibru < -1 && s.id > nod->dreapta->info.id)
        return rotatieStanga(nod);

    if (factorEchilibru > 1 && s.id > nod->stanga->info.id) {
        nod->stanga = rotatieStanga(nod->stanga);
        return rotatieDreapta(nod);
    }

    if (factorEchilibru < -1 && s.id < nod->dreapta->info.id) {
        nod->dreapta = rotatieDreapta(nod->dreapta);
        return rotatieStanga(nod);
    }

    return nod;
}

void afisareInordine(NodAVL* nod) {
    if (nod != NULL) {
        afisareInordine(nod->stanga);
        afisareStudent(nod->info);
        afisareInordine(nod->dreapta);
    }
}

void afisareStudentiFacultate(NodAVL* nod, const char* facultate) {
    if (nod != NULL) {
        afisareStudentiFacultate(nod->stanga, facultate);

        if (strcmp(nod->info.facultate, facultate) == 0) {
            afisareStudent(nod->info);
        }

        afisareStudentiFacultate(nod->dreapta, facultate);
    }
}

float calculMedieStudenti(NodAVL* nod) {
    if (nod == NULL)
        return 0;

    static float sumaNote = 0;
    static int numarStudenti = 0;

    sumaNote += nod->info.medie;
    numarStudenti++;

    calculMedieStudenti(nod->stanga);
    calculMedieStudenti(nod->dreapta);

    return sumaNote / numarStudenti;
}

Student* cautareStudentDupaID(NodAVL* nod, int id) {
    if (nod == NULL)
        return NULL;

    if (nod->info.id == id)
        return &(nod->info);

    if (id < nod->info.id)
        return cautareStudentDupaID(nod->stanga, id);
    else
        return cautareStudentDupaID(nod->dreapta, id);
}

void afisareStudentiPeAnDeStudiu(NodAVL* nod, int an) {
    if (nod != NULL) {
        afisareStudentiPeAnDeStudiu(nod->stanga, an);

        if (nod->info.anStudiu == an) {
            afisareStudent(nod->info);
        }

        afisareStudentiPeAnDeStudiu(nod->dreapta, an);
    }
}

void modificareMedie(NodAVL* nod, int id, float nouaMedie) {
    Student* student = cautareStudentDupaID(nod, id);

    if (student != NULL) {
        student->medie = nouaMedie;
        printf("Media studentului cu ID %d a fost actualizata la %.2f\n", id, nouaMedie);
    }
    else {
        printf("Studentul cu ID %d nu a fost gasit!\n", id);
    }
}

void dezalocareArbore(NodAVL** nod) {
    if (*nod != NULL) {
        dezalocareArbore(&((*nod)->stanga));
        dezalocareArbore(&((*nod)->dreapta));

        free((*nod)->info.nume);
        free((*nod)->info.facultate);
        free(*nod);
        *nod = NULL;
    }
}

int main() {
    NodAVL* radacina = NULL;

    radacina = inserareStudent(radacina, initializareStudent(101, "Popescu Ion", "Informatica", 9.5, 2));
    radacina = inserareStudent(radacina, initializareStudent(205, "Ionescu Maria", "Matematica", 8.7, 3));
    radacina = inserareStudent(radacina, initializareStudent(150, "Georgescu Andrei", "Informatica", 7.8, 1));
    radacina = inserareStudent(radacina, initializareStudent(302, "Popa Elena", "Fizica", 9.2, 2));
    radacina = inserareStudent(radacina, initializareStudent(178, "Dumitrescu Mihai", "Informatica", 8.3, 2));

    printf("Lista tuturor studentilor (inordine):\n");
    afisareInordine(radacina);

    printf("\nStudenti din facultatea de Informatica:\n");
    afisareStudentiFacultate(radacina, "Informatica");

    printf("\nStudenti din anul 2:\n");
    afisareStudentiPeAnDeStudiu(radacina, 2);

    int idCautat = 150;
    Student* studentGasit = cautareStudentDupaID(radacina, idCautat);

    if (studentGasit != NULL) {
        printf("\nStudentul cu ID-ul %d a fost gasit:\n", idCautat);
        afisareStudent(*studentGasit);
    }
    else {
        printf("\nStudentul cu ID-ul %d nu a fost gasit.\n", idCautat);
    }

    printf("\nActualizare medie:\n");
    modificareMedie(radacina, 150, 9.0);

    studentGasit = cautareStudentDupaID(radacina, idCautat);
    if (studentGasit != NULL) {
        printf("\nStudentul cu ID-ul %d dupa actualizare:\n", idCautat);
        afisareStudent(*studentGasit);
    }

    printf("\nMedia generala a studentilor: %.2f\n", calculMedieStudenti(radacina));

    dezalocareArbore(&radacina);

    return 0;
}
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

struct NodArbore {
    Masina info;
    struct NodArbore* stanga;
    struct NodArbore* dreapta;
    int inaltime;
};
typedef struct NodArbore NodArbore;

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

int max(int a, int b) {
    return (a > b) ? a : b;
}

int getInaltime(NodArbore* nod) {
    if (nod == NULL)
        return 0;
    return nod->inaltime;
}

int getFactorEchilibru(NodArbore* nod) {
    if (nod == NULL)
        return 0;
    return getInaltime(nod->stanga) - getInaltime(nod->dreapta);
}

NodArbore* creareNod(Masina masina) {
    NodArbore* nod = (NodArbore*)malloc(sizeof(NodArbore));
    nod->info = masina;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    nod->inaltime = 1;
    return nod;
}

NodArbore* rotatieDreapta(NodArbore* y) {
    NodArbore* x = y->stanga;
    NodArbore* T2 = x->dreapta;

    x->dreapta = y;
    y->stanga = T2;

    y->inaltime = max(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;
    x->inaltime = max(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;

    return x;
}

NodArbore* rotatieStanga(NodArbore* x) {
    NodArbore* y = x->dreapta;
    NodArbore* T2 = y->stanga;

    y->stanga = x;
    x->dreapta = T2;

    x->inaltime = max(getInaltime(x->stanga), getInaltime(x->dreapta)) + 1;
    y->inaltime = max(getInaltime(y->stanga), getInaltime(y->dreapta)) + 1;

    return y;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** radacina, Masina masinaNoua) {
    if (*radacina == NULL) {
        *radacina = creareNod(masinaNoua);
        return;
    }

    if (masinaNoua.id < (*radacina)->info.id)
        adaugaMasinaInArboreEchilibrat(&((*radacina)->stanga), masinaNoua);
    else if (masinaNoua.id > (*radacina)->info.id)
        adaugaMasinaInArboreEchilibrat(&((*radacina)->dreapta), masinaNoua);
    else {
        printf("Masina cu ID-ul %d exista deja in arbore!\n", masinaNoua.id);
        return;
    }

    (*radacina)->inaltime = 1 + max(getInaltime((*radacina)->stanga), getInaltime((*radacina)->dreapta));

    int factorEchilibru = getFactorEchilibru(*radacina);

    if (factorEchilibru > 1 && masinaNoua.id < (*radacina)->stanga->info.id)
        *radacina = rotatieDreapta(*radacina);

    else if (factorEchilibru < -1 && masinaNoua.id >(*radacina)->dreapta->info.id)
        *radacina = rotatieStanga(*radacina);

    else if (factorEchilibru > 1 && masinaNoua.id > (*radacina)->stanga->info.id) {
        (*radacina)->stanga = rotatieStanga((*radacina)->stanga);
        *radacina = rotatieDreapta(*radacina);
    }

    else if (factorEchilibru < -1 && masinaNoua.id < (*radacina)->dreapta->info.id) {
        (*radacina)->dreapta = rotatieDreapta((*radacina)->dreapta);
        *radacina = rotatieStanga(*radacina);
    }
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        return NULL;
    }

    NodArbore* radacina = NULL;
    char buffer[100];

    while (fgets(buffer, 100, f) != NULL) {
        fseek(f, -strlen(buffer), SEEK_CUR);
        Masina m = citireMasinaDinFisier(f);
        adaugaMasinaInArboreEchilibrat(&radacina, m);
    }

    fclose(f);
    return radacina;
}

void afisareInordine(NodArbore* radacina) {
    if (radacina != NULL) {
        afisareInordine(radacina->stanga);
        afisareMasina(radacina->info);
        afisareInordine(radacina->dreapta);
    }
}

void afisarePreordine(NodArbore* radacina) {
    if (radacina != NULL) {
        afisareMasina(radacina->info);
        afisarePreordine(radacina->stanga);
        afisarePreordine(radacina->dreapta);
    }
}

void afisarePostordine(NodArbore* radacina) {
    if (radacina != NULL) {
        afisarePostordine(radacina->stanga);
        afisarePostordine(radacina->dreapta);
        afisareMasina(radacina->info);
    }
}

void afisareMasiniDinArbore(NodArbore* radacina) {
    printf("Parcurgere INORDINE:\n");
    afisareInordine(radacina);

    printf("\nParcurgere PREORDINE:\n");
    afisarePreordine(radacina);

    printf("\nParcurgere POSTORDINE:\n");
    afisarePostordine(radacina);
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
    if (*radacina != NULL) {
        dezalocareArboreDeMasini(&((*radacina)->stanga));
        dezalocareArboreDeMasini(&((*radacina)->dreapta));

        free((*radacina)->info.model);
        free((*radacina)->info.numeSofer);
        free(*radacina);
        *radacina = NULL;
    }
}

Masina getMasinaByID(NodArbore* radacina, int id) {
    Masina m;
    m.id = -1;
    m.model = NULL;
    m.numeSofer = NULL;

    if (radacina == NULL) {
        return m;
    }

    if (radacina->info.id == id) {
        return radacina->info;
    }
    else if (id < radacina->info.id) {
        return getMasinaByID(radacina->stanga, id);
    }
    else {
        return getMasinaByID(radacina->dreapta, id);
    }
}

int determinaNumarNoduri(NodArbore* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    return radacina->inaltime;
}

float calculeazaPretTotal(NodArbore* radacina) {
    if (radacina == NULL) {
        return 0;
    }

    return radacina->info.pret + calculeazaPretTotal(radacina->stanga) +
        calculeazaPretTotal(radacina->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* radacina, const char* numeSofer) {
    if (radacina == NULL) {
        return 0;
    }

    float total = 0;

    if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
        total += radacina->info.pret;
    }

    total += calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer);
    total += calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);

    return total;
}

int main() {
    NodArbore* radacina = (NodArbore*)citireArboreDeMasiniDinFisier("masini.txt");

    printf("Afisare masini din arbore AVL:\n");
    afisareMasiniDinArbore(radacina);

    printf("\nNumar total de noduri: %d\n", determinaNumarNoduri(radacina));

    printf("Inaltimea arborelui: %d\n", calculeazaInaltimeArbore(radacina));

    printf("Pretul total al masinilor: %.2f\n", calculeazaPretTotal(radacina));

    const char* numeSoferCautat = "Dan Petre";
    printf("Pretul total al masinilor soferului %s: %.2f\n",
        numeSoferCautat, calculeazaPretulMasinilorUnuiSofer(radacina, numeSoferCautat));

    int idCautat = 3;
    Masina masinaCautata = getMasinaByID(radacina, idCautat);

    if (masinaCautata.id != -1) {
        printf("\nMasina cu ID-ul %d a fost gasita:\n", idCautat);
        afisareMasina(masinaCautata);
    }
    else {
        printf("\nMasina cu ID-ul %d nu a fost gasita.\n", idCautat);
    }

    dezalocareArboreDeMasini(&radacina);

    return 0;
}
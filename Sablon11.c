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

struct NodListaSecundara {
    int idDestinatie;
    struct NodListaSecundara* next;
};
typedef struct NodListaSecundara NodListaSecundara;

struct NodListaPrincipala {
    Masina masina;
    NodListaSecundara* listaAdiacenta;
    struct NodListaPrincipala* next;
};
typedef struct NodListaPrincipala NodListaPrincipala;

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

NodListaPrincipala* inserareNodPrincipal(NodListaPrincipala* lista, Masina masina) {
    NodListaPrincipala* nodNou = malloc(sizeof(NodListaPrincipala));
    nodNou->masina = masina;
    nodNou->listaAdiacenta = NULL;
    nodNou->next = lista;
    return nodNou;
}

NodListaSecundara* inserareNodSecundar(NodListaSecundara* lista, int id) {
    NodListaSecundara* nodNou = malloc(sizeof(NodListaSecundara));
    nodNou->idDestinatie = id;
    nodNou->next = lista;
    return nodNou;
}

NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* listaPrincipala, int id) {
    NodListaPrincipala* curent = listaPrincipala;
    while (curent != NULL) {
        if (curent->masina.id == id) {
            return curent;
        }
        curent = curent->next;
    }
    return NULL;
}

void inserareMuchie(NodListaPrincipala* listaPrincipala, int idStart, int idStop) {
    NodListaPrincipala* nodStart = cautaNodDupaID(listaPrincipala, idStart);
    if (nodStart != NULL) {
        nodStart->listaAdiacenta = inserareNodSecundar(nodStart->listaAdiacenta, idStop);
    }
}

NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului %s!\n", numeFisier);
        return NULL;
    }
    
    NodListaPrincipala* listaPrincipala = NULL;
    
    while (!feof(file)) {
        Masina masina = citireMasinaDinFisier(file);
        if (!feof(file)) {
            listaPrincipala = inserareNodPrincipal(listaPrincipala, masina);
        }
    }
    
    fclose(file);
    return listaPrincipala;
}

void citireMuchiiDinFisier(NodListaPrincipala* listaPrincipala, const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Eroare la deschiderea fisierului %s!\n", numeFisier);
        return;
    }
    
    int idStart, idStop;
    while (fscanf(file, "%d,%d", &idStart, &idStop) == 2) {
        inserareMuchie(listaPrincipala, idStart, idStop);
    }
    
    fclose(file);
}

void dezalocareListaSecundara(NodListaSecundara* lista) {
    while (lista != NULL) {
        NodListaSecundara* temp = lista;
        lista = lista->next;
        free(temp);
    }
}

void dezalocareNoduriGraf(NodListaPrincipala* listaPrincipala) {
    while (listaPrincipala != NULL) {
        NodListaPrincipala* temp = listaPrincipala;
        listaPrincipala = listaPrincipala->next;
        
        free(temp->masina.model);
        free(temp->masina.numeSofer);
        
        dezalocareListaSecundara(temp->listaAdiacenta);
        
        free(temp);
    }
}

void afisareGraf(NodListaPrincipala* listaPrincipala) {
    NodListaPrincipala* curent = listaPrincipala;
    
    while (curent != NULL) {
        printf("Nod ID %d (Model: %s) -> ", curent->masina.id, curent->masina.model);
        
        NodListaSecundara* adiacent = curent->listaAdiacenta;
        if (adiacent == NULL) {
            printf("NULL");
        } else {
            while (adiacent != NULL) {
                printf("%d", adiacent->idDestinatie);
                if (adiacent->next != NULL) {
                    printf(" -> ");
                }
                adiacent = adiacent->next;
            }
        }
        printf("\n");
        curent = curent->next;
    }
    printf("\n");
}

void afisareDetaliiNoduri(NodListaPrincipala* listaPrincipala) {
    NodListaPrincipala* curent = listaPrincipala;
    
    while (curent != NULL) {
        afisareMasina(curent->masina);
        curent = curent->next;
    }
}

int numaraGradulExtern(NodListaPrincipala* listaPrincipala, int id) {
    NodListaPrincipala* nod = cautaNodDupaID(listaPrincipala, id);
    if (nod == NULL) return 0;
    
    int grad = 0;
    NodListaSecundara* curent = nod->listaAdiacenta;
    while (curent != NULL) {
        grad++;
        curent = curent->next;
    }
    return grad;
}

int main() {
    NodListaPrincipala* graf = citireNoduriMasiniDinFisier("masini.txt");
    
    if (graf == NULL) {
        printf("Nu s-au putut citi datele din fisier!\n");
        return 1;
    }
    
    
    afisareDetaliiNoduri(graf);
    
    citireMuchiiDinFisier(graf, "muchii.txt");
    
    afisareGraf(graf);
    
    int idCautat = 1;
    NodListaPrincipala* nodGasit = cautaNodDupaID(graf, idCautat);
    if (nodGasit != NULL) {
        printf("Nodul cu ID %d a fost gasit: %s\n", idCautat, nodGasit->masina.model);
        printf("Gradul extern al nodului: %d\n\n", numaraGradulExtern(graf, idCautat));
    } else {
        printf("Nodul cu ID %d nu a fost gasit!\n\n", idCautat);
    }
    
    dezalocareNoduriGraf(graf);
    
    return 0;
}
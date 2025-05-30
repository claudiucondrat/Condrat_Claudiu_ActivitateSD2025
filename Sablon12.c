#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

struct NodStiva {
    int id;
    struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

struct Stiva {
    NodStiva* top;
};
typedef struct Stiva Stiva;

struct NodCoada {
    int id;
    struct NodCoada* next;
};
typedef struct NodCoada NodCoada;

struct Coada {
    NodCoada* front;
    NodCoada* rear;
};
typedef struct Coada Coada;

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

Stiva* creeazaStiva() {
    Stiva* stiva = malloc(sizeof(Stiva));
    stiva->top = NULL;
    return stiva;
}

void push(Stiva* stiva, int id) {
    NodStiva* nodNou = malloc(sizeof(NodStiva));
    nodNou->id = id;
    nodNou->next = stiva->top;
    stiva->top = nodNou;
}

int pop(Stiva* stiva) {
    if (stiva->top == NULL) return -1;
    NodStiva* temp = stiva->top;
    int id = temp->id;
    stiva->top = stiva->top->next;
    free(temp);
    return id;
}

bool esteGoalaStiva(Stiva* stiva) {
    return stiva->top == NULL;
}

void dezalocareStiva(Stiva* stiva) {
    while (!esteGoalaStiva(stiva)) {
        pop(stiva);
    }
    free(stiva);
}

Coada* creeazaCoada() {
    Coada* coada = malloc(sizeof(Coada));
    coada->front = NULL;
    coada->rear = NULL;
    return coada;
}

void enqueue(Coada* coada, int id) {
    NodCoada* nodNou = malloc(sizeof(NodCoada));
    nodNou->id = id;
    nodNou->next = NULL;
    
    if (coada->rear == NULL) {
        coada->front = nodNou;
        coada->rear = nodNou;
    } else {
        coada->rear->next = nodNou;
        coada->rear = nodNou;
    }
}

int dequeue(Coada* coada) {
    if (coada->front == NULL) return -1;
    NodCoada* temp = coada->front;
    int id = temp->id;
    coada->front = coada->front->next;
    if (coada->front == NULL) {
        coada->rear = NULL;
    }
    free(temp);
    return id;
}

bool esteGoalaCoada(Coada* coada) {
    return coada->front == NULL;
}

void dezalocareCoada(Coada* coada) {
    while (!esteGoalaCoada(coada)) {
        dequeue(coada);
    }
    free(coada);
}

int calculeazaNrNoduriGraf(NodListaPrincipala* listaPrincipala) {
    int numar = 0;
    NodListaPrincipala* curent = listaPrincipala;
    while (curent != NULL) {
        numar++;
        curent = curent->next;
    }
    return numar;
}

void afisareGrafInAdancime(NodListaPrincipala* listaPrincipala, int idPlecare) {
    int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
    if (nrNoduri == 0) {
        return;
    }
    
    int* idNoduri = malloc(nrNoduri * sizeof(int));
    bool* vizitat = malloc(nrNoduri * sizeof(bool));
    
    NodListaPrincipala* curent = listaPrincipala;
    for (int i = 0; i < nrNoduri; i++) {
        idNoduri[i] = curent->masina.id;
        vizitat[i] = false;
        curent = curent->next;
    }
    
    Stiva* stiva = creeazaStiva();
    
    int indexPlecare = -1;
    for (int i = 0; i < nrNoduri; i++) {
        if (idNoduri[i] == idPlecare) {
            indexPlecare = i;
            break;
        }
    }
    
    if (indexPlecare == -1) {
        free(idNoduri);
        free(vizitat);
        dezalocareStiva(stiva);
        return;
    }
    
    vizitat[indexPlecare] = true;
    push(stiva, idPlecare);
    
    while (!esteGoalaStiva(stiva)) {
        int idCurent = pop(stiva);
        printf("%d ", idCurent);
        
        NodListaPrincipala* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
        if (nodCurent != NULL) {
            NodListaSecundara* vecin = nodCurent->listaAdiacenta;
            while (vecin != NULL) {
                int indexVecin = -1;
                for (int i = 0; i < nrNoduri; i++) {
                    if (idNoduri[i] == vecin->idDestinatie) {
                        indexVecin = i;
                        break;
                    }
                }
                
                if (indexVecin != -1 && !vizitat[indexVecin]) {
                    vizitat[indexVecin] = true;
                    push(stiva, vecin->idDestinatie);
                }
                
                vecin = vecin->next;
            }
        }
    }
    
    printf("\n\n");
    
    free(idNoduri);
    free(vizitat);
    dezalocareStiva(stiva);
}

void afisareGrafInLatime(NodListaPrincipala* listaPrincipala, int idPlecare) {
    
    int nrNoduri = calculeazaNrNoduriGraf(listaPrincipala);
    if (nrNoduri == 0) {
        return;
    }
    
    int* idNoduri = malloc(nrNoduri * sizeof(int));
    bool* vizitat = malloc(nrNoduri * sizeof(bool));
    
    NodListaPrincipala* curent = listaPrincipala;
    for (int i = 0; i < nrNoduri; i++) {
        idNoduri[i] = curent->masina.id;
        vizitat[i] = false;
        curent = curent->next;
    }
    
    Coada* coada = creeazaCoada();
    
    int indexPlecare = -1;
    for (int i = 0; i < nrNoduri; i++) {
        if (idNoduri[i] == idPlecare) {
            indexPlecare = i;
            break;
        }
    }
    
    if (indexPlecare == -1) {
        free(idNoduri);
        free(vizitat);
        dezalocareCoada(coada);
        return;
    }
    
    vizitat[indexPlecare] = true;
    enqueue(coada, idPlecare);
    
    
    while (!esteGoalaCoada(coada)) {
        int idCurent = dequeue(coada);
        printf("%d ", idCurent);
        
        NodListaPrincipala* nodCurent = cautaNodDupaID(listaPrincipala, idCurent);
        if (nodCurent != NULL) {
            NodListaSecundara* vecin = nodCurent->listaAdiacenta;
            while (vecin != NULL) {
                int indexVecin = -1;
                for (int i = 0; i < nrNoduri; i++) {
                    if (idNoduri[i] == vecin->idDestinatie) {
                        indexVecin = i;
                        break;
                    }
                }
                
                if (indexVecin != -1 && !vizitat[indexVecin]) {
                    vizitat[indexVecin] = true;
                    enqueue(coada, vecin->idDestinatie);
                }
                
                vecin = vecin->next;
            }
        }
    }
    
    printf("\n\n");
    
    free(idNoduri);
    free(vizitat);
    dezalocareCoada(coada);
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

int main() {
    NodListaPrincipala* graf = citireNoduriMasiniDinFisier("masini.txt");
    
    if (graf == NULL) {
        printf("Nu s-au putut citi datele din fisier!\n");
        return 1;
    }
    
    citireMuchiiDinFisier(graf, "muchii.txt");
    
    afisareGraf(graf);
    
    printf("Numarul total de noduri: %d\n\n", calculeazaNrNoduriGraf(graf));
    
    afisareGrafInAdancime(graf, 1);
    afisareGrafInLatime(graf, 1);
    
    dezalocareNoduriGraf(graf);
    
    return 0;
}
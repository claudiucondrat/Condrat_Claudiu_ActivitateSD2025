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

typedef struct NodLD {
	Masina masina;
	struct NodLD* prev;
	struct NodLD* next;
} NodLD;

typedef struct ListaDubla {
	NodLD* prim;
	NodLD* ultim;
} ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (!fgets(buffer, 100, file)) {
		Masina m = { 0 };
		return m;
	}
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

void afisareListaMasini(ListaDubla lista) {
	NodLD* p = lista.prim;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	NodLD* nodNou = (NodLD*)malloc(sizeof(NodLD));
	nodNou->masina = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->ultim;

	if (lista->ultim != NULL)
		lista->ultim->next = nodNou;
	else
		lista->prim = nodNou;

	lista->ultim = nodNou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	NodLD* nodNou = (NodLD*)malloc(sizeof(NodLD));
	nodNou->masina = masinaNoua;
	nodNou->prev = NULL;
	nodNou->next = lista->prim;

	if (lista->prim != NULL)
		lista->prim->prev = nodNou;
	else
		lista->ultim = nodNou;

	lista->prim = nodNou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	ListaDubla lista = { NULL, NULL };

	if (!file) {
		printf("Nu s-a putut deschide fisierul.\n");
		return lista;
	}

	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		if (m.model != NULL && m.numeSofer != NULL)
			adaugaMasinaInLista(&lista, m);
	}
	fclose(file);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	NodLD* p = lista->prim;
	while (p) {
		NodLD* temp = p;
		p = p->next;
		free(temp->masina.model);
		free(temp->masina.numeSofer);
		free(temp);
	}
	lista->prim = lista->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
	float suma = 0;
	int count = 0;
	NodLD* p = lista.prim;
	while (p) {
		suma += p->masina.pret;
		count++;
		p = p->next;
	}
	return (count > 0) ? (suma / count) : 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	NodLD* p = lista->prim;
	while (p) {
		if (p->masina.id == id) {
			if (p->prev)
				p->prev->next = p->next;
			else
				lista->prim = p->next;

			if (p->next)
				p->next->prev = p->prev;
			else
				lista->ultim = p->prev;

			free(p->masina.model);
			free(p->masina.numeSofer);
			free(p);
			return;
		}
		p = p->next;
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	NodLD* p = lista.prim;
	Masina* maxMasina = NULL;
	while (p) {
		if (!maxMasina || p->masina.pret > maxMasina->pret)
			maxMasina = &p->masina;
		p = p->next;
	}
	if (maxMasina) {
		char* rezultat = malloc(strlen(maxMasina->numeSofer) + 1);
		strcpy(rezultat, maxMasina->numeSofer);
		return rezultat;
	}
	return NULL;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");

	printf("Lista citita din fisier:\n");
	afisareListaMasini(lista);

	printf("\nPret mediu: %.2f\n", calculeazaPretMediu(lista));

	printf("\nStergem masina cu ID = 2...\n");
	stergeMasinaDupaID(&lista, 2);
	afisareListaMasini(lista);

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	if (numeSofer) {
		printf("\nNume sofer masina cea mai scumpa: %s\n", numeSofer);
		free(numeSofer);
	}

	dezalocareLDMasini(&lista);
	return 0;
}

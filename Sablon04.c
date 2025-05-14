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

typedef struct Nod {
	Masina masina;
	struct Nod* next;
} Nod;

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

void afisareListaMasini(Nod* cap) {
	Nod* p = cap;
	while (p != NULL) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->masina = masinaNoua;
	nodNou->next = NULL;

	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		Nod* p = *cap;
		while (p->next != NULL)
			p = p->next;
		p->next = nodNou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->masina = masinaNoua;
	nodNou->next = *cap;
	*cap = nodNou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Nu s-a putut deschide fisierul.\n");
		return NULL;
	}
	Nod* cap = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		if (m.model != NULL && m.numeSofer != NULL)
			adaugaMasinaInLista(&cap, m);
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod* cap) {
	Nod* temp;
	while (cap) {
		temp = cap;
		cap = cap->next;
		free(temp->masina.model);
		free(temp->masina.numeSofer);
		free(temp);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int count = 0;
	while (cap) {
		suma += cap->masina.pret;
		count++;
		cap = cap->next;
	}
	return (count > 0) ? (suma / count) : 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	Nod* p = *cap;
	Nod* prev = NULL;

	while (p != NULL) {
		if (p->masina.serie == serieCautata) {
			if (prev == NULL) {
				*cap = p->next;
				free(p->masina.model);
				free(p->masina.numeSofer);
				free(p);
				p = *cap;
			}
			else {
				prev->next = p->next;
				free(p->masina.model);
				free(p->masina.numeSofer);
				free(p);
				p = prev->next;
			}
		}
		else {
			prev = p;
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->masina.numeSofer, numeSofer) == 0)
			suma += cap->masina.pret;
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* listaMasini = citireListaMasiniDinFisier("masini.txt");
	printf("Lista initiala:\n");
	afisareListaMasini(listaMasini);

	printf("Pret mediu: %.2f\n", calculeazaPretMediu(listaMasini));

	printf("\nStergere masini cu seria 'A':\n");
	stergeMasiniDinSeria(&listaMasini, 'A');
	afisareListaMasini(listaMasini);

	printf("\nPret total masini pentru soferul Claudiu: %.2f\n",
		calculeazaPretulMasinilorUnuiSofer(listaMasini, "Claudiu"));

	dezalocareListaMasini(listaMasini);
	return 0;
}

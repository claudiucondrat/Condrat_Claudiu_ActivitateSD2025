#include<stdio.h>
#include<malloc.h>

struct Masina {
	int id;
	char* model;
	float tonaj;
	char serie;
};
struct Masina initializare(int idNou, const char* modelNou, float tonajNou, char serieNou) {
	struct Masina m;
	m.id = idNou;
	m.model = malloc(sizeof(char) * (strlen(modelNou) + 1));
	strcpy_s(m.model, strlen(modelNou) + 1, modelNou);
	m.tonaj = tonajNou;
	m.serie = serieNou;

	return m;
}
void afisare(struct Masina m) {
	printf("Masina cu id-ul %d are modelul %s, tonajul%5.2f si seria %c.\n", m.id, m.model, m.tonaj, m.serie);
}

void afisareVector(struct Masina* vector, int nrElemente) {
	printf("Afisare vector: \n");
	for (int i = 0;i < nrElemente;i++)
		afisare(vector[i]);
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	struct Masina* vectorNou = malloc(sizeof(struct Masina) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++)
	{
		vectorNou[i] = vector[i];
		vectorNou[i].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
		strcpy_s(vectorNou[i].model, strlen(vector[i].model) + 1, vector[i].model);
	}
	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++)
	{
		free((*vector)[i].model);
	}		
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	(*dimensiune) = 0;
	(*vectorNou) = malloc(sizeof(struct Masina) * nrElemente);
	for (int i = 0;i < nrElemente;i++)
		if (vector[i].tonaj > prag)
		{
			(*vectorNou)[*dimensiune] = vector[i];
			(*vectorNou)[*dimensiune].model = malloc(sizeof(char) * (strlen(vector[i].model) + 1));
			strcpy_s((*vectorNou)[*dimensiune].model, strlen(vector[i].model) + 1, vector[i].model);
			(*dimensiune)++;
		}
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina s;
	s.id = 1;
	return s;
}



int main() {
	struct Masina m1 = initializare(1, "Mercedes", 1.5, 'G');
	struct Masina m2 = initializare(2, "BMW", 2.5, 'X');
	afisare(m1);
	afisare(m2);
	int nrMasini = 3;
	struct Masina* vectorMasini = malloc(sizeof(struct Masina) * nrMasini);
	vectorMasini[0] = initializare(1, "Mercedes", 1.5, 'G');
	vectorMasini[1] = initializare(2, "BMW", 2.5, 'X');
	vectorMasini[2] = initializare(3, "Audi", 1.8, 'R');
	afisareVector(vectorMasini, nrMasini);
	struct Masina* vectorMasiniCopiate = NULL;
	int nrElementeCopiate = 2;
	vectorMasiniCopiate = copiazaPrimeleNElemente(vectorMasini, nrMasini, nrElementeCopiate);
	printf("\nMasini copiate:\n");
	afisareVector(vectorMasiniCopiate, nrElementeCopiate);
	dezalocare(&vectorMasiniCopiate, &nrElementeCopiate);

	printf("\nMasini usoare copiate: \n");
	copiazaAnumiteElemente(vectorMasini, nrMasini, 2.0, &vectorMasini, &nrMasini);
	afisareVector(vectorMasini, nrMasini);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#define MAX 256

data_structure* alocare_structura(char *text)
{
	// Aloc memorie pentru data_structure, in care voi salva informatiile despre dedicatie
	data_structure *data = malloc(sizeof(data_structure));
	if (!data) {
		return NULL;
	}

	data->header = malloc(sizeof(head));
	if (!data->header) {
		free(data);
		return NULL;
	}

	// Prelucrez textul primit, salvand diferitele informatii in pointeri
	unsigned char type = data->header->type = atoi(text);

	char *name1 = strtok(NULL, " ");
	text = strtok(NULL, " ");
	int val1 = atoi(text);
	text = strtok(NULL, " ");
	int val2 = atoi(text);
	char *name2 = strtok(NULL, " ");

	data->header->len = strlen(name1) + strlen(name2) + 2;

	// Salvez datele in functie de tip
	if (type == 1) {
		data->header->len += 2 * sizeof(int8_t);

		data->data = malloc(data->header->len);
		if (!data->data) {
			free(data->header);
			free(data);
			return NULL;
		}

		int len = 0;

		memcpy(data->data, name1, strlen(name1) + 1);
		len += strlen(name1) + 1;
		memcpy(data->data + len, &val1, sizeof(int8_t));
		len += sizeof(int8_t);
		memcpy(data->data + len, &val2, sizeof(int8_t));
		len += sizeof(int8_t);
		memcpy(data->data + len, name2, strlen(name2) + 1);
	} else if (type == 2) {
		data->header->len += sizeof(int16_t) + sizeof(int32_t);

		data->data = malloc(data->header->len);
		if (!data->data) {
			free(data->header);
			free(data);
			return NULL;
		}

		int len = 0;

		memcpy(data->data, name1, strlen(name1) + 1);
		len += strlen(name1) + 1;
		memcpy(data->data + len, &val1, sizeof(int16_t));
		len += sizeof(int16_t);
		memcpy(data->data + len, &val2, sizeof(int32_t));
		len += sizeof(int32_t);
		memcpy(data->data + len, name2, strlen(name2) + 1);
	} else if (type == 3) {
		data->header->len += 2 * sizeof(int32_t);

		data->data = malloc(data->header->len);
		if (!data->data) {
			free(data->header);
			free(data);
			return NULL;
		}

		int len = 0;

		memcpy(data->data, name1, strlen(name1) + 1);
		len += strlen(name1) + 1;
		memcpy(data->data + len, &val1, sizeof(int32_t));
		len += sizeof(int32_t);
		memcpy(data->data + len, &val2, sizeof(int32_t));
		len += sizeof(int32_t);
		memcpy(data->data + len, name2, strlen(name2) + 1);
	}

	// Returnez adresa catre structura alocata
	return data;
}

void afisare(void *data_block, head *header, int *size)
{
	unsigned char type = header->type;

	// Afisez tipul
	printf("Tipul %d\n", type);

	// Declar pointerii catre cele doua nume
	char *name1, *name2;

	name1 = data_block + *size;

	(*size) += strlen(name1) + 1;

	// Variabilele utilizate pentru salvarea bancnotelor in functie de tipul lor
	int8_t val1;
	int8_t val2;
	int16_t val3;
	int32_t val4;
	int32_t val5;

	// Salvez in variabilele anterioare valoarea numerica a bancnotelor in functie de tip
	if (type == 1) {
		memcpy(&val1, data_block + *size, sizeof(int8_t));
		++(*size);
		memcpy(&val2, data_block + *size, sizeof(int8_t));
		++(*size);
	} else if (type == 2) {
		memcpy(&val3, data_block + *size, sizeof(int16_t));
		(*size) += 2;
		memcpy(&val4, data_block + *size, sizeof(int32_t));
		(*size) += 4;
	} else if (type == 3) {
		memcpy(&val4, data_block + *size, sizeof(int32_t));
		(*size) += 4;
		memcpy(&val5, data_block + *size, sizeof(int32_t));
		(*size) += 4;
	}

	name2 = data_block + *size;

	(*size) += strlen(name2) + 1;

	printf("%s pentru %s\n", name1, name2);

	if (type == 1) {
		printf("%d\n%d\n\n", val1, val2);
	} else if (type == 2) {
		printf("%d\n%d\n\n", val3, val4);
	} else if (type == 3) {
		printf("%d\n%d\n\n", val4, val5);
	}
}

// Functie care determina numarul de octeti care sunt alocati vectorului generic
int get_nr_of_bytes(void *data_block, int len)
{
	int poz = 0; // pozitia curenta ca index
	int cur_bytes = 0; // numarul curent de octeti

	// Aloc memorie pentru header
	head *header = malloc(sizeof(head));
	if (!header) {
		return -1;
	}

	// Parcurg tot vectorul pentru a-i numara octetii
	while (poz < len) {
		memcpy(header, data_block + cur_bytes, sizeof(head));
		cur_bytes += sizeof(head) + header->len;

		++poz;
	}
	// Eliberez memoria alocata pentru header
	free(header);

	return cur_bytes;
}

int add_last(void **arr, int *len, data_structure *data)
{
	int nr_of_bytes = get_nr_of_bytes(*arr, *len);

	// Verific daca mai intai arr a fost alocat sau nu. Daca nu a fost alocat, il aloc, daca a fost alocat,
	// il redimensionez.
	if (!(*arr)) {
		// Alocarea
		(*arr) = malloc(data->header->len + sizeof(head));

		if (!(*arr)) {
			printf("Nu s-a putut aloca vectorul!\n");
			return 1;
		}
	} else {
		// Redimensionarea
		void *aux;
		aux = realloc(*arr, nr_of_bytes + data->header->len + sizeof(head));
		
		if (!aux) {
			printf("Nu s-a putut realoca vectorul!\n");
			return 1;
		}
		*arr = aux;
	}

	// Adaug in arr informatiile catre care puncteaza data
	memcpy(*arr + nr_of_bytes, data->header, sizeof(head));
	memcpy(*arr + nr_of_bytes + sizeof(head), data->data, data->header->len);

	++(*len);
	
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	// Tratez conditiile mentionate in enunt
	if (index < 0) {
		return 0;
	}
	if (index > *len) {
		return add_last(arr, len, data);
	}

	// Aloc memorie pentru header, pentru a putea citi din arr
	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In functia add_at nu s-a putut aloca headerul!\n");
		return 1;
	}

	int size = get_nr_of_bytes(*arr, *len);
	int new_size = size + sizeof(head) + data->header->len;
	
	// Redimensionez vectorul dupa noua lungime
	void *aux = realloc(*arr, new_size);
	if (!aux) {
		free(header);
		return 1;
	}
	*arr = aux;

	int poz = 0;
	int len2 = 0;

	// Parcurg primele index - 1 dedicatii
	while (poz < index) {
		memcpy(header, *arr + len2, sizeof(head));
		len2 += header->len + sizeof(head);
		
		poz++;
	}

	void *arr2 = malloc(new_size);
	if (!arr2) {
		free(header);
		return 1;
	}

	// Copiez vectorul vechi in vectorul auxiliar arr2
	memcpy(arr2, *arr, new_size);
	// Mut elementele incepand de la elementul index mai la dreapta
	memcpy(*arr + len2 + sizeof(head) + data->header->len, arr2 + len2, size - len2);
	// Adaug header-ul, dupa care adaug si datele
	memcpy(*arr + len2, data->header, sizeof(head));
	memcpy(*arr + len2 + sizeof(head), data->data, data->header->len);

	// Eliberez memoria alocata
	free(header);
	free(arr2);

	(*len)++;

	return 0;
}

void find(void *data_block, int len, int index) 
{
	// Verific conditiile din enunt
	if (index > len || index < 0) {
		return;
	}

	// Aloc memorie pentru un header, pentru aputea citi din data_block
	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In find nu s-a putut aloca memorie pentru header!\n");
		return;
	}

	int poz = 0;
	int curr_bytes = 0;

	// Parcurg primele index - 1 dedicatii
	while (poz <= index) {
		memcpy(header, data_block + curr_bytes, sizeof(head));
		curr_bytes += sizeof(header) + header->len;

		++poz;
	}

	curr_bytes -= header->len;

	// Afisez dedicatia cu numarul index
	afisare(data_block, header, &curr_bytes);

	// Eliberez memoria alocata pentru header
	free(header);
}

int delete_at(void **arr, int *len, int index)
{
	// Verific conditiile mentionate in enunt
	if (*len == 1) {
		*len = 0;
		free(*arr);
		*arr = NULL;
		return 0;
	}

	// Aloc memorie pentru headerul 
	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In delete_at nu s-a putut aloca header-ul!\n");
		return 1;
	}

	int size = get_nr_of_bytes(*arr, *len);
	int poz = 0;
	int len2 = 0;
	// Parcurg primele elemente pana la elementul index
	while (poz < index) {
		memcpy(header, *arr + len2, sizeof(head));

		len2 += sizeof(head) + header->len;

		++poz;
	}

	int len3 = len2;
	memcpy(header, *arr + len2, sizeof(head));
	len3 += sizeof(head) + header->len;

	// Suprascriu datele elementului pe care trebuie sa il sterg, shiftand bitii catre stanga
	memcpy(*arr + len2, *arr + len3, size - len3);

	// Redimensionez vectorul
	void *aux = realloc(*arr, size - (len3 - len2));
	if (!aux) {
		printf("In delete_at nu s-a putut realoca\n");
		free(header);
		return 1;
	}

	*arr = aux;
	--(*len);

	free(header);

	return 0;
}

void print(void *data_block, int len)
{
	int curr_bytes = 0; // Numarul curent de octeti numarati
	int nr_of_bytes = get_nr_of_bytes(data_block, len); // Numarul total de octeti

	// Aloc headerul utilizat pentru citirea datelor de tip head
	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In print nu s-a putut aloca memorie pentru header!\n");
		return;
	}

	while (curr_bytes < nr_of_bytes) {
		// Copiez in header tipul si lungimea
		memcpy(header, data_block + curr_bytes, sizeof(head));
		// Incrementez numarul curent de octeti numarati cu dimensiunea header-ului
		curr_bytes += sizeof(head);

		// Afisez functia de afisare pentru un element din vector
		afisare(data_block, header, &curr_bytes);
	}

	free(header);
}

int main() {
	void *arr = NULL;
	int len = 0;

	// Sirul de caractere folosit pentru citirea liniilor
	char *readline = malloc(MAX * sizeof(char));
	if (!readline) {
		printf("Nu s-a putut aloca memorie pentru citirea liniilor!\n");
		return -1;
	}

	while (fgets(readline, MAX, stdin)) {
		// Inlocuiesc '\n' cu '\0'
		readline[strcspn(readline, "\n")] = '\0';

		if (strstr(readline, "insert_at")) { // Verific daca se introduce comanda insert_at
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			int insert = atoi(parser);

			parser = strtok(NULL, " ");

			data_structure *data = alocare_structura(parser);
			if (!data) {
				printf("Nu s-a putut aloca memorie pentru structura de date!\n");
				free(readline);
				if (arr) {
					free(arr);
				}
				return -1;
			}

			add_at(&arr, &len, data, insert);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "insert")) { // Verific daca se introduce comanda insert
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			data_structure *data = alocare_structura(parser);
			if (!data) {
				printf("Nu s-a putut aloca memorie pentru structura de date!\n");
				free(readline);
				if (arr) {
					free(arr);
				}
				return -1;
			}

			add_last(&arr, &len, data);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "delete_at")) { // Verific daca se introduce comanda delete_at
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			delete_at(&arr, &len, atoi(parser));
		} else if (strstr(readline, "print")) { // Verific daca se introduce comanda print
			print(arr, len);
		} else if(strstr(readline, "find")) { // Verific daca se introduce comanda find
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			find(arr, len, atoi(parser));
		} else if (strstr(readline, "exit")) { // Verific daca se introduce comanda exit
			break;
		}
	}

	// Eliberez memoria alocata la inceputul programului
	free(readline);
	free(arr);

	return 0;
}

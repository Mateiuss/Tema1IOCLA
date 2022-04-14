#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#define MAX 256

//TODO: rename some variables

data_structure* alocare_structura(char *text)
{
	data_structure *data = malloc(sizeof(data_structure));
	if (!data) {
		return NULL;
	}

	data->header = malloc(sizeof(head));
	if (!data->header) {
		free(data);
		return NULL;
	}

	unsigned char type = data->header->type = atoi(text);

	char *name1 = strtok(NULL, " ");
	text = strtok(NULL, " ");
	int val1 = atoi(text);
	text = strtok(NULL, " ");
	int val2 = atoi(text);
	char *name2 = strtok(NULL, " ");

	data->header->len = strlen(name1) + strlen(name2) + 2;

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

	return data;
}

void afisare(void *data_block, head *header, int *size)
{
	unsigned char type = header->type;

	printf("Tipul %d\n", type);

	char *name1, *name2;

	int i = *size;

	while (*(char*)(data_block + i) != 0) {
		++i;
	}

	name1 = malloc(i - *size + 1);
	if (!name1) {
		free(header);
		return;
	}
	memcpy(name1, data_block + *size, i - *size + 1);

	(*size) = i + 1;

	int8_t val1;
	int8_t val2;
	int16_t val3;
	int32_t val4;
	int32_t val5;

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

	i = *size;

	while (*(char*)(data_block + i) != 0) {
		++i;
	}

	name2 = malloc(i - *size + 1);
	if (!name2) {
		free(name1);
		free(header);
		return;
	}
	memcpy(name2, data_block + *size, i - *size + 1);

	(*size) = i + 1;

	printf("%s pentru %s\n", name1, name2);

	if (type == 1) {
		printf("%d\n%d\n\n", val1, val2);
	} else if (type == 2) {
		printf("%d\n%d\n\n", val3, val4);
	} else if (type == 3) {
		printf("%d\n%d\n\n", val4, val5);
	}

	free(name1);
	free(name2);
}

int get_nr_of_bytes(void *data_block, int len)
{
	int poz = 0;
	int cur_bytes = 0;

	head *header = malloc(sizeof(head));
	if (!header) {
		return -1;
	}

	while (poz < len) {
		memcpy(header, data_block + cur_bytes, sizeof(head));
		cur_bytes += sizeof(head) + header->len;

		++poz;
	}
	free(header);

	return cur_bytes;
}

int add_last(void **arr, int *len, data_structure *data)
{
	int size = get_nr_of_bytes(*arr, *len);

	if (!(*arr)) {
		(*arr) = malloc(data->header->len + sizeof(head));

		if (!(*arr)) {
			printf("Nu s-a putut aloca vectorul!\n");
			return 1;
		}
	} else {
		void *aux;
		aux = realloc(*arr, size + data->header->len + sizeof(head));
		
		if (!aux) {
			printf("Nu s-a putut realoca vectorul!\n");
			return 1;
		}
		*arr = aux;
	}

	memcpy(*arr + size, data->header, sizeof(head));
	memcpy(*arr + size + sizeof(head), data->data, data->header->len);

	++(*len);
	
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	if (index < 0) {
		return 0;
	}
	if (index > *len) {
		return add_last(arr, len, data);
	}

	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In functia add_at nu s-a putut aloca headerul!\n");
		return 1;
	}

	int size = get_nr_of_bytes(*arr, *len);
	int new_size = size + sizeof(head) + data->header->len;
	
	void *aux = realloc(*arr, new_size);
	if (!aux) {
		free(header);
		return 1;
	}
	*arr = aux;

	int poz = 0;
	int len2 = 0;

	while (poz < index) {
		memcpy(header, *arr + len2, sizeof(head));
		len2 += header->len + sizeof(head);
		
		poz++;
	}

	void *arr2 = malloc(new_size);
	memcpy(arr2, *arr, new_size);

	memcpy(*arr + len2 + sizeof(head) + data->header->len, arr2 + len2, size - len2);
	memcpy(*arr + len2, data->header, sizeof(head));
	memcpy(*arr + len2 + sizeof(head), data->data, data->header->len);

	free(header);
	free(arr2);

	(*len)++;

	return 0;
}

void find(void *data_block, int len, int index) 
{
	if (index > len || index < 0) {
		return;
	}

	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In find nu s-a putut aloca memorie pentru header!\n");
		return;
	}

	int poz = 0;
	int curr_bytes = 0;

	while (poz <= index) {
		memcpy(header, data_block + curr_bytes, sizeof(head));
		curr_bytes += sizeof(header) + header->len;

		++poz;
	}

	curr_bytes -= header->len;

	afisare(data_block, header, &curr_bytes);

	free(header);
}

int delete_at(void **arr, int *len, int index)
{
	if (*len == 1) {
		*len = 0;
		free(*arr);
		*arr = NULL;
		return 0;
	}

	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In delete_at nu s-a putut aloca header-ul!\n");
		return 1;
	}

	int size = get_nr_of_bytes(*arr, *len);
	int poz = 0;
	int len2 = 0;
	while (poz < index) {
		memcpy(header, *arr + len2, sizeof(head));

		len2 += sizeof(head) + header->len;

		++poz;
	}

	int len3 = len2;
	memcpy(header, *arr + len2, sizeof(head));
	len3 += sizeof(head) + header->len;

	memcpy(*arr + len2, *arr + len3, size - len3);

	*arr = realloc(*arr, size - (len3 - len2));
	--(*len);

	free(header);

	return 0;
}

void print(void *data_block, int len)
{
	int curr_bytes = 0;
	int nr_of_bytes = get_nr_of_bytes(data_block, len);

	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In print nu s-a putut aloca memorie pentru header!\n");
		return;
	}

	while (curr_bytes < nr_of_bytes) {
		memcpy(header, data_block + curr_bytes, sizeof(head));
		curr_bytes += sizeof(head);

		afisare(data_block, header, &curr_bytes);
	}

	free(header);
}

int main() {
	void *arr = NULL;
	int len = 0;

	char *readline = malloc(MAX * sizeof(char));
	if (!readline) {
		printf("Nu s-a putut aloca memorie pentru citirea liniilor!\n");
		return -1;
	}

	while (fgets(readline, MAX, stdin)) {
		readline[strcspn(readline, "\n")] = '\0';

		if (strstr(readline, "insert_at")) {
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			int insert = atoi(parser);

			parser = strtok(NULL, " ");

			data_structure *data = alocare_structura(parser);
			if (!data) {
				printf("Nu s-a putut aloca memorie pentru structura de date!\n");
				return -1;
			}

			add_at(&arr, &len, data, insert);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "insert")) {
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			data_structure *data = alocare_structura(parser);
			if (!data) {
				printf("Nu s-a putut aloca memorie pentru structura de date!\n");
				return -1;
			}

			add_last(&arr, &len, data);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "delete_at")) {
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			delete_at(&arr, &len, atoi(parser));
		} else if (strstr(readline, "print")) {
			print(arr, len);
		} else if(strstr(readline, "find")) {
			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			find(arr, len, atoi(parser));
		} else if (strstr(readline, "exit")) {
			break;
		}
	}

	free(readline);
	free(arr);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#define MAX 256

int get_size(void *arr, int len)
{
	int poz = 0;
	int size = 0;

	head *header = malloc(sizeof(head));
	if (!header) {
		return -1;
	}

	while (poz < len) {
		memcpy(header, arr + size, sizeof(head));

		int len2 = header->len;

		size += sizeof(head) + len2;
		++poz;
	}

	free(header);

	return size;
}

int add_last(void **arr, int *len, data_structure *data)
{
	int size = get_size(*arr, *len);

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

// int add_at(void **arr, int *len, data_structure *data, int index)
// {
// 	if (index < 0) {

// 	}
// }

// void find(void *data_block, int len, int index) 
// {

// }

// int delete_at(void **arr, int *len, int index)
// {

// }

void print(void *arr, int len)
{
	int poz = 0;
	int size = get_size(arr, len);

	head *header = malloc(sizeof(head));
	if (!header) {
		printf("In print nu s-a putut aloca memorie pentru header!\n");
		return;
	}

	while (poz < size) {
		memcpy(header, arr + poz, sizeof(head));
		poz += sizeof(head);
		unsigned char type = header->type;

		printf("Tipul %d\n", type);

		char *name1, *name2;

		int i = poz;

		while (*(char*)(arr + i) != 0) {
			++i;
		}

		name1 = malloc(i - poz + 1);
		if (!name1) {
			free(header);
			return;
		}
		memcpy(name1, arr + poz, i - poz + 1);

		poz = i + 1;

		int8_t val1;
		int8_t val2;
		int16_t val3;
		int32_t val4;
		int32_t val5;

		if (type == 1) {
			memcpy(&val1, arr + poz, sizeof(int8_t));
			++poz;
			memcpy(&val2, arr + poz, sizeof(int8_t));
			++poz;
		} else if (type == 2) {
			memcpy(&val3, arr + poz, sizeof(int16_t));
			poz += 2;
			memcpy(&val4, arr + poz, sizeof(int32_t));
			poz += 4;
		} else if (type == 3) {
			memcpy(&val4, arr + poz, sizeof(int32_t));
			poz += 4;
			memcpy(&val5, arr + poz, sizeof(int32_t));
			poz += 4;
		}

		i = poz;

		while (*(char*)(arr + i) != 0) {
			++i;
		}

		name2 = malloc(i - poz + 1);
		if (!name2) {
			free(name1);
			free(header);
			return;
		}
		memcpy(name2, arr + poz, i - poz + 1);

		poz = i + 1;

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
		readline[strcspn(readline, "\n")] = 0;

		if (strstr(readline, "insert_at")) {
			data_structure *data = malloc(sizeof(data_structure));
			if (!data) {
				free(readline);
				printf("Nu s-a putut aloca memorie pentru structura!\n");
				return -1;
			}

			data->header = malloc(sizeof(head));
			if (!data->header) {
				free(data);
				free(readline);
				printf("Nu s-a putut aloca memorie pentru header!\n");
				return -1;
			}

			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			int insert = atoi(parser);

			parser = strtok(NULL, " ");

			int type = data->header->type = atoi(parser);

			char *name1 = strtok(NULL, " ");
			parser = strtok(NULL, " ");
			int val1 = atoi(parser);
			parser = strtok(NULL, " ");
			int val2 = atoi(parser);
			char *name2 = strtok(NULL, " ");

			data->header->len = strlen(name1) + strlen(name2) + 2;

			if (type == 1) {
				data->header->len += 2 * sizeof(int8_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int8_t));
				len += sizeof(int8_t);
				memcpy(data->data + len, &val2, sizeof(int8_t));
				len += sizeof(int8_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			} else if (type == 2) {
				data->header->len += sizeof(int16_t) + sizeof(int32_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int16_t));
				len += sizeof(int16_t);
				memcpy(data->data + len, &val2, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			} else if (type == 3) {
				data->header->len += 2 * sizeof(int32_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, &val2, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			}

			// add_at(&arr, &len, data, insert);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "insert")) {
			data_structure *data = malloc(sizeof(data_structure));
			if (!data) {
				free(readline);
				printf("Nu s-a putut aloca memorie pentru structura!\n");
				return -1;
			}

			data->header = malloc(sizeof(head));
			if (!data->header) {
				free(data);
				free(readline);
				printf("Nu s-a putut aloca memorie pentru header!\n");
				return -1;
			}

			char *parser = strtok(readline, " ");
			parser = strtok(NULL, " ");

			int type = data->header->type = atoi(parser);

			char *name1 = strtok(NULL, " ");
			parser = strtok(NULL, " ");
			int val1 = atoi(parser);
			parser = strtok(NULL, " ");
			int val2 = atoi(parser);
			char *name2 = strtok(NULL, " ");

			data->header->len = strlen(name1) + strlen(name2) + 2;

			if (type == 1) {
				data->header->len += 2 * sizeof(int8_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int8_t));
				len += sizeof(int8_t);
				memcpy(data->data + len, &val2, sizeof(int8_t));
				len += sizeof(int8_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			} else if (type == 2) {
				data->header->len += sizeof(int16_t) + sizeof(int32_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int16_t));
				len += sizeof(int16_t);
				memcpy(data->data + len, &val2, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			} else if (type == 3) {
				data->header->len += 2 * sizeof(int32_t);

				data->data = malloc(data->header->len);
				if (!data->data) {
					free(data->header);
					free(data);
					free(readline);
					printf("Nu s-a putut aloca zona de memorie de date!\n");
					return -1;
				}

				int len = 0;

				memcpy(data->data, name1, strlen(name1) + 1);
				len += strlen(name1) + 1;
				memcpy(data->data + len, &val1, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, &val2, sizeof(int32_t));
				len += sizeof(int32_t);
				memcpy(data->data + len, name2, strlen(name2) + 1);

				// for (int i = 0; i < data->header->len; ++i) {
				// 	printf("%d ", *(char*)(data->data + i));
				// }
			}

			add_last(&arr, &len, data);

			free(data->header);
			free(data->data);
			free(data);

		} else if (strstr(readline, "print")) {
			print(arr, len);
		} else if (strstr(readline, "exit")) {
			break;
		}
	}

	free(readline);
	free(arr);

	return 0;
}

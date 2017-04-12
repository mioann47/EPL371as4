#include "configurations.h"

int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start
			&& strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void readConfigurations(CONFIG* cfg) {
	FILE *fp;

	long lSize;
	char *JSON_STRING;

	fp = fopen("config.json", "rb");
	if (!fp) {
		return;
	}
	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	/* allocate memory for entire content */
	JSON_STRING = calloc(1, lSize + 1);
	if (!JSON_STRING) {
		fclose(fp);
		fputs("memory alloc fails", stderr);
		exit(1);
	}

	/* copy the file into the buffer */
	if (1 != fread(JSON_STRING, lSize, 1, fp)) {
		fclose(fp);
		free(JSON_STRING);
		fputs("entire read fails", stderr);
		exit(1);
	}

	fclose(fp);

	char* temp;
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
			sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "port") == 0) {

			temp = (char*) malloc(100);
			if (temp == NULL) {
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			sprintf(temp, "%.*s", t[i + 1].end - t[i + 1].start,
					JSON_STRING + t[i + 1].start);
			cfg->port_number = atoi(temp);
			free(temp);

			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "threads") == 0) {

			temp = (char*) malloc(100);
			if (temp == NULL) {
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			sprintf(temp, "%.*s", t[i + 1].end - t[i + 1].start,
					JSON_STRING + t[i + 1].start);
			cfg->number_of_threads = atoi(temp);
			free(temp);

			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "folder") == 0) {

			temp = (char*) malloc(100);
			if (temp == NULL) {
				perror("malloc");
				exit(EXIT_FAILURE);
			}
			sprintf(temp, "%.*s", t[i + 1].end - t[i + 1].start,
					JSON_STRING + t[i + 1].start);

			cfg->server_file_folder = (char*) malloc(strlen(temp) + 1);
			strcpy(cfg->server_file_folder, temp);
			free(temp);

			i++;

		} else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
					JSON_STRING + t[i].start);
		}
	}
}


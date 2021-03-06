#include "filemodule.h"

char *fileContents(char *path) {
	FILE *fp;
	long lSize;
	char *buffer;

	fp = fopen(path, "rb");

	if (!fp) {
		return NULL;
	}

	lSize = getFileSize(path);

	/* allocate memory for entire content */
	buffer = calloc(1, lSize + 1);
	if (!buffer) {
		fclose(fp);
		perror("calloc buffer");
		exit(EXIT_FAILURE);
	}

	/* copy the file into the buffer */
	if (1 != fread(buffer, lSize, 1, fp)) {
		fclose(fp);
		free(buffer);
		perror("fread");
		exit(EXIT_FAILURE);
	}

	fclose(fp);

	return buffer;
}

char *get_filetype(char *filename) {
	char *temp = malloc(31);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (strstr(filename, ".html") || strstr(filename, ".htm"))
		strcpy(temp, "text/html");
	else if (strstr(filename, ".gif"))
		strcpy(temp, "image/gif");
	else if (strstr(filename, ".jpg") || strstr(filename, ".jpeg"))
		strcpy(temp, "image/jpeg");
	else if (strstr(filename, ".js"))
		strcpy(temp, "text/javascript");
	else if (strstr(filename, ".css"))
		strcpy(temp, "text/css");
	else if (strstr(filename, ".pdf"))
		strcpy(temp, "application/pdf");
	else if (strstr(filename, ".txt") || strstr(filename, ".sed")
			|| strstr(filename, ".awk") || strstr(filename, ".c")
			|| strstr(filename, ".h"))
		strcpy(temp, "text/plain");

	else
		strcpy(temp, "application/octet-stream");

	return temp;
}

int getFileSize(char *path) {
	struct stat fstat;
	lstat(path, &fstat);
	return fstat.st_size;

}

static int checkConnection(char * con) {
	if (strlen(con) == 10) {

		if (((con[0] == 'k' || con[0] == 'K')) && (con[1] == 'e')
				&& (con[2] == 'e') && (con[3] == 'p') && (con[4] == '-')
				&& ((con[5] == 'a') || (con[5] == 'A')) && (con[6] == 'l')
				&& (con[7] == 'i') && (con[8] == 'v') && (con[9] == 'e'))
			return TRUE; //alive
	}

	return FALSE;
}

static int isFolder(char *path) {
	struct stat fstat;
	lstat(path, &fstat);
	return S_ISDIR(fstat.st_mode);

}

int writeIntoSock(int newsock, char *buf, CONFIG *cfg) {
	const char s[2] = " ";
	/** replace **/
	char *p = buf;
	while ((*p) != '\0') {
		if ((*p) == '\r' && (*(p + 1)) == '\n') {
			(*p) = ' ';
			(*(p + 1)) = ' ';
		}
		p++;
	}

	/** TOKENIZER **/
	char *token = NULL;
	token = strtok(buf, s);
	char *request = token;
	char *msgtemp = NULL;
	char *file = NULL;
	char *connection = NULL;

	if (request == NULL) {
		if (connection == NULL) {
			connection = "close";
		}

		msgtemp = msg_bad_request(connection);
		if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
			perror("write");
			return FALSE;
		}

		if (msgtemp != NULL)
			free(msgtemp);

		return checkConnection(connection); //check connection

	}
	int implementedFunction = FALSE;
	while (token != NULL) {
		if (strcmp(token, "GET") == 0 || strcmp(token, "HEAD") == 0
				|| strcmp(token, "DELETE") == 0) {
			token = strtok(NULL, s);
			file = token;
			implementedFunction = TRUE;
			if (file == NULL) {
				if (connection == NULL) {
					connection = "close";
				}
				msgtemp = msg_bad_request(connection);
				if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
					perror("write");
					return FALSE;
				}

				if (msgtemp != NULL)
					free(msgtemp);

				return checkConnection(connection);
			} //check connection

		}

		if (implementedFunction && strcmp(token, "Connection:") == 0) {
			token = strtok(NULL, s);
			connection = token;
		}
		token = strtok(NULL, s);
	}

	if (connection == NULL) {
		connection = "close";
	}

	if (strcmp(request, "GET") == 0 || strcmp(request, "HEAD") == 0) {

		/*build new path*/
		char* path = (char*) malloc(
				strlen(file) + strlen(cfg->server_file_folder) + 1);
		if (path == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strcpy(path, cfg->server_file_folder);
		strcat(path, file);

		if (isFolder(path)) {
			msgtemp = msg_bad_request(connection);
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}

			if (msgtemp != NULL)
				free(msgtemp);

			return checkConnection(connection); //check connection
		}

		/*get contents*/
		char *filebuffer = fileContents(path);
		int fsize = getFileSize(path);

		if (filebuffer == NULL) {
			msgtemp = msg_not_found(connection);

			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}

			if (msgtemp != NULL)
				free(msgtemp);
			return checkConnection(connection); //check connection
		} else {

			/*get filetype*/
			char *filetype = NULL;
			filetype = get_filetype(path);

			/*get header*/
			msgtemp = msg_ok(filebuffer, connection, filetype, fsize);

			/* Send header */
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}

			/* Send contents */
			if (strcmp(request, "GET") == 0) {
				if (write(newsock, filebuffer, fsize + 1) < 0) {
					perror("write");
					return FALSE;
				}
			}

			//free
			if (msgtemp != NULL)
				free(msgtemp);
			if (filetype != NULL)
				free(filetype);
			if (filebuffer != NULL)
				free(filebuffer);
			if (path != NULL)
				free(path);
		}
		return checkConnection(connection); //check connection
	} else if (strcmp(request, "DELETE") == 0) {

		/*build new path*/
		char* path = (char*) malloc(
				strlen(file) + strlen(cfg->server_file_folder) + 1);
		if (path == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strcpy(path, cfg->server_file_folder);
		strcat(path, file);

		if (isFolder(path)) {
			msgtemp = msg_bad_request(connection);
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}

			if (msgtemp != NULL)
				free(msgtemp);

			return checkConnection(connection); //check connection
		}
		FILE *fp;
		fp = fopen(path, "rb");

		if (!fp) {

			msgtemp = msg_not_found(connection);
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}
			if (msgtemp != NULL)
				free(msgtemp);
			return checkConnection(connection); //check connection

		}

		if (remove(path) == 0) {
			msgtemp = msg_deleted_ok(connection);
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}
		} else {
			msgtemp = msg_not_deleted_ok(connection);
			if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
				perror("write");
				return FALSE;
			}
		}
		fclose(fp);
		if (msgtemp != NULL)
			free(msgtemp);
		return checkConnection(connection); //check connection
	} else {

		msgtemp = msg_not_implemented(connection);
		if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
			perror("write");
			return FALSE;
		}
		if (msgtemp != NULL)
			free(msgtemp);
		return checkConnection(connection); //check connection

	}

}


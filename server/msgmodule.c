#include "msgmodule.h"

char *msg_not_implemented(char *con) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 501 Not Implemented\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sConnection: %s\r\n", temp, con);
	sprintf(temp, "%sContent-type: text/plain\r\n", temp);
	sprintf(temp, "%sContent-Length: 24\r\n\r\nMethod not implemented!\n",
			temp);

	return temp;

}

char *msg_not_found(char *con) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 404 Not Found\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sConnection: %s\r\n", temp, con);
	sprintf(temp, "%sContent-type: text/plain\r\n", temp);
	sprintf(temp, "%sContent-Length: 20\r\n\r\nDocument not found!\n", temp);

	return temp;

}

char *msg_ok(char *contents, char *connection, char* filetype, int len) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 200 OK\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sContent-length: %d\r\n", temp, len);
	sprintf(temp, "%sConnection: %s\r\n", temp, connection);
	sprintf(temp, "%sContent-type: %s\r\n\r\n", temp, filetype);

	return temp;
}

char *msg_deleted_ok(char *con) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 200 OK\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sConnection: %s\r\n", temp, con);
	sprintf(temp, "%sContent-type: text/plain\r\n", temp);
	sprintf(temp, "%sContent-Length: 14\r\n\r\nFile deleted!\n", temp);

	return temp;

}

char *msg_not_deleted_ok(char *con) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 405 Method Not Allowed\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sConnection: %s\r\n", temp, con);
	sprintf(temp, "%sContent-type: text/plain\r\n", temp);
	sprintf(temp, "%sContent-Length: 18\r\n\r\nFile not deleted!\n", temp);

	return temp;

}

char *msg_bad_request(char *con) {

	char *temp = malloc(200);
	if (temp == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	sprintf(temp, "HTTP/1.1 400 Bad Request\r\n");
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sConnection: %s\r\n", temp, con);
	sprintf(temp, "%sContent-type: text/plain\r\n", temp);
	sprintf(temp, "%sContent-Length: 13\r\n\r\nBad Request!\n", temp);

	return temp;

}


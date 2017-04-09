#include "msgmodule.h"

char *msg_not_implemented() {
	char *reply =
			"HTTP/1.1 501 Not Implemented\r\nServer: EPL371_SERVER\r\nConnection: close\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nMethod not implemented!\n";
	char *tmp = (char*) malloc(strlen(reply) + 1);
	strcpy(tmp, reply);
	return tmp;
}

char *msg_not_found() {
	char *reply =
			"HTTP/1.1 404 Not Found\r\nServer: EPL371_SERVER\r\nContent-Length: 20\r\nConnection: keep-alive\r\nContent-Type: text/plain\r\n\r\nDocument not found!\n";
	char *tmp = (char*) malloc(strlen(reply) + 1);
	strcpy(tmp, reply);
	return tmp;
}

char *msg_ok(char *contents, char *connection, char* filetype) {
	int len = strlen(contents);

	char *temp = malloc(strlen(contents) + 200);
	sprintf(temp, "HTTP/1.1 200 OK\r\n");    
	sprintf(temp, "%sServer: EPL371_SERVER\r\n", temp);
	sprintf(temp, "%sContent-length: %d\r\n", temp, len);
	sprintf(temp, "%sConnection: %s\r\n", temp, connection);
	sprintf(temp, "%sContent-type: %s\r\n\r\n", temp, filetype);
	sprintf(temp, "%s%s", temp, contents);

	return temp;
}

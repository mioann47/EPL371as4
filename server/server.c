/* File: 
 server.c
 */
#include "libs.h"
#include "filemodule.h"
#include "msgmodule.h"
#include "configurations.h"

//char test[1024];
const char s[2] = " ";

// function prototype for reversing func.
/* Server with Internet stream sockets */

char test[1024];
int main(int argc, char *argv[]) {


	CONFIG *cfg=(CONFIG*)malloc(sizeof(CONFIG));
	readConfigurations(cfg);
	
	char *token;

	sprintf(test, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
	sprintf(test, "%sServer: EPL371_SERVER\r\n", test);
	sprintf(test, "%sContent-length: 211\r\n", test);
	sprintf(test, "%sConnection: keep-alive\r\n", test);
	sprintf(test, "%sContent-type: text/html\r\n\r\n", test);
	sprintf(test, "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1>Click at the image to see a sample text!<br><a href=“sample.txt”><img src=“sample.gif”></a></body></html>",
			test);

	int newsock;
	int port, sock, serverlen;
	unsigned int clientlen;
	char buf[2560];
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;

	/*if (argc < 2) {
		printf("Please give the port number \n");
		exit(1);
	}*/

	/* Create socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	/* Convert port number to integer */
	port = cfg->port_number;
	server.sin_family = PF_INET; /* Internet domain */
	server.sin_addr.s_addr = htonl(INADDR_ANY); /* My Internet address */
	server.sin_port = htons(port); /* The given port */
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof(server);

	/* Bind socket to address */
	if (bind(sock, serverptr, serverlen) < 0) {
		perror("bind");
		exit(1);
	}
	/* Listen for connections */
	if (listen(sock, 5) < 0) {
		/* 5
		 max. requests in queue */
		perror("listen");
		exit(1);
	}
	printf("Listening for connections to port %d\n", port);

	while (1) {
		clientptr = (struct sockaddr *) &client;
		clientlen = sizeof(client);
		/* Accept connection */
		if ((newsock = accept(sock, clientptr, &clientlen)) < 0) {
			perror("accept");
			exit(1);
		}
		/* Using IP address find DNS name (i.e., reverse DNS)*/
		if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
			herror("gethostbyaddr");

			exit(1);
		}
		printf("Accepted connection from %s\n", rem->h_name);

		/* Create child for serving the client */
		switch (fork()) {
		case -1:
			perror("fork");
			exit(1);
		case 0: /* Child process */
			do {
				bzero(buf, sizeof(buf)); /* Initialize buffer */
				if (read(newsock, buf, sizeof(buf)) < 0) { /* Get message */
					perror("read");
					exit(1);
				}

				//printf("%s",buf);

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
				token = strtok(buf, s);
				char *file = NULL;
				char *connection = NULL;
				while (token != NULL) {
					if (strcmp(token, "GET") == 0) {
						token = strtok(NULL, s);
						file = token;
					} else if (strcmp(token, "HEAD") == 0) {
						/*head code */
					} else if (strcmp(token, "DELETE") == 0) {
						/*delete code */
					} else {
						/* NOT IMPLEMENTED MSG CODE*/
					}

					if (strcmp(token, "Connection:") == 0) {
						token = strtok(NULL, s);
						connection = token;
					}
					token = strtok(NULL, s);
				}

				char *msgtemp = NULL;
				char *filetype = NULL;


				char* path= (char*)malloc(strlen(file)+strlen(cfg->server_file_folder)+1);
				strcpy(path,cfg->server_file_folder);
				strcat(path,file);
				char *filebuffer = fileContents(path);

/*
			if (file[0] == '/')
				memmove(file, file + 1, strlen(file));*/

			struct stat fstat;
			lstat(path, &fstat);

				//printf("FILEDATA = %s\n",filebuffer);
				if (filebuffer == NULL) {
					msgtemp = msg_not_found();
					printf("NOT FOUND FILE: %s\n", path);
				} else {
					filetype = get_filetype(path);

					//printf("FILETYPE = %s\n",filetype);
					msgtemp = msg_ok(filebuffer, connection, filetype,fstat.st_size);
					printf("Get file %s\n", file);

				}

				printf("MESSAGE = %s\n",msgtemp);

				//if (filebuffer!=NULL){printf("Get file %s with size %d and message size %d\n",file,strlen(filebuffer),strlen(msgtemp));printf("msg = \n%s",filebuffer);}
				
				if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {/* Send message */
					perror("write");
					exit(1);
				}


				if (filebuffer!=NULL){
				if (write(newsock, filebuffer, fstat.st_size+1) < 0) {/* Send message */
					perror("write");
					exit(1);
				}}


				if (msgtemp != NULL)
					free(msgtemp);
				if (filetype != NULL)
					free(filetype);
				if (filebuffer != NULL)
					free(filebuffer);

			} while (strcmp(buf, "dne") != 0); /*Finish on "end" message*/
			close(newsock); /* Close socket */
			exit(0);
		} /* end of switch */
	} /* end of while(1) */
} /* end of main() */


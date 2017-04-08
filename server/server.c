/* File: 
 server.c
 */
#include "libs.h"
void reverse(char *);





char *fileContents(char *path) {
    FILE *fp;
    long lSize;
    char *buffer;

    if (path[0] == '/')
        memmove(path, path + 1, strlen(path));

    fp = fopen(path, "rb");

    if (!fp) {
        return NULL;
    }
    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

/* allocate memory for entire content */
    buffer = calloc(1, lSize + 1);
    if (!buffer) {
        fclose(fp);
        fputs("memory alloc fails", stderr);
        exit(1);
    }

/* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp)) {
        fclose(fp);
        free(buffer);
        fputs("entire read fails", stderr);
        exit(1);
    }


    //printf("\n\n %s",buffer);

    fclose(fp);
    // free(buffer);

    return buffer;
}






char *msg_not_implemented(){
char *reply ="HTTP/1.1 501 Not Implemented\r\nServer: my_webserver\r\nConnection: close\r\nContent-Type: text/plain\r\nContent-Length: 24\r\n\r\nMethod not implemented!\n";
char *tmp=(char*) malloc (strlen(reply)+1);
strcpy(tmp,reply);
return tmp;
}



char *msg_not_found(){
char *reply = "HTTP/1.1 404 Not Found\r\nServer: My_test_server\r\nContent-Length: 20\r\nConnection: keep-alive\r\nContent-Type: text/plain\r\n\r\nDocument not found!\n";
char *tmp=(char*) malloc (strlen(reply)+1);
strcpy(tmp,reply);
return tmp;
}


//char test[1024];
const char s[2]=" ";

// function prototype for reversing func.
/* Server with Internet stream sockets */




char *msg_ok(char *contents,char *connection,char* filetype){
int len= strlen(contents);

char *temp=malloc(strlen(contents)+200);
	sprintf(temp, "HTTP/1.1 200 OK\r\n");    //line:netp:servestatic:beginserve
   	sprintf(temp, "%sServer: Sysstatd Web Server\r\n", temp);
   	sprintf(temp, "%sContent-length: %d\r\n", temp,len);
	sprintf(temp, "%sConnection: %s\r\n", temp,connection);
	sprintf(temp, "%sContent-type: %s\r\n\r\n", temp,filetype);
	sprintf(temp, "%s%s",temp,contents);

return temp;
}



char *get_filetype(char *filename) 
{
char *temp=malloc(20);
    if (strstr(filename, ".html") )
	strcpy(temp, "text/html");
    else if (strstr(filename, ".gif"))
	strcpy(temp, "image/gif");
    else if (strstr(filename, ".jpg"))
	strcpy(temp, "image/jpeg");
    else if (strstr(filename, ".js"))
	strcpy(temp, "text/javascript");
    else if (strstr(filename, ".css"))
	strcpy(temp, "text/css");
    else
	strcpy(temp, "text/plain");

return temp;
}  









char test[1024];
main(int argc, char *argv[]) {
	

char *token;


    sprintf(test, "HTTP/1.0 200 OK\r\n");    //line:netp:servestatic:beginserve
    sprintf(test, "%sServer: Sysstatd Web Server\r\n", test);
    sprintf(test, "%sContent-length: 211\r\n", test);
    sprintf(test, "%sConnection: keep-alive\r\n", test);
    sprintf(test, "%sContent-type: text/html\r\n\r\n", test);
    sprintf(test,
            "%s<html><head><title>It worked!!!</title></head><body><h1>Yes, It worked!!!</h1>Click at the image to see a sample text!<br><a href=“sample.txt”><img src=“sample.gif”></a></body></html>",
            test);



	


	int port, sock, newsock, serverlen, clientlen;
	char buf[2560];
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;


	if (argc < 2) { /* Check if server's port number is given */
		printf("Please give the port number \n");
		exit(1);
	}

	/* Create socket */
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	/* Convert port number to integer */
	port = atoi(argv[1]);
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
				char *p=buf;
				while ( (*p)!='\0' ){
					if ((*p)=='\r'  && (*(p+1))=='\n') {(*p)=' ';(*(p+1))=' ';}
					p++;
				}					



				/** TOKENIZER **/
				token=strtok(buf,s);
				char *file,*connection;
				while (token!=NULL){
						if (strcmp(token,"GET")==0)  { 	     token=strtok(NULL,s); file=token; }
						if (strcmp(token,"Connection:")==0) {token=strtok(NULL,s); connection=token;}
						token=strtok(NULL,s);
					}
				

				char *msgtemp;
				char *filetype;
				char *filebuffer = fileContents(file);
				//printf("FILEDATA = %s\n",filebuffer);
				if (filebuffer==NULL) {msgtemp=msg_not_found();printf("NOT FOUND FILE: %s\n",file);}
				else {
				filetype=get_filetype(file);

				//printf("FILETYPE = %s\n",filetype);
				msgtemp=msg_ok(filebuffer,connection,filetype);
				printf("Get file %s\n",file);
		
				}

				//printf("MESSAGE = %s\n",msgtemp);
				
				//if (filebuffer!=NULL){printf("Get file %s with size %d and message size %d\n",file,strlen(filebuffer),strlen(msgtemp));printf("msg = \n%s",filebuffer);}

				if (write(newsock, msgtemp, strlen(msgtemp)+1) < 0) {/* Send message */
					perror("write");
					exit(1);
				}
				if (msgtemp!=NULL) free(msgtemp);
				if (filetype!=NULL)free(filetype);
				if (filebuffer!=NULL)free(filebuffer);
				
			} while (strcmp(buf, "dne") != 0); /*Finish on "end" message*/
			close(newsock); /* Close socket */
			exit(0);
		} /* end of switch */
	} /* end of while(1) */
} /* end of main() */





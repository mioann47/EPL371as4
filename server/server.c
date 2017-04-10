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

int getFileSize(char *path){
struct stat fstat;
lstat(path, &fstat);
return fstat.st_size;

}

void writeIntoSock(int newsock,char *buf,CONFIG *cfg){
	
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
				char *token=NULL;
				token = strtok(buf, s);
				char *request=token;
				
				char *file = NULL;
				char *connection = NULL;






				while (token != NULL) {
					if (strcmp(token, "GET") == 0       || strcmp(token, "HEAD") == 0 || strcmp(token, "DELETE") == 0) {
						token = strtok(NULL, s);
						file = token;
					}

					if (strcmp(token, "Connection:") == 0) {
						token = strtok(NULL, s);
						connection = token;
					}
					token = strtok(NULL, s);
				}

				
				
				if (connection==NULL){
				connection="close";
				}

				char *msgtemp = NULL;

				if (strcmp(request,"GET") == 0 || strcmp(request, "HEAD") == 0){

				

				

				/*build new path*/
				char* path= (char*)malloc(strlen(file)+strlen(cfg->server_file_folder)+1);
				strcpy(path,cfg->server_file_folder);
				strcat(path,file);

				/*get contents*/
				char *filebuffer = fileContents(path);
				int fsize=getFileSize(path);


				if (filebuffer == NULL) {
					msgtemp = msg_not_found();
					printf("NOT FOUND FILE: %s\n", file);
				if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
					perror("write");
					exit(1);
				}

				if (msgtemp != NULL)
					free(msgtemp);
				} else {
						
					/*get filetype*/
					char *filetype = NULL;
					filetype = get_filetype(path);

					/*get header*/
					msgtemp = msg_ok(filebuffer, connection, filetype,fsize);
					printf("%s file %s\n",request,file);

				

				
				/* Send header */	
				if (write(newsock, msgtemp, strlen(msgtemp)) < 0) {
					perror("write");
					exit(1);
				}

				/* Send contents */
				if (strcmp(request,"GET") == 0){
					if (write(newsock, filebuffer, fsize+1) < 0) {
						perror("write");
						exit(1);
					}
				}

				
				//free
				if (msgtemp != NULL)
					free(msgtemp);
				if (filetype != NULL)
					free(filetype);
				if (filebuffer != NULL)
					free(filebuffer);
				if (path!=NULL)
					free(path);
				}
			}else if (strcmp(request,"DELETE") == 0)  {

                    FILE *fp;
                    fp = fopen(file, "rb");

                    if (!fp) {
                        printf("HTTP/1.1 Not Found\n");
                        //exit(0);
                    }

                    if (remove(file) == 0)
                        printf("HTTP/1.1 200 OK\n");
                    else
                        printf("File deletion failed\n");


                } else {
                    printf("%s", msg_not_implemented());
			/*not implemented code*/			
			}

}


void *connection_handler(void *);



typedef struct{
CONFIG *cfg;
int sock;
}CONNECTION_INFO;

int main(int argc, char *argv[]) {


	CONFIG *cfg=(CONFIG*)malloc(sizeof(CONFIG));
	readConfigurations(cfg);
	
	int err;
	// error code 
	pthread_t tid[40];
	// Thread ID

	

	int newsock;
	int port, sock, serverlen;
	unsigned int clientlen;
	
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;

	

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
	int i=0;
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
		printf("Accepted connection from %s %d\n", rem->h_name,newsock);


	CONNECTION_INFO *cinfo=(CONNECTION_INFO*) malloc(sizeof(CONNECTION_INFO));
	cinfo->cfg=cfg;
	cinfo->sock=newsock;
			fprintf(stderr,"SOCK1 = %d\n",cinfo->sock);
	




		if (err = pthread_create( &tid[i], NULL, &connection_handler, (void*) cinfo)) {

			  exit(1);
			}
			
		i++;		
		
	//////////
			
	} /* end of while(1) */




} /* end of main() */



void *connection_handler(void *cinfo1) {
	// Get the socket descriptor
CONNECTION_INFO *cinfo=  (CONNECTION_INFO*) cinfo1;
CONFIG *cfg= cinfo->cfg;
int newsock=cinfo->sock;


printf("serving connection = %d\n",newsock);




char buf[2560];
				bzero(buf, sizeof(buf)); 
				if (read(newsock, buf, sizeof(buf)) < 0) { 
					perror("read");
					exit(1);
				}
				if (strlen(buf)==0) { free(cinfo);
							close(newsock);
							return;}
				writeIntoSock(newsock,buf,cfg);

free(cinfo);
close(newsock);	
}


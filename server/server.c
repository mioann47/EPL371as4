/* File: 
 server.c
 */
#include "libs.h"
#include "filemodule.h"
#include "msgmodule.h"
#include "configurations.h"
#include "queue.h"

void *connection_handler(void *);
void signal_handler(int sig);
Queue *qSock;

pthread_mutex_t mutex;
pthread_cond_t cond;
int err;

int main(int argc, char *argv[]) {
	/* Install signal handler */
	

	ConstructQueue(&qSock);
	NODE *pN;

	CONFIG *cfg = (CONFIG *) malloc(sizeof(CONFIG));
	readConfigurations(cfg);
	signal(SIGINT, signal_handler); 
	
	// Thread ID
	pthread_t tid[cfg->number_of_threads];
	

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
	int i = 0;
	pthread_mutex_init(&mutex, NULL);
	for (i = 0; i < cfg->number_of_threads; i++) {

		if ((err = pthread_create(&tid[i], NULL, &connection_handler,
				(void *) cfg)) == 1) {

			exit(1);
		}

	}

	while (1) {
		clientptr = (struct sockaddr *) &client;
		clientlen = sizeof(client);
		/* Accept connection */
		if ((newsock = accept(sock, clientptr, &clientlen)) < 0) {
			perror("accept");
			exit(1);
		}

		pN = (NODE *) malloc(sizeof(NODE));
		pN->data = newsock;
		Enqueue(qSock, pN);

		/* send signal to condition  */
		if ((err = pthread_cond_signal(&cond)) == TRUE) {
			printf("pthread_cond_signal: %s\n", strerror(err));
			exit(1);
		}

		/* Using IP address find DNS name (i.e., reverse DNS)*/
		if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
			herror("gethostbyaddr");

			exit(1);
		}
		printf("Accepted connection from %s %d added to queue\n", rem->h_name,
				newsock);


	} /* end of while(1) */

	

} /* end of main() */

void *connection_handler(void *cinfo1) {
	int x = TRUE;
	int newsock;
	while (TRUE) {
		if ((err = pthread_mutex_lock(&mutex)) == TRUE) { /* lock mutex */
			printf("pthread_mutex_lock: %s\n", strerror(err));
			exit(1);
		}

		while (isEmpty(qSock)) {

			// wait server to put a node in the queue
			if ((err = pthread_cond_wait(&cond, &mutex)) == TRUE) {
				printf("pthread_cond_wait: %s\n", strerror(err));
				exit(1);
			}

		}

		newsock = Dequeue(qSock);

		if ((err = pthread_mutex_unlock(&mutex)) == TRUE) { /* unlock mutex */
			printf("pthread_mutex_unlock: %s\n", strerror(err));
			exit(1);
		}

		CONFIG *cfg = (CONFIG *) cinfo1;

		printf("serving connection = %d\n", newsock);

		do {
			char buf[2560];
			bzero(buf, sizeof(buf));
			if (read(newsock, buf, sizeof(buf)) < 0) {
				perror("read");
				x = FALSE;
				break;
			}
			if (strlen(buf) == 0) {
				close(newsock);
				x = FALSE;
				break;
			}
			x = writeIntoSock(newsock, buf, cfg);

		} while (x == TRUE);
		

		close(newsock);

	}
}


void signal_handler(int sig) {
printf("Server Terminated\n");
exit(EXIT_SUCCESS);
}


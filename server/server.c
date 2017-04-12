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
	signal(SIGINT, signal_handler);

	ConstructQueue(&qSock);
	NODE *pN;

	CONFIG *cfg = (CONFIG *) malloc(sizeof(CONFIG));
	if (cfg == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	readConfigurations(cfg);

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

	int reuse = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(int))
			< 0) {
		perror("setsockopt");
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
			perror("pthread_create");
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
		if (pN == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		pN->data = newsock;
		Enqueue(qSock, pN);

		/* send signal to condition  */
		if ((err = pthread_cond_signal(&cond)) == TRUE) {
			perror("pthread_cond_signal");
			exit(1);
		}

		/* Using IP address find DNS name (i.e., reverse DNS)*/
		if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
			herror("gethostbyaddr");

			exit(1);
		}
		printf("Accepted connection from %s\n", rem->h_name);

	} /* end of while(1) */

} /* end of main() */

void *connection_handler(void *cinfo1) {
	int x = TRUE;
	int newsock;
	char buf[2560];
	while (TRUE) {
		if ((err = pthread_mutex_lock(&mutex)) == TRUE) { /* lock mutex */
			perror("pthread_mutex_lock");
			exit(1);
		}

		while (isEmpty(qSock)) {

			// wait server to put a node in the queue
			if ((err = pthread_cond_wait(&cond, &mutex)) == TRUE) {
				perror("pthread_cond_wait");
				exit(1);
			}

		}

		newsock = Dequeue(qSock);

		if ((err = pthread_mutex_unlock(&mutex)) == TRUE) { /* unlock mutex */
			perror("pthread_mutex_unlock");
			exit(1);
		}

		CONFIG *cfg = (CONFIG *) cinfo1;

		//printf("serving connection = %d\n", newsock);

		do {
			//clear buf
			memset( buf, 0x00, sizeof(buf) );

			
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


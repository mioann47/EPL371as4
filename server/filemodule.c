#include "filemodule.h"

char *fileContents(char *path) {
    FILE *fp;
    long lSize;
    char *buffer;


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

char *get_filetype(char *filename) {
    char *temp = malloc(31);
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
    else if (strstr(filename, ".txt") || strstr(filename, ".sed") || strstr(filename, ".awk") ||
             strstr(filename, ".c") || strstr(filename, ".h"))
        strcpy(temp, "text/plain");

    else
        strcpy(temp, "application/octet-stream");

    return temp;
}


int getFileSize(char *path){
struct stat fstat;
lstat(path, &fstat);
return fstat.st_size;

}




void writeIntoSock(int newsock,char *buf,CONFIG *cfg){
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


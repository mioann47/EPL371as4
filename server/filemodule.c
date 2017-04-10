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


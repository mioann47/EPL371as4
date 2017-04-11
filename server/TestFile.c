

//
// Created by andreas on 11/04/2017.
//
#include "libs.h"
#include "msgmodule.h"
#include "filemodule.h"

#ifdef DEBUG

int main() {
    char *connection = "keep-alive";
    char *contents = "<dwsync><file name=\"style.css\" server=\"www//WebData/courses/EPL435/\" local=\"129685859484112482\" remote=\"129685860440000000\"/><file name=\"style.css\" server=\"www//WebData/courses/EPL034/\" local=\"129767981486051878\" remote=\"129768094380000000\"/><file name=\"style.css\" server=\"www2.cs.ucy.ac.cy//sys-data/WebData/courses/EPL035/\" local=\"129907184860000000\" remote=\"129908819840000000\"/></dwsync>";
    char *filetype;
    char *filename = "config.json";
    int len = 10;

    printf("\n\n\n############################################################################\n");
    printf("Testing Function msg_not_implemented\n");
    printf("############################################################################\n");
    printf("%s\n", msg_not_implemented(connection));
    printf("############################################################################\n\n\n");


    printf("############################################################################\n");
    printf("Testing Function msg_not_found\n");
    printf("############################################################################\n");
    printf("%s\n", msg_not_found(connection));
    printf("############################################################################\n\n\n");

    printf("############################################################################\n");
    printf("Testing Function get_filetype\n");
    printf("############################################################################\n");
    filetype = get_filetype(filename);
    printf("%s\n", filetype);
    printf("############################################################################\n\n\n");


    printf("############################################################################\n");
    printf("Testing Function msg_ok\n");
    printf("############################################################################\n");
    printf("%s\n", msg_ok(contents, connection, filetype, len));
    printf("############################################################################\n\n\n");

    printf("############################################################################\n");
    printf("Testing Function msg_deleted_ok\n");
    printf("############################################################################\n");
    printf("%s\n", msg_deleted_ok(connection));
    printf("############################################################################\n\n\n");


    printf("############################################################################\n");
    printf("Testing Function msg_not_deleted_ok\n");
    printf("############################################################################\n");
    printf("%s\n", msg_not_deleted_ok(connection));
    printf("############################################################################\n\n\n");


    printf("############################################################################\n");
    printf("Testing Function msg_bad_request\n");
    printf("############################################################################\n");
    printf("%s\n", msg_bad_request(connection));
    printf("############################################################################\n\n\n");

    printf("############################################################################\n");
    printf("Testing Function fileContents\n");
    printf("############################################################################\n");
    printf("%s\n", fileContents(filename));
    printf("############################################################################\n\n\n");

    printf("############################################################################\n");
    printf("Testing Function getFileSize\n");
    printf("############################################################################\n");
    printf("%d\n", getFileSize(filename));
    printf("############################################################################\n\n\n");


}

#endif
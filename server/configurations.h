#include "libs.h"
#include "jsmn.h"



typedef struct{
int port_number;
int number_of_threads;
char *server_file_folder;
}CONFIG;




int jsoneq(const char *json, jsmntok_t *tok, const char *s);



void readConfigurations(CONFIG* cfg);

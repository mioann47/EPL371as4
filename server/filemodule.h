#ifndef __FMOD_H__
#define __FMOD_H__

#include "libs.h"
#include "configurations.h"
#include "msgmodule.h"
/**
 Return file contents
 **/
char *fileContents(char *path);

/**
 return file type NOT FINISHED
 **/
char *get_filetype(char *filename);

int getFileSize(char *path);
int writeIntoSock(int newsock, char *buf, CONFIG *cfg);
#endif

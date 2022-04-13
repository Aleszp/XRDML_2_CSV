#ifndef _SETUP_H_
#define _SETUP_H_

int handleStartup(int argc,char** argv,int* optind_,char* separator);
int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut);
char selectSeparator(int argc,char** argv,int optind);

#endif

#ifndef _MESSAGES_H_
#define _MESSAGES_H_

void GPLnotice();
void printHelp();
void wrongUsage();
void wrongFile(char* msg,char* path);

int handleStartup(int argc,char** argv,int* optind_);

#endif

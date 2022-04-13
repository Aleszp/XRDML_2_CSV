#ifndef _SETUP_H_
#define _SETUP_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

int handleStartup(int argc,char** argv,int* optind_,char* separator);
int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut);
char selectSeparator(int argc,char** argv,int optind);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//_SETUP_H_

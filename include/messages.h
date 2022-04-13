#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#ifdef __cplusplus
extern "C" //make C++ compatible (just in case)
{
#endif

void GPLnotice();
void printHelp();
void wrongUsage();
void wrongFile(char* mode,char* path);

#ifdef __cplusplus
} //end of extern "C"
#endif

#endif	//end of _MESSAGES_H_

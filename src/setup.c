#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "messages.h"
#include "setup.h"


int handleStartup(int argc,char** argv,int* optind_,char* separator)
{
	uint_fast8_t notice=1,help=0;
	char cc;
	//Detect cli switches and set proper flags
	while((cc=getopt(argc,argv,":shctSn"))!=-1)
	{	
		switch(cc)
		{
			//should help be printed?
			case '?':
			case 'h': 
				help=1;
				break;
			//should GNU Notice be supressed?
			case 'n':
				notice=0;
				break;
				
			//force comma ',' as separator
			case 'c':
				*separator=',';
				break;
			//force tab '\t' as separator
			case 't':
				*separator='\t';
				break;
			//force space ' ' as separator
			case 's':
				*separator=' ';
				break;
			//force semicolon ';' as separator
			case 'S':
				*separator=';';
				break;
		}
	}
    *optind_=optind;
	
	//Print notice about GPL
	if(notice)
	{
		GPLnotice();
	}
	if(help)
	{
		printHelp();
		return -1;
	}
	
	//Detect if proper number of arguments are present
	if(argc-optind!=2)
	{
		wrongUsage();
		return 1;
	}
	
	return 0;
}

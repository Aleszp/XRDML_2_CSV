#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "messages.h"
#include "setup.h"
#include "error.h"

/**
 * Detect CLI arguments and (optionally) set separator based on them.
 * @param argc - counter of CLI arguments (just pass argc from main)
 * @param argv - table with CLI arguments (just pass argv from main)
 * @param optiond_ - pointer to external int variable that would pass optind to other functions
 * @param separator - pointer to external char variable that would pass separator (i.e. comma ',')
 * @return int with error code, ==-1 user asked for help, ==1 - error, ==0 - proper execution 
 */ 
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
		return HELP;
	}
	//If no separator was forced
	if(*separator=='\0')
	{
		//try to autodetect separator based on output file name
		*separator=selectSeparator(argc,argv,optind);
	}
	
	//Detect if proper number of arguments are present
	if(argc-optind!=2)
	{
		wrongUsage();
		return ARGUMENTS;
	}
	
	return OK;
}

/**
 * Get separator from file extention.
 * @param argc - counter of CLI arguments (just pass argc from main)
 * @param argv - table with CLI arguments (just pass argv from main) 
 * @param optind - int passed from handleStartup
 * @param fileIn - pointer to FILE pointer of input file (to be passed to other functions)
 * @param fileOut - pointer to FILE pointer of output file (to be passed to other functions)
 * @return int with error code, ==0 proper execution, ==2 - failed to open file for reading, ==3 - failed to open file for writting
 */ 
int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut)
{
	//Open intput file for reading
	*fileIn=fopen(argv[argc-optind],"r");
	if(!(*fileIn))
	{
		wrongFile("reading",argv[argc-optind]);
		return INPUT;
	}
	//Open intput file for writting
	*fileOut=fopen(argv[argc-optind+1],"w");
	if(!(*fileOut))
	{
		wrongFile("writting",argv[argc-optind+1]);
		return OUTPUT;
	}
	return OK;
}

/**
 * Get separator from file extention.
 * @param argc - counter of CLI arguments (just pass argc from main)
 * @param argv - table with CLI arguments (just pass argv from main) 
 * @param optind - int passed from handleStartup
 * @return char with separator to be used
 */ 
char selectSeparator(int argc,char** argv,int optind)
{
	//char *txt,*ssv,*xy,*tsv,*csv;
	
	//brak funkcji strrstr, użyj strchr by znaleźć ostatnią kropkę i po niej określ rozszerzenie
	/*txt=strrstr(argv[argc-optind],".txt");
	ssv=strrstr(argv[argc-optind],".ssv");
	xy=strrstr(argv[argc-optind],".xy");
	tsv=strrstr(argv[argc-optind],".tsv");
	csv=strrstr(argv[argc-optind],".csv");
	*/
	//char* last=txt;
	
	//find which extention is last (i.e data.csv.txt is txt, not csv), warning data.csv123 is treated as .csv as 123 is not defined and igonred
	/*if(last>ssv)
		last=ssv;
	if(last>xy)
		last=xy;
	if(last>tsv)
		last=tsv;	
	if(last>csv)
		last=csv;	
	
	if(last==txt)	//if txt return space ' '
	{	
		return ' ';
	}
	if(last==xy)	//if ssv return semicolon ';'
	{
		return ';';
	}
	if(last==tsv)	//if TSV or XY return tab '\t'
	{
		return '\t';
	}*/
	//comma separator (csv extention) is default, fallback
	return ',';
}

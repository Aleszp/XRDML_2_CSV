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


int openFiles(int argc,char** argv,int optind,FILE** fileIn, FILE** fileOut)
{
	//Open intput file for reading
	*fileIn=fopen(argv[argc-optind],"r");
	if(!(*fileIn))
	{
		wrongFile("reading",argv[argc-optind]);
		return 2;
	}
	//Open intput file for writting
	*fileOut=fopen(argv[argc-optind+1],"w");
	if(!(*fileOut))
	{
		wrongFile("writting",argv[argc-optind+1]);
		return 3;
	}
	return 0;
}

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

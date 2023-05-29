/*
 * Part of program XRDML_2_CSV
 * Setup functions implementation file.
 * 
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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
	if(*optind_==1)	//If no switches were used:
	{
		*optind_=2;
	}
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
 * Open file based on arguments
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
		//Close input file, as it is already opened
		fclose(*fileIn);
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
	//Find last occurance od dot - this way file extention can be detected.
	char* extention=strrchr(argv[argc-optind+1],'.');

	//If no dot could be found - no extention - fallback to comma separator
	if(!extention)
	{
		return ',';
	}
	if(strcmp(extention,".txt")==0)	//if txt return space ' '
	{	
		return ' ';
	}
	if(strcmp(extention,".ssv")==0)	//if ssv return semicolon ';'
	{
		return ';';
	}
	if(strcmp(extention,".tsv")==0||strcmp(extention,".xy")==0)	//if TSV or XY return tab '\t'
	{
		return '\t';
	}
	//comma separator (csv extention) is default, fallback
	return ',';
}

/**
 * Close opened files and return error code.
 * @param fileIn - pointer to input file pointer
 * @param fileOut - pointer to output file pointer
 * @param errorCode - code to be returned
 * @return errorCode (passed from parameter)
 **/
int exitProgram(FILE** fileIn, FILE** fileOut,int errorCode)
{
	fclose(*fileIn);
	fclose(*fileOut);
	return errorCode;
}

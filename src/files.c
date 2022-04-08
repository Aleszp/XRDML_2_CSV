#include <stdio.h>
#include <stdint.h>
#include "files.h"
#include "messages.h"

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

uint64_t countAngles(FILE* fileIn)
{
	uint64_t counter=0;
	char character;
	do
	{
		character=fgetc(fileIn);
		if(character==' ')
		{
			counter++;
		}
		if(feof(fileIn))
		{
			break;
		}
	}
	while(character!='<');
	return counter;
}

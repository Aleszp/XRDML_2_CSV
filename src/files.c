#include <stdio.h>
#include <stdint.h>
#include <string.h>
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

void getStartStop(FILE* fileIn,long double* start,long double* stop)
{
	//prepare input buffer
	char buffer[255];
	char* ptr;
	while(!feof(fileIn))
	{
		//skip unused header data (before angle info)
		fgets(buffer,255,fileIn);
		ptr=strstr(buffer,"<startPosition>");
		if(ptr!=NULL)
		{
			break;
		}
	}
	//
	sscanf(ptr+15,"%Lf",start);
	fgets(buffer,255,fileIn);
	ptr=strstr(buffer,"<endPosition>");
	sscanf(ptr+13,"%Lf",stop);
}

void skipHeader(FILE* fileIn)
{
	long offset;
	//prepare input buffer
	char buffer[255];
	char* ptr;
	
	while(!feof(fileIn))
	{
		//skip rest of header to find start of data
		//save offset
		offset = ftell(fileIn);
		fgets(buffer,255,fileIn);
		ptr=strstr(buffer,"<intensities ");
		if(ptr!=NULL)
		{
			fseek(fileIn, offset, SEEK_SET);
			fgets(buffer,32,fileIn);
			break;
		}
	}
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

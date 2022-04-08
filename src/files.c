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
		//for each separating space
		if(character==' ')
		{
			//increase counter
			counter++;
		}
		//if file ended
		if(feof(fileIn))
		{
			//stop
			break;
		}
	}
	while(character!='<');	//is it end of data?
	return counter;
}

long double getDtheta(FILE* fileIn, long double* start,long double* stop)
{
	//save offset for further rewind
	long offset=ftell(fileIn);
	
	//Count number of individual angles to calculate single step difference (in .xrdml we get accurate initial and final angle data, single step info is too roughly rounded)
	uint64_t count=countAngles(fileIn);	
	
	//after this - rewind file to start of data
	fseek(fileIn, offset, SEEK_SET);
	
	return (*stop-*start)/((long double) count);
}

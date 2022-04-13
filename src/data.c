#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "data.h"
#include "messages.h"

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

void convertData(FILE* fileIn,FILE* fileOut,char separator,long double start,long double Dtheta)
{
	//Print header in output file. Use CR+LF for widest OS support
	fprintf(fileOut,"2theta%cintensity%c\ndegree%ccounts%c\r\n",separator,separator,separator,separator);
	char character;
	//While looping: count lines, if EOF detected - stop
	for(uint64_t ii=0;!feof(fileIn);ii++)
	{
		//Print single line:
		//Print angle with 0.8Lf precision 
		fprintf(fileOut,"%0.8Lf%c",start+((long double)ii)*Dtheta,separator);
		do
		{
			//Just copy values from input (to avoid unnecessary conversion losses)
			character=fgetc(fileIn);
			//Space ' ' is separator between measured data - start new datapoint upon detection
			//'<' is start of "</intensities>: it's end of data - break the loop
			if(character=='<'||character==' ')
			{
				break;
			}
			fprintf(fileOut,"%c",character);
			//Space ' ' is separator between measured data - start new datapoint upon detection
		}
		while(character!=' ');
		
		//If '<' - start of "</intensities> is found - it's end of data - break the loop
		if(character=='<')
		{
			break;
		}
		//Add end of line (CR+LF for wide OS support)
		fprintf(fileOut,"%c\r\n",separator);
	}
}

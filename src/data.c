/*
 * Part of program XRDML_2_CSV
 * Data oriented functions implementation file.
 * 
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "data.h"
#include "messages.h"
#include "error.h"

/**
 * Get start and stop angles in measurement from input file
 * @param fileIn - pointer to input file
 * @param start - pointer to longdouble to which first angle value will be copied
 * @param stop -  pointer to longdouble to which last angle value will be copied
 */
int getStartStop(FILE* fileIn,long double* start,long double* stop)
{
	//prepare input buffer
	char buffer[255];
	char* ptr=NULL;
	while(ptr==NULL)
	{
		if(feof(fileIn))
		{
			return EMPTY;
		}
		//skip unused header data (before angle info)
		fgets(buffer,255,fileIn);
		ptr=strstr(buffer,"<startPosition>");
	}

	sscanf(ptr+15,"%Lf",start);
	fgets(buffer,255,fileIn);
	ptr=strstr(buffer,"<endPosition>");
	sscanf(ptr+13,"%Lf",stop);
	return 0;
}

/**
 * Read input file to skip unneeded part of header.
 * @param fileIn - pointer to input file
 * @return error code, ==0 - OK, !=0 - error
 */ 
int skipHeader(FILE* fileIn)
{
	long offset=ftell(fileIn);
	//prepare input buffer
	char buffer[255];
	char* ptr=NULL;
	
	while(ptr==NULL)
	{
		//skip rest of header to find start of data
		//save offset
		offset=ftell(fileIn);
		fgets(buffer,255,fileIn);
		ptr=strstr(buffer,"<intensities ");
		
		if(feof(fileIn)) //if EOF prematurely
		{
			fprintf(stderr,"No intensities in input file or broken input file.\n");
			return NOINTENSITIES;
		}
	}
	fseek(fileIn, offset, SEEK_SET);
	fgets(buffer,32,fileIn);
	return OK;
}

/**
 * Count how many individual angles there are.
 * @param fileIn - pointer to input file
 * @return number of angles (unsigned 64bit integer), returns zero if no data is detected or EOF file reached within data
 */
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
			//stop and return info about lack of intensities data
			return 0;
		}
	}
	while(character!='<');	//is it end of data?
	return counter;
}

/**
 * Calculate difference between angles.
 * @param fileIn - pointer to input file
 * @param start - pointer to long double with start angle value
 * @param stop - pointer to long double with stop angle value
 * @return long double with difference (should be >0), negative values mean error
 */ 
long double getDtheta(FILE* fileIn, long double* start,long double* stop)
{
	//save offset for further rewind
	long offset=ftell(fileIn);
	
	//Count number of individual angles to calculate single step difference (in .xrdml we get accurate initial and final angle data, single step info is too roughly rounded)
	uint64_t count=countAngles(fileIn);	
	if(count==0)
	{
		fprintf(stderr,"No intensities in input file or broken input file.\n");
		return -1.0;	//return number <0
	}
	
	//after this - rewind file to start of data
	fseek(fileIn, offset, SEEK_SET);
	
	return (*stop-*start)/((long double) count);
}

/**
 * Convert data from input file into output file.
 * @param fileIn - pointer to input file
 * @param fileOut - pointer to output file
 * @param separator - character that would be used as separator in output file
 * @param 
 */
void convertData(FILE* fileIn,FILE* fileOut,char separator,long double* start,long double* Dtheta)
{
	//Print header in output file. Use CR+LF for widest OS support
	fprintf(fileOut,"2theta%cintensity%c\ndegree%ccounts%c\r\n",separator,separator,separator,separator);
	char character;
	//While looping: count lines, if EOF detected - stop
	for(uint64_t ii=0;!feof(fileIn);ii++)
	{
		//Print single line:
		//Print angle with 0.8Lf precision 
		fprintf(fileOut,"%0.8Lf%c",(*start)+((long double)ii)*(*Dtheta),separator);
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

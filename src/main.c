/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version alpha-1.5
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "messages.h"

int main(int argc, char** argv)
{
	uint_fast8_t notice=1,help=0;
	//Print notice about GPL
	if(notice)
	{
		GPLnotice();
	}
	if(help)
	{
		printHelp();
	}
	//If there is wrong number of arguments: inform user and terminate.
	if(argc!=3)
	{
		//fprintf(stderr,"Incorrect number of arguments. %d used, should be %d.\n. Correct use: xrdml2xy [input_path/file.xrdml] [output_path/file.csv].\n", argc-1, 2);
		fprintf(stdout,"Incorrect usage, please type \"xrdml2xy -s\" for help.\n");
		return 1;
	}
	//Open intput file for reading
	FILE* fileIn=fopen(argv[1],"r");
	if(!fileIn)
	{
		//Open intput file for reading
		fprintf(stderr,"Could not open file for reading: %s\n",argv[1]);
		return 2;
	}
	//Open intput file for writting
	FILE* fileOut=fopen(argv[2],"w");
	if(!fileOut)
	{
		fprintf(stderr,"Could not open file for writting: %s\n",argv[2]);
		return 3;
	}
	//prepare input buffer
	char buffer[255];
	char* ptr;
	char separator=',';
	
	long double start,stop;
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
	sscanf(ptr+15,"%Lf",&start);
	fgets(buffer,255,fileIn);
	ptr=strstr(buffer,"<endPosition>");
	sscanf(ptr+13,"%Lf",&stop);
	
	long offset;
	
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
	char character;
	//save offset for further rewind
	offset = ftell(fileIn);
	uint64_t count=0;
	//Count number of individual angles to calculate single step difference (in .xrdml we get accurate initial and final angle data, single step info is too roughly rounded)
	do
	{
		character=fgetc(fileIn);
		if(character==' ')
		{
			count++;
		}
		if(feof(fileIn))
		{
			break;
		}
	}
	while(character!='<');	
	
	//after this - rewind file to start of data
	fseek(fileIn, offset, SEEK_SET);
	//Print header in output file. Use CR+LF for widest OS support
	fprintf(fileOut,"2theta%cintensity%c\ndegree%ccounts%c\r\n",separator,separator,separator,separator);
	long double Dtheta=(stop-start)/((long double) count);
	
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
	
	//Close input and output files
	fclose(fileIn);
	fclose(fileOut);
	return 0;
}

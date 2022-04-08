/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version beta-1.1
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "messages.h"
#include "files.h"

int main(int argc,char** argv)
{
	int optind=0;
	int err;
	char separator=',';
	
	//Handle CLI switches, print help, GNU Notice...
	err=handleStartup(argc,argv,&optind,&separator);
	if(err!=0)
		return err;
	
	FILE* fileIn;
	FILE* fileOut;
	
	//Open input and output files
	err=openFiles(argc,argv,optind,&fileIn,&fileOut);
	if(err!=0)
		return err;
	
	//prepare input buffer
	char buffer[255];
	char* ptr;
	
	
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
	//Print header in output file. Use CR+LF for widest OS support
	fprintf(fileOut,"2theta%cintensity%c\ndegree%ccounts%c\r\n",separator,separator,separator,separator);
	
	char character;
	//save offset for further rewind
	offset = ftell(fileIn);
	uint64_t count=0;
	//Count number of individual angles to calculate single step difference (in .xrdml we get accurate initial and final angle data, single step info is too roughly rounded)
	count=countAngles(fileIn);	
	
	//after this - rewind file to start of data
	fseek(fileIn, offset, SEEK_SET);
	
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

/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version alpha-1.2b
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void GPLnotice();

int main(int argc, char** argv)
{
	//Print notice about GPL
	GPLnotice();
	//If there is wrong number of arguments: inform user and terminate.
	if(argc!=3)
	{
		fprintf(stderr,"Incorrect number of arguments. %d used, should be %d.\n. Correct use: xrdml2xy [input_path/file.xrdml] [output_path/file.csv].\n", argc-1, 2);
		return 1;
	}
	//Open intput file for reading
	FILE* plikWe=fopen(argv[1],"r");
	if(!plikWe)
	{
		//Open intput file for reading
		fprintf(stderr,"Could not open file for reading: %s\n",argv[1]);
		return 2;
	}
	//Open intput file for writting
	FILE* plikWy=fopen(argv[2],"w");
	if(!plikWy)
	{
		fprintf(stderr,"Could not open file for writting: %s\n",argv[2]);
		return 3;
	}
	//prepare input buffer
	char bufor[255];
	char* wsk;
	
	long double start,stop;
	while(!feof(plikWe))
	{
		//skip unused header data (before angle info)
		fgets(bufor,255,plikWe);
		wsk=strstr(bufor,"<startPosition>");
		if(wsk!=NULL)
		{
			break;
		}
	}
	//
	sscanf(wsk+15,"%Lf",&start);
	fgets(bufor,255,plikWe);
	wsk=strstr(bufor,"<endPosition>");
	sscanf(wsk+13,"%Lf",&stop);
	
	long offset;
	
	while(!feof(plikWe))
	{
		//skip rest of header to find start of data
		//save offset
		offset = ftell(plikWe);
		fgets(bufor,255,plikWe);
		wsk=strstr(bufor,"<intensities ");
		if(wsk!=NULL)
		{
			fseek(plikWe, offset, SEEK_SET);
			fgets(bufor,32,plikWe);
			break;
		}
	}
	char c;
	//save offset for further rewind
	offset = ftell(plikWe);
	uint64_t liczba=0;
	//Count number of individual angles to calculate single step difference (in .xrdml we get accurate initial and final angle data, single step info is too roughly rounded)
	do
	{
		c=fgetc(plikWe);
		if(c==' ')
		{
			liczba++;
		}
		if(feof(plikWe))
		{
			break;
		}
	}
	while(c!='<');	
	
	//after this - rewind file to start of data
	fseek(plikWe, offset, SEEK_SET);
	//Print header in output file. Use CR+LF for widest OS support
	fprintf(plikWy,"2theta,intensity,\ndegree,a.u.,\r\n");
	long double Dtheta=(stop-start)/((long double) liczba);
	
	//While looping: count lines, if EOF detected - stop
	for(uint64_t ii=0;!feof(plikWe);ii++)
	{
		//Print single line:
		//Print angle with 0.8Lf precission 
		fprintf(plikWy,"%0.8Lf,",start+((long double)ii)*Dtheta);
		do
		{
			//Just copy values from input (to avoid unnecessary conversion losses)
			c=fgetc(plikWe);
			if(c=='<'||c==' ')
			{
				break;
			}
			fprintf(plikWy,"%c",c);
			//Space ' ' is separator between measured data - start new datapoint upon detection
		}
		while(c!=' ');
		
		//If '<' - start of "</intensities> is found - it's end of data - break the loop
		if(c=='<')
		{
			break;
		}
		//Add end of line (CR+LF for wide OS support)
		fprintf(plikWy,",\r\n");
	}
	
	//Close input and output files
	fclose(plikWe);
	fclose(plikWy);
	return 0;
}

//Print GPL notice at startup
void GPLnotice()
{
	fprintf(stdout,"XRDML_2_CSV, Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).\n\n");
	fprintf(stdout,"Copyright (C) 2022  Aleksander Szpakiewicz-Szatan\n");
    fprintf(stdout,"This program is free software: you can redistribute it and/or modify\n");
    fprintf(stdout,"it under the terms of the GNU General Public License as published\n");
    fprintf(stdout,"the Free Software Foundation, either version 3 of the License, or\n");
    fprintf(stdout,"(at your option) any later version.\n\n");

    fprintf(stdout,"This program is distributed in the hope that it will be useful,\n");
    fprintf(stdout,"but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    fprintf(stdout,"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    fprintf(stdout,"GNU General Public License for more details.\n\n");

    fprintf(stdout,"You should have received a copy of the GNU General Public License\n");
    fprintf(stdout,"along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n");
    
    fprintf(stdout,"Source available: <https://github.com/Aleszp/XRDML_2_CSV>.\n");
    fprintf(stdout,"Contact: aleksander.szsz(a)gmail.com\n\n");
}

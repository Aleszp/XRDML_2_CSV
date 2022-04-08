#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "messages.h"

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

//print Help upon request
void printHelp()
{
	fprintf(stdout,"Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).\nUsage:\n");
	fprintf(stdout,"xrdml2xy [option] input_file.xrdml [output_file.csv].\n");
	fprintf(stdout,"xrdml2xy [-h]\n");
	fprintf(stdout,"-s - prevents display of GNU License Notice (silent).\n");
	fprintf(stdout,"-h, -? - displays this help.\n");
	fprintf(stdout,"-c, force (default) comma \',\' as separator.\n");
	fprintf(stdout,"-S, force semicolon \';\' as separator.\n");
	fprintf(stdout,"-t, force tabulator \'\\t\' as separator.\n");
	fprintf(stdout,"If more then one separator selecting switch is used, behaviour is undefined, probably based on order.\n");
}

//print if wrong number of parameters was used
void wrongUsage()
{
	fprintf(stderr,"Wrong number of arguments, besides swithces there input filename and output filename are required.\nPlease run program with -h switch for help.\n");
}

//Inform user that file [path] could not be opened in [mode] mode
void wrongFile(char* mode,char* path)
{
	fprintf(stderr,"Could not open file for %s: %s\n",mode,path);
}

int handleStartup(int argc,char** argv,int* optind_,char* separator)
{
	uint_fast8_t notice=1,help=0;
	char cc;
	//Detect cli switches and set proper flags
	while((cc=getopt(argc,argv,":shctS"))!=-1)
	{	
		switch(cc)
		{
			//should help be printed?
			case '?':
			case 'h': 
				help=1;
				break;
			//should GNU Notice be supressed?
			case 's':
				notice=0;
				break;
				
			//force ',' as separator
			case 'c':
				*separator=',';
				break;
			//force '\t' as separator
			case 't':
				*separator='\t';
				break;
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

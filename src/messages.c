/*
 * Part of program XRDML_2_CSV
 * Messages functions implementation file.
 * 
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * Licensed under GPL-3.0 License
 */ 

#include <stdio.h>

#include "error.h"
#include "messages.h"

/**
 * Print GPL notice (at startup)
 */ 
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

/**
 * print Help (upon request)
 */ 
void printHelp()
{
	fprintf(stdout,"Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).\nUsage:\n");
	fprintf(stdout,"xrdml2xy [option] input_file.xrdml [output_file.ext].\n");
	fprintf(stdout,"xrdml2xy [-h]\n");
	fprintf(stdout,"-n - prevents display of GNU License Notice (no-notice).\n");
	fprintf(stdout,"-h, -? - displays this help.\n");
	fprintf(stdout,"-c - force (default) comma \',\' as separator.\n");
	fprintf(stdout,"-S - force semicolon \';\' as separator.\n");
	fprintf(stdout,"-s - force space \' \' as separator.\n");
	fprintf(stdout,"-t - force tabulator \'\\t\' as separator.\n");
	fprintf(stdout,"If more then one separator selecting switch is used, behaviour is undefined, probably based on order.\n");
	fprintf(stdout,"If no separator selecting switch is used, autodetection based on extention is performed: \n.txt - space, \n.tsv or .xy - tab, \n.ssv - semicolon, \n.csv (or as fallback if unrecognised extention is used) - comma.\n");
	fprintf(stdout,"CR+LF is used as line end for widest OS compability.\n");
}

/**
 * Inform user that wrong number of parameters was used
 */
void wrongUsage()
{
	fprintf(stderr,"Wrong number of arguments, besides swithces there input filename and output filename are required.\nPlease run program with -h switch for help.\n");
}

/**
 * Inform user that file  could not be opened in chosen mode
 * @param mode - mode in which user tried to open file (text to print in message i.e. "reading"
 * @param path - name (and path) of file that user tired to open
 */
void wrongFile(char* mode,char* path)
{
	fprintf(stderr,"Could not open file for %s: %s\n",mode,path);
}


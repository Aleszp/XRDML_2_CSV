/*
 * Program XRDML_2_CSV
 * Simple CLI utility for extraction of XRD data from XRDML format into CSV compatible (or into other ASCII based format).
 * Author: mgr inż. Aleksander Szpakiewicz-Szatan
 * (c) 2021-2022
 * version alpha-1.1
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void GPLnotice();

int main(int argc, char** argv)
{
	void GPLnotice();
	if(argc!=3)
	{
		fprintf(stderr,"Nieprawidłowa liczba argumentów. Składnia: xrdml2xy [plikwe] [plikwy].\n");
		return 1;
	}
	FILE* plikWe=fopen(argv[1],"r");
	if(!plikWe)
	{
		fprintf(stderr,"Błąd otwarcia pliku do odczytu: %s\n",argv[1]);
		return 2;
	}
	FILE* plikWy=fopen(argv[2],"w");
	if(!plikWy)
	{
		fprintf(stderr,"Błąd otwarcia pliku do zapisu: %s\n",argv[2]);
		return 3;
	}
	char bufor[255];
	char* wsk;
	
	long double start,stop;
	while(!feof(plikWe))
	{
		//pomiń nagłówek aż do kątów
		fgets(bufor,255,plikWe);
		wsk=strstr(bufor,"<startPosition>");
		if(wsk!=NULL)
		{
			break;
		}
	}
	sscanf(wsk+15,"%Lf",&start);
	fgets(bufor,255,plikWe);
	wsk=strstr(bufor,"<endPosition>");
	sscanf(wsk+13,"%Lf",&stop);
	
	long offset;
	
	while(!feof(plikWe))
	{
		//pomiń nagłówek aż do danych
		//zachowaj położenie
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
	//zachowaj położenie pliku (do przewinięcia)
	offset = ftell(plikWe);
	uint64_t liczba=0;
	//Zlicz ilość kątów by policzyć przyrost (z pliku xrdml znamy kąt początkowy i końcowy, krok w pliku xrdml jest zbyt zaokrąglony)
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
	
	fseek(plikWe, offset, SEEK_SET);
	
	fprintf(plikWy,"2theta,intensity,\ndegree,a.u.,\r\n");
	long double Dtheta=(stop-start)/((long double) liczba);
	printf("Dtheta=%Lf\n",Dtheta);
	for(uint64_t ii=0;!feof(plikWe);ii++)
	{
		fprintf(plikWy,"%0.8Lf,",start+((long double)ii)*Dtheta);
		do
		{
			c=fgetc(plikWe);
			if(c=='<'||c==' ')
			{
				break;
			}
			fprintf(plikWy,"%c",c);
		}
		while(c!=' ');
		if(c=='<')
		{
			break;
		}
		fprintf(plikWy,",\r\n");
	}
	
	fclose(plikWe);
	fclose(plikWy);
	return 0;
}

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

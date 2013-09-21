/**************************************************************
*		File: compile_psl.cpp                                  *
*		ID: PS_compile_psl_cpp                                 *
*		Author: Alexandru Radovici                             *
*		Date: February 5, 2004                                 *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file contains the source code for        *
*               building text language files (.pslt) to       *
*               binary compressed language files (.psl)       *
*               for PhotoStorm.                               *
**************************************************************/

#include <zlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct psl_date_
{
    char var[100];
    char valoare[1000];
} psl_date;

struct text_
{
    char original[32000];
    char tradus[32000];
} texte;

int main(int argc, char *argv[])
{
    FILE *f;
    gzFile t;
	
	printf ("Building %s\n   Output File: %s\n",argv[1],argv[2]);
	
    f=fopen (argv[1],"r");
    unsigned short n=0;
    if (f!=NULL) 
	{
		char s[64000];
		t=gzopen (argv[2],"wb");
		printf ("   Signing file\n");
		gzwrite (t,"PSL",3);
		n=3; //PSL
		n=n+2; //loc pentru n
		printf ("   Scanning for variables\n");
		
		while (!feof(f))
    	{
			fscanf (f,"%[^\n]\n",&s);
		
			if (s[0]=='#')
			{
			    sscanf (&s[1],"%s %[^\n]\n",psl_date.var, psl_date.valoare);
	    		//printf ("     %s=%s\n",psl_date.var, psl_date.valoare);
				unsigned short v;
		
				v=(unsigned short)strlen(psl_date.var);
				//gzwrite (t,&v,sizeof(unsigned short));
				//gzwrite (t,psl_date.var,(unsigned int)v);
				n=n+2+v;
				
				v=strlen(psl_date.valoare);
				//gzwrite (t,&v,sizeof(unsigned short));
				//gzwrite (t,psl_date.valoare,(unsigned int)v);
				n=n+2+v;
			}
		}
		
		printf ("   Setting header length\n");
		
		gzwrite (t,&n,sizeof (unsigned short));
		
		fseek (f,0,SEEK_SET);
		
		printf ("   Writing header data\n");
		printf ("   Variables are:\n");
		
		while (!feof(f))
    	{
			fscanf (f,"%[^\n]\n",&s);
		
			if (s[0]=='#')
			{
			    sscanf (&s[1],"%s %[^\n]\n",psl_date.var, psl_date.valoare);
	    		printf ("     %s=%s\n",psl_date.var, psl_date.valoare);
				unsigned short v;
		
				v=(unsigned short)strlen(psl_date.var);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,psl_date.var,(unsigned int)v);
			
				v=strlen(psl_date.valoare);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,psl_date.valoare,(unsigned int)v);
			}
		}
		
		fseek (f,0,SEEK_SET);
		
		printf ("   Writing texts\n");
		printf ("   Texts are:\n");
		
		while (!feof(f))
    	{
			fscanf (f,"%[^\n]\n",&s);
		
			if (s[0]=='@')
			{
			    sscanf (&s[1],"`%[^`]` `%[^`]\n",texte.original, texte.tradus);
	    		printf ("      [%s]=[%s]\n",texte.original, texte.tradus);
				unsigned short v;
		
				v=(unsigned short)strlen(texte.original);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,texte.original,(unsigned int)v);
			
				v=strlen(texte.tradus);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,texte.tradus,(unsigned int)v);
				n=n+v;
			}
		}
		
		gzclose (t);
		fclose (f);
		
		printf ("Building complete.\n");
    }
    return 0;
}

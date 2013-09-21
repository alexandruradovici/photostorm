/**************************************************************
*		File: compile_q.cpp                                    *
*		ID: PS_compile_q_cpp                                   *
*		Author: Alexandru Radovici                             *
*		Date: April 30, 2004                                   *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file contains the source code for        *
*               building quote databases (.psq) from text     *
*               files (.txt) for PhotoStorm.                  *
**************************************************************/

#include "../quotes.h"
#include <zlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct psq_date_
{
    char var[100];
    char valoare[1000];
} psq_date;

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
		gzwrite (t,"PSQ",3);
		n=3; //PSL
		n=n+2; //loc pentru n
		printf ("   Scanning for variables\n");
		
		while (!feof(f))
    	{
			fscanf (f,"%[^\n]\n",&s);
			if (s[0]=='#')
			{
			    sscanf (&s[1],"%s %[^\n]\n",psq_date.var, psq_date.valoare);
	    		//printf ("     %s=%s\n",psl_date.var, psl_date.valoare);
				unsigned short v;
		
				v=(unsigned short)strlen(psq_date.var);
				//gzwrite (t,&v,sizeof(unsigned short));
				//gzwrite (t,psl_date.var,(unsigned int)v);
				n=n+2+v;
				
				v=strlen(psq_date.valoare);
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
			    sscanf (&s[1],"%s %[^\n]\n",psq_date.var, psq_date.valoare);
	    		printf ("     %s=%s\n",psq_date.var, psq_date.valoare);
				unsigned short v;
		
				v=(unsigned short)strlen(psq_date.var);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,psq_date.var,(unsigned int)v);
			
				v=strlen(psq_date.valoare);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,psq_date.valoare,(unsigned int)v);
			}
		}
		
		fseek (f,0,SEEK_SET);
		
		printf ("   Writing quotes\n");
		printf ("   Quotes are:\n");
		
		while (!feof(f))
    	{
			fscanf (f,"%[^\n]\n",&s);
		
			//printf ("line: %s\n", s);
			if (s[0]!='#' && strcmp (s,"")!=0)
			{
				char text[10000];
				char autor [100];
				char keywords [100];
				strcpy (text, s);
				fscanf (f,"%[^\n]\n",autor);
				fscanf (f,"%[^\n]\n",keywords);
	    		printf ("      Quote:\n			Quote: %s\n			Author: %s\n			Keywords: %s\n",text , autor, keywords);
				unsigned short v;
		
				v=(unsigned short)strlen(text);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,text,(unsigned int)v);
			
				v=strlen(autor);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,autor,(unsigned int)v);
				
				v=(unsigned short)strlen(keywords);
				gzwrite (t,&v,sizeof(unsigned short));
				gzwrite (t,keywords,(unsigned int)v);
				n=n+v;
			}
		}
		
		gzclose (t);
		fclose (f);
		
		printf ("Building complete.\n");
    }
    return 0;
}

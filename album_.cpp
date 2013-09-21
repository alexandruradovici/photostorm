/**************************************************************
*		File: album.cpp                                        *
*		ID: PS_album_cpp                                       *
*		Author: Alexandru Radovici                             *
*		Date: February 9, 2004                                 *
*		Modified:                                              *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This is the album processing source file of   *
*      PhotoStorm.                                            *
***************************************************************/

#include "album.h"
#include "data.h"
#include "design.h"
#include "misc.h"
#include "lang/translate.h"

#include <zlib.h>
#include <crypt.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <zlib.h>
#include <dirent.h>

char *current_album;

char * ps_get_current_album ()
{
	return strdup (current_album);
}

#ifdef LINUX
void ps_load_albums_from_path (char *calea)
{
	if (calea!=NULL)
	{
		struct dirent **namelist;
		int n=scandir (calea, &namelist, 0, alphasort);
		if (n<0) printf (__("Error loading albums from %s."), calea);
		else
		{
			for (int i=0;i<n;i++)
			{
				char *ext=NULL;
				if (rindex (namelist[i]->d_name, '.')!=NULL) ext=strdup(rindex (namelist[i]->d_name, '.'));
				if (ext!=NULL)
				{
					if (strcasecmp (ext,".psa")==0)
					{
						char *temp;
						int t=1;
						if (calea[strlen(calea)]!='/') t=2;
						temp=(char*)malloc (strlen (calea)+strlen(namelist[i]->d_name)+t);
						if (temp!=NULL)
						{
							strcpy (temp, calea);
							if (t==2) strcat (temp, "/");
							strcat (temp, namelist[i]->d_name);
							ps_add_album_from_file (temp);
							free (temp);
						}
					}
					free (ext);
				}
				free (namelist[i]);
			}
		}
	}
}
#endif

int ps_create_new_albume (char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola, char *file)
{
	int result=0;
	_ps_album *date_album = ps_create_new_album_in_memory (nume, autor, categoria, descriere, metadata, parola);
	if (date_album!=NULL)
	{
		result = ps_write_album_from_memory (file, date_album);
		ps_album_free (date_album);
	}
	return result;
}

_ps_album * ps_create_new_album_in_memory (char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola)
{
	_ps_album *date_album = NULL;
	if (nume!=NULL)
	{
		if (autor==NULL) autor=strdup("");
		if (categoria==NULL) categoria=strdup ("");
		if (descriere==NULL) descriere=strdup ("");
		if (metadata==NULL) metadata=strdup ("");
		if (strlen(nume)>255) nume[255]='\0';
		if (strlen(autor)>255) autor[255]='\0';
		if (strlen(categoria)>255) categoria[255]='\0';
		if (strlen(descriere)>32000) descriere[32000]='\0';
		if (strlen(metadata)>32000) metadata[32000]='\0';
		if (parola!=NULL) 
		{ 
			if (strlen(parola)>255) parola[255]='\0'; 
		}
		else parola = strdup ("");
		parola = crypt (parola, "$1$PS_Album$");
		parola = &parola[12];
		time_t tmp;
		time (&tmp);
		date_album = (_ps_album*)malloc (sizeof(_ps_album));
		if (date_album!=NULL)
		{
			date_album->nume=ps_replace_str (nume, 255);
			date_album->autor=ps_replace_str (autor, 255);
			date_album->categoria=ps_replace_str (categoria, 255);
			date_album->descriere=ps_replace_str (descriere, 32000);
			date_album->metadata=ps_replace_str (metadata, 32000);
			date_album->parola=ps_replace_str (parola, 255);
			date_album->data=tmp;
			date_album->nr_poze=0;
			date_album->n_poze=0;
			date_album->date=NULL;
		}
	}
	return date_album;
}

void ps_add_album_from_file (char *albumul)
{
	if (albumul!=NULL)
	{
		_ps_album *date_album = ps_load_album (albumul);
		if (date_album!=NULL)
		{
			adauga_album (date_album->categoria, date_album->nume, albumul, 1);
		}
	}
}

_ps_album * ps_load_album (char *albumul)
{
	_ps_album * result=NULL;
	short n_antet;
	unsigned char n_nume;
	unsigned char n_autor;
	unsigned char n_categoria;
	unsigned short n_descriere;
	unsigned short n_metadata;
	unsigned char n_parola;
	unsigned char n_version;
	unsigned char nr_poze;
	long n_poze;
	long e=0;
	char * antet = NULL;
	char * pozele = NULL;
	if (albumul!=NULL)
	{
		gzFile f;
		f=gzopen (albumul, "rb");
		if (f!=NULL)
		{
			char id[4];
			char *version=NULL;
			e=gzread (f, &id, 3);
			id[3]='\0';
			if (e!=3) e=-1;
			if (strcmp (id,"PSA")!=0) e=-1;
			if (e!=-1) { e=gzread (f,&n_version,1); if (e!=1) e=-1; }
			if (e!=-1) 
			{ 
				version=(char*)malloc (n_version+1);
				e=gzread (f,version,n_version); 
				version[n_version]='\0';
				if (e!=n_version) e=-1; 
				if (strcmp (version,"0.1")!=0) e=-1;
			}
			if (e!=-1) { e=gzread (f,&n_antet,2); if (e!=2) e=-1; }
			if (e!=-1 && n_antet>0)
			{
				antet=(char*)malloc (n_antet);
				if (antet!=NULL) e=gzread (f,&antet[6+n_version],n_antet-6-n_version); else e=-1;
				if (e!=n_antet-6-n_version) e=-1;
			} else e=-1;
			memcpy (&nr_poze, &antet[n_antet-5], 1);
			memcpy (&n_poze, &antet[n_antet-4], 4);
			if (n_poze>0)
			{
				pozele = (char*)malloc (n_poze);
				if (pozele!=NULL) e=gzread (f, pozele, n_poze); else e=-1;
				if (e!=n_poze) e=-1;
			} else n_poze=0;
			if (e!=-1)
			{
				result = (_ps_album*)malloc (sizeof(_ps_album));
				if (result!=NULL)
				{
					result->n_antet=n_antet;
					result->n_poze=n_poze;
					result->nr_poze=nr_poze;
					printf ("Nr de poze: %d\n",nr_poze);
					memcpy (&antet[0],id,3);
					memcpy (&antet[3],&n_version,1);
					memcpy (&antet[4],version,n_version);
					memcpy (&antet[4+n_version],&n_antet,2);
					result->date = (char*)malloc (n_antet+n_poze);
					//memcpy (&result->date[0], antet, n_antet); - MODIFICARE
					result->antet=antet;
					if (n_poze>0) result->date=pozele;
					//memcpy (&result->date[n_antet], pozele, n_poze); - MODIFICAT
					// despartirea antetului
					long pos=4+n_version+2;
					// numele
					memcpy (&n_nume, &antet[pos], 1);
					pos=pos+1;
					result->nume=(char*)malloc (n_nume+1);
					if (result->nume!=NULL)
					{
						memcpy (&result->nume[0], &antet[pos], n_nume);
						result->nume[n_nume]='\0';
					} else n_nume=0;
					pos=pos+n_nume;
					printf ("%d: %s\n", pos, result->nume);
					// autorul
					memcpy (&n_autor, &antet[pos], 1);
					pos=pos+1;
					result->autor=(char*)malloc (n_autor+1);
					if (result->autor!=NULL)
					{
						memcpy (&result->autor[0], &antet[pos], n_autor);
						result->autor[n_autor]='\0';
					} else n_autor=0;
					pos=pos+n_autor;
					printf ("%d: %s\n", pos, result->autor);
					// categoria
					memcpy (&n_categoria, &antet[pos], 1);
					pos=pos+1;
					result->categoria=(char*)malloc (n_categoria+1);
					if (result->categoria!=NULL)
					{
						memcpy (&result->categoria[0], &antet[pos], n_categoria);
						result->categoria[n_categoria]='\0';
					} else n_categoria=0;
					pos=pos+n_categoria;
					printf ("%d: %s\n", pos, result->categoria);
					// descrierea
					memcpy (&n_descriere, &antet[pos], 2);
					pos=pos+2;
					result->descriere=(char*)malloc (n_descriere+1);
					if (result->descriere!=NULL)
					{
						memcpy (&result->descriere[0], &antet[pos], n_descriere);
						result->descriere[n_descriere]='\0';
					} else n_descriere=0;
					pos=pos+n_descriere;
					printf ("%d: %s\n", pos, result->descriere);
					// metadata
					memcpy (&n_metadata, &antet[pos], 2);
					pos=pos+2;
					result->metadata=(char*)malloc (n_metadata+1);
					if (result->metadata!=NULL)
					{
						memcpy (&result->metadata[0], &antet[pos], n_metadata);
						result->metadata[n_metadata]='\0';
					} else n_metadata=0;
					pos=pos+n_metadata;
					printf ("%d: %s\n", pos, result->metadata);
					// parola
					memcpy (&n_parola, &antet[pos], 1);
					pos=pos+1;
					result->parola=(char*)malloc (n_parola+1);
					if (result->parola!=NULL)
					{
						memcpy (&result->parola[0], &antet[pos], n_parola);
						result->parola[n_parola]='\0';
					} else n_parola=0;
					pos=pos+n_parola;
					printf ("%d: %s\n", pos, result->parola);
					// data
					memcpy (&result->data, &antet[pos], sizeof(time_t));
					pos=pos+sizeof(time_t);
					printf ("%d: %u\n", pos, result->data);
				}
			}
			if (version!=NULL) free (version);
			//if (antet!=NULL) free (antet); - MODIFICAT
			//if (pozele!=NULL) free(pozele); - MODIFICAT
			gzclose (f);
		}
	}
	return result;
}

char * ps_pack_album (_ps_album *album, long &n)
{
	char * result=NULL;
	if (album!=NULL)
	{
		unsigned char n_versiune;
		unsigned char n_nume;
		unsigned char n_autor;
		unsigned char n_categoria;
		unsigned short n_descriere;
		unsigned short n_metadata;
		unsigned char n_parola;
		if (album->nume==NULL) album->nume = strdup ("");
		if (album->autor==NULL) album->autor = strdup ("");
		if (album->categoria==NULL) album->categoria = strdup ("");
		if (album->descriere==NULL) album->descriere = strdup ("");
		if (album->metadata==NULL) album->metadata = strdup ("");
		if (album->parola==NULL) album->parola = strdup ("");
		album->n_antet=4 + strlen(psa_version) + 2 + strlen(album->nume) + 1 
					   + strlen(album->autor) + 1 + strlen(album->categoria) + 1 
					   + strlen(album->descriere) + 2 + strlen(album->metadata) + 2 
					   + strlen(album->parola) + 1 +sizeof(time_t) + 1 + 4;
		n = album->n_antet +  album->n_poze;
		result = (char*)malloc (n);
		long pozitie=0;
		// semnatura PSA
		memcpy (&result[pozitie], "PSA", 3);
		pozitie=pozitie+3;
		// lungimea versiunii
		n_versiune=strlen (psa_version);
		memcpy (&result[pozitie], &n_versiune, 1);
		pozitie=pozitie+1;
		// versiunea
		memcpy (&result[pozitie], &psa_version[0], n_versiune);
		pozitie=pozitie+n_versiune;
		// lungimea totala a antetului
		memcpy (&result[pozitie],&album->n_antet,2);
		pozitie=pozitie+2;
		// lungimea numelui
		n_nume = strlen(album->nume);
		memcpy (&result[pozitie],&n_nume,1);
		pozitie=pozitie+1;
		// numele
		if (n_nume>0) memcpy (&result[pozitie],&album->nume[0],n_nume);
		pozitie=pozitie+n_nume;
		// lungimea autorului
		n_autor = strlen(album->autor);
		memcpy (&result[pozitie],&n_autor,1);
		pozitie=pozitie+1;
		// autorul
		if (n_autor) memcpy (&result[pozitie],&album->autor[0],n_autor);
		pozitie=pozitie+n_autor;
		// lungimea categoriei
		n_categoria = strlen(album->categoria);
		memcpy (&result[pozitie],&n_categoria,1);
		pozitie=pozitie+1;
		// categoria
		if (n_categoria) memcpy (&result[pozitie],&album->categoria[0],n_categoria);
		pozitie=pozitie+n_categoria;
		// lungimea descrierii
		n_descriere = strlen(album->descriere);
		memcpy (&result[pozitie],&n_descriere,2);
		pozitie=pozitie+2;
		// descrierea
		if (n_descriere>0) memcpy (&result[pozitie],&album->descriere[0],n_descriere);
		pozitie=pozitie+n_descriere;
		// lungimea metadata
		n_metadata = strlen(album->metadata);
		memcpy (&result[pozitie],&n_metadata,2);
		pozitie=pozitie+2;
		// metadata
		if (n_metadata>0) memcpy (&result[pozitie],&album->metadata[0],n_metadata);
		pozitie=pozitie+n_metadata;
		// lungimea parolei
		n_parola = strlen(album->parola);
		memcpy (&result[pozitie],&n_parola,1);
		pozitie=pozitie+1;
		// parola
		if (n_parola>0) memcpy (&result[pozitie],&album->parola[0],n_parola);
		pozitie=pozitie+n_parola;
		// data
		memcpy (&result[pozitie],&album->data,sizeof(time_t));
		pozitie=pozitie+sizeof(time_t);
		// nr de poze
		memcpy (&result[pozitie],&album->nr_poze,1);
		pozitie=pozitie+1;
		// lungimea datelor pozelor
		memcpy (&result[pozitie],&album->n_poze,4);
		pozitie=pozitie+4;
		// datele pozelor
		if (album->n_poze>0) memcpy (&result[pozitie],&album->date[0],album->n_poze);
		pozitie=pozitie+album->n_poze;
	}
	return result;	
}

int ps_modify_album_from_file (char *albumul, char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola)
{
	int result = 0;
	_ps_album * date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		if (ps_modify_album_from_memory (date_album, nume, autor, categoria, descriere, metadata, parola))
		{
			result=ps_write_album_from_memory (albumul, date_album);
		}
	}
	return result;
}

int ps_modify_album_from_memory (_ps_album *date_album, char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola)
{
	int result = 0;
	if (date_album!=NULL)
	{
		date_album->nume=ps_replace_str (nume, 255);
		date_album->autor=ps_replace_str (autor, 255);
		date_album->categoria=ps_replace_str (categoria, 255);
		date_album->descriere=ps_replace_str (descriere, 32000);
		date_album->metadata=ps_replace_str (metadata, 32000);
		parola = crypt (parola, "$1$PS_Album$");
		parola = &parola[12];
		date_album->parola=ps_replace_str (parola, 255);
	}
	return result;
}

int ps_modify_photo_nr_from_album_file (_ps_photo *poza, char *albumul, int pos)
{
	int result=0;
	_ps_album * date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		result = ps_modify_photo_nr_from_album_memory (poza, date_album, pos);
		if (result) result=ps_write_album_from_memory (albumul, date_album);
		ps_album_free (date_album);
	}
	return result;
}

int ps_modify_photo_nr_from_album_memory (_ps_photo *poza, _ps_album *date_album, int pos)
{
	int result=0;
	if (date_album!=NULL)
	{
		result = ps_insert_photo_into_album_from_memory_into_memory (poza, date_album, pos);
		if (result) 
		{
			result = ps_delete_photo_nr_from_album_memory (date_album, pos+1);
		}
	}
	return result;
}

int ps_list_album_from_file (char *albumul)
{
	int result=0;
	_ps_album *date_album;
	date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		result = ps_list_album_from_memory (ps_load_album (albumul));
		ps_album_free (date_album);
	} else ps_sterge_lista_poze();
	return result;
}

int ps_list_album_from_memory (_ps_album *date_album)
{
	ps_sterge_lista_poze();
	_ps_photo * date_poza=NULL;
	unsigned char n_nume;
	unsigned char n_autor;
	unsigned short n_descriere;
	unsigned char n_optiuni;
	unsigned short n_metadata;
	long n_poza_preview;
	long n_poza;
	int e_posibil=1;
	if (date_album!=NULL)
	{
		long pozitie=0;
		for (int i=1;i<=date_album->nr_poze;i++)
		{
			long d;
			if (e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				pozitie=pozitie+4;
				if (pozitie+d-4<=(date_album->n_antet+date_album->n_poze))
				{
					date_poza=(_ps_photo*)malloc (sizeof(_ps_photo));
					if (date_poza!=NULL)
					{
						// numele
						n_nume=date_album->date[pozitie];
						//printf ("n_nume=%d\n",n_nume);
						pozitie=pozitie+1;
						date_poza->nume = (char*)malloc (n_nume+1);
						if (date_poza->nume!=NULL)
						{
							if (n_nume>0) memcpy (&date_poza->nume[0], &date_album->date[pozitie], n_nume);
							date_poza->nume[n_nume]='\0';
							pozitie=pozitie+n_nume;
							//printf ("Numele: %s\n",date_poza->nume);
						} 
						else e_posibil=0;
						// autor
						n_autor=date_album->date[pozitie];
						//printf ("n_autor=%d\n",n_autor);
						pozitie=pozitie+1;
						date_poza->autor = (char*)malloc (n_autor+1);
						if (date_poza->autor!=NULL)
						{
							if (n_autor>0) memcpy (&date_poza->autor[0], &date_album->date[pozitie], n_autor);
							date_poza->autor[n_autor]='\0';
							pozitie=pozitie+n_autor;
							//printf ("Autorul: %s\n",date_poza->autor);
						} 
						else e_posibil=0;
						// descriere
						memcpy (&n_descriere, &date_album->date[pozitie], 2);
						//printf ("n_descriere=%d\n",n_descriere);
						pozitie=pozitie+2;
						date_poza->descriere = (char*)malloc (n_descriere+1);
						if (date_poza->descriere!=NULL)
						{
							if (n_descriere>0) memcpy (&date_poza->descriere[0], &date_album->date[pozitie], n_descriere);
							date_poza->descriere[n_descriere]='\0';
							pozitie=pozitie+n_descriere;
							//printf ("Descrierea: %s\n",date_poza->descriere);
						} 
						else e_posibil=0;
						// optiuni
						memcpy (&n_optiuni, &date_album->date[pozitie], 1);
						pozitie=pozitie+1;
						//printf ("n_optiuni=%d\n",n_optiuni);
						date_poza->optiuni = (char*)malloc (n_optiuni+1);
						if (date_poza->optiuni!=NULL)
						{
							if (n_optiuni>0) memcpy (&date_poza->optiuni[0], &date_album->date[pozitie], n_optiuni);
							date_poza->optiuni[n_optiuni]='\0';
							pozitie=pozitie+n_optiuni;
							//printf ("Optiunile: %s\n",date_poza->optiuni);
						} 
						else e_posibil=0;
						// metadata
						memcpy (&n_metadata, &date_album->date[pozitie], 2);
						//printf ("n_metadata=%d\n",n_metadata);
						pozitie=pozitie+2;
						date_poza->metadata = (char*)malloc (n_metadata+1);
						if (date_poza->metadata!=NULL)
						{
							if (n_metadata>0) memcpy (&date_poza->metadata[0], &date_album->date[pozitie], n_metadata);
							date_poza->metadata[n_metadata]='\0';
							pozitie=pozitie+n_metadata;
							//printf ("Metadata: %s\n",date_poza->metadata);
						} 
						else e_posibil=0;
						// preview
						memcpy (&n_poza_preview, &date_album->date[pozitie], 4);
						//printf ("n_poza_preview=%d\n",n_poza_preview);
						pozitie=pozitie+4;
						date_poza->n_poza_preview=n_poza_preview;
						date_poza->date_preview = (char*)malloc (n_poza_preview);
						if (date_poza->date_preview!=NULL)
						{
							if (n_poza>0) memcpy (&date_poza->date_preview[0], &date_album->date[pozitie], n_poza_preview);
							pozitie=pozitie+n_poza_preview;
						} 
						else e_posibil=0;
						// poza
						memcpy (&n_poza, &date_album->date[pozitie], 4);
						//printf ("n_poza=%d\n",n_poza);
						pozitie=pozitie+4;
						date_poza->n_poza=n_poza;
						date_poza->date = (char*)malloc (n_poza);
						if (date_poza->date!=NULL)
						{
							if (n_poza>0) memcpy (&date_poza->date[0], &date_album->date[pozitie], n_poza);
							pozitie=pozitie+n_poza;
						} 
						else e_posibil=0;
					} 
					else e_posibil=0;
				}
			}
			if (e_posibil) adauga_poza (ps_create_pixbuf_from_memory(date_poza->date_preview, date_poza->n_poza_preview), 
									date_poza->nume, date_poza->autor, date_poza->descriere,
									date_poza->optiuni, date_poza->metadata, i, TRUE);
			if (date_poza!=NULL) ps_photo_free (date_poza);
		}
	}
	if (e_posibil) ps_lista_poze_selecteaza_prima_poza ();
	return e_posibil;
}

_ps_photo * ps_load_photo_nr_from_album_file (char *albumul, int pos)
{
	_ps_photo *date_poza=NULL;
	_ps_album *date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		date_poza = ps_load_photo_nr_from_album_memory (date_album, pos);
		ps_album_free (date_album);
	}
	return date_poza;
}

_ps_photo * ps_load_photo_nr_from_album_memory (_ps_album *date_album, int pos)
{
	_ps_photo * date_poza=NULL;
	unsigned char n_nume;
	unsigned char n_autor;
	unsigned short n_descriere;
	unsigned char n_optiuni;
	unsigned short n_metadata;
	long n_poza_preview;
	long n_poza;
	int e_posibil=1;
	if (date_album!=NULL)
	{
		if (date_album->nr_poze<pos) e_posibil=0;
		if (e_posibil)
		{
			int i=1;
			long pozitie=0;
			long d;
			while (i<pos && e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				pozitie=pozitie+d;
				//printf ("%u / %u\n", pozitie, date_album->n_antet+date_album->n_poze);
				if (pozitie>(date_album->n_antet+date_album->n_poze)) e_posibil=0;
				//printf ("%d, ",i);
				i++;
			}
			if (i>pos) e_posibil=0;
			//printf ("\n");
			//abort ();
			if (e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				pozitie=pozitie+4;
				if (pozitie+d-4<=(date_album->n_antet+date_album->n_poze))
				{
					date_poza=(_ps_photo*)malloc (sizeof(_ps_photo));
					if (date_poza!=NULL)
					{
						// numele
						n_nume=date_album->date[pozitie];
						//printf ("n_nume=%d\n",n_nume);
						pozitie=pozitie+1;
						date_poza->nume = (char*)malloc (n_nume+1);
						if (date_poza->nume!=NULL)
						{
							if (n_nume>0) memcpy (&date_poza->nume[0], &date_album->date[pozitie], n_nume);
							date_poza->nume[n_nume]='\0';
							pozitie=pozitie+n_nume;
							//printf ("Numele: %s\n",date_poza->nume);
						} 
						else e_posibil=0;
						// autor
						n_autor=date_album->date[pozitie];
						//printf ("n_autor=%d\n",n_autor);
						pozitie=pozitie+1;
						date_poza->autor = (char*)malloc (n_autor+1);
						if (date_poza->autor!=NULL)
						{
							if (n_autor>0) memcpy (&date_poza->autor[0], &date_album->date[pozitie], n_autor);
							date_poza->autor[n_autor]='\0';
							pozitie=pozitie+n_autor;
							//printf ("Autorul: %s\n",date_poza->autor);
						} 
						else e_posibil=0;
						// descriere
						memcpy (&n_descriere, &date_album->date[pozitie], 2);
						//printf ("n_descriere=%d\n",n_descriere);
						pozitie=pozitie+2;
						date_poza->descriere = (char*)malloc (n_descriere+1);
						if (date_poza->descriere!=NULL)
						{
							if (n_descriere>0) memcpy (&date_poza->descriere[0], &date_album->date[pozitie], n_descriere);
							date_poza->descriere[n_descriere]='\0';
							pozitie=pozitie+n_descriere;
							//printf ("Descrierea: %s\n",date_poza->descriere);
						} 
						else e_posibil=0;
						// optiuni
						memcpy (&n_optiuni, &date_album->date[pozitie], 1);
						pozitie=pozitie+1;
						//printf ("n_optiuni=%d\n",n_optiuni);
						date_poza->optiuni = (char*)malloc (n_optiuni+1);
						if (date_poza->optiuni!=NULL)
						{
							if (n_optiuni>0) memcpy (&date_poza->optiuni[0], &date_album->date[pozitie], n_optiuni);
							date_poza->optiuni[n_optiuni]='\0';
							pozitie=pozitie+n_optiuni;
							//printf ("Optiunile: %s\n",date_poza->optiuni);
						} 
						else e_posibil=0;
						// metadata
						memcpy (&n_metadata, &date_album->date[pozitie], 2);
						//printf ("n_metadata=%d\n",n_metadata);
						pozitie=pozitie+2;
						date_poza->metadata = (char*)malloc (n_metadata+1);
						if (date_poza->metadata!=NULL)
						{
							if (n_metadata>0) memcpy (&date_poza->metadata[0], &date_album->date[pozitie], n_metadata);
							date_poza->metadata[n_metadata]='\0';
							pozitie=pozitie+n_metadata;
							//printf ("Metadata: %s\n",date_poza->metadata);
						} 
						else e_posibil=0;
						// preview
						memcpy (&n_poza_preview, &date_album->date[pozitie], 4);
						//printf ("n_poza_preview=%d\n",n_poza_preview);
						pozitie=pozitie+4;
						date_poza->n_poza_preview=n_poza_preview;
						date_poza->date_preview = (char*)malloc (n_poza_preview);
						if (date_poza->date_preview!=NULL)
						{
							if (n_poza>0) memcpy (&date_poza->date_preview[0], &date_album->date[pozitie], n_poza_preview);
							pozitie=pozitie+n_poza_preview;
						} 
						else e_posibil=0;
						// poza
						memcpy (&n_poza, &date_album->date[pozitie], 4);
						//printf ("n_poza=%d\n",n_poza);
						pozitie=pozitie+4;
						date_poza->n_poza=n_poza;
						date_poza->date = (char*)malloc (n_poza);
						if (date_poza->date!=NULL)
						{
							if (n_poza>0) memcpy (&date_poza->date[0], &date_album->date[pozitie], n_poza);
							pozitie=pozitie+n_poza;
						} 
						else e_posibil=0;
					} 
					else e_posibil=0;
				} else e_posibil=0;
			}
		}
	}
	if (!e_posibil) if (date_poza!=NULL) ps_photo_free (date_poza);
	return date_poza;
}

int ps_delete_photo_nr_from_album_file (char *albumul, int pos)
{
	int e_posibil=0;
	_ps_album * date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		e_posibil=ps_delete_photo_nr_from_album_memory (date_album, pos);
		ps_album_free (date_album);
	}
	return e_posibil;
}

int ps_delete_photo_nr_from_album_memory (_ps_album *date_album, int pos)
{
	int e_posibil=1;
	if (date_album!=NULL)
	{
		if (date_album->nr_poze<pos) e_posibil=0;
		if (e_posibil)
		{
			int i=1;
			long pozitie=0;
			long date_ramase=date_album->n_poze;
			long d;
			if (i>pos) e_posibil=0;
			while (i<pos && e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				pozitie=pozitie+d;
				date_ramase=date_ramase-d;
				//printf ("%u / %u\n", pozitie, date_album->n_antet+date_album->n_poze);
				if (pozitie>(date_album->n_antet+date_album->n_poze)) e_posibil=0;
				//printf ("%d, ",i);
				i++;
			}
			//printf ("\n");
			//abort ();
			if (e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				if (pozitie+d<=(date_album->n_antet+date_album->n_poze))
				{
					char *temp;
					temp=(char*)malloc (date_album->n_poze-d);
					if (temp!=NULL)
					{
						memcpy (&temp[0], &date_album->date[0], pozitie);
						date_ramase=date_ramase-d;
						memcpy (&temp[pozitie], &date_album->date[pozitie+d], date_ramase);
						if (date_album->date!=NULL) free (date_album->date);
						date_album->date=temp;
						date_album->n_poze=date_album->n_poze-d;
						date_album->nr_poze--;
					}
				} else e_posibil=0;
			}
		}
	}
	return e_posibil;
}

int ps_add_photo_to_album_from_file (char *poza, char *albumul)
{
	int result = 0;
	_ps_album *date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		result = ps_add_photo_to_album_from_file_into_memory (poza, date_album);
		ps_album_free (date_album);
	}
	return result;
}

int ps_add_photo_to_album_from_file_into_memory (char *poza, _ps_album *date_album)
{
	int result = 0;
	_ps_photo *date_poza = ps_create_photo (poza);
	if (date_poza!=NULL)
	{
		result = ps_add_photo_to_album_from_memory_into_memory (date_poza, date_album);
		ps_photo_free (date_poza);
	}
	return result;
}

int ps_add_photo_to_album_from_memory (_ps_photo *date_poza, char *albumul)
{
	int result = 0;
	_ps_album *date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		result = ps_add_photo_to_album_from_memory_into_memory (date_poza, date_album);
		ps_album_free (date_album);
	}
	return result;
}

int ps_add_photo_to_album_from_memory_into_memory (_ps_photo *date_poza, _ps_album *date_album)
{
	int result = 0;
	long n_poza;
	char *temp=NULL;
	char *poza_scris=NULL;
	if (date_poza!=NULL) poza_scris = ps_pack_photo (date_poza, n_poza);
	if (date_album!=NULL && poza_scris!=NULL)
	{
		if (date_album!=NULL)
		{
			date_album->nr_poze++;
			temp = (char*)malloc (date_album->n_poze+n_poza);
			if (temp!=NULL)
			{
				memcpy (&temp[0], &date_album->date[0], date_album->n_poze);
				memcpy (&temp[date_album->n_poze], poza_scris, n_poza);
				date_album->n_poze=date_album->n_poze+n_poza;
				free (date_album->date);
				date_album->date=temp;
				result=1;
			}
			if (poza_scris!=NULL) free (poza_scris);
		}
	}
	return result;
}

int ps_insert_photo_into_album_from_file (char *poza, char *albumul, int pos)
{
	int e_posibil=0;
	_ps_album * date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		e_posibil=ps_insert_photo_into_album_from_file_into_memory (poza, date_album, pos);
		ps_album_free (date_album);
	}
	return e_posibil;
}

int ps_insert_photo_into_album_from_file_into_memory (char *poza, _ps_album *date_album, int pos)
{
	int e_posibil=0;
	_ps_photo *date_poza = ps_create_photo (poza);
	if (date_poza!=NULL)
	{
		e_posibil = ps_insert_photo_into_album_from_memory_into_memory (date_poza, date_album, pos);
		ps_photo_free (date_poza);
	}
	return e_posibil;
}

int ps_insert_photo_into_album_from_memory (_ps_photo *date_poza, char *albumul, int pos)
{
	int e_posibil=0;
	_ps_album *date_album = ps_load_album (albumul);
	if (date_album!=NULL)
	{
		e_posibil = ps_insert_photo_into_album_from_memory_into_memory (date_poza, date_album, pos);
		ps_album_free (date_album);
	}
	return e_posibil;
}

int ps_insert_photo_into_album_from_memory_into_memory (_ps_photo *date_poza, _ps_album *date_album, int pos)
{
	int e_posibil = 1;
	long n_poza;
	char *temp=NULL;
	char *poza_scris=NULL;
	if (date_poza!=NULL) poza_scris = ps_pack_photo (date_poza, n_poza);
	if (date_album!=NULL && poza_scris!=NULL)
	{
		if (date_album!=NULL)
		{
			int i=1;
			long pozitie=0;
			long d;
			if (i>pos) e_posibil=0;
			while (i<pos && e_posibil)
			{
				memcpy (&d, &date_album->date[pozitie], 4);
				pozitie=pozitie+d;
				//printf ("%u / %u\n", pozitie, date_album->n_antet+date_album->n_poze);
				if (pozitie>(date_album->n_antet+date_album->n_poze)) e_posibil=0;
				//printf ("%d, ",i);
				i++;
			}
			if (e_posibil)
			{
				date_album->nr_poze++;
				long date_ramase=date_album->n_poze-pozitie;
				temp = (char*)malloc (date_album->n_poze+n_poza);
				if (temp!=NULL)
				{
					memcpy (&temp[0], &date_album->date[0], pozitie);
					memcpy (&temp[pozitie], poza_scris, n_poza);
					memcpy (&temp[pozitie+n_poza], &date_album->date[pozitie], date_ramase);
					date_album->n_poze=date_album->n_poze+n_poza;
					free (date_album->date);
					date_album->date=temp;
					e_posibil=1;
				}
			}
			if (poza_scris!=NULL) free (poza_scris);
		} else e_posibil=0;
	} else e_posibil=0;
	return e_posibil;
}

void ps_album_free (_ps_album *date_album)
{
	if (date_album!=NULL) 
	{
		if (date_album->nume!=NULL) free (date_album->nume);
		if (date_album->autor!=NULL) free (date_album->autor);
		if (date_album->categoria!=NULL) free (date_album->categoria);
		if (date_album->descriere!=NULL) free (date_album->descriere);
		if (date_album->metadata!=NULL) free (date_album->metadata);
		if (date_album->parola!=NULL) free (date_album->parola);
		if (date_album->antet!=NULL) free (date_album->antet);
		if (date_album->date!=NULL) free (date_album->date);				
		free (date_album);
	}
}

int ps_write_album (char *albumul, char *date, long n)
{
	int e=0;
	gzFile f;
	f=gzopen (albumul, "wb");
	if (f!=NULL)
	{
		e=gzwrite (f, date, n);
		gzclose (f);
	}
	return e;
}

int ps_write_album_from_memory (char *albumul, _ps_album *date_album)
{
	int result=0;
	long n;
	char *date = ps_pack_album (date_album, n);
	if (date!=NULL)
	{
		result=ps_write_album (albumul, date, n);
		free (date);
	}
	return result;
}

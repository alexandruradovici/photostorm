/**************************************************************
*		File: photo.cpp                                        *
*		ID: PS_photo_cpp                                       *
*		Author: Alexandru Radovici                             *
*		Date: February 9, 2004                                 *
*		Modified:                                              *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This is the photo processing source file of   *
*      PhotoStorm.                                            *
***************************************************************/

#include "data.h"

#include "photo.h"

#include "configs.h"

#include "misc.h"
#include "gdlib/gd.h"

#ifndef PHOTOSTORM_WEBSITE
#include "ps_system.h"
#include "mail.h"
#include "lang/translate.h"
#include "plugins.h"
#include "messages.h"
#endif


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#ifndef PHOTOSTORM_WEBSITE
#include "design.h"
#include "ps_dialogs.h"
#endif

char * ps_load_picture (const char *poza, long &n)
{
	char * result=NULL;
	FILE *f;
	f=fopen (poza, "rb");
	if (f!=NULL)
	{
		fseek (f, 0, SEEK_END);
		n=ftell (f);
		fseek (f, 0, SEEK_SET);
		if (n>-1)
		{
			result = (char*)malloc (n);
			if (fread (result, n, 1, f)!=1) result=NULL;
		}
		fclose (f);
	}
	return result;
}

char * ps_create_photo_data (char * poza, long n, int &rn)
{
	char * result=NULL;
	rn=0;
	gdImagePtr img = NULL;
	img = gdImageCreateFromJpegPtr (n, poza);
	if (img==NULL) img = gdImageCreateFromPngPtr (n, poza);
	if (img!=NULL)
	{
		result = (char*)gdImageJpegPtr (img, &rn, 100);
		gdImageDestroy (img);
	}
	return result;
}

char * ps_create_photo_preview_data_resized (char * poza, long n, int r, int t, int &rn, int b)
{
	char * result=NULL;
	rn=0;
	gdImagePtr img = NULL;
	gdImagePtr img_preview = NULL;
	img = gdImageCreateFromJpegPtr (n, poza);
	if (img==NULL) img = gdImageCreateFromPngPtr (n, poza);
	if (img!=NULL)
	{
		int dx=gdImageSX (img);
		int dy=gdImageSY (img);
		int rx, tx;
		if (r>0) rx=r; else rx = (int)(dx*(double)t/dy);
		if (t>0) tx=t; else tx = (int)(dy*(double)r/dx);
		img_preview = gdImageCreateTrueColor (rx, tx);
		if (rx>0 && tx>0)
		{
			gdImageCopyResized (img_preview, img, 0, 0, 0, 0, rx, tx, dx, dy);
			if (b)
			{
				int alb=gdImageColorAllocate (img_preview, 255, 255, 255);
				int gri=gdImageColorAllocate (img_preview, 1, 1, 1);
				gdImageRectangle (img_preview, 0, 0, rx-1, tx-1, gri);
				gdImageFilledRectangle (img_preview, 1, 1, rx-2, 2, alb);
				gdImageFilledRectangle (img_preview, rx-3, 2, rx-2, tx-2, alb);
				gdImageFilledRectangle (img_preview, 1, tx-3, rx-3, tx-2, alb);
				gdImageFilledRectangle (img_preview, 1, 2, 2, tx-3, alb);
			}
		}
		result = (char*)gdImageJpegPtr (img_preview, &rn, 90);
		gdImageDestroy (img_preview);
		gdImageDestroy (img);
	}
	return result;
}

char * ps_create_photo_preview_data (char * poza, long n, int r, int t, int &rn, int b)
{
	char * result=NULL;
	rn=0;
	gdImagePtr img = NULL;
	gdImagePtr img_preview = NULL;
	img = gdImageCreateFromJpegPtr (n, poza);
	if (img==NULL) img = gdImageCreateFromPngPtr (n, poza);
	if (img!=NULL)
	{
		int dx=gdImageSX (img);
		int dy=gdImageSY (img);
		int rx, tx;
		if (r>0) rx=r; else rx = (int)(dx*(double)t/dy);
		if (t>0) tx=t; else tx = (int)(dy*(double)r/dx);
		img_preview = gdImageCreateTrueColor (rx, tx);
		if (rx>0 && tx>0)
		{
			gdImageCopyResampled (img_preview, img, 0, 0, 0, 0, rx, tx, dx, dy);
			if (b)
			{
				int alb=gdImageColorAllocate (img_preview, 255, 255, 255);
				int gri=gdImageColorAllocate (img_preview, 1, 1, 1);
				gdImageRectangle (img_preview, 0, 0, rx-1, tx-1, gri);
				gdImageFilledRectangle (img_preview, 1, 1, rx-2, 2, alb);
				gdImageFilledRectangle (img_preview, rx-3, 2, rx-2, tx-2, alb);
				gdImageFilledRectangle (img_preview, 1, tx-3, rx-3, tx-2, alb);
				gdImageFilledRectangle (img_preview, 1, 2, 2, tx-3, alb);
			}
		}
		result = (char*)gdImageJpegPtr (img_preview, &rn, 60);
		gdImageDestroy (img_preview);
		gdImageDestroy (img);
	}
	return result;
}

char * ps_create_photo_export_data (char * poza, long n, int &rn)
{
	char * result=NULL;
	rn=0;
	gdImagePtr img = NULL;
	//gdImagePtr img_preview = NULL;
	img = gdImageCreateFromJpegPtr (n, poza);
	if (img==NULL) img = gdImageCreateFromPngPtr (n, poza);
	if (img!=NULL)
	{
		int dx=gdImageSX (img);
		int dy=gdImageSY (img);
		//gdImageCopyResampled (img_preview, img, 0, 0, 0, 0, dx-10, dy-10, dx, dy);
		int color = gdImageColorAllocate (img, 255, 255, 255);
		int ucolor = gdImageColorAllocateAlpha (img, 9, 9, 9, 80);
		int spatiu_text[8];
		FILE *f = fopen (pict_path ("logos/photostorm2.png"), "rb");
		if (f!=NULL)
		{
			gdImagePtr img2 = gdImageCreateFromPng (f);
			fclose (f);
			if (img2!=NULL)
			{
				int dx2=gdImageSX (img2);
				int dy2=gdImageSY (img2);
				gdImageCopy (img, img2, dx-240, dy-90, 0, 0, dx2, dy2);
				gdImageDestroy (img2);
			}
		}
		gdImageStringFT (img, &spatiu_text[0], ucolor, font_path ("tahoma"), 15, 0.0, dx-220+2, dy-35+2, "www.photostorm.net");
		gdImageStringFT (img, &spatiu_text[0], color, font_path ("tahoma"), 15, 0.0, dx-220, dy-35, "www.photostorm.net");
		result = (char*)gdImageJpegPtr (img, &rn, 100);
		//gdImageDestroy (img_preview);
		gdImageDestroy (img);
	}
	return result;
}

_ps_photo * ps_create_photo_from_memory (const char *poza, char *date_poza, long n)
{
	_ps_photo *result = NULL;
	unsigned char n_byte;
	int e=0;
	char *date_poza_preview=NULL;
	if (date_poza!=NULL && poza!=NULL)
	{
		result = (_ps_photo*)malloc (sizeof(_ps_photo));
		result->date_preview=NULL;
		result->date=NULL;
		char *nume;
		nume = strrchr (poza,'/');
		if (nume==NULL) nume=strrchr (poza,'\\');
		if (nume!=NULL)
		{
		    if (strlen(nume)>1) nume++;
		}
		if (nume==NULL) nume=strdup (poza);
		if (strlen(nume)>255) nume[255]='\0';
		n_byte=strlen (nume);
		result->nume = (char*)malloc (n_byte+1);
		strcpy (result->nume, nume);
		result->autor=NULL;
		result->descriere=NULL;
		result->optiuni=NULL;
		result->metadata=NULL;
		date_poza_preview = ps_create_photo_preview (date_poza, n, 0, 50, result->n_poza_preview);
		result->date_preview = (char*)malloc (result->n_poza_preview);
		if (result->date_preview!=NULL && result->n_poza_preview>0)
		{
			memcpy (&result->date_preview[0], date_poza_preview, result->n_poza_preview);
		}
		else e=1;
		//result->date = (char*)malloc (n);
		//int n_temp_date;
		//char *temp_date = ps_create_photo_data (date_poza, n, n_temp_date);
		//char *temp_date = date_poza;
		//if (e==0 && result->date!=NULL)
		//printf ("n_temp_date: %u\n", n);
		result->date = (char*)malloc (n);
		if (e==0 && result->date!=NULL)
		{
			memcpy (&result->date[0], date_poza, n);
			result->n_poza=n;
			//free (temp_date);
		}
		else e=1;
		if (date_poza_preview!=NULL) free (date_poza_preview);
	}
	if (e==1) if (result!=NULL) ps_photo_free (result);
	if (e!=0) result=NULL;
	return result;
}

_ps_photo * ps_create_photo (const char *poza)
{
	_ps_photo *result=NULL;
	long n;
	unsigned char n_byte;
	int e=0;
	char *date_poza=NULL;
	char *date_poza_preview=NULL;
	date_poza = ps_load_picture (poza, n);
	if (date_poza!=NULL && poza!=NULL)
	{
		result = (_ps_photo*)malloc (sizeof(_ps_photo));
		result->date_preview=NULL;
		result->date=NULL;
		char *nume;
		nume = strrchr (poza,'/');
		if (nume==NULL) nume=strrchr (poza,'\\');
		if (nume!=NULL)
		{
		    if (strlen(nume)>1) nume++;
		}
		if (nume==NULL) nume=strdup (poza);
		if (strlen(nume)>255) nume[255]='\0';
		n_byte=strlen (nume);
		result->nume = (char*)malloc (n_byte+1);
		strcpy (result->nume, nume);
		result->autor=strdup ("");
		result->descriere=strdup ("");
		result->optiuni=strdup ("");
		result->metadata=strdup ("");
		date_poza_preview = ps_create_photo_preview (date_poza, n, 0, 50, result->n_poza_preview);
		result->date_preview = (char*)malloc (result->n_poza_preview);
		if (result->date_preview!=NULL && result->n_poza_preview>0)
		{
			memcpy (&result->date_preview[0], date_poza_preview, result->n_poza_preview);
		}
		else e=1;
		//result->date = (char*)malloc (n);
		int n_temp_date;
		//char *temp_date = ps_create_photo_data (date_poza, n, n_temp_date);
		//if (e==0 && result->date!=NULL)
		//printf ("n_temp_date: %u\n", n_temp_date);
		result->date = (char*)malloc (n);
		if (e==0 && date_poza!=NULL)
		{
			memcpy (&result->date[0], date_poza, n);
			result->n_poza=n;
			//free (temp_date);
		}
		else e=1;
		if (date_poza_preview!=NULL) free (date_poza_preview);
		if (date_poza!=NULL) free (date_poza);
	}
	if (e==1) if (result!=NULL) ps_photo_free (result);
	if (e!=0) result=NULL;
	return result;
}

_ps_photo * ps_create_photo_with_data (char *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata)
{
	_ps_photo *result=NULL;
	long n;
	unsigned char n_nume;
	unsigned char n_autor;
	unsigned short n_descriere;
	unsigned char n_optiuni;
	unsigned short n_metadata;
	int e=0;
	char *date_poza=NULL;
	char *date_poza_preview=NULL;
	date_poza = ps_load_picture (poza, n);
	if (date_poza!=NULL && poza!=NULL)
	{
		result = (_ps_photo*)malloc (sizeof(_ps_photo));
		result->date_preview=NULL;
		result->date=NULL;
		// verificarea datelor
		if (nume==NULL) nume = strdup ("");
		if (autor==NULL) autor = strdup ("");
		if (descriere==NULL) descriere = strdup ("");
		if (optiuni==NULL) optiuni = strdup ("");
		if (metadata==NULL) metadata = strdup ("");
		// verificarea dimensiunilor
		if (strlen(nume)>255) nume[255]='\0';
		if (strlen(autor)>255) autor[255]='\0';
		if (strlen(descriere)>32000) descriere[32000]='\0';
		if (strlen(optiuni)>255) optiuni[255]='\0';
		if (strlen(metadata)>32000) metadata[32000]='\0';			
		// calcularea dimensiunilor
		n_nume=strlen (nume);
		n_autor=strlen (autor);
		n_descriere=strlen (descriere);
		n_optiuni=strlen (optiuni);
		n_metadata=strlen (metadata);
		// scrierea datelor
		result->nume = (char*)malloc (n_nume+1);
		strcpy (result->nume, nume);
		result->autor = (char*)malloc (n_autor+1);
		strcpy (result->autor, autor);
		result->descriere = (char*)malloc (n_descriere+1);
		strcpy (result->descriere, descriere);
		result->optiuni = (char*)malloc (n_optiuni+1);
		strcpy (result->optiuni, optiuni);
		result->metadata = (char*)malloc (n_metadata+1);
		strcpy (result->metadata, metadata);
		date_poza_preview = ps_create_photo_preview (date_poza, n, 0, 50, result->n_poza_preview);
		if (result->n_poza_preview>0) result->date_preview = (char*)malloc (result->n_poza_preview);
		if (result->date_preview!=NULL)
		{
			memcpy (&result->date_preview[0], date_poza_preview, result->n_poza_preview);
		}
		else e=1;
		result->date = (char*)malloc (n);
		if (e==0 && result->date!=NULL)
		{
			memcpy (&result->date[0], date_poza, n);
			result->n_poza=n;
		}
		else e=1;
		if (date_poza_preview!=NULL) free (date_poza_preview);
		if (date_poza!=NULL) free (date_poza);
	}
	if (e==1) if (result!=NULL) ps_photo_free (result);
	if (e!=0) result=NULL;
	return result;
}

void ps_modify_photo (_ps_photo *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata)
{
	//long n;
	unsigned char n_nume;
	unsigned char n_autor;
	unsigned short n_descriere;
	unsigned char n_optiuni;
	unsigned short n_metadata;
	/*int e=0;
	char *date_poza=NULL;
	char *date_poza_preview=NULL;*/
	if (poza!=NULL)
	{
		// verificarea datelor
		if (nume==NULL) nume = strdup ("");
		if (autor==NULL) autor = strdup ("");
		if (descriere==NULL) descriere = strdup ("");
		if (optiuni==NULL) optiuni = strdup ("");
		if (metadata==NULL) metadata = strdup ("");
		// verificarea dimensiunilor
		if (strlen(nume)>255) nume[255]='\0';
		if (strlen(autor)>255) autor[255]='\0';
		if (strlen(descriere)>32000) descriere[32000]='\0';
		if (strlen(optiuni)>255) optiuni[255]='\0';
		if (strlen(metadata)>32000) metadata[32000]='\0';			
		// calcularea dimensiunilor
		n_nume=strlen (nume);
		n_autor=strlen (autor);
		n_descriere=strlen (descriere);
		n_optiuni=strlen (optiuni);
		n_metadata=strlen (metadata);
		// scrierea datelor
		poza->nume = (char*)malloc (n_nume+1);
		strcpy (poza->nume, nume);
		poza->autor = (char*)malloc (n_autor+1);
		strcpy (poza->autor, autor);
		poza->descriere = (char*)malloc (n_descriere+1);
		strcpy (poza->descriere, descriere);
		poza->optiuni = (char*)malloc (n_optiuni+1);
		strcpy (poza->optiuni, optiuni);
		poza->metadata = (char*)malloc (n_metadata+1);
		strcpy (poza->metadata, metadata);
		/*result->date_preview = (char*)malloc (poza->n_poza_preview);
		if (result->date_preview!=NULL)
		{
			memcpy (&result->date_preview[0], date_poza_preview, result->n_poza_preview);
		}
		else e=1;
		result->date = (char*)malloc (n);
		if (result->date!=NULL)
		{
			memcpy (&result->date[0], date_poza, n);
			result->n_poza=n;
		}
		else e=1;
		if (date_poza_preview!=NULL) free (date_poza_preview);
		if (date_poza!=NULL) free (date_poza);*/
	}
	/*if (e==1) if (result!=NULL) ps_photo_free (result);
	return result;*/
}

char *ps_pack_photo (_ps_photo *poza, long &n)
{
	char * result=NULL;
	if (poza!=NULL)
	{
		unsigned char n_nume;
		unsigned char n_autor;
		unsigned short n_descriere;
		unsigned char n_optiuni;
		unsigned short n_metadata;
		if (poza->nume==NULL) 
		{
			poza->nume = (char*)malloc (1);
			poza->nume[0]='\0';
		}
		if (poza->autor==NULL) 
		{
			poza->autor = (char*)malloc (1);
			poza->autor[0]='\0';
		}
		if (poza->descriere==NULL) 
		{
			poza->descriere = (char*)malloc (1);
			poza->descriere[0]='\0';
		}
		if (poza->optiuni==NULL) 
		{
			poza->optiuni = (char*)malloc (1);
			poza->optiuni[0]='\0';
		}
		if (poza->metadata==NULL) 
		{
			poza->metadata = (char*)malloc (1);
			poza->metadata[0]='\0';
		}
		n = 4 + strlen(poza->nume) + 1 + strlen(poza->autor) + 1 + strlen(poza->descriere) + 2 + strlen(poza->optiuni) + 1 + strlen(poza->metadata) + 2 + poza->n_poza_preview + 4 + poza->n_poza + 4;
		result = (char*)malloc (n);
		long pozitie=0;
		// lungimea totala
		memcpy (&result[pozitie],&n,4);
		pozitie=pozitie+4;
		// lungimea numelui
		n_nume = strlen(poza->nume);
		memcpy (&result[pozitie],&n_nume,1);
		pozitie=pozitie+1;
		// numele
		if (n_nume>0) memcpy (&result[pozitie],&poza->nume[0],n_nume);
		pozitie=pozitie+n_nume;
		// lungimea autorului
		n_autor = strlen(poza->autor);
		memcpy (&result[pozitie],&n_autor,1);
		pozitie=pozitie+1;
		// autorul
		if (n_autor) memcpy (&result[pozitie],&poza->autor[0],n_autor);
		pozitie=pozitie+n_autor;
		// lungimea descrierii
		n_descriere = strlen(poza->descriere);
		memcpy (&result[pozitie],&n_descriere,2);
		pozitie=pozitie+2;
		// descrierea
		if (n_descriere>0) memcpy (&result[pozitie],&poza->descriere[0],n_descriere);
		pozitie=pozitie+n_descriere;
		// lungimea optiunilor
		n_optiuni = strlen(poza->optiuni);
		memcpy (&result[pozitie],&n_optiuni,1);
		pozitie=pozitie+1;
		// optiunile
		if (n_optiuni) memcpy (&result[pozitie],&poza->optiuni[0],n_optiuni);
		pozitie=pozitie+n_optiuni;
		// lungimea metadata
		n_metadata = strlen(poza->metadata);
		memcpy (&result[pozitie],&n_metadata,2);
		pozitie=pozitie+2;
		// metadata
		if (n_metadata>0) memcpy (&result[pozitie],&poza->metadata[0],n_metadata);
		pozitie=pozitie+n_metadata;
		// lungimea datelor preview-ului
		memcpy (&result[pozitie],&poza->n_poza_preview,4);
		pozitie=pozitie+4;
		// preview-ul
		if (poza->n_poza_preview>0) memcpy (&result[pozitie],&poza->date_preview[0],poza->n_poza_preview);
		pozitie=pozitie+poza->n_poza_preview;
		// lungimea datelor pozei
		memcpy (&result[pozitie],&poza->n_poza,4);
		pozitie=pozitie+4;
		// datele pozei
		if (poza->n_poza>0) memcpy (&result[pozitie],&poza->date[0],poza->n_poza);
		pozitie=pozitie+poza->n_poza;
		//printf ("%u / %u\n", pozitie,n);
	}
	return result;
}

#ifdef PHOTOSTORM_WEBSITE

char * ps_create_photo_wallpaper (_ps_photo *poza, int &nr, _ps_config *config)
{
	char *data = NULL;
	nr=0;
	if (config==NULL) return data;
	int dx=config->system.scr_width, dy=config->system.scr_height;	// ASTEA VIN MUTATE DE AICI
	#ifdef WIN32
	if (config->system.scr_size_autodetect)
	{
	     LPDEVMODE lpDevMode = (LPDEVMODE)malloc (sizeof(DEVMODE));
         lpDevMode->dmSize=sizeof (DEVMODE);
	     lpDevMode->dmDriverExtra = 0;
		 if (EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, lpDevMode))
	     {
	     	   config->system.scr_width = lpDevMode->dmPelsWidth;
     		   config->system.scr_height = lpDevMode->dmPelsHeight;
         }
    }
	#endif
	if (dx<1) dx=10;
	if (dy<1) dy=10;
	int result = 0;
	if (poza!=NULL)
	{
		char *date=poza->date;
		long n=poza->n_poza;
		if (date!=NULL && n>0)
		{
			gdImage *img, *img_poza;
			img_poza = gdImageCreateFromJpegPtr (n, date);
			if (img_poza==NULL) img_poza = gdImageCreateFromPngPtr (n, date);
			if (img_poza!=NULL)
			{				
				int dxp = gdImageSX (img_poza);
				int dyp = gdImageSY (img_poza);
				int dx2=dxp;
				int dy2=dyp;
				// redimensionare
				int optiuni = 0;
				if (strlen (poza->optiuni)>0 && poza->optiuni[0]-'A'>=0 && poza->optiuni[0]-'A'<=8) optiuni = poza->optiuni[0]-'A';
				if (optiuni == 0) optiuni = config->wallpaper.dtype;
					else optiuni--;
				switch (optiuni)
				{
					case 0:
					if ((dxp) > (dyp)) 
					{
						dx2 = dx;						
						dy2 = (int)(dyp*(double)dx/dxp);
					}
					else 
					{
						dx2 = (int)(dxp*(double)dy/dyp)+1;
						dy2 = dy;
					}
						break;
					case 1:
						if (dxp>dx || dyp>dy)
						{
							if ((dxp/dx) < (dyp/dy)) 
							{
								dx2 = dx;
								dy2 = (int)(dyp*(double)dx/dxp);
							}
							else 
							{	
								dx2 = (int)(dxp*(double)dy/dyp)+1;
								dy2 = dy;
							}
						}
						break;
					case 2:
						if (dxp<dx || dyp<dy)
						{
							if ((dxp/dx) < (dyp/dy)) 
							{
								dx2 = dx;
								dy2 = (int)(dyp*(double)dx/dxp);
							}
							else 
							{
								dx2 = (int)(dxp*(double)dy/dyp)+1;
								dy2 = dy;
							}
						}
						break;
					case 3:
						dx2 = dx;
						dy2 = dy;
						break;
					case 5:
						dx2 = dx;
						dy2 = dy;
						break;
				}
				img = gdImageCreateTrueColor (dx2, dy2);
				if (optiuni!=5) gdImageCopyResampled (img, img_poza, 0, 0, 0, 0, dx2, dy2, dxp, dyp);
					else 
					{
						gdImageSetTile (img, img_poza);
						gdImageFill (img, 0, 0, gdTiled);
					}
				gdImageDestroy (img_poza);
				dx = dx2;
				dy = dy2;
				if (config->wallpaper.dinfo)
				{
					ps_write_information (img, poza, config->wallpaper.format, config->wallpaper.pos.x, config->wallpaper.pos.y, config->wallpaper.fsize, dx, dy, &config->wallpaper.fcolor, config->wallpaper.border, config->wallpaper.pos.a); 
				}
				// calendar
				_ps_color *color = ps_pack_color (255, 255, 255, 0);
				_ps_color *marcaj = ps_pack_color (255, 255, 0, 0);
				_ps_color *marcaj_mesaj = ps_pack_color (255, 0, 0, 0);
				if (config->calendar.disp)
				{
					int big_text = dx/100;
					int small_text = dx/150;
					if (config->calendar.cmfsize>0) big_text = config->calendar.cmfsize;
					if (config->calendar.omfsize>0) small_text = config->calendar.omfsize;
					int prev=0;
					int next=0;
					if (config->calendar.p_month) prev=1;
					if (config->calendar.n_month) next=1;
					int cx = dx - big_text*20 - (prev+next)*small_text*20;
					int cy = 40;
					if (!config->calendar.pos.a)
					{
						cx = config->calendar.pos.x;
						cy = config->calendar.pos.y;
					}
					const int big_cx = cx+(prev)*small_text*21;
					for (int i=0;i<config->calendar.p_month;i++)
						ps_write_calendar (img, small_text, cx, cy+small_text+i*small_text*17, -config->calendar.p_month+i, 0, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
					for (int j=0;j<config->calendar.n_month;j++)
						ps_write_calendar (img, small_text, cx+(prev*small_text)*20+big_text*17+small_text*5, cy+small_text+j*small_text*17, (j+1), 0, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
					ps_write_calendar (img, big_text, big_cx, cy+big_text, 0, config->calendar.mark_date, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
				}
				// citate (quotes)
				if (config->quotes.disp)
				{
					_ps_quote *q = NULL;
					//if (config->quotes.inet) q = ps_get_quote_online (config->quotes.server, config->quotes.port, config->quotes.keywords, !config->quotes.only_keywords);
					q = ps_get_quote (psw_quotes_data, config->quotes.keywords, !config->quotes.only_keywords);
					if (q!=NULL)
					{
						ps_write_quote (img, q, config->quotes.pos.x, config->quotes.pos.y, config->quotes.fsize, dx, dy, &config->quotes.fcolor, config->quotes.border, config->quotes.pos.a);
						ps_quote_free (q);
					}
				}
				
				free (color);
				free (marcaj);
				free (marcaj_mesaj);
				//ps_write_to_image (img, 200, 100, font_path ("arial"), 10, color, 0.0, "<b>dsadsa</b>");
				//ps_write_to_image (img, 150, dy-200, poza->nume);
				data = (char*)gdImageJpegPtr (img, &nr, 90);
				gdImageDestroy (img);
				result = 1;
			}
		}
	}
	return data;
}

#endif

#ifndef PHOTOSTORM_WEBSITE
char * ps_creaza_poza_pentru_afisare (_ps_photo *date_poza, int r, int t, int &n)
{
	char *result=NULL;
	if (date_poza!=NULL)
	{
		gdImagePtr img;
		gdImagePtr img_afis;
		img=gdImageCreateFromJpegPtr (date_poza->n_poza, date_poza->date);
		if (img!=NULL)
		{
			int dx=gdImageSX (img);
			int dy=gdImageSY (img);
			int rx, ty;
			if (r>0) rx=r; else rx = (int)(dx*(double)t/dy);
			if (t>0) ty=t; else ty = (int)(dy*(double)r/dx);
			img_afis = gdImageCreateTrueColor (rx, ty);
			gdImageCopyResampled (img_afis, img, 0, 0, 0, 0, rx, ty, dx, dy);
			int brect[8];
			int alb=gdImageColorAllocate (img_afis, 255, 255, 255);
			gdImageStringFT (NULL, &brect[0], alb, "/mnt/windows/windows/fonts/arial.ttf", 10, 0.0, 0, 0, date_poza->descriere);
			for (int i=0;i<8;i++) printf ("%d ",brect[i]);
			printf ("\n");
			gdImageStringFT (img_afis, &brect[0], alb, "/mnt/windows/windows/fonts/arial.ttf", 10, 0.0, rx-brect[2]+brect[6]-10, ty-brect[3]+brect[7]-10, date_poza->descriere);
			result = (char*)gdImageJpegPtr (img_afis, &n, 90);
			gdImageDestroy (img);
			gdImageDestroy (img_afis);
		}
	}
	return result;
}
#endif

void ps_photo_free (_ps_photo *date_poza)
{
	if (date_poza!=NULL)
	{
		if (date_poza->nume!=NULL) free (date_poza->nume);
		if (date_poza->autor!=NULL) free (date_poza->autor);
		if (date_poza->descriere!=NULL) free (date_poza->descriere);
		if (date_poza->optiuni!=NULL) free (date_poza->optiuni);
		if (date_poza->metadata!=NULL) free (date_poza->metadata);
		if (date_poza->date!=NULL) free (date_poza->date);
		if (date_poza->date_preview!=NULL) free (date_poza->date_preview);
		free (date_poza);
	}
}

void ps_write_to_image (gdImage *img, int x, int y, char *font, int size, _ps_color color, float direction, const char *text)
{
	if (text!=NULL)
	{
		int brect[8];
		int culoare=gdImageColorAllocateAlpha (img, color.red, color.green,color.blue, color.alpha);
		gdImageStringFT (img, &brect[0], culoare, font, size, direction, x, y, (char*)text);
	}
}

#ifndef PHOTOSTORM_WEBSITE

void ps_run_plugins (int plugin_run, _ps_photo *poza)
{
	GList *ps_plugins_list = get_ps_plugins_list ();
	int nr = g_list_length (ps_plugins_list);
	//printf ("nr: %d\n",nr);
	int gasit = 0;
	for (int j=0;j<nr && !gasit;j++)
	{
		_ps_plugin *plugin = (_ps_plugin*)g_list_nth_data (ps_plugins_list, j);
		if (plugin!=NULL && plugin->filename!=NULL && plugin->enabled==1 && plugin->process_image!=NULL && plugin->run == plugin_run)
		{
			plugin->process_image (poza);
		}
	}
}

void * ps_thread_photo_wallpaper (void *data)
{
	//pthread_yield();
	_ps_config *config = ps_get_config ();
	_ps_progress *progress = (_ps_progress*)data;
	if (progress == NULL) g_thread_exit (0);
	#ifdef WIN32
	if (config->system.scr_size_autodetect)
	{
	     LPDEVMODE lpDevMode = (LPDEVMODE)malloc (sizeof(DEVMODE));
         lpDevMode->dmSize=sizeof (DEVMODE);
	     lpDevMode->dmDriverExtra = 0;
		 if (EnumDisplaySettings (NULL, ENUM_CURRENT_SETTINGS, lpDevMode))
	     {
	     	   config->system.scr_width = lpDevMode->dmPelsWidth;
     		   config->system.scr_height = lpDevMode->dmPelsHeight;
         }
    }
	#endif
	int dx=config->system.scr_width, dy=config->system.scr_height;	// ASTEA VIN MUTATE DE AICI
	if (dx<1) dx=10;
	if (dy<1) dy=10;
	int result = 0;
	_ps_photo *poza = (_ps_photo*)progress->data;
	//return NULL;
	int r=0;
	if (poza!=NULL)
	{
		_ps_album *album = ps_create_new_album_in_memory ("Current Wallpaper Album", "PhotoStorm", "Photostorm Data", "", "psa_cwd", "photostorm_current_wallpaper"); 
		if (album!=NULL)
		{
			char t[1000];
			sprintf (t, "%s%c.photostorm%cphotostorm.cwa", ps_get_user_dir(), ps_dir_sep, ps_dir_sep);
			//printf ("t: %s\n", t);
			r = ps_add_photo_to_album_from_memory_into_memory (poza, album);
			if (r) r=ps_write_album_from_memory (t, album);
			ps_album_free (album);
		}
	}
	if (poza!=NULL && r!=0)
	{
		ps_progress_set_progress (progress, 10);
		ps_progress_set_text (progress, __("Loading photo"));
		ps_progress_set_progress (progress, 14);
		ps_progress_set_text (progress, __("Running Plugins (before filters)"));
		ps_run_plugins (PS_PLUGIN_RUN_BEFORE_FILTERS, poza);
		char *date=poza->date;
		long n=poza->n_poza;
		if (date!=NULL && n>0)
		{
			ps_progress_set_progress (progress, 20);
			ps_progress_set_text (progress, __("Resampling photo"));
			gdImage *img, *img_poza;
			img_poza = gdImageCreateFromJpegPtr (n, date);
			if (img_poza==NULL) img_poza = gdImageCreateFromPngPtr (n, date);
			if (img_poza!=NULL)
			{				
				int dxp = gdImageSX (img_poza);
				int dyp = gdImageSY (img_poza);
				int dx2=dxp;
				int dy2=dyp;
				// redimensionare
				int optiuni = 0;
				if (strlen (poza->optiuni)>0 && poza->optiuni[0]-'A'>=0 && poza->optiuni[0]-'A'<=8) optiuni = poza->optiuni[0]-'A';
				if (optiuni == 0) optiuni = config->wallpaper.dtype;
					else optiuni--;
				switch (optiuni)
				{
					case 0:
					if ((dxp) > (dyp)) 
					{
						dx2 = dx;						
						dy2 = (int)(dyp*(double)dx/dxp);
					}
					else 
					{
						dx2 = (int)(dxp*(double)dy/dyp)+1;
						dy2 = dy;
					}
						break;
					case 1:
						if (dxp>dx || dyp>dy)
						{
							if ((dxp/dx) < (dyp/dy)) 
							{
								dx2 = dx;
								dy2 = (int)(dyp*(double)dx/dxp);
							}
							else 
							{	
								dx2 = (int)(dxp*(double)dy/dyp)+1;
								dy2 = dy;
							}
						}
						break;
					case 2:
						if (dxp<dx || dyp<dy)
						{
							if ((dxp/dx) < (dyp/dy)) 
							{
								dx2 = dx;
								dy2 = (int)(dyp*(double)dx/dxp);
							}
							else 
							{
								dx2 = (int)(dxp*(double)dy/dyp)+1;
								dy2 = dy;
							}
						}
						break;
					case 3:
						dx2 = dx;
						dy2 = dy;
						break;
					case 5:
						dx2 = dx;
						dy2 = dy;
						break;
				}
				if (dx2!=dxp || dy2!=dyp)
				{
					img = gdImageCreateTrueColor (dx2, dy2);
					if (optiuni!=5) gdImageCopyResampled (img, img_poza, 0, 0, 0, 0, dx2, dy2, dxp, dyp);
						else 
						{
							gdImageSetTile (img, img_poza);
							gdImageFill (img, 0, 0, gdTiled);
						}
					gdImageDestroy (img_poza);
				}
				else img = img_poza;
				dx = dx2;
				dy = dy2;
				if (config->wallpaper.dinfo)
				{
					ps_progress_set_progress (progress, 60);
					ps_progress_set_text (progress, __("Applying filters (Photo's Information)"));
					ps_write_information (img, poza, config->wallpaper.format, config->wallpaper.pos.x, config->wallpaper.pos.y, config->wallpaper.fsize, dx, dy, &config->wallpaper.fcolor, config->wallpaper.border, config->wallpaper.pos.a); 
				}
				// calendar
				_ps_color *color = ps_pack_color (255, 255, 255, 0);
				_ps_color *marcaj = ps_pack_color (255, 255, 0, 0);
				_ps_color *marcaj_mesaj = ps_pack_color (255, 0, 0, 0);
				if (config->calendar.disp)
				{
					ps_progress_set_progress (progress, 65);
					ps_progress_set_text (progress, __("Applying filters (Calendar)"));
					int big_text = dx/100;
					int small_text = dx/150;
					if (config->calendar.cmfsize>0) big_text = config->calendar.cmfsize;
					if (config->calendar.omfsize>0) small_text = config->calendar.omfsize;
					int prev=0;
					int next=0;
					if (config->calendar.p_month) prev=1;
					if (config->calendar.n_month) next=1;
					int cx = dx - big_text*20 - (prev+next)*small_text*20;
					int cy = 40;
					if (!config->calendar.pos.a)
					{
						cx = config->calendar.pos.x;
						cy = config->calendar.pos.y;
					}
					const int big_cx = cx+(prev)*small_text*21;
					for (int i=0;i<config->calendar.p_month;i++)
						ps_write_calendar (img, small_text, cx, cy+small_text+i*small_text*17, -config->calendar.p_month+i, 0, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
					for (int j=0;j<config->calendar.n_month;j++)
						ps_write_calendar (img, small_text, cx+(prev*small_text)*20+big_text*17+small_text*5, cy+small_text+j*small_text*17, (j+1), 0, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
					ps_write_calendar (img, big_text, big_cx, cy+big_text, 0, config->calendar.mark_date, config->calendar.fcolor, config->calendar.mcolor, config->calendar.border);
				}
				// citate (quotes)
				if (config->quotes.disp)
				{
					ps_progress_set_progress (progress, 70);
					ps_progress_set_text (progress, __("Applying filters (Quotes)"));
					_ps_quote *q = NULL;
					if (config->quotes.inet) q = ps_get_quote_online (config->quotes.server, config->quotes.port, config->quotes.keywords, !config->quotes.only_keywords);
					if (q==NULL) q = ps_get_quote (quotes_path ("quotes1"), config->quotes.keywords, !config->quotes.only_keywords);
					if (q!=NULL)
					{
						ps_write_quote (img, q, config->quotes.pos.x, config->quotes.pos.y, config->quotes.fsize, dx, dy, &config->quotes.fcolor, config->quotes.border, config->quotes.pos.a);
						ps_quote_free (q);
					}
				}
				// mesaje (messages)
				if (config->messages.disp)
				{
					ps_progress_set_progress (progress, 75);
					ps_progress_set_text (progress, __("Applying filters (Messages)"));
					ps_write_messages (img, get_messages_list(), config->messages.pos.x, config->messages.pos.y, config->messages.fsize, dx, dy, &config->messages.tcolor, &config->messages.mcolor, config->messages.border, config->messages.pos.a); 
				}
				// e-mail-uri
				if (config->email.disp)
				{
					ps_progress_set_progress (progress, 80);
					ps_progress_set_text (progress, __("Applying filters (e-mails)"));
					_ps_emails *messages = ps_get_mail_list (config->email.server, config->email.port, config->email.username, config->email.password, config->email.max_nr);
					if (messages!=NULL)
					{
						/*for (int i=0;i<messages->count;i++)
							ps_write_message (img, 200, (int)(450+i*(12*3/2)), 12, font_path ("arial"), 
											  messages->email[i]->from, messages->email[i]->subject,
											  *marcaj, *color);*/
						ps_write_email (img, messages, config->email.pos.x, config->email.pos.y, config->email.fsize, dx, dy, &config->email.fcolor, &config->email.scolor, config->email.border, config->email.pos.a); 
					}
					ps_emails_free (messages);
				}
				
				free (color);
				free (marcaj);
				free (marcaj_mesaj);
				//ps_write_to_image (img, 200, 100, font_path ("arial"), 10, color, 0.0, "<b>dsadsa</b>");
				//ps_write_to_image (img, 150, dy-200, poza->nume);
				ps_progress_set_progress (progress, 85);
				ps_progress_set_text (progress, __("Running Plugins (after filters)"));
				int n = 0;
	    		char * date = (char*)gdImageJpegPtr (img, &n, 100);
	    		if (date!=NULL && n>0)
	    		{
					if (poza->date!=NULL) free (poza->date);
					poza->date = date;
					poza->n_poza = n;
	    		}
	    		//gdImageDestroy (img);
				ps_run_plugins (PS_PLUGIN_RUN_AFTER_FILTERS, poza);
				ps_progress_set_progress (progress, 90);
				ps_progress_set_text (progress, __("Writing Image"));
				FILE *f;
				f=fopen (config_path ("photostorm_wallpaper.png"), "wb");
				//fwrite (poza->date, poza->n_poza, 1, f);
				gdImagePngEx (img, f, 0);
				fclose (f);
				gdImageDestroy (img);
				result = 1;
			}
		}
		ps_photo_free (poza);
	}
	if (result==1) 
	{	
		ps_progress_set_progress (progress, 95);
		ps_progress_set_text (progress, __("Setting wallpaper"));
		ps_set_wallpaper (config_path ("photostorm_wallpaper.png"));
		ps_progress_set_progress (progress, 100);
		ps_progress_set_text (progress, __("Wallpaper was set"));
	}
	//return result;
	
	//while (1==1) { printf ("Helo\n"); }
	ps_progress_dialog_close (progress);
	g_thread_exit (NULL);
	return (0);
}

int ps_photo_wallpaper (_ps_photo *poza)
{
	if (poza!=NULL)
	{
		_ps_progress *progress = ps_progress_dialog (ps_get_main_window(), poza);
		ps_progress_set_progress (progress, 0);
		ps_progress_set_text (progress, __("Searching Photo"));
		progress->quit = 0;
		GThread *firul;
		firul = g_thread_create (ps_thread_photo_wallpaper, progress, TRUE, NULL);
	} else return 0;
	return 1;
}

int ps_wallpaper_update (int quit)
{
	int r = 1;
	char t[1000];
	sprintf (t, "%s%c.photostorm%cphotostorm.cwa", ps_get_user_dir(), ps_dir_sep, ps_dir_sep);
	_ps_photo *poza = ps_load_photo_nr_from_album_file (t, 1);
	GThread *firul;
	if (poza!=NULL)
	{
		_ps_progress *progress = ps_progress_dialog (ps_get_main_window(), poza);
		ps_progress_set_progress (progress, 0);
		ps_progress_set_text (progress, __("Searching Photo"));
		progress->quit = quit;
		firul = g_thread_create (ps_thread_photo_wallpaper, progress, TRUE, NULL);
	} else r = 0;
	return r;
}

int ps_wallpaper_new (int quit)
{
	char t[1000];
	sprintf (t, "%s%c.photostorm%cphotostorm.cwa", ps_get_user_dir(), ps_dir_sep, ps_dir_sep);
	char *albumul = ps_get_random_album ();
	int r = 1;
	GThread *firul;
	if (albumul!=NULL)
	{
		_ps_photo *poza = ps_load_photo_random_from_album_file (albumul);
		if (poza!=NULL)
		{
			_ps_progress *progress = ps_progress_dialog (ps_get_main_window(), poza);
			ps_progress_set_progress (progress, 0);
			ps_progress_set_text (progress, __("Searching Photo"));
			progress->quit = quit;
			firul = g_thread_create (ps_thread_photo_wallpaper, progress, TRUE, NULL);
		} else r = 0;
	} else r = 0;
	return r;
}

#endif

// filtre pentru imagine

void ps_write_calendar (gdImage *img, int fsize, int x, int y, int m_offset, int mark_current_day, _ps_color culoare, _ps_color marcaj, int b)
{
	time_t tmp;
	time (&tmp);
	tm *mytime = localtime (&tmp);
	mytime->tm_year += (m_offset)/12;
	mytime->tm_mon += (m_offset)%12;
	tmp = mktime (mytime);
	mytime = localtime (&tmp);
	if (img!=NULL && mytime!=NULL)
	{
		_ps_color *umbra = ps_pack_color (0, 0, 0, 100+27*(culoare.alpha)/127);
		const char *days[7] = { 
		                    __("Mon"), __("Tue"), __("Wed"), 
						    __("Thr"), __("Fri"), __("Sat"), 
						    __("Sun")
		                  };
		const char *months[12] = { 
		                    		__("January"), __("February"), __("March"), __("April"), 
						    		__("May"), __("June"), __("July"), __("August"), 
						    	    __("September"), __("October"), __("November"),
									__("December")
		                  		  };
		if (b) ps_draw_box (img, (int)(x-fsize), (int)(y-(fsize*3/2)), x+(int)(fsize*18), (int)(y+fsize*14));
		int m = mytime->tm_mon+1;
		int an = mytime->tm_year;
		int azi = mytime->tm_mday;
		//int month_start = 6-((mytime->tm_mday-mytime->tm_wday) % 7);
		int month_start = 6-((mytime->tm_mday+(7-mytime->tm_wday)) % 7);
		int nr_days=0;
		if (m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12) nr_days=31;
		if (m==4 || m==6 || m==9 || m==11) nr_days=30;
		if (m==2) if ((an%4==0 && an%100!=0) || (an%1000==0)) nr_days=29;
				  else nr_days=28;
		char luna[100];
		sprintf (luna, "%s %d", months[m-1], 1900+an);
		ps_write_to_image (img, x+(int)((20-strlen (luna))*fsize*0.8)/2+2, y+2, font_path ("monosb"),
				  		 	fsize, *umbra, 0.0, luna);
		ps_write_to_image (img, x+(int)((20-strlen (luna))*fsize*0.8)/2, y, font_path ("monosb"),
				  		 	fsize, culoare, 0.0, luna);
		int i;
		int _y = (int)(fsize*1.5);
		for (i=0;i<=6;i++) 
		{	
			ps_write_to_image (img, (int)(x+i*fsize*2.5)+2, y+_y+2, font_path ("monosb"), 
							   fsize*75/100, *umbra, 0.0, days[i]);
			ps_write_to_image (img, (int)(x+i*fsize*2.5), y+_y, font_path ("monosb"), 
							   fsize*75/100, culoare, 0.0, days[i]);
		}
		for (i=1;i<=42;i++)
		{
			char day[4];
			int d = (i-month_start > 0 && i-month_start <= nr_days)?i-month_start:0;
			sprintf (day, "%d", d);
			ps_write_to_image (img, (int)(x+(i%7)*fsize*2.5)+((d<10)?fsize/2:0)+2, y+_y+((i/7)+1)*fsize*2+2, font_path ("monosb"), 
							   fsize, *umbra, 0.0, (strcmp(day,"0")!=0)?day:"");
			ps_write_to_image (img, (int)(x+(i%7)*fsize*2.5)+((d<10)?fsize/2:0), y+_y+((i/7)+1)*fsize*2, font_path ("monosb"), 
							   fsize, (d==azi && mark_current_day)?marcaj:culoare, 0.0, (strcmp(day,"0")!=0)?day:"");
		}
		free (umbra);
	}
}

void ps_write_message (gdImage *img, int x, int y, int fsize, char *font, const char *from, const char *message, _ps_color col_from, _ps_color col_message)
{
	if (img!=NULL && font!=NULL && from!=NULL && message!=NULL)
	{
		int brect[8], brect2[8];
		int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(col_from.alpha)/127);
		int colf = gdImageColorAllocateAlpha (img, col_from.red, col_from.green, col_from.blue, col_from.alpha);
		gdImageStringFT (img, &brect[0], colu, font, fsize, 0.0, x+2, y+2, (char*)from);
		gdImageStringFT (img, &brect[0], colf, font, fsize, 0.0, x, y, (char*)from);
		gdImageStringFT (NULL, &brect[0], colf, font, fsize, 0.0, x, y, (char*)from);
		int colm = gdImageColorAllocateAlpha (img, col_message.red, col_message.green, col_message.blue, col_message.alpha);
		gdImageStringFT (img, &brect2[0], colu, font,fsize, 0.0, x+brect[2]-brect[6]+10+2, y+2, (char*)message);
		gdImageStringFT (img, &brect2[0], colm, font,fsize, 0.0, x+brect[2]-brect[6]+10, y, (char*)message);
	}
}

void ps_write_information (gdImage *img, _ps_photo *poza, const char *format, int x, int y, int fsize, int dx, int dy, _ps_color *col, int box, int pos_auto)
{
	if (poza!=NULL && format!=NULL)
	{
		if (fsize==0) fsize = dx/90;
		char temp [10000];
		strcpy (temp, "");
		for (unsigned int i=0;i<strlen (format);i++)
		{
			if (format[i]=='%' && strlen (format)>i+1)
			{
				i++;
				switch (format[i])
				{
					case 'n': if (poza->nume!=NULL) strcat (temp, poza->nume);
						break;
					case 'a': if (poza->autor!=NULL) strcat (temp, poza->autor);
						break;
					case '&': if (poza->autor!=NULL) strcat (temp, "&");
						break;
				}
			}
			else
			if (format[i]=='\\' && strlen (format)>i+1)
			{
				i++;
				switch (format[i])
				{
					case 'n': if (poza->nume!=NULL) strcat (temp, "\n");
						break;
				}
			}
			else
			{
				int n = strlen(temp);
				temp[n+1]='\0';
				temp[n]=format[i];
			}
		}
		/*int linie_noua=0;
		for (unsigned int i=0;i<strlen(q->quote);i++)
		{
			sprintf (&temp[strlen(temp)], "%c\0", q->quote[i]);
			if ((i+1)%(dx/fsize/2)==0) linie_noua=1;
			if (q->quote[i]==' ' && linie_noua)
			{
				sprintf (&temp[strlen(temp)], "\n\0"); 
				linie_noua=0;
			}
		}*/
		int colt = gdImageColorAllocateAlpha (img, col->red, col->green, col->blue, col->alpha);
		int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(col->alpha)/127);
		int spatiu_text[8];
		//int spatiu_autor[8];
		gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("arial"), fsize, 0.0, x, y, temp);
		if (pos_auto!=0)
		{
			x = dx/70;
			y = dy-(spatiu_text[3]-spatiu_text[5]+dy/30);
			gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("arial"), fsize, 0.0, x, y, temp);
		}
		if (box!=0)
		{
			gdImageFilledRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+5, gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(col->alpha)/127));
			gdImageRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+5, gdImageColorAllocateAlpha (img, 0, 0, 0, 60+67*(col->alpha)/127));
		}
		//_ps_color *colu = ps_pack_color (0,0,0, 100);
		gdImageStringFT (img, &spatiu_text[0], colu, font_path ("arial"), fsize, 0.0, x+2, y+2, temp);
		gdImageStringFT (img, &spatiu_text[0], colt, font_path ("arial"), fsize, 0.0, x, y, temp);
		
		/*gdImageStringFT (img, &spatiu_autor[0], colu, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0]+2, spatiu_text[1]+fsize+5, q->author);
		gdImageStringFT (img, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0], spatiu_text[1]+fsize+3, q->author);*/
		
		//ps_write_to_image (img, x+2, y+2, font_path ("signature"), fsize, *colu, 0.0, temp);
		//ps_write_to_image (img, x, y, font_path ("signature"), fsize, *col, 0.0, temp);
		//free (colu);
	}
}

void ps_write_quote (gdImage *img, _ps_quote *q, int x, int y, int fsize, int dx, int dy, _ps_color *col, int box, int pos_auto)
{
	if (q!=NULL && q->quote!=NULL && q->author!=NULL)
	{
		if (fsize==0) fsize = dx/96;
		char temp [10000];
		strcpy (temp, "");
		int linie_noua=0;
		for (unsigned int i=0;i<strlen(q->quote);i++)
		{
			sprintf (&temp[strlen(temp)], "%c", q->quote[i]);
			if ((i+1)%(dx/fsize/2)==0) linie_noua=1;
			if (q->quote[i]==' ' && linie_noua)
			{
				sprintf (&temp[strlen(temp)], "\n"); 
				linie_noua=0;
			}
		}
		int colt = gdImageColorAllocateAlpha (img, col->red, col->green, col->blue, col->alpha);
		int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(col->alpha)/127);
		int spatiu_text[8];
		int spatiu_autor[8];
		gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		if (pos_auto!=0)
		{
			x = dx-(spatiu_text[2]-spatiu_text[0]+50);
			y = dy-(spatiu_text[3]-spatiu_text[5]+80);
			gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		}
		gdImageStringFT (NULL, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, x, y, q->author);
		if (box!=0)
		{
			gdImageFilledRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(col->alpha)/127));
			gdImageRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 60+67*(col->alpha)/127));
		}
		//_ps_color *colu = ps_pack_color (0,0,0, 100);
		gdImageStringFT (img, &spatiu_text[0], colu, font_path ("signature"), fsize, 0.0, x+2, y+2, temp);
		gdImageStringFT (img, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		
		gdImageStringFT (img, &spatiu_autor[0], colu, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0]+2, spatiu_text[1]+fsize+5, q->author);
		gdImageStringFT (img, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0], spatiu_text[1]+fsize+3, q->author);
		
		//ps_write_to_image (img, x+2, y+2, font_path ("signature"), fsize, *colu, 0.0, temp);
		//ps_write_to_image (img, x, y, font_path ("signature"), fsize, *col, 0.0, temp);
		//free (colu);
	}
}

#ifndef PHOTOSTORM_WEBSITE

void ps_write_email (gdImage *img, _ps_emails *emails, int x, int y, int fsize, int dx, int dy, _ps_color *fcol, _ps_color *scol, int box, int pos_auto)
{
	if (emails!=NULL)
	{
		int d=0, by=0, mf=0, ms=0;
		int nr_e=0;
		if (fsize==0) fsize = dx/110;
		int m_text[8];
		int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(fcol->alpha)/127);
		int colf = gdImageColorAllocateAlpha (img, fcol->red, fcol->green, fcol->blue, fcol->alpha);
		int cols = gdImageColorAllocateAlpha (img, scol->red, scol->green, scol->blue, scol->alpha);
		for (int i=0;i<emails->count;i++)
		{
			if (emails->email[i]!=NULL) 
			{
				nr_e++;
				for (int l=0;l<8;l++) m_text[i]=0;
				if (emails->email[i]->from!=NULL) gdImageStringFT (NULL, &m_text[0], colf, font_path ("arial"), fsize, 0.0, x, y, emails->email[i]->from);
				int t=m_text[2]-m_text[0];
				if (t>mf) mf = t;
					
				for (int l=0;l<8;l++) m_text[i]=0;
				if (emails->email[i]->subject!=NULL) gdImageStringFT (NULL, &m_text[0], cols, font_path ("arial"), fsize, 0.0, x, y, emails->email[i]->subject);
				t=m_text[2]-m_text[0];
				if (t>ms) ms = t;
				
				t=m_text[3]-m_text[5];
				if (d<t) d=t;
				by=by+d+3;					
			}
		}
		if (pos_auto!=0)
		{
			x = dx/10;
			y = dy/10;
		}
		if (box!=0)
		{
			gdImageFilledRectangle (img, x-7, y-d-3, x+ms+mf+20, y+by-3, gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(fcol->alpha)/127));
			gdImageRectangle (img, x-7, y-d-3, x+ms+mf+20, y+by-3, gdImageColorAllocateAlpha (img, 0, 0, 0, 60+67*(fcol->alpha)/127));
		}
		for (int i=0;i<emails->count;i++)
		{
			if (emails->email[i]!=NULL) 
			{
				if (emails->email[i]->from!=NULL) 
				{
					gdImageStringFT (img, &m_text[0], colu, font_path ("arial"), fsize, 0.0, x+2, y+(d+3)*i+2, emails->email[i]->from);
					gdImageStringFT (img, &m_text[0], colf, font_path ("arial"), fsize, 0.0, x, y+(d+3)*i, emails->email[i]->from);
				}
				if (emails->email[i]->subject!=NULL) 
				{
					gdImageStringFT (img, &m_text[0], colu, font_path ("arial"), fsize, 0.0, x+mf+10+2, y+(d+3)*i+2, emails->email[i]->subject);
					gdImageStringFT (img, &m_text[0], cols, font_path ("arial"), fsize, 0.0, x+mf+10, y+(d+3)*i, emails->email[i]->subject);
				}
			}
		}
		/*int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100);
		int spatiu_text[8];
		int spatiu_autor[8];
		gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		if (pos_auto!=0)
		{
			x = dx-(spatiu_text[2]-spatiu_text[0]+50);
			y = dy-(spatiu_text[3]-spatiu_text[5]+80);
			gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		}
		gdImageStringFT (NULL, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, x, y, q->author);
		if (box!=0)
		{
			gdImageFilledRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 100));
			gdImageRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 60));
		}
		//_ps_color *colu = ps_pack_color (0,0,0, 100);
		gdImageStringFT (img, &spatiu_text[0], colu, font_path ("signature"), fsize, 0.0, x+2, y+2, temp);
		gdImageStringFT (img, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		
		gdImageStringFT (img, &spatiu_autor[0], colu, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0]+2, spatiu_text[1]+fsize+5, q->author);
		gdImageStringFT (img, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0], spatiu_text[1]+fsize+3, q->author);*/
		
		//ps_write_to_image (img, x+2, y+2, font_path ("signature"), fsize, *colu, 0.0, temp);
		//ps_write_to_image (img, x, y, font_path ("signature"), fsize, *col, 0.0, temp);
		//free (colu);
	}
}

void ps_write_messages (gdImage *img, GList *messages_list, int x, int y, int fsize, int dx, int dy, _ps_color *tcol, _ps_color *mcol, int box, int pos_auto)
{
	if (messages_list!=NULL)
	{
		time_t tmp;
		time (&tmp);
		tm *mytime = localtime (&tmp);
		int d=0, by=0, mf=0, ms=0;
		int nr_msg=0;
		if (fsize==0) fsize = dx/110;
		int m_text[8];
		int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(mcol->alpha)/127);
		int colt = gdImageColorAllocateAlpha (img, tcol->red, tcol->green, tcol->blue, tcol->alpha);
		int colm = gdImageColorAllocateAlpha (img, mcol->red, mcol->green, mcol->blue, mcol->alpha);
		int nr_msgs = g_list_length (messages_list);
		int wday = mytime->tm_wday;
		if (wday==0) wday=7;
		for (int i=0;i<nr_msgs;i++)
		{
			_ps_message *message = (_ps_message*)g_list_nth_data (messages_list, i);
			if (message!=NULL && 
				(message->year==0 || (message->year==(mytime->tm_year+1900))) &&
				(message->month==0 || (message->month==(mytime->tm_mon+1))) &&
				(message->date==0 || (message->date==mytime->tm_mday)) &&
				(message->wday==0 || (message->wday==wday))
			   )
			{
				nr_msg++;
				char msg_time[100];
				sprintf (msg_time, "%.2d:%.2d", message->hour, message->minute);
				for (int l=0;l<8;l++) m_text[i]=0;
				gdImageStringFT (NULL, &m_text[0], colt, font_path ("monosb"), fsize, 0.0, x, y, msg_time);
				int t=m_text[2]-m_text[0];
				if (t>mf) mf = t;
					
				for (int l=0;l<8;l++) m_text[i]=0;
				gdImageStringFT (NULL, &m_text[0], colm, font_path ("arial"), fsize, 0.0, x, y, message->message_text);
				t=m_text[2]-m_text[0];
				if (t>ms) ms = t;
				
				t=m_text[3]-m_text[5];
				//printf ("t: %d\n", t);
				if (d<t) d=t;
				by=by+d+3;					
			}
		}
		if (pos_auto!=0)
		{
			x = dx/10;
			y = dy/2;
		}
		if (box!=0)
		{
			gdImageFilledRectangle (img, x-7, y-d-3, x+ms+mf+20, y+by-3, gdImageColorAllocateAlpha (img, 0, 0, 0, 100+27*(mcol->alpha)/127));
			gdImageRectangle (img, x-7, y-d-3, x+ms+mf+20, y+by-3, gdImageColorAllocateAlpha (img, 0, 0, 0, 60+67*(mcol->alpha)/127));
		}
		nr_msgs = g_list_length (messages_list);
		nr_msg = 0;
		char *l_time = NULL;
		for (int i=0;i<nr_msgs;i++)
		{
			_ps_message *message = (_ps_message*)g_list_nth_data (messages_list, i);
			if (message!=NULL && 
				(message->year==0 || (message->year==(mytime->tm_year+1900))) &&
				(message->month==0 || (message->month==(mytime->tm_mon+1))) &&
				(message->date==0 || (message->date==mytime->tm_mday)) &&
				(message->wday==0 || (message->wday==wday))
			   )
			{
				char msg_time[100];
				sprintf (msg_time, "%.2d:%.2d", message->hour, message->minute);
				if (l_time==NULL || (l_time!=NULL && strcmp (l_time, msg_time)!=0))
				{
					gdImageStringFT (img, &m_text[0], colu, font_path ("monosb"), fsize, 0.0, x+2, y+(d+3)*nr_msg+2, msg_time);
					gdImageStringFT (img, &m_text[0], colt, font_path ("monosb"), fsize, 0.0, x, y+(d+3)*nr_msg, msg_time);
					if (l_time!=NULL) free (l_time);
					l_time = strdup (msg_time);
				}
				gdImageStringFT (img, &m_text[0], colu, font_path ("arial"), fsize, 0.0, x+mf+10+2, y+(d+3)*nr_msg+2, message->message_text);
				gdImageStringFT (img, &m_text[0], colm, font_path ("arial"), fsize, 0.0, x+mf+10, y+(d+3)*nr_msg, message->message_text);
				nr_msg++;
			}
		}
		if (l_time!=NULL) free (l_time);
		/*int colu = gdImageColorAllocateAlpha (img, 0, 0, 0, 100);
		int spatiu_text[8];
		int spatiu_autor[8];
		gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		if (pos_auto!=0)
		{
			x = dx-(spatiu_text[2]-spatiu_text[0]+50);
			y = dy-(spatiu_text[3]-spatiu_text[5]+80);
			gdImageStringFT (NULL, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		}
		gdImageStringFT (NULL, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, x, y, q->author);
		if (box!=0)
		{
			gdImageFilledRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 100));
			gdImageRectangle (img, spatiu_text[6]-7, spatiu_text[7]-7, spatiu_text[2]+7, spatiu_text[3]+fsize+5+7, gdImageColorAllocateAlpha (img, 0, 0, 0, 60));
		}
		//_ps_color *colu = ps_pack_color (0,0,0, 100);
		gdImageStringFT (img, &spatiu_text[0], colu, font_path ("signature"), fsize, 0.0, x+2, y+2, temp);
		gdImageStringFT (img, &spatiu_text[0], colt, font_path ("signature"), fsize, 0.0, x, y, temp);
		
		gdImageStringFT (img, &spatiu_autor[0], colu, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0]+2, spatiu_text[1]+fsize+5, q->author);
		gdImageStringFT (img, &spatiu_autor[0], colt, font_path ("arial"), fsize-4, 0.0, spatiu_text[2]-spatiu_autor[2]+spatiu_autor[0], spatiu_text[1]+fsize+3, q->author);*/
		
		//ps_write_to_image (img, x+2, y+2, font_path ("signature"), fsize, *colu, 0.0, temp);
		//ps_write_to_image (img, x, y, font_path ("signature"), fsize, *col, 0.0, temp);
		//free (colu);
	}
}

#endif

void ps_draw_box (gdImage *img, int x, int y, int r, int t)
{
	gdImageFilledRectangle (img, x, y, r, t, gdImageColorAllocateAlpha (img, 0, 0, 0, 100));
	gdImageRectangle (img, x, y, r, t, gdImageColorAllocateAlpha (img, 0, 0, 0, 60));
}

void test ()
{
	gdImagePtr p;
	p = gdImageCreateFromJpegPtr (1, NULL);
	p = gdImageCreateFromPngPtr (1, NULL);
}

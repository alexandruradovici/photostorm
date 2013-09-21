/**************************************************************
*		File: photo.h                                          *
*		ID: PS_photo_h                                         *
*		Author: Alexandru Radovici                             *
*		Date: February 9, 2004                                 *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file is the interface header of          *
*               the photo processing of PhotoStorm.           *
**************************************************************/

#ifndef _PHOTO__H
#define _PHOTO__H

#include "data.h"

#ifdef PHOTOSTORM_PROGRAM

#include "gdlib/gd.h"
#include "quotes.h"

#ifndef PHOTOSTORM_WEBSITE

#include "mail.h"

#endif

#endif

#define ps_create_photo_preview(poza, n, r, t, rn) ps_create_photo_preview_data (poza, n, r, t, rn, true)

struct _ps_photo
{
	char *nume;
	char *autor;
	char *descriere;
	char *optiuni;
	char *metadata;
	int n_poza_preview;
	char *date_preview;
	unsigned long n_poza;
	char *date;
};

struct _ps_color
{
	int red;
	int green;
	int blue;
	int alpha;
};

#include "configs.h"

#ifdef PHOTOSTORM_PROGRAM

// loading
char * ps_load_picture (const char *poza, long &n);

// creating
_ps_photo * ps_create_photo (const char *poza);
_ps_photo * ps_create_photo_with_data (char *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata);
char * ps_create_photo_preview_data (char * poza, long n, int r, int t, int &rn, int b);

// modifying
void ps_modify_photo (_ps_photo *poza, char *nume, char *autor, char *descriere, char *optiuni, char *metadata);

// writing
char *ps_pack_photo (_ps_photo *poza, long &n);

// system functions
int ps_wallpaper_new (int quit);
int ps_photo_wallpaper (_ps_photo *poza);
int ps_wallpaper_update (int quit);

// filters
void ps_write_to_image (gdImage *img, int x, int y, char *font, int size, _ps_color color, float direction, const char *text);
void ps_write_information (gdImage *img, _ps_photo *poza, const char *format, int x, int y, int fsize, int dx, int dy, _ps_color *col, int box, int pos_auto);
void ps_write_calendar (gdImage *img, int fsize, int x, int y, int m_offset, int mark_current_day, _ps_color culoare, _ps_color marcaj, int b);
void ps_write_message (gdImage *img, int x, int y, int fsize, char *font, const char *from, const char *message, _ps_color col_from, _ps_color col_message);
void ps_write_quote (gdImage *img, _ps_quote *q, int x, int y, int fsize, int dx, int dy, _ps_color *col, int box, int pos_auto);
#ifndef PHOTOSTORM_WEBSITE
void ps_write_email (gdImage *img, _ps_emails *emails, int x, int y, int fsize, int dx, int dy, _ps_color *fcol, _ps_color *scol, int box, int pos_auto);
#endif
void ps_draw_box (gdImage *img, int x, int y, int r, int t);

// freeing
void ps_photo_free (_ps_photo *date_poza);

#ifdef PHOTOSTORM_WEBSITE

#ifdef _PS_CONFIG
char * ps_create_photo_wallpaper (_ps_photo *poza, int &nr, _ps_config *config);
#endif

#endif

#ifndef PHOTOSTORM_WEBSITE
char * ps_creaza_poza_pentru_afisare (_ps_photo *date_poza, int r, int t, int &n);
#endif

#endif

#endif

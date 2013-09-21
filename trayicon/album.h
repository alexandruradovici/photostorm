/**************************************************************
*		File: album.h                                          *
*		ID: PS_album_h                                         *
*		Author: Alexandru Radovici                             *
*		Date: February 9, 2004                                 *
*		Modified: never                                        *
*		This file is copyrighted to Alexandru Radovici         *
*		and complies to the terms of PSLA v0.1                 *
*                                                             *
*		Comment: This file is the interface header of          *
*               the album processing of PhotoStorm.           *
**************************************************************/

#ifndef _ALBUM__H
#define _ALBUM__H

#include "data.h"

#include "photo.h"
#include <time.h>

#define psa_sign "PSA"
#define psa_version "0.1"

#define ps_load_album(albumul) ps_load_album_data (albumul, true)
#define ps_load_album_heather(albumul) ps_load_album_data (albumul, false)

#ifdef PHOTOSTORM_PROGRAM

#define ps_load_photo_nr_from_album_file(albumul, pos) ps_load_photo_data_nr_from_album_file (albumul, pos, true)
#define ps_load_photo_nr_from_album_memory(albumul, pos) ps_load_photo_data_nr_from_album_memory (albumul, pos, true)
#define ps_load_photo_random_from_album_file(albumul) ps_load_photo_data_random_from_album_file (albumul, true)
#define ps_load_photo_random_from_album_memory(albumul) ps_load_photo_data_random_from_album_memory (albumul, true)

#endif

typedef struct _ps_album
{
	char *nume;
	char *autor;
	char *categoria;
	char *descriere;
	char *metadata;
	time_t data;
	char *parola;
	long n_antet;
	long n_poze;
	unsigned char nr_poze;
	//char *antet;
	char *date;
};

#ifdef PHOTOSTORM_PROGRAM_DESIGN

// static functions
_ps_album * ps_get_current_album_data ();
void ps_load_current_album ();
void ps_free_current_album ();

// design part
void ps_load_albums_from_path (const char *calea);
void ps_clear_albums_list ();
void ps_add_album_from_file (const char *albumul);
int ps_list_album_from_file (const char *albumul);
int ps_list_album_from_memory (_ps_album *date_album);

#endif

#ifdef PHOTOSTORM_PROGRAM

// importing album
void import_album (char *albumul);

// create album
int ps_create_new_album (char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola, const char *file);
_ps_album * ps_create_new_album_in_memory (char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola);

#endif

// loading
_ps_album * ps_load_album_data (const char *albumul, int b);

#ifdef PHOTOSTORM_PROGRAM

_ps_photo * ps_load_photo_data_nr_from_album_file (const char *albumul, int pos, int b);
_ps_photo * ps_load_photo_data_nr_from_album_memory (_ps_album *date_album, int pos, int b);
_ps_photo * ps_load_photo_data_random_from_album_file (const char *albumul, int b);
_ps_photo * ps_load_photo_data_random_from_album_memory (_ps_album *date_album, int b);

// adding
int ps_add_photo_to_album_from_file (const char *poza, char *albumul);
int ps_add_photo_to_album_from_file_into_memory (const char *poza, _ps_album *date_album);
int ps_add_photo_to_album_from_memory (_ps_photo *date_poza, const char *albumul);
int ps_add_photo_to_album_from_memory_into_memory (_ps_photo *date_poza, _ps_album *date_album);

// inserting
int ps_insert_photo_into_album_from_file (const char *poza, const char *albumul, int pos);
int ps_insert_photo_into_album_from_file_into_memory (const char *poza, _ps_album *date_album, int pos);
int ps_insert_photo_into_album_from_memory (_ps_photo *date_poza, const char *albumul, int pos);
int ps_insert_photo_into_album_from_memory_into_memory (_ps_photo *date_poza, _ps_album *date_album, int pos);

// modifiying
int ps_modify_album_from_file (const char *albumul, char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola);
int ps_modify_album_from_memory (_ps_album *date_album, char *nume, char *autor, char *categoria, char *descriere, char *metadata, char *parola);
int ps_modify_photo_nr_from_album_file (_ps_photo *poza, const char *albumul, int pos);
int ps_modify_photo_nr_from_album_memory (_ps_photo *poza, _ps_album *date_album, int pos);

// deleting
int ps_delete_photo_nr_from_album_file (const char *albumul, int pos);
int ps_delete_photo_nr_from_album_memory (_ps_album *date_album, int pos);

// writing
char * ps_pack_album (_ps_album *album, long &n);
int ps_write_album (const char *albumul, char *date, long n);
int ps_write_album_from_memory (const char *albumul, _ps_album *date_album);

#endif

// freeing
void ps_album_free (_ps_album *date_album);

#endif

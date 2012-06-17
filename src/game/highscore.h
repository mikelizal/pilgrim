// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

// en el sprintf que copia el nombre del textfield hay un %20s
// que habria que cambiar si cambias esto
#define HIGHSCORE_NAME_MAXL 20
#define HIGHSCORE_MAX 10
#define HIGHSCORE_MAX_TUMBAS 3
#define HIGHSCORE_MINX_TUMBAS 101
// ojo si lo pones mas pequeño que los records que se generan por defecto

extern char *highscore_filename;

struct _highscore_entry {
    char name[HIGHSCORE_NAME_MAXL+1];
    int score;
    int hits;
};

typedef struct _highscore_entry t_highscore_entry;

extern t_highscore_entry highscore_table[];

void highscore_setfilename(const char *path,const char *file);

void highscore_load(void);

void highscore_save(void);

int highscore_enter_pos(int score);

void highscore_insert_name_pos(char *name,int score,int pos);

void highscore_print(void);

void highscore_space_encode(char *s);
void highscore_space_decode(char *s);


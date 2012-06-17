// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

char *highscore_filename;

t_highscore_entry highscore_table[HIGHSCORE_MAX];

void highscore_setfilename(const char *path,const char *filename) {
    int len=(int)strlen(filename)+(int)strlen(path)+5;
    if (highscore_filename) {
        free(highscore_filename);
    }
    highscore_filename=malloc(len);
    sprintf(highscore_filename,"%s/%s",path,filename);
    persistence_setpath(path);
}


void highscore_load(void) {
    FILE *f;
    char buf[HIGHSCORE_NAME_MAXL+20];
    int i;
    int distance;
    f=fopen(highscore_filename,"r");
    if (f!=NULL) {
        for (i=0;i<HIGHSCORE_MAX;i++) {
            if (fgets(buf,HIGHSCORE_NAME_MAXL+20,f)) {
                sscanf(buf,"%d %u %s",&distance,&(highscore_table[i].hits),highscore_table[i].name);
                highscore_space_decode(highscore_table[i].name);
                highscore_table[i].score=distance;
            }
        }
        fclose(f);
    } else {
        for (i=0;i<HIGHSCORE_MAX;i++) {
            sprintf(highscore_table[i].name,"Phil Green");
            highscore_table[i].score=100-i;
            highscore_table[i].hits=0;
        }
    }
}

void highscore_save(void) {
    FILE *f;
    int i;
    f=fopen(highscore_filename,"w");
    for (i=0;i<HIGHSCORE_MAX;i++) {
        highscore_space_encode(highscore_table[i].name);
        fprintf(f,"%d %u %s\n", highscore_table[i].score,highscore_table[i].hits,highscore_table[i].name);
        highscore_space_decode(highscore_table[i].name);
    }
    fclose(f);
}

// devuelve la posicion en la que entra en la lista de records -1 para decir que no entra
int highscore_enter_pos(int score) {
    int r=0;
    while (r<HIGHSCORE_MAX && highscore_table[r].score>score) {
        r+=1;
    }
    
    if (r<HIGHSCORE_MAX) {
        return r;
    } else {
        return -1;
    }
}

// puede coger mas datos de stats
void highscore_insert_name_pos(char *name,int score,int pos) {
    int i;
    for (i=HIGHSCORE_MAX-1;i>pos;i--) {
        sprintf(highscore_table[i].name,"%s",highscore_table[i-1].name);
        highscore_table[i].score=highscore_table[i-1].score;
        highscore_table[i].hits=highscore_table[i-1].hits;
    }
    sprintf(highscore_table[pos].name,"%s",name);
    highscore_table[pos].score=score;
    highscore_table[pos].hits=stats.peregrinos_hit+stats.bordoneros_hit;    
    highscore_save();
}

void highscore_print(void) {
    int i;
    for (i=0;i<HIGHSCORE_MAX;i++) {
        printf("%d.- %s ( %d )\n",i,highscore_table[i].name,highscore_table[i].score);
    }
}

void highscore_space_encode(char *s) {
    int i;
    for (i=0;i<strlen(s);i++) {
        if (s[i]==' ') {
            s[i]='+';
        }
    }
}

void highscore_space_decode(char *s) {
    int i;
    for (i=0;i<strlen(s);i++) {
        if (s[i]=='+') {
            s[i]=' ';
        }
    }    
}

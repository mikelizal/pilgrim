// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"

#include <string.h>

char *persistence_path;
char *persistence_filepath;

int persistence_total_played_games=0;

void persistence_setpath(const char *path) {
    int len=(int)strlen(path)+5;
    if (persistence_path) {
        free(persistence_path);
    }
    persistence_path=malloc(len);
    sprintf(persistence_path,"%s",path);

    if (persistence_filepath) {
        free(persistence_filepath);
    }
    persistence_filepath=malloc(len+50);
    sprintf(persistence_filepath,"%s/%s",persistence_path,"persistence");
}

void persistence_load(void) {
    FILE *f;
    char buf[100];
    
    f=fopen(persistence_filepath,"r");
    if (f) {
        fgets(buf,100,f);
        sscanf(buf,"%u",&persistence_total_played_games);
        fclose(f);
    }
}

void persistence_save(void) {
    FILE *f;

    f=fopen(persistence_filepath,"w");
    fprintf(f,"%u\n",persistence_total_played_games);
    fclose(f);
}

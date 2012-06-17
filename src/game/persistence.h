// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


extern char *persistence_path;

extern int persistence_total_played_games;


void persistence_setpath(const char *path);
void persistence_load(void);
void persistence_save(void);



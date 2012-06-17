// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details



struct _stats {
    t_pos distance;
    t_time time;
    int peregrinos_hit;
    int bordoneros_hit;
    int caidas;
    int comida;
};

typedef struct _stats t_stats;

extern t_stats stats;


void stats_reset(void);

void stats_add_caida(void);
void stats_add_hit(int tipo);
void stats_add_comida(int tipo);

void stats_end_game_timedistance(void);

void stats_sprintf_distancia(char *buf);
void stats_sprintf_time(char *buf);
void stats_sprintf_speed(char *buf);
void stats_sprintf_hits(char *buf,int tipo);
void stats_sprintf_comida(char *buf,int tipo);
void stats_sprintf_caidas(char *buf);

void stats_print(void);

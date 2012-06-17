// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


t_stats stats;

void stats_reset(void) {
    stats.distance=0.0;
    stats.time=0.0;
    stats.peregrinos_hit=0;
    stats.bordoneros_hit=0;
    stats.caidas=0;
    stats.comida=0;
    //printf("stats reset\n");
}


void stats_add_caida(void) {
    stats.caidas+=1;
    //printf("stats caida\n");
}

void stats_add_hit(int tipo) {
    if (tipo==SPRCLS_ID_PEREGRINO) {
        stats.peregrinos_hit+=1;
    } else if (tipo==SPRCLS_ID_BORDONERO) {
        stats.bordoneros_hit+=1;
    }
    //printf("stats hit\n");
}

void stats_add_comida(int tipo) {
    stats.comida+=1;
    //printf("stats comida\n");
}


void stats_end_game_timedistance(void) {
    stats.time=world.t-world_t0;
    stats.distance=(world.player.x-world_pos0);
}


void stats_sprintf_distancia(char *buf) {
    sprintf(buf,"%.0f m",(float)(int)(stats.distance/100.0));
}

void stats_sprintf_time(char *buf) {
    sprintf(buf,"%.0f s",stats.time);
}

void stats_sprintf_speed(char *buf) {
    sprintf(buf,"%.0f m/s",stats.distance*0.01/stats.time);
}

void stats_sprintf_hits(char *buf,int tipo) {
    switch (tipo) {
        case 0: // todos
            sprintf(buf,"%u hits :",stats.peregrinos_hit+stats.bordoneros_hit);
            break;
        case 1: // peregrinos
            sprintf(buf,"%u",stats.peregrinos_hit);
            break;
        case 2: // bordoneros
            sprintf(buf,"%u",stats.bordoneros_hit);
            break;
        default:
            sprintf(buf,"%u",stats.peregrinos_hit+stats.bordoneros_hit);
            break;
    }
}

void stats_sprintf_comida(char *buf,int tipo) {
    sprintf(buf,"%u",stats.comida);    
}

void stats_sprintf_caidas(char *buf) {
    sprintf(buf,"%u",stats.caidas);
}

void stats_print(void) {
    printf("-- Stats --------\n");
    printf("distance: %f pixels %f m\n",stats.distance, stats.distance/100.0);
    printf("time: %f s (incluye segundos de espera en ready)\n",stats.time);
    printf("velocidad media: %f m/s (incluyendo tiempo parado en ready)\n",stats.distance*0.01/stats.time);
    printf("golpeados: %u\n",stats.peregrinos_hit+stats.bordoneros_hit);
    printf("  %u peregrinos\n",stats.peregrinos_hit);
    printf("  %u bordoneros\n",stats.bordoneros_hit);
    printf("comida: %u\n",stats.comida);
    printf("caidas: %u\n",stats.caidas);
    printf("-----------------\n");
}


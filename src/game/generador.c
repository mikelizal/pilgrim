// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"

#include <math.h>


t_pos generador_bicis_min_x = 50000;
t_pos generador_bicis_dx_to_generate = -2000; // distancia del player a la que se crea el sprite
t_pos generador_bicis_base_interarrival_x = 5000;
t_pos generador_bicis_min_interarrival_x = 2000;
t_pos generador_bicis_max_interarrival_x = 10000;
t_pos generador_bicis_rate_multiplier = 1.0;



t_pos generador_cansados_min_x = 20000;
t_pos generador_cansados_dx_to_generate = 2000; // distancia del player a la que se crea el sprite
t_pos generador_cansados_base_interarrival_x = 2000;
t_pos generador_cansados_min_interarrival_x = 500;
t_pos generador_cansados_max_interarrival_x = 2500;
t_pos generador_cansados_rate_multiplier = 1.0;



t_pos generador_walker_min_x = 1000;
t_pos generador_walker_dx_to_generate = 2000; // distancia del player a la que se crea el sprite
t_pos generador_walker_base_interarrival_x = 2000;
t_pos generador_walker_min_interarrival_x = 200;
t_pos generador_walker_max_interarrival_x = 3000;
t_pos generador_walker_rate_multiplier = 1.0;


t_bool generador_keep_constant_seed;


t_pos next_bici;
unsigned int generador_bici_seed;

t_pos next_cansado;
unsigned int generador_cansado_seed;

t_pos next_walker;
unsigned int generador_walker_seed;

unsigned int generador_zone_seed;
unsigned int generador_items_seed;

void generador_init(void) {
    generador_keep_constant_seed=0;
    
    
    generador_bicis_rate_multiplier = 1.0;
    generador_cansados_rate_multiplier = 1.0;
    generador_walker_rate_multiplier = 1.0;
    
    
    if (generador_keep_constant_seed) {
        generador_cansado_seed=1;
        generador_walker_seed=100;
        generador_bici_seed=42;
        generador_zone_seed=1001;
    } else {
        srand(time_get_time_seed());
        generador_cansado_seed=rand();
        generador_walker_seed=rand();
        generador_bici_seed=rand();
        generador_zone_seed=1001;
        generador_items_seed=rand();
    }
    
    
    
    
    next_cansado=generador_cansados_min_x + generador_next_interarrival_with_seed(&generador_cansado_seed,generador_cansados_base_interarrival_x,generador_cansados_rate_multiplier,generador_cansados_min_interarrival_x,generador_cansados_max_interarrival_x);
    
    next_walker=generador_walker_min_x + generador_next_interarrival_with_seed(&generador_walker_seed,generador_walker_base_interarrival_x,generador_walker_rate_multiplier,generador_walker_min_interarrival_x,generador_walker_max_interarrival_x);

    next_bici=generador_bicis_min_x + generador_next_interarrival_with_seed(&generador_bici_seed,generador_bicis_base_interarrival_x,generador_bicis_rate_multiplier,generador_bicis_min_interarrival_x,generador_bicis_max_interarrival_x);

    
//    printf("first cansado: %f\n",next_cansado);
//    printf("first walker: %f\n",next_walker);
//    printf("first bici: %f\n",next_bici);
}




void generador_update(void) {
    t_pos x=world.player.x;
//    t_time t=world.t;
    
    next_walker+=world_walker_v*game_dt;

    int grow_thres=60000;
    if (x>grow_thres) {
        int ifactor=((int)((x-grow_thres)/100.0))%100;
        ifactor=ifactor>20?20:ifactor;
        generador_walker_rate_multiplier=1.0+0.1*ifactor;
    }
    
    if ( next_cansado < x+generador_cansados_dx_to_generate ) {
        world_create_walker(next_cansado-x,2);
        next_cansado+=generador_next_interarrival_with_seed(&generador_cansado_seed,generador_cansados_base_interarrival_x,generador_cansados_rate_multiplier,generador_cansados_min_interarrival_x,generador_cansados_max_interarrival_x);
    }

    
    if ( next_walker < x+generador_walker_dx_to_generate ) {
        int r=rand_r(&generador_walker_seed),walker_type;
        walker_type=0;
        if (r%4==0) walker_type=1;
        world_create_walker(next_walker-x,walker_type);
        next_walker+=generador_next_interarrival_with_seed(&generador_walker_seed,generador_walker_base_interarrival_x,generador_walker_rate_multiplier,generador_walker_min_interarrival_x,generador_walker_max_interarrival_x);
    }

    
    if ( next_bici < x+generador_bicis_dx_to_generate ) {
        world_create_walker(next_bici-x,3);
        next_bici+=generador_next_interarrival_with_seed(&generador_bici_seed,generador_bicis_base_interarrival_x,generador_bicis_rate_multiplier,generador_bicis_min_interarrival_x,generador_bicis_max_interarrival_x);
    }
    
    
    
    
}




t_pos generador_next_interarrival_with_seed(unsigned int *seed, t_pos interarrival, float increase_factor, t_pos min_interarrival, t_pos max_interarrival) {
    t_pos next_dx;
    
    next_dx=generador_random_exponential_for_seed(increase_factor*1.0/interarrival,seed);
    if (next_dx<min_interarrival) next_dx=min_interarrival;
    if (next_dx>max_interarrival) next_dx=max_interarrival;
    
    //printf("next_dx ======= %f\n",next_dx);
    return next_dx;
}


float generador_random_exponential_for_seed(float lambda,unsigned int *seed) {
    float u;
    u=rand_r(seed)*1.0/RAND_MAX;
    return -log(u)/lambda;
}




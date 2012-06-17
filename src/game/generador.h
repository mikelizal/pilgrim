// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

extern t_bool generador_keep_constant_seed;

// para que la use el generador de zonas
extern unsigned int generador_zone_seed;
extern unsigned int generador_items_seed;


void generador_init(void);

void generador_update(void);

float generador_random_exponential_for_seed(float lambda,unsigned int *seed);

t_pos generador_next_interarrival_with_seed(unsigned int *seed, t_pos interarrival, float increase_factor, t_pos min_interarrival, t_pos max_interarrival);

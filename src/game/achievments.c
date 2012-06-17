// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


#include "pilgrim.h"

#define ACHIEVMENT_DEBUG 0

const char *achievments_name_play1time="play1time";
const char *achievments_name_startplay="startplay";
const char *achievments_name_play100="play100";
const char *achievments_name_reach250="reach250";
const char *achievments_name_reach500="reach500";
const char *achievments_name_reach1000="reach1000";

const char *achievments_name_before100="before100";
const char *achievments_name_downbybike="downbybike";

const char *achievments_name_reach250unfallen="reach250unfallen";
const char *achievments_name_reach500unfallen="reach500unfallen";
const char *achievments_name_reach250fruitonly="reach250fruitonly";

const char *achievments_name_walk250nonviolent="walk250nonviolent";
const char *achievments_name_walk500nonviolent="walk500nonviolent";
const char *achievments_name_walk250panic="walk250panic";



bool achievments_report_play1time=false;
int achievments_reported_startplay=0;
bool achievments_report_reach250=false;
bool achievments_report_reach500=false;
bool achievments_report_reach1000=false;
bool achievments_report_before100=false;
bool achievments_report_downbybike=false;
bool achievments_report_reach250unfallen=false;
bool achievments_report_reach500unfallen=false;
bool achievments_report_reach250fruitonly=false;
bool achievments_report_walk250nonviolent=false;
bool achievments_report_walk500nonviolent=false;
bool achievments_report_walk250panic=false;


bool achievments_play1time=false;
bool achievments_startplay_armed=false;
int achievments_startplay=0;
bool achievments_reach250=false;
bool achievments_reach500=false;
bool achievments_reach1000=false;
bool achievments_before100=false;
bool achievments_downbybike=false;


bool achievments_www_visited;

t_pos achievments_inthisgame_last_fallen;
t_pos achievments_inthisgame_last_nonfruit_eaten;

bool achievments_reach250fruitonly=false;
bool achievments_reach250unfallen=false;
bool achievments_reach500unfallen=false;


bool achievments_walk250nonviolent=false;
bool achievments_walk500nonviolent=false;

bool achievments_walk250panic=false;
bool achievments_inthisgame_inpanicnow=false;
t_pos achievments_inthisgame_initpanic=0.0;
t_pos achievments_inthisgame_lastpos_walker_hit=0.0;






void achievments_check_update(void) {
    if (!achievments_reach1000) {
        if (world_pos_to_m(world.player.x)>1000) {
            achievments_reach1000=true;
            achievments_report_reach1000=true;
#if ACHIEVMENT_DEBUG == 1
            printf("ACHIEVMENT 1000\n");
#endif
        }
        if (!achievments_reach500) {
            if (world_pos_to_m(world.player.x)>500) {
                achievments_reach500=true;
                achievments_report_reach500=true;
#if ACHIEVMENT_DEBUG == 1
                printf("ACHIEVMENT 500\n");
#endif
            }        
            if (!achievments_reach250) {
                //printf("llego a 250?? %f\n",world_pos_to_m(world.player.x));
                if (world_pos_to_m(world.player.x)>250) {
                    achievments_reach250=true;
                    achievments_report_reach250=true;
#if ACHIEVMENT_DEBUG == 1
                    printf("ACHIEVMENT 250\n");
#endif
                }
            }
        }
    }
    
    if (achievments_startplay_armed) {
        if (world_pos_to_m(world.player.x)>10.0) {
            achievments_startplay_armed=false;
            achievments_startplay+=1;
            
            if (!achievments_play1time) {
                achievments_play1time=true;
                achievments_report_play1time=true;
            }
        }
    }

    if (!achievments_walk250panic) {
        if (achievments_inthisgame_inpanicnow && world_pos_to_m(world.player.x-achievments_inthisgame_initpanic)>250.0) {
            achievments_walk250panic=true;
            achievments_report_walk250panic=true;
        }        
    }
    
    if (!achievments_walk500nonviolent) {
        if (world_pos_to_m(world.player.x-achievments_inthisgame_lastpos_walker_hit)>500.0) {
            achievments_walk500nonviolent=true;
            achievments_report_walk500nonviolent=true;
        }
        if (!achievments_walk250nonviolent) {
            if (world_pos_to_m(world.player.x-achievments_inthisgame_lastpos_walker_hit)>250.0) {
                achievments_walk250nonviolent=true;
                achievments_report_walk250nonviolent=true;
            }
        }
    }
    
    if (!achievments_report_reach250fruitonly) {
        if (world_pos_to_m(world.player.x-achievments_inthisgame_last_nonfruit_eaten)>250.0 && achievments_inthisgame_last_nonfruit_eaten<1.0) {
            achievments_reach250fruitonly=true;
            achievments_report_reach250fruitonly=true;
        }
    }
    
    if (!achievments_reach500unfallen) {
        if (world_pos_to_m(world.player.x-achievments_inthisgame_last_fallen)>500.0 && achievments_inthisgame_last_fallen<1.0) {
            achievments_reach500unfallen=true;
            achievments_report_reach500unfallen=true;
        }
        if (!achievments_reach250unfallen) {
            if (world_pos_to_m(world.player.x-achievments_inthisgame_last_fallen)>250.0 && achievments_inthisgame_last_fallen<1.0) {
                achievments_reach250unfallen=true;
                achievments_report_reach250unfallen=true;
            }
        }
    }
    
}


void achievments_hitbybike(void) {
    if (!achievments_downbybike) {
        achievments_report_downbybike=true;
        achievments_downbybike=true;
    }
}

void achievments_inpanicnow(bool inpanic) {
    if (inpanic) {
        if (!achievments_inthisgame_inpanicnow) {
            achievments_inthisgame_initpanic=world.player.x;
#if ACHIEVMENT_DEBUG == 1
            printf("entered panic in %f %f m\n",world.player.x,world_pos_to_m(world.player.x));
#endif
        }
        achievments_inthisgame_inpanicnow=true;
    } else {
        achievments_inthisgame_inpanicnow=false;
    }
}

void achievments_hitwalker(void) {
    achievments_inthisgame_lastpos_walker_hit=world.player.x;
}

void achievments_eaten(t_itemPtr fooditem) {
    if (item_isfruit(fooditem)) {
        return;
    }
    achievments_inthisgame_last_nonfruit_eaten=world.player.x;
}

void achievments_fallen(void) {
    achievments_inthisgame_last_fallen=world.player.x;
}


void achievments_newgame(void) {
    achievments_startplay_armed=true;
    
    achievments_inthisgame_last_fallen=0.0;
    achievments_inthisgame_last_nonfruit_eaten=0.0;
    achievments_inthisgame_lastpos_walker_hit=0.0;
    
    achievments_inthisgame_inpanicnow=false;
    achievments_inthisgame_initpanic=0.0;

}

void achievments_endgame(void) {
    if (!achievments_before100) {
        if (world_pos_to_m(world.player.x)<100.0) {
            achievments_before100=true;
            achievments_report_before100=true;
        }
    }
}


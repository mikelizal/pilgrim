// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

// nombres
extern const char *achievments_name_play1time;
extern const char *achievments_name_startplay;
extern const char *achievments_name_play100;
extern const char *achievments_name_reach250;
extern const char *achievments_name_reach500;
extern const char *achievments_name_reach1000;

extern const char *achievments_name_before100;
extern const char *achievments_name_downbybike;

extern const char *achievments_name_reach250unfallen;
extern const char *achievments_name_reach500unfallen;
extern const char *achievments_name_reach250fruitonly;

extern const char *achievments_name_walk250nonviolent;
extern const char *achievments_name_walk500nonviolent;
extern const char *achievments_name_walk250panic;


// para informar al objective-c de que avise del achievment
extern bool achievments_report_play1time;
extern int achievments_reported_startplay;
extern bool achievments_report_reach250;
extern bool achievments_report_reach500;
extern bool achievments_report_reach1000;
extern bool achievments_report_before100;
extern bool achievments_report_downbybike;
extern bool achievments_report_reach250unfallen;
extern bool achievments_report_reach500unfallen;
extern bool achievments_report_reach250fruitonly;
extern bool achievments_report_walk250nonviolent;
extern bool achievments_report_walk500nonviolent;
extern bool achievments_report_walk250panic;


// para lleva la cuenta internamente de si el achievment se cumple o se ha cumplido
extern bool achievments_play1time;
extern int achievments_startplay;
extern bool achievments_reach250;
extern bool achievments_reach500;
extern bool achievments_reach1000;
extern bool achievments_before100;
extern bool achievments_downbybike;

extern bool achievments_www_visited;

extern t_pos achievments_inthisgame_last_fallen;
extern t_pos achievments_inthisgame_last_nonfruit_eaten;


extern bool achievments_reach250fruitonly;
extern bool achievments_reach250unfallen;
extern bool achievments_reach500unfallen;


extern bool achievments_walk250nonviolent;
extern bool achievments_walk500nonviolent;
//extern t_pos achievments_inthisgame_lastwalkerhit;

extern bool achievments_walk250panic;
extern bool achievments_inthisgame_inpanicnow;
extern t_pos achievments_inthisgame_initpanic;
extern t_pos achievments_inthisgame_lastpos_walker_hit;



void achievments_check_update(void);

void achievments_hitbybike(void);
void achievments_inpanicnow(bool inpanic);
void achievments_hitwalker(void);
void achievments_eaten(t_itemPtr fooditem);
void achievments_fallen(void);

void achievments_newgame(void);
void achievments_endgame(void);


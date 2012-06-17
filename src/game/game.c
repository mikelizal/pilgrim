// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


#include "pilgrim.h"


int game_salto_pressed=0;
int game_hit_pressed=0;
int game_stop_pressed=0;
int game_esquivar_pressed=0;
int game_other_pressed=0;
int game_other2_pressed=0;
t_time game_dt=0.01;
t_time game_music_keepalive_dt=10.0;
t_time game_last_music_keepalive=0.0;

int game_playing=0;
int game_paused=0;
t_time game_paused_time=0.0;

// para informar de si necesito los controles de juego o los de menu

int game_init_game_controls=0;
int game_init_menu_controls=0;
int game_init_clicktocontinue_controls=0;
int game_init_pause_controls=0;
int game_init_backandfw_help_controls=0;
int game_init_backandlink_credits_controls=0;
int game_init_backandgamecenterscores_controls=0;

#if PILGRIM_INCLUDE_WEBLINK
int game_jump_to_weblink=0;
#endif

// int game_show_credits=0;

int game_achievment_report_play1time=0;
int game_achievment_play1time_reported=0;

int game_show_name_dialog=0;
int game_highscore_name_entered=0;
char game_highscore_name[HIGHSCORE_NAME_MAXL];
int game_highscore_pos=-1;
int game_last_score=0;

int game_enter_gamecenter_hof=0;
int game_enter_gamecenter_achievments=0;
int game_report_score_to_gamecenter=0;

//int game_show_credits=0;

bool game_gamecenter_available=false;


// para los fps
int game_fps_n=0;
t_time game_fps_last_t=0;
t_time game_fps_dt=1.0; // promedia cada segundo
float game_fps=0;


t_pos game_last_dead=-1000.0;


void game_init(void) {
    assets_load("assetsx2.lst");
    highscore_load();
    persistence_load();
    render_init();
    
    game_playing=0;
    menu_init();
    
// Ahora se inicializa el mundo cada vez que empieza la partida
//    world_init();
//    time_init();
	
//    printf("game initiated\n");
    audio_start_music(AUDIO_MUSIC_MENU);

}


void game_update(void) {
    // para que aunque se quite la pausa este update funcione como estaba la pausa al empezar
    t_bool paused_this_update=game_paused;
    t_time tnow;

    tnow=time_now();

    if (! game_playing) {
        menu_update(tnow);
        return;
    }
    
    if (game_other_pressed) {
        if (!game_paused) {
            game_pause();
        } else {
            audio_stop_music();
            game_end();
            game_paused=0;
        }
    }

    if (game_other2_pressed && game_paused) {
        game_paused=0;
        game_init_game_controls=1;
        time_new_ref(game_paused_time);
        game_salto_pressed=0;
    }
    
	if (tnow>game_fps_last_t+game_fps_dt) {
        game_fps=game_fps_n/(tnow-game_fps_last_t);
        game_fps_last_t=tnow;
        game_fps_n=0;
    }
    
    if (!paused_this_update) {
        while (tnow>world.t+game_dt) {
            world_update_dt();
        }
	}
        
    render();
    game_fps_n+=1;

    if (tnow-game_last_music_keepalive>game_music_keepalive_dt) {
        game_last_music_keepalive=tnow;
        audio_keep_music();
    }
    
}


void game_start(void) {
    game_playing=1;
    game_paused=0;
    audio_stop_music();
    audio_start_music(AUDIO_MUSIC_COMIENZO);
    game_init_game_controls=1;
    world_init();
    audio_restart();
    time_init();
    stats_reset();
    achievments_newgame();
    
    if (!game_achievment_play1time_reported) {
        game_achievment_play1time_reported=1;
        game_achievment_report_play1time=1;
    }
}


void game_pause(void) {
    if (!game_playing) return;
    if (game_paused) return;
    t_time tnow=time_now();
    game_paused=1;
    game_paused_time=tnow;
    game_init_pause_controls=1;   
}

void game_end(void) {
    game_playing=False;
    achievments_endgame();
    
    if (!game_paused) {
        game_last_dead=world.player.x;
        game_last_score=(int)world_pos_to_m(world.player.x-world_pos0);
    
        game_highscore_pos=highscore_enter_pos(game_last_score);
        
        persistence_total_played_games+=1;
        persistence_save();
//        stats_print();
    
        text_results_gentexture(1024,1024);
    } else {
        game_highscore_pos=-1;
    }

    if (game_highscore_pos==-1) {
        // no hay record ve al menu
        audio_start_music(AUDIO_MUSIC_MENU);
        if (game_paused) {
            menu_init();
        } else {
            menu_results();
        }
    } else {
        // inserta nombre para el record
        //menu_results();
        
        menu_enter_hof();
    }
    
    game_report_score_to_gamecenter=1;
    
}



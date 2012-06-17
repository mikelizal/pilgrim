// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file 
/// \brief Interfaz principal del juego con el programa principal
///
/// Funciones para controlar, iniciar y parar el juego.
/// El programa principal es responable de llamar a game_update frecuentemente


/// \name Entrada para controles
/// poniendo valores a estas variables se informa a pilgrim de que estan activados los controles
//@{
extern int game_salto_pressed; /// salto
extern int game_hit_pressed; /// golpear
extern int game_stop_pressed; /// parar
extern int game_esquivar_pressed; /// esquivar
extern int game_other_pressed; /// otro para pausa y similares
extern int game_other2_pressed; /// otro para pausa y similares
//@}

/// dt para la evaluacion de la fisica
extern t_time game_dt;

/// medida de fps
extern float game_fps;


/// 
extern t_time game_music_keepalive_dt;

// hace falta que render lo vea para dibujar el estado en pausa
extern int game_paused;


extern int game_init_game_controls;
extern int game_init_menu_controls;
extern int game_init_clicktocontinue_controls;
extern int game_init_pause_controls;
extern int game_init_backandfw_help_controls;
extern int game_init_backandlink_credits_controls;
extern int game_init_backandgamecenterscores_controls;


#if PILGRIM_INCLUDE_WEBLINK
extern int game_jump_to_weblink;
#endif

//extern int game_show_credits;

extern int game_show_name_dialog;
extern int game_highscore_name_entered;
extern char game_highscore_name[];
extern int game_highscore_pos;
extern int game_last_score;

extern int game_enter_gamecenter_hof;
extern int game_enter_gamecenter_achievments;
extern int game_report_score_to_gamecenter;

//extern int game_show_credits;


//extern int game_achievment_report_play1time;
//extern int game_achievment_play1time_reported;

extern bool game_gamecenter_available;


extern t_pos game_last_dead;

/// \name Interfaz del juego
///
/// \brief Interfaz de control del juego desde el programa principal
//@{
/// inicializar el juego
///
/// se llama a esta funcion para inicializar todo lo necesario para el juego
/// aun no tengo decidido si el juego es una partida o todo el juego como aplicacion con menu y demas
void game_init(void);

/// actualizar el juego
///
/// Debe ser llamada frecuentemente desde el programa principal para que funcione la fisica y el dibujo
void game_update(void);
//@}



/// Evento del menu que pide empezar
void game_start(void);

void game_pause(void);

/// Evento del juego que pide acabar
void game_end(void);

/// Si el juego esta en menu se deriva el update a esta funcion para que dibuje el menu y decida sobre el
void game_update_en_menu(void);

// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para manejar los sprites del juego. Incluyendo toda la informacion sobre la animacion de los personajes


#define SPRCLS_ID_PLAYER 0
#define SPRCLS_ID_PEREGRINO 1
#define SPRCLS_ID_BORDONERO 2
#define SPRCLS_ID_CANSADO 3
#define SPRCLS_ID_BICI 4

#define STID_PLAYER_WALK 0
#define STID_PLAYER_STOP 4
#define STID_PLAYER_PREPARINGHIT 5
#define STID_PLAYER_HIT 6
#define STID_PLAYER_PREPARINGJUMP 7
#define STID_PLAYER_JUMP 8
#define STID_PLAYER_FALL 9
#define STID_PLAYER_FALLEN 10
#define STID_PLAYER_SIDE 11
#define STID_PLAYER_TIRED 12

#define STID_WALKER_WALK 0
#define STID_WALKER_STOP 4
#define STID_WALKER_RECEIVING_HIT 5
#define STID_WALKER_RECEIVED_HIT 6


// el estado de animacion incluye un id, un tiempo en el que se esta en ese estado, la descripcion del grafico y el siguiente estado al que se pasa, o una funcion que lo decide o algo asi
struct _anim_state {
    t_id id;
    t_bool auto_change;
    t_bool duration_depends_on_v;
    t_time duration;
    t_pos paso; // distancia que tiene que durar la animacion total  este frame dura duration*paso
    t_id next_state_id;
    t_bool post_state_action;
    
    t_bb out_bounds; // limites
    t_bb col_bounds; // limites para colision
    
    // otras cosas que utilice el motor de juego para dibujar...
    // ids de textura
    // vertices precalculados cosas asi
    
    GLuint tex_id;
    GLfloat vertex_data[8];
    GLfloat tex_data[8];

    // provisional para tener solo algunos sprites que se renderizen con texturas
    t_bool tex_ready;
    
};

typedef struct _anim_state t_anim_state, *t_anim_statePtr;

// tipo general de personaje
// como jugador, peregrino, bordonero ...
// tendra datos sobre su animacion y cosas generales
struct _sprite_class {
    t_id id;
    int n_anim_states;
    
    t_anim_state animstate[1]; // mucho esfuerzo... podrian tenerse todos los animstates por ahi y no en el sprite_class simplificaria
};

typedef struct _sprite_class t_sprite_class, *t_sprite_classPtr;

// un personaje concreto
// tendra datos sobre su posicion y el estado de animacion
struct _sprite {
    t_bool is_player;
    t_pos x,y; // la x se entiende que es con relacion a otra cosa por ejemplo al jugador
    t_v vx,vy;
    int n_hits; // vida = golpes que aguanta antes de apartarse
    t_sprite_classPtr sprite_class;
    t_anim_statePtr anim_state;
    t_id anim_state_id;
    t_time anim_lastT; // tiempo en el que se entro en el estado actual de la animacion
};

typedef struct _sprite t_sprite,*t_spritePtr;


// Array con los tipos de personajes
extern t_sprite_classPtr *sprite_classes; 

// utility functions
t_sprite_classPtr alloc_sprite_class(int nanim);
void sprite_init_anim_state(t_anim_statePtr astate,t_id aid,t_bool auto_change,t_time duration,t_bool depends_on_v,t_id nextid, t_bool post_state_action);
void sprite_anim_state_set_bounds(t_anim_statePtr astate,t_pos out_w,t_pos out_h,t_pos out_ox,t_pos out_oy,t_pos col_w,t_pos col_h,t_pos col_ox,t_pos col_oy);
void sprite_anim_state_readimagedata(t_anim_statePtr astate,int img_id);

// to be called from outside
void sprite_classes_init(void);  // genera todos los sprite_classes del juego


void sprite_update_pos(t_spritePtr spr);
void sprite_update_v(t_spritePtr spr);
void sprite_update_anim(t_spritePtr spr);

void sprite_update(t_spritePtr spr);

void sprite_test_print_sprite_class_tree(void);

void sprite_set_state(t_spritePtr spr,int state_id);

//test
void sprite_test_print_sprite_class_tree();


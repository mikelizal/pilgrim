// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para mantener y actualizar el estado del mundo en el juego


/// Maximo numero de sprites enemigos para las estructuras estaticas
#define MAX_N_SPRITES 30

/// Maximo numero de items para las estructuras estaticas
#define MAX_N_ITEMS 10



#define ZONE_NLEVELS 2



extern t_pos world_pos0;
t_time world_t0;


struct _world_zone {
    int i[ZONE_NLEVELS];
    // factor de reduccion de velocidad el nivel del jugador tiene 1
    // el de mas atras tiene mas por ejemplo 2=cada 2 pixeles avanza 1
    t_pos prueba_h;
    int arbol_present[4]; // 0 no 1 tipo 1 2 tipo 2
    int arbol_pos[4]; // desde el principio de la zona
    
    int back_colina_pos[2];
};

typedef struct _world_zone t_world_zone,*t_world_zonePtr;



/// Estado del mundo del juego
struct _world {
    /// Tiempo actual
    t_time t;

    t_bool waiting_to_start;
    
    /// anchura de zona
    ///
    /// deberia ser al menos de igual tamaño que la pantalla
    t_pos zone_width;
    float zone_level_factor[ZONE_NLEVELS];
    // actual zone index
    int i_zone;
    /// zonas del mundo generadas
    /// 
    /// 3 posiciones para mantener siempre la acutual la siguiente o la anterior
    /// el id de la zona es (int)(player_x/zone_width) y se pone en la casilla id%3
    t_world_zone zone[3];
    
    
    /// \name Sprites e items en el mundo
    //@{
    /// sprite del juegador 
    t_sprite player;
    
    /// \name Caracteristicas del jugador
    //@{
    /// vida para probar
    float player_vida;
    /// siguiente checkpoint
    float player_checkpoint;
    /// tiempo restante
    float player_deadline;
    /// invulnerabilidad momentanea
    t_time player_untouchable_time;
    
    
    //@}
    
    
    /// Array con todos los demas sprites
    t_sprite sprites[MAX_N_SPRITES];
    /// Array que indica los sprites validos 
    
    /// si no es valido se puede reutilizar cuando haga falta crear un sprite
    t_bool sprite_valid[MAX_N_SPRITES];
    /// numero de sprites validos
    int sprites_n;
    
    
    /// Array con los items
    t_item items[MAX_N_ITEMS];
    /// Array que indica los items validos
    
    /// si no es valido se puede reutilizar cuando haga falta crear un item
    t_bool item_valid[MAX_N_ITEMS];
    /// numero de items validos
    int items_n;
    
    //@}
    
//    /// \name En desuso
//    //@{
//    // probando para impaciencia del player y que golpee automatico
//    int player_slow;
//    t_time player_slow_since;
//    //@}
    
//    /// \name Estado para los generadores
//    //@{
//    t_time time_next_piligrim;
//    t_time time_next_cansado;
//    t_time time_next_bici;
//    //@}
    
    /// \name camera postion (coordenada de la izquierda de la pantalla es player_x+cam_x )
    //@{
    t_pos cam_x;
    t_v cam_v;
    //@}
};


// adelantada esta definicion para poder usarla en item
//typedef struct _world t_world,*t_worldPtr;




struct _sprite_iterator {
	int i;	
};

typedef struct _sprite_iterator t_sprite_iterator, *t_sprite_iteratorPtr;


extern t_v world_g;
extern t_v world_walker_v;


extern t_world world;


/// \name Funciones de conversion de coordenadas
/// 
/// 3 sistemas de coordenadas  <br>
///    world - desde el principio del camino <br>
///    player - desde el player  <br>
///    screen - desde la izquierda de la pantalla <br>
//@{
t_pos world_player_coords_to_screen_coords(t_pos player_coord_x);
t_pos world_screen_coords_to_player_coords(t_pos screen_coord_x);
t_pos world_player_coords_to_world_coords(t_pos player_coord_x);
t_pos world_world_coords_to_player_coords(t_pos world_coord_x);
t_pos world_world_coords_to_screen_coords(t_pos world_coord_x);
t_pos world_screen_coords_to_world_coords(t_pos screen_coord_x);
//@}


/// Inicializa el mundo para una partida
void world_init(void);

/// Da el paso de un dt del motor del juego
void world_update_dt(void);

t_pos world_zone_getscreenpos(int level, int i);
void world_update_zones(void);
void world_update_zone_if_needed(int level, int izone);

t_pos world_get_left_absolute_x(void);

t_pos world_pos_to_m(t_pos pos);

t_pos world_m_to_pos(t_pos m);

// lista de sprites
void world_zero_sprites(void);
void world_iterate_sprites(t_sprite_iteratorPtr iter);
t_spritePtr world_iterate_next(t_sprite_iteratorPtr iter);
void world_delete_sprite(int i);
t_spritePtr world_create_sprite(void);
void world_create_walker (t_pos position_x, int tipo);



void world_post_animation_action(t_spritePtr spr, t_id fromid, t_id toid);
void world_handle_collision_candidate(t_spritePtr player, t_spritePtr otro);
t_bool world_collision_check(t_spritePtr player, t_spritePtr otro);
t_bool world_nonplayer_collision_check(t_spritePtr uno, t_spritePtr otro);
void world_handle_collision(t_spritePtr player, t_spritePtr otro);

t_bool world_item_collision_check(t_spritePtr player, t_itemPtr otro);

void world_gain_life(float g);
void world_lose_life(float l);





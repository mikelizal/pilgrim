// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para renderizar

#define LABEL_NUM_BASE 0

struct _assets_tile_info;


extern int render_ipad_res;

extern float render_display_w;
extern float render_display_h;


void render_set_lowres(void);

/// Inicializa el renderizado en la partida
///
/// para opciones de opengl que se mantengan
void render_init(void);

/// Dibuja el estado del juego en el instante actual
void render(void);

/// Renderiza la posicion de la camara
void render_cam(float width, float height);

void render_flat_tile(float x, float y, float w, float h);

/// Renderiza un cuadrado 
void render_tile(float x, float y, struct _assets_tile_info *tile);

/// renderiza un tile
void render_tile_num(float x, float y, int tile_num);

/// renderiza una textura
void render_texture_rect(float x, float y, GLuint tex_id, GLfloat *vertex_data, GLfloat *tex_data);


/// renderiza una etiqueta
void render_label_num(float x, float y, int label_num);

/// renderiza una cadena de numeros
void render_numstr(float x, float y, char *numstr, int pos);

/// Renderiza un cuadrado
void render_box(float x, float y, float rx, float ry);

/// Renderiza el fondo
void render_background(void);

// Renderiza una montaña
void render_mountain(float x, float y, float w, float h);

// Renderiza un sprite
void render_sprite(t_spritePtr spr);

void render_item(t_itemPtr item);

void render_tileframe(float x, float y, float w, float h);

void render_back_zone(int level,int i);

// Renderiza el mundo... llama a las anteriores
void render_world(void);

void render_marcador_linea(GLfloat x0, GLfloat y0, GLfloat xmax, GLfloat normalized_value);

void render_marcadores(void);

void render_menu(void);

void render_clean_bocina(void);
void render_show_bocina(void);
void render_bocina(void);



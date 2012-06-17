// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para gestionar los recursos del juego




#define AST_TILE_BOCINA 0
#define AST_TILE_CASAS1 1
#define AST_TILE_ARBOL1 2
#define AST_TILE_ARBOL2 3
#define AST_TILE_FONDO 4
#define AST_TILE_SUELO 5
#define AST_TILE_NUBE1 6
#define AST_TILE_NUBE2 7
#define AST_TILE_NUBE3 8
#define AST_TILE_BTN_ESQUIVAR 9
#define AST_TILE_BTN_PEGAR 10
#define AST_TILE_BTN_SALTAR 11
#define AST_TILE_BTN_ANDAR 12
#define AST_TILE_MARCADOR 13
#define AST_TILE_VIDA 14
#define AST_TILE_BTN_PAUSE 15
#define AST_TILE_BTN_NEXT 16
#define AST_TILE_BTN_BACK 17
#define AST_TILE_BTN_RESUME 18
#define AST_TILE_BTN_QUIT 19
#define AST_TILE_ICN_DISTANCE 20 
#define AST_TILE_ICN_TIME 21
#define AST_TILE_ICN_SPEED 22
#define AST_TILE_ICN_FALLS 23
#define AST_TILE_ICN_PEREGRINO_HIT 24 
#define AST_TILE_ICN_BORDONERO_HIT 25
#define AST_TILE_ICN_FRUTA 26
#define AST_TILE_ICN_CARNE 27
#define AST_TILE_MENU_PAUSE 28
#define AST_TILE_ICN_SND_ON 29
#define AST_TILE_ICN_SND_OFF 30
#define AST_TILE_BTN_GC_LEADERBOARDS 31
#define AST_TILE_BTN_GC_ACHIEVMENTS 32
#define AST_TILE_MENU_FONDO 33
#define AST_TILE_SPLASH 34
#define AST_TILE_MENU_PHIL 35
#define AST_TILE_INSTRUCCIONES2 36
#define AST_TILE_INSTRUCCIONES3 37
#define AST_TILE_MENU_TITLE 38
#define AST_TILE_MENU_PLAY 39
#define AST_TILE_MENU_HELP 40
#define AST_TILE_MENU_HIGHSCORES 41
#define AST_TILE_MENU_CREDITS 42
#define AST_TILE_GO 43
#define AST_TILE_READY 44
#define AST_TILE_GAMEOVER 45
#define AST_TILE_CIRCLE 46
#define AST_TILE_INSTRUCCIONES1 47
#define AST_TILE_HOF 48
#define AST_TILE_FONDO2 49
#define AST_TILE_FONDO_TRANS12 50
#define AST_TILE_FONDO_TRANS21 51


/// Estructura con informacion de una imagen cargada de fichero.

/// Una imagen es un trozo de textura con rectangulo de colision para un sprite
/// Esta información solo se mantiene dentro de la funcion assets_load() 
/// y contiene la informacion de las imagenes hasta que se crean los sprites
/// y almacenan todas las caracteristicas necesarias
struct _assets_image_info {
    int tex_id;
    int tex_container_w,tex_container_h;
    t_pos r_x,r_y,r_w,r_h;
    t_pos off_x,off_y;
    t_pos col_w,col_h,col_off_x, col_off_y;
};
typedef struct _assets_image_info t_assets_image_info, *t_assets_image_infoPtr;

/// Estructura con informacion de un fondo cargado de fichero.

/// Un tile es un trozo de una textura sin rectangulo de colision ni offsets
struct _assets_tile_info {
    int tex_id;
    //int tex_container_w,tex_container_h;
    t_bb out_bounds;
    GLfloat vertex_data[8];
    GLfloat tex_data[8];
};
typedef struct _assets_tile_info t_assets_tile_info, *t_assets_tile_infoPtr;

/// Array de imagenes leidas de fichero

/// Es un contenedor provisional con el se construyen los sprite_classes
/// una vez construido el array de imagenes se destruye
/// Solo existe durante la llamada a assets_load
extern t_assets_image_infoPtr assets_images;

/// Array de tiles leidas de fichero

/// Si que se mantiene una vez creado
extern t_assets_tile_infoPtr assets_tiles;

/// Array de labels leidas de fichero

/// Si que se mantiene una vez creado
extern t_assets_tile_infoPtr assets_labels;



/// Establece el resourcepath donde estan los ficheros con recursos

/// Es llamada desde el programa principal que puede calcular el path del bundle
void assets_setresourcepath(const char *resourcepath);

/// Carga todos los assets del juego

/// Lee el fichero assets.lst interpretando las lineas y cargando los ficheros que pide
t_bool assets_load(const char *filename);

/// Carga una textura de fichero y genera la textura opengl

/// libera todos los recursos usados
int assets_gettexture(const char *filename,int *containerw,int *containerh);

/// Carga un fichero de audio y lo pone el sndbufer de openal especificado
//void assets_getaudiofile(char *filename,ALuint sndbuffer);
void assets_getaudiofile(const char *filename,unsigned int sndbuffer);




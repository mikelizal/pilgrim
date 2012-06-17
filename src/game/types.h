// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief definiciones de tipos basicos



/// tipo para los booleanos
typedef int t_bool;
#define True 1
#define False 0


/// tipo para el tiempo
typedef float t_time;
// funciones para el tiempo del sistema en time.h

/// tipo para las coordenadas espaciales
typedef float t_pos;

/// tipo para las velocidades
typedef float t_v;

/// tipos para id numerico en general
typedef int t_id;

/// tipo para rectangulos de borde bounding box
struct _bb {
    t_pos w,h; /// anchura y altura
    t_pos off_x,off_y; /// offset del centro de coordenadas del sprite respecto al borde izquierdo abajo
};

typedef struct _bb t_bb, *t_bbPtr;


/// generador exponencial
/// provisional probablemente lo pondre en otro sitio
t_time random_exp_time(t_time average);




void generate_vertex_data_from_bb(GLfloat *vertex_data, t_bb *bounds);
void generate_texcoord_data_from_rect(GLfloat *texcoord_data, GLfloat r_x, GLfloat r_y, GLfloat r_w, GLfloat r_h, int texcontainer_w, int texcontainer_h);
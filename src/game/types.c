// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


#include "pilgrim.h"


//t_time random_exp_time(t_time average) {
//    float u;
//    u=rand()*1.0/RAND_MAX;
//    return -log(u)*average;
//}




/// Genera un array de datos de vertices para opengl para un bounding box
void generate_vertex_data_from_bb(GLfloat *vertex_data, t_bb *bounds) {
    GLfloat min_x,max_x,min_y,max_y;
    min_x=-bounds->off_x;
    max_x=bounds->w+min_x;
    min_y=-bounds->off_y;
    max_y=bounds->h+min_y;
    vertex_data[0]=min_x;
    vertex_data[1]=min_y;
    vertex_data[2]=min_x;
    vertex_data[3]=max_y;
    vertex_data[4]=max_x;
    vertex_data[5]=min_y;
    vertex_data[6]=max_x;
    vertex_data[7]=max_y;
}


/// Genera un array de datos de coordenadas de textura para opengl para una subimagen
void generate_texcoord_data_from_rect(GLfloat *texcoord_data, GLfloat r_x, GLfloat r_y, GLfloat r_w, GLfloat r_h, int texcontainer_w, int texcontainer_h) {
    GLfloat min_x,max_x,min_y,max_y;
    min_x=r_x;
    min_y=r_y;
    max_x=r_w+min_x;
    max_y=r_h+min_y;
    texcoord_data[0]=min_x/texcontainer_w;
    texcoord_data[1]=max_y/texcontainer_h;
    texcoord_data[2]=min_x/texcontainer_w;
    texcoord_data[3]=min_y/texcontainer_h;
    texcoord_data[4]=max_x/texcontainer_w;
    texcoord_data[5]=max_y/texcontainer_h;
    texcoord_data[6]=max_x/texcontainer_w;
    texcoord_data[7]=min_y/texcontainer_h;
}

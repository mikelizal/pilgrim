// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


t_sprite_classPtr *sprite_classes=NULL;

// alloc a sprite_class with n animation states
t_sprite_classPtr alloc_sprite_class(int nanim) {
    t_sprite_classPtr result;
    result=malloc(sizeof(t_sprite_class)+(nanim-1)*sizeof(t_anim_state));
    
    result->n_anim_states=nanim;
    
    return result;
}



void sprite_init_anim_state(t_anim_statePtr astate,t_id aid,t_bool auto_change,t_time duration,t_bool depends_on_v,t_id nextid, t_bool post_state_action) {
    astate->id=aid;
    astate->auto_change=auto_change;
    astate->duration_depends_on_v=depends_on_v;
    astate->duration=duration;
    astate->next_state_id=nextid;
    astate->post_state_action=post_state_action;
    // cosas a añadir...
}

void sprite_anim_state_set_bounds(t_anim_statePtr astate,t_pos out_w,t_pos out_h,t_pos out_ox,t_pos out_oy,t_pos col_w,t_pos col_h,t_pos col_ox,t_pos col_oy) {
    astate->out_bounds.w=out_w;
    astate->out_bounds.h=out_h;
    astate->out_bounds.off_x=out_ox;
    astate->out_bounds.off_y=out_oy;
    astate->col_bounds.w=col_w;
    astate->col_bounds.h=col_h;
    astate->col_bounds.off_x=col_ox;
    astate->col_bounds.off_y=col_oy;
    
    astate->tex_ready=False;
}

void sprite_anim_state_readimagedata(t_anim_statePtr astate,int img_id) {
    t_assets_image_infoPtr img_info;
    img_info=&(assets_images[img_id]);

    astate->out_bounds.w=img_info->r_w;
    astate->out_bounds.h=img_info->r_h;
    astate->out_bounds.off_x=img_info->off_x;
    astate->out_bounds.off_y=img_info->off_y;
    astate->col_bounds.w=img_info->col_w;
    astate->col_bounds.h=img_info->col_h;
    astate->col_bounds.off_x=img_info->col_off_x;
    astate->col_bounds.off_y=img_info->col_off_y;
    
    // generar vertex y texture arrays
//    GLfloat min_x,max_x,min_y,max_y;
//    min_x=-img_info->off_x;
//    max_x=img_info->r_w+min_x;
//    min_y=-img_info->off_y;
//    max_y=img_info->r_h+min_y;
//    astate->vertex_data[0]=min_x;
//    astate->vertex_data[1]=min_y;
//    astate->vertex_data[2]=min_x;
//    astate->vertex_data[3]=max_y;
//    astate->vertex_data[4]=max_x;
//    astate->vertex_data[5]=min_y;
//    astate->vertex_data[6]=max_x;
//    astate->vertex_data[7]=max_y;
    // ahora con funcion
    generate_vertex_data_from_bb(astate->vertex_data,&(astate->out_bounds));
        
    GLfloat tex_container_w=img_info->tex_container_w;
    GLfloat tex_container_h=img_info->tex_container_h;
    
//    min_x=img_info->r_x;
//    min_y=img_info->r_y;
//    max_x=img_info->r_w+min_x;
//    max_y=img_info->r_h+min_y;
//    astate->tex_data[0]=min_x/tex_container_w;
//    astate->tex_data[1]=max_y/tex_container_h;
//    astate->tex_data[2]=min_x/tex_container_w;
//    astate->tex_data[3]=min_y/tex_container_h;
//    astate->tex_data[4]=max_x/tex_container_w;
//    astate->tex_data[5]=max_y/tex_container_h;
//    astate->tex_data[6]=max_x/tex_container_w;
//    astate->tex_data[7]=min_y/tex_container_h;
    // ahora con funcion
    generate_texcoord_data_from_rect(astate->tex_data,img_info->r_x,img_info->r_y,img_info->r_w,img_info->r_h,tex_container_w,tex_container_h);
    
    
    astate->tex_id=img_info->tex_id;
    
    astate->tex_ready=True;
}


// para ser llamada desde assets_load con la lista de imagenes creada en assets_images
void sprite_classes_init(void) {
    t_sprite_classPtr *result;
    t_sprite_classPtr unspriteclass;
    int n_sprite_classes=5;
    int n_anim_states;
    result=malloc(sizeof(t_sprite_classPtr)*n_sprite_classes);
    
    
    n_anim_states=13;
    // player
    result[SPRCLS_ID_PLAYER]= ( unspriteclass=alloc_sprite_class(n_anim_states) );
    unspriteclass->id=SPRCLS_ID_PLAYER;
    sprite_init_anim_state(&(unspriteclass->animstate[0]), 0, True, 0.3, True, 1, False);  // andando 0
    unspriteclass->animstate[0].paso=400.0;
    sprite_init_anim_state(&(unspriteclass->animstate[1]), 1, True, 0.2, True, 2, False);  // andando 1
    unspriteclass->animstate[1].paso=400.0;
    sprite_init_anim_state(&(unspriteclass->animstate[2]), 2, True, 0.3, True, 3, False);  // andando 2
    unspriteclass->animstate[2].paso=400.0;
    sprite_init_anim_state(&(unspriteclass->animstate[3]), 3, True, 0.2, True, 0, False);  // andando 3
    unspriteclass->animstate[3].paso=400.0;
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[0]), 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[1]), 1);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[2]), 2);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[3]), 3);    
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[0]), 48, 48, 24, 0, 40, 48, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[1]), 48, 48, 24, 0, 40, 48, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[2]), 48, 48, 24, 0, 40, 48, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[3]), 48, 48, 24, 0, 40, 48, 20, 0);
    sprite_init_anim_state(&(unspriteclass->animstate[4]), 4, False, 1.0, False, 0, False);  // parado
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[4]), 48, 48, 24, 0, 40, 48, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[4]), 11);
    sprite_init_anim_state(&(unspriteclass->animstate[5]), 5, True, .2, False, 6, True);  // preparando para pegar
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[5]), 40, 52, 24, 0, 40, 48, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[5]), 4);
    sprite_init_anim_state(&(unspriteclass->animstate[6]), 6, True, .2, False, 0, True);  // pegando
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[6]), 60, 40, 24, 0, 60, 48, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[6]), 5);

    sprite_init_anim_state(&(unspriteclass->animstate[7]), 7, True, 0.1, False, 8, True);  // preparando salto
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[7]), 48, 38, 24, 0, 40, 38, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[7]), 8);
    sprite_init_anim_state(&(unspriteclass->animstate[8]), 8, False, 0, False, 0, False);  // salto
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[8]), 52, 52, 24, 0, 40, 40, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[8]), 9);
    
    sprite_init_anim_state(&(unspriteclass->animstate[9]), 9, False, 0, False, 0, False);  // cayendo
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[9]), 60, 60, 24, 0, 50, 50, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[9]), 13);
    sprite_init_anim_state(&(unspriteclass->animstate[10]), 10, True, 2.0, False, 0, True);  // en el suelo
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[10]), 52, 30, 24, 0, 52, 30, 24, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[10]), 14);

    sprite_init_anim_state(&(unspriteclass->animstate[11]), 11, False, 1.0, False, 0, False);  // apartado
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[11]), 48, 48, 24, -5, 40, 48, 24, -5);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[11]), 7);

    sprite_init_anim_state(&(unspriteclass->animstate[12]), 12, True, 6.0, False, 12, True);  // cansado
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[12]), 6);
    
    
    
    n_anim_states=6;
    // peregrino
    result[SPRCLS_ID_PEREGRINO]= ( unspriteclass=alloc_sprite_class(n_anim_states) );
    unspriteclass->id=SPRCLS_ID_PEREGRINO;
    sprite_init_anim_state(&(unspriteclass->animstate[0]), 0, True, 0.3, True, 1, False);  // andando 0
    unspriteclass->animstate[0].paso=240.0;
    sprite_init_anim_state(&(unspriteclass->animstate[1]), 1, True, 0.2, True, 2, False);  // andando 1
    unspriteclass->animstate[1].paso=240.0;
    sprite_init_anim_state(&(unspriteclass->animstate[2]), 2, True, 0.3, True, 3, False);  // andando 2
    unspriteclass->animstate[2].paso=240.0;
    sprite_init_anim_state(&(unspriteclass->animstate[3]), 3, True, 0.2, True, 0, False);  // andando 3
    unspriteclass->animstate[3].paso=240.0;
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[0]), 16);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[1]), 17);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[2]), 18);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[3]), 19);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[0]), 48, 80, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[1]), 48, 80, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[2]), 48, 80, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[3]), 48, 80, 24, 0, 40, 80, 20, 0);
    sprite_init_anim_state(&(unspriteclass->animstate[4]), 4, False, 0, False, 0, False);  // apartado
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[4]), 48, 70, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[4]), 28);
    sprite_init_anim_state(&(unspriteclass->animstate[5]), 5, True, .6, False, 4, False);  // golpeado
    //    sprite_anim_state_set_bounds(&(unspriteclass->animstate[4]), 48, 70, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[5]), 27);

    
    n_anim_states=7;
    // bordonero
    result[SPRCLS_ID_BORDONERO]= ( unspriteclass=alloc_sprite_class(n_anim_states) );
    unspriteclass->id=SPRCLS_ID_BORDONERO;
    sprite_init_anim_state(&(unspriteclass->animstate[0]), 0, True, 0.3, True, 1, False);  // andando 0
    unspriteclass->animstate[0].paso=320.0;
    sprite_init_anim_state(&(unspriteclass->animstate[1]), 1, True, 0.2, True, 2, False);  // andando 1
    unspriteclass->animstate[1].paso=320.0;
    sprite_init_anim_state(&(unspriteclass->animstate[2]), 2, True, 0.3, True, 3, False);  // andando 2
    unspriteclass->animstate[2].paso=320.0;
    sprite_init_anim_state(&(unspriteclass->animstate[3]), 3, True, 0.2, True, 0, False);  // andando 3
    unspriteclass->animstate[3].paso=320.0;
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[0]), 48, 90, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[1]), 48, 90, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[2]), 48, 90, 24, 0, 40, 80, 20, 0);
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[3]), 48, 90, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[0]), 21);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[1]), 22);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[2]), 23);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[3]), 24);    
    sprite_init_anim_state(&(unspriteclass->animstate[4]), 4, False, 0, False, 0, False);  // apartado
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[4]), 48, 70, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[4]), 26);
//    sprite_init_anim_state(&(unspriteclass->animstate[5]), 5, True, .3, False, 6, False);  // 1 hit impegable 
    sprite_init_anim_state(&(unspriteclass->animstate[5]), 5, True, .5, False, 0, True);  // 1 hit impegable 
    // el tiempo de este 0.3 tiene que ser mayor que el tiempo pegando pero menor que el tiempo pegando+t preparando pegar
//    sprite_anim_state_set_bounds(&(unspriteclass->animstate[5]), 48, 80, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[5]), 25);
    sprite_init_anim_state(&(unspriteclass->animstate[6]), 6, True, .9, False, 0, True);  // 1 hit pegable
    //    sprite_anim_state_set_bounds(&(unspriteclass->animstate[5]), 48, 80, 24, 0, 40, 80, 20, 0);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[6]), 25);
    
	
    n_anim_states=2;
    // cansado
    result[SPRCLS_ID_CANSADO]= ( unspriteclass=alloc_sprite_class(n_anim_states) );
    unspriteclass->id=SPRCLS_ID_CANSADO;
    sprite_init_anim_state(&(unspriteclass->animstate[0]), 0, True, 3.0, False, 1, False);  // cansado 0
    sprite_init_anim_state(&(unspriteclass->animstate[1]), 1, True, 1.0, False, 0, False);  // andando 1
    sprite_anim_state_set_bounds(&(unspriteclass->animstate[0]), 48, 48, 24, 0, 40, 48, 20, 0);
    sprite_anim_state_set_bounds(&(unspriteclass->animstate[1]), 48, 48, 24, 0, 40, 48, 20, 0);

    sprite_anim_state_readimagedata(&(unspriteclass->animstate[0]), 20);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[1]), 20);


    n_anim_states=2;
    // bici
    result[SPRCLS_ID_BICI]= ( unspriteclass=alloc_sprite_class(n_anim_states) );
    unspriteclass->id=SPRCLS_ID_BICI;
    sprite_init_anim_state(&(unspriteclass->animstate[0]), 0, True, 0.5, True, 1, False);  // andando 0
    unspriteclass->animstate[0].paso=96.0;
    sprite_init_anim_state(&(unspriteclass->animstate[1]), 1, True, 0.5, True, 0, False);  // andando 1
    unspriteclass->animstate[1].paso=96.0;
    //sprite_anim_state_set_bounds(&(unspriteclass->animstate[0]), 100, 60, 50, 0, 100, 60, 50, 0);
    //sprite_anim_state_set_bounds(&(unspriteclass->animstate[1]), 100, 60, 50, 0, 100, 60, 50, 0);
    
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[0]), 29);
    sprite_anim_state_readimagedata(&(unspriteclass->animstate[1]), 30);
	
    sprite_classes=result;
    
}



// esta funcion se usa para todos los casos en que el motor de juego debe forzar el estado de un sprite
// como consecuencia de una accion del jugador o respuesta a colision o lo que sea
// el stateid es el id del animstate
// solo algunos son aceptables i.e. para saltar se pone el sprite en el estado JUMP que es la preparacion y al finalizar la animacion el salta y se le pone la velocidad como respuesta al cambio de animacion
void sprite_set_state(t_spritePtr spr,int state_id) {
    spr->anim_state=&(spr->sprite_class->animstate[state_id]);
    spr->anim_state_id=state_id;
    spr->anim_lastT=world.t;
//    if (spr->is_player) {
//        if (state_id==STID_PLAYER_WALK) {
//        } else if (state_id==STID_PLAYER_HIT) {
//        } else if (state_id==STID_PLAYER_JUMP) {
//        } else if (state_id==STID_PLAYER_FALL) {
//
//        } else if (state_id==STID_PLAYER_FALLEN) {
//        }
//    }
    
}


void sprite_update_pos(t_spritePtr spr) {
    if ( spr->is_player ) {
        spr->x+=spr->vx*game_dt;
        spr->y+=spr->vy*game_dt;
        
        if (spr->y<0) {
            spr->y=0;
            spr->vy=0; // esto en realidad habria que ponerlo mas abajo
        }
    } else {
        spr->x+=(spr->vx-world.player.vx)*game_dt;
        // no se actualiza la y porque nadie mas que el player tiene
    }
}

void sprite_update_v(t_spritePtr spr) {
    if (spr->y > 0) {
        spr->vy-=world_g*game_dt;
    }
}

void sprite_update_anim(t_spritePtr spr) {
   // t_sprite_classPtr spr_cls=spr->sprite_class;
    t_anim_statePtr anim=spr->anim_state;
    if (anim->auto_change) {
        t_time tchange;
        if (anim->duration_depends_on_v) {
            tchange=spr->anim_lastT+(anim->duration)*(anim->paso)/spr->vx;        
        } else {
            tchange=spr->anim_lastT+anim->duration;
        }
        if ( tchange < world.t ) {
            t_id fromid=spr->anim_state_id;
            spr->anim_lastT=tchange;
            spr->anim_state=&(spr->sprite_class->animstate[anim->next_state_id]);
            spr->anim_state_id=anim->next_state_id;
            if (anim->post_state_action) world_post_animation_action(spr,fromid,anim->next_state_id);
        }
    }
}

void sprite_update(t_spritePtr spr) {
    sprite_update_pos(spr);
    if (spr->is_player) sprite_update_v(spr);
    sprite_update_anim(spr);
}

// test functions
void sprite_test_print_sprite_class_tree(void) {
    t_sprite_classPtr sprite_class;
    int i,j;
    for (i=0;i<5;i++) {
        printf("sprite class %d ptr %x\n",i,(unsigned int)&(sprite_classes[i]));
        sprite_class=sprite_classes[i];
        printf("nanimstates: %d\n",sprite_class->n_anim_states);
        for (j=0;j<sprite_class->n_anim_states;j++) {
            printf("  anim %d (%x)  %s %s\n",sprite_class->animstate[j].id,(unsigned int)&(sprite_class->animstate[j]),sprite_class->animstate[j].auto_change?"auto":"static",sprite_class->animstate[j].duration_depends_on_v?"vdependent":"fixed");
            printf("  duration: %f  next: %x\n",sprite_class->animstate[j].duration,sprite_class->animstate[j].next_state_id);
            printf("  out w: %.1f h: %.1f ox: %.1f oy: %.1f \n",sprite_class->animstate[j].out_bounds.w,sprite_class->animstate[j].out_bounds.h,sprite_class->animstate[j].out_bounds.off_x,sprite_class->animstate[j].out_bounds.off_y);
            printf("  col w: %.1f h: %.1f ox: %.1f oy: %.1f \n",sprite_class->animstate[j].col_bounds.w,sprite_class->animstate[j].col_bounds.h,sprite_class->animstate[j].col_bounds.off_x,sprite_class->animstate[j].col_bounds.off_y);
        }
    }
}









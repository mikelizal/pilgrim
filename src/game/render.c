// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/gl.h>
#endif

#include <string.h>


#define DIBUJA_COL_RECT
//#define DIBUJA_ZONE_RECT


//float render_display_w=480;
//float render_display_h=320;

float render_display_w=960;
float render_display_h=640;


int render_debug_rects=0;
int render_marcadores_debug=0;
int render_marcadores_fps=0;

int low_res=0;
int render_ipad_res=0;

// constantes
t_pos c_floor_level=100;



// para la bocina
t_time bocina_deadline=-1.0;
t_time c_bocina_stay_time=1.5;



void render_set_lowres(void) {
    low_res=1;
}


void render_init(void) {
#if TARGET_OS_IPHONE
    glFogx(GL_FOG_MODE, GL_LINEAR);    
#else
    glFogi(GL_FOG_MODE, GL_LINEAR);
#endif
    GLfloat fog_color[]={1,1,1,1};
    glFogfv(GL_FOG_COLOR,fog_color);
    
    
    bocina_deadline=-1.0;
}


void render(void) {
	render_cam(render_display_w,render_display_h);

//	render_box(30,50,24,24);
//	render_box(0,0,24,24);
//	render_box(480,320,24,24);

	render_world();

    render_marcadores();
}




// Establece coordenadas
// 0,0 es la esquina inferior izquierda
// la anchura y altura en pixeles es el limite

void render_cam(float width, float height) {

// ya viene hecho del prepareFrame	
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//#if TARGET_OS_IPHONE
//    glOrthof(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
//#else
//    glOrtho(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
//#endif    
	
    // para las texturas
	//    glMatrixMode(GL_TEXTURE);
	//    glLoadIdentity();
	//    glScalef(1.0/world.t,1.0/world.t,0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if (low_res) {  // si la resolucion es la de un iphone 3G- dibujamos haciendo en alta resolucion y que opengl la reduzca
        glScalef(0.5,0.5,1);
    }
    
    if (render_ipad_res) {
        glScalef(2048.0/960.0,1536.0/640.0,1);
    }
        
	// glScalef(1.0/world.t,1.0/world.t,0);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glDisable(GL_TEXTURE_2D);
}


GLfloat unit_square_vs[]={-1,-1,-1,1,1,-1,1,1 , 0,0 ,1,1};
GLfloat unit_square_texcoords[]={0,0,0,1,1,0,1,1 , 0,0 ,1,1};

void render_box(float x, float y, float rx, float ry) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(rx,ry,0);
    glVertexPointer(2, GL_FLOAT, 0, unit_square_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
}


GLfloat background_square_vs[]={0,0.4,0,1,1,0.4,1,1  };
GLfloat background_colors[]={.55,.7,.9,1,  .27,.44,.68,1,  .55,.7,.9,1,  .27,.44,.68,1 };

void render_background(void) {
    glPushMatrix();
//    glTranslatef(x,y,0);
//    glColor4f(1.0,1.0,1.0,1.0);
    
    glScalef(render_display_w,render_display_h,0);
    glVertexPointer(2, GL_FLOAT, 0, background_square_vs);
    glColorPointer(4,GL_FLOAT,0,background_colors);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_COLOR_ARRAY);
    glPopMatrix();
    
    glEnable(GL_FOG);
    glPushMatrix();
    glTranslatef(0, 0, 0.6);
    render_tile_num(80,470,AST_TILE_NUBE1);
    render_tile_num(250,500,AST_TILE_NUBE2);
    render_tile_num(500,470,AST_TILE_NUBE3);
    render_tile_num(650,460,AST_TILE_NUBE1);
    glPopMatrix();
    glDisable(GL_FOG);
    
    
    //render_tile_num(0, 0, AST_TILE_FONDO);
}




GLfloat unit_tile_vs[]={0,0,0,1,1,0,1,1 , 0,0 ,1,1};

void render_flat_tile(float x, float y, float w, float h) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(w,h,0);
    glVertexPointer(2, GL_FLOAT, 0, unit_tile_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
}

void render_tile(float x, float y, struct _assets_tile_info *tile) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glVertexPointer(2, GL_FLOAT, 0, tile->vertex_data);
    glTexCoordPointer(2, GL_FLOAT, 0, tile->tex_data);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, tile->tex_id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);    
}



void render_tile_num(float x, float y, int tile_num) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glVertexPointer(2, GL_FLOAT, 0, assets_tiles[tile_num].vertex_data);
    glTexCoordPointer(2, GL_FLOAT, 0, assets_tiles[tile_num].tex_data);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, assets_tiles[tile_num].tex_id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);    
}

void render_texture_rect(float x, float y, GLuint tex_id, GLfloat *vertex_data, GLfloat *tex_data) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glVertexPointer(2, GL_FLOAT, 0, vertex_data);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_data);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);    
}



void render_label_num(float x, float y, int label_num) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glVertexPointer(2, GL_FLOAT, 0, assets_labels[label_num].vertex_data);
    glTexCoordPointer(2, GL_FLOAT, 0, assets_labels[label_num].tex_data);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, assets_labels[label_num].tex_id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);    
}

void render_numstr(float x, float y, char *numstr, int pos) {
    // pos 0 dibuja desde la izquierda
    // pos 1 dibuja desde la derecha
    int i,istart,iend,idelta, car, num_label;
    GLfloat pos_x,pos_y=y;
    pos_x=x;
    if (pos==0) {
        istart=0;
        iend=(int)strlen(numstr);
        idelta=1;
    } else if (pos==1) {
        istart=(int)strlen(numstr)-1;
        iend=-1;
        idelta=-1;
    } else {
        printf("render_numstr: pos %d not supported\n",pos);
    }
    for (i=istart;i!=iend;i+=idelta) {
        car=numstr[i];
        num_label=LABEL_NUM_BASE+car-'0';
        if (car=='.') num_label=12;
        if (pos==1) pos_x-=assets_labels[num_label].out_bounds.w;
        render_label_num(pos_x,pos_y,num_label);
        if (pos==0) pos_x+=assets_labels[num_label].out_bounds.w;
    }
}

GLfloat unit_mountain_vs[]={0,0,0.5,1,1,0,1,1 , 0,0 ,1,1};

void render_mountain(float x, float y, float w, float h) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(w,h,0);
    glVertexPointer(2, GL_FLOAT, 0, unit_mountain_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);    
    glPopMatrix();
}



GLfloat unit_sprite_vs[]={0,0,0,1,1,0,1,1 , 0,0 ,1,1};
GLfloat frame_sprite_vs[]={0,0,0,1,1,1,1,0 , 0,0 ,1,1};


void render_sprite(t_spritePtr spr) {
    // posicion en la pantalla
    int ghost_sprite=0;
    
    float sx,sy;
    if (spr->is_player) {
        sx=0;
    } else {
        sx=spr->x;
    }
    sy=spr->y;
    t_anim_statePtr anim=spr->anim_state;
    float x,y,w,h;
    x=sx - world.cam_x;
    y=sy + c_floor_level;
    
    if (!anim->tex_ready) {
        x-=anim->out_bounds.off_x;
        y-=anim->out_bounds.off_y;
    }
    
    w=anim->out_bounds.w;
    h=anim->out_bounds.h;

    float fake_color=(anim->id+4)*0.1;
    if (spr->is_player) {
        glColor4f(fake_color,1.0,fake_color,1);
        if (world.player_untouchable_time>world.t && world.player.anim_state_id!=STID_PLAYER_TIRED) {
            ghost_sprite=1;
//            if ( (((int)((world.player_untouchable_time-world.t)*5)) % 2) < 1 ) {
//                return;
//            }
            
        }
    } else {
        glColor4f(1.0,fake_color,fake_color,1);
    }
    
    if (anim->tex_ready) {
        //glColor4f(0,0,1,1);
        glPushMatrix();
        glTranslatef(x,y,0);
        glVertexPointer(2, GL_FLOAT, 0, &(anim->vertex_data));
        glTexCoordPointer(2, GL_FLOAT, 0, &(anim->tex_data));
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindTexture(GL_TEXTURE_2D, anim->tex_id);
        if (ghost_sprite) {
            // glBlendFunc(GL_SRC_ALPHA,GL_ONE);
            // mejor con GL_FOG
            glEnable(GL_FOG);
            glTranslatef(0, 0, 0.8);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
        glPopMatrix();
        if (ghost_sprite) {
            // glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            // mejor con GL_FOG
            glDisable(GL_FOG);
        }
        glDisable(GL_TEXTURE_2D);
        
        // Esto dibuja el rectangulo de colision
#ifdef DIBUJA_COL_RECT
        if (render_debug_rects) {
        glColor4f(1,0,0,1);
        glPushMatrix();
        x-=anim->col_bounds.off_x;
        y-=anim->col_bounds.off_y;
        glTranslatef(x,y,0);
        glScalef(anim->col_bounds.w, anim->col_bounds.h, 0);
        glVertexPointer(2, GL_FLOAT, 0, frame_sprite_vs);        
        glDrawArrays(GL_LINE_STRIP, 0, 5);    
        glPopMatrix();
        }
#endif
    } else {
        
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(w,h,0);
    glVertexPointer(2, GL_FLOAT, 0, unit_sprite_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();

    }
}

void render_item(t_itemPtr item) {
    // posicion en la pantalla
    float sx,sy;
    sx=item->x;
    sy=item->y;
    t_item_classPtr item_class=item->item_class;
    float x,y;
//    float w,h;
    x=sx - world.cam_x;
    y=sy + c_floor_level;
    
    x-=item_class->out_bounds.off_x;
    y-=item_class->out_bounds.off_y;
        
    glPushMatrix();
    glTranslatef(x,y,0);
    glVertexPointer(2, GL_FLOAT, 0, &(item_class->vertex_data));
    glTexCoordPointer(2, GL_FLOAT, 0, &(item_class->tex_data));
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, item_class->tex_id);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    if (item_class->type==ITEM_CLS_TUMBA && item->tumba_owner>=0) {
        render_label_num(-15, 110, LABEL_NUM_BASE+item->tumba_owner+1);
    }
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
        
    // Esto dibuja el rectangulo de colision
#ifdef DIBUJA_COL_RECT
    if (render_debug_rects) {
    glColor4f(1,0,0,1);
    glPushMatrix();
    x-=item_class->col_bounds.off_x;
    y-=item_class->col_bounds.off_y;
    glTranslatef(x,y,0);
    glScalef(item_class->col_bounds.w, item_class->col_bounds.h, 0);
    glVertexPointer(2, GL_FLOAT, 0, frame_sprite_vs);        
    glDrawArrays(GL_LINE_STRIP, 0, 5);    
    glPopMatrix();
    }
#endif
}



void render_tileframe(float x, float y, float w, float h) {
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(w,h,0);
    glVertexPointer(2, GL_FLOAT, 0, frame_sprite_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_LINE_STRIP, 0, 5);    
    glPopMatrix();
}


void render_back_zone(int level,int i) {
    t_pos zone_minx=world_zone_getscreenpos(level,i);

#ifdef DIBUJA_ZONE_RECT
    if (render_debug_rects) {
        render_tileframe(zone_minx,c_floor_level,world.zone_width,400-level*100);
        render_flat_tile(zone_minx+400,c_floor_level+world.zone[i].prueba_h+level*100,100,40);
    }
#endif
    
    if (level==0) {
    int j,present;
    for (j=0;j<4;j++) {
        present=world.zone[i].arbol_present[j];
        if (present>0)
            render_tile_num(world.zone[i].arbol_pos[j]+zone_minx, c_floor_level, present );
    }
    } else if (level==1) {
        glEnable(GL_FOG);
        glPushMatrix();
        glTranslatef(0, 0, 0.2);
        if (world.zone[i].back_colina_pos[0]>-1) {
            render_tile_num(world.zone[i].back_colina_pos[0]+zone_minx,c_floor_level,AST_TILE_CASAS1);
        }
        if (world.zone[i].back_colina_pos[1]>-1) {
            render_tile_num(world.zone[i].back_colina_pos[1]+zone_minx,c_floor_level,AST_TILE_CASAS1);
        }
        glPopMatrix();
        glDisable(GL_FOG);
    }
}



void render_world(void) {
    // posicion absoluta en el mundo de la izquierda de la pantalla
    t_pos left_absolute_x=world_get_left_absolute_x();
    t_pos block_width=500.0;
    
    int iblock=(int)(left_absolute_x/block_width);
    int i;
//    int ibackblock=(int)((left_absolute_x/2.0)/(block_width*2.0));
	
    //background
    
    render_background();
    
    t_pos tile_x;
    
    float slow_factor=1/10.0;  // 1 = foreground speed  0 = static background
    float level_factor=10.0;  // 1 = foreground speed  n = 1 background por cada n de foreground
    
    t_pos back_tile_x;
    t_pos back_tile_w=assets_tiles[4].out_bounds.w;
    back_tile_x= back_tile_w*(int)(world.player.x*slow_factor/back_tile_w) - world.player.x*slow_factor +(world.cam_x+200)*slow_factor -world.cam_x;

    back_tile_x= (int)((world.player.x+world.cam_x)/(back_tile_w*level_factor)) + (world.player.x+world.cam_x)/level_factor -world.player.x - world.cam_x;

    back_tile_x= back_tile_w*(int)((world.player.x+world.cam_x)/(back_tile_w*level_factor)) + (world.player.x+world.cam_x)*(1.0-1.0/level_factor) -world.player.x - world.cam_x;

    int back_i;
    back_i=(int)((world.player.x+world.cam_x)/(back_tile_w*level_factor));
    
    back_tile_x=round(back_tile_x);
//    back_tile_x+=400;
    
//    glBlendColor(0.8,0.8,0.8,0.6);
//    glBlendFunc(GL_CONSTANT_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FOG);
    glPushMatrix();
    glTranslatef(0, 0, 0.4);
    
    int fondo_id_ant,fondo_id,fondo_id_sig,fondo_id_sigsig;
    fondo_id=((back_i/3)%2)==0?AST_TILE_FONDO:AST_TILE_FONDO2;
    fondo_id_ant=(((back_i-1)/3)%2)==0?AST_TILE_FONDO:AST_TILE_FONDO2;
    fondo_id_sig=(((back_i+1)/3)%2)==0?AST_TILE_FONDO:AST_TILE_FONDO2;
    fondo_id_sigsig=(((back_i+2)/3)%2)==0?AST_TILE_FONDO:AST_TILE_FONDO2;
    
    render_tile_num(back_tile_x-back_tile_w, c_floor_level-20.0, fondo_id_ant);
    render_tile_num(back_tile_x, c_floor_level-20.0, fondo_id);
    render_tile_num(back_tile_x+back_tile_w, c_floor_level-20.0, fondo_id_sig);
    if (fondo_id_ant==AST_TILE_FONDO && fondo_id==AST_TILE_FONDO2) {
        render_tile_num(back_tile_x-145, c_floor_level-20.0 -1 , AST_TILE_FONDO_TRANS12);
    } else if (fondo_id_ant==AST_TILE_FONDO2 && fondo_id==AST_TILE_FONDO) {
        render_tile_num(back_tile_x-134, c_floor_level-30.0 +9 , AST_TILE_FONDO_TRANS21);
    }
    if (fondo_id==AST_TILE_FONDO && fondo_id_sig==AST_TILE_FONDO2) {
        render_tile_num(back_tile_x+back_tile_w-145, c_floor_level-20.0 -1 , AST_TILE_FONDO_TRANS12);
    } else if (fondo_id==AST_TILE_FONDO2 && fondo_id_sig==AST_TILE_FONDO) {
//        glColor3f(1,1,1);
//        render_box(back_tile_x+back_tile_w-134+134-340 +340/2, c_floor_level-30.0 +9 +100 +390/2,340/2,390/2);
        render_tile_num(back_tile_x+back_tile_w-134, c_floor_level-30.0 +9 , AST_TILE_FONDO_TRANS21);
    }
    if (fondo_id_sig==AST_TILE_FONDO && fondo_id_sigsig==AST_TILE_FONDO2) {
        render_tile_num(back_tile_x+2*back_tile_w-145, c_floor_level-20.0 -1 , AST_TILE_FONDO_TRANS12);
    } else if (fondo_id_sig==AST_TILE_FONDO2 && fondo_id_sigsig==AST_TILE_FONDO) {
        //        glColor3f(1,1,1);
        //        render_box(back_tile_x+back_tile_w-134+134-340 +340/2, c_floor_level-30.0 +9 +100 +390/2,340/2,390/2);
        render_tile_num(back_tile_x+2*back_tile_w-134, c_floor_level-30.0 +9 , AST_TILE_FONDO_TRANS21);
    }
    glPopMatrix();
    glDisable(GL_FOG);
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    
    // solo provisional por ver tres planos
//    slow_factor=0.6;
//    back_tile_w=assets_tiles[1].out_bounds.w+100;
//    back_tile_x=-left_absolute_x*slow_factor+back_tile_w*(int)(left_absolute_x*slow_factor/back_tile_w);
//    render_tile_num(back_tile_x, c_floor_level, AST_TILE_CASAS1);
//    render_tile_num(back_tile_x+back_tile_w, c_floor_level, AST_TILE_CASAS1);

    
    glColor4f(1,0,0,1);
    render_back_zone(1,0);
    glColor4f(0,1,0,1);
    render_back_zone(1,1);
    glColor4f(0,0,1,1);
    render_back_zone(1,2);    
    
    glColor4f(1,0,0,1);
    render_back_zone(0,0);
    glColor4f(0,1,0,1);
    render_back_zone(0,1);
    glColor4f(0,0,1,1);
    render_back_zone(0,2);
    
    
//    for (i=0;i<2;i++) {
//        tile_x=(ibackblock+i)*block_width*2.0-(world.player.x+world.cam_x)/2.0;
//        glColor4f(0,.3,0,1);
//        render_mountain(tile_x,c_floor_level+50,470,150);
//    }
		
//    for (i=0;i<3;i++) {
//        tile_x=(iblock+i)*block_width-world.player.x-world.cam_x - 100.0;
//     //   render_tile_num(tile_x, c_floor_level-70, AST_TILE_SUELO);
//     //   glColor4f(.5,.3,.3,1);
//     //   render_tile(tile_x,c_floor_level-30,block_width-10,40);
//     //   glColor4f(.3,.3,.3,1);
//        //        render_tile(tile_x+110,c_floor_level,100,100);
//        //        render_tile_num(tile_x+110,c_floor_level,AST_TILE_ARBOL2);
//    }	
	
	
    // items
    t_item_iterator iter_i;
    item_iterate_init(&iter_i);
    t_itemPtr item;
    while ( NULL != (item=item_iterate_next(&world, &iter_i)) ) {
        render_item(item);
    }

    // cansados
    t_sprite_iterator iter;
    world_iterate_sprites(&iter);
    t_spritePtr sprite;
    while ( NULL != (sprite=world_iterate_next(&iter)) ) {
        if (sprite->sprite_class->id==SPRCLS_ID_CANSADO) {
            render_sprite(sprite);
        }
    }
    
    // walkers pero no bicis
    world_iterate_sprites(&iter);
    while ( NULL != (sprite=world_iterate_next(&iter)) ) {
        if (sprite->sprite_class->id==SPRCLS_ID_PEREGRINO || sprite->sprite_class->id==SPRCLS_ID_BORDONERO) {
            render_sprite(sprite);
        }
    }
    
    
    //player
    render_sprite(&world.player);

	
    // bicis aparte
    world_iterate_sprites(&iter);
    while ( NULL != (sprite=world_iterate_next(&iter)) ) {
        if (sprite->sprite_class->id==SPRCLS_ID_BICI) {
            render_sprite(sprite);
        }
    }

    
    
    for (i=0;i<3;i++) {
        tile_x=(iblock+i)*block_width-world.player.x-world.cam_x;
        glColor4f(0,1,0,1);
        //        render_tile(tile_x,c_floor_level-40,block_width-10,40);
        render_tile_num(tile_x, c_floor_level-100.0, AST_TILE_SUELO);
        glColor4f(.3,.3,.3,1);
        //        render_tile(tile_x+110,c_floor_level,100,100);
        //        render_tile_num(tile_x+110,c_floor_level,AST_TILE_ARBOL2);
    }
    
    
    render_bocina();
    
    int pading=30, hpading=10;
    render_tile_num(pading,pading-hpading*game_salto_pressed,AST_TILE_BTN_SALTAR);
    render_tile_num(160+pading,pading-hpading*game_esquivar_pressed,AST_TILE_BTN_ESQUIVAR);
    render_tile_num(810+pading,pading-hpading*game_hit_pressed,AST_TILE_BTN_PEGAR);
    render_tile_num(660+pading,pading-hpading*game_stop_pressed,AST_TILE_BTN_ANDAR);
    
    render_tile_num(860, 535, AST_TILE_BTN_PAUSE);
    
    if ( world.player.anim_state_id==STID_PLAYER_TIRED ) {
        //render_label_num(250, 300, 15);
        //render_tile(300, 300, &text_gameover_tile);
        render_tile_num(180,300,AST_TILE_GAMEOVER);
    }
    
    if (world.t<2.8) {
        render_tile_num(305,300,AST_TILE_READY);
    } else if (world.t<3.6) {
        render_tile_num(390/*-world.player.x+500.0*/,300,AST_TILE_GO);        
    }

}

/// \brief Dibuja un marcador de tipo linea
/// Utiliza el color establecido con glColor
void render_marcador_linea(GLfloat x0, GLfloat y0, GLfloat xmax, GLfloat normalized_value) {
    glPushMatrix();
    glTranslatef(x0,y0,0);
    glScalef((xmax-x0)*normalized_value,30,0);
    glVertexPointer(2, GL_FLOAT, 0, unit_tile_vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);    
    glPopMatrix();
}

void render_marcadores(void) {
    // vida 
    float vida=world.player_vida;
//    float green_factor=vida>0.75?vida:(vida>0.25?0.75:0.75*vida/0.25);
//    float red_factor=vida>0.75?0:(vida<0.25?1.0:0.75*(1-(vida-0.25)/(0.75-.25)));

    
    render_tile_num(70-30, 560, AST_TILE_VIDA);    
    render_tile_num(690-89, 550, AST_TILE_MARCADOR);
    
    glColor4f(1.0*(1-vida),1.0*vida,0,1);
    render_marcador_linea(140-30, render_display_h-65, /*render_display_w-130*/ 670 -89 , vida);

    /*
    glColor4f(0,0,1,1);
    float remaining_time=(world.player_deadline-world.t)/(60.0);
    if (remaining_time<0.0) remaining_time=0.0;
    render_marcador_linea(10, render_display_h-23, render_display_w-20, remaining_time);
    glColor4f(1,0,0,1);
    render_marcador_linea(10, render_display_h-31, render_display_w-20, (world.player.x-(world.player_checkpoint-7000.0))/(7000.0));
    */
    
    char xs[10];
    //sprintf(xs,"%.1f",world.player.x);
//    sprintf(xs,"%.0f",world.player.x/100.0);
// el problema del de arriba es que redondea para arriba
    sprintf(xs,"%u",(int)((world.player.x-world_pos0)/100.0));
    render_numstr(880-89,545,xs,1);
    
    if (render_marcadores_debug) {
        sprintf(xs,"%u",world.sprites_n);
        render_numstr(10,530,xs,0);
        sprintf(xs,"%u",world.items_n);
        render_numstr(10,480,xs,0);
    }

    if (render_marcadores_fps) {
        sprintf(xs,"%.0f",game_fps);
        render_numstr(900,480,xs,1);
    }
    
    
    if (game_paused) {
        render_tile_num(252, 169, AST_TILE_MENU_PAUSE);
        render_tile_num(252+100, 169+160, AST_TILE_BTN_RESUME);
        render_tile_num(252+100, 169+20, AST_TILE_BTN_QUIT);
    }
    
}

void render_clean_bocina(void) {
    bocina_deadline=-1.0;
}

void render_show_bocina(void) {
    bocina_deadline=world.t+c_bocina_stay_time;
}

void render_bocina(void) {
    if (bocina_deadline>world.t) {
        float f=(world.t-bocina_deadline+c_bocina_stay_time);
        int nf=4;
        f=nf*f-(int)(nf*f);
        render_tile_num(100+f*30,500+f*10,AST_TILE_BOCINA);
    }
}



// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


t_item_classPtr *item_classes=NULL;





t_item_classPtr alloc_item_class(t_id type) {
    t_item_classPtr item_class;
    item_class=malloc(sizeof(t_item_class));
    item_class->type=type;
    return item_class;
}

// muy parecida a sprite_anim_state_readimagedata
void item_class_readimagedata(t_item_classPtr item_class,int img_id) {
    t_assets_image_infoPtr img_info;
    img_info=&(assets_images[img_id]);

    item_class->out_bounds.w=img_info->r_w;
    item_class->out_bounds.h=img_info->r_h;
    item_class->out_bounds.off_x=img_info->off_x;
    item_class->out_bounds.off_y=img_info->off_y;
    item_class->col_bounds.w=img_info->col_w;
    item_class->col_bounds.h=img_info->col_h;
    item_class->col_bounds.off_x=img_info->col_off_x;
    item_class->col_bounds.off_y=img_info->col_off_y;
    
    generate_vertex_data_from_bb(item_class->vertex_data,&(item_class->out_bounds));

    GLfloat tex_container_w=img_info->tex_container_w;
    GLfloat tex_container_h=img_info->tex_container_h;

    generate_texcoord_data_from_rect(item_class->tex_data,img_info->r_x,img_info->r_y,img_info->r_w,img_info->r_h,tex_container_w,tex_container_h);

    item_class->tex_id=img_info->tex_id;
}




void item_classes_init(void) {
    int n_item_classes=N_ITEM_CLASSES;
    
    item_classes=malloc(sizeof(t_item_classPtr)*n_item_classes);
    item_classes[ITEM_CLS_COMIDA_1]=alloc_item_class(ITEM_CLS_COMIDA_1);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_1], ITEM_IMG_COMIDA_1);
    item_classes[ITEM_CLS_COMIDA_2]=alloc_item_class(ITEM_CLS_COMIDA_2);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_2], ITEM_IMG_COMIDA_2);
    item_classes[ITEM_CLS_COMIDA_3]=alloc_item_class(ITEM_CLS_COMIDA_3);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_3], ITEM_IMG_COMIDA_3);
    item_classes[ITEM_CLS_COMIDA_4]=alloc_item_class(ITEM_CLS_COMIDA_4);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_4], ITEM_IMG_COMIDA_4);
    item_classes[ITEM_CLS_COMIDA_5]=alloc_item_class(ITEM_CLS_COMIDA_5);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_5], ITEM_IMG_COMIDA_5);
    item_classes[ITEM_CLS_COMIDA_6]=alloc_item_class(ITEM_CLS_COMIDA_6);
    item_class_readimagedata(item_classes[ITEM_CLS_COMIDA_6], ITEM_IMG_COMIDA_6);
    item_classes[ITEM_CLS_POWERUP_LIFE]=alloc_item_class(ITEM_CLS_POWERUP_LIFE);
    item_class_readimagedata(item_classes[ITEM_CLS_POWERUP_LIFE], ITEM_IMG_POWERUP_LIFE);    
    item_classes[ITEM_CLS_TUMBA]=alloc_item_class(ITEM_CLS_TUMBA);
    item_class_readimagedata(item_classes[ITEM_CLS_TUMBA], ITEM_IMG_TUMBA);

}


void item_zero(t_world *theworld) {
    int i;
    theworld->items_n=0;
    for (i=0;i<MAX_N_ITEMS;i++) {
        theworld->item_valid[i]=False;
    }
}

void item_delete(t_world *theworld,int i) {
    theworld->item_valid[i]=False;
    theworld->items_n-=1;
}

t_itemPtr item_create(t_world *theworld, t_id item_class_id) {
    int i;
    for (i=0;i<MAX_N_ITEMS;i++) {
        if (!theworld->item_valid[i]) {
            theworld->item_valid[i]=True;
            theworld->items_n+=1;
            
            theworld->items[i].item_class=item_classes[item_class_id];
            
            return &(theworld->items[i]);
        }
    }
    return NULL;
}

void item_iterate_init(t_item_iteratorPtr iter) {
    iter->i=-1;
}

t_itemPtr item_iterate_next(t_world *theworld,t_item_iteratorPtr iter) {
    int i;
    for (i=(iter->i)+1;i<MAX_N_ITEMS;i++) {
        if (theworld->item_valid[i]) {
            iter->i=i;
            return &(theworld->items[i]);
        }
    }
    iter->i=MAX_N_ITEMS;
    return NULL;
}

void item_update(t_itemPtr item) {
    item->x-=world.player.vx*game_dt;
    
    // no movement in y
    return;
    
    if (item->y>0) { 
        item->y-=100.0*game_dt;
    } else {
        item->y=0;
    }
}

bool item_isfruit(t_itemPtr item) {
    t_id type=item->item_class->type;
    switch (type) {
        case ITEM_CLS_COMIDA_2:
        case ITEM_CLS_COMIDA_3:
        case ITEM_CLS_COMIDA_4:
            return true;
    }
    return false;
}



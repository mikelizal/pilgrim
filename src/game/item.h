// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para manejar los items estaticos del juego. Comida y demas objetos cogibles



#define N_ITEM_CLASSES 8
#define ITEM_CLS_COMIDA_1 0
#define ITEM_CLS_COMIDA_2 1
#define ITEM_CLS_COMIDA_3 2
#define ITEM_CLS_COMIDA_4 3
#define ITEM_CLS_COMIDA_5 4
#define ITEM_CLS_COMIDA_6 5
#define ITEM_CLS_POWERUP_LIFE 6
#define ITEM_CLS_TUMBA 7

#define ITEM_IMG_COMIDA_1 31
#define ITEM_IMG_COMIDA_2 32
#define ITEM_IMG_COMIDA_3 33
#define ITEM_IMG_COMIDA_4 34
#define ITEM_IMG_COMIDA_5 35
#define ITEM_IMG_COMIDA_6 36
#define ITEM_IMG_POWERUP_LIFE 37
#define ITEM_IMG_TUMBA 15



typedef struct _world t_world;

struct _item_class {
    t_id type;
    
    t_bb out_bounds; // limites
    t_bb col_bounds; // limites para colision
        
    // datos para rendering
    GLuint tex_id;
    GLfloat vertex_data[8];
    GLfloat tex_data[8];
};

typedef struct _item_class t_item_class, *t_item_classPtr;



struct _item {
    t_pos x,y;
    t_item_classPtr item_class;

    // para que las tumbas almacenen info
    int tumba_owner; // i numero en la lista de records -1 last
};

typedef struct _item t_item, *t_itemPtr;


struct _item_iterator {
    int i;	
};

typedef struct _item_iterator t_item_iterator, *t_item_iteratorPtr;

t_item_classPtr alloc_item_class(t_id type);
void item_class_readimagedata(t_item_classPtr item_class,int img_id);

void item_classes_init(void);
void item_zero(t_world *theworld);
void item_delete(t_world *theworld,int i);
t_itemPtr item_create(t_world *theworld, t_id item_class_id);
void item_iterate_init(t_item_iteratorPtr iter);
t_itemPtr item_iterate_next(t_world *theworld,t_item_iteratorPtr iter);
void item_update(t_itemPtr item);

bool item_isfruit(t_itemPtr item);



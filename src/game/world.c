// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"
#include "sys/times.h"

t_world world;


t_pos pos_per_m=100.0;

t_time jump_semi_t=160.0/150.0    *0.5*0.5 ; // tiempo que dura la mitad del salto (en alcanzar la altura maxima)
t_pos jump_max_h= 0.5 * 160.0/150.0 * 160.0 *1.2 *2.0;  // maxima altura en pixeles alcanzada en el salto

// estos dos se recalculan al empezar para conseguir los valores de semi_t y altura buscados
t_v world_g=150.0; // aceleracion de la gravedad en pixeles por segundo en cada segundo
t_v world_jump_vy=160.0; // velocidad vertical de salto
float world_jump_vx_fraction=1.0;  // fraccion de la velocidad horizontal al saltar

// constantes de las velocidades rapida y lenta
// en pixeles por segundo? de momento
//t_v c_v_fast=80.0;
//t_v c_v_slow=40.0;
t_v c_v_fast=620.0/1.0;
t_v c_v_slow=160.0;
t_v c_v_a=80.0*32;

t_v world_walker_v;

float c_life_loss_per_second=1.0/10.0; // 15 s to die
float c_life_loss_per_jump=0.0;
float c_life_loss_per_hit=0.25;
float c_life_gain_per_food=0.2;

t_time invulnerability_time=2.0;

t_time world_waiting_to_start_time=2.8;
t_pos world_pos0=500.0;
t_time world_t0=2.8;

t_pos c_cam_offset=200.0;
t_bool c_cam_init_free; // flag para marcar el periodo inicial de la camara

int cam_type=1;
//  0 muelle
//  1 limites
//  2 fija

t_spritePtr collision_candidates[MAX_N_SPRITES];
int collision_candidates_n;

t_spritePtr collision_bici;
int collision_bici_n;

t_time debug_time_printed=0.0;;


t_pos world_player_coords_to_screen_coords(t_pos player_coord_x) {
    return player_coord_x-world.cam_x;
}

t_pos world_screen_coords_to_player_coords(t_pos screen_coord_x) {
    return screen_coord_x+world.cam_x;
}

t_pos world_player_coords_to_world_coords(t_pos player_coord_x) {
    return player_coord_x+world.player.x;
}

t_pos world_world_coords_to_player_coords(t_pos world_coord_x) {
    return world_coord_x-world.player.x;
}

t_pos world_world_coords_to_screen_coords(t_pos world_coord_x) {
    return world_coord_x-world.player.x-world.cam_x;
}

t_pos world_screen_coords_to_world_coords(t_pos screen_coord_x) {
    return screen_coord_x+world.cam_x+world.player.x;
}





void world_init(void) {
    
    world_jump_vy=2*jump_max_h/jump_semi_t;
    world_g=world_jump_vy/jump_semi_t;

    world_walker_v=c_v_slow;
    
    struct tms paraeltimes;
    srand((unsigned int)times(&paraeltimes));
    
    world.t=0.0;
    
    world.waiting_to_start=True;
    
    world.player.is_player=True;
    world.player.x=0.0;
    world.player.y=0.0;
    world.player.vx=c_v_slow*0; // para que empiece parado hasta el go
    world.player.vy=0.0;
    world.player.sprite_class=sprite_classes[SPRCLS_ID_PLAYER];
    sprite_set_state(&world.player, STID_PLAYER_STOP);
   // world.player.anim_state=&(sprite_classes[SPRCLS_ID_PLAYER]->animstate[0]);
    world.player.anim_lastT=world.t;
    
    
    world.player_vida=1.0;
    world.player_deadline=60.0;
    world.player_checkpoint=7000.0;
    world.player_untouchable_time=0.0;
    
    world_zero_sprites();
    item_zero(&world);

//    world_create_walker(200,0);
//    world_create_walker(400,0);
//    world_create_walker(600,0);
//
//    world_create_walker(700,1);
//    world_create_walker(1000,1);
//
//    
//    world_create_walker(900,2);
//
//    world_create_walker(-800,3);
//    
//    world.time_next_piligrim=random_exp_time(10.0);
//    world.time_next_cansado=random_exp_time(5.0);
//    world.time_next_bici=random_exp_time(10.0);
//    printf("primer generado en t=%f\n",world.time_next_piligrim);
    
    render_clean_bocina();
    
    world.cam_x=-c_cam_offset;
    world.cam_v=0;
    // para que el jugador empiece fuera de la pantalla
    c_cam_init_free=1;
    world.cam_x+=c_cam_offset+300;
    // para que empiece dentro y parado esperando al go
    world.cam_x=-2*c_cam_offset;
    world.player.x+=world_pos0;
    
    world.zone_width=1000;
    world.zone_level_factor[0]=1.0;
    world.zone_level_factor[1]=5;
    world.zone[0].i[0]=-100;
    world.zone[1].i[0]=-100;
    world.zone[2].i[0]=-100;
    
    
    generador_init();

    t_itemPtr tumba_item=item_create(&world, ITEM_CLS_TUMBA);
    tumba_item->x=game_last_dead;
    tumba_item->y=0.0;
    tumba_item->tumba_owner=-1;
    int itumba;
    for (itumba=0; itumba<HIGHSCORE_MAX_TUMBAS; itumba++) {
        if ( highscore_table[itumba].score > HIGHSCORE_MINX_TUMBAS ) {
            t_itemPtr tumba_item=item_create(&world, ITEM_CLS_TUMBA);
            tumba_item->x=100.0*highscore_table[itumba].score+world_pos0;
            tumba_item->y=0.0;
            tumba_item->tumba_owner=itumba;
        }
    }
        
//    t_itemPtr aux_item=item_create(&world, ITEM_CLS_COMIDA_1);
//    aux_item->x=1030.0;
//    aux_item->y=150.0;

    
}


void world_update_dt(void) {
    t_sprite_iterator iter;
    t_spritePtr sprite;
    t_item_iterator iiter;
    t_itemPtr item;
    
//    if (world.t-debug_time_printed > 10.0) {
//        debug_time_printed+=10.0;
//        printf("t= %.1f   %d sprites nexts: %.1f %.1f %.1f\n",debug_time_printed,world.sprites_n,world.time_next_piligrim,world.time_next_cansado,world.time_next_bici);
//    }
    
    
    
    // en la iteracion de actualizacion acumula collision candidates para despues
    collision_candidates_n=0;
    collision_bici_n=0;
    t_pos collision_x=world.player.vx*game_dt;
    // y calcula el walker mas retrasado y su velocidad
    t_pos min_x=500;
    t_v min_v=c_v_fast;
    world_iterate_sprites(&iter);
    // actualiza posicion y velocidad y estado de la animacion
    // tiene que hacerse al final para el player porque su velocidad se usa en el calculo de los demas
    while ( NULL != (sprite=world_iterate_next(&iter)) ) {
        sprite_update(sprite);

        if (sprite->anim_state_id > 100) {
            printf("warning : anim state invalid\n");
        }
        
        if (abs(sprite->x-collision_x)<200.0) { // si estan cerca pueden colisionar
            collision_candidates[collision_candidates_n]=sprite;
            collision_candidates_n+=1;
        }
        if ((sprite->sprite_class->id==SPRCLS_ID_PEREGRINO || sprite->sprite_class->id==SPRCLS_ID_BORDONERO) && (sprite->anim_state_id<STID_WALKER_STOP || sprite->anim_state_id==STID_WALKER_RECEIVED_HIT)) {// || sprite->anim_state_id==STID_WALKER_RECEIVING_HIT)) {
            if (sprite->x<min_x && sprite->x>0) {
                min_x=sprite->x;
                min_v=sprite->vx;
            }
        }
        if (collision_bici_n<1 && sprite->sprite_class->id==SPRCLS_ID_BICI) {
            collision_bici_n+=1;
            collision_bici=sprite;
        }
        
        // posible destruccion de sprites lejanos cuidado no usar despues sprite
        if (sprite->x-world.cam_x < -100 && sprite->sprite_class->id<SPRCLS_ID_BICI ) {
            world_delete_sprite(iter.i);
        }
        if (sprite->x-world.cam_x > 2000 && sprite->sprite_class->id==SPRCLS_ID_BICI ) {
            world_delete_sprite(iter.i);
            audio_una_bici_menos();
        }
    }
    
    //if (collision_candidates_n>0) printf("collision candidates: %d\n",collision_candidates_n);
    
    item_iterate_init(&iiter);
    while ( NULL != (item=item_iterate_next(&world, &iiter)) ) {
        item_update(item);
        
        if (item->x<-800) {
            item_delete(&world,iiter.i);
        }
    }
    

    if (world.player.x>world.player_checkpoint) {
//        printf("checkpoint !!!\n");
        world.player_checkpoint+=7000.0;
        world.player_deadline=world.t+60.0;
    }
    
    if (!world.waiting_to_start) {
        if (world.player.anim_state_id!=STID_PLAYER_FALLEN) {
            world_lose_life(game_dt*c_life_loss_per_second);
        }
    }
        
    if ( ( world.player_deadline<world.t || world.player_vida<=0.0 ) && ( world.player.anim_state_id<STID_PLAYER_STOP || world.player.anim_state_id==STID_PLAYER_SIDE ) ) {
        if (world.player.anim_state_id!=STID_PLAYER_TIRED) {
            audio_stop_music();
            audio_play_player_clip(AUDIO_SND_GAMEOVER);
            stats_end_game_timedistance();
        }
        world.player.vx=0;
        sprite_set_state(&world.player, STID_PLAYER_TIRED);
    }
    
    if (world.player.anim_state_id<STID_PLAYER_STOP && (world.player_untouchable_time<=world.t) ) {
        if(min_x<90.0) {
            world.player.vx=0.5*min_v;
        } else if (min_x<100.0) {
            world.player.vx=min_v;        
        }
    }
    
    sprite_update(&world.player);
    
    // controles
    t_id player_state=world.player.anim_state_id;

    if (!world.waiting_to_start) {
        if (world.player.vx<c_v_fast && player_state<STID_PLAYER_STOP) world.player.vx+=c_v_a*game_dt;
    } else {
        if (world.t>world_waiting_to_start_time) {
            world.waiting_to_start=False;
            sprite_set_state(&world.player, STID_PLAYER_WALK);
        }
    }

    if ( game_salto_pressed && player_state<STID_PLAYER_STOP ) {
        sprite_set_state(&world.player, STID_PLAYER_PREPARINGJUMP);
        audio_play_player_clip(AUDIO_SND_SALTO);
    }
    if ( game_hit_pressed && player_state<STID_PLAYER_STOP ) {
//        audio_play_player_clip(AUDIO_SND_HIT);
        audio_play_golpe_inicio();
        sprite_set_state(&world.player, STID_PLAYER_PREPARINGHIT);
    }
    if ( game_esquivar_pressed && player_state<STID_PLAYER_STOP ) {
        world.player.vx=0;
        sprite_set_state(&world.player, STID_PLAYER_SIDE);
    }
    if ( game_stop_pressed && player_state<STID_PLAYER_STOP ) {
        world.player.vx=c_v_slow*.7+c_v_fast*.3;
        //world_gain_life(1.0);
        //sprite_set_state(&world.player, STID_PLAYER_STOP);
    }
//    if ( !game_stop_pressed && player_state==STID_PLAYER_STOP ) {
//        world.player.vx=c_v_slow;
//        sprite_set_state(&world.player, STID_PLAYER_WALK);
//    }
    if ( !game_esquivar_pressed && player_state==STID_PLAYER_SIDE ) {
        world.player.vx=c_v_slow;
        sprite_set_state(&world.player, STID_PLAYER_WALK);
    }
    
//    // el estado puede haber cambiado por los controles
//    player_state=world.player.anim_state_id;
//    if ( world.player_vida<.1 && player_state<STID_PLAYER_STOP ) {
//        world.player.vx=c_v_slow*1.5;
//    }
//    
//    if ( world.player_vida<.05 && player_state<STID_PLAYER_STOP ) {
//        world.player.vx=c_v_slow*1.0;
//    }
    
    // si tienes poca vida y aun no estas muerto declarate in panic
    if (world.player_vida<.3 &&  world.player.anim_state_id!=STID_PLAYER_TIRED ) {
        float vida=world.player_vida;
        int ifreq=(vida<0.1)?3:((vida<0.2)?2:1);
        ifreq=1; // anular frecuencia variable del warning
        audio_low_warning_play(ifreq);
        achievments_inpanicnow(true);
    } else {
        achievments_inpanicnow(false);
    }
    
    
    // colisiones
    if ( world.player.y <0.001 && world.player.vy<0.001 ) {
        if ( player_state==STID_PLAYER_JUMP ) {
            world.player.y=0;
            world.player.vy=0;
//            world.player.vx=c_v_fast;
            sprite_set_state(&world.player, STID_PLAYER_WALK);
            audio_play_player_clip(AUDIO_SND_CAIDABIEN);
        } else if ( player_state==STID_PLAYER_FALL ) {
            world.player.y=0;
            world.player.vy=0;
            //world.player.vx=c_v_slow*0.5;
            sprite_set_state(&world.player, STID_PLAYER_FALLEN);
            audio_play_player_clip(AUDIO_SND_CAIDA);
        } else if ( player_state==STID_PLAYER_FALLEN ) {
            world.player.vx-=c_v_fast*0.02;
            if (world.player.vx<0) world.player.vx=0;
        }
    }
    
    int i;
    for (i=0; i<collision_candidates_n; i++) {
        sprite=collision_candidates[i];
        // colisionan sprite y player ???
        world_handle_collision_candidate(&world.player, sprite);
    }
    
    item_iterate_init(&iiter);
    while ( NULL != (item=item_iterate_next(&world, &iiter)) ) {
        if (item->item_class->type==ITEM_CLS_TUMBA) continue;
        if (world_item_collision_check(&world.player, item) /* && player_state==STID_PLAYER_STOP */ ) {
            audio_play_player_clip(AUDIO_SND_COMER);
            if (item->item_class->type==ITEM_CLS_POWERUP_LIFE) {
                world_gain_life(1.0);                
            } else {
                world_gain_life(c_life_gain_per_food);
                achievments_eaten(item);
            }
            item_delete(&world,iiter.i);
            stats_add_comida(item->item_class->type);
        }
    }
    
    // vuelta extra para que las bicis arrasen
    // del collision candidates se ha quedado con una bici y prueba si colisiona con otros
    if (collision_bici_n>0 && collision_bici->x<1500.0) {
        world_iterate_sprites(&iter);
        while ( NULL != (sprite=world_iterate_next(&iter)) ) {
            if (sprite!=collision_bici) {
               
                if (world_nonplayer_collision_check(collision_bici,sprite)) {
                    if ( (sprite->sprite_class->id==SPRCLS_ID_PEREGRINO || sprite->sprite_class->id==SPRCLS_ID_BORDONERO) && sprite->anim_state_id<STID_WALKER_STOP) {
                        audio_play_other_clip(AUDIO_SND_CAIDA);
                        sprite_set_state(sprite, STID_WALKER_RECEIVING_HIT);
                        sprite->vx=0.0;
                    }
                }
            }
        }
    }
    
    //int i;
//	int min_i;
//	t_pos min_x=200;
//	t_v min_v=100;
//    for (i=0;i<MAX_N_WALKER;i++) {
//        if (world.walker_list[i].valid) {
//            world.walker_list[i].x+=(world.walker_list[i].v-world.player_vx)*game_dt;
//			if (!world.walker_list[i].apartado) {
//				if (world.walker_list[i].x<min_x) {
//					min_x=world.walker_list[i].x;
//					min_v=world.walker_list[i].v;
//					min_i=i;
//				}
//			}
//            // regenera rapido para probar
//            if (world.walker_list[i].x-world.cam_x<-25.0) {
//               // printf("regenerate now\n");
//                world.walker_list[i].x=480+world.cam_x+24-100*log(rand()*1.0/RAND_MAX);
//                world.walker_list[i].v=c_v_slow+5.0*(int)(4.0*(rand()*1.0/RAND_MAX));
//				world.walker_list[i].apartado=0;
//            }
//            
//        }
//    }
//
//    if (min_x<50) {
//		world.player_vx=min_v;
//		if (!world.player_slow) {
//			world.player_slow=1;
//			world.player_slow_since=world.t;
//		} else {
//			if (world.t-world.player_slow_since>3.0) {
//				world.walker_list[min_i].apartado=1;
//				world.walker_list[min_i].v=0;
//			}
//		}
//	
//	} else {
//		world.player_vx=c_v_fast;
//		world.player_slow=0;
//	}
//	
//	if (world.player.y<0.0001) {
//		world.player.y=0;
////		if (game_salto_pressed) {
////			world.player_vy=100;
////		}
//        }	
//    
    
    //genera
    
//    if (world.t > world.time_next_piligrim) {
//        int tipo=rand()%2;   // de momento 50 - 50 cambiar probabilidades
//        world_create_walker(1000.0, tipo);
//        world.time_next_piligrim=world.t+random_exp_time(20.0);
//    }    
//    if (world.t > world.time_next_cansado) {
//        world_create_walker(1000.0, 2);
//        world.time_next_cansado=world.t+random_exp_time(20.0);
//    }    
//    if (world.t > world.time_next_bici) {
//        world_create_walker(-400.0, 3);
//        world.time_next_bici=world.t+random_exp_time(20.0);
//    }
    
    
    //probando cosas mas creativas para la camara
    // camara on fuerza que proporcional a la distancia al punto deseado
    // y con rozamiento viscoso para que no oscile

    if (cam_type==0) {    
    
        t_pos cam_now,cam_target;
        cam_now=world.cam_x-world.player.vx*game_dt;
        cam_target=-c_cam_offset;
//
    world.cam_v+=250.0*(cam_target-cam_now)*game_dt-1.5*world.cam_v;

//    if (abs(world.cam_v)>500.0) world.cam_v=copysign(500.0,world.cam_v);
//        world.cam_v=copysign(200,(cam_target-cam_now));
    
        float smooth_alfa=0; // 0 no smooth
        
        t_pos cam_smooth=cam_now+world.cam_v*game_dt;
        world.cam_x=world.cam_x*smooth_alfa+cam_smooth*(1-smooth_alfa);
        
    
 //   if (world.cam_x>cam_target) {world.cam_x=cam_target;world.cam_v=world.player.vx;}
//    if (world.cam_x+200<cam_target) {world.cam_x=cam_target;world.cam_v=world.player.vx;}

//    if (world.cam_x<-300) {world.cam_x=-300; world.cam_v=world.player.vx;}
//    if (world.cam_x>-100) {world.cam_x=-100; world.cam_v=world.player.vx;}
    } else if (cam_type==1) {
        
        // camara simple con distancia maxima y minima
        t_pos max_cam_distance=300;
        t_pos min_cam_distance=200-10000*c_cam_init_free;
        t_pos cam_now=world.cam_x-world.player.vx*game_dt+world.cam_v*game_dt;
        if (cam_now<-max_cam_distance) {world.cam_v=world.player.vx;cam_now=-max_cam_distance;}
        if (cam_now>-min_cam_distance) {world.cam_v=0*world.player.vx;cam_now=-min_cam_distance;}
        world.cam_x=cam_now;
        // cuando la camara alcanza al jugador se acaba el estado inicial de camara
        if (c_cam_init_free && cam_now<-200) c_cam_init_free=0;
    
    } else if (cam_type==2) {
        // de momento la camara sigue al jugador siempre a una distancia
        world.cam_x=-c_cam_offset;
    }
    
    
//    if(game_hit_pressed) {
//        world.cam_x+=10;
//    }
//    if(game_esquivar_pressed) {
//        world.cam_x-=10;
//    }
    
    
    world_update_zones();
    generador_update();
    
    world.t+=game_dt;
    
    achievments_check_update();
}

t_pos world_zone_getscreenpos(int level, int i) {
//    t_pos zone_minx_player_coords=world.zone_width*(world.zone[i].i[level]) - world.player.x/world.zone_level_factor[level];
    t_pos zone_minx_world_coords=world.zone_width*(world.zone[i].i[level]) + (level!=0)*(world.player.x+world.cam_x)*(1.0-1.0/world.zone_level_factor[level]);
//    zone_minx_player_coords+=(world.cam_x+c_cam_offset)*(level!=0)/world.zone_level_factor[level];
    t_pos zone_screen_minx=world_world_coords_to_screen_coords(zone_minx_world_coords);

//    printf("z %d %d   %d\n",i,level,world.zone[i].i[level]);
//    if (level==0) {
//        printf("min_pc=%f\n",zone_minx_world_coords);
//    }
    return zone_screen_minx;
}


void world_update_zones(void) {
    world.i_zone=(int)(world.player.x/world.zone_width);
    world_update_zone_if_needed(0,world.i_zone-1);
    world_update_zone_if_needed(0,world.i_zone);
    // en realidad las dos anteriores no hace falta llamarlas porque nunca deberian necesitar actualizacion
    // salvo la primera vez
    world_update_zone_if_needed(0,world.i_zone+1);
    int i_level1=(int)((world.player.x/world.zone_level_factor[1])/world.zone_width);
    world_update_zone_if_needed(1,i_level1-1);
    world_update_zone_if_needed(1,i_level1);
    // en realidad las dos anteriores no hace falta llamarlas porque nunca deberian necesitar actualizacion
    // salvo la primera vez
    world_update_zone_if_needed(1,i_level1+1);    
}

void world_update_zone_if_needed(int level, int izone) {
    int zone_seed=generador_zone_seed;
    
    int i_zone_storage=izone%3;
    if (izone<0) i_zone_storage+=3;
    
    if (world.zone[i_zone_storage].i[level]==izone) {
        // no need to update
        return;
    }
    //printf("generated new zone %d level %d\n",izone, level);
    world.zone[i_zone_storage].i[level]=izone;
    
    t_world_zonePtr thezone;
    thezone=&(world.zone[i_zone_storage]);
    if (level==0) {  // genera zonas de nivel 0 o de primer plano
        unsigned int s=izone+zone_seed;
        thezone->prueba_h=(rand_r(&s)%10)*10.0;
        // genera numeros aleatorios y los objetos de la zona
        
        // 4 arboles al azar con posicion uniformemente distribuida en la zona
        int i;
        for (i=0;i<4;i++) {
            thezone->arbol_present[i]=0;
            if (rand_r(&s)%100 <25) {
                thezone->arbol_present[i]=2+(rand_r(&s)%2);
                thezone->arbol_pos[i]=rand_r(&s)%1000;
            }
        }
    
//        int hay_caido=(rand_r(&s)%100<25);  // posible peregrino sentado
        //if (hay_caido) world_create_walker(800,2);
                                  
//        int hay_bici=(rand_r(&s)%100<25);  // posible bici
//        if (hay_bici) world_create_walker(-800*2,3);

        if (izone<0) return;
        
//        for (i=0;i<3;i++) {
//            if (rand_r(&s)%100 <25) {
//                int walker_type=rand_r(&s)%2;
//                float walker_pos=world.zone_width+world.zone_width*(rand_r(&s)%100)*1.0/100.0;
//                world_create_walker(walker_pos, walker_type);
//            }
//        }
        
        int items_seed=generador_items_seed;
        s+=items_seed;
        int hay_comida=(rand_r(&s)%100<105); // posible comida
        if (hay_comida) {
            int comida_tipo=rand_r(&s)%6;
            if (izone%50==49) {
                comida_tipo=ITEM_CLS_POWERUP_LIFE;
            }
            t_itemPtr unitem=item_create(&world, comida_tipo);
            if (unitem) {
//                printf("creada comida tipo %d\n", comida_tipo);
                unitem->x=1000.0-200.0+400.0*(rand_r(&s)%100)/100.0;
                //unitem->y=1.0*(rand_r(&s)%500);
                unitem->y=250.0;
            }
        }
        
        
        
    } else if (level==1) { // zonas de nivel 1 segundo plano
        unsigned int s=izone+zone_seed+1;
        if (rand_r(&s)%100 <80) {  // posible colina 1
            thezone->back_colina_pos[0]=20;
        } else {
            thezone->back_colina_pos[0]=-1;                
        }
        if (rand_r(&s)%100 <80) {  // posible colina 2
            thezone->back_colina_pos[1]=520;
        } else {
            thezone->back_colina_pos[1]=-1;                
        }
    }
        
}



t_pos world_get_left_absolute_x(void) {
	return world.player.x+world.cam_x;
}


t_pos world_pos_to_m(t_pos pos) {
    return pos/pos_per_m;
}

t_pos world_m_to_pos(t_pos m) {
    return m*pos_per_m;
}



void world_zero_sprites(void) {
    int i;
    for (i=0;i<MAX_N_SPRITES;i++) {
        world.sprite_valid[i]=False;
    }
    world.sprites_n=0;
}

void world_iterate_sprites(t_sprite_iteratorPtr iter) {
    iter->i=-1;
}

t_spritePtr world_iterate_next(t_sprite_iteratorPtr iter) {
    int i;
    for (i=(iter->i)+1;i<MAX_N_SPRITES;i++) {
        if (world.sprite_valid[i]) {
            iter->i=i;
            return &(world.sprites[i]);
        }
    }
    iter->i=MAX_N_SPRITES;
    return NULL;
}

void world_delete_sprite(int i) {
    world.sprite_valid[i]=False;
    world.sprites_n-=1;
}

// devuelve null si no puede
t_spritePtr world_create_sprite(void) {
	int i;
	for (i=0;i<MAX_N_SPRITES;i++) {
		if (!world.sprite_valid[i]) {
			world.sprite_valid[i]=True;
            world.sprites_n+=1;
			return &(world.sprites[i]);
		}
	}
	return NULL;
}


void world_create_walker (t_pos position_x, int tipo) {
    t_spritePtr spr;
    spr=world_create_sprite();
//    printf("creating %d ...\n",tipo);
    if (spr==NULL) return; // si no puede pues lo deja
//    printf("ok\n");
    spr->is_player=False;
    spr->x=position_x;
    spr->y=0;
    spr->vx=c_v_slow;
    spr->vy=0;
    
    if (tipo==0) {
        spr->sprite_class=sprite_classes[SPRCLS_ID_PEREGRINO];
        spr->n_hits=1;
    } else if (tipo==1) {
        spr->sprite_class=sprite_classes[SPRCLS_ID_BORDONERO];        
        spr->n_hits=2;
    } else if (tipo==2) {
        spr->sprite_class=sprite_classes[SPRCLS_ID_CANSADO];
        spr->vx=0;
    } else if (tipo==3) {
        spr->sprite_class=sprite_classes[SPRCLS_ID_BICI];
        spr->vx=3*c_v_fast;
        audio_una_bici_mas();
        audio_play_other_clip(AUDIO_SND_BOCINA);
        render_show_bocina();
    }
    
    spr->anim_state=&(spr->sprite_class->animstate[0]);
    spr->anim_state_id=0;
    
    spr->anim_lastT=world.t;
}


void world_post_animation_action(t_spritePtr spr, t_id fromid, t_id toid) {
//    printf("post action... f %d t %d \n",fromid,toid);
    if (spr->is_player) {
        if (fromid==STID_PLAYER_HIT) {
         //   spr->vx=c_v_fast;
        }
        if (fromid==STID_PLAYER_PREPARINGHIT) {
         //   spr->vx=0;
        }        
        if (fromid==STID_PLAYER_PREPARINGJUMP) {
            spr->vx*=world_jump_vx_fraction;
            spr->vy=world_jump_vy;
            world_lose_life(c_life_loss_per_jump);
        }
        if (fromid==STID_PLAYER_FALLEN) {
            spr->vx=c_v_slow;
            world.player_untouchable_time=world.t+invulnerability_time;
            achievments_fallen();
        }
        if (fromid==STID_PLAYER_SIDE) {
            spr->vx=c_v_slow;
        }
        if (fromid==STID_PLAYER_TIRED) {
            game_end();
        }
    } else {
        if (fromid==STID_WALKER_RECEIVED_HIT) {
            spr->vx=c_v_slow;
            spr->n_hits=2;
        }
        if (fromid==STID_WALKER_RECEIVING_HIT) {
            spr->vx=c_v_slow;
            spr->n_hits=2;
        }
    }
    
}



void world_handle_collision_candidate(t_spritePtr player, t_spritePtr otro) {

    // aqui antes se eliminan los que no pueden colisionar por su estado
    
    // no se colisiona con los apartados
    if ( (otro->sprite_class->id==SPRCLS_ID_PEREGRINO || otro->sprite_class->id==SPRCLS_ID_BORDONERO)
        && (otro->anim_state_id==STID_WALKER_STOP) ) {
        return;
    }
    
    // no se colisiona si vas despacio
    if ( (otro->sprite_class->id==SPRCLS_ID_PEREGRINO || otro->sprite_class->id==SPRCLS_ID_BORDONERO || otro->sprite_class->id==SPRCLS_ID_CANSADO) && (world.player.anim_state_id<STID_PLAYER_STOP) && world.player.vx<c_v_fast) {
        return;
    }
    
    if (world_collision_check(player, otro)) {
        world_handle_collision(player,otro);
    }
}

// ojo que la x del player se interpreta diferente
t_bool world_collision_check(t_spritePtr player, t_spritePtr otro) {
    t_bbPtr player_bb,otro_bb;
    t_anim_statePtr player_anim,otro_anim;
    t_pos sx,sy;
    t_bool result;
    player_anim=player->anim_state;
    otro_anim=otro->anim_state;
    player_bb=&(player_anim->col_bounds);
    otro_bb=&(otro_anim->col_bounds);
    sx=abs( -player_bb->off_x+0.5*player_bb->w - (otro->x-otro_bb->off_x+0.5*otro_bb->w) );
    sy=abs( player->y-player_bb->off_y+0.5*player_bb->h - (otro->y-otro_bb->off_y+0.5*otro_bb->h) );
    sx-=0.5*player_bb->w+0.5*otro_bb->w;
    sy-=0.5*player_bb->h+0.5*otro_bb->h;
    result= ( sx<0.0 && sy<0.0 );
    return result;
}


// ojo que la x del player se interpreta diferente
t_bool world_nonplayer_collision_check(t_spritePtr uno, t_spritePtr otro) {
    t_bbPtr uno_bb,otro_bb;
    t_anim_statePtr uno_anim,otro_anim;
    t_pos sx,sy;
    t_bool result;
    uno_anim=uno->anim_state;
    otro_anim=otro->anim_state;
    uno_bb=&(uno_anim->col_bounds);
    otro_bb=&(otro_anim->col_bounds);
    sx=abs( uno->x-uno_bb->off_x+0.5*uno_bb->w - (otro->x-otro_bb->off_x+0.5*otro_bb->w) );
    sy=abs( uno->y-uno_bb->off_y+0.5*uno_bb->h - (otro->y-otro_bb->off_y+0.5*otro_bb->h) );
    sx-=0.5*uno_bb->w+0.5*otro_bb->w;
    sy-=0.5*uno_bb->h+0.5*otro_bb->h;
    result= ( sx<0.0 && sy<0.0 );
    return result;
}

                    
                    
void world_handle_collision(t_spritePtr player, t_spritePtr otro) {
    t_id player_state, otro_id, otro_state;
    player_state=player->anim_state->id;
    otro_state=otro->anim_state->id;
    otro_id=otro->sprite_class->id;
    t_bool invulnerable=(world.player_untouchable_time>world.t);

    if (otro_id==SPRCLS_ID_BICI && player_state<STID_PLAYER_FALL) {  // una bici atropella al player
        //player->vx=c_v_fast;
        if (invulnerable) return;
        if (player_state<STID_PLAYER_JUMP) {
            player->vy=120;
        }
        sprite_set_state(player, STID_PLAYER_FALL);
        world_lose_life(c_life_loss_per_hit);
        audio_play_player_clip(AUDIO_SND_PEREGRINO_OW);
        stats_add_caida();
        achievments_hitbybike();
    } else if (otro_id==SPRCLS_ID_CANSADO && player_state<STID_PLAYER_FALL) {   // choco con un cansado
        if (invulnerable) return;
        if (player_state!=STID_PLAYER_HIT) {
            if (player_state<STID_PLAYER_JUMP) {
                player->vy=100;
            }
            sprite_set_state(player, STID_PLAYER_FALL);
            world_lose_life(c_life_loss_per_hit);
            audio_play_player_clip(AUDIO_SND_PEREGRINO_OW);
            stats_add_caida();
        } else {
            // para hacer si le pegas a un cansado
            sprite_set_state(player, STID_PLAYER_WALK);
        }
    } else if ( (otro_id==SPRCLS_ID_PEREGRINO || otro_id==SPRCLS_ID_BORDONERO) && player_state==STID_PLAYER_HIT ) {   // player pega a un andante
        if (otro_state==STID_WALKER_RECEIVING_HIT) {
            return;
        }
        otro->vx=0;
        otro->n_hits-=1;
        if (otro->n_hits<=0) {
            if (otro_id==SPRCLS_ID_BORDONERO) {
                otro->n_hits+=1;
                sprite_set_state(otro, STID_WALKER_STOP);
                audio_play_golpe_hit();
            } else {
                sprite_set_state(otro, STID_WALKER_RECEIVING_HIT);
                audio_play_golpe_hit();
                stats_add_hit(SPRCLS_ID_PEREGRINO);
            }
        } else {
            sprite_set_state(otro, STID_WALKER_RECEIVING_HIT);
            audio_play_golpe_hit();
            stats_add_hit(SPRCLS_ID_BORDONERO);
        }
        achievments_hitwalker();
    } else if ( (otro_id==SPRCLS_ID_PEREGRINO || otro_id==SPRCLS_ID_BORDONERO) && player_state==STID_PLAYER_JUMP && otro_state<STID_WALKER_STOP) { // choco en vuelo con un andante
        if (invulnerable) return;
        sprite_set_state(player, STID_PLAYER_FALL);
        world_lose_life(c_life_loss_per_hit);
        audio_play_player_clip(AUDIO_SND_PEREGRINO_OW);
        stats_add_caida();
        // el otro tambien se cae
        sprite_set_state(otro, STID_WALKER_STOP);
        otro->vx=0;
    }
    
    

}
    

t_bool world_item_collision_check(t_spritePtr player, t_itemPtr otro) {
    t_bbPtr player_bb,otro_bb;
    t_anim_statePtr player_anim;
    t_item_classPtr otro_class;
    t_pos sx,sy;
    t_bool result;
    player_anim=player->anim_state;
    otro_class=otro->item_class;
    player_bb=&(player_anim->col_bounds);
    otro_bb=&(otro_class->col_bounds);
    sx=abs( -player_bb->off_x+0.5*player_bb->w - (otro->x-otro_bb->off_x+0.5*otro_bb->w) );
    sy=abs( player->y-player_bb->off_y+0.5*player_bb->h - (otro->y-otro_bb->off_y+0.5*otro_bb->h) );
    sx-=0.5*player_bb->w+0.5*otro_bb->w;
    sy-=0.5*player_bb->h+0.5*otro_bb->h;
    result= ( sx<0.0 && sy<0.0 );
    return result;
}



void world_gain_life(float g) {
    world.player_vida+=g;
    if ( world.player_vida>1.0 ) world.player_vida=1.0;
}

void world_lose_life(float l) {
    world.player_vida-=l;
    // check for die
    if ( world.player_vida<0.0 ) {
        world.player_vida=0.0;
    }    
}





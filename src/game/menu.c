// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"

#define MENU_ST_MENU 0
#define MENU_ST_HELP 1
#define MENU_ST_HELPSTART 2
#define MENU_ST_HOF 3
#define MENU_ST_CREDITS 4
#define MENU_ST_ENTERHOF 5
#define MENU_ST_RESULTS 6


#define MAX_GAMES_AUTO_SHOW_HELP 3


//int gamecenter_enter_hof_or_achievments=0;


int menu_estado;

int menu_help_page=0;

float menu_pilgrim_tt=0.3;
float menu_pilgrim_x0=500.0;
float menu_pilgrim_y0=0.0;
float menu_pilgrim_xf=30.0;
float menu_pilgrim_yf=30.0;
float menu_pilgrim_x;
float menu_pilgrim_y;


float menu_title_tt=0.5;
float menu_title_x0=1100.0;
float menu_title_y0=425.0;
float menu_title_xf=400.0;
float menu_title_yf=425.0;
float menu_title_x;
float menu_title_y;



float menu_credits_tt=1.5;
float menu_credits_x0=30.0;
float menu_credits_y0=-700.0;
float menu_credits_xf=30.0;
float menu_credits_yf=20.0;
float menu_credits_x;
float menu_credits_y;

float menu_hof_tt=1.5;
float menu_hof_x0=30.0;
float menu_hof_y0=600.0;
float menu_hof_xf=30.0;
float menu_hof_yf=20.0;
float menu_hof_x;
float menu_hof_y;




void menu_init(void) {
    game_init_menu_controls=1;
    menu_pilgrim_x=menu_pilgrim_x0;
    menu_pilgrim_y=menu_pilgrim_y0;
    menu_title_x=menu_title_x0;
    menu_title_y=menu_title_y0;
    time_init();
    menu_estado=MENU_ST_MENU;
//    printf("M  menu\n");
}



void menu_update(t_time t) {
    menu_render();

    
    if (menu_estado==MENU_ST_MENU) {
        if (t<menu_pilgrim_tt) {
            float alfa=(menu_pilgrim_tt-t)/menu_pilgrim_tt;
            menu_pilgrim_x=alfa*menu_pilgrim_x0+(1-alfa)*menu_pilgrim_xf;
            menu_pilgrim_y=alfa*menu_pilgrim_y0+(1-alfa)*menu_pilgrim_yf;
        } else {
            menu_pilgrim_x=menu_pilgrim_xf;
            menu_pilgrim_y=menu_pilgrim_yf;
        }
        if (t<menu_title_tt) {
            float alfa=(menu_title_tt-t)/menu_title_tt;
            menu_title_x=alfa*menu_title_x0+(1-alfa)*menu_title_xf;
            menu_title_y=alfa*menu_title_y0+(1-alfa)*menu_title_yf;
        } else {
            menu_title_x=menu_title_xf;
            menu_title_y=menu_title_yf;
        }
    } else if (menu_estado==MENU_ST_CREDITS) {
        if (t<menu_credits_tt) {
            float alfa=(menu_credits_tt-t)/menu_credits_tt;
            menu_credits_x=alfa*menu_credits_x0+(1-alfa)*menu_credits_xf;
            menu_credits_y=alfa*menu_credits_y0+(1-alfa)*menu_credits_yf;
        } else {
            menu_credits_x=menu_credits_xf;
            menu_credits_y=menu_credits_yf;
        }
        

    } else if (menu_estado==MENU_ST_HOF) {
        if (t<menu_hof_tt) {
            float alfa=(menu_hof_tt-t)/menu_hof_tt;
            menu_hof_x=alfa*menu_hof_x0+(1-alfa)*menu_hof_xf;
            menu_hof_y=alfa*menu_hof_y0+(1-alfa)*menu_hof_yf;
        } else {
            menu_hof_x=menu_hof_xf;
            menu_hof_y=menu_hof_yf;
        }
        
        
    }
    
    
    
    if (menu_estado==MENU_ST_MENU) {
        if ( game_hit_pressed ) {
            if (persistence_total_played_games<MAX_GAMES_AUTO_SHOW_HELP) {
                menu_help(1);
            } else {
                game_start();
            }
        } else if (game_stop_pressed) {
            menu_help(0);
        } else if (game_esquivar_pressed) {
            menu_hof();
        } else if (game_salto_pressed) {
            menu_credits();
        } else if (game_other_pressed) {
            audio_switch_deactivate();
        }
    } else if (menu_estado==MENU_ST_CREDITS || menu_estado==MENU_ST_HOF) {
        
        if ( game_other_pressed ) {
            if (menu_estado==MENU_ST_HOF) {
                audio_stop_music();
                audio_start_music(AUDIO_MUSIC_MENU);
            }
            
            menu_init();
        }
        if ( game_other2_pressed ) {
            if (menu_estado==MENU_ST_HOF) {
//                if (gamecenter_enter_hof_or_achievments==0) {
//                    game_enter_gamecenter_hof=1;
//                    gamecenter_enter_hof_or_achievments=1;
//                } else {
//                    game_enter_gamecenter_achievments=1;
//                    gamecenter_enter_hof_or_achievments=0;
//                }
                game_enter_gamecenter_hof=1;
            }
        }
        if (game_stop_pressed) {
            if (menu_estado==MENU_ST_HOF) {
                game_enter_gamecenter_achievments=1;
            }
        }

#if PILGRIM_INCLUDE_WEBLINK
        if (menu_estado==MENU_ST_CREDITS && game_other2_pressed) {
            game_jump_to_weblink=1;
        }
#endif
            
    } else if (menu_estado==MENU_ST_RESULTS) {
        if ( game_other_pressed ) {
            if (game_highscore_pos!=-1) {
                menu_enter_hof();
            } else {
                menu_init();
            }
        }
    } else if (menu_estado==MENU_ST_HELP || menu_estado==MENU_ST_HELPSTART) {
        if (game_other_pressed) {
            menu_help_page+=1;
            if (menu_help_page==3) {
                if (menu_estado==MENU_ST_HELPSTART) {
                    game_start();
                } else {
                    menu_init();
                }
            }
        }
        if (game_other2_pressed) {
            menu_help_page-=1;
            if (menu_help_page==-1) {
                menu_init();
            }
        }

        
    } else if (menu_estado==MENU_ST_ENTERHOF) {
        if (game_highscore_name_entered) {
            game_highscore_name_entered=0;
            //highscore_print();
            //printf("nuevo record de %s\n",game_highscore_name);
            highscore_insert_name_pos(game_highscore_name, game_last_score, game_highscore_pos);
            //highscore_print();
            menu_hof();
        }
    }

    
        
}


void menu_help(int andstart) {
    game_init_backandfw_help_controls=1;
    
    menu_estado=andstart?MENU_ST_HELPSTART:MENU_ST_HELP;
//    printf("M  help\n");
    menu_help_page=0;
    
}

void menu_hof(void) {
    game_init_backandgamecenterscores_controls=1;
    menu_estado=MENU_ST_HOF;
    audio_stop_music();
    audio_start_music(AUDIO_MUSIC_HIGHSCORES);
//    printf("M  hof\n");
    menu_hof_x=menu_hof_x0;
    menu_hof_y=menu_hof_y0;
    text_highscores_gentexture(1024,1024);
    time_init();

}

void menu_enter_hof(void) {
    game_show_name_dialog=1;
    game_init_clicktocontinue_controls=1;
    menu_estado=MENU_ST_ENTERHOF;
//    printf("M enter hof\n");    
}

void menu_results(void) {
    game_init_clicktocontinue_controls=1;
    menu_estado=MENU_ST_RESULTS;
    //text_results_gentexture(1024,1024);
//    printf("M results\n");    
}

void menu_credits(void) {
#if PILGRIM_INCLUDE_WEBLINK
    game_init_backandlink_credits_controls=1;
//    game_init_clicktocontinue_controls=1;
#else
    game_init_clicktocontinue_controls=1;
#endif
    menu_estado=MENU_ST_CREDITS;
//    printf("M  credits\n");
//    game_show_credits=1;
    menu_credits_x=menu_credits_x0;
    menu_credits_y=menu_credits_y0;
    time_init();


}


void menu_render(void) {
    switch (menu_estado) {
        case MENU_ST_MENU:
            menu_render_menu();
            break;
        case MENU_ST_HELP:
        case MENU_ST_HELPSTART:
            menu_render_help();
            break;
        case MENU_ST_HOF:
            menu_render_hof();
            break;
        case MENU_ST_ENTERHOF:
            menu_render_enter_hof();
            break;
        case MENU_ST_RESULTS:
            menu_render_results();
            break;
        case MENU_ST_CREDITS:
            menu_render_credits();
            break;    
    }
} 
    
void menu_render_menu(void) {
    render_cam(render_display_w,render_display_h);
    render_tile_num(0,0,AST_TILE_MENU_FONDO);
    
//    render_tile_num(800,500,AST_TILE_CIRCLE1);
    render_tile_num(500,5,AST_TILE_CIRCLE);
    render_tile_num(340,95,AST_TILE_CIRCLE);
    render_tile_num(200,200,AST_TILE_CIRCLE);
    render_tile_num(40,280,AST_TILE_CIRCLE);
// render_tile_num(125,220,AST_TILE_CIRCLE3);
    
    render_tile_num(menu_pilgrim_x,menu_pilgrim_y,AST_TILE_MENU_PHIL);
    render_tile_num(menu_title_x,menu_title_y,AST_TILE_MENU_TITLE);
    
//    render_tile(30,20,&text_menu_credits_tile);
    render_tile_num(590,35,AST_TILE_MENU_CREDITS);
//    render_tile(30,130,&text_menu_start_tile);
    render_tile_num(380,150,AST_TILE_MENU_HIGHSCORES);
//    render_tile(30,240,&text_menu_hof_tile);
    render_tile_num(260,270,AST_TILE_MENU_HELP);
//    render_tile(30,350,&text_menu_help_tile);
    render_tile_num(100,350,AST_TILE_MENU_PLAY);
//    render_tile(600,0,&text_menu_start_selected_tile);
//    render_tile(600,60,&text_menu_help_selected_tile);
//    render_tile(600,120,&text_menu_credits_selected_tile);
//    render_tile(600,180,&text_menu_hof_selected_tile);

    if (!audio_deactivated) {
        render_tile_num(40,40,AST_TILE_ICN_SND_ON);
    } else {
        render_tile_num(40,39,AST_TILE_ICN_SND_OFF);        
    }

    
//    render_label_num(100,100,0);
//    render_label_num(0,0,15);
//    render_numstr(500,100,"12345678",0);
//    render_numstr(500,90,"12349",1);
}

void menu_render_help(void) {
    render_cam(render_display_w,render_display_h);
//    render_tile_num(0,0,AST_TILE_MENU_FONDO);
    if (menu_help_page==0) {
        render_tile_num(0,0,AST_TILE_INSTRUCCIONES1);
    } else if (menu_help_page==1) {
        render_tile_num(0,0,AST_TILE_INSTRUCCIONES2);
    } else {
        render_tile_num(0,0,AST_TILE_INSTRUCCIONES3);        
    }
    render_tile_num(871-10,100+10,AST_TILE_BTN_NEXT);
    render_tile_num(0+10,100+10,AST_TILE_BTN_BACK);
}

void menu_render_hof(void) {
    render_cam(render_display_w,render_display_h);
    render_tile_num(0,0,AST_TILE_MENU_FONDO);
    render_tile(menu_hof_x,menu_hof_y,&text_highscores_tile);
    render_tile_num(871-10,0+10,AST_TILE_BTN_NEXT);
// botones del gamecenter
    if (game_gamecenter_available) {
        render_tile_num(871-10-30,540+10-20,AST_TILE_BTN_GC_LEADERBOARDS);
        render_tile_num(782-10-60,540+10-20,AST_TILE_BTN_GC_ACHIEVMENTS);
    }
}

void menu_render_enter_hof(void) {
    render_cam(render_display_w,render_display_h);
//    render_tile_num(0,0,AST_TILE_MENU_FONDO);
//#if TARGET_OS_IPHONE
//    render_tile(30,20,&text_highscores_tile);
//#else
//    render_tile(30,-20,&text_highscores_tile);
//#endif
#if !TARGET_OS_IPHONE
    glTranslatef(0, -50, 0);
#endif
    menu_render_results_panel();
}

void menu_render_results(void) {
    render_cam(render_display_w,render_display_h);
    menu_render_results_panel();
    render_tile_num(871-10,0+10,AST_TILE_BTN_NEXT);
}

void menu_render_credits(void) {
    render_cam(render_display_w,render_display_h);
    render_tile_num(0,0,AST_TILE_MENU_FONDO);
   // render_numstr(300, 500, "0100001010010101...", 0);
    render_tile(menu_credits_x,menu_credits_y,&text_credits_tile);
    render_tile_num(menu_credits_x+620,menu_credits_y+415,AST_TILE_MENU_TITLE);
    render_tile_num(871-10,0+10,AST_TILE_BTN_NEXT);
}

void menu_render_results_panel(void) {
    float top_line=600;
    render_tile_num(0,0,AST_TILE_MENU_FONDO);
    render_tile(30,20,&text_highscores_tile);
    render_tile_num(40,top_line-1*80,AST_TILE_ICN_DISTANCE);
    render_tile_num(290,top_line-1*80,AST_TILE_ICN_TIME);
    //render_tile_num(510,top_line-2*80,AST_TILE_ICN_SPEED);
    render_tile_num(520,top_line-1*80,AST_TILE_ICN_PEREGRINO_HIT);
    render_tile_num(640,top_line-1*80,AST_TILE_ICN_BORDONERO_HIT);
    //render_tile_num(740,top_line-1*80,AST_TILE_ICN_FALLS);
    render_tile_num(820,top_line-1*80+5,AST_TILE_ICN_CARNE);
    render_tile_num(835,top_line-1*80,AST_TILE_ICN_FRUTA);    
}


// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


extern t_assets_tile_info text_credits_tile;
extern t_assets_tile_info text_highscores_tile;

extern t_assets_tile_info text_menu_start_tile;
extern t_assets_tile_info text_menu_start_selected_tile;
extern t_assets_tile_info text_menu_help_tile;
extern t_assets_tile_info text_menu_help_selected_tile;
extern t_assets_tile_info text_menu_credits_tile;
extern t_assets_tile_info text_menu_credits_selected_tile;
extern t_assets_tile_info text_menu_hof_tile;
extern t_assets_tile_info text_menu_hof_selected_tile;

extern t_assets_tile_info text_gameover_tile;

void text_init(void);
void text_credits_init(void);
int text_credits_gettexture(int tex_container_w, int tex_container_h);
void text_highscores_gentexture(int tex_container_w, int tex_container_h);
void text_results_gentexture(int tex_container_w, int tex_container_h);
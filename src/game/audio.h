// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

/// \file
/// \brief Funciones para el manejo de sonidos

#import <OpenAL/al.h>
#import <OpenAL/alc.h>

#define AUDIO_SRC_PLAYER_BASE 0
#define AUDIO_SRC_PLAYER_N 3
#define AUDIO_SRC_OTHER_BASE 3
#define AUDIO_SRC_OTHER_N 3
#define AUDIO_SRC_BICIS 6
// experimental source para poner sonidos queued para hacer el golpe
#define AUDIO_SRC_GOLPES 7

#define AUDIO_SND_HIT 0
#define AUDIO_SND_GOLPE 1
#define AUDIO_SND_BICI 2
#define AUDIO_SND_PEREGRINO_OW 3
#define AUDIO_SND_SALTO 4
#define AUDIO_SND_CAIDA 5
#define AUDIO_SND_GAMEOVER 6
#define AUDIO_SND_BOCINA 7
#define AUDIO_SND_CAIDABIEN 8
#define AUDIO_SND_COMER 9
#define AUDIO_SND_BEEP 10


#define AUDIO_MUSIC_COMIENZO 0
#define AUDIO_MUSIC_TEMA 1
#define AUDIO_MUSIC_HIGHSCORES 2
#define AUDIO_MUSIC_MENU 3



extern int audio_deactivated;


/// Array de fuentes de sonido
extern ALuint *audio_source;

/// Array de buffers de sonidos
extern ALuint *audio_snd_buffer;

/// Array de buffers de musicas
extern ALuint *audio_mus_buffer;


/// Inicializa el sistema de audio indicando numero de buffers
void audio_init(int nsounds, int nmusics);

/// iniciando el sistema de audio para una partida
void audio_restart(void);

void audio_activate(void);
void audio_deactivate(void);
void audio_switch_deactivate(void);

/// Reproduce un clip asociandola al jugador
void audio_play_player_clip(int i);

void audio_play_golpe_inicio(void);
void audio_play_golpe_hit(void);

/// Reproduce un clip asociandolo a otra fuente no relacionada con el jugador
void audio_play_other_clip(int i);

/// Empieza a reproducir la musica
void audio_start_music(int n);

/// Aviso de una bici mas para gestionar el audio de bicis
void audio_una_bici_mas(void);

/// Aviso de una bici menos para gestionar el audio de bicis
void audio_una_bici_menos(void);

/// Para la musica
void audio_stop_music(void);

/// Debe ser llamado frecuentemente para mantener la musica
void audio_keep_music(void);


void audio_low_warning_play(int ifreq);


/// Imprime estado de openal para depurar
void audio_print_openalerrorstate_at(char *context);


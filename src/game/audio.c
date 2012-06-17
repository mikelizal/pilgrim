// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"


int audio_deactivated=0;


ALuint *audio_source;
ALuint *audio_snd_buffer;
ALuint *audio_mus_buffer;
ALuint audio_music_source;
ALuint audio_menumusic_source;

void print_source_state(ALint source, char *sourcename);

int audio_current_music=0;

int next_player_source, next_other_source;

int audio_pre_hit=0;

int audio_cuenta_bicis;

t_time audio_lowwarning_last_t=0;

void audio_init(int nsounds, int nmusics) {
    ALenum err;
        
    ALCdevice *audiodev;
    audiodev=alcOpenDevice(NULL);
    if (audiodev==NULL) {
        printf("audio error!\n");
        exit(0);
    }
    ALCcontext *audioctx;
    audioctx=alcCreateContext(audiodev, NULL);
    /// no se porque esta instruccion genera datos por la salida estandar
    alcMakeContextCurrent(audioctx);
    
    audio_source=malloc(8*sizeof(ALuint));
    audio_snd_buffer=malloc(nsounds*sizeof(ALuint));
    audio_mus_buffer=malloc(nmusics*sizeof(ALuint));
    
    err=alGetError();
    alGenBuffers(nsounds, audio_snd_buffer);
    alGenBuffers(nmusics, audio_mus_buffer);
//    audio_print_openalerrorstate_at("creating audio buffers: ");

//    int i;
//    printf("sndbufs "); for (i=0;i<nsounds;i++) { printf("%d ",audio_snd_buffer[i]); } printf("\n");
//    printf("musbufs "); for (i=0;i<nmusics;i++) { printf("%d ",audio_mus_buffer[i]); } printf("\n");
    alGenSources(8, audio_source);   // 6 sources 0-2 sonidos del jugador  3-5 sonidos de otros 6 bicis se mantiene looping mientras haya bicis
    alGenSources(1, &audio_music_source);
    alGenSources(1, &audio_menumusic_source);

//    audio_print_openalerrorstate_at("creating audio sources: ");
    

}

void audio_restart(void) {
    audio_cuenta_bicis=0;
    audio_lowwarning_last_t=0;
}

void audio_activate(void) {
    audio_deactivated=0;
    audio_start_music(AUDIO_MUSIC_MENU);
}

void audio_deactivate(void) {
    audio_stop_music();
    audio_deactivated=1;
}

void audio_switch_deactivate(void) {
    if (audio_deactivated) {
        audio_activate();
    } else {
        audio_deactivate();
    }
}


// play a clip in the player queues
void audio_play_player_clip(int i) {
    if (audio_deactivated) return;
    alGetError();
    alSourceStop(audio_source[next_player_source]);
    alSourcei(audio_source[next_player_source],AL_BUFFER,audio_snd_buffer[i]);
    alSourcef(audio_source[next_player_source], AL_GAIN, 1.0);    
    alSourcePlay(audio_source[next_player_source]);

    //audio_print_openalerrorstate_at("play player clip: ");
        
    int offset=next_player_source-AUDIO_SRC_PLAYER_BASE;
    offset+=1;
    offset%=AUDIO_SRC_PLAYER_N;
    next_other_source=AUDIO_SRC_PLAYER_BASE+offset;
    
}

// play a clip in the other queues
void audio_play_other_clip(int i) {
    if (audio_deactivated) return;
    alGetError();
    alSourceStop(audio_source[next_other_source]);
    alSourcei(audio_source[next_other_source],AL_BUFFER,audio_snd_buffer[i]);
    alSourcef(audio_source[next_other_source], AL_GAIN, 1.0);    
    alSourcePlay(audio_source[next_other_source]);

    //audio_print_openalerrorstate_at("play other clip: ");
    
    int offset=next_other_source-AUDIO_SRC_OTHER_BASE;
    offset+=1;
    offset%=AUDIO_SRC_OTHER_N;
    next_other_source=AUDIO_SRC_OTHER_BASE+offset;
}


void audio_play_golpe_inicio(void) {
    if (audio_deactivated) return;

    //alSourceStop(audio_source[AUDIO_SRC_GOLPES]);
    //alSourcei(audio_source[AUDIO_SRC_GOLPES],AL_BUFFER,audio_snd_buffer[AUDIO_SND_GOLPE]);

    audio_pre_hit=1;

    ALint state;
    alGetSourcei(audio_source[AUDIO_SRC_GOLPES], AL_SOURCE_STATE, &state);
//    if (state==AL_PLAYING) {
//        printf("golpe source still playing !!\n");
//    }
    
    //printf("queuing buffer init at source %d\n",audio_source[AUDIO_SRC_GOLPES]);
    alSourceStop(audio_source[AUDIO_SRC_GOLPES]);
    alSourcei(audio_source[AUDIO_SRC_GOLPES],AL_BUFFER,0);
    alSourceQueueBuffers(audio_source[AUDIO_SRC_GOLPES],1,&(audio_snd_buffer[AUDIO_SND_GOLPE]));
    alSourcePlay(audio_source[AUDIO_SRC_GOLPES]);
}

void audio_play_golpe_hit(void) {
    // cambiado a no esforzarse por hacer el hit al final del golpe
    // eso podria simplificar bastante y quitarse las funciones audio
    // player_golpe_inicio y _hit y dejarlas simplemente ne play_player_clips
    if (audio_deactivated) return;
  
    
    audio_play_player_clip(AUDIO_SND_HIT);

    
    return;
    
    if (!audio_pre_hit) {
        return;
    }
    audio_pre_hit=0;
    //printf("queuing buffer hit at source %d\n",audio_source[AUDIO_SRC_GOLPES]);
    alSourceQueueBuffers(audio_source[AUDIO_SRC_GOLPES],1,&(audio_snd_buffer[AUDIO_SND_HIT]));
//    alSourceStop(audio_source[AUDIO_SRC_GOLPES]);
    //    alSourcei(audio_source[AUDIO_SRC_GOLPES],AL_BUFFER,audio_snd_buffer[AUDIO_SND_HIT]);
//    alSourcePlay(audio_source[AUDIO_SRC_GOLPES]);    
}

void audio_start_music(int n) {
    audio_current_music=n;
    if (audio_deactivated) return;

//    printf("starting mus buffer %d %d\n",n,audio_mus_buffer[n]);
    if (n==AUDIO_MUSIC_MENU || n==AUDIO_MUSIC_HIGHSCORES) {
        //alSourceStop(audio_menumusic_source);
        alSourcei(audio_menumusic_source,AL_BUFFER,audio_mus_buffer[n]);        
        alSourcei(audio_menumusic_source, AL_LOOPING, AL_TRUE);
        alSourcePlay(audio_menumusic_source);
    } else {
        alSourceQueueBuffers(audio_music_source, 1, &(audio_mus_buffer[n]));

//    alSourcei(audio_music_source, AL_LOOPING, AL_TRUE);
//    alSourcef(audio_music_source, AL_GAIN, 1.0);    
        if (n==AUDIO_MUSIC_COMIENZO) {
            alSourceQueueBuffers(audio_music_source, 1, &(audio_mus_buffer[AUDIO_MUSIC_TEMA]));        
            alSourceQueueBuffers(audio_music_source, 1, &(audio_mus_buffer[AUDIO_MUSIC_TEMA]));        
        //alSourceUnqueueBuffers(audio_music_source, 1, unqueuedbufs);
        //printf("unqueued buffer [[[ %d ]]]\n",unqueuedbufs[0]);
        }
        //alSourceUnqueueBuffers(audio_music_source, 1, unqueuedbufs);
       // printf("unqueued buffer [[[ %d ]]]\n",unqueuedbufs[0]);
//        print_source_state(audio_music_source,"music source despues del if");
        alSourcePlay(audio_music_source);
//        print_source_state(audio_music_source,"music source despues del play");    
//        audio_print_openalerrorstate_at("queueing buffer to start music");

    }

}

void audio_una_bici_mas(void) {
    if (audio_deactivated) return;
    
    ALint state;
    alGetSourcei(audio_source[AUDIO_SRC_BICIS],AL_SOURCE_STATE,&state);
    //printf("una bici mas...\n");
    if (state!=AL_PLAYING) {
        alSourcei(audio_source[AUDIO_SRC_BICIS],AL_BUFFER,audio_snd_buffer[AUDIO_SND_BICI]);
        alSourcei(audio_source[AUDIO_SRC_BICIS],AL_LOOPING,AL_TRUE);
        alSourcePlay(audio_source[AUDIO_SRC_BICIS]);
        //printf("snd bici...\n");
    }
    audio_cuenta_bicis+=1;
}

void audio_una_bici_menos(void) {
    if (audio_deactivated) return;

    audio_cuenta_bicis-=1;
    if (audio_cuenta_bicis==0) {
        alSourceStop(audio_source[AUDIO_SRC_BICIS]);
    }
}



void audio_stop_music(void) {
    if (audio_deactivated) return;

    if (audio_current_music==AUDIO_MUSIC_MENU||audio_current_music==AUDIO_MUSIC_HIGHSCORES) {
        alSourceStop(audio_menumusic_source);
    } else if (audio_current_music==AUDIO_MUSIC_TEMA||audio_current_music==AUDIO_MUSIC_COMIENZO) {
   // ALuint unqueuedbuf[0];
   // alSourceUnqueueBuffers(audio_music_source, 1, unqueuedbuf);
        alSourceStop(audio_music_source);
        alSourcei(audio_music_source,AL_BUFFER,0);
    }
}

void audio_keep_music(void) {
    if (audio_deactivated) return;

    if (audio_current_music==AUDIO_MUSIC_COMIENZO||audio_current_music==AUDIO_MUSIC_TEMA) {
        //printf("keeping music!!!\n");
        ALint nprocessed;
        ALuint buf;
        alGetSourcei(audio_music_source,AL_BUFFERS_PROCESSED,&nprocessed);
        if (nprocessed>0) {
            alSourceUnqueueBuffers(audio_music_source, 1, &buf);
            //printf("unqueued music buffer %d\n",buf);
            if (buf==audio_mus_buffer[AUDIO_MUSIC_TEMA]) {
                //printf("feeding music...\n");
                alSourceQueueBuffers(audio_music_source, 1, &(audio_mus_buffer[AUDIO_MUSIC_TEMA]));        
            }
        }
    }
}




void audio_low_warning_play(int ifreq) {
    t_time dt=0.4/ifreq;
    if (world.t>audio_lowwarning_last_t+dt) {
//        printf("beep!!!\n");
        audio_play_player_clip(AUDIO_SND_BEEP);
        audio_lowwarning_last_t=world.t;
    }
}


void audio_print_openalerrorstate_at(char *context) {
    ALuint err;
    err=alGetError();
    char *errorStr;
    switch (err) {
        case AL_NO_ERROR:
            errorStr="OK No error";
            break;
        case AL_INVALID_NAME:
            errorStr="Error invalid name";
            break;
        case AL_INVALID_ENUM:
            errorStr="Error invalid enum";
            break;
        case AL_INVALID_VALUE:
            errorStr="Error invalid value";
            break;
        case AL_INVALID_OPERATION:
            errorStr="Error invalid operation";
            break;
        case AL_OUT_OF_MEMORY:
            errorStr="Error out of memory";
            break;
        default:
            errorStr="Unknown error code";
            break;
    }    
    
    printf("%s: %s\n",context,errorStr);
}

void print_source_state(ALint source, char *sourcename) {
    ALint p_buffers,q_buffers,s_status;
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &p_buffers);
    alGetSourcei(source, AL_BUFFERS_QUEUED, &q_buffers);
    printf("Source %d [%s] : ",source, sourcename);
    printf("p %d q %d",p_buffers,q_buffers);
    alGetSourcei(source, AL_SOURCE_STATE, &s_status);
    char *s_statusStr;
    switch (s_status) {
        case AL_PLAYING:
            s_statusStr="AL_PLAYING";
            break;
        case AL_PAUSED:
            s_statusStr="AL_PAUSED";
            break;
        case AL_STOPPED:
            s_statusStr="AL_STOPPED";
            break;
        default:
            s_statusStr="UNKNOWN STATE";
    }
    printf(" %s\n",s_statusStr);
}



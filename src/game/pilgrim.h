// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

// Doxygen main page for pilgrim_engine
/// \mainpage Pilgrim engine
///
/// \brief Documentacion del motor de juego de Pilgrim
///
/// Para usarlo se debe incluir el fichero pilgrim.h
/// que incluye a su vez a todos los necesarios
///


/// \file
/// \brief Pilgrim game engine
///
///   Para usarlo vale con incluir pilgrim.h
///

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>



#define PILGRIM_INCLUDE_WEBLINK 1
#define PILGRIM_WEBLINK "http://bwarestudios.com/"


#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/gl.h>
#endif




#include "types.h"
#include "time.h"
#include "sprite.h"
#include "item.h"
#include "game.h"
#include "render.h"
#include "world.h"
#include "assets.h"
#include "audio.h"
#include "generador.h"
#include "menu.h"
#include "text.h"
#include "highscore.h"
#include "persistence.h"
#include "stats.h"
#include "achievments.h"



extern char pilgrim_version_number[];
extern char pilgrim_build_number[];





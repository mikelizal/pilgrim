// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/gl.h>
#endif

@interface MG_GrafTexture : NSObject {
#if TARGET_OS_IPHONE
    UIImage *image;
#else
    NSImage *image;
#endif
    int width, height;
    int tex_w,tex_h;
    GLint tex_internalFormat;
    GLenum tex_type;
@public
    GLuint tex_name;
}

- (id) initWithFile:(NSString*)filename;

- (void) setInternalTextureFormat:(GLint)internalFormat type:(GLenum)type;
- (id) prepare;
- (void) drawAtX:(int)x Y:(int)y;


@end

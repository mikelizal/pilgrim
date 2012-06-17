// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "MG_GrafTexture.h"
#import <math.h>

@implementation MG_GrafTexture;

- (id) initWithFile:(NSString*)filename {
    printf("loading image %s\n",[filename UTF8String]);
#if TARGET_OS_IPHONE
    image=[UIImage imageNamed:filename];
#else
    image=[NSImage imageNamed:filename];
#endif
//    printf("%s\n",(image==NULL?"Error":"OK"));
    width=[image size].width;
    height=[image size].height;
    int buf_w,buf_h;
    buf_w=(int)pow(2,(1+(int)(log(width-1)/log(2))));
    buf_h=(int)pow(2,(1+(int)(log(height-1)/log(2))));
    if (buf_w>1024) buf_w=1024;
    if (buf_h>1024) buf_h=1024;
    tex_w=buf_w;
    tex_h=buf_h;
//    printf("image size [%d,%d]\n",width,height);
//    printf("texture size [%d,%d]\n",tex_w,tex_h);
    
    [self setInternalTextureFormat:GL_RGBA type: GL_UNSIGNED_BYTE];
    return self;
}

- (void) setInternalTextureFormat:(GLint)internalFormat type:(GLenum)type {
    tex_internalFormat=internalFormat;
    tex_type=type;
}


- (id) prepare {
    CGImageRef iref;    
    void *imagebuf;

#if TARGET_OS_IPHONE
    iref=[image CGImage];
#else
    NSRect r=NSMakeRect(0, 0, [image size].width, [image size].height);
    iref=[image CGImageForProposedRect:&r context:[NSGraphicsContext currentContext] hints:nil];
#endif
    
    imagebuf=calloc(tex_w*tex_h,4);
    CGContextRef bufCtx=CGBitmapContextCreate(imagebuf, tex_w, tex_h, 8, 4*tex_w, CGColorSpaceCreateDeviceRGB(), kCGImageAlphaPremultipliedLast);
#if TARGET_OS_IPHONE
    CGContextSetFillColorWithColor(bufCtx, [[UIColor greenColor] CGColor]);
#else
    CGColorRef green;
    green=CGColorCreateGenericRGB(0, 1, 0, 0);
    CGContextSetFillColorWithColor(bufCtx, green);    
    CGColorRelease(green);
#endif
    
    //    CGContextFillRect(bufCtx,CGRectMake(0, 0 , buf_w, buf_h));
//    printf("dibujando la textura para opengl en %d %d\n",width,height);
    CGContextDrawImage(bufCtx, CGRectMake(0, 0 , width, height), iref);

    // habria que comprobar que hay openglcontext y que no se genera el texname mas que una vez
    
    glGenTextures(1, &tex_name);
    glBindTexture(GL_TEXTURE_2D, tex_name);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, tex_internalFormat, tex_w, tex_h, 0, GL_RGBA, tex_type, imagebuf);
    GLuint err=glGetError();
    if (err != GL_NO_ERROR) {
        printf("glTexImage2D(): Error al cargar la textura e=%u\n",err);
    }
    
    CGContextRelease(bufCtx);
    free(imagebuf);
    return self;
}

- (void) drawAtX:(int)x Y:(int)y {
    const GLfloat squareVertices[] = {
        0, 1,
        0,  0,
        1,   1,
        1,   0,
    };           
    const GLfloat squareTC[] = {
        0, 0,
        0,   1,
        1,    0,
        1,   1,
        1,  1,
    };        
//    const GLubyte squareColors[] = {
//        25, 255,   25, 255,
//        25,   255, 25, 255,
//        25,     255,   25,   255,
//        25,   255, 25, 255,
//    };
//    const GLfloat lineVertices[] = {
//        0,0,0,
//        3,3,3,
//    };
    
    

    
    
    glVertexPointer(2, GL_FLOAT, 0, squareVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, squareTC);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, tex_name);    
    glPushMatrix();
    glTranslatef(x,y,0);
    glScalef(tex_h, tex_w, 1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();
    
//    glTranslatef(0,64,0);
//    glPushMatrix();
//    glScalef(width, height, 1);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glPopMatrix();
//
//    int i;
//    for (i=0;i<9;i++) {
//    glTranslatef(32,0,0);
//    glPushMatrix();
//    glScalef(width, height, 1);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glPopMatrix();
//    }
//    
//    
//    GLenum err=glGetError();
//    printf("GL_TRIANGLE_STRIP err=%u\n",err);

//    glVertexPointer(3, GL_FLOAT, 0, lineVertices);
//    glDrawArrays(GL_LINE_STRIP, 0, 2);
//    err=glGetError();
//    printf("GL_LINES err=%u\n",err);


}

@end

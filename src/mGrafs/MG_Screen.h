// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#if TARGET_OS_IPHONE
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#else
#import <OpenGL/gl.h>
#endif

@class MG_Control;

#if TARGET_OS_IPHONE
@interface MG_Screen : UIView {
#else
@interface MG_Screen : NSView {    
#endif
    // width and height of the screen (for openGL width and height with choosen orientation
    @public GLint width,height;
    // width and heigth of the UIView (with normal iPhone orientation)
    int view_w, view_h;
    // Orientacion de la pantalla 1 noraml 2 landscape right 3 landscape left 4 bocaabajo?
    @public int orientation;
    // privados la verdad no interesan fuera
    GLuint framebuffer, renderbuffer;
#if TARGET_OS_IPHONE
    EAGLContext *context;
    UIWindow *w;
#else
    NSOpenGLView *openglView;
    NSOpenGLContext *context;
    NSWindow *w;
#endif
    // A delegate of type MG_Control to receive touchevents
    MG_Control *controlDelegate;
    
}

+ (id)screen;
    
#if TARGET_OS_IPHONE
+ (Class)layerClass;
#endif

- (id)init;

- (id)initWithOrientation:(int)orient;
    
#if !TARGET_OS_IPHONE
- (id)initWithSize:(NSSize)size;
#endif

#if TARGET_OS_IPHONE
- (void)changeOrientation:(int)orient;
#endif
    
- (void)prepareScreen;

- (void)prepareFrame;

- (void)renderTest;

- (void)renderFrame;

- (void)setControlDelegate:(MG_Control*)delegate;

- (void)splashScreenWithFile:(NSString *)filename;


#if TARGET_OS_IPHONE
- (BOOL)canBecomeFirstResponder;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event;    
- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event;
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event;

#else
//- (BOOL)acceptsFirstResponder;
- (void)mouseDown:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
#endif
    
@end

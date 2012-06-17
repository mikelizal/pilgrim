// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "MG_Screen.h"

#import "MG_GrafTexture.h"

#import "MG_Control.h"

MG_Screen *theScreen=nil;

@implementation MG_Screen

+ (id)screen {
    return theScreen;
}

#if TARGET_OS_IPHONE
+ (Class)layerClass {
    return [CAEAGLLayer class];
}
#endif

-(id)init {
    return [self initWithOrientation:1];
}

-(void)setControlDelegate:(MG_Control*)delegate {
    self->controlDelegate=delegate;
}

-(id)initWithOrientation:(int)orient {
    orientation=orient;

#if TARGET_OS_IPHONE
    //printf("init MG_Screen v1\n");
    CGRect r=[[UIScreen mainScreen] bounds];
    
    
    if ([super initWithFrame:r]==nil) return nil;
    [self setBackgroundColor:[UIColor colorWithRed: 1.0 green: 1.0 blue: 1.0 alpha: 1.0]];
    [self setOpaque:YES];
    // Create Window and add the view
    //UIWindow *w;
    w=[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];

    if ([self respondsToSelector:@selector(contentScaleFactor)]) {
//        printf("Tengo retina display !! aumentando resolucion\n");
        self.contentScaleFactor=2.0f;
    }
    
    [w addSubview:self];


    [w makeKeyAndVisible];    

    [[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:NO];

    // openGL
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    context=[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    [EAGLContext setCurrentContext:context];

    // crear nombres para frame y renderbuffer
    glGenFramebuffersOES(1, &framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
    glGenRenderbuffersOES(1, &renderbuffer);  
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);    
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderbuffer);
    
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &view_w);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &view_h);

//    printf("screen view [%d,%d]\n",view_w,view_h);
//    glViewport(0,0, width, height);

    
    GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) ;
    if(status != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"failed to make complete framebuffer object %x", status);
    }
    
    //para probar si va opengl
    // poner aqui la apariencia por defecto de la ventana que dibujara MG_Screen
//    glClearColor(.3,.9,.3,1);
//    glClear(GL_COLOR_BUFFER_BIT);
//    [context presentRenderbuffer:GL_RENDERBUFFER_OES];

    
    if ( orientation==1 || orientation==4 ) {
        width=view_w;
        height=view_h;
    } else {
        width=view_h;
        height=view_w;
    }
    
//    printf("orientation=%d\n",orientation);
//    printf("opengl screen [%d,%d]\n",width,height);
        
    [self setMultipleTouchEnabled:YES];
    [self becomeFirstResponder];
    
    theScreen=self;
    return self;
#else
//    printf("redirection orientation: %u\n",orientation);
    if ( orientation==1 || orientation==4 ) {
        // iphone 4
//        return [self initWithSize:NSMakeSize(320,480)];
        return [self initWithSize:NSMakeSize(640,960)];
    } else {
//        return [self initWithSize:NSMakeSize(480,320)];
        return [self initWithSize:NSMakeSize(960,640)];
    }
#endif
}


#if !TARGET_OS_IPHONE
- (id)initWithSize:(NSSize)size {
//    printf("creating window...\n");
    //NSWindow *w
    w=[[NSWindow alloc] initWithContentRect:NSMakeRect(0,0,size.width,size.height) styleMask:NSTitledWindowMask|NSClosableWindowMask backing:NSBackingStoreBuffered defer:NO ];
    
    NSRect r = [[w screen] visibleFrame];
    
    width=view_w=size.width;
    height=view_h=size.height;
    
    //NSOpenGLPixelFormatAttribute atribs[]={
    //    0
    //};
    
    NSRect r2=NSMakeRect(0,0,size.width,size.height);
    openglView=[[NSOpenGLView alloc] initWithFrame:r2 pixelFormat:[NSOpenGLView defaultPixelFormat]];
    [super initWithFrame:r2];
    [w setContentView: self];
    [self addSubview: openglView];
    
    context=[openglView openGLContext];
    
    [w setFrameOrigin:NSMakePoint(r.size.width/2.0-size.width/2.0,r.size.height/2.0-size.height/2.0)];
    [w makeKeyAndOrderFront:self];
 
	
	//para probar si va opengl
    // poner aqui la apariencia por defecto de la ventana que dibujara MG_Screen
//    glClearColor(.3,.9,.3,1);
//    glClear(GL_COLOR_BUFFER_BIT);
//	glFlush();
	
//    printf("ignoresMouseEvents: %d  first responder:%x %x\n",[w ignoresMouseEvents],[w firstResponder],openglView);
    theScreen=self;

    return self;
}
#endif



#if TARGET_OS_IPHONE
- (void)changeOrientation:(int)orient {
    self->orientation=orient;
    [controlDelegate screenOrientationChangedTo:orient];
}
#endif


// Para ser llamada antes de utilizar el Screen para dibujar
// Cosas de openGL que no hace falta que se repitan cada frame
- (void)prepareScreen {
#if TARGET_OS_IPHONE
    [EAGLContext setCurrentContext:context];
#else
    [context makeCurrentContext];
#endif
    glViewport(0,0, view_w, view_h);

    // activar texturas
    glEnable(GL_TEXTURE_2D);
    // modo= replace
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // y esto para que se haga blend de los colores que genera el replace y el alfa funcione para transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    // estos aun estan por discutir
    // no son parametros generales son para la textura en curso... movidos al codigo de cargar texturas
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

// Para ser llamada antes de cada frame 
// cosas de openGL que hace falta que se repitan cada frame
// principalmente la camara
- (void)prepareFrame {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
#if TARGET_OS_IPHONE
    switch (orientation) {
        case 1:
            break;
        case 2:
            glRotatef(90,0,0,1);
            break;
        case 3:
            glRotatef(90,0,0,-1);
            break;
        case 4:
            glRotatef(180,0,0,1);
            break;
        default:
            break;
    }
#endif
    
//    glOrthof(0.0f, height*1.0f, 0.0f, width*1.0f, -1.0f, 1.0f);
#if TARGET_OS_IPHONE
    glOrthof(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
#else
    glOrtho(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
#endif
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // activar texturas
    glEnable(GL_TEXTURE_2D);
}

-(void)renderTest {
    const GLfloat vtxs[]={1,1,101,1,1,1,1,101,479,319,479,219,479,319,379,319,319,479,319,379,319,479,219,479,1,1,479,319,1,1,319,479,};

    glVertexPointer(2, GL_FLOAT, 0, vtxs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glColor4f(1,1,1,1);
    glDisable(GL_TEXTURE_2D);
    glDrawArrays(GL_LINES,0,16);
}

// Dibuja el frame
- (void)renderFrame {
    //[EAGLContext setCurrentContext:context];    
    //printf("rendering now\n");
//    [context presentRenderbuffer:GL_RENDERBUFFER_OES];

    // recupera las coordenadas por si el codigo de dibujolas cambia
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
#if TARGET_OS_IPHONE
    switch (orientation) {
        case 1:
            break;
        case 2:
            glRotatef(90,0,0,1);
            break;
        case 3:
            glRotatef(90,0,0,-1);
            break;
        case 4:
            glRotatef(180,0,0,1);
            break;
        default:
            break;
    }
#endif
	
	
#if TARGET_OS_IPHONE
    glOrthof(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
#else
    glOrtho(0.0f, width*1.0f, 0.0f, height*1.0f, -1.0f, 1.0f);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//  fin de recuperar
    [controlDelegate render];
    
#if TARGET_OS_IPHONE
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
#else

    glFlush();
    //[context flushBuffer];
#endif
}

- (void)splashScreenWithFile:(NSString *)filename {
    MG_GrafTexture *splash_texture;
    splash_texture=[[MG_GrafTexture alloc] initWithFile:filename];
    [splash_texture prepare];
    [self prepareScreen];
    [self prepareFrame];
    [splash_texture drawAtX:0 Y:0];
//    [self renderTest];
    [self renderFrame];
}




#if TARGET_OS_IPHONE
- (BOOL)canBecomeFirstResponder {
//    printf("soy el first responder??\n");
    return YES;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [controlDelegate touchesBegan:touches withEvent:event];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    [controlDelegate touchesMoved:touches withEvent:event];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    [controlDelegate touchesEnded:touches withEvent:event];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    [controlDelegate touchesEnded:touches withEvent:event];
}

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [controlDelegate motionBegan:motion withEvent:event];
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [controlDelegate motionCancelled:motion withEvent:event];
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    [controlDelegate motionEnded:motion withEvent:event];
}

#else
/*- (BOOL)acceptsFirstResponder {
    printf("soy el first responder??\n");
    return YES;
}*/
    
- (void)mouseDown:(NSEvent *)event {
    [controlDelegate mouseDown:event];
}

- (void)mouseDragged:(NSEvent *)event {
    [controlDelegate mouseDragged:event];
}

- (void)mouseUp:(NSEvent *)event {
    [controlDelegate mouseUp:event];
}

- (void)keyDown:(NSEvent *)event {
    [controlDelegate keyDown:event];
}

- (void)keyUp:(NSEvent *)event {
    [controlDelegate keyUp:event];    
}

- (BOOL)acceptsFirstResponder {
    return YES;
}


#endif

@end

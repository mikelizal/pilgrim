// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "MG_Control.h"
#import "MG_Screen.h"

struct control_data {
    int control_id;
    int control_type;  // 0 boton normal 1 shake
    CGRect control_rect;
    BOOL control_pressed,control_last_pressed;
#if !TARGET_OS_IPHONE
    CGKeyCode keyCode;
#endif
};


@implementation MG_Control

-(id)initWithScreen: (MG_Screen*)screen andNumberOfButtons: (int)n {
    // general vars
    theScreen=screen;
    scr_width=screen->width;
    scr_height=screen->height;
    scr_orientation=screen->orientation;
#if TARGET_OS_IPHONE
    scr_scalefactor=screen.contentScaleFactor;
#else
    scr_scalefactor=1.0;
#endif
    
    int i;    
    // buttons vars
    buttons_n=n;
    buttons_data=malloc(n*sizeof(struct control_data));
    for (i=0; i<n; i++) {
        buttons_data[i].control_id=-1;
        // -1 is empty
    }
    
    // touch tracking state
    for (i=0;i<5;i++) {
        finger_touch[i]=0;
    }
    
    self->display=NO;
    
    [screen setControlDelegate:self];
    return self;
}

-(void)setDisplay:(bool)disp {
    self->display=disp;
}


-(void)unregisterButtonWithId:(int)theid {
    buttons_data[theid].control_id=-1;
}

-(void)registerButtonWithId:(int)theid rect:(CGRect)r {
    if (theid>=buttons_n) {
        printf("Error id too big registering button. Ignored\n");
        return;
    }
    buttons_data[theid].control_id=theid;
    buttons_data[theid].control_type=0;
    buttons_data[theid].control_rect=r;
    buttons_data[theid].control_pressed=NO;
    buttons_data[theid].control_last_pressed=NO;
#if !TARGET_OS_IPHONE
    buttons_data[theid].keyCode=0xFFFF;
#endif    
}

-(void)registerShakeWithId:(int)theid {
    if (theid>=buttons_n) {
        printf("Error id too big registering button. Ignored\n");
        return;
    }
    buttons_data[theid].control_id=theid;
    buttons_data[theid].control_type=1;
    buttons_data[theid].control_pressed=NO;
    buttons_data[theid].control_last_pressed=NO;    
#if !TARGET_OS_IPHONE
    buttons_data[theid].keyCode=0xFFFF;
#endif        
}


#if !TARGET_OS_IPHONE
-(void)controlWithID:(int)theid assignKeyCode:(CGKeyCode)keycode {
    buttons_data[theid].keyCode=keycode;
}
#endif


-(int)stateForButtonWithId:(int)theid {
    struct control_data *c=&(buttons_data[theid]);
    int estado=0; 
    if (theid!=c->control_id) {
        return -1;
        // undefined control
    }
    if (c->control_pressed) {
        estado=1;
        if (!c->control_last_pressed) { // es la primera vez que se ve pulsado
        //    c->control_last_pressed=YES;
            estado=2;
        }
//        if (c->control_type==1 && estado==2) { // el de tipo shake se resetea cuando se devuelve
//            c->control_pressed=0;
//            estado=1;
//        }
    }
    c->control_last_pressed=c->control_pressed;
    // -1 control no definido
    // 0 no pulsado
    // 1 pulsado
    // 2 pulsado y se acaba de pulsar
    return estado;
}







-(int)numActiveTouches {
    int i,c=0;
    
    for (i=0;i<5;i++) {
        if (finger_touch[i]) {
            c+=1;
        }
    }
    return c;
}


- (void)updateButtonsState {
    int i,j;
    CGPoint loc;
    for (j=0;j<buttons_n;j++) {
        buttons_data[j].control_pressed=NO;
    }
    for (i=0;i<5;i++) {
        if (finger_touch[i]!=0) {
            loc=finger_loc[i];
            for (j=0;j<buttons_n;j++) {
                if (buttons_data[j].control_id!=-1 && buttons_data[j].control_type==0) {
                    if (CGRectContainsPoint(buttons_data[j].control_rect, loc)) {
                        buttons_data[j].control_pressed=YES;
                    }
                }
            }
        }
    }
}

#if !TARGET_OS_IPHONE
- (void)updateButtonsStateForKey:(CGKeyCode)kc to:(BOOL)down {
    int j;
    for (j=0;j<buttons_n;j++) {
        if (buttons_data[j].keyCode==kc) {
          //  if (buttons_data[j].control_type==0) {
                buttons_data[j].control_pressed=down;
          //  } else {
          //      buttons_data[j].control_pressed=1;
          //  }
        }
        //printf("keycode: %u\n",kc);
    }    
}
#endif


-(void)activate {
    [theScreen setControlDelegate:self];
}

#if TARGET_OS_IPHONE
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
//    printf("touches began!! [%d]\n",[touches count]);
    NSEnumerator *i=[touches objectEnumerator];
    UITouch *touch;
    int j;
    while(touch=[i nextObject]) {
        for (j=0;j<5;j++) {
            if (finger_touch[j]==NULL) {
                finger_touch[j]=touch;
                finger_loc[j]=[touch locationInView:nil];
                [self adjustPointCoordsToOrientation:&(finger_loc[j])];
                break;
            }
        }
//        printf("  obj: %x ts: %f\n",touch,[touch timestamp]);
    }
    [self updateButtonsState];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
//    printf("touches moved!! [%d]\n",[touches count]);
    NSEnumerator *i=[touches objectEnumerator];
    UITouch *touch;
    int j;
    while(touch=[i nextObject]) {
        for (j=0;j<5;j++) {
            if (finger_touch[j]==touch) {
                finger_loc[j]=[touch locationInView:nil];
                [self adjustPointCoordsToOrientation:&(finger_loc[j])];
            }
        }        
//        printf("  obj: %x ts: %f\n",touch,[touch timestamp]);
    }
    [self updateButtonsState];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
//    printf("touches end!! [%d]\n",[touches count]);
    NSEnumerator *i=[touches objectEnumerator];
    UITouch *touch;
    int j;
    while(touch=[i nextObject]) {
        for (j=0;j<5;j++) {
            if (finger_touch[j]==touch) {                
                finger_touch[j]=0;
            }
        }        
//        printf("  obj: %x ts: %f\n",touch,[touch timestamp]);
    }
    [self updateButtonsState];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
//    printf("touches cancelled!! [%d]\n",[touches count]);
    NSEnumerator *i=[touches objectEnumerator];
    UITouch *touch;
    int j;
    while(touch=[i nextObject]) {
        for (j=0;j<5;j++) {
            if (finger_touch[j]==touch) {
                finger_touch[j]=0;
            }
        }        
//        printf("  obj: %x ts: %f\n",touch,[touch timestamp]);
    }
    [self updateButtonsState];    
}

- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    int j;
    for (j=0;j<buttons_n;j++) {
        if (buttons_data[j].control_type==1) {
            buttons_data[j].control_pressed=1;
        }
    }
}

- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    int j;
    for (j=0;j<buttons_n;j++) {
        if (buttons_data[j].control_type==1) {
            buttons_data[j].control_pressed=0;
        }
    }
}

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    int j;
    for (j=0;j<buttons_n;j++) {
        if (buttons_data[j].control_type==1) {
            buttons_data[j].control_pressed=0;
        }
    }
}


#else

- (void)mouseDown:(NSEvent *)event {
    finger_touch[0]=(void *)1;
    NSPoint loc=[event locationInWindow];
    finger_loc[0]=*(CGPoint*)&loc;
    [self updateButtonsState];
}

- (void)mouseDragged:(NSEvent *)event {
    finger_touch[0]=(void *)1;
    NSPoint loc=[event locationInWindow];
    finger_loc[0]=*(CGPoint*)&loc;
    [self updateButtonsState];
}

- (void)mouseUp:(NSEvent *)event {
    finger_touch[0]=0;
    [self updateButtonsState];
}

- (void)keyDown:(NSEvent *)event {
    //printf("keydown keycode: %x [%c]\n",event.keyCode);
    [self updateButtonsStateForKey:event.keyCode to:YES];
}

- (void)keyUp:(NSEvent *)event {
    //printf("keyup keycode: %x [%c]\n",event.keyCode);
    [self updateButtonsStateForKey:event.keyCode to:NO];
}
        
#endif

- (void)screenOrientationChangedTo:(int)orient {
    scr_orientation=orient;
}

- (void)adjustPointCoordsToOrientation:(CGPoint *)p {
    CGFloat aux;
    //printf("(%f,%f) -> ",p->x,p->y);
    switch (scr_orientation) {
        case 1:
            p->x=p->x*scr_scalefactor;
            p->y=scr_height-p->y*scr_scalefactor;
            break;
        case 2:
            aux=p->x;
            p->x=scr_width-p->y*scr_scalefactor;
            p->y=scr_height-aux*scr_scalefactor;
            break;
        case 3:
            aux=p->x;
            p->x=p->y*scr_scalefactor;
            p->y=aux*scr_scalefactor;
            break;
        case 4:
            p->x=scr_width-p->x*scr_scalefactor;
            p->y=p->y*scr_scalefactor;
            break;            
    }
    //printf("(%f,%f)\n",p->x,p->y);
}

-(void)render {
    if (!display) return; 
    GLfloat lineVtx[]={100.0,0.0,100.0,400.0,  0.0,100.0,1.0,100.0,    1.0,0.0,1.0,1.0,  0.0,1.0,1.0,1.0,    1.0,0.0,1.0,1.0,  0.0,1.0,1.0,1.0,    1.0,0.0,1.0,1.0,  0.0,1.0,1.0,1.0,    1.0,0.0,1.0,1.0,  0.0,1.0,1.0,1.0,};
    //GLubyte lineClr[]={255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  255,0,0,255,255,0,0,255,  };
    int pos=0,n=0;
    CGPoint loc;
    int i;
    for (i=0;i<5;i++) {
        if (finger_touch[i]) {
            loc=finger_loc[i];
            lineVtx[pos+0]=loc.x;
            lineVtx[pos+2]=loc.x;
            lineVtx[pos+3]=scr_height;

            lineVtx[pos+5]=loc.y;
            lineVtx[pos+6]=scr_width;
            lineVtx[pos+7]=loc.y;            
            n+=1;
            pos+=8;
        }
    }
    glDisable(GL_TEXTURE_2D);
    if (n>0) {
        //glLineWidth(5.0);
        //glEnable(GL_LINE_SMOOTH);
        glVertexPointer(2, GL_FLOAT, 0, lineVtx);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        //glColorPointer(4,GL_UNSIGNED_BYTE, 0, lineClr);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor4f(1,0,0,1);
//        glDrawArrays(GL_LINE_STRIP, 0, 3);
        glDrawArrays(GL_LINES, 0, 4*n);
    }
    for (i=0; i<buttons_n; i++) {
        if (buttons_data[i].control_id!=-1 && buttons_data[i].control_type==0) {
            const float squareVtx[]={1.0,0.0,1.0,1.0,0.0,0.0,0.0,1.0};
            CGRect r=buttons_data[i].control_rect;
            glVertexPointer(2, GL_FLOAT, 0, squareVtx);
            glEnableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            if (!buttons_data[i].control_pressed) {
                glColor4f(1,1,1,0.3);
            } else {
                glColor4f(1,1,1,0.8);                
            }
            glPushMatrix();
            glTranslatef(r.origin.x,r.origin.y,0);
            glScalef(r.size.width,r.size.height,0);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glPopMatrix();
        }
    }
}


@end



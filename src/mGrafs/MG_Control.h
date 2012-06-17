// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

//#import <Foundation/Foundation.h>
//#import <UIKit/UIKit.h>

@class MG_Screen;

struct control_data;

@interface MG_Control : NSObject {
    MG_Screen   *theScreen;
    int scr_width, scr_height, scr_orientation;
    float scr_scalefactor;
    
    int buttons_n;
    struct control_data *buttons_data;

    bool display;
    
    void *finger_touch[5];
    CGPoint finger_loc[5];
}

-(id)initWithScreen: (MG_Screen*)screen andNumberOfButtons: (int)n;
-(void)setDisplay:(bool)disp;
-(void)unregisterButtonWithId:(int)theid;
-(void)registerButtonWithId:(int)theid rect:(CGRect)r;
-(void)registerShakeWithId:(int)theid;
#if !TARGET_OS_IPHONE
-(void)controlWithID:(int)theid assignKeyCode:(CGKeyCode)keycode;
#endif
-(int)stateForButtonWithId:(int)theid;
-(int)numActiveTouches;

-(void)render;

-(void)activate;

- (void)screenOrientationChangedTo:(int)orient;

- (void)adjustPointCoordsToOrientation:(CGPoint *)p;

//- (void)updateButtonsState;

#if TARGET_OS_IPHONE
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event;    
- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event;
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event;
#else
- (void)mouseDown:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)keyDown:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
#endif



@end

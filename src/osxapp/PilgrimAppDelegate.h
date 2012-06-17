// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "mGrafs/MG_Screen.h"
#import "mGrafs/MG_Control.h"
#import "mGrafs/MG_Clock.h"
#include "pilgrim.h"

#if !TARGET_OS_IPHONE
#import <WebKit/WebKit.h>
#endif

#if TARGET_OS_IPHONE
#import <GameKit/GameKit.h>
#import "GameCenterInterface.h"
#endif


#if TARGET_OS_IPHONE
@interface PilgrimAppDelegate : NSObject <UIApplicationDelegate, MG_ClockListener,UITextFieldDelegate, UIWebViewDelegate,GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate> {
#else
@interface PilgrimAppDelegate : NSObject <NSApplicationDelegate, MG_ClockListener,NSTextFieldDelegate> {    
#endif
		
	MG_Screen *screen;
    MG_Control *control;
    MG_Clock *clock;

#if TARGET_OS_IPHONE
    UITextField *nameTF;
    
    BOOL inBackground;
    int orientation;
    
    UIViewController *screen_viewcontroller;

    UIApplication *the_application; 
    // para poder cambiar la orientacion al sacar el game center
    
    GameCenterInterface *gamecenter_manager;
    
#else
    NSTextField *auxLabelTF;
    NSTextField *nameTF;
#endif
    
//#if TARGET_OS_IPHONE
//    UIWebView *creditsView;
//#else
////    WebView *creditsView;    
//    NSTextView *creditsView;
//#endif

}


	
-(void)tickTime:(float)t dt:(float)dt i:(int)i;
-(void)layoutControlsFor:(int)state;

#if TARGET_OS_IPHONE
    - (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;
#endif
    
#if TARGET_OS_IPHONE
    - (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent ;
#endif

    
	
@end


// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "PilgrimAppDelegate.h"



@implementation PilgrimAppDelegate


#pragma mark -
#pragma mark Application lifecycle


#if TARGET_OS_IPHONE
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
#else
- (void)applicationDidFinishLaunching:(NSApplication *)application {    	
#endif
	

    NSString *version_number=[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    NSString *build_number=[[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
    sprintf(pilgrim_version_number,"%s",[version_number UTF8String]);
    sprintf(pilgrim_build_number,"%s",[build_number UTF8String]);
   // printf("Pilgrim version %s build %s \n",pilgrim_version_number,pilgrim_build_number);
    
    
#if TARGET_OS_IPHONE	
    [application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
    the_application=application;
    orientation=3;
    inBackground=NO;
#endif

    nameTF=nil;
    
    // Override point for customization after application launch.
	
//	printf("Appdelegate launched...\n");
    

	
	screen=[[MG_Screen alloc] initWithOrientation:3];
        
    if (screen->width==2048 || screen->height==2048) render_ipad_res=1;
    
    control=[[MG_Control alloc] initWithScreen:screen andNumberOfButtons:7];
    [control setDisplay:NO];

#if TARGET_OS_IPHONE
    screen_viewcontroller=[[UIViewController alloc] init];
    screen_viewcontroller.view=screen;
    
    game_gamecenter_available=isGameCenterAPIAvailable();
    gamecenter_manager=nil;
    if (game_gamecenter_available) {
        gamecenter_manager=[GameCenterInterface new];
        
    }
#endif
    
    [self layoutControlsFor:1];
    
    [control activate];

//    creditsView=nil;

#if TARGET_OS_IPHONE
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:@"UIDeviceOrientationDidChangeNotification" object:nil ];
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];    
#endif
    
    [screen prepareScreen];	
//    [screen splashScreenWithFile:@"Default.png"];

	clock=[[MG_Clock alloc] initWithFreq:40 callbackTarget:self];

    assets_setresourcepath([[[NSBundle mainBundle] resourcePath] UTF8String]);
    NSFileManager *fm = [NSFileManager defaultManager];
    NSArray *dirs=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDirectory,YES);
    NSString *dir=[dirs objectAtIndex:0];
#if !TARGET_OS_IPHONE
    dir=[dir stringByAppendingString:@"/Pilgrim"];
#endif
    BOOL isdir;
    if (![fm fileExistsAtPath:dir isDirectory:&isdir]) {
        [fm createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:NULL];
    }
    highscore_setfilename([dir fileSystemRepresentation],"highscores");
    
    if (screen->width<960) render_set_lowres();
    game_init();
    

    
	[clock start];
    
#if TARGET_OS_IPHONE	
	return YES;
#endif
}

#if TARGET_OS_IPHONE
- (void)orientationChanged:(NSNotification *)notification {
    UIDeviceOrientation new_or=[[UIDevice currentDevice] orientation];
    if (new_or==UIDeviceOrientationLandscapeLeft) {
        orientation=3;
        [screen changeOrientation:3];
        //[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
    } else if (new_or==UIDeviceOrientationLandscapeRight) {
        orientation=2;
        [screen changeOrientation:2];
        //[[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft animated:NO];
    }
}
#endif
    
#if TARGET_OS_IPHONE	
- (void)textFieldDidEndEditing:(UITextField *)textField {
    if (textField==nameTF) {
        game_highscore_name_entered=1;
        sprintf(game_highscore_name,"%.20s",[nameTF.text UTF8String]);
        [nameTF retain];
        [nameTF removeFromSuperview];
    }
}
    
- (BOOL)textFieldShouldReturn:(UITextField *)textField {
    [nameTF resignFirstResponder];
    return YES;
}
#else
- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor {
//    printf("should end editing??\n");
    return YES;
}

- (void)controlTextDidEndEditing:(NSNotification *)aNotification {
//    printf("did end editing\n");
    // suponemos que viene del textfield que no me apetece mirar como comprobarlo
    game_highscore_name_entered=1;
//    printf("nombre= %.20s\n",[[nameTF stringValue] UTF8String]);
    sprintf(game_highscore_name,"%.20s",[[nameTF stringValue] UTF8String]);
    [nameTF retain];
    [auxLabelTF retain];
    [nameTF removeFromSuperview];
    [auxLabelTF removeFromSuperview];
    [screen->w makeFirstResponder:screen];
    [screen->openglView setFrameOrigin:CGPointMake(0, 0)];
}
    
#endif
    
#if TARGET_OS_IPHONE
- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
//    printf("have to go to background...\n");
    
    game_pause();
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
//    printf("entered background..\n");
    inBackground=YES;
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    inBackground=NO;
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
//    printf("entered terminated\n");
}


    
#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}
#endif

#pragma mark -
#pragma mark Clock Listener
	
//    int itick=0;
    
-(void)tickTime:(float)t dt:(float)dt i:(int)i {
   // itick+=1;
   // if (itick%20==0)    printf("tick\n");
#if TARGET_OS_IPHONE
    if (inBackground) return;
#endif    

    if (game_init_game_controls) {
        game_init_game_controls=0;
        [self layoutControlsFor:0];
    }

    if (game_init_menu_controls) {
        game_init_menu_controls=0;
        [self layoutControlsFor:1];
    }

    if (game_init_clicktocontinue_controls) {
        game_init_clicktocontinue_controls=0;
        [self layoutControlsFor:2];
    }

    if (game_init_backandfw_help_controls) {
        game_init_backandfw_help_controls=0;
        [self layoutControlsFor:4];
    }

    if (game_init_backandlink_credits_controls) {
        game_init_backandlink_credits_controls=0;
        [self layoutControlsFor:5];
    }
    
    if (game_init_backandgamecenterscores_controls) {
        game_init_backandgamecenterscores_controls=0;
        [self layoutControlsFor:6];
    }
    
    if (game_init_pause_controls) {
        game_init_pause_controls=0;
        [self layoutControlsFor:3];
    }
    
#if PILGRIM_INCLUDE_WEBLINK
    if (game_jump_to_weblink) {
        game_jump_to_weblink=0;
        
        NSURL *bwareurl=[NSURL URLWithString:[NSString stringWithFormat:@"%s",PILGRIM_WEBLINK]];

#if TARGET_OS_IPHONE
        if (!achievments_www_visited) {
            [gamecenter_manager reportAchievment:@"visitweb" value:100.0];
            achievments_www_visited=true;
        } 
        [[UIApplication sharedApplication] openURL:bwareurl];
#else
        [[NSWorkspace sharedWorkspace] openURL:bwareurl];
#endif

    }
#endif
    
    if (game_show_name_dialog) {
#if TARGET_OS_IPHONE	
        game_show_name_dialog=0;
        if (nameTF==nil) {
//            printf("----------------\ncreando rect para el view\n");
            CGRect brect = CGRectMake(160-90 -30 , 240-25, 160+180, 50);
//            printf("construyendo UITextField\n");
            nameTF=[[UITextField alloc] initWithFrame:brect];
//            printf("setting text\n");
            nameTF.placeholder=@"write your name, pilgrim";
            //nameTF.bounds=brect;
//            printf("setting border\n");
            nameTF.borderStyle=UITextBorderStyleRoundedRect;
//            printf("setting color\n");
            nameTF.backgroundColor=[UIColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:1.0];
//            printf("setting font\n");
            nameTF.font=[UIFont fontWithName:@"MarkerFelt-Wide" size:26];
//            printf("setting transform\n");
//            printf("setting clear\n");
            nameTF.clearButtonMode=UITextFieldViewModeWhileEditing;
//            printf("setting delegate\n");
            nameTF.delegate=self;
//            printf("retain\n");
            [nameTF retain];
        }
        CGAffineTransform tr1;
        CGPoint centro=[nameTF center];
//        printf("centro x:%f y:%f\n",centro.x,centro.y);
        if (orientation==2) {
            centro.x=125;
            centro.y=240;
            tr1 = CGAffineTransformMakeRotation(-3.141592/2.0);
        } else {
            centro.x=195;
            centro.y=240;
            tr1 = CGAffineTransformMakeRotation(3.141592/2.0);                
        }
        nameTF.center=centro;
        nameTF.transform=tr1;
//        printf("add subview\n");
        [screen addSubview:nameTF];
//        printf("firstresponder\n");
        [nameTF becomeFirstResponder];
#else
        game_show_name_dialog=0;
        if (nameTF==nil) {
//            NSWindow *aw = [[NSWindow alloc] initWithContentRect:CGRectMake(100, 100, 200, 200) styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO];
            NSFont *tfont=[NSFont fontWithName:@"MarkerFelt-Wide" size:30];
            CGRect brect = CGRectMake(400, 0, 560, 50);
            nameTF=[[NSTextField alloc] initWithFrame:brect];
//            [nameTF setEditable: YES];
            //[nameTF setEnabled: YES];
            [nameTF setFont:tfont];
            [nameTF setBezeled:YES];
            [nameTF setDrawsBackground:YES];
            NSColor *bcolor=[NSColor colorWithDeviceRed:1.0 green:0.81 blue:0.51 alpha:1.0];
//            [auxLabelTF setBackgroundColor:bcolor];
            [nameTF setSelectable:YES];
            [nameTF setStringValue:@"Phil Green"];
            [nameTF setDelegate:self];

            auxLabelTF=[[NSTextField alloc] initWithFrame:CGRectMake(0,0,400,50)];
            [auxLabelTF setBezeled:NO];
            [auxLabelTF setEditable:NO];
            [auxLabelTF setFont:tfont];
            [auxLabelTF setBackgroundColor:bcolor];
            [auxLabelTF setDrawsBackground:YES];
            [auxLabelTF setStringValue:@"Write your name, pilgrim:"];
            
        }
        [screen->openglView setFrameOrigin:CGPointMake(0, 50)];
        [screen addSubview:auxLabelTF];
        [screen addSubview:nameTF];
        [screen->w makeFirstResponder:nameTF];
        //[screen setNeedsDisplay:YES];
#endif
    }
    
#if TARGET_OS_IPHONE	
    if (game_enter_gamecenter_hof) {
        game_enter_gamecenter_hof=0;
        [GKLeaderboard loadCategoriesWithCompletionHandler:^(NSArray *categories, NSArray *titles, NSError *error) {
            if (error != nil)
            {
                // handle the error
            }
            // use the category and title information
//            NSLog("categories: %@\n",[categories objectAtIndex:0]);
//            NSLog("titles: %@\n",[titles objectAtIndex:0]);
        }];
        
        [the_application setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
        printf("show gamecenter hof\n");
        GKLeaderboardViewController *lbVC = [[GKLeaderboardViewController alloc] init];
        if (lbVC != nil)
        {
            lbVC.leaderboardDelegate = self;
            [screen_viewcontroller presentModalViewController: lbVC animated: YES];
        }
    }

    
    if (game_enter_gamecenter_achievments) {
        game_enter_gamecenter_achievments=0;
        
        [the_application setStatusBarOrientation:UIInterfaceOrientationPortrait animated:NO];
        GKAchievementViewController *lbVC = [[GKAchievementViewController alloc] init];
        if (lbVC != nil)
        {
            lbVC.achievementDelegate = self;
            [screen_viewcontroller presentModalViewController: lbVC animated: YES];
        }
    }
    
    
    
    if (game_report_score_to_gamecenter) {
        game_report_score_to_gamecenter=0;
        if (!game_paused) {
            // game_paused significa que hemos salido con un quit
            // en ese caso no es un highscore pero se acumula la distancia total
            [gamecenter_manager reportScore:@"com.bwarestudios.pilgrim_top" value:game_last_score];
        }
        [gamecenter_manager reportScore:@"com.bwarestudios.pilgrim_total" value:game_last_score];
    }
    
    if (achievments_report_play1time) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_play1time] value:100.0];
            achievments_report_play1time=false;
        }
    }
    if (achievments_report_reach250) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach250] value:100.0];
            achievments_report_reach250=false;
        }
    }
    if (achievments_report_reach500) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach500] value:100.0];
            achievments_report_reach500=false;
        }
    }
    if (achievments_report_reach1000) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach1000] value:100.0];
            achievments_report_reach1000=false;
        }
    }
    if (achievments_report_before100) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_before100] value:100.0];
            achievments_report_before100=false;
        }
    }
    if (achievments_report_downbybike) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_downbybike] value:100.0];
            achievments_report_downbybike=false;
        }
    }
    if (achievments_report_walk250panic) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_walk250panic] value:100.0];
            achievments_report_walk250panic=false;
        }
    }
    if (achievments_report_walk250nonviolent) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_walk250nonviolent] value:100.0];
            achievments_report_walk250nonviolent=false;
        }
    }
    if (achievments_report_walk500nonviolent) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_walk500nonviolent] value:100.0];
            achievments_report_walk500nonviolent=false;
        }
    }
    if (achievments_report_reach250unfallen) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach250unfallen] value:100.0];
            achievments_report_reach250unfallen=false;
        }
    }
    if (achievments_report_reach500unfallen) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach500unfallen] value:100.0];
            achievments_report_reach500unfallen=false;
        }
    }
    if (achievments_report_reach250fruitonly) {
        if (game_gamecenter_available) {
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_reach250fruitonly] value:100.0];
            achievments_report_reach250fruitonly=false;
        }
    }

    if (achievments_startplay>0) {
        if (game_gamecenter_available) {
            int report_startplay=achievments_startplay;
            achievments_startplay=0;
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_startplay] value:report_startplay*100.0/5.0];
            [gamecenter_manager reportAchievment:[NSString stringWithUTF8String:achievments_name_play100] value:report_startplay*100.0/100.0];
        }
    }



#endif    
    
    
//    if (game_show_credits) {
//        game_show_credits=0;
//        if (creditsView==nil) {
//#if TARGET_OS_IPHONE
//            printf("credits...\n");
//            CGAffineTransform tr1 = CGAffineTransformMakeRotation(3.141592/2.0);
//            CGRect vrect = CGRectMake(-65, 90, 450, 300);
//            creditsView=[[UIWebView alloc] initWithFrame:vrect];
//        //creditsView.text=@"Pilgrim by Bware studios\n\n";
//        //creditsView.editable=NO;
////        creditsView.font=[UIFont systemFontOfSize:25];
//        //creditsView.backgroundColor=[UIColor clearColor];
//            creditsView.transform=tr1;
//            
//            creditsView.delegate=self;
//#else
////            creditsView=[[WebView alloc] initWithFrame:[screen bounds] frameName:nil groupName:nil];
//            creditsView=[[NSTextView alloc] initWithFrame:[screen bounds]];
//#endif
//        }
//        NSString *creditsStr=@"<html><body><img src=\"x.png\"><h1>Pilgrim</h1><p>by Bware studios<p><a href=\"http://www.google.com\">http://www.google.com</a></body></html>" ;
//        NSURL *creditsBaseURL=[NSURL URLWithString:@""];
//#if TARGET_OS_IPHONE
//        [creditsView loadHTMLString:creditsStr baseURL:creditsBaseURL];
//#else
////        [[creditsView mainFrame] loadHTMLString:creditsStr baseURL:[NSURL URLWithString:@""] ];
////        [[[creditsView mainFrame] loadHTMLString:creditsStr baseURL:[NSURL URLWithString:@""] ] ];    
//        NSAttributedString *creditsContent = [[NSAttributedString alloc] initWithHTML: [creditsStr dataUsingEncoding:NSUTF8StringEncoding] baseURL:creditsBaseURL documentAttributes:NULL];
//        [creditsView insertText:creditsContent];
//        [creditsView setDrawsBackground:YES];
//        [creditsView setBackgroundColor:[NSColor colorWithDeviceRed:1.0 green:1.0 blue:0.0 alpha:1.0]];
//        [creditsView setEditable:NO];
//        [screen->openglView setFrameOrigin:CGPointMake(0,[screen bounds].size.height)];
//
//
//#endif
//        [screen addSubview:creditsView];
//        
//    }
    
    if ( [control stateForButtonWithId:0] == 1 ) {
        game_salto_pressed=1;
    } else {
        game_salto_pressed=0;		
    }

    if ( [control stateForButtonWithId:1] == 1 ) {
        game_hit_pressed=1;
    } else {
        game_hit_pressed=0;		
    }
    
    if ( [control stateForButtonWithId:2] == 1 ) {
        game_esquivar_pressed=1;
    } else {
        game_esquivar_pressed=0;		
    }
    
    if ( [control stateForButtonWithId:3] == 1 ) {
        game_stop_pressed=1;
    } else {
        game_stop_pressed=0;		
    }

    if ( [control stateForButtonWithId:4] == 2 ) {
//        printf("ESCAPE !!!! <<<<<<<<<\n");
    }
    
    if ( [control stateForButtonWithId:5] == 2 ) {
        game_other_pressed=1;
    } else {
        game_other_pressed=0;		
    }
    if ( [control stateForButtonWithId:6] == 2 ) {
        game_other2_pressed=1;
    } else {
        game_other2_pressed=0;		
    }
    
    [screen prepareFrame];
	game_update();
	[screen renderFrame];    
}
	
#pragma mark -	
- (void)dealloc {
    [super dealloc];
}


- (void)layoutControlsFor:(int)state {
    int full=1; // full resolution

    switch (state) {
        case 0: // game
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];
            [control unregisterButtonWithId:6];
            if (render_ipad_res) {
                [control registerButtonWithId:0 rect:CGRectMake(1024, 768, 1024, 768)];
                [control registerButtonWithId:1 rect:CGRectMake(1024, 0, 1024, 768)];
                [control registerButtonWithId:2 rect:CGRectMake(0, 0, 1024, 768)];
                [control registerButtonWithId:3 rect:CGRectMake(0, 768, 480, 768)];
            } else {
                if (full) { // controles grandes
                    //            [control registerButtonWithId:0 rect:CGRectMake(480, 320, 480, 320)];
                    //            [control registerButtonWithId:1 rect:CGRectMake(480, 0, 480, 320)];
                    //            [control registerButtonWithId:2 rect:CGRectMake(0, 0, 480, 320)];
                    //            [control registerButtonWithId:3 rect:CGRectMake(0, 320, 480, 320)];
                    [control registerButtonWithId:3 rect:CGRectMake(660, 0, 150, 150)];
                    [control registerButtonWithId:1 rect:CGRectMake(810, 0, 150, 150)];
                    [control registerButtonWithId:0 rect:CGRectMake(0, 0, 150, 150)];
                    [control registerButtonWithId:2 rect:CGRectMake(150, 0, 150, 150)];
                    
                    // pause button
                    [control registerButtonWithId:5 rect:CGRectMake(850,530 , 110, 110)];
                    
                } else {
                    [control registerButtonWithId:0 rect:CGRectMake(240, 160, 240, 160)];
                    [control registerButtonWithId:1 rect:CGRectMake(240, 0, 240, 160)];
                    [control registerButtonWithId:2 rect:CGRectMake(0, 0, 240, 160)];
                    [control registerButtonWithId:3 rect:CGRectMake(0, 160, 240, 160)];
                }
            }
            [control registerShakeWithId:4];
            
           
            
            break;
        case 1: // main menu
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];
            [control unregisterButtonWithId:6];

            [control registerButtonWithId:0 rect:CGRectMake(590, 35, 200, 90)];
            [control registerButtonWithId:2 rect:CGRectMake(380, 150, 200, 90)];
            [control registerButtonWithId:3 rect:CGRectMake(260, 270, 200, 90)];
            [control registerButtonWithId:1 rect:CGRectMake(60, 350, 200, 90)];
            [control registerButtonWithId:5 rect:CGRectMake(20, 20, 120, 120)];
            
            
            break;
        case 2: // back from hof and others
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];  
            [control unregisterButtonWithId:6];

            [control registerButtonWithId:5 rect:CGRectMake(871-20, 0, 109, 110)];
            
            break;
        case 3: // game pause mode
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];  
            [control unregisterButtonWithId:6];
            
            // en pausa other es quitar y salta salir de la pausa
            [control registerButtonWithId:6 rect:CGRectMake(352, 169+160, 249, 127)];            
            [control registerButtonWithId:5 rect:CGRectMake(352, 169+20, 249, 127)];            
            break;
        case 4: // back and forward in help from credits 
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];  
            [control unregisterButtonWithId:6];
            
            [control registerButtonWithId:5 rect:CGRectMake(871-20, 100, 109, 110)];
            [control registerButtonWithId:6 rect:CGRectMake(0, 100, 109, 110)];            
            break;
        case 5: // back + weblink from credits  
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];  
            [control unregisterButtonWithId:6];
            
            [control registerButtonWithId:5 rect:CGRectMake(871-20, 0, 109, 110)];
            [control registerButtonWithId:6 rect:CGRectMake(560, 380, 400, 260)];
            
            break;
        case 6: // back and gamecenter show from hof
            [control unregisterButtonWithId:0];
            [control unregisterButtonWithId:1];
            [control unregisterButtonWithId:2];
            [control unregisterButtonWithId:3];
            [control unregisterButtonWithId:5];  
            [control unregisterButtonWithId:6];
            
            [control registerButtonWithId:5 rect:CGRectMake(871-20, 0, 109, 110)];
            if (game_gamecenter_available) {
                [control registerButtonWithId:6 rect:CGRectMake(871-20-25, 540-15, 109, 110)];
                [control registerButtonWithId:3 rect:CGRectMake(789-30-55, 540-15, 109, 110)];
            }
            break;

    }
     
#if !TARGET_OS_IPHONE
    [control controlWithID:0 assignKeyCode:0x7e];
    [control controlWithID:1 assignKeyCode:0x7c];
    [control controlWithID:2 assignKeyCode:0x7b];
    [control controlWithID:3 assignKeyCode:0x7d];
    [control controlWithID:4 assignKeyCode:53];
    [control controlWithID:5 assignKeyCode:0x24];
#endif
    

    
}
    
    
#if TARGET_OS_IPHONE
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController
{
    [screen_viewcontroller dismissModalViewControllerAnimated:YES];
    [the_application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
}    
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController {
    [screen_viewcontroller dismissModalViewControllerAnimated:YES];
    [the_application setStatusBarOrientation:UIInterfaceOrientationLandscapeRight animated:NO];
}

#endif


#if TARGET_OS_IPHONE
- (void) reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent {
    GKAchievement *achievement = [[[GKAchievement alloc] initWithIdentifier: identifier] autorelease];
    if (achievement) {
        achievement.percentComplete = percent;
        achievement.showsCompletionBanner = YES;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error) {
            if (error != nil) {
                printf("Cannot report achievment\n");
            }
        }];
    }
}
#endif
    
//#if TARGET_OS_IPHONE
//- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
////    printf("deberia cargar?\n");
//    // aqui redirigir la carga al navegador
//    return NO;
//}
//#endif


@end

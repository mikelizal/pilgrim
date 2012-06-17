// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "GameCenterInterface.h"

#define GCINTERFACE_DEBUG 0

@implementation GameCenterInterface


- (GameCenterInterface *) init {
#if GCINTERFACE_DEBUG == 1
    printf("GCI Gamencenter init\n");
#endif    
    self=[super init];
    if (self==nil) return nil;

    achievments_loaded=false;
    
    gamecenterCachePath = [NSString stringWithFormat:@"%@/gamecenter_cache",
    [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDirectory,YES) objectAtIndex:0]];
    
    // si hay fichero guardado con cosas pendientes deberia restaurarse aqui a los dictionaries
    // si se va a quitar la aplicacion hay que guardar lo que haya pendiente
    
    stats=[[NSMutableDictionary alloc] initWithCapacity:2]; 
    stats_reported=[[NSMutableDictionary alloc] initWithCapacity:2]; 
    stats_waiting=[[NSMutableDictionary alloc] initWithCapacity:2]; 
    //NSLog(@"cache_path= %@",gamecenterCachePath);
    
    localplayer=[GKLocalPlayer localPlayer];

    [self startAuthenticate];
    
    return self;
}


- (void)startAuthenticate {
#if GCINTERFACE_DEBUG == 1
    printf("GCI start authenticate \n");
#endif    
    [localplayer authenticateWithCompletionHandler:^(NSError *error) {
        if (localplayer.isAuthenticated)
        {
            [self didAuthenticate];            
//            
//            
//            // Perform additional tasks for the authenticated player.
//            [GKLeaderboard loadCategoriesWithCompletionHandler:^(NSArray *categories, NSArray *titles, NSError *error) {
//                NSEnumerator *ii=[categories objectEnumerator];
//                id ob;
//                while (ob=[ii nextObject]) {
//                    NSLog(@">:: %@ %@\n",ob,[ob class]);
//                }
//            }
//             ];
        }
        // falta reaccionar a que pasa si no se autentifica a la primera
    }];
}

- (void)requestScore:(NSString *)scoreid {
#if GCINTERFACE_DEBUG == 1
    printf("GCI request score \n");
#endif    
    GKLeaderboard *request = [[GKLeaderboard alloc] initWithPlayerIDs:[NSArray arrayWithObject:localplayer.playerID]];
    request.category=scoreid;
    request.timeScope=GKLeaderboardTimeScopeAllTime; // aunque da igual porque este es el default
    [request loadScoresWithCompletionHandler:^(NSArray *scores, NSError *error)
        {
            if (error!=nil) {
#if GCINTERFACE_DEBUG == 1
                NSLog(@"GCI error geting %@ %@\n",scoreid,error);
#endif    
                // avisar de que ha fallado??
                return;
            }
            if (scores!=nil) {
                // recibido
                [self didReceiveScore:scoreid value:(GKScore*)[scores objectAtIndex:0]];
            } else {
                // confirmado que no tenia ese score
                [self didReceiveScore:scoreid value:nil];
            }
        }
    ];
}


- (void)requestAchievments {
#if GCINTERFACE_DEBUG == 1
    printf("GCI request achievments \n");
#endif    
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error)
        {
            if (error!=nil) {
#if GCINTERFACE_DEBUG == 1
                NSLog(@"error geting achievments %@\n",error);
#endif    
                // avisar de que ha fallado??
                return;
            }
            [self didReceiveAchievments:achievements];
        }
    ];
}





//- (void)sendTotalDistance {
//    GKScore *total_distance_in_cloud=[player_stats objectForKey:@"pilgrim_total"];    
//    GKScore *total_distance=[player_stats_to_report objectForKey:@"pilgrim_total"];
//    
//    GKScore *total_distance_to_send=[[GKScore alloc]initWithCategory:@"com.bwarestudios.pilgrim_total"];
//    total_distance_to_send.value=total_distance_in_cloud.value+total_distance.value;
//    
//    [total_distance_to_send reportScoreWithCompletionHandler:^(NSError *error) {
//        if (error != nil)
//        {
//            printf("error reporting score\n");
//        } else {
//            
//        }
//    }];
//}

- (void)sendScore:(NSString *)scoreid value:(GKScore *)score currentValue:(GKScore *)currentscore {
#if GCINTERFACE_DEBUG == 1
    printf("GCI send score \n");
#endif    
    
    if ((id)currentscore==[NSNull null]) {
        currentscore=[[GKScore alloc] initWithCategory:scoreid];
        currentscore.value=0;
    }
    
    if ([scoreid isEqualToString:@"com.bwarestudios.pilgrim_total"]) {
        currentscore.value+=score.value;
    } else {
        if (score.value>currentscore.value) {
            currentscore.value=score.value;
        }
    }
    printf("sending score [%s] :: %lld \n",[scoreid UTF8String],currentscore.value);
    [player_stats_reported setObject:currentscore forKey:scoreid];
    
    [currentscore reportScoreWithCompletionHandler:^(NSError *error)
        {
            if (error!=nil) {
                [self scoreReported:scoreid withResult:NO];
            } else {
                [self scoreReported:scoreid withResult:YES];
            }
        }
    ];
}

- (void)sendAchievment:(NSString *)achievid value:(GKAchievement *)achievment {
#if GCINTERFACE_DEBUG == 1
    printf("GCI send achievment \n");
#endif    
    
    achievment.showsCompletionBanner=YES; 
    if (achievment.percentComplete<100.0) {
        achievment.showsCompletionBanner=NO;         
    }
    [player_stats_reported setObject:achievment forKey:achievid];
    
    [achievment reportAchievementWithCompletionHandler:^(NSError *error) 
        {
            if (error!=nil) {
                [self achievmentReported:achievid withResult:NO];
            } else {
                [self achievmentReported:achievid withResult:YES];
            }
        }
    ];
}



- (void)didAuthenticate {
    player_stats=[stats objectForKey:localplayer.playerID];
    player_stats_reported=[stats_reported objectForKey:localplayer.playerID];
    player_stats_waiting=[stats_reported objectForKey:localplayer.playerID];
#if GCINTERFACE_DEBUG == 1
    printf("GCI did autenticate in game center\n");
    [self dumpState];
#endif    
    
    if (player_stats==nil) {
        // si no estaba se crea
        player_stats=[[[NSMutableDictionary alloc] initWithCapacity:3] autorelease];
        [stats setObject:player_stats forKey:localplayer.playerID];
    }

    if (player_stats_reported==nil) {
        // si no estaba se crea
        player_stats_reported=[[[NSMutableDictionary alloc] initWithCapacity:3] autorelease];
        [stats_reported setObject:player_stats_reported forKey:localplayer.playerID];
    }

    if (player_stats_waiting==nil) {
        // si no estaba se crea
        player_stats_waiting=[[[NSMutableDictionary alloc] initWithCapacity:3] autorelease];
        [stats_waiting setObject:player_stats_waiting forKey:localplayer.playerID];
    }

    // falta poner que hago si ya habia cosas pendientes en el fichero al autentificarte hay que volverlas a enviar
    
    
    // pide la distancia total y los achievments para ese player
//    GKLeaderboard *my_total_request = [[GKLeaderboard alloc] initWithPlayerIDs:[NSArray arrayWithObject:localplayer.playerID]];
//    [my_total_request loadScoresWithCompletionHandler:^(NSArray *scores, NSError *error) {
//        if (error!=nil) {
//            NSLog(@"error geting total_distance %@\n",error);
//            return;
//        }
//        GKScore *my_total;
//        if (scores!=nil) {
//            my_total=(GKScore*)[scores objectAtIndex:0];
//            printf("recibido total_distance=%lld\n",my_total.value);
//        } else {
//            my_total = [[[GKScore alloc] initWithCategory:@"com.bwarestudios.pilgrim_total"] autorelease];
//            my_total.value=0;
//        }
//        [player_stats setObject:my_total forKey:@"pilgrim_total"];
//        [self didReceiveTotalDistance];
//    }];

    [self requestScore:@"com.bwarestudios.pilgrim_top"];
    [self requestScore:@"com.bwarestudios.pilgrim_total"];
    [self requestAchievments];
}

- (void)didReceiveScore:(NSString *)scoreid value:(GKScore *)score {
#if GCINTERFACE_DEBUG == 1
    printf("Recibido score %s :: %lld\n",[scoreid UTF8String],[score value]);
#endif
    GKScore *current=[player_stats objectForKey:scoreid];
    if (current==nil) {
        // este deberia ser el caso comun
        id thenewscore = score;
        if (score==nil) thenewscore=[NSNull null];
        [player_stats setObject:thenewscore forKey:scoreid];
    } else {
        // esto significa que he pedido los scores a pesar de que ya los tenia
        
    }
    
    // si tenia algun score pendiente de saber el actual para enviarse ahora es el momento
    
}


- (void)didReceiveAchievments:(NSArray *)achievarray {
#if GCINTERFACE_DEBUG == 1
    printf("Recibidos achievments\n");
#endif
    NSEnumerator *e = [achievarray objectEnumerator];
    GKAchievement *iachievment;
    while (iachievment = (GKAchievement *)[e nextObject]) {
        NSString *achievid = iachievment.identifier;
        GKAchievement *current = [player_stats objectForKey:achievid];
        if (current==nil) {
            // este deberia ser el caso comun
            [player_stats setObject:iachievment forKey:achievid];
        } else {
            // modificar el achievment actual
        }
        
    }
    
    achievments_loaded=true;
    // si tenia algun achievmente pendiente saber esto para enviar ahora es el momento
    [self dumpState];
}


- (void)scoreReported:(NSString *)scoreid withResult:(BOOL)result {
#if GCINTERFACE_DEBUG == 1
    printf("GCI score reported %s\n",result?"OK":"FAIL");
    [self dumpState];
#endif
    if (result) {
        [player_stats setObject:[player_stats_reported objectForKey:scoreid] forKey:scoreid];
        [player_stats_reported removeObjectForKey:scoreid];   
    }
}


- (void)achievmentReported:(NSString *)achievid withResult:(BOOL)result {
#if GCINTERFACE_DEBUG == 1
    printf("GCI achievment reported %s\n",result?"OK":"FAIL");
    [self dumpState];
#endif
    if (result) {
        [player_stats setObject:[player_stats_reported objectForKey:achievid] forKey:achievid];
        [player_stats_reported removeObjectForKey:achievid];
    }    
}


- (void)reportScore:(NSString *)scoreid value:(int)value {
#if GCINTERFACE_DEBUG == 1
    printf("GCI report score \n");
#endif

    if (localplayer.isAuthenticated) {
        GKScore *current=[player_stats objectForKey:scoreid];
     //   GKScore *reported=[player_stats_reported objectForKey:scoreid];
        GKScore *waiting=[player_stats_waiting objectForKey:scoreid];
        GKScore *newscore=[[[GKScore alloc] initWithCategory:scoreid] autorelease];
        newscore.value=value;
        
        if (current==nil) {
            // no tenemos el score esperamos
            if (waiting==nil) {
                [player_stats_waiting setObject:newscore forKey:scoreid];
            } else {
                if ([scoreid isEqualToString:@"com.bwarestudios.pilgrim_total"]) {
                    waiting.value+=newscore.value;
                } else {
                    if (newscore.value>waiting.value) {
                        waiting.value=newscore.value;
                    }
                }
            }
        } else {
            [self sendScore:scoreid value:newscore currentValue:current];
        }
        
        
    } else {
        // en caso contrario guardar el score para despues
    }
        
}


- (void)reportAchievment:(NSString *)achievid value:(double)value {
#if GCINTERFACE_DEBUG == 1
    printf("GCI report achievment [%s]\n",[achievid UTF8String]);
#endif
    
    if (localplayer.isAuthenticated) {
        GKAchievement *current=[player_stats objectForKey:achievid];
    //    GKAchievement *reported=[player_stats_reported objectForKey:achievid];
        GKAchievement *waiting=[player_stats_waiting objectForKey:achievid];
        
        GKAchievement *newachievment=[[[GKAchievement alloc] initWithIdentifier:achievid] autorelease];
        newachievment.percentComplete=value;
        if (!achievments_loaded) {
            // no tenemos los achievments esperamos (aunque para los 100% no haria falta esperar
            if (waiting==nil) {
                [player_stats_waiting setObject:newachievment forKey:achievid];
            } else {
                waiting.percentComplete+=newachievment.percentComplete;
            }
        
        } else {
            double currentpercent=0.0;
            if (current!=nil) {
                currentpercent=current.percentComplete;
            }
            newachievment.percentComplete+=currentpercent;
            if (newachievment.percentComplete>100.0) {
                newachievment.percentComplete=100.0;
            }
            if (currentpercent<100.0) {
                [self sendAchievment:achievid value:newachievment];
            }
        }
    } 
    
    
}



//- (void)reportResult:(int)resultado {
//    GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:@"com.bwarestudios.pilgrim_top"] autorelease];
//    scoreReporter.value = resultado;
//    //scoreReporter.shouldSetDefaultLeaderboard=YES;
//    NSLog(@"reporting to gamecenter.... :\n%@\n",scoreReporter);
//    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
//        if (error != nil)
//        {
//            printf("error reporting score\n");
//        } else {
//            printf("score reported ok %d\n",(unsigned int)error);                
//        }
//    }];
//    total_delta_toreport+=resultado;
//    
//    
//    
//    GKLeaderboard *my_total_request = [[GKLeaderboard alloc] initWithPlayerIDs:[NSArray arrayWithObject:localplayer.playerID]];
//    my_total_request.category=@"com.bwarestudios.pilgrim_total";
//    NSLog(@"leaderboard request: %@",my_total_request);
//    [my_total_request loadScoresWithCompletionHandler:^(NSArray *scores, NSError *error) {
//        printf("get total returned...\n");
//        if (error!=nil) {
//            NSLog(@"error geting total_distance %@\n",error);
//            return;
//        }
//        if (scores!=nil) {
//            GKScore *my_total=(GKScore*)[scores objectAtIndex:0];
//            printf("recibido total_distance=%d\n",my_total.value);
//            if (total_delta_toreport>0) {
//                my_total.value+=total_delta_toreport;
//                total_delta_reported=total_delta_toreport;
//                [my_total reportScoreWithCompletionHandler:^(NSError *error) {
//                    if (error != nil)
//                    {
//                        printf("error reporting score\n");
//                    } else {
//                        printf("score reported ok %d\n",(unsigned int)error);
//                        total_delta_toreport-=total_delta_reported;
//                    }
//                }
//                ];
//            }
//        } else { // no hay score porque nunca he recorrido nada
//            if (total_delta_toreport>0) {
//                GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:@"com.bwarestudios.pilgrim_total"] autorelease];
//                scoreReporter.value = total_delta_toreport;
//                //scoreReporter.shouldSetDefaultLeaderboard=YES;
//                NSLog(@"reporting to gamecenter.... :\n%@\n",scoreReporter);
//                total_delta_reported=total_delta_toreport;
//                [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
//                    if (error != nil)
//                    {
//                        printf("error reporting score\n");
//                    } else {
//                        printf("score reported ok %d\n",(unsigned int)error);
//                        total_delta_toreport-=total_delta_reported;
//
//                    }
//                }];
//            }
//            
//        }
//    } 
//    ];
//}

- (void)dumpState {
    NSEnumerator *i;
    id o;
    printf("player_stats -----\n");
    i=[player_stats objectEnumerator];
    while (o=[i nextObject]) {
        if ([o isMemberOfClass:NSClassFromString(@"GKScore")]) {
            GKScore *s=(GKScore *)o;
            printf("Score %s :: %lld\n",[s.category UTF8String],s.value );
        }
        if ([o isKindOfClass:NSClassFromString(@"GKAchievement")]) {
            GKAchievement *a=(GKAchievement *)o;
            printf("Achievment %s :: %f\n",[a.identifier UTF8String],a.percentComplete );
        }
    }
    printf("player_stats_reported -----\n");
    i=[player_stats objectEnumerator];
    while (o=[i nextObject]) {
        if ([o isMemberOfClass:NSClassFromString(@"GKScore")]) {
            GKScore *s=(GKScore *)o;
            printf("Score %s :: %lld\n",[s.category UTF8String],s.value );
        }
        if ([o isKindOfClass:NSClassFromString(@"GKAchievment")]) {
            GKAchievement *a=(GKAchievement *)o;
            printf("Achievment %s :: %f\n",[a.identifier UTF8String],a.percentComplete );
        }
    }
    
}

@end



BOOL isGameCenterAPIAvailable(void) {
    // Check for presence of GKLocalPlayer class.
    BOOL localPlayerClassAvailable = (NSClassFromString(@"GKLocalPlayer")) != nil;
    
    // The device must be running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (localPlayerClassAvailable && osVersionSupported);
}



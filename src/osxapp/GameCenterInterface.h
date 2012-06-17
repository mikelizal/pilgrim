// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE
#import <GameKit/GameKit.h>
#endif

extern BOOL gamecenter_available;

BOOL isGameCenterAPIAvailable(void);


@interface GameCenterInterface : NSObject
{
    GKLocalPlayer *localplayer;
    
    NSString *gamecenterCachePath;
    
    // Dictionary for current player stats
    // store dictionaries with key playerid
    NSMutableDictionary *stats;
    NSMutableDictionary *stats_reported; // reported waitng confirmation
    NSMutableDictionary *stats_waiting; // waiting to be reported
    
    NSMutableDictionary *player_stats;
    NSMutableDictionary *player_stats_reported;
    NSMutableDictionary *player_stats_waiting;
    
    bool achievments_loaded;
    
}

- (GameCenterInterface *) init;

// own actions

/// start authentication at the begginigng or after local player has changed returning from background
- (void)startAuthenticate;

- (void)requestScore:(NSString *)scoreid;

- (void)requestAchievments;

- (void)sendScore:(NSString *)scoreid value:(GKScore *)score currentValue:(GKScore *)currentscore;

- (void)sendAchievment:(NSString *)achievid value:(GKAchievement *)achievment;

// callbacks de final de operacion

- (void)didAuthenticate;

- (void)didReceiveScore:(NSString *)scoreid value:(GKScore *)score;

- (void)didReceiveAchievments:(NSArray *)achievarray;

- (void)scoreReported:(NSString *)scoreid withResult:(BOOL)result;

- (void)achievmentReported:(NSString *)achievid withResult:(BOOL)result;

// eventos del juego

- (void)reportScore:(NSString *)scoreid value:(int)value;

- (void)reportAchievment:(NSString *)achievid value:(double)value;





//- (void)reportResult:(int)resultado;

// debug
- (void)dumpState;


@end

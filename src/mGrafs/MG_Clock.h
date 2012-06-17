// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import <Foundation/Foundation.h>

@protocol MG_ClockListener

-(void)tickTime:(float)t dt:(float)dt i:(int)i;

@end


@interface MG_Clock : NSObject {
    int i;
    float time;
    float last_time;
    CFAbsoluteTime t0;

    
    NSTimer *mainTimer;
    float interval;
    id <MG_ClockListener> callback_target;
    SEL callback_selector;
}

-(id)initWithFreq:(int)timespersecond callbackTarget:(id <MG_ClockListener>)target;
-(void)start;
-(void)stop;

- (void)tick:(NSTimer*)theTimer;

@end

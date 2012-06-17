// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#import "MG_Clock.h"






@implementation MG_Clock

// habria que hacer el target con un interface que obligue a implementar la funcion que se va a llamar
-(id)initWithFreq:(int)timespersecond callbackTarget:(id <MG_ClockListener>)target {
    interval=1.0/timespersecond;
    callback_target=target;

    i=0;
    time=0.0;
    last_time=0;
    return self;
}

-(void)start {
    mainTimer = [NSTimer scheduledTimerWithTimeInterval:interval target:self selector:@selector(tick:) userInfo:nil repeats:YES];
    t0=CFAbsoluteTimeGetCurrent();
    last_time=0;
}

-(void)stop {
    if (mainTimer) [mainTimer invalidate];
    mainTimer=nil;
}

- (void)tick:(NSTimer*)theTimer {
    i+=1;
    CFAbsoluteTime tnow;
    tnow=CFAbsoluteTimeGetCurrent();
    time=tnow-t0;
    float dt=time-last_time;    
    [callback_target tickTime:time dt:dt i:i];
    last_time=time;
}

@end

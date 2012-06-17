// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details

#include "pilgrim.h"
#include <sys/time.h>

struct timeval tref;
t_time t_at_tref;

void time_init(void) {
    gettimeofday(&tref, NULL);
    t_at_tref=0;
}

// se podria hacer solo con esta
// basicamente time_init es hacer time_new_ref(0.0)
void time_new_ref(t_time t) {
    gettimeofday(&tref, NULL);
    t_at_tref=t;
}


t_time time_now(void) {
    struct timeval now,d;
    t_time tresult;
    gettimeofday(&now, NULL);
    d.tv_usec=now.tv_usec-tref.tv_usec;
    d.tv_sec=now.tv_sec-tref.tv_sec;//+(d.tv_usec/1000000);
    d.tv_usec%=1000000;
    tresult=1.0*d.tv_sec+0.000001*d.tv_usec;
    return t_at_tref + tresult;
}

int time_get_time_seed(void) {
    struct timeval tseed;
    gettimeofday(&tseed, NULL);
    return tseed.tv_usec;
}


// Copyright (C) 2012 Mikel Izal Azcarate
// Copyright (C) 2012 Bware Studios
//
// This software is distributed under a MIT license, see LICENSE file for details


#if TARGET_OS_IPHONE
int main(int argc, char *argv[]) {    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
#else
int main(int argc, char *argv[]) {
    return NSApplicationMain(argc,  (const char **) argv);
}
#endif


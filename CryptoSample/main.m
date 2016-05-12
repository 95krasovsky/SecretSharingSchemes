////
////  main.m
////  CryptoSample
////
////  Created by Vladislav Krasovsky on 4/11/16.
////  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
////
//
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "ShamirWrapper.h"
int main(int argc, char * argv[]) {
//    @autoreleasepool {
//        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
//    }
    [[ShamirWrapper sharedInstance] testAndCompare];
    
}


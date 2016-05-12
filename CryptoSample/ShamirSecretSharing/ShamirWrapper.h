//
//  ShamirWrapper.h
//  CryptoSample
//
//  Created by Admin on 5/10/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ShamirWrapper : NSObject

+ (instancetype)sharedInstance;


- (void)testShamir;
- (void)testModular;

@end

//
//  ShamirWrapper.m
//  CryptoSample
//
//  Created by Admin on 5/10/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#import "ShamirWrapper.h"

#include "ShamirSSScheme.hpp"
#include "ModularSSScheme.hpp"

#include "base64.hpp"
#include <string>
#include <NTL//ZZX.h>
#include <NTL//ZZ.h>
#include <NTL//GF2X.h>

#import "GMEllipticCurveCrypto.h"
#import "GMEllipticCurveCrypto+hash.h"

using namespace std;
using namespace NTL;
@implementation ShamirWrapper


+ (instancetype)sharedInstance
{
    static ShamirWrapper *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ShamirWrapper alloc] init];
        // Do any other initialisation stuff here
    });
    return sharedInstance;
}

- (void)testModular{
    for (int i = 0; i < 4; i++) {
        
        GMEllipticCurve curve = GMEllipticCurveNone;
        NSString *curveName = nil;
        GMEllipticCurveCrypto *crypto;
        switch (i) {
            case 0:
                curve = GMEllipticCurveSecp128r1;
                curveName = @"GMEllipticCurveSecp128r1";
                break;
                
            case 1:
                curve = GMEllipticCurveSecp192r1;
                curveName = @"GMEllipticCurveSecp192r1";
                break;
                
            case 2:
                curve = GMEllipticCurveSecp256r1;
                curveName = @"GMEllipticCurveSecp256r1";
                break;
                
            case 3:
                curve = GMEllipticCurveSecp384r1;
                curveName = @"GMEllipticCurveSecp384r1";
                break;
        }
        
        NSLog(@"Testing: %@", curveName);
        
        // Generate a key pair
        // NSLog(@"  Generate Key Pair");
        crypto = [GMEllipticCurveCrypto generateKeyPairForCurve:curve];
        
        //        NSLog(@"    Public Key:  %@", crypto.publicKeyBase64);
               // NSLog(@"    Private Key: %@", crypto.privateKey);
               // NSLog(@"    Private Key64: %lu", crypto.privateKeyBase64.length);

        ZZ z = ZZFromBytes((unsigned char *)[crypto.privateKey bytes], [crypto.privateKey length]);
        ModularSSScheme shamir(3,3, z);
        ModularSSScheme::BigPolyVec peopleSecrets;
        ModularSSScheme::BigPolyVec openKeys;

        peopleSecrets = shamir.GetSecretParts();
        openKeys = shamir.GetOpenKeys();

        int nr = 3;
        
        ModularSSScheme::BigPolyVec keys(nr+1);
        ModularSSScheme::BigPolyVec secrets(nr);
        keys[0] = openKeys[0];

        for (int i =0; i < nr; i++){
            secrets[i] = peopleSecrets[i];
            keys[i+1] = openKeys[i+1];
        }
        
        shamir.AccesSecret(3, keys, secrets);
    }
    
}


- (void)testShamir {
    
    
    for (int i = 0; i < 4; i++) {
        
        GMEllipticCurve curve = GMEllipticCurveNone;
        NSString *curveName = nil;
        GMEllipticCurveCrypto *crypto;
        switch (i) {
            case 0:
                curve = GMEllipticCurveSecp128r1;
                curveName = @"GMEllipticCurveSecp128r1";
                break;
                
            case 1:
                curve = GMEllipticCurveSecp192r1;
                curveName = @"GMEllipticCurveSecp192r1";
                break;
                
            case 2:
                curve = GMEllipticCurveSecp256r1;
                curveName = @"GMEllipticCurveSecp256r1";
                break;
                
            case 3:
                curve = GMEllipticCurveSecp384r1;
                curveName = @"GMEllipticCurveSecp384r1";
                break;
        }
        
        NSLog(@"Testing: %@", curveName);
        
        // Generate a key pair
       // NSLog(@"  Generate Key Pair");
        crypto = [GMEllipticCurveCrypto generateKeyPairForCurve:curve];
 
//        NSLog(@"    Public Key:  %@", crypto.publicKeyBase64);
//        NSLog(@"    Private Key: %@", crypto.privateKeyBase64);
        NSDate *startDate = [NSDate date];
        ShamirSSScheme shamir(3,3, b64_to_ZZ(string([crypto.privateKeyBase64 UTF8String])));
        ShamirSSScheme::BigNrVec peopleSecrets;
        
        peopleSecrets = shamir.GetSecretParts();
        int nr = 3;
        vector<ShamirSSScheme::UInt> people(nr);
        people[0] = 0;
        people[1] = 1;
        people[2] = 2;
        ShamirSSScheme::BigNrVec secrets(nr);
        for (int i =0; i < nr; i++)
            secrets[i] = peopleSecrets[people[i]];
        
        shamir.AccesSecret(people, secrets);
        NSLog(@"Working time: %fu\n\n", [[NSDate date] timeIntervalSinceDate:startDate]);
        
    }
    
    
}



@end

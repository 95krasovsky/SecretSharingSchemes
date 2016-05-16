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
#include <iostream>
#include <fstream>

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
- (void)testModularManual{

        //        NSLog(@"    Public Key:  %@", crypto.publicKeyBase64);
        // NSLog(@"    Private Key: %@", crypto.privateKey);
        // NSLog(@"    Private Key64: %lu", crypto.privateKeyBase64.length);
        
        ZZ z = to_ZZ("257");
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
        ModularSSScheme modular(3,3, z);
        ModularSSScheme::BigPolyVec peopleSecrets;
        ModularSSScheme::BigPolyVec openKeys;

        peopleSecrets = modular.GetSecretParts();
        openKeys = modular.GetOpenKeys();

        int nr = 3;
        
        ModularSSScheme::BigPolyVec keys(nr+1);
        ModularSSScheme::BigPolyVec secrets(nr);
        keys[0] = openKeys[0];

        for (int i =0; i < nr; i++){
            secrets[i] = peopleSecrets[i];
            keys[i+1] = openKeys[i+1];
        }
        
        modular.AccesSecret(3, keys, secrets);
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

- (void)testAndCompare {
    
    for (int a = 0; a<5; a++){
        cout<<"5\t6"<<endl;
    }
    int repeat = 10;
        for (int i = 0; i < 2; i++) {
            GMEllipticCurve curve = GMEllipticCurveNone;
            NSString *curveName = nil;
            GMEllipticCurveCrypto *crypto;
            switch (i) {
                case 0:
                    curve = GMEllipticCurveSecp128r1;
                    curveName = @"GMEllipticCurveSecp128r1";
                    break;
                case 1:
                    curve = GMEllipticCurveSecp256r1;
                    curveName = @"GMEllipticCurveSecp256r1";
                    break;
            }
            
      //      NSLog(@"Testing: %@", curveName);
            
            // Generate a key pair
            // NSLog(@"  Generate Key Pair");
            crypto = [GMEllipticCurveCrypto generateKeyPairForCurve:curve];
            for (int io = 0; io < 2; io++){ // generating/accessing
                for (int N = 2; N <= 50; N += 8){
                    int pitch = ceil((N - 2)/10.0);
                    if (pitch <= 0) pitch = 1;
                    cout << ((i == 0) ? "128\t" : "256\t") << ((io == 0) ? "generating\t" : "accessing\t") << "N = " << N << endl;
                    cout << "K:\t" << "shamir time:\t" << "modular time:" << endl;

                    for (int K = 2; K <= N; K += pitch){
                        
                        NSTimeInterval shamirGeneratingInterval, shamirAccessingInterval, modularGeneratingInterval, modularAccessingInterval;
                        
                        for (int ii = 0; ii < repeat; ii++){

                            //        NSLog(@"    Public Key:  %@", crypto.publicKeyBase64);
                            //        NSLog(@"    Private Key: %@", crypto.privateKeyBase64);
                            NSDate *startDate = [NSDate date];
                            ShamirSSScheme shamir(N,K, b64_to_ZZ(string([crypto.privateKeyBase64 UTF8String])));
                            NSTimeInterval sGen = [[NSDate date] timeIntervalSinceDate:startDate];

                            ShamirSSScheme::BigNrVec peopleSecrets;
                            
                            peopleSecrets = shamir.GetSecretParts();
                            vector<ShamirSSScheme::UInt> people(K);
                                for (int i = 0; i < K; i++){
                                    people[i] = i;
                                }

                            ShamirSSScheme::BigNrVec secrets(K);
                            for (int i =0; i < K; i++)
                                secrets[i] = peopleSecrets[people[i]];
                            
                            
                            startDate = [NSDate date];
                            shamir.AccesSecret(people, secrets);
                            NSTimeInterval sAcc = [[NSDate date] timeIntervalSinceDate:startDate];

                            ZZ z = ZZFromBytes((unsigned char *)[crypto.privateKey bytes], [crypto.privateKey length]);
                            startDate = [NSDate date];
                            ModularSSScheme modular(N, K, z);
                            NSTimeInterval mGen = [[NSDate date] timeIntervalSinceDate:startDate];

                            ModularSSScheme::BigPolyVec peoplSecrets;
                            ModularSSScheme::BigPolyVec openKeys;
                            
                            peoplSecrets = modular.GetSecretParts();
                            openKeys = modular.GetOpenKeys();
                            
                            
                            ModularSSScheme::BigPolyVec keys(K+1);
                            ModularSSScheme::BigPolyVec secretss(K);
                            keys[0] = openKeys[0];
                            
                            for (int i =0; i < K; i++){
                                secretss[i] = peoplSecrets[i];
                                keys[i+1] = openKeys[i+1];
                            }
                            
                            startDate = [NSDate date];
                            modular.AccesSecret(K, keys, secretss);
                            NSTimeInterval mAcc = [[NSDate date] timeIntervalSinceDate:startDate];

//                            NSLog(@"Shamir working  time: %.5f", shamirInterval);
//                            NSLog(@"Modular working time: %.5f\n\n\n", modularInterval);
                            shamirGeneratingInterval += sGen;
                            shamirAccessingInterval += sAcc;
                            modularGeneratingInterval += mGen;
                            modularAccessingInterval += mAcc;
                        }
                        shamirGeneratingInterval /= repeat;
                        shamirAccessingInterval /= repeat;
                        modularGeneratingInterval /= repeat;
                        modularAccessingInterval /= repeat;
                        cout << K << "\t";
                        if (io == 0){
                            cout<< shamirGeneratingInterval << "\t" << modularGeneratingInterval << endl;
                        } else{
                            cout<< shamirAccessingInterval << "\t" << modularAccessingInterval << endl;

                        }
                    }
                }
            }

                        
      //  cout<<ii;
        }

    
//    NSLog(@"Avarage Shamir  time1: %.5f", s1/repeat);
//    NSLog(@"Avarage modular time1: %.5f\n\n\n", m1/repeat);
//    NSLog(@"Avarage Shamir  time2: %.5f", s2/repeat);
//    NSLog(@"Avarage modular time2: %.5f\n\n\n", m2/repeat);
//    
//    NSLog(@"AVERAGE SHAMIR  TIME: %.5f", (s1+s2)/2/repeat);
//    NSLog(@"AVERAGE MODULAR TIME: %.5f\n\n\n", (m1+m2)/2/repeat);


}



@end

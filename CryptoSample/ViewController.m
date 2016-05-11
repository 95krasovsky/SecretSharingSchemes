//
//  ViewController.m
//  CryptoSample
//
//  Created by Vladislav Krasovsky on 4/11/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

static const UInt8 publicKeyIdentifier[] = "com.apple.sample.publickey\0";
static const UInt8 privateKeyIdentifier[] = "com.apple.sample.privatekey\0";

- (void)generateKeyPairPlease
{
    OSStatus status = noErr;
    NSMutableDictionary *privateKeyAttr = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *publicKeyAttr = [[NSMutableDictionary alloc] init];
    NSMutableDictionary *keyPairAttr = [[NSMutableDictionary alloc] init];
    // 2
    
    NSData * publicTag = [NSData dataWithBytes:publicKeyIdentifier
                                        length:strlen((const char *)publicKeyIdentifier)];
    NSData * privateTag = [NSData dataWithBytes:privateKeyIdentifier
                                         length:strlen((const char *)privateKeyIdentifier)];
    // 3
    
    SecKeyRef publicKey = NULL;
    SecKeyRef privateKey = NULL;                                // 4
    
    [keyPairAttr setObject:(__bridge id)kSecAttrKeyTypeEC
                    forKey:(__bridge id)kSecAttrKeyType]; // 5
    [keyPairAttr setObject:[NSNumber numberWithInt:256]
                    forKey:(__bridge id)kSecAttrKeySizeInBits]; // 6
    
    [privateKeyAttr setObject:[NSNumber numberWithBool:YES]
                       forKey:(__bridge id)kSecAttrIsPermanent]; // 7
    [privateKeyAttr setObject:privateTag
                       forKey:(__bridge id)kSecAttrApplicationTag]; // 8
    
    [publicKeyAttr setObject:[NSNumber numberWithBool:YES]
                      forKey:(__bridge id)kSecAttrIsPermanent]; // 9
    [publicKeyAttr setObject:publicTag
                      forKey:(__bridge id)kSecAttrApplicationTag]; // 10
    
    [keyPairAttr setObject:privateKeyAttr
                    forKey:(__bridge id)kSecPrivateKeyAttrs]; // 11
    [keyPairAttr setObject:publicKeyAttr
                    forKey:(__bridge id)kSecPublicKeyAttrs]; // 12
    
    status = SecKeyGeneratePair((__bridge CFDictionaryRef)keyPairAttr,
                                &publicKey, &privateKey); // 13
    //    error handling...
    
    
    if(publicKey) CFRelease(publicKey);
    if(privateKey) CFRelease(privateKey);                       // 14
}
- (void)viewDidLoad {
    [super viewDidLoad];
    [self generateKeyPairPlease];
    
    // Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end

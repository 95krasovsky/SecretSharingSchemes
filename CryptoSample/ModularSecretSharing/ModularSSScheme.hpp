//
//  ModularSSScheme.hpp
//  CryptoSample
//
//  Created by Admin on 5/11/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#ifndef ModularSSScheme_hpp
#define ModularSSScheme_hpp

#include <stdio.h>
#include <NTL//ZZ.h>
#include <NTL//GF2X.h>

#include <vector>

#endif /* ModularSSScheme_hpp */

class ModularSSScheme
{
public:
    typedef unsigned int UInt;
    typedef NTL::ZZ BigNumber;
    typedef std::vector<NTL::GF2X> BigPolyVec;

public:
    ModularSSScheme(UInt n, UInt k, BigNumber sec);
    
public:
    ~ModularSSScheme(void);
    
public:
    const BigPolyVec& GetSecretParts();
    const BigPolyVec& GetOpenKeys();

    bool AccesSecret(int peopleNum, const BigPolyVec &openKeys, const BigPolyVec &peopleSecrets);
    
private:
    void GenerateParameters();
    void CalculateSharingParts();
    
private:
//    BigNrVec polynom; // coef of base polynom
    // polynom[0] = the secret
//    BigNrVec sharingParts; // secret parts know by each people
    BigPolyVec sharingParts; // secret parts know by each people
    BigPolyVec openKeys; // secret parts know by each people

    UInt N; // total number of people which have a piece of secret
    UInt K; // nr of people need to discover the secret
    long secretLength;
    BigNumber secret; // the prime number for modulo opration
};
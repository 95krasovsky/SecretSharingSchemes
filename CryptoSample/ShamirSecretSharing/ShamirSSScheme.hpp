//
//  ShamirSSScheme.hpp
//  CryptoSample
//
//  Created by Admin on 5/10/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#ifndef ShamirSSScheme_hpp
#define ShamirSSScheme_hpp

#include <stdio.h>
#include <NTL//ZZ.h>

#include <vector>

#endif /* ShamirSSScheme_hpp */

class ShamirSSScheme
{
public:
    typedef unsigned int UInt;
    typedef NTL::ZZ BigNumber;
    typedef std::vector<BigNumber> BigNrVec;
public:
    ShamirSSScheme(UInt n, UInt k);
    ShamirSSScheme(UInt n, UInt k, BigNumber sec);

public:
    ~ShamirSSScheme(void);
    
public:
    const BigNrVec& GetSecretParts();
    bool AccesSecret(const std::vector<UInt>& people, const BigNrVec &peopleSecrets);
    
private:
    void GeneratePolynom();
    void CalculateSharingParts();
    
private:
    BigNrVec polynom; // coef of base polynom
    // polynom[0] = the secret
    BigNrVec sharingParts; // secret parts know by each people
    
    UInt N; // total number of people which have a piece of secret
    UInt K; // nr of people need to discover the secret
    
    BigNumber primeNr; // the prime number for modulo opration
};

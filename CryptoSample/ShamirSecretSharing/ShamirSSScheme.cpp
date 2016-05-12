//
//  ShamirSSScheme.cpp
//  CryptoSample
//
//  Created by Admin on 5/10/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#include "ShamirSSScheme.hpp"
#include <iostream>
using namespace std;
using namespace NTL;


ShamirSSScheme::ShamirSSScheme(UInt n, UInt k, BigNumber sec) : N(n), K(k), sharingParts(n), polynom(k)
{
    primeNr = NextPrime(sec);
   // cout << "primeNr " << primeNr << '\n';
    polynom[0] = sec;
    GeneratePolynom();
    CalculateSharingParts();
}

ShamirSSScheme::~ShamirSSScheme(void)
{
}

// Generate the base polynom for calcul
// the coeficient are in the order-
//  polynom[i] = coef for x^i
// polynom[0] = the secret
void ShamirSSScheme::GeneratePolynom()
{
    cout << "Shamir SSS generating parameters..."<<endl;

//    cout << "secret: pol[0] = " << polynom[0] << '\n';

    for (UInt i = 1; i < K; i++)    {
        RandomBnd(polynom[i], primeNr);
    //    cout << "pol[" << i << "] = " << polynom[i] << '\n';
    }    
}

void ShamirSSScheme::CalculateSharingParts()
{
    BigNumber aux;
    for (UInt i = 0; i < N; i++)
    {
        sharingParts[i] = 0;
        for (UInt j = 0; j < K; j++)
        {
            NTL::PowerMod(aux, NTL::to_ZZ(i + 1), j, primeNr);
            sharingParts[i] = (sharingParts[i] + polynom[j] * aux) % primeNr;
        }
        //cout << "share[" << i << "] = " << sharingParts[i] << '\n';
    }
}

const ShamirSSScheme::BigNrVec& ShamirSSScheme::GetSecretParts()
{
    return sharingParts;
}

bool ShamirSSScheme::AccesSecret(const std::vector<UInt>& vPeople, const ShamirSSScheme::BigNrVec &vPeopleSecrets)
{
    cout << "Trying to acces the secret...\n";
    UInt peopleNr = (UInt)vPeople.size();
    if (peopleNr != vPeopleSecrets.size())
    {
        cout << "People nr and secret nr are  not equal\n";
        return false;
    }
    
    BigNumber secret, aux, aux1;
    for (UInt i = 0; i < peopleNr; i++)
    {
        aux1 = 1;
        for (UInt j = 0; j < peopleNr; j++)
        {
            if (vPeople[j] != vPeople[i])
            {
                aux = (int)vPeople[j] - (int)vPeople[i];
                while (aux <= 0)
                    aux += primeNr;
                MulMod(aux1, aux1, ((vPeople[j] + 1)*InvMod(aux, primeNr)) % primeNr, primeNr);
            }
        }
        secret = (secret + vPeopleSecrets[i] * aux1) % primeNr;
    }
    
//    cout << "Secret: " << polynom[0] << '\n';
//    cout << "Secret discovered: " << secret << '\n';
    if (secret == polynom[0])
    {
        cout << "Secret Succefuly accesed"<<endl;
        return true;
    }
    
    cout << "\nAcces Denied!\n";
    return false;
}
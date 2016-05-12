//
//  ModularSSScheme.cpp
//  CryptoSample
//
//  Created by Admin on 5/11/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#include "ModularSSScheme.hpp"
#include <NTL//GF2X.h>
using namespace std;
using namespace NTL;

ModularSSScheme::ModularSSScheme(UInt n, UInt k, BigNumber sec) : N(n), K(k), openKeys(n+1), sharingParts(n)
{
    secret = sec;
    secretLength = NumBits(secret);

  //  cout << "secret " << secret << '\n';
    GenerateParameters();
    CalculateSharingParts();
}

ModularSSScheme::~ModularSSScheme(void)
{
}

const ModularSSScheme::BigPolyVec& ModularSSScheme::GetSecretParts()
{
    return sharingParts;
}

const ModularSSScheme::BigPolyVec& ModularSSScheme::GetOpenKeys()
{
    return openKeys;
}

void ModularSSScheme::GenerateParameters()
{
    cout<<"secret: "<<secret<< " secret size: "<<secretLength<<endl;
    for (UInt i = 0; i < N + 1; i++)    {
        GF2X m;
        //cout<<m<<" deg: "<< deg(m)<<endl;
        bool flag = false;
        while(!flag){
            random(m, secretLength);
            while(deg(m) < secretLength - 1){
                random(m, secretLength);
            }
            flag = true;
            for (int j = 0; j < i; j++){
                GF2X gcd = GCD(openKeys[j], m);
                if (!IsOne(gcd)){
                    flag = false;
                    break;
                }
            }
        }
        
        openKeys[i] = m;

        //RandomBnd(polynom[i], primeNr);
            cout << "sharing[" << i << "] = " << openKeys[i] << '\n';
    }
}

void convertZZtoGF2X(GF2X& poly, const ZZ& num){
    poly = GF2X();
    long n = NumBits(num);
    for (int i = 0; i < n; i++){
        SetCoeff(poly, i, bit(num, i));
    }
}


void ModularSSScheme::CalculateSharingParts()
{
    GF2X q = random_GF2X(10);
    GF2X S;
    convertZZtoGF2X(S, secret);
    cout<<"Secret Poy = "<< S<<endl;
    GF2X C = (GF2X(secretLength) + openKeys[0])*q + S;
    for (int i = 0; i < N; i++){
        sharingParts[i] = C % (GF2X(secretLength) + openKeys[i+1]);
    }
}


bool ModularSSScheme::AccesSecret(int peopleNum, const BigPolyVec &keys, const BigPolyVec &secrets)

{
    cout << "\nTrying to acces the secret...\n";
    int j = 0;
    GF2X C = secrets[0];
    GF2X g = GF2X(secretLength) + keys[0 + 1];
    while (j < peopleNum - 1){
        j++;
        GF2X d, u, v;
        XGCD(d, u, v, g, GF2X(secretLength) + keys[j + 1]);
        if (!IsOne(d)){
            cout<<"ERROR: d != 1."<<endl;
            return false;
        }
        C = (u*g*secrets[j] + v*(GF2X(secretLength) + keys[j + 1]) * C) % (g * (GF2X(secretLength) + keys[j + 1]));
        g = g * (GF2X(secretLength) + keys[j + 1]);
    }
    
    GF2X S = C % (GF2X(secretLength) + keys[0]);
    

    GF2X sInitial;
    convertZZtoGF2X(sInitial, secret);
    cout << "vvvvvvvvvvvSecret: " << sInitial << '\n';
    cout << "Secret discovered: " << S << '\n';

    if (S == sInitial)
    {
        cout << "Secret Succefuly accesed\n";
        return true;
    }
    
    cout << "\nAcces Denied!\n";
    return false;
}

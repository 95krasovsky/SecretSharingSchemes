//
//  ModularSSScheme.cpp
//  CryptoSample
//
//  Created by Admin on 5/11/16.
//  Copyright © 2016 Vladislav Krasovsky. All rights reserved.
//

#include "ModularSSScheme.hpp"
#include <NTL//GF2X.h>
#include <NTL//GF2XFactoring.h>

using namespace std;
using namespace NTL;

void convertZZtoGF2X(GF2X& poly, const ZZ& num){
    poly = GF2X();
    long n = NumBits(num);
    for (int i = 0; i < n; i++){
        SetCoeff(poly, i, bit(num, i));
    }
}

ModularSSScheme::ModularSSScheme(UInt n, UInt k, BigNumber sec) : N(n), K(k), openKeys(n+1), sharingParts(n)
{
    secret = sec;
    secretLength = NumBits(secret);
   
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

//    GF2X x;
//    ZZ a;
//    a = to_ZZ("54863710505754252083741140341725622621948596604629535392624646333749541278275");
//    convertZZtoGF2X(x, a);
//    openKeys[0] = x;
//    
//    a = to_ZZ("65588484318624717913731539570285584164536780528862741872927973684712808212748");
//    convertZZtoGF2X(x, a);
//    openKeys[1] = x;
//    
//    a = to_ZZ("3347206698724854103002953444486242113548352197114481485974885049400441924807");
//    convertZZtoGF2X(x, a);
//    openKeys[2] = x;
//    
//    a = to_ZZ("100174858641616987760707287488097658608969342163298014951309050976362938695027");
//    convertZZtoGF2X(x, a);
//    openKeys[3] = x;
//    
//    a = to_ZZ("50595296500842625054703671115424258349258634545219818303972216446715604396999");
//    convertZZtoGF2X(x, a);
//    openKeys[4] = x;
//    
//    a = to_ZZ("37740638661174081410779557396186074059139498677587208606077767036844084663269");
//    convertZZtoGF2X(x, a);
//    openKeys[5] = x;

    
//    cout<<"secret: "<<secret<< " secret size: "<<secretLength<<endl;
//    for (UInt i = 0; i < N + 1; i++)    {
//        GF2X m;
//        //cout<<m<<" deg: "<< deg(m)<<endl;
//        bool flag = false;
//        while(!flag){
//            random(m, secretLength);
//            while(deg(m) < secretLength - 1){
//                random(m, secretLength);
//            }
//            flag = true;
//            for (int j = 0; j < i; j++){
//                GF2X gcd = GCD(openKeys[j], m);
//                if (!IsOne(gcd)){
//                    flag = false;
//                    break;
//                }
//            }
//        }
//        
//        openKeys[i] = m;
//
//        cout << "key[" << i << "] = " << openKeys[i] << '\n';
//    }
    cout<<"secret: "<<secret<< " secret size: "<<secretLength<<endl;
    cout << "Modular SSS generating parameters..."<<endl;

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
                GF2X gcd = GCD(openKeys[j] + GF2X(secretLength, 1),GF2X(secretLength, 1) + m);
                if (!IsOne(gcd)){
                    flag = false;
                    break;
                }
            }
            
        }
        
        openKeys[i] = m;
        
       // cout << "key[" << i << "] = " << openKeys[i] << '\n';
    }

}




void ModularSSScheme::CalculateSharingParts()
{
    GF2X q = random_GF2X(4);
//    GF2X q;
//    ZZ a = to_ZZ("5409037024278451086635049474788354402632644625124230974937869125975942450074978026710434893365869426277343983225313724806031323228813621889726983928112557");
//    convertZZtoGF2X(q, a);
    
    
    GF2X S;
    convertZZtoGF2X(S, secret);
   // cout<<"Secret Poly = "<< S<<endl;
    GF2X C;
    C = (GF2X(secretLength, 1) + openKeys[0]) * q + S;
    //cout << "middle secret C: "<< C<<endl;

    for (int i = 0; i < N; i++){
        sharingParts[i] = C % (GF2X(secretLength, 1) + openKeys[i+1]);
       // cout << "shared[" << i << "] = " << sharingParts[i] << '\n';
    }
}


bool ModularSSScheme::AccesSecret(int peopleNum, const BigPolyVec &keys, const BigPolyVec &secrets)

{
    cout << "Trying to acces the secret...\n";
    int j = 0;
    GF2X C = secrets[0];
    GF2X g = GF2X(secretLength, 1) + keys[0 + 1];

    while (j < peopleNum - 1){
        j++;
//        cout<<"j: "<<j<<endl;
//        cout<<"C: "<<C<<endl;
//        cout<<"g: "<<g<<endl;
//        cout<<"f: "<<GF2X(secretLength, 1) + keys[j + 1]<<endl;
//        
        GF2X d, u, v;
        XGCD(d, u, v, g, GF2X(secretLength, 1) + keys[j + 1]);
//        cout<<"d: "<<d<<endl;
//        cout<<"u: "<<u<<endl;
//        cout<<"v: "<<v<<endl;
        if (!IsOne(d)){
            cout<<"ERROR: d != 1."<<endl;
            return false;
        }
        C = (u*g*secrets[j] + v*(GF2X(secretLength, 1) + keys[j + 1]) * C) % (g * (GF2X(secretLength, 1) + keys[j + 1]));
        g = g * (GF2X(secretLength, 1) + keys[j + 1]);
    }
    
    GF2X S = C % (GF2X(secretLength, 1) + keys[0]);
    

    GF2X sInitial;
    convertZZtoGF2X(sInitial, secret);
//    cout << "initial    secret: " << sInitial << '\n';
//    cout << "discovered secret: " << S << '\n';

    if (S == sInitial)
    {
        cout << "Secret Succefuly accesed!"<<endl;
        return true;
    }
    
    cout << "\nAcces Denied!\n";
    return false;
}

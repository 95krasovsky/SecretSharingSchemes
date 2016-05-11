
#include <ZZ_pX.h>

#include <cstdio>

NTL_CLIENT


double clean_data(double *t)
{
   double x, y, z;
   long i, ix, iy, n;

   x = t[0]; ix = 0;
   y = t[0]; iy = 0;

   for (i = 1; i < 5; i++) {
      if (t[i] < x) {
         x = t[i];
         ix = i;
      }
      if (t[i] > y) {
         y = t[i];
         iy = i;
      }
   }

   z = 0; n = 0;
   for (i = 0; i < 5; i++) {
      if (i != ix && i != iy) z+= t[i], n++;
   }

   z = z/n;  

   return z;
}

void print_flag()
{


#if (defined(NTL_TBL_REM))
printf("TBL_REM ");
#elif (defined(NTL_TBL_REM_LL))
printf("TBL_REM_LL ");
#else
printf("DEFAULT ");
#endif


printf("\n");

}


int main()
{

#if (defined(NTL_TBL_REM) && defined(NTL_GMP_LIP) && !(defined(NTL_HAVE_LL_TYPE) && NTL_ZZ_NBITS == NTL_BITS_PER_LONG))
   {
      printf("999999999999999 ");
      print_flag();
      return 0;
   }

#endif

#if (defined(NTL_TBL_REM_LL) && !defined(NTL_GMP_LIP) && !defined(NTL_HAVE_LL_TYPE))
   {
      printf("999999999999999 ");
      print_flag();
      return 0;
   }

#endif

   SetSeed(ZZ(0));

   long n, k;

   n = 200;
   k = 10*NTL_ZZ_NBITS;

   ZZ p;

   RandomLen(p, k);
   if (!IsOdd(p)) p++;


   ZZ_p::init(p);         // initialization

   ZZ_pX f, g, h, r1, r2, r3;

   random(g, n);    // g = random polynomial of degree < n
   random(h, n);    // h =             "   "
   random(f, n);    // f =             "   "

   SetCoeff(f, n);  // Sets coefficient of X^n to 1

   // For doing arithmetic mod f quickly, one must pre-compute
   // some information.

   ZZ_pXModulus F;
   build(F, f);

   PlainMul(r1, g, h);  // this uses classical arithmetic
   PlainRem(r1, r1, f);

   MulMod(r2, g, h, F);  // this uses the FFT

   MulMod(r3, g, h, f);  // uses FFT, but slower

   // compare the results...

   if (r1 != r2) {
      printf("999999999999999 ");
      print_flag();
      return 0;
   }
   else if (r1 != r3) {
      printf("999999999999999 ");
      print_flag();
      return 0;
   }

   double t;
   long i;
   long iter;

   n = 1024;
   k = 1600;
   RandomLen(p, k);
   if (!IsOdd(p)) p++;

   ZZ_p::init(p);

   ZZ_pX a;
   random(a, n);
   long da = deg(a);

   ZZ_pXModRep modrep;
   ToZZ_pXModRep(modrep, a, 0, da);

   iter = 1;

   do {
     t = GetTime();
     for (i = 0; i < iter; i++) {
        ToZZ_pXModRep(modrep, a, 0, da);
     }
     t = GetTime() - t;
     iter = 2*iter;
   } while(t < 1);

   iter = iter/2;

   iter = long((3/t)*iter) + 1;

   double tvec[5];
   long w;

   for (w = 0; w < 5; w++) {
     t = GetTime();
     for (i = 0; i < iter; i++) {
        ToZZ_pXModRep(modrep, a, 0, da);
     }
     t = GetTime() - t;
     tvec[w] = t;
   } 


   t = clean_data(tvec);

   t = floor((t/iter)*1e12);

   if (t < 0 || t >= 1e15)
      printf("999999999999999 ");
   else
      printf("%015.0f ", t);

   printf(" [%ld] ", iter);

   print_flag();

   return 0;
}

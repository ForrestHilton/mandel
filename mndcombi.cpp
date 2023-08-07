/* mndcombi.cpp  by Wolf Jung (C) 2007-2019.  Defines classes:
   mndAngle, mndCombi, mandelPath, mandelPathTwo, mandelPathThree,
   mandelbrotPath, mandelPathExpo.

   These classes are part of Mandel 5.17, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/

#include <cmath>
#include <new>
#include "mndynamo.h"

int mndAngle::normalize(qulonglong &n, qulonglong &d, int &k)
{  if (!d) return 0;
   n %= d; while (!(n & 1) && !(d & 1)) { n >>= 1; d >>= 1; }
   int p; qulonglong n0, n1 = n, d1 = d, np;
   k = 0; while (!(d1 & 1)) { k++; d1 >>= 1; if (n1 >= d1) n1 -= d1; }
   n0 = n1;
   for (p = 1; p <= 65 - k; p++) { twice(n1, d1); if (n1 == n0) break; }
   if (k + p > 64) return 0;
   np = 1LL << (p - 1); np--; np <<= 1; np++; //2^p - 1 for p <= 64
   n0 = np; d >>= k; n1 = d; if (n1 > n0) { n1 = n0; n0 = d; }
   while (1) { d1 = n0 % n1; if (!d1) break; n0 = n1; n1 = d1; } //gcd n1
   n /= d/n1; n *= np/n1; d = np << k;
   return p;
}

void mndAngle::twice(qulonglong &n, qulonglong &d)
{  if (n >= d) return;
   if (!(d & 1)) { d >>= 1; if (n >= d) n -= d; return; }
   qulonglong large = 1LL; large <<= 63; //avoid overflow:
   if (n < large) { n <<= 1; if (n >= d) n -= d; return; }
   n -= large; n <<= 1; large -= (d - large); n += large;
}

int mndAngle::conjugate(qulonglong &n, qulonglong &d)
{  int j, k, p = normalize(n, d, k); if (!p || k) return 0;
   if (p == 2) n = 3LL - n; if (p <= 2) return 1;
   qulonglong np, u, na, nb, n1; //A is na < np < nb
   if (n & 1LL) { u = 0LL; na = n >> 1; np = na + (d >> 1) + 1LL; nb = np; }
   else { u = 1LL; np = n >> 1; na = np; nb = np + (d >> 1) + 1LL; }
   //Compute preimages of  n/d , such that the preperiodic angle
   // (u + n/d)/2^j  is on the same side  A  or  B  as the periodic  np/d .
   for (j = 2; j <= p; j++)
   {  if (np & 1LL) { np >>= 1; np += d >> 1; np++; } else np >>= 1;
      //Check if the old preperiodic angle is  < n/d , label this by k = 1.
      //Only for p <= 32 :  if (d*u < ((1LL << (j - 1)) - 1LL)*n)
      k = 0; n1 = n >> (p + 1 - j); //first j - 1 digits of p digits
      if (u < n1) k = 1;
      else if (u == n1)
      {  n1 |= (((1LL << (p + 1 - j)) - 1LL) & n) << (j - 1); //swap words
         if (n < n1) k = 1;
      }
      if (k)  { if (na < np && np < nb) u += 1LL << (j - 1); }
      else  { if (np <= na || nb <= np) u += 1LL << (j - 1); }
   }
   n = u; //Preperiod digits of the preimage determine the conjugate angle.
   for (j = 1; j <= p; j++) { twice(np, d); if (np == u) break; }
   if (j >= p) return p;
   while (1) { k = p % j; if (!k) break; p = j; j = k; }
   return j; //gcd of former j and p is period of characteristic point.
}//conjugate64

qulonglong mndAngle::wake(int k, int r, qulonglong &n)
{  if (k <= 0 || k >= r || r > 64) return 0LL;
   qulonglong d = 1LL; int j, s = 0; n = 1LL;
   for (j = 1; j < r; j++)
   {  s -= k; if (s < 0) s += r; if (!s) return 0LL;
      if (s > r - k) n += d << j;
   }
   d <<= (r - 1); d--; d <<= 1; d++; //2^r - 1 for r <= 64
   return d;
}

mdouble mndAngle::radians(qulonglong n, qulonglong d)
{  if (n >= d) return 0.0L;
   return 2.0L * PI * ((mdouble)(n)) / ((mdouble)(d));
}

int mndAngle::setAngle(qulonglong n, qulonglong d, int &k)
{  if (n >= d) return 0;
   N = n; D = d; P = normalize(N, D, K);
   k = K; return P;
}

/*
int mndAngle::realSpider(qulonglong n, qulonglong d, mdouble &c)
{  if (n >= d) return 0;
   int j, k, p = normalize(n, d, k); if (k || !p) return 0;
//update this old implementation:
   long int num2 = denom - num, n = num; if (2*n > denom) n = denom - num;
   mdouble z1 = 2.0*PI/mdouble(denom), *z = new mdouble[period+1];
   z[1] = cos(z1*n);
   for (j = 2; j <= 32; j++)
   {  if (j == 32) return -2;  //period > 30
      n *= 2; if (n > denom) n -= denom;
      if (num < n && n < num2) return j;  //z_j < z_1
      if (n == num) { period = j - 1; break; }
      //if (n == num2) from doubling
      z[j] = cos(z1*n);
   }
   int *side = new int[period]; z1 = cos(z1*0.5*num); z[period] = 0;
   for (j = 1; j < period; j++)
   { z[j] = 2.0*(z[j] - z1); if (z[j] < 0) side[j] = 1; else side[j] =0; }
   for (k = 1; k <= 50; k++)
   {  z1 = z[1];
      for (j = 1; j < period; j++)
      {  z[j] = z[j+1] - z1; if (z[j] < 0) z[j] = 0;
         z[j] = sqrt(z[j]); if (side[j]) z[j] = -z[j];
      }
   }
   c = z[1]; delete[] z; delete[] side; return 0;
   return 0;
}//*/

mdouble mndAngle::lambda(qulonglong N, qulonglong D,
   mdouble eps, int nmax) // = 1.0e-8, 100
{  eps *= 0.1L; if (!N || N >= D) return 1.0L;
   int j, k = 0; qulonglong s = 1ULL, N1 = N >> 1, N2 = N1 + (D >> 1);
   for (j = 1; j <= 63; j++)
   {  N <<= 1; if (N >= D) N -= D;
      if (N1 < N && N <= N2) s |= (1ULL << j);
      if (!N) { k = j; break; }
   }
   if (!k) return 3.0L; if (k == 1) return 2.0L;
   /* Now  k  is the preperiod and  s  is the kneading sequence,
      with bit  1 ~ c1 , 2 ~ c2 , 4 ~ c3 ...
      To determine a block of the Thurston matrix,  the path  [c, beta]
      is iterated.  A[j]  encodes the path  [c' , c'']  defined in step j
      as  2^16 * c' + c'' , with  beta ~ 0  and  c ~ k .
      X[j]  is a stack of elements not yet mapped.
      F[j] = 2^16 * a' + a''  means that path j is mapped to path a'
      and path a'', with  a'' = 0  when there is only one image.
      No mdouble array is used to encode the matrix,  because there
      are only one or two nonzero entries per column.
   */
   int c1, c2, nf, na = 1, nx = 1, Na, Nx; int *A, *F, *X;
   if (k <= 22) Na = ((k*(k + 1)) >> 1) + 2; else Na = 255; Nx = Na;
   A = new int[Na+1]; F = new int[Na+1]; X = new int[Nx+1];
   A[1] = k; X[1] = 1;
   while (nx)
   {  nf = X[nx]; c1 = A[nf]; c2 = c1 & 0xFF; c1 >>= 16; nx--;
      if ( ((s >> (k - c1)) & 1) == ((s >> (k - c2)) & 1) ) //same side
      { c2--; if (c1) c1--; c1 <<= 16; c1 |= c2; c2 = 0; }
      else //covers 0, two images
      { c2--; if (c1) c1--; c1 <<= 16; c1 |= k; c2 <<= 16; c2 |= k; }
      for (j = 1; j <= na; j++) if (A[j] == c1)
      { F[nf] = j << 16; c1 = 0; break; }
      if (c1) { na++; A[na] = c1; F[nf] = na << 16; nx++; X[nx] = na; }
      if (na >= Na - 1) return (mdouble)(na); //or increase Na
      if (!c2) continue;
      for (j = 1; j <= na; j++) if (A[j] == c2)
      { F[nf] |= j; c2 = 0; break; }
      if (c2) { na++; A[na] = c2; F[nf] |= na; nx++; X[nx] = na; }
      if (nx == Nx || na >= Na - 1) return (mdouble)(nx); //or increase Na , Nx
   }
   delete A; delete X;
   /* Since the block is primitve,  the largest eigenvalue is obtained
      fast by iterating the matrix and computing the growth factor.
      In principle,  repeated squaring would be faster,  but it
      requires two full mdouble arrays and more arithmetic operations.
   */
   qulonglong *V, *Vold, *Vtemp; mdouble lambda = 42.0L, lambdaold = 0.0L;
   V = new qulonglong[na+1]; Vold = new qulonglong[na+1];
   Vold[0] = (qulonglong)(na); for (j = 1; j<= na; j++) Vold[j] = 1uLL;
   s = 1ULL << 63; nf = 0;
   for (k = 1; k <= nmax; k++)
   {  for (j = 0; j <= na; j++) V[j] = 0ULL;
      for (j = 1; j <= na; j++)
      {  nx = F[j] & 0xFF; if (nx) V[nx] += Vold[j];
         V[F[j] >> 16] += Vold[j];
      }
      for (j = 1; j<= na; j++) V[0] += V[j];
      if (V[0] & s) { Vold[0] >>= 2; for (j = 0; j <= na; j++) V[j] >>= 2; }
      lambda = ((mdouble)(V[0])) / ((mdouble)(Vold[0]));
      lambdaold -= lambda; if (lambdaold < 0) lambdaold = -lambdaold;
      if (lambdaold < eps) nf++; else nf = 0; if (nf & 4) break;
      lambdaold = lambda; Vtemp = V; V = Vold; Vold = Vtemp;
   }
   delete Vold; delete V; delete F; return lambda;
   /* nf is needed because there may be little change at the beginning.
      This happens, e.g., for  1/4  6351/524288  3067/524288 .
      Another problem is given by approximate 2-cycles, e.g., for
      87383/1048576  86431/1048576 .
   */
}

/* Assuming that  u1  and  u2  are the numerators of conjugate angles of
   period  k , and  w  is the numerator of an angle of period  q , this
   function returns the numerator of the tuned angle of period  qk .
   The optional argument  n  requests the closest larger angle of period
   n , which is used in  toAngles()  before calling  backAngles() .
*/
qulonglong mndAngle::truncatedTuning(int k, qulonglong u1, qulonglong u2,
   int q, qulonglong w, int n) // = 0
{  if (!n) n = q*k;
   /* The implementation assumes that all arguments are > 0 , the numerators
      are appropriate for periods  k  and  q , respectively,  q >= 2 , and
      k < n <= 64 . Moreover,  (q - 2)k < n <= qk . The formulas get
      involved by allowing  qk > 64 , which happens already for
      1-2-4-5-9-13-17-22-25  in  toAngles()  since  q = 3 , k = 22, n = 25 .
   */
   int i, j; qulonglong u, L, M = 0LL, N = 0LL;
   //N  is first  n  digits of the numerator of the qk-periodic tunded angle.
   //If the n-periodic angle is smaller than the qk-periodic one,  take  ++N .
   if (q*k <= 64)
   {  for (i = q - 1; i >= 0; i--) M |= (w & (1LL << i) ? u2 : u1) << i*k;
      if (n == q*k) return M;
      //Now  N  will be the first  n  digits.  Denote the last  qk - n  digits
      //by  L , then the truncated angle will be larger if  NL > LN .
      L = 1LL << (q*k - n); L--; L &= M; N = M >> (q*k - n);
      if (M > ((L << n) | N)) return N; else return ++N;
   }
   for (i = q - 1; i >= 0; i--)
   {  j = n + (i - q)*k; u = (w & (1LL << i) ? u2 : u1);
      if (j <= 0) { N |= u >> -j; break; }
      N |= u << j;
   }
   if (n == q*k) return N;
   for (i = q - 1; i >= 0; i--) M |= (w & (1LL << i) ? u2 : u1) << i*k;
   //Now  N  is the first  n  digits and  M  is the last 64 digits of the
   //tuned numerator.  Denote the last  qk - n  digits by  L , then the
   //truncated angle will be larger if  NL > LN .
   if (q*k <= 2*n) //L is not longer than N
   {  L = 1LL << (q*k - n - 1); L--; L <<= 1; L++; L &= M;
      u1 = N >> (2*n - q*k); //first  qk - n  digits
      if (u1 > L) return N; if (u1 < L) return ++N;
      u1 = N & ((1LL << (2*n - q*k)) - 1LL); //last  2n - qk  digits
      u2 = N >> (q*k - n); //first  2n - qk  digits
      if (u1 > u2) return N; if (u1 < u2) return ++N;
      u2 = 1LL << (q*k - n - 1); u2--; u2 <<= 1; u2++; u2 &= N; //last qk - n
      if (L > u2) return N; else return ++N;
   }
   //Finally,  L is longer than  N , so replace it with  ML , where  L
   //denotes the last  n  digits of the tuned numerator and  M  is
   //the middle  qk - 2n  digits,  then the truncated angle will be
   //larger if  NML > MLN . Note that  M  will be shorter than  N  and  L .
   L = 1LL << (n - 1); L--; L <<= 1; L++; L &= M; M = 0LL;
   for (i = q - 1; i >= 0; i--)
   {  j = i*k - n; u = (w & (1LL << i) ? u2 : u1);
      if (j <= 0) { M |= u >> -j; break; }
      M |= u << j;
   }
   M &= (1LL << (q*k - 2*n)) - 1LL;
   u1 = (M << (3*n - q*k)) | (L >> (q*k - 2*n));
   if (N > u1) return N; if (N < u1) return ++N;
   u2 = L & ((1LL << (q*k - 2*n)) - 1LL);
   if (M > u2) return N; if (M < u2) return ++N;
   if (L > N) return N; else return ++N;
}

/////////////////////////////////////////////////////////////////////

int mndCombi::phi(int n) //Euler totient for  n <= 120
{  if (n <= 0 || n > 120) return 0;
   int phi = 1;  if (!(n % 2)) n /= 2; while (!(n % 2)) { n /= 2; phi *= 2; }
   if (!(n % 3)) { n /= 3; phi *= 2; } while (!(n % 3)) { n /= 3; phi *= 3; }
   if (!(n % 5)) { n /= 5; phi *= 4; } while (!(n % 5)) { n /= 5; phi *= 5; }
   if (!(n % 7)) { n /= 7; phi *= 6; } while (!(n % 7)) { n /= 7; phi *= 7; }
   if (n > 1) phi *= (n - 1); //last  n  was prime if  1 < n < 121
   return phi;
}

/* rho(m)  is the smallest index  j > m , such that the  j-th  entry of the
   kneading sequence differs from the periodic continuation of the first
   m  entries,  or simpler,  such that it differs from the entry at  j - m .
   This implementation treats the kneading sequence as *-periodic,  thus
   rho(m) <= P  for  m < P , and it does not work for  m >= P .
*/
int mndCombi::rho(int m) const
{  if (m <= 0 || m >= P) return 0;
   int j; qulonglong u = K >> m; u ^= K;
   for (j = m + 1; j < P; j++) if ((u >> (P - j)) & 1LL) return j;
   return P;
}

int mndCombi::setKneading(qulonglong k)
{  qulonglong u = 1LL; if (k <= u) { P = 0; return 0; }
   K = k; for (P = 1; P <= 64; P++) { k >>= 1; if (!k) break; }
   int m = 1, j = 0; A = u;
   while (m < P) { j = m; m = rho(m); A |= u << (m - 1); }
   //K  is the upper sequence,  if it differs from  k  in the last entry:
   k = K >> j; if (!(k & u)) K |= u; else if (K & u) K--;
   return P;
}

int mndCombi::setAddress(qulonglong a)
{  K = 1LL; if (a <= K || !(a & 1LL)) { P = 0; return 0; }
   A = a; P = 1; int i, j, p;
   for (i = 2; i <= 64; i++)
   {  a >>= 1; if (!(a & 1LL)) continue;
      p = P; P = i; K <<= P - p;
      for (j = 1; j <= (P - 1)/p; j++) K |= K >> p;
      if (K & 1LL) K--; else K++;
   }
   return P;
}

int mndCombi::fromAngle(qulonglong n, qulonglong d)
{  K = 1LL; int i; P = mndAngle::normalize(n, d, i);
   if (i || !P) { P = 0; return 0; }
   qulonglong na = n >> 1, nb = na + (d >> 1) + 1; //na < n < nb is A
   for (i = 1; i < P; i++)
   { K <<= 1; mndAngle::twice(n, d); if (na < n && n < nb) K |= 1LL; }
   setKneading(K); return P; //resets last entry to upper sequence
}

int mndCombi::renorm() const
{  int m, r; if (!P) return 0;
   for (r = 2; r < P; r++)
   {  if (!((A >> (r - 1)) & 1LL)) continue;
      for (m = r + 1; m <= P; m++)
      {  if (!((A >> (m - 1)) & 1LL)) continue;
         if (m % r) { m = 0; break; }
      }
      if (m) return r;
   }
   return 1;
}

int mndCombi::failsBS() const
{  int m, r; if (!P) return -1;
   for (m = rho(1) + 1; m < P; m++)
   {  if ((A >> (m - 1)) & 1LL) continue; //m  in address,  thus not evil
      for (r = 2; r < m; r++) if (!(m % r) && rho(r) > m) { r = 0; break; }
      if (!r) continue; //not exact period  m
      r = rho(m) % m; if (!r) return m; // ~ r = m , so  m  is in  orb(r)
      while (r < P) { if (r == m) return m; r = rho(r); }
   }
   return 0;
}//*/

int mndCombi::count() const //assumes the combinatorics is realized
{  int j, k = 1, n, q, N = 1; if (!P) return -1; //invalid
   //Determine the denominators for all periods in the internal address:
   for (n = 2; n <= P; n++)
   {  if (!((A >> (n - 1)) & 1LL)) continue; //not in the internal address
      q = n/k; j = n % k;
      if (j) { q += 2; while (j < k) { j = rho(j); if (j == k) q--; } }
      N *= phi(q); k = n;
   }
   return N;
}

int mndCombi::toAngles(qulonglong &n1, qulonglong &n2, qulonglong &d) const
{  if (!P) return 1000; //invalid
   int j, k = 1, n, q; n1 = 0LL; n2 = 1LL; d = 1LL;
   //Determine characteristic angles for all periods in the internal address:
   for (n = 2; n <= P; n++)
   {  if (!((A >> (n - 1)) & 1LL)) continue; //not in the internal address
      //Determine the denominator of the sublimb  1/q :
      q = n/k; j = n % k;
      if (j) { q += 2; while (j < k) { j = rho(j); if (j == k) q--; } }
      //Restrict the old characteristic wake to  n-periodic angles:
      d = n1; n1 = mndAngle::truncatedTuning(k, d, n2, q, 1LL, n);
      n2 = mndAngle::truncatedTuning(k, d, n2, q, 2LL, n);
      d = 1LL << (n - 1); d--; d <<= 1; d++; //2^n - 1 for n <= 64
      if (!(n % k)) { k = n; continue; } //satellite bifurcation
      k = n; n2--; // n1/d  is larger and  n2/d  is smaller than  qk-angles.
      if (n2 - n1 == 1LL) continue; //always admissible
#ifdef COUNT_EARLY_RETURNS
      qulonglong u, u1 = n1, u2 = n2;
#endif
      //Iterate the old wake backwards along the new kneading sequence:
      if (backAngles(n, n1, n2, d)) return n; //Error, thus not admissible
#ifdef COUNT_EARLY_RETURNS
      if (n == P) //count the returns to previous wake
      {  q = 0; u = n1;
         for (j = 1; j < n; j++)
         { mndAngle::twice(u, d); if (u1 <= u && u <= u2) q++; }
         return -q;
      }
#endif
   }
   return 0;
}

int mndCombi::backAngles(int n,
   qulonglong &n1, qulonglong &n2, qulonglong d) const
{  int j; qulonglong N1, N2, N3, N4;
   //The preimages  [N1, N2]  and  [N3, N4]  of the reduced characteristic
   //wake  [n1, n2]  will be pulled back following the kneading sequence.
   N1 = (n1 + 1LL) >> 1; N2 = n2 >> 1;
   N4 = (d >> 1) + 1LL; N3 = (n1 >> 1) + N4; N4 += (n2 - 1LL) >> 1;
   for (j = n - 1; j > 0; j--)
   {  //When preimages belong to the characteristic wake,  try subdivision:
      if (n1 <= N1 && N1 <= n2)
      {  if (N1 > N4) { N1 = N3; N4 = N2; }
         N2 = n2; n2 = N1 - 1LL; //first subinterval
         if (n1 < n2 && !backAngles(n, n1, n2, d)) return 0;
         n1 = N4 + 1LL; n2 = N2; //second subinterval
         if (n1 < n2) return backAngles(n, n1, n2, d); else return 1;
      }
      //The preimages of the intervals are shrunk to  1/2  at least,
      //or more depending on a numerator being even or odd:
      if ((K >> (P - j)) & 1LL) //A in kneading sequence
      {  if (N1 > n2) { N1++; N1 >>= 1; N2 >>= 1; }
         else { N1 >>= 1; N1++; N1 += d >> 1; N2++; N2 >>= 1; N2 += d >> 1; }
         if (N3 > n2) { N3++; N3 >>= 1; N4 >>= 1; }
         else { N3 >>= 1; N3++; N3 += d >> 1; N4++; N4 >>= 1; N4 += d >> 1; }
      }
      else //B in kneading sequence
      {  if (N2 < n1) { N1++; N1 >>= 1; N2 >>= 1; }
         else { N1 >>= 1; N1++; N1 += d >> 1; N2++; N2 >>= 1; N2 += d >> 1; }
         if (N4 < n1) { N3++; N3 >>= 1; N4 >>= 1; }
         else { N3 >>= 1; N3++; N3 += d >> 1; N4++; N4 >>= 1; N4 += d >> 1; }
      }
      if (N1 > N2 || N3 > N4) return 1;
   }
   //Now  N1 = N2  and  N3 = N4  are the new characteristic angles times d.
   if (N1 > N3) { n2 = N1; n1 = N3; } else { n1 = N1; n2 = N3; }
   return 0;
}//backAngles

//////////////////////////////////////////////////////////////////////////

void mandelPath::root(mdouble x, mdouble y, mdouble &u, mdouble &v)
{  v = sqrt(x*x + y*y);
   if (x > 0.0L) { u = sqrt(0.5L*(v + x)); v = 0.5L*y/u; return; }
   if (x < 0.0L)
   { v = sqrt(0.5L*(v - x)); if (y < 0.0L) v = -v; u = 0.5L*y/v; return; }
   if (y >= 0.0L) { u = sqrt(0.5L*y); v = u; return; }
   u = sqrt(-0.5L*y); v = -u;
}

//////////////////////////////////////////////////////////////////////////

int mandelPathMate::mating(int k0, int p0, mdouble rc, mdouble ic,
   mdouble logR1, int which)
{  if (which) { K = k0; P = p0; rq = rc; iq = ic; }
   else { k = k0; p = p0; rp = rc; ip = ic; }
   if (logR1 > 0.0L) logR = logR1; if (!(p*P)) return 1;
   int i, m, kp = k + p, kpkp = kp + K + P;
   mdouble x = rp, y = ip, u, v, w, RS, ors, omt, rt, it;
   delete[] X; delete[] Y;
   X = new mdouble[1 + M*kpkp]; Y = new mdouble[1 + M*kpkp];
   X[1] = x; Y[1] = y;
   for (i = 2; i < kp; i++)
   {  w = x*x - y*y + rp; y = 2.0L*x*y + ip; x = w; X[i] = x; Y[i] = y;
      if (x*x + y*y > 4.0L) { p = 0; return -1; }
   }
   if (k) { X[kp] = -X[k]; Y[kp] = -Y[k]; } else { X[p] = 0.0L; Y[p] = 0.0L; }
   x = rq; y = iq; X[kp + 1] = x; Y[kp + 1] = y;
   for (i = 2; i < K + P; i++)
   {  w = x*x - y*y + rq; y = 2.0L*x*y + iq; x=w; X[kp + i] = x; Y[kp + i]= y;
      if (x*x + y*y > 4.0L) { P = 0; return -1; }
   }
   if (K) { X[kpkp] = -X[kp + K]; Y[kpkp] = -Y[kp + K]; }
   else  { X[kpkp] = 0.0L; Y[kpkp] = 0.0L; } //should be infinite
   for (m = M - 1; m >= 0; m--)
   {  omt = ((mdouble)(m))/((mdouble)(M)); RS = exp(logR*exp(logTwo*omt));
      w = RS*RS; ors = omt/(w*RS);
      x = w + omt*rq; y = omt*iq; u = w + omt*rp; v = omt*ip;
      w = u*u + v*v; rt = (x*u + y*v)/w; it = (y*u - x*v)/w;
      for (i = 1; i < kp; i++)
      {  u = RS + ors*(rq*(X[i] - rp) - iq*(Y[i] - ip));
         v = ors*(iq*(X[i] - rp) + rq*(Y[i] - ip)); w = u*u + v*v;
         x = (X[i]*u + Y[i]*v)/w; y = (Y[i]*u - X[i]*v)/w;
         X[i + m*kpkp] = rt*x - it*y; Y[i + m*kpkp] = rt*y + it*x;
      }
      if (k) { X[kp + m*kpkp]= -X[k + m*kpkp]; Y[kp + m*kpkp]= -Y[k + m*kpkp];}
      else { X[kp + m*kpkp]= 0.0L; Y[kp + m*kpkp]= 0.0L; }
      for (i = kp + 1; i < kpkp; i++)
      {  u = RS + ors*(rp*(X[i] - rq) - ip*(Y[i] - iq));
         v = ors*(ip*(X[i] - rq) + rp*(Y[i] - iq)); w = X[i]*X[i] + Y[i]*Y[i];
         x = (X[i]*u + Y[i]*v)/w; y = (X[i]*v - Y[i]*u)/w;
         X[i + m*kpkp] = rt*x - it*y; Y[i + m*kpkp] = rt*y + it*x;
      }
      if (K)
      {X[(m+1)*kpkp]= -X[kp + K + m*kpkp]; Y[(m+1)*kpkp]= -Y[kp + K + m*kpkp];}
      else { X[(m+1)*kpkp]= 0.0L; Y[(m+1)*kpkp]= 0.0L; } //unused
   }
   if (draw)
   {  delete[] X0; delete[] Y0;
      X0 = new mdouble[1 + M*kpkp]; Y0 = new mdouble[1 + M*kpkp];
      for (i = 1; i <= M*kpkp; i++) { X0[i] = X[i]; Y0[i] = Y[i]; }
   }
  /*cout << rp << "  " << ip << " p&q " << rq << "  " << iq << endl
  << X[1] << "  " << Y[1] << " and " << X[kp+1] << "  " << Y[kp+1] << endl;//*/
   return 0;
}

/*
At present there is no check for homotopy violations.  I need to figure out
a simple way of dealing with curves that are not hyperbolas.
Maybe there is a tradeoff,  such that many small segments mean fewer homotopy
violations,  but more numerical cancellations.
*/

int mandelPathMate::step(mdouble &rC, mdouble &iC)
{  if (!(p*P)) return -1;
   int i, kpkp = k + p + K + P, oldset = 0, offset = (M - 1)*kpkp;
   mdouble rc, ic, rd, id, rt, it, u, v, w, x, y;
   while (offset >= 0)
   {  rc = X[1 + offset]; ic = Y[1 + offset];
      rd = X[k + p + 1 + offset]; id = Y[k + p + 1 + offset];
      x = rd - 1.0L; u = rc - 1.0L; w = u*u + ic*ic; if (w<1.0e-80L) return 1;
      rt = (x*u + id*ic)/w; it = (id*u - x*ic)/w;
      for (i = 1; i < kpkp; i++)
      {  if (i == k + p) continue;
         if (!K && i == kpkp - 1) { x = 1.0L; y = 0.0L; } 
         else
         {  u = X[i + 1 + offset]; v = Y[i + 1 + offset];
            x = u - rc; y = v - ic; u -= rd; v -= id;
            w = u*u + v*v; if (w < 1.0e-80L) return 2;
            u /= w; v /= w; w = x*u + y*v; y = y*u - x*v; x = w;
         }
         root(rt*x - it*y, rt*y + it*x, x, y);
         if (x*X[i + oldset] + y*Y[i + oldset] < 0.0L) { x = -x; y = -y; }
         X[i + offset] = x; Y[i + offset] = y;
      }
      if (k) //else stays 0
      { X[k+p + offset] = -X[k + offset]; Y[k+p + offset] = -Y[k + offset]; }
      if (K) //else stays unused 0
      { X[kpkp+offset] = -X[k+p+K+offset]; Y[kpkp+offset] = -Y[k+p+K+offset]; }
      oldset = offset; offset -= kpkp;
   }
   rC = X[1]; iC = Y[1]; rD = X[k + p + 1]; iD = Y[k + p + 1]; return 0;
}

//////////////////////////////////////////////////////////////////////////

int mandelPathTwo::capture(int k0, int p0, mandelPathinfo *mpi)
{  type = 1; k = k0; p = p0; rp = -mpi->rc[0]; ip = -mpi->ic[0];
   int i, m; mdouble x, y, u, v, w;
   while (mpi->n < M) //interpolate
   {  mdouble *rz = new mdouble[2*mpi->n + 1], *iz = new mdouble[2*mpi->n + 1];
      for (m = 0; m <= mpi->n; m++)
      { rz[2*m] = mpi->rc[m]; iz[2*m] = mpi->ic[m]; }
      for (m = 1; m <= mpi->n; m++)
      {  rz[2*m - 1] = 0.5L*(mpi->rc[m - 1] + mpi->rc[m]);
         iz[2*m - 1] = 0.5L*(mpi->ic[m - 1] + mpi->ic[m]);
      }
      mpi->n *= 2;
      delete[] mpi->rc; delete mpi->ic; mpi->rc = rz; mpi->ic = iz;
   }
   delete[] X; delete[] Y;
   X = new mdouble[1 + M*(k+p)]; Y = new mdouble[1 + M*(k+p)];
   for (m = M - 1; m >= 0; m--) //copy first path
   {  X[1 + m*(k+p)] = mpi->rc[mpi->n - (mpi->n*m)/M];
      Y[1 + m*(k+p)] = mpi->ic[mpi->n - (mpi->n*m)/M];
   }
   x = X[1]; y = Y[1];
   for (i = 2; i < k+p; i++) //endpoints
   {  u = rp + 1.0L, v = ip, w = x*x - y*y - 1.0L; y = 2.0L*x*y; x = w;
      w = x*x + y*y; if (w<1.0e-100L) { type = 0; return -1; } x /= w; y /= w;
      w = u*x + v*y; y = v*x - u*y; x = w + 1.0L; X[i] = x; Y[i] = y;
   }
   if (k)
   {  if (p) { X[k+p] = -X[k]; Y[k+p] = -Y[k]; }
      else { X[k] = -1.0L; Y[k] = 0.0L; } //and iterate backwards !!!!!!!!!!!
   }
   else { X[p] = 0.0L; Y[p] = 0.0L; } //and iterate backwards !!!!!!!!!!!
   for (m = M - 1; m > 0; m--) for (i = 2; i <= k+p; i++) //stay
   { X[i + m*(k+p)] = X[i]; Y[i + m*(k+p)] = Y[i]; }
   if (draw)
   {  delete[] X0; delete[] Y0;
      X0 = new mdouble[1 + M*(k+p)]; Y0 = new mdouble[1 + M*(k+p)];
      for (i = 1; i <= M*(k+p); i++) { X0[i] = X[i]; Y0[i] = Y[i]; }
   }
   return 0;
}

/*int mandelPathTwo::spider(qulonglong N, qulonglong D, mdouble logR1, int)
{  type = 2;
   return 0;
}//*/

int mandelPathTwo::mating(int k0, int p0, mdouble rc, mdouble ic,
   mdouble logR1, int)
{  type = 2; k = k0; p = p0; rp = rc; ip = ic; twoLogR = 2.0*logR1;
   int i, m; mdouble x = rp, y = ip, u, v, w, RS, omt;
   delete[] X; delete[] Y;
   X = new mdouble[1 + M*(k+p)]; Y = new mdouble[1 + M*(k+p)];
   X[1] = x; Y[1] = y;
   for (i = 2; i < k+p; i++)
   {  w = x*x - y*y + rp; y = 2.0L*x*y + ip; x = w; X[i] = x; Y[i] = y;
      if (x*x + y*y > 4.0L) { type = 0; return -1; }
   }
   if (k) { X[k+p] = -X[k]; Y[k+p] = -Y[k];} else { X[p] = 0.0L; Y[p] = 0.0L; }
   for (m = M - 1; m >= 0; m--)
   {  omt = ((mdouble)(m))/((mdouble)(M)); RS = exp(twoLogR*exp(logTwo*omt));
      for (i = 1; i <= k+p; i++)
      {  u = omt*(X[i] - rp) - RS; v = omt*(ip - Y[i]);
         w = u*u + v*v; u /= w; v /= w;
         w = u*X[i] - v*Y[i]; Y[i+m*(k+p)] = u*Y[i] + v*X[i]; X[i+m*(k+p)] = w;
      }
   }
   if (draw)
   {  delete[] X0; delete[] Y0;
      X0 = new mdouble[1 + M*(k+p)]; Y0 = new mdouble[1 + M*(k+p)];
      for (i = 1; i <= M*(k+p); i++) { X0[i] = X[i]; Y0[i] = Y[i]; }
   }
   return 0;
}

int mandelPathTwo::anti(int k0, int p0, mdouble rQ, mdouble iQ,
   mdouble logR1, int)
{  type = 3; k = k0; p = p0; rp = rQ; ip = iQ; twoLogR = 2.0L*logR1;
   int i, m; mdouble x = -rp, y = -ip, u, v, w, RS,
      omt = rp*rp + ip*ip, ri = rp/omt, ii = -ip/omt;
   delete[] X; delete[] Y;
   X = new mdouble[1 + M*(k+p)]; Y = new mdouble[1 + M*(k+p)];
   X[1] = x; Y[1] = y;
   for (i = 2; i < k+p; i++)
   {  u = x*x - y*y; v = 2.0L*x*y;
      if (i & 1) { u--; x = rp*u - ip*v; y = rp*v + ip*u; }
      else { x = ri*u - ii*v + 1.0L; y = ri*v + ii*u; }
      X[i] = x; Y[i] = y;
      if (x*x + y*y > 16.0L) { type = 0; return -1; }
      if (x*x + y*y < 0.1L && (i & 1) ) { k = i - 1; p = 0; }
   }
   if (k)
   {  if (p) { X[k+p] = -X[k]; Y[k+p] = -Y[k]; }
      else { X[k] = -1.0L; Y[k] = 0.0L; }
   }
   else { X[p] = 0.0L; Y[p] = 0.0L; }
   for (m = M - 1; m >= 0; m--)
   {  omt = ((mdouble)(m))/((mdouble)(M)); RS = exp(twoLogR*exp(logTwo*omt));
      for (i = 1; i <= k+p; i += 2)
      {  x = X[i]; y = Y[i]; w = x*x + y*y;
         if (w < 1.0e-100L) { type = 0; return -1; }
         x /= w; y /= -w; u = RS + omt*rp; v = omt*ip;
         X[i+m*(k+p)] = u*x - v*y + omt; Y[i+m*(k+p)] = u*y + v*x;
      }
      omt /= RS; RS = omt*ip; omt *= rp; 
      for (i = 2; i <= k+p; i += 2)
      {  x = X[i]; y = Y[i];
         u = 1.0L + omt*(1.0L - x) + RS*y; v = RS*(1.0L - x) - omt*y;
         w = u*u + v*v; if (w < 1.0e-100L) { type = 0; return -1; }
         u /= w; v /= -w;
         X[i+m*(k+p)] = u*x - v*y; Y[i+m*(k+p)] = u*y + v*x;
      }
   }
   if (draw)
   {  delete[] X0; delete[] Y0;
      X0 = new mdouble[1 + M*(k+p)]; Y0 = new mdouble[1 + M*(k+p)];
      for (i = 1; i <= M*(k+p); i++) { X0[i] = X[i]; Y0[i] = Y[i]; }
   }
   return 0;
}

int mandelPathTwo::step(mdouble &rc, mdouble &ic)
{  if (!type) return -1;
   int i, oldset = 0, offset = (M - 1)*(k+p); mdouble u, v, w, x, y;
   while (offset >= 0)
   {  u = X[1 + offset] - 1.0L; v = Y[1 + offset];
      for (i = 1; i < k+p; i++)
      {  x = 1.0L - X[i + 1 + offset]; y = Y[i + 1 + offset];
         w = x*x + y*y; if (w < 1.0e-20L) return 1;
         x /= w; y /= w; root(1.0L + u*x - v*y, u*y + v*x, x, y);
         if (x*X[i + oldset] + y*Y[i + oldset] < 0.0L) { x = -x; y = -y; }
         X[i + offset] = x; Y[i + offset] = y;
      }
      if (k) //else z_p stays 0
      {  if (p)
         { X[k+p + offset] = -X[k + offset]; Y[k+p + offset] = -Y[k + offset];}
         else //needed for antimating,  and for capture path when  k = 1
         { X[k+p + offset] = -1.0L; Y[k+p + offset] = 0.0L; }
      }
      oldset = offset; offset -= k+p;
   }
   rc = -X[1]; ic = -Y[1]; return 0;
}

int mandelPathTwo::sequence(int n, int f, int fps, mandelPathinfo *mpi)
{  if (!type || !draw) return -1;
   int i, m = (fps - f)*M/fps; mdouble x, y;
   delete[] mpi->coeff; delete[] mpi->rc; delete[] mpi->ic;
   if (type == 1) mpi->coeff = 0;
   else
   {  mpi->coeff = new mdouble[4];
      mdouble omt = ((mdouble)(m))/((mdouble)(M)),
         RS = exp(twoLogR*exp(logTwo*omt));
      mpi->coeff[0] = RS;
      if (type == 2)
      { mpi->coeff[1] = -(RS + omt*rp); mpi->coeff[2] = -omt*ip; }
      mpi->coeff[3] = omt;
   }
   mpi->rc = new mdouble[n + 1]; mpi->ic = new mdouble[n + 1]; mpi->n = n;
   mpi->rc[0] = rp; mpi->ic[0] = ip;
   for (i = 1; i <= M*(k+p); i++) { X[i] = X0[i]; Y[i] = Y0[i]; }
   mpi->rc[1] = -X[1+m*(k+p)]; mpi->ic[1] = -Y[1+m*(k+p)];
   for (i = 2; i <= n; i++)
   {  if (step(x, y)) return 1;
      mpi->rc[i] = -X[1+m*(k+p)]; mpi->ic[i] = -Y[1+m*(k+p)];
   }
   return 0;
}
//////////////////////////////////////////////////////////////////////////
/*   mandelPathThree
q2   -1.754877666246693  +0.000000000000000
q3   -0.122561166876654  +0.744861766619744
q4   -0.122561166876654  -0.744861766619744
c2   1.324717957244746  +0.000000000000000
c3   -0.662358978622373  +0.562279512062301
c4   -0.662358978622373  -0.562279512062301
*/

//////////////////////////////////////////////////////////////////////////

int mandelbrotPath::capture(int k0, int p0, mandelPathinfo *mpi)
{  steps = 0; k = k0; p = p0; mdouble rp = mpi->rc[0], ip = mpi->ic[0];
   int i, m; mdouble x, y, w;
   while (mpi->n < M) //interpolation
   {  mdouble *rz = new mdouble[2*mpi->n + 1], *iz = new mdouble[2*mpi->n + 1];
      for (m = 0; m <= mpi->n; m++)
      { rz[2*m] = mpi->rc[m]; iz[2*m] = mpi->ic[m]; }
      for (m = 1; m <= mpi->n; m++)
      {  rz[2*m - 1] = 0.5L*(mpi->rc[m - 1] + mpi->rc[m]);
         iz[2*m - 1] = 0.5L*(mpi->ic[m - 1] + mpi->ic[m]);
      }
      mpi->n *= 2;
      delete[] mpi->rc; delete mpi->ic; mpi->rc = rz; mpi->ic = iz;
   }
   delete[] X; delete[] Y;
   X = new mdouble[1 + M*(k+p)]; Y = new mdouble[1 + M*(k+p)];
   for (m = M - 1; m >= 0; m--) //copy first path
   {  X[1 + m*(k+p)] = mpi->rc[mpi->n - (mpi->n*m)/M];
      Y[1 + m*(k+p)] = mpi->ic[mpi->n - (mpi->n*m)/M];
   }
   x = X[1]; y = Y[1];
   for (i = 2; i < k+p; i++) //endpoints
   {  w = x*x - y*y + rp; y = 2.0L*x*y + ip; x = w;
      if (x*x + y*y > 4.0L) { steps = -1; return -1; }
      X[i] = x; Y[i] = y;
   }
   if (k) { X[k+p] = -X[k]; Y[k+p] = -Y[k]; }
   else { X[p] = 0.0L; Y[p] = 0.0L; } //and iterate backwards!!!!!!!!
   for (m = M - 1; m > 0; m--) for (i = 2; i <= k+p; i++) //stay
   { X[i + m*(k+p)] = X[i]; Y[i + m*(k+p)] = Y[i]; }
   return 0;
}

int mandelbrotPath::spider(qulonglong N, qulonglong D, mdouble logR, int)
{  p = mndAngle::normalize(N, D, k); if (!p) { steps = -1; return -1; }
   if (logR <= 0.0L) { steps = 0; Mmax = M - 1; }
   else { steps = 5; Ms = M + 1; Mmax = Ms + 350; } // 5 divides 350
   delete[] X; delete[] Y;
   X = new mdouble[1 + (Mmax+1)*(k+p)]; Y = new mdouble[1 + (Mmax+1)*(k+p)];
   int i, offset; mdouble u, v, w;
   for (i = 1; i <= k+p; i++)
   {  u = mndAngle::radians(N, D); mndAngle::twice(N, D);
      X[i] = cos(u); Y[i] = sin(u); //cout << endl << u/(2.0L*PI);
   }
   if (steps) //spider with legs
   {  v = logTwo/steps; logR *= exp(-Ms*v); v /= k+p;
      for (offset = Ms*(k+p); offset > 0; offset -= k+p)
      {  u = exp(logR*exp(offset*v));
         for (i = 1; i <= k+p; i++)
         {  w = u; if (i == p && !k) w--;
            X[i + offset] = w*X[i]; Y[i + offset] = w*Y[i];
         }
      }
      if (!k) { X[p] = 0.0L; Y[p] = 0.0L; }
      for (i = 1; i <= k+p; i++)
      { X[i+Mmax*(k+p)] = X[i+Ms*(k+p)]; Y[i+Mmax*(k+p)] = Y[i+Ms*(k+p)]; }
      return 0;
   }
   v = 1.0L/((k+p)*M); //spider with path
   for (offset = (M - 1)*(k+p); offset > 0; offset -= k+p)
   {  u = 1.0L - offset*v;
      X[1 + offset] = u*X[1]; Y[1 + offset] = u*Y[1];
      for (i = 2; i <= k+p; i++)
      { X[i + offset] = X[i]; Y[i + offset] = Y[i]; }
      if (!k) { u = 1.0L - u; X[p + offset] *= u; Y[p + offset] *= u; }
   }
   return 0;
}

int mandelbrotPath::step(mdouble &rc, mdouble &ic)
{  if (steps < 0) return -1; if (steps) return gait(rc, ic); //with legs
   int i, j, oldset = 0, offset = (M - 1)*(k+p), W = 0;
   mdouble u, v, x, y;
   while (offset >= 0) //pullback
   {  u = X[1 + offset]; v = Y[1 + offset];
      for (i = 1; i < k+p; i++)
      {  root(X[i + 1 + offset] - u, Y[i + 1 + offset] - v, x, y);
         if (x*X[i + oldset] + y*Y[i + oldset] < 0.0L) { x = -x; y = -y; }
         X[i + offset] = x; Y[i + offset] = y;
      }
      if (k)
      { X[k+p + offset] = -X[k + offset]; Y[k+p + offset] = -Y[k + offset]; }
      else { X[p + offset] = 0.0L; Y[p + offset] = 0.0L; } //only first step
      for (i = 1; i < k+p; i++) //check
      {  if (W) break;
         for (j = i + 1; j <= k+p; j++) if (checkp(
            X[i + offset], Y[i + offset], X[i + oldset], Y[i + oldset],
            X[j + offset], Y[j + offset], X[j + oldset], Y[j + oldset]) > 0)
         { W = 1; break; }
      }
      oldset = offset; offset -= k+p;
   }
   rc = X[1]; ic = Y[1]; return W;
}

/*
Note that although both the spider and capture with a path,  and the spider
with legs,  check for homotopy violations when a hyperbola segment is replaced
with a line segment,  there are other sources of inaccuracy:  one is related to
very small segments and narrow triangles.  More importantly,  there may be
numerical loss of precision by subtraction:  E.g.,  for the satellite component
with internal angle  7/20  and external angle  299593 1048575 , both spider
algorithms give the same divergent sequence.  The final points are at least
0.05  apart and the initial points  0.000005 .
*/

int mandelbrotPath::gait(mdouble &rc, mdouble &ic)
{  int i, j, offset = Ms*(k+p), oldset = Mmax*(k+p), kp = k+p; if (k) kp--;
   mdouble u = X[1], v = Y[1], x, y, x0, y0, x1, y1;
   while (offset >= 0) //pullback
   {  x1 = X[1 + offset]; y1 = Y[1 + offset];
      for (i = 1; i <= kp; i++)
      {  x0 = X[i + oldset]; y0 = Y[i + oldset];
         if (i == p && !k) { x = x1; y = y1; }
         else { x = X[i + 1 + offset]; y = Y[i + 1 + offset]; }
         root(x - u, y - v, x, y);
         if (x*x0 + y*y0 < 0.0L) { x = -x; y = -y; }
         X[i + offset] = x; Y[i + offset] = y;
      }
      if (k)
      { X[k+p + offset] = -X[k + offset]; Y[k+p + offset] = -Y[k + offset]; }
      oldset = offset; offset -= k+p;
   }
   rc = X[1]; ic = Y[1]; kp = 0; offset = 0; oldset = k+p;
   while (offset < Ms*(k+p)) //check
   {  if (X[1 + offset]*X[1 + offset]+Y[1 + offset]*Y[1 + offset] >9.0L) break;
      for (i = 1; i <= k+p; i++)
      {  for (j = 1; j <= k+p; j++)
         {  //if (j == i) continue;
            if (checks(X[i + offset], Y[i + offset], X[i + oldset],
               Y[i + oldset], X[j], Y[j]) > 0)
            { kp = i + offset; break; }
         }
         if (kp) break;
      }
      offset = oldset; oldset += k+p; if (kp) break;
   }
   if (Ms < Mmax) //extend
   {  offset = Ms*(k+p); oldset = Mmax*(k+p);
      for (i = 1; i <= k+p; i++)
      {  u = (X[i + oldset] - X[i + offset])/steps;
         v = (Y[i + oldset] - Y[i + offset])/steps;
         for (j = 1; j <= steps; j++)
         { X[i + offset + j*(k+p)] += j*u; Y[i + offset + j*(k+p)] += j*v; }
      }
      Ms += steps; 
   }
   return kp;
}

int mandelbrotPath::checks(mdouble ru, mdouble iu, mdouble rv, mdouble iv,
   mdouble rw, mdouble iw) //spider with legs
{  mdouble s = 1.0L, t, z = ru*iv - iu*rv; if (z < 0.0L) { s = -1.0L; z = -z; }
   if (z < (ru*ru + iu*iu + rv*rv + iv*iv)*1.0e-15L) return -1;
   rv -= ru; iv -= iu; rw -= ru; iw -= iu; t = s*(rw*iv - iw*rv);
   if (t > 0.0L || t < -z) return 0;
   t = rw*rw - iw*iw + 2.0L*(rw*ru - iw*iu); iw = 2.0L*(rw*iw + rw*iu + iw*ru);
   rw = t;
   t = rv*rv - iv*iv + 2.0L*(rv*ru - iv*iu); iv = 2.0L*(rv*iv + rv*iu + iv*ru);
   rv = t; if (s*(rw*iv - iw*rv) < 0.0L) return 0;
   return 1;
}

/*
Both kinds of check are unsatisfactory for two reasons:  first there is a
somewhat arbitrary bound to avoid warnings caused by nemerical cancellations.
Second,  the code has not been tested extensively due to a limited number of
examples: the spider with legs warns for  1048577 3145728  when  M = 2 .
*/

int mandelbrotPath::checkp(mdouble ra, mdouble ia, mdouble ru, mdouble iu,
   mdouble rc, mdouble ic, mdouble rv, mdouble iv) //spider / capture with path
{  if ( (ra+ru)*(rc+rv) + (ia+iu)*(ic+iv) <= 0.0L) return 0;
   //given four points a, b, c, d now u = b - a, v = d - c
   ru -= ra; iu -= ia; rv -= rc; iv -= ic; mdouble x, y, w;
   x = ru*ru - iu*iu - rv*rv + iv*iv; y = 2.0L*(ru*iu - rv*iv); //u^2 - v^2
   w = 2.0L*(ra*ru - ia*iu - rc*rv + ic*iv); //u = 2(au - cv)
   iu = 2.0L*(ra*iu + ia*ru - rc*iv - ic*rv); ru = w;
   w = rc*rc - ic*ic - ra*ra + ia*ia; ia = 2.0L*(rc*ic - ra*ia);
   ra = w; //c^2 - a^2
   rv = x; iv = y; //now solve  ux + vy = a,  warn 0 < x < 1 && x^2 <= y <= x
   w = ru*iv - iu*rv; y = (w > 0.0L ? w : -w);
   x = ru*ru + iu*iu + rv*rv + iv*iv; if (x < 1.0e-100L) return -1;
   if (y < x*1.0e-15L) return -1;
   //mdouble e = y/x;
   x = (ra*iv - ia*rv)/w; if (x <= 0.0L || x >= 1.0L) return 0;
   y = (ru*ia - iu*ra)/w; if (y > x || y < x*x) return 0;
   //cout << endl<<e<<endl;
   return 1;
}

////////////////////////////////////////////////////////////////////////

//The initial legs are horizontal,  the first one from  R  to  10*R
//and the other ones from  2*R  to  10*R ; the preimages start at  ~logR .
int mandelPathExpo::spider(qulonglong N, qulonglong D, mdouble R, int)
{  k = (int)(D >> 16); p = (int)(D & 65535ULL);
   if (!k || !p || k + p > 32) { steps = -1; return -1; }
   if (R <= 0.0L)
   { steps = 0; Mmax = M - 1; if (R <= -10.0L) R = -R; else R = 100.0L; }
   else { steps = 10; Ms = M + 1; Mmax = Ms + 500; if (R < 10.0L) R = 100.0L; }
   delete[] X; delete[] Y;
   X = new mdouble[1 + (Mmax+1)*(k+p)]; Y = new mdouble[1 + (Mmax+1)*(k+p)];
   int i, offset = 4; mdouble u = 2.0L*PI, v = 128.0L/15.0L, w, y[k+p+1];
   if (k+p > 21) { offset = 2; v = 8.0L/3.0L; }
   else if (k+p > 16) { offset = 3; v = 32.0L/7.0L; } 
   D = 1ULL << offset*(k+p - 1); w = (mdouble)(D); D--; D <<= offset;
   for (i = 1; i <= k+p; i++)
   {  y[i] = u*(((mdouble)(N))/w - v);
      N = ((N << offset) & D);
      N |= (N >> offset*p) & ((1ULL << offset) - 1ULL);
   }
   //To encode the external address in a single qulonglong, it is assumed
   //that  k+p <= 16  and each entry between  -7  and  7 ; each  +8  has 4 bits
   //Or up to 21 entries with 3 bits,  or 32 entries with 2 bits.
   //When the bound on the entries should be increased,  increase  R  as well.
   if (steps) //spider with legs
   {  u = R/(Ms*(k+p)); v = 9.0L*u; u *= 8.0L;
      for (offset = Ms*(k+p); offset >= 0; offset -= k+p)
      {  X[1 + offset] = R + v*offset; Y[1 + offset] = y[1];
         for (i = 2; i <= k+p; i++)
         { X[i + offset] = 2.0L*R + u*offset; Y[i + offset] = y[i]; }
      }
      for (i = 1; i <= k+p; i++)
      { X[i + Mmax*(k+p)] = 10.0L*R; Y[i + Mmax*(k+p)] = y[i]; }
      return 0;
   }
   for (i = 1; i <= k+p; i++) //spider with path
   {  if (i < k+p) v = y[i + 1]; else v = y[k + 1];
      v -= y[1]; X[i] = 0.5L*log(R*R + v*v); v = atan2(v, R);
      u = (y[i] - v)*(0.5L/PI); w = floor(u);
      if (u > w + 0.5L) w++; Y[i] = v + 2.0L*PI*w;
   }
   u = R/((M/2)*(k+p));
   for (offset = (M - 1)*(k+p); offset >= (M/2)*(k+p); offset -= k+p)
      for (i = 2; i <= k+p; i++)
      { X[i + offset] = u*offset; Y[i + offset] = y[i]; }
   for (i = 2; i <= k+p; i++)
   {  u = (R - X[i])/((M/2)*(k+p)); v = (y[i] - Y[i])/((M/2)*(k+p));
      for (offset = (M/2 - 1)*(k+p); offset > 0; offset -= k+p)
      { X[i + offset] = X[i] + u*offset; Y[i + offset] = Y[i] + v*offset; }
   }
   u = (R - X[1])/(M*(k+p)); v = (y[1] - Y[1])/(M*(k+p));
   for (offset = (M - 1)*(k+p); offset > 0; offset -= k+p)
   { X[1 + offset] = X[1] + u*offset; Y[1 + offset] = Y[1] + v*offset; }
   return 0;
}

int mandelPathExpo::step(mdouble &rc, mdouble &ic)
{  if (steps < 0) return -1; if (steps) return gait(rc, ic); //with legs
   int i, oldset = 0, offset = (M - 1)*(k+p);
   mdouble u, v, w, x, y;
   while (offset >= 0)
   {  u = X[1 + offset]; v = Y[1 + offset];
      for (i = 1; i <= k+p; i++)
      {  if (i < k+p)
         {  x = X[i + 1 + offset] - u; y = Y[i + 1 + offset] - v;
            X[i + offset] = 0.5L*log(x*x + y*y); y = atan2(y, x);
         }
         else { X[i + offset] = X[k + offset]; y = Y[k + offset]; }
         x = (Y[i + oldset] - y)*(0.5L/PI); w = floor(x);
         if (x > w + 0.5L) w++; Y[i + offset] = y + 2.0L*PI*w;
      }
      oldset = offset; offset -= k+p;
   }
   w = exp(X[1]); rc = w*cos(Y[1]); ic = w*sin(Y[1]); return 0;
}

int mandelPathExpo::gait(mdouble &rc, mdouble &ic)
{  int i, j, kp = k + p, offset = Ms*kp, oldset = Mmax*kp, W = 0;
   mdouble u = X[1], v = Y[1], x, y, y0, w;
   while (offset >= 0) //pullback
   {  for (i = 1; i < kp; i++)
      {  y0 = Y[i + oldset];
         x = X[i + 1 + offset] - u; y = Y[i + 1 + offset] - v;
         X[i + offset] = 0.5L*log(x*x + y*y); y = atan2(y, x);
         x = (y0 - y)*(0.5L/PI); w = floor(x);
         if (x > w + 0.5L) w++; Y[i + offset] = y + 2.0L*PI*w;
      }
      X[kp + offset] = X[k + offset]; y = Y[k + offset];
      x = (Y[kp + oldset] - y)*(0.5L/PI); w = floor(x);
      if (x > w + 0.5L) w++; Y[kp + offset] = y + 2.0L*PI*w;
      oldset = offset; offset -= kp;
   }
   w = exp(X[1]); rc = w*cos(Y[1]); ic = w*sin(Y[1]); offset = 0; oldset = kp;
   while (offset < Ms*(kp)) //check
   {  if (X[1 + offset] > 100.0L) break;
      for (i = 1; i <= kp; i++)
      {  for (j = 1; j <= kp; j++)
         {  //if (j == i) continue;
            if (checks(X[i + offset], Y[i + offset], X[i + oldset],
               Y[i + oldset], X[j], Y[j]) > 0)
            { W = i + offset; break; }
         }
         if (W) break;
      }
      offset = oldset; oldset += kp; if (W) break;
   }
   if (Ms < Mmax) //extend
   {  offset = Ms*kp; oldset = Mmax*kp;
      for (i = 1; i <= kp; i++)
      {  u = (X[i + oldset] - X[i + offset])/steps;
         v = (Y[i + oldset] - Y[i + offset])/steps;
         for (j = 1; j <= steps; j++)
         { X[i + offset + j*kp] += j*u; Y[i + offset + j*kp] += j*v; }
      }
      Ms += steps; 
   }
   return W;
}

int mandelPathExpo::checks(mdouble ru, mdouble iu, mdouble rv, mdouble iv,
   mdouble rw, mdouble iw) //spider with legs
{  mdouble e, s = 1.0L, t, z = 0.5*(iu + iv) - iw;
   if (z >= PI || z <= -PI) return 0;
   z = (rw-ru)*(iv-iu) - (iw-iu)*(rv-ru); if (z < 0.0L) { z = -z; s = -s; }
   e = z/((rw-ru)*(rw-ru)+(iw-iu)*(iw-iu)+(rv-ru)*(rv-ru)+(iv-iu)*(iv-iu));
   if (e < 1.0e-5L) return -1;
   z = exp(rw-ru); t = exp(rv-ru); ru = cos(iu); iu = sin(iu); 
   rw = z*cos(iw) - ru; iw = z*sin(iw) - iu; 
   rv = t*cos(iv) - ru; iv = t*sin(iv) - iu; 
   if (s*(rw*iv - iw*rv) > 0.0L) return 0; //cout << e << endl;
   return 1;
}

//maybe prune initial segments instead of pulling in.

//draw spider in which parametrization?  After ^return in yellow?

////////////////////////////////////////////////////////////////////////

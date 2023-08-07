/* mndlbrot.cpp  by Wolf Jung (C) 2007-2023.  Defines classes:
   mndlbrot, mndmulti, mndbfpoly, mndcubic, mndquartic, mndsurge,
   mndrealcubic, mndtricorn, mndhenon, mndifs, mndlambda, mndScale.

   These classes are part of Mandel 5.18, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/

#include <cmath>
#include <complex>
#include "mndynamo.h"
using namespace std;
typedef  complex<mdouble>  mndplex;
inline mndplex operator*(const mdouble &r, const mndplex &z)
{ return mndplex(r) * z; }
inline mndplex operator+(const mndplex &z, const mdouble &r)
{ return z + mndplex(r); }
inline mndplex operator-(const mndplex &z, const mdouble &r)
{ return z - mndplex(r); }

void mndlbrot::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{ mdouble u = x*x - y*y + a; y = 2*x*y + b; x = u; }//*/

void mndlbrot::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0 || mode < -2) return;
   if (A*A + B*B < 1e-20) //inverse with principal value
   {  root(x - A, y - B, x, y);
      if (mode & 1) { x = -x; y = -y; }
      return;
   }
   mdouble u, v, w = A*A + B*B; //inverse with argument adjusted
   root(-A/w, -B/w, u, v); root(w - A*x - B*y, B*x - A*y, x, y);
   w = u*x - v*y; y = u*y + v*x; x = w;
   if (mode & 1) { x = -x; y = -y; }
}

void mndlbrot::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg > 0) { xmid = -0.5; rewidth = 1.6; }
   else { xmid = 0; rewidth = 2.0; }
}

int mndlbrot::find(int sg, uint preper, uint per, mdouble &x, mdouble &y) const
{  mdouble a = A, b = B, fx, fy, px, py, w; uint i, j;
   for (i = 0; i < 30; i++)
   {  if (sg > 0) { a = x; b = y; }
      if (!preper)
      {  if (sg > 0) { fx = 0; fy = 0; px = 0; py = 0; }
         else { fx = -x; fy = -y; px = -1; py = 0; }
      }
      else
      {  fx = x; fy = y; px = 1.0; py = 0;
         for (j = 1; j < preper; j++)
         {  if (px*px + py*py > 1e100) return 1;
            w = 2*(fx*px - fy*py); py = 2*(fx*py + fy*px);
            px = w; if (sg > 0) px++;
            w = fx*fx - fy*fy + a; fy = 2*fx*fy + b; fx = w;
         }
      }
      mdouble Fx = fx, Fy = fy, Px = px, Py = py;
      for (j = 0; j < per; j++)
      {  if (px*px + py*py > 1e100) return 2;
         w = 2*(fx*px - fy*py); py = 2*(fx*py + fy*px);
         px = w; if (sg > 0) px++;
         w = fx*fx - fy*fy + a; fy = 2*fx*fy + b; fx = w;
      }
      fx += Fx; fy += Fy; px += Px; py += Py;
      w = px*px + py*py; if (w < 1e-100) return -1;
      x -= (fx*px + fy*py)/w; y += (fx*py - fy*px)/w;
   }
   return 0;
}

int mndlbrot::bifurcate(mdouble t, mdouble &a, mdouble &b) const
{  int per = 1; if (a < -0.75L) per = 2;
   if (a < -1.5L || b > sqrt(27.0L/64.0L) || b < -sqrt(27.0L/64.0L) ) per = 3;
   if (t <= -1.0L) return per;
   t *= (2.0L*PI);
   if (per == 1)
   { a = 0.5L*cos(t) - 0.25L*cos(2.0L*t); b = 0.5L*sin(t) - 0.25L*sin(2.0L*t); }
   if (per == 2) { a = 0.25L*cos(t) - 1.0L; b = 0.25L*sin(t); }
   if (per <= 2) return per;
   mndplex u, c, c1, l = mndplex(cos(t), sin(t));
   if (a < -1.54L) c1 = -1.754877666L;
   else
   { c1 = mndplex(-.122561167L, .744861767L); if (b < 0.0L) c1 = conj(c1); }
   c = 81.0L*l*l-528.0L*l+4416.0; root(real(c), imag(c), a, b);
   u = pow(-13.5*l*l+144.0*l-800.0L + (-1.5L*l+12.0L)*mndplex(a, b),1.0L/3.0L);
   c = (1.0L/3.0L)*(.25L*u + (1.5L*l+4.0L)/u - 2.0L);
   if (norm(c - c1) > .25L)
   {  u *= mndplex(-0.5L, sqrt(0.75L));
      c = (1.0L/3.0L)*(.25L*u + (1.5L*l+4.0L)/u - 2.0L);
   }
   if (norm(c - c1) > .25L)
   {  u *= mndplex(-0.5L, sqrt(0.75L));
      c = (1.0L/3.0L)*(.25L*u + (1.5L*l+4.0L)/u - 2.0L);
   }
   a = real(c); b = imag(c); return 3;
} //bifurcate

void mndlbrot::prepare(int sg, uint nmax, uint &dm, mdouble *t)
{  if (dm == 201) //exray
   {  sign = sg; drawmode = 0; temp[0] = *t;
      for (int j = 1; j <= 63; j++) temp[j] = 0;
      return;
   }
   if (dm == 202) //activate comparison in turnsign()
   { drawmode = 1; return; }
   mndynamics::prepare(sg, nmax, dm, t);
   if (drawmode > 255)
   {  if (!similarity(65003u, (drawmode >> 8), t[1], t[2], temp)
          && temp[0] > 40.0)
       temp[0] = 10.0/temp[0]; else temp[0] = t[0];
   }
   if (drawmode > 2 && drawmode <= 11) temp[1] = *t;
   //{  mdouble u = 0.001*maxiter; maxiter = 500; temp[1] = (*t)*u*u; }
   if (drawmode == 6)
   { *temp = 2.5/sqrt(*t); if (*temp < 600.0) *temp = 600.0; }
   /*/Thurston eigenvalues at real centers
   if (sign < 0)
   {  mdouble X = A, Y = B; Period = period(X, Y, 1);
      if (Period < 3 || Period > 63) { subtype = 1000; return; }
      temp[2] = 1.0L/A; mdouble U = A; uint J; subtype = Period;
      for (J = 3; J <= subtype; J++)
      { U = U*U + A; temp[J] = temp[J-1]/U; }
   }//*/
   /*/Thurston eigenvalues at real 41/96
   if (sign < 0)
   {  subtype = 6; A = -1.697555393237548L; mdouble U = A, t[6]; uint J;
      t[1] = 1.0L/A; for (J = 2; J <= 5; J++) { U = U*U + A; t[J] = 1.0L/U; }
      temp[2] = t[1]+t[5]; temp[3] = t[1]*(t[2] + t[5]);
      temp[4] = t[1]*t[2]*(t[3] + t[5]); temp[5] = t[1]*t[2]*t[3]*(t[4] + t[5]);
      temp[6] = t[1]*t[2]*t[3]*t[4]*t[5]*2.0L;
   }//*/
}

uint mndlbrot::parabolic(mdouble x, mdouble y)
{  uint j; mdouble u, v;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y;
      if (u + v <= bailout)
      { y = 2*x*y; x = u - v + A; }
      else return j;
      if (A > 0 && x >= 0 && x <= 0.5 && (y > 0 ? y : -y) <= 0.5 - x)
         return (y > 0 ? 65281u : 65289u);
      if (A < 0 && x >= -0.5 && x <= 0 && (y > 0 ? y : -y) <= 0.3 + 0.6*x)
      {  if (j & 1) return (y > 0 ? 65282u : 65290u);
         else return (y > 0 ? 65281u : 65289u);
      }
      /*if (x > -0.25 && y > 0 && x + y < 0.183012701892219)
      {  j %= 3; if (!j) return 65290;
            else if (j & 1) return 65291; else return 65289;} //c para 1/3 */
   }
   return 65293u;
}

uint mndlbrot::esctime(mdouble x, mdouble y)
{  uint j = 0;
   mdouble a, b, x0, y0, u, v;
   if (sign > 0) { a = x; b = y; } //z = c, not z = 0 !
   else { a = A; b = B; }
/*   x0 = -0.45; y0 = 0.86; mdouble r = 0.25, R = 0.75, aa, bb;
   aa = x - x0; bb = y - y0; u = aa*aa + bb*bb;
   if (r < u && u < R)
   {  u -= r; u *= 2.0*PI/(R - r); x = cos(u); y = sin(u);
      u = x0 + aa*x - bb*y; y = y0 + aa*y + bb*x; x = u;
   }
//Dehn*/
   if (b == 0.0 && !drawmode && sign < 0
      && (a == 0.25 || a == -0.75)) return parabolic(x, y);
   if (drawmode > 255)
   {  if (sign > 0) { x = 0; y = 0; }
      return renormtime(a, b, x, y);
   }
   if (sign > 0 && (x*x + y*y - .0625)*(x*x + y*y - .0625) <
        ((x - .25)*(x - .25) + y*y)*.25 ) j = 1;
   else if (sign > 0 && (x + 1)*(x + 1) + y*y < .0625 ) j = 2;
   if (sign > 0 && -1.76875 < x && x < -1.74375 && y*y < .00015625)
   {  root(-4*a - 7, -4*b, x0, y0);
      u = a + 2 + a*x0 - b*y0; v = b + a*y0 + b*x0;
      if (u*u + v*v < .0625) j = 3;
   }
   if (sign > 0 && ( (x + .125)*(x + .125)+(y - .75)*(y - .75) < .015625 ||
         (x + .125)*(x + .125) + (y + .75)*(y + .75) < .015625 ) )
   {  root(-4*a - 7, -4*b, x0, y0);
      u = a + 2 - a*x0 + b*y0; v = b - a*y0 - b*x0;
      if (u*u + v*v < .0625) j = 3;
   }
   if (j)
   { if (drawmode) return 65280u | (drawmode & 15); else return 65280 | j; }
   /*if (drawmode && x*x + y*y < 5 && y < 0)
   {  long long int U, V, W = 1LL << 28, Bailout = 5LL << 56;
      long int X = (long int)(x*W), Y = (long int)(y*W),
         A = (long int)(a*W), B = (long int)(b*W);
      for (j = 1; j <= maxiter; j++)
      {  U = X; U *= X; V = Y; V *= Y; if (U + V >= Bailout) return j;
         W = X; W *= Y; W >>= 27; Y = (long int)(W) + B;
         U -= V; U >>= 28; X = (long int)(U) + A;
      }
      return 65284;//0 | (drawmode & 15);
   }//test integer speed up ... problem with Green*/
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y;
      if (u + v <= bailout)
      { y = 2*x*y + b; x = u - v + a; }
      else return j;
      if (sign < 0 && !drawmode
         && (x - temp[0])*(x - temp[0]) + (y - temp[1])*(y - temp[1]) < 1e-6)
      {  int cl = 1 + j % Period;
         if (Period <= 4 && j % (2*Period) >= Period ) cl |= 8;
         return 65280u | cl;
      }
   }
   if (drawmode) return 65280u | (drawmode & 15);
   if (sign > 0)
   {  mdouble x0 = x, y0 = y;
      for (j = 0; j < 60; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y);
         else return maxiter + j;
         if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-6 )
           return 65281u + (j  % 12);
      }
   }
   return 65293u;
}

uint mndlbrot::pixcolor(mdouble x, mdouble y)
{  /*/Thurston eigenvalues at real centers
   if (sign < 0 && subtype <= 63)
   {  mdouble U = 1.0L, V = 0.0L, W; uint J;
      for (J = 2; J <= subtype; J++)
      { W = x*U - y*V; V = x*V  +y*U; U = W + temp[J]; }
      if (U > 0.0L) return (V > 0.0L ? 9 : 12);
      return (V > 0.0L ? 10 : 11);
   }//*/
   /*/IMG for dyadic rays of Basilica
   if (sign < 0)
   {  mdouble U; uint J;
      for (J = 1; J <= maxiter; J++)
      {  if (x*x + y*y >= 4.0L)
         { if (y >= 0.0L && x*x + y*y <= 6.0L + x) return 12; else return 9; }
         U = x*x - y*y; y *= 2.0L*x; x = U + A;
      }
      return 0;
   }//*/
   /*/test spider path preperiodic as pullback of disk
   mdouble h = 0.02L, u = 2.0L*PI*1.0L/4.0L, v, c = cos(u), s = sin(u);
   for (uint n = 1; n <= maxiter; n++)
   {  u = x*x; v = y*y; if (u + v > 100.0L) return 10;
      y *= 2.0L*x; x = u - v;
      u = c*x + s*y; v = c*y - s*x;
      if (v < h && v > -h && u > -h)
      {  u += (1.0L - v*v/(h*h))*(1.0L + u/h);
         x = c*u - s*v; y = c*v + s*u;
      }
   }
   if (x*x + y*y > 1.0L) return 10; return 1; //*/

   /*/test spider path basilica as pullback of disk
   uint n; mdouble u = 1.0L, v, aa[maxiter], bb[maxiter];
   aa[1] = -0.5L*u; bb[1] = sqrt(0.75L)*u;
   for (n = 2; n < maxiter; n++)
   { root(-aa[n-1], -bb[n-1], u, v); aa[n] = -u; bb[n] = -v; }
   for (n = maxiter - 1; n > 0; n--)
   {  u = x*x; v = y*y; if (u + v > 1000000.0L) return 10;
      y = 2.0L*x*y + bb[n]; x = u - v + aa[n];
   }
   if (x*x + y*y > 1.0L) return 10; return 1; //*/

   /*/test spider path airplane as pullback of disk
   uint n; mdouble u = 2.0L*PI*3.0L/7.0L, v = 2.0L*PI*6.0L/7.0L,
      aa[maxiter], bb[maxiter], aaa, bbb;
   aa[1] = cos(u); bb[1] = sin(u); aaa = cos(v); bbb = sin(v);
   for (n = 2; n < maxiter; n++)
   {  root(aaa - aa[n-1], bbb - bb[n-1], u, v);
      root(-aa[n-1], -bb[n-1], aaa, bbb);
      aa[n] = -u; bb[n] = -v;
   }
   for (n = maxiter - 1; n > 0; n--)
   {  u = x*x; v = y*y; if (u + v > 1000000.0L) return 10;
      y = 2.0L*x*y + bb[n]; x = u - v + aa[n];
   }
   if (x*x + y*y > 1.0L) return 10; return 1; //*/

   /*/test spider path rabbit as pullback of disk
   uint n; mdouble u = 2.0L*PI*1.0L/7.0L, v = 2.0L*PI*2.0L/7.0L,
   //uint n; mdouble u = 2.0L*PI*2.0L/7.0L, v = 2.0L*PI*4.0L/7.0L,
      aa[maxiter], bb[maxiter], aaa, bbb;
   aa[1] = cos(u); bb[1] = sin(u); aaa = cos(v); bbb = sin(v);
   for (n = 2; n < maxiter; n++)
   {  root(aaa - aa[n-1], bbb - bb[n-1], u, v);
      root(-aa[n-1], -bb[n-1], aaa, bbb); aaa = -aaa; bbb = -bbb;
      if (v > 0) { aa[n] = u; bb[n] = v; } else { aa[n] = -u; bb[n] = -v; }
   }
   for (n = maxiter - 1; n > 0; n--)
   {  u = x*x; v = y*y; if (u + v > 1000000.0L) return 10;
      y = 2.0L*x*y + bb[n]; x = u - v + aa[n];
   }
   if (x*x + y*y > 1.0L) return 10; return 1; //*/

   /*/JPT use drawmode 9, note possible problem with redefining AB
   if (sign > 0)
   {  sign = -1; A = x; B = y;
      //uint cl = (pixcolor(2+x, y) + pixcolor(-2+x, y)) & 15;
      //uint cl = (pixcolor(1+x, y) + pixcolor(-1+x, y)) & 15;
      //uint cl = (pixcolor(2*x+y, 0) + pixcolor(-2*x+y, 0)) & 15;
      //uint cl = (pixcolor(1+2*x, 2*y) + pixcolor(-1+2*x, 2*y)) & 15;
      //uint cl = pixcolor(2*cos(PI/subtype)+4*x, 4*y);
      uint cl = pixcolor(2*cos(PI/subtype)-4*x, -4*y);
      sign = 1; return cl;
   }
   else
   {  uint j, n; mdouble xo, yo, xn, yn, xt, yt;
      for (n = 1; n <= maxiter; n++)
      {  xo = x; yo = y; xn = x*x - y*y - 2.0L; yn = 2.0L*x*y;
         for (j = 3; j <= subtype; j++)
         {  xt = xn; yt = yn; xn =x*xt - y*yt - xo; yn = x*yt + y*xt - yo;
            xo = xt; yo = yt;
            if (xn*xn + yn*yn > 1000000.0L) return 9 + (n & 1);
         }
         //x = xn + A; y = yn + B;
         //x = 0.5*xn + A; y = 0.5*yn + B;
         //x = A*xn + B; y = A*yn;
         //x = A*xn - B*yn + 1.0L; y = A*yn + B*xn;
         //x = A*xn - B*yn + 2*cos(PI/subtype) + 2*A; y = A*yn + B*xn + 2*B;
         x = A*xn - B*yn + 2*cos(PI/subtype) - 2*A; y = A*yn + B*xn - 2*B;
         if (x*x + y*y > 10000.0L) return 9 + (n & 1);
      }
   }
   return 0;//*/

   /*/capture pullback
   if (sign < 0)
   {  mdouble w, pw = temp[1]; int j, n = pathInfo->n; if (n <= 0) return 0;
      for (j = 0; j < n; j++)
      {  f(pathInfo->rc[n-j], pathInfo->ic[n-j], x, y);
         w =x*x + y*y; if (w > bailout) return 10;
         pw *= 4.0*w; if (pw < 1e-100 || pw > 1e100) return 0;
      }
      return 10*(1 & mandelMating::martyM(pathInfo->rc[0], pathInfo->ic[0],
         x, y, pw, -1));
      //for (j = n; j < maxiter; j++)
      //{  f(pathInfo->rc[0], pathInfo->ic[0], x, y);
      //   if (x*x + y*y > bailout) return 10;
      //}
      //return 0;
   }//*/

   uint j, cl = 0; mdouble a, b;
   /*if (x <= -2 || x >= 0 || y <= -2 || y >= 0) return 0;
   a = x*x + x; b = y*y + y;
   for (int i = 7; i >= 0; i--)
   { a = a*a + x; b = b*b + y; if (a*b < 0) cl |= (1 << i); }
   return cl & 255; //test metric,  adapt drawing hue->rgb*/
   if (sign > 0) { a = x; b = y; } //z = c, not z = 0 !
   else { a = A; b = B; }

   /*/Erdoesz lemniscate cubic real
   if (sign > 0) { a *= a*a; if (4.0*a*a + b*b <= 1.0) return 1; return 10; }
   mdouble u = x*x - y*y + 3*a*a, v = 2.0*x*y; b += x*u - y*v; a = x*v + y*u;
   if (a*a + b*b <= 1.0) return 1; return 10;//*/
   /*/even quartic lemniscate
   if (sign > 0) { a = a*a - b*b - 1.0; b *= 2.0*x;
      if (a*a + b*b <= 1.0) return 1; return 10; }
   mdouble u = x*x - y*y, v = 2.0*x*y; x = u*u - v*v - 2.0*a*u + 2.0*b*v + 1.0;
   y = 2.0*(u*v - a*v - b*u); if (x*x + y*y <= 1.0) return 1; return 10;//*/
   /*/quartic Chebyshef lemiscate
   a = x*x - y*y; b = 2.0*x*y; x = a*a - b*b - 2.8284271247461901*a + 1.0;
   y = 2.0*a*b -2.8284271247461901*b;
   if (x*x + y*y <= 1.0) return 1; return 10;//*/

   if (drawmode == 4) return marty(a, b, x, y);
   if (drawmode == 7) return binary(a, b, x, y);
   if (drawmode == 8) return yoccoz(a, b, x, y);
   if (sign < 0 ||
      ( (x*x + y*y - .0625)*(x*x + y*y - .0625) >
         ((x - .25)*(x - .25) + y*y)*.25  &&
      (x + 1)*(x + 1) + y*y > .0625 ) )  //not per 1, 2 in M
   {  mdouble x1 = x, y1 = y, u, v;
      for (j = 1; j <= maxiter; j++)
      {  u = x*x; v = y*y;
         if (u + v <= bailout) { y = 2*x*y + b; x = u - v + a; }
         else
         /*{  temp[2] = 1; temp[3] = temp[2]/log(2.0);
            temp[1] = temp[2]*maxiter - temp[3]*log(log(bailout));
            u = temp[1] - temp[2]*j + temp[3]*log(log(u + v));
            if (u > 255) cl = 255; else cl = uint(u);
            break;
         }//*/
         {  if (drawmode == 2)
            { cl = ((j % 24) >> 3); if (!cl) cl = 4; if (j & 1) cl |= 8; }
            else cl = j;
            break;
         }//*/
         /*/test parabolic for 1/7
         x1 = x - 0.311744900929367; y1 = y -0.390915741234015;
         if (x1 + y1 > -0.5 && y1 > x1 && y1 < 0.4*x1) return 9 + j % 7;//*/
      }
      x = x1; y = y1;
   }
   if (drawmode == 2 || drawmode == 3) return cl;
   if (cl) cl = dist(a, b, x, y);
   if (cl & 2) Period = 1;
   else { Period = 0; if (drawmode == 6) return 0; }
   if (drawmode == 6) return turn(a, b, x, y);
   if (drawmode == 9 && sign < 0) return quadrant(a, b, x, y);
   if (drawmode == 9) return quadrantqn(a, b);
   if (drawmode == 10) return quadrantqnp(a, b);
   if (drawmode == 11) return newton(a, b);
   return cl; //drawmode == 5
}

uint mndlbrot::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = 1.0, yp = 0, nz, nzp, Marty = 0; //zp = 1
   for (j = 1; j <= maxiter; j++)
   {  nz = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = nz; //zp = 2*z*zp;
      if (sign > 0) xp++; //zp = 2*z*zp + 1
      nz = x*x - y*y + a; y = 2*x*y + b; x = nz; //z = z*z + c;
      nz = x*x + y*y; nzp = xp*xp + yp*yp;
      if (nzp > 1e40 || nz > 1e3) break;
      nz++; nzp *= temp[1]/(nz*nz); //spherical derivative*pw squared
      if (nzp > 8.0) return 1;
      if (Marty < nzp) Marty = nzp;
   }
   if (Marty > 2.0) return 9;
   if (nz > bailout) return 10;
   return 0;
}

int mndlbrot::dist(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = 1, yp = 0, nz, nzp; //zp = 1
   for (j = 1; j <= maxiter; j++)
   {  nz = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = nz; //zp = 2*z*zp;
      if (sign > 0) xp++; //zp = 2*z*zp + 1
      nz = x*x - y*y + a; y = 2*x*y + b; x = nz; //z = z*z + c;
      nz = x*x + y*y; nzp = xp*xp + yp*yp;
      if (nzp > 1e40 || nz > bailout) break;
   }
   if (nz < bailout) return 1; //not escaping, rare
   if (nzp < nz) return 10; //includes escaping through 0
   x = log(nz); x = x*x*nz / (temp[1]*nzp); //4*square of dist/pixelwidth
   if (x < 0.04) return 1;
   if (x < 0.24) return 9;
   return 10;
} //dist

int mndlbrot::turn(mdouble a, mdouble b, mdouble x, mdouble y)
{  //Already checked that escaping. Requires z = c instead of z = 0
   int j; mdouble s = 1.0, dr = 0.5, theta, u, v, r;
   if (x*x  + y*y < 1e-12) return 8; //prevent atan2(0, 0) if disconnected
   theta = atan2(y, x); root(.25 - a, -b, u, v); mdouble X = .5 - u, Y = -v;
   for (j = 1; j <= 63; j++)
   {  s *= dr; u = x*x; v = y*y; r = u + v; if (r < 1e-12) return 8;
      u -= v; v = 2*x*y; x = u + a; y = v + b;
      //theta += s*u; Adjust in triangle:
      u = atan2(u*y - v*x, u*x + v*y);
      if ( (y*a - x*b)*(Y*a - X*b) > 0
        && (y*X - x*Y)*(b*X - a*Y) > 0
        && ((b-y)*(a-X) - (a-x)*(b-Y))*(a*Y - b*X) > 0)
      { if (u < 0) u += 2*PI; else u -= 2*PI; }
      theta += s*u; if (r > 1e18*s) break;
   }
   theta *= (.5/PI); theta -= floor(theta);
   theta *= (*temp); if (theta > 1e9) return 1;
   return 9 + (long int)(theta) % 4;
/* j = (long int)(theta); int cl = (j % 24) >> 3; if (!cl) cl = 4;
   if (j & 1) cl |= 8; return cl; //*/
} //turn

int mndlbrot::turnsign(mdouble x, mdouble y)
{/*Calculate the turn, i.e., the argument of Phi. Return +-1 by comparing
   temp[0] and the turn, 0 for failure or far from the ray. Using two
   tricks to reduce the ambiguity from the multi-valued argument:
   First, the argument should jump on the Julia set instead of the
   line [0, c]. Approximate the Julia set by the lines [0, alpha] and
   [alpha, c] and change the argument accordingly within the triangle.
   Second, keep track of the arguments in temp[j] to detect jumps.
   Before searching a starting point for drawing the external ray,
   calling prepare(201) sets temp[0] = theta and temp[j] = 0,
   and it disables comparison by setting drawmode = 0. Later on, before
   tracing the ray, prepare(202) enables comparison by drawmode = 1.
   */
   mdouble a = x, b = y; if (sign < 0) { a = A; b = B; }
   int j; mdouble s = 1.0, dr = 0.5, theta, u, v, r, eps = 0.004;
   if (x*x + y*y < 1e-12) return 0; //prevent atan2(0, 0) if disconnected
   theta = atan2(y, x); root(.25 - a, -b, u, v); mdouble X = .5 - u, Y = -v;
   for (j = 1; j <= 63; j++)
   {  s *= dr; u = x*x; v = y*y; r = u + v; if (r < 1e-12) return 0;
      u -= v; v = 2*x*y; x = u + a; y = v + b;
      //theta += s*u; First adjust argument in triangle:
      u = atan2(u*y - v*x, u*x + v*y);
      if ( (y*a - x*b)*(Y*a - X*b) > 0
        && (y*X - x*Y)*(b*X - a*Y) > 0
        && ((b-y)*(a-X) - (a-x)*(b-Y))*(a*Y - b*X) > 0)
      { if (u < 0) u += 2*PI; else u -= 2*PI; }
      //Second compare and shift.  3.6 is ok for initial value 0:
      if (drawmode)
      {  if (u > temp[j] + 3.6) u -= 2*PI;
         if (u < temp[j] - 3.6) u += 2*PI;
         temp[j] = u;
      }
      theta += s*u; if (r > 1e18*s) break;
   }
   //Problem: j larger is inaccuarte, but thus ray ends at esctime 64:
   if (r < 100) return 0; //prevent strong inaccuracy. Or r < 1e10*s ?
   theta *= (.5/PI);
   theta -= temp[0]; theta -= floor(theta); // 0 <= theta < 1
   if (theta < eps) return 1; if (1 - eps < theta) return -1;
   return 0;
} //turnsign

mdouble mndlbrot::green(int sg, mdouble x, mdouble y)
{  mdouble a = x, b = y; if (sg < 0) { a = A; b = B; }
   uint j; mdouble s = 1.0, dr = 0.5, u, v;
   for (j = 0; j <= maxiter; j++)
   {  s *= dr; u = x*x; v = y*y;
      if (u + v > 1e12) return s*log(u + v);
      y = 2*x*y + b; x = u - v + a;
   }
   return 0;
}

int mndlbrot::binary(mdouble a, mdouble b, mdouble x, mdouble y)
{  mdouble u, v; uint j;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y;
      if (u + v <= 1e4) { y = 2*x*y + b; x = u - v + a; }
      else { if (j & 1) j = 2; else j = 10; if (y > 0) j += 2; return j; }
      //else return (x > 50.0 + y && x > 50.0 - y ? 12 : 10); //carrots
   }
   return 0;
} //binary

int mndlbrot::yoccoz(mdouble a, mdouble b, mdouble x, mdouble y)
{  mdouble u, v; uint j;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y;
      if (u + v <= 1e4) { y = 2*x*y + b; x = u - v + a; }
      else
      {  if (j & 1) j = 2; else j = 10;
         if (a < -0.75)
         { if (x*sqrt(3.0L) < y && x*sqrt(3.0L) < -y) j += 2; return j; }
         if (b < 0) { b = -b; y = -y; } //conjugate 2/3 -> 1/3
         if (b <= 0.65) return 7;
         if (y < x*tan(2*PI/7.0) || y < x*tan(PI/7.0)) j += 2;
         else if (y > x*tan(4*PI/7.0)) j--;
         return j;
      }
   }
   return 0;
} //yoccoz

int mndlbrot::newton3(mdouble a, mdouble b)
{  //newton for (c^2 + c)^2 + c . exterior checked in period (dist)
   mndplex c(a, b), f, fp; uint j, cl = 15;
   for (j = 1; j <= maxiter; j++)
   {  fp = c*c + c; f = fp*fp + c; fp = fp*(4.0*c + 2.0) + 1.0;
      if (norm(f) > 1e100 || norm (fp) > 1e100 || norm(fp)< 1e-20) return cl;
      c -= f/fp;
      if (norm(c) < .1) { cl = 0; break; }
      if (norm(c - mndplex(-1.754877666246693)) < .1) { cl = 1; break; }
      if (norm(c - mndplex(-0.122561166876654, 0.744861766619744)) < .05)
      { cl = 4; break; }
      if (norm(c - mndplex(-0.122561166876654, -0.744861766619744)) < .05)
      { cl = 2; break; }
   }
   if (Period && cl < 15) cl |= 8; //M is dark
   return cl;
} //newton3

int mndlbrot::newton(mdouble a, mdouble b)
{  //newton for q_n(c), n = subtype . exterior checked in period (dist)
   if (subtype == 3) return newton3(a, b);
   mndplex c(a, b), c0, f, fp; int i, cl = 15; uint j;
   for (j = 1; j <= maxiter; j++)
   {  c0 = c; f = c; fp = 1.0;
      for (i = 1; i < subtype; i++)
      { fp = 2.0*f*fp + 1.0; f = f*f + c; if (norm(f) > 1e100) return cl; }
      if (norm (fp) > 1e100 || norm(fp) < 1e-20) return cl; c -= f/fp;
      if (norm(c) < 1/64.0) { cl = 0; break; }
      if (norm(c - c0) < 1e-6)
      { cl = 2; if (j & 1) cl = 4; /*if (j <= 2) cl = 1;*/ break; }
   }
   if (Period && cl < 15) cl |= 8; //M is dark
   return cl;
} //newton

int mndlbrot::quadrant(mdouble a, mdouble b, mdouble x, mdouble y)
{  //exterior checked in Period (dist)
   int cl = 1, j;
   for (j = 1; j < subtype; j++)
   { f(a, b, x, y); if (x*x + y*y > 1e100) return 0; }
   if (x > 0) cl = 3; if (y < 0) cl++;
   if (Period) cl |= 8; return cl;
}//quadrant

int mndlbrot::quadrantqn(mdouble a, mdouble b)
{  //exterior checked in Period (dist)
   mdouble x = a, y = b; int cl = 1, j;
   for (j = 1; j < subtype; j++)
   { f(a, b, x, y); if (x*x + y*y > 1e100) return 0; }
   if (x > 0) cl = 3; if (y < 0) cl++;
   if (Period) cl |= 8; return cl;
}//quadrantqn

int mndlbrot::quadrantqnp(mdouble a, mdouble b)
{  //exterior checked in Period (dist)
   int cl = 1, j;
   mndplex C(a, b), Q = C, P = 1.0;
   for (j = 1; j < subtype; j++)
   {  P = 2.0*Q*P + 1.0; Q = Q*Q + C;
      if (norm(Q) > 1e100 || norm (P) > 1e100) return 0;
   }
   if (real(P) > 0) cl = 3; if (imag(P) < 0) cl++;
   if (Period) cl |= 8; return cl;
} //quadrantqnp

/* The following function is implementing local similarity,  return 0 for ok.
   a, b is the current parameter close to a primitive center of period per or
   a Misiurewicz point. It is computing a region in the dynamic plane from a
   region in the parameter plane,  both given by t[]. When preper is 65001 or
   65002, affine renormalization in one plane is performed. For 65003, the
   modulus of zp^2 is returned in *t (only periodic case).
*/
int mndlbrot::similarity(uint preper, uint per, mdouble &a, mdouble &b,
   mdouble *t) const
{  uint i, k = preper; if (preper > 65000u) k = 0;
   mdouble a0 = a, b0 = b; if (find(1, k, per, a0, b0)) return 1;
   mdouble x = a0, y = b0, xp = 1.0, yp = 0, xd = 1.0, yd = 0,
      xp0 = 0, yp0 = 0, xd0 = 0, yd0 = 0, u;
   for (i = 1; i < k + per; i++)
   {  if (i == k) { xp0 = xp; yp0 = yp; xd0 = xd; yd0 = yd; }
      u = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = u;
      u = 2*(x*xd - y*yd); yd = 2*(x*yd + y*xd); xd = u + 1.0;
      u = x*x - y*y + a0; y = 2*x*y + b0; x = u;
      if (x*x + y*y + xp*xp + yp*yp + xd*xd + yd*yd > 1e100) return 2;
      if (!k && i <  per - 1 && x*x + y*y < 1e-20) return 3; //period < per
      if (!k && i == per - 1 && x*x + y*y > 1e-20) return 4; //period > per
   }
   xp += xp0; yp += yp0; xd += xd0; yd += yd0;
   yd0 = xp*xp + yp*yp; if (yd0 < 1e-100) return 5;
   u = (xp*xd + yp*yd)/yd0; yd = (xp*yd - yp*xd)/yd0; xd = u; //zd = lambda
   if (preper == 65003u) { t[0] = xp*xp + yp*yp; return 0; }
   if (k) //c -> z preperiodic
   {  t[0] -= a; t[1] -= b;
      u = a + xd*t[0] - yd*t[1]; t[1] = b + xd*t[1] + yd*t[0]; t[0] = u;
      u = xd*t[2] - yd*t[3]; t[3] = xd*t[3] + yd*t[2]; t[2] = u;
      return 0;
   }
   u = xp*xp - yp*yp; yp = 2*xp*yp; xp = u; //zp = A
   if (!preper) //c -> z periodic
   {  yd0 = 2*(xp*xp + yp*yp); if (yd0 < 1e-100) return 6;
      x = xp/yd0; y = -yp/yd0; //z = 1/2A
      t[0] -= a; t[1] -= b;
      u = a + xd*t[0] - yd*t[1] + x; t[1] = b + xd*t[1] + yd*t[0] + y; t[0] = u;
      u = xd*t[2] - yd*t[3]; t[3] = xd*t[3] + yd*t[2]; t[2] = u;
      return 0;
   }
   x = xp*xd - yp*yd; y = xp*yd + yp*xd; //z = A*lambda
   if (preper & 1) //c -> ~c periodic
   {  t[0] -= a0; t[1] -= b0;
      u = x*t[0] - y*t[1]; t[1] = x*t[1] + y*t[0]; t[0] = u;
      u = x*t[2] - y*t[3]; t[3] = x*t[3] + y*t[2]; t[2] = u;
      return 0;
   }
   t[0] -= a; t[1] -= b; a -= a0; b -= b0; //z -> ~z periodic
   u = x*a - y*b; b = a*y + x*b; a = u; //~c
   u = a + xp*t[0] - yp*t[1]; t[1] = b + xp*t[1] + yp*t[0]; t[0] = u;
   u = xp*t[2] - yp*t[3]; t[3] = xp*t[3] + yp*t[2]; t[2] = u;
   return 0;
 }

//////////////////////////////////////////////////////////////////////

void mndmulti::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  int j = 1; mdouble u = x, v = y, U, V;
   while (2*j <= subtype)
   {  j *= 2; U = u*u; V = v*v; if (U + V > 1e100) return;
      v *= (2*u); u = U - V;
   }
   while (j < subtype)
   {  j++; U = u*x - v*y; v = u*y + v*x; u = U;
      if (u*u + v*v > 1e100) return;
   }
   x = u + a; y = v + b;
}//multi*/
/*{  mdouble u = x*x - y*y + a; y = 2*x*y + b; x = u;
   const mdouble x0 = -0.45, y0 = 0.87, rr = 0.25, RR = 0.75;
   a = x - x0; b = y - y0; u = a*a + b*b;
   if (u <= rr || u >= RR) return;
   u = (u - rr)/(RR - rr); u *= u*(3.0 - 2.0*u);
   u *= 2.0*PI; x = cos(u); y = sin(u);
   u = x0 + a*x - b*y; y = y0 + a*y + b*x; x = u;
}//Dehn Tf*/
/*{  mdouble A = a, B = b, u;
   const mdouble x0 = -0.45, y0 = 0.87, rr = 0.25, RR = 0.75;
   a = x - x0; b = y - y0; u = a*a + b*b;
   if (u > rr && u < RR)
   {  u = (u - rr)/(RR - rr); u *= u*(3.0 - 2.0*u);
      u *= 2.0*PI; x = cos(u); y = sin(u);
      u = x0 + a*x - b*y; y = y0 + a*y + b*x; x = u;
   }
   u = x*x - y*y + A; y = 2*x*y + B; x = u;
}//Dehn fT*/
/*{  a = x*x - y*y - 1.0; b = 2.0*x*y; if (x < 0) { x = -x; y = -y; }
   mdouble t = 2.0*(x - (y >= 0 ? y :-y)/sqrt(3.0));
   if (t < 0) t = 0; if (t > 2.0) t = 2.0;
   x = a + t*(1.0 - x); y = b - t*y;
}//topo basilica, non-expanding*/
/*{  a = x*x - y*y; b = 2.0*x*y;
   y = 2.0*b + 0.8; if (y > 0.5) y = 0.5; if (y < -0.5) y = -0.5;
   x = a - y; y = b + sqrt(0.75);
}//spider basilica*/
/*{  a = x*x; b = y*y; y *= 2*x; x = a - b - 1.0;
   a += b; b =sqrt(2.0); if (a >= 1.0) return;
   if (a <= b - 1.0) { x += 1.0 - b; return; }
   x += 0.5*b*(a - 1.0);
}//rhomboid*/

void mndmulti::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg > 0) { xmid = 0; rewidth = 1.76; }
   else { xmid = 0; rewidth = 2.0; }
}

int mndmulti::find(int sg, uint preper, uint per, mdouble &x, mdouble &y) const
{  mdouble a = A, b = B, fx, fy, px, py, u, v, w; uint i, j, l;
   for (i = 0; i < 30; i++)
   {  if (sg > 0) { a = x; b = y; }
      fx = x; fy = y; px = 1; py = 0;
      for (j = 0; j < preper; j++)
      {  u = fx; v = fy; if (px*px + py*py > 1e100) return 1;
         for (l = 3; l <= uint(subtype); l++) //^= subtype - 1
         { w = u*fx - v*fy; fy = u*fy + v*fx; fx = w; }
         w = subtype*(fx*px - fy*py); py = subtype*(fx*py + fy*px);
         px = w; if (sg > 0) px++;
         w = u*fx - v*fy + a; fy = u*fy + v*fx + b; fx = w;
      }
      if (!preper && sg > 0) { fx = 0; fy = 0; px = 0; py = 0; }
      mdouble Fx = fx, Fy = fy, Px = px, Py = py;
      for (j = 0; j < per; j++)
      {  u = fx; v = fy; if (px*px + py*py > 1e100) return 2;
         for (l = 3; l <= uint(subtype); l++) //^= subtype - 1
         { w = u*fx - v*fy; fy = u*fy + v*fx; fx = w; }
         w = subtype*(fx*px - fy*py); py = subtype*(fx*py + fy*px);
         px = w; if (sg > 0) px++;
         w = u*fx - v*fy + a; fy = u*fy + v*fx + b; fx = w;
      }
      fx -= Fx; fy -= Fy; px -= Px; py -= Py;
      w = px*px + py*py; if (w < 1e-100) return -1;
      x -= (fx*px + fy*py)/w; y += (fx*py - fy*px)/w;
   }
   return 0;
}

int mndmulti::bifurcate(mdouble t, mdouble &a, mdouble &b) const
{  if (t <= -1.0) return 1;
   t *= (2*PI); a = 1 - cos(t)/subtype; b = -sin(t)/subtype;
   if (!(subtype & 1)) t += (subtype - 2)*PI;
   t /= (subtype - 1); mdouble x = cos(t), y = sin(t);
   t = pow(1.0/subtype, 1.0/(subtype - 1)); x *= t; y *= t;
   t = a*x - b*y; b = a*y + b*x; a = t; return 1;
}

void mndmulti::prepare(int sg, uint nmax, uint &dm, mdouble *t)
{  if (dm == 201) //exray
   {  sign = sg; drawmode = 0; temp[0] = *t;
      for (int j = 1; j <= 63; j++) temp[j] = 0;
      return;
   }
   if (dm == 202) //activate comparison in turnsign()
   { drawmode = 1; return; }
   mndynamics::prepare(sg, nmax, dm, t);
   if (drawmode > 255) return;
   if (drawmode > 2 && drawmode <= 11) temp[1] = *t;
   if (drawmode == 4 || drawmode == 5) drawmode = 2; //!!!!!!!!!!!!!!!!
   if (drawmode == 6)
   { *temp = 2.5/sqrt(*t); if (*temp < 600.0) *temp = 600.0; }
}

uint mndmulti::pixcolor(mdouble x, mdouble y)
{  uint j, cl = 0; mdouble a, b;
   if (sign > 0) { a = x; b = y; } //z = c, not z = 0 !
   else { a = A; b = B; }
   /*/R6 Daniel
   mdouble u, v, w;
   for (j = 1; j <= maxiter; j++)
   {  a = x*x - y*y; b = 2.0*x*y;
      x = a*a - b*b - 2.25*a + 81.0/64.0; y = 2.0*a*b - 2.25*b;
      u = a*x - b*y; v = a*y + b*x; w = u*u + v*v; if (w < 1.0e-60) return 0;
      a--; w = (27.0/32.0)/w; x = w*(a*u + b*v) + 1.0; y = w*(b*u - a*v);
   }
   if (y < 0.0) return 9; else return 10;
   ////////////////////////////////////////////*/
   mdouble x1 = x, y1 = y;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y);
      else
      {  if (drawmode == 2)
         { cl = ((j % 24) >> 3); if (!cl) cl = 4; if (j & 1) cl |= 8; }
         else cl = j;
         break;
      }
   }
   if (drawmode == 2 || drawmode == 3 || !cl) return cl;
   x = x1; y = y1;
   //cl = dist(a, b, x, y); if (drawmode == 3) return cl;!!!!!!!!!!!!!!!!
   //if (!(cl & 2)) return 0;
   return turn(a, b, x, y);
}

/*
int mndmulti::dist(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint i; mdouble xp = 1, yp = 0, nz, nzp; //zp = 1
   for (i = 1; i <= maxiter; i++)
   {  nz = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = nz; //zp = 2*z*zp;
      if (sign > 0) xp++; //zp = 2*z*zp + 1
      nz = x*x - y*y + a; y = 2*x*y + b; x = nz; //z = z*z + c;
      nz = x*x + y*y; nzp = xp*xp + yp*yp;
      if (nzp > 1e60 || nz > 1e60) break;
   }
   if (nzp < 0.01 || nz < 0.1) return 1; x = log(nz);
   x = x*x*nz / (temp[1]*nzp);
   if (x < 0.08) return 1;
   if (x < 0.15) return 9;
   if (x < 1.0) return 3;
   return 11;
} //dist ... error: in disconnected Julia,  z = 0 is black
*/

int mndmulti::turn(mdouble a, mdouble b, mdouble x, mdouble y)
{  //Already checked that escaping. Requires z = c instead of z = 0
   int j; mdouble s = 1.0, dr = 1.0/subtype, theta, u, v, r;
   if (x*x  + y*y < 1e-12) return 8; //prevent atan2(0, 0) if disconnected
   theta = atan2(y, x);
   for (j = 1; j <= 63; j++)
   {  s *= dr; f(a, b, x, y); u = x - a; v = y - b;
      r = x*x  + y*y; if (r < 1e-12) return 8;
      theta += s*atan2(u*y - v*x, u*x + v*y); if (r > 1e18*s) break;
   }
   theta *= (.5/PI); theta -= floor(theta);
   theta *= (*temp); if (theta > 1e9) return 1;
   return 9 + (long int)(theta) % 4;
} //turn

int mndmulti::turnsign(mdouble x, mdouble y)
{  //Calculate the turn, return +-1 by comparing temp[0] and the turn,
   //0 for failure or far from the ray. Cf. mndlbrot documentation.
   mdouble a = x, b = y; if (sign < 0) { a = A; b = B; }
   int j; mdouble s = 1.0, dr = 1.0/subtype, theta, u, v, r, eps = .004;
   if (x*x  + y*y < 1e-12) return 0; //prevent atan2(0, 0) if disconnected
   theta = atan2(y, x);
   for (j = 1; j <= 63; j++)
   {  s *= dr; f(a, b, x, y); u = x - a; v = y - b;
      r = x*x  + y*y; if (r < 1e-12) return 0;
      u = atan2(u*y - v*x, u*x + v*y);
      if (drawmode) //compare and shift.  3.6 is ok for initial value 0
      {  if (u > temp[j] + 3.6) u -= 2*PI;
         if (u < temp[j] - 3.6) u += 2*PI;
         temp[j] = u;
      }
      theta += s*u; if (r > 1e18*s) break;
   }
   if (r < 100) return 0;
   theta *= (.5/PI);
   theta -= temp[0]; theta -= floor(theta); // 0 <= theta < 1
   if (theta < eps) return 1; if (1 - eps < theta) return -1;
   return 0;
} //turnsign

//mdouble mndmulti::green(int sg, mdouble x, mdouble y)
mdouble mndmulti::green(int, mdouble, mdouble)
{

   return 0;
}

//////////////////////////////////////////////////////////////////////

void mndbfpoly::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u, v, w; int j, q = subtype; //assume q >= 2
   w = a*x - b*y; b = a*y + b*x; a = w;
   x = 1 + x/q; y = y/q;
   u = x*x - y*y; v = 2*x*y;
   for (j = 3; j <= q; j++)
   { w = u*x - v*y; v = u*y + v*x; u = w; }
   x = u*a - v*b; y = u*b + v*a;
}

void mndbfpoly::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg > 0)
   { xmid = 2.0*(subtype + 1); rewidth = 3.2*(subtype + 1); }
   else { xmid = -subtype - 1.0; rewidth = 1.3*(subtype + 1); }
}

//////////////////////////////////////////////////////////////////////

void mndcubic::critical(mdouble, mdouble, mdouble &x, mdouble &y) const
{  x = 1.0; y = 0;
   if (subtype == 1 && rb*rb + ib*ib > 0 && (maxiter & 1) ) x = -1.0;
}

void mndcubic::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (subtype == 6) { frabbit(a, b, x, y); return; }
   if (subtype == 5 || subtype == 7) { fbasilica(a, b, x, y); return; }
   mdouble u, v, w; u = x*x - y*y - 3; v = 2*x*y;
   w = x*u - y*v; y = x*v + y*u; x = w; if (subtype > 1) x -= 2; //odd
   w = a*x - b*y; y = a*y + b*x; x = w;
   if (subtype == 1) { x += rb; y += ib; }
   if (subtype == 2) x++; //
   if (subtype == 3) x -= 2; //
   if (subtype == 4) x--; //
} // c(z^3 - 3z - 2) + b

void mndcubic::fbasilica(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble A = a*a - b*b, B = 2*a*b;
   if (subtype == 5) { A -= (a + 2); B -= b; }
   else { A += (2*a + 1); B += 2*b; }
   mdouble R = A*A + B*B; if (R < 1e-20) { x = 1e100; return; }
   mdouble u, v, w; u = x*x - y*y - 3; v = 2*x*y;
   w = x*u - y*v; y = x*v + y*u; x = w - 2;
   w = a - (A*x + B*y)/R; y = b + (B*x - A*y)/R; x = w;
} // b - (z^3 - 3z - 2)/(b^2 - b - 2) , b = c
  // b - (z^3 - 3z - 2)/(b^2 - 2b + 1) , b = c

void mndcubic::frabbit(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mndplex c(a, b), z(x, y), c3 = c*c*c, c4 = c3*c - c + 1.0;
   c3 = c3 - c + 1.0; //c4 = c^4 - c + 1, c3 = c^3 - c + 1
   if (norm(c4 - c3) < 1e-30 || norm(c3) < 1e-30 ||
       norm(c4) < 1e-30) { x = 1e100; return; }
   z = c4*c4*c/(9.0*(c4 - c3)*c3) * (z*(z*z - 3.0) - 2.0) + 3.0*c*c3/c4 - 1.0;
   x = real(z); y = imag(z);
} // A(z^3 - 3z - 2) + b

void mndcubic::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg && subtype == 1) { rb = xmid; ib = rewidth; return; }
   xmid = 0; rewidth = 3.0; if (subtype == 1 || subtype == 4) rewidth = 2.8;
   if (subtype == 7) rewidth = 8.0;
   if (sg > 0)
   {  if (subtype == 1) { rewidth = 1.2; xmid = 0; }
      if (subtype == 2) { rewidth = 0.7; xmid = 0.2; }
      if (subtype == 3) { rewidth = 0.63; xmid = -0.43; }
      if (subtype == 4) { rewidth = 0.58; xmid = -0.24; }
      if (subtype == 5) { rewidth = 4.2; xmid = 1.0; }
      if (subtype == 6) { rewidth = 2.4; xmid = -0.25; }
      if (subtype == 7) { rewidth = 8.5; xmid = -2.0; }
   }
}

int mndcubic::bifurcate(mdouble t, mdouble &a, mdouble &b) const
{  if (subtype > 4 || rb*rb + ib*ib > 0) return 0;
   int per = 1; if (subtype == 1 && (a < -2/3.0 || a > 1/3.0) ) per = 2;
   if (t <= -1.0) return per;
   t *= (2*PI); mdouble l = cos(t), m = sin(t);
   if (subtype == 1 && (per & 1) )
   {  if (a < -1/3.0) { a = l/6 - 0.5; b = m/6; }
      else { a = -l/3; b = -m/3; }
      return 1;
   }
   if (subtype == 1) //per 2
   {  if (a > 2/3.0 || a < 0) //a < -2/3.0
      {  t = a; root(0.5 - l/18, -m/18, a, b);
         if (t < 0) { a = -a; b = -b; }
      }
      else
      {  t = b; root(l/36, m/36, a, b);
         if (t*b >= 0) { a += 0.5; } else { a = 0.5 - a; b = -b; }
      }
      return 2;
   }
   if (subtype == 4)
   {  t = (l - 2)*(l - 2) + m*m; a = (l - 4 + (l - 2)/t)/9;
      b = m*(1 - 1/t)/9; return 1;
   }
   t = a; mndplex c, p, q, mu = mndplex(l/3, m/3);
   if (subtype == 2)
   {  l = -1/48.0; p = ((3.0/16)*mu - 1.0/8)*mu;
      q = 0.125*mu*(mu - 1.0)*(mu - 1.0);
   }
   else
   {  l = -0.6; p = (-3.0/8)*mu*mu + 2.0*mu - 3.0;
      q = -0.25*mu*(mu - 1.0)*(mu - 1.0);
   }
   q = p*p - q; root(real(q), imag(q), a, b); q = mndplex(a, b); p += q;
   if ( (real(p) - l)*(t - l) <= 0) p -= 2.0*q;
   a = real(p); b = imag(p); return 1;
} //bifurcate

void mndcubic::prepare(int sg, uint nmax, uint &dm, mdouble *t)
{  mndynamics::prepare(sg, nmax, dm, t);
   if (drawmode == 4) temp[1] = *t;
}

uint mndcubic::pixcolor(mdouble x, mdouble y)
{  if (drawmode < 4) return mndynamics::pixcolor(x, y);
   uint cl1; //subtype <= 4, drawmode = 4
   if (sign < 0) { cl1 = marty(A, B, x, y); return (cl1 ? cl1 : 10); }
   cl1 = marty(x, y, 1.0, 0);
   //bd.locus of +1 red, of -1 blue, section magenta, exterior is green
   if (subtype == 1 && rb*rb + ib*ib > 1e-20)
   {  cl1 = (cl1 ? 12 : 0); uint cl2 = (marty(x, y, -1.0, 0) ? 9 : 0);
      cl1 |= cl2; if (cl1 == 13) cl1 = 5;
   }
   return (cl1 ? cl1 : 10);
}

uint mndcubic::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = (sign > 0 ? 0 : 1.0), yp = 0, u, v, w;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x - y*y - 3; v = 2*x*y;
      w = x*u - y*v; y = x*v + y*u; x = w; if (subtype > 1) x -= 2;
      w = u + 2; u = 3*(a*w - b*v); v = 3*(a*v + b*w);
      w = u*xp - v*yp; yp = u*yp + v*xp; xp = w;
      if (sign > 0) { xp += x; yp += y; }
      w = a*x - b*y; y = a*y + b*x; x = w;
      if (subtype == 1) { x += rb; y += ib; }
      if (subtype == 2) x++; //
      if (subtype == 3) x -= 2; //
      if (subtype == 4) x--; //
      u = xp*xp + yp*yp; v = x*x + y*y + 1;
      if (u > 1e60 || v > 1e3) return 0;
      if (temp[1]*u > 2.25*v*v) return 4;
   }
   return (v > bailout ? 0 : 12);
}

//////////////////////////////////////////////////////////////////////

void mandelQPPQ::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a*a + b*b, v, ai, bi; if (u < 1e-10) return; ai = a/u; bi = -b/u;
   u = x*x - y*y; v = 2.0*x*y;
   if (subtype & 1) { u--; x = a*u - b*v; y = a*v + b*u; }
   else { x = ai*u - bi*v + 1.0; y = ai*v + bi*u; }
   u = x*x - y*y; v = 2.0*x*y;
   if (subtype & 2) { u--; x = a*u - b*v; y = a*v + b*u; }
   else { x = ai*u - bi*v + 1.0; y = ai*v + bi*u; }
}
// P c(z^2 - 1)  and  Q = z^2/c + 1 , performs  QP  or  PQ

void mandelQPPQ::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = (sg > 0 ? -1.0 : 0.0); rewidth = 2.0; }

//////////////////////////////////////////////////////////////////////

void mndquartic::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y, v = 2*x*y;
   if (subtype == 2000)
   { u += a; v += b; x = u*u - v*v; y = 2*u*v; return; }
   if (subtype == 3000)
   {  mdouble X = 3*u - 4*(a*x - b*y) - 6, Y = 3*v - 4*(a*y + b*x),
      w = X*u - Y*v + 12*(a*x - b*y) + 8*a + 3;
      y = X*v + Y*u + 12*(a*y + b*x) + 8*b; x = w;
      u = a*a - b*b - 3; v = 2*a*b;
      X = u*u - v*v - 8*a - 12; Y = 2*u*v - 8*b;
      w = X*X + Y*Y; if (w < 1e-10) { x = 1e100; return; }
      u = ((a - 1)*X + b*Y)/w; v = (b*X + (1 - a)*Y)/w;
      w = u*x - v*y + a; y = u*y + v*x + b; x = w; return;
   }
   /*/iterated Chebyshev, set critical = 0
   mdouble w; u -= 2.0L;
   for (int i = 2; i <= subtype; i++)
   {  w = u*u - v*v; v *= 2.0L*u; u = w - 2.0L;
      if (u*u + v*v > 1e10) { x = 1e100; return; }
   }
   x = a*u - b*v; y = a*v + b*u; //*/
   mdouble w;
   for (int i = 2; i < subtype; i++)
   {  w = x*u - y*v; v = x*v + y*u; u = w;
      if (u*u + v*v > 1e10) { x = 1e100; return; }
   }
   u -= subtype*x; v -= subtype*y;
   x = a*u - b*v; y = a*v + b*u; //*/
}
//...: c(z^q - qz)
//2000: (z^2 + c)^2
//3000: A(3z^4 - 4cz^3 - 6z^2 + 12cz + 8c + 3) + c ,
//   A = (b - 1) / (b^4 - 6b^2 - 8b - 3)

void mndquartic::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  xmid = 0;
   if (subtype < 2000)
   { if (sg > 0) rewidth = 0.75; else rewidth = 2.8; }
   if (subtype == 2000)
   { if (sg > 0) rewidth = 1.76; else rewidth = 2.0; }
   if (subtype == 3000)
   { if (sg > 0) rewidth = 7.0; else rewidth = 4.0; }
}

//////////////////////////////////////////////////////////////////////

void mndsurge::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (subtype == 3) { finv(a, b, x, y); return; }
   mdouble u, v, A, B; if (a >= -.75) { x = 10000; return; }
   if (subtype == 2 && x > 0) { x = -x; y = -y; } //modified
   root(.25 - a, -b, u, v); A = .5 - u; B = -v; // A + iB = alpha
   x -= A; y -= B;
   if (2*x + y > 0 && 2*x <= y) //small top sector
   { u = -.5*x -1.25*y; v = .5*x - 1.75*y; x = u + A; y = v + B; return; }
   if (x + y < 0 && x <= y) //left sector
   { u = -.4*y; v = .8*x; x = u + A; y = v + B; return; }
   if (x > y && 2*x + y < 0) //small lower sector
   { u = 1.25*y; y = -2.5*x; x = u; }
   x += A; y += B; u = x*x - y*y + a; y = 2*x*y + b; x = u;
}

void mndsurge::finv(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u, A, B; if (b <= 0.649519052838329) { x = 10000; return; }
   root(.25 - a, -b, A, B); A = .5 - A; B = -B; // A + iB = alpha
   x -= A; y -= B;
   if (y < .25*x && y < 2*x) //P0
   { x += A; y += B; u = x*x - y*y + a; y = 2*x*y + b; x = u; return; }
   if (x + .6*y > 0) //P1
   {  x += A; y += B; u = x*x - y*y + a; y = 2*x*y + b;
      x = u*u - y*y + a; y = 2*u*y + b; return;
   }
   y = B - x; x = A - 4*x;
}

void mndsurge::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg < 0) { xmid = 0; rewidth = 2.0; return; }
   if (subtype <= 2) { xmid = -1.4; rewidth = 0.7; }
   else { xmid = 1.1; rewidth = 2.65; }
   //better  hmid = -0.113; vmid = 0.888; rewidth = 0.343;
}

//////////////////////////////////////////////////////////////////////

void mndrealcubic::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y - 3*a, v = 2*x*y; if (subtype & 2) { u = -u; v = -v; }
   b += (x*u - y*v); y = x*v + y*u; x = b;
}

void mndrealcubic::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (a <= 0) { x = 0; y = sqrt(-a); return; }
   x = sqrt(a); y = 0; //slower orbit:
   if ( (b < 0 && subtype == 1) || (b > 0 && subtype == 2) ) x = -x;
}

void mndrealcubic::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y); if (mode <= 0) return;
   critical(A, B, x, y); if (mode == 2) { x = -x; y = -y; }
}

void mndrealcubic::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; if (sg > 0) rewidth = 1.4; else rewidth = 2.8; }

uint mndrealcubic::pixcolor(mdouble x, mdouble y)
{  uint cl = mndynamics::pixcolor(x, y);
   if (sign < 0 || x <= 0 || cl) return cl; //otherwise slower orbit bounded
   uint j; mdouble a = x, b = y;
   critical(a, b, x, y); x = -x; y = -y; //faster critical point
   for (j = 1; j <= maxiter; j++)
   { if (x*x + y*y <= bailout) f(a, b, x, y); else return 7; }
   return 0;
}

///////////////////////////////////////////////////////////////////////

/*void mndtricorn::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y + 1.0, v = 2.0*x*y, w = u*u + v*v; if (w < .0001) return;
   w = -2.0/w; u *= w; v *= w; w = x*u + y*v; y = y*u - x*v; x = w;
   root(x, y, x, y); if (x + y < 0.0) { x = -x; y = -y; }
}//2222c*/

/*void mndtricorn::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  x -= (2.0L/3.0L)*a; y -= (2.0L/3.0L)*b; mdouble u, v, w; int i;
   for (i = 0; i <= 1; i++)
   {  if (i) a = -a;
      u = x*x - y*y; v = 2.0L*x*y; x += a; y += b;
      w = x*u - y*v; y = x*v + y*u; x = w;
   }
   x -= (2.0L/3.0L)*a; y += (2.0L/3.0L)*b;
}//anti mating antipode preserving*/

/*void mndtricorn::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y, v = 2.0L*x*y, w;
   x = a*u - b*v + 1.0L; y = a*v + b*u;
   w = b; b = a + 1.0L; a = -w;
   w = a*u - b*v + 1.0L; v = a*v + b*u; u = w;
   w = x*x + y*y; if (w < 1.0E-100L) { x = 1.0E100L; return; }
   u /= w; v /= w; w = u*x + v*y; y = v*x - u*y; x = w;   
}//root of Misi K_i at 0 */

void mndtricorn::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  int j = 1; y = -y; mdouble u = x, v = y, U, V;
   while (2*j <= subtype)
   {  j *= 2; U = u*u; V = v*v; if (U + V > 1e100) return;
      v *= (2*u); u = U - V;
   }
   while (j < subtype)
   {  j++; U = u*x - v*y; v = u*y + v*x; u = U;
      if (u*u + v*v > 1e100) return;
   }
   x = u + a; y = v + b;
}//normal*/

void mndtricorn::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0 || mode < -2 || subtype != 2) return;
   root(x - A, y - B, x, y);
   if (mode & 1) x = -x; else y = -y;
}

void mndtricorn::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  xmid = 0; rewidth = 2.0; if (sg < 0) return;
   rewidth = 1.6; if (subtype == 2) xmid = -0.5;
}

///////////////////////////////////////////////////////////////////////

void mndhenon::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{ mdouble u = a + y - x*x; y = b*x; x = u; }

void mndhenon::finv(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{ if (b*b < 1e-10) return; mdouble u = x + y*y/(b*b) - a; x = y/b; y = u; }

void mndhenon::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode < 0) finv(A, B, x, y);
}

void mndhenon::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.0; }

uint mndhenon::pixcolor(mdouble x, mdouble y)
{  if (sign > 0)
   {  uint cl = 0; mdouble yy= y*y; x *= 4;
      if (x > -1.0 + 2*y - yy) cl = 1;
      if (x > 3.0 - 6*y + 3*yy) cl = 2;
      if (x > 5.0 - 6*y + 5*yy) cl = 4;
      if (x > 7.0 + 10*y + 7*yy) cl = 3;
      if (y > -1.0 && y < 1.0) cl |= 8;
      return cl;
   }
   uint j, cl = 0; mdouble x0 = x, y0 = y, z;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y > 1e100) return 7;
      if (x + 2*y <= bailout) finv(A, B, x, y);
      else { cl = j; break; }
   }
   x = x0; y = y0; //for period 2:
   z = 0.5*(1.0 - B); y0 = A - 3*z*z; if (y0 < 0) y0 = 0;
   x0 = z + sqrt(y0); y0 = B*(2*z - x0); //periodic point
   z = (4*A  + 6*B)/(1.0 + B*B); //attracting when 3.0 < z < 5.0
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y > 1e100) return 7;
      if (2*x + y >= -bailout) f(A, B, x, y);
      else { if (cl) return 1 + 8*((cl + j) & 1); else return 2 + 8*(j & 1); }
      if (3.0 < z && z < 5.0 && cl && (x-x0)*(x-x0) + (y-y0)*(y-y0) < 1e-6)
         return ( (j & 1 ? 4 : 5) + 8*(cl & 1) );
   }
   if (cl && (z < 3.0 || z > 5.0) ) return 4 + 8*(cl & 1); else return 0;
}

///////////////////////////////////////////////////////////////////////

void mndifs::f(mdouble, mdouble, mdouble &, mdouble &) const
{ }

void mndifs::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  mdouble u = A*A + B*B;
   if (u < 0.0001 || (0.99 < u && u < 1.01) || u > 10000.0) return;
   if (u < 1.0 && mode < 0)
   {  x += ((mode & 1) ? 1.0 : -1.0);
      u = A*x - B*y; y = A*y + B*x; x = u; return;
   }
   if (u < 1.0)
   {  x = 1.0 - A; u = x*x + B*B; if (mode & 1) u = -u;
      y = B/u; x /= u; u = 2*A - 1.0;
      mdouble b = 2.0*B, a = x*u - y*b; b = x*b + y*u;
      x = A*a - b*B; y = A*b + B*a; return;
   }
   if (mode < 0)
   {  mdouble xp = x + 1.0; x = A*xp + B*y; y = A*y - B*xp;
      if (mode == -1) u = -u; x /= u; y /= u; return;
   }
   if (mode & 1) { x = 1.0 + A; y = -B; } else { x = 1.0 - A; y = B; }
   u = x*x + y*y; x /= u; y /= u;
}

void mndifs::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.0; }

/* Since subtype 0 and 1 are inversion-symmetric,  |c| < 1  is inverted
   to have a clearer criterion for a root and for escape.  A root is
   suspected when  |P| < d|P'|  with  d = half pixel width;  for  |c| < 1
   the half is canceled approximately with the derivative of the inversion.
   This is more involved for subtype 2 with  |c| < 1 : inversion corresponds
   to the reverse polynomial and the computation for the numerator of the
   external angle is different.  Unfortunately,  the recursion cannot be
   stopped when a non-admissible root is found:  for  |c| < 1  the kneading
   sequence is prepended,  and for  |c| > 1  the lower sequence of a non-real
   one may have real extensions again.  E.g.,  ABAAB*  is non-admissible but
   ABAABA*  is real-admissible.
*/
uint mndifs::pixcolor(mdouble x, mdouble y)
{  Y = x*x + y*y; if (y < 0.0) y = -y;
   if (sign < 0 || Y < 0.25 || Y > 4.0) return 10;
   if (Y < 0.39 && y > 0.02) return 10; //empirical
   if (subtype & 6 && Y > 1.6 - 0.176*x && y > 0.02) return 10; //empirical
   if (!subtype && Y >= 0.5 && Y <= 2.0) return 9;
   if (Y >= 0.7072 && Y <= 1.4142)
   {  if (subtype & 1) return 12;
      if ((x > -1.09 && x < 0.325) //empirical
         || (x > 0.0 && Y <= 1.1025 + 0.2204*y*y)) return 0;
   }
   if (maxiter > 32u) return 11;
   int cl; bailout = 1.0/(1.0 + Y - 2.0*sqrt(Y));
   if (Y >= 1.0)
   {  if (subtype & 6) subtype = 2;
      X = x; Y = y; temp[0] = 0.25*temp[1];
   }
   else
   {  if (subtype & 6) subtype = 4; bailout *= Y;
      X = x/Y; Y = y/Y; temp[0] = temp [1];
   }
   if (subtype & 6)
   {  cl = recur2(0ULL, 1.0, 0.0, 0.0, 0.0, 0u);
      if (cl > 0) return 0; return 10;
   }
   cl = recur1(1.0, 0.0, 0.0, 0.0, 0u);
   if (cl > 0) return (subtype ? 12 : 9);
   if (cl) return 10; return 11;
}

int mndifs::recur1(mdouble x, mdouble y, mdouble xp, mdouble yp, uint n)
{  mdouble u = x*x + y*y;
   if (u < temp[0]*(xp*xp + yp*yp)) return 1;
   if (u >= bailout) return -1;
   if (n >= maxiter) return 0;
   u = x + X*xp - Y*yp; yp = y + X*yp + Y*xp; xp = u;
   u = X*x - Y*y; y = X*y + Y*x; x = u; n++;
   int cl1 = recur1(x + 1.0, y, xp, yp, n); if (cl1 > 0) return 1;
   int cl2 = recur1(x - 1.0, y, xp, yp, n);
   if (!subtype)
   {  if (cl2 > 0) return 1;
      if (cl2 > cl1) cl1 = cl2; cl2 = recur1(x, y, xp, yp, n);
   }
   if (cl1 > cl2) cl2 = cl1; return cl2;
}

int mndifs::recur2(qulonglong N, mdouble x, mdouble y, mdouble xp, mdouble yp,
   uint n)
{  mdouble u = x*x + y*y;
   if (u < temp[0]*(xp*xp + yp*yp)) //check admissibility
   {  uint j; qulonglong N0, N1 = N, H = 1ULL, D;
      H <<= n; D = (H << 1) - 1ULL; if (N1 >= H) N1 ^= D; N0 = N1;
      for (j = 0u; j < n; j++)
      { N1 <<= 1; if (N1 >= H) N1 ^= D; if (N1 > N0) { N1 = 0ULL; break; } }
      if (N1) return 1;
   }
   if (u >= bailout) return -1;
   if (n >= maxiter) return 0;
   if (subtype & 2) N <<= 1;
   u = x + X*xp - Y*yp; yp = y + X*yp + Y*xp; xp = u;
   u = X*x - Y*y; y = X*y + Y*x; x = u; n++;
   int cl1 = recur2(N, x + 1.0, y, xp, yp, n); if (cl1 > 0) return 1;
   qulonglong U = 1ULL; if (subtype & 4) U <<= n; N |= U;
   int cl2 = recur2(N, x - 1.0, y, xp, yp, n);
   if (cl1 > cl2) cl2 = cl1; return cl2;
}

///////////////////////////////////////////////////////////////////////

void mndlambda::f(mdouble, mdouble, mdouble &x, mdouble &y) const
{  if (x < 0.0 || y < 0.0 || x > 1.0 || y > 1.0) return;
   x *= 2.0; y *= 2.0; if (x > 1.0) x--; if (y > 1.0) y--;
}

void mndlambda::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) { f(A, B, x, y); return; }
   if (mode > 0) { x = 0.5*A; y = x + 0.5; return; }
   mdouble u = 0.5*A; x *= 0.5; y *= 0.5;
   if (mode == -1)
   { if (x < u) x += 0.5; if (y < u) y += 0.5; return; }
   if (x > u) x += 0.5; if (y > u) y += 0.5;
}

void mndlambda::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.0; }

uint mndlambda::pixcolor(mdouble x, mdouble y)
{  if (sign > 0) return 10;
   if (x < 0.0 || x > 1.0 || y < 0.0 || y > 1.0 || A< 0.0 || A> 1.0) return 9;
   uint j; mdouble u = 0.5*A, v = u + 0.5;
   for (j = 1; j <= maxiter; j++)
   {  if ((x < u || x > v) && y > u && y < v) return 10;
      if ((y < u || y > v) && x > u && x < v) return 12;
      x *= 2.0; y *= 2.0; if (x >= 1.0) x--; if (y >= 1.0) y--;
   }
   return 0;
}

///////////////////////////////////////////////////////////////////////

uint mndScale::set(int oldn, int newn, mdouble *t)
{  mdouble x0, y0; uint lsppp;
   if (newn < 0) //initialization of parameters
   {  pre = uint(oldn); cent = pre >> 16; pre &= 0xFFF; per = uint(-newn);
      ra = t[0]; ia = t[1];
      rlogrho = log(t[2]); ilogrho = t[3];
      if (oldn > 0) { rk = t[4]; ik = t[5]; } else { rk = 0.1; ik = 0; }
   }
   lsppp = pre; lsppp <<= 16; lsppp |= per; if (newn < 0) return lsppp;
   if (oldn < 0) //init. of viewport.  t[] = r/i cmid, r/i cwidth, gamma
   {  if (!cent) t[4] = 4.0;
      if (t[4] > 2.0)
      { t[0] = ra; t[1] = ia; t[2] = 1.3*sqrt(rk*rk + ik*ik); }
      else
      {  x0 = ra + rk; y0 = ia + ik; find(cent, x0, y0); lsppp = cent;
         t[0] = x0; t[1] = y0; t[2] = (2.3 - t[4])*sqrt(rk*rk + ik*ik);
      }
      t[3] = 0; return lsppp;
   }
   //else changing  n . t[] = r/i cmid, r/i cwidth, r/i c, gamma
   if (!cent) t[6] = 4.0;
   if (t[6] > 2.0) //Tan Lei scaling around  a
   {  x0 = t[4] - ra; y0 = t[5] - ia;
      expmult(oldn - newn, x0, y0); t[4] = x0 + ra; t[5] = y0 + ia;
      x0 = t[0] - ra; y0 = t[1] - ia;
      expmult(oldn - newn, x0, y0); t[0] = x0 + ra; t[1] = y0 + ia;
      x0 = t[2]; y0 = t[3]; expmult(oldn - newn, x0, y0);
      t[2] = x0; t[3] = y0;
      return lsppp;
   }
   //else scaling around  c_n
   mdouble x = rk, y = ik; expmult(-oldn, x, y); x += ra; y += ia;
   find(cent + oldn*per, x, y); //c_oldn
   mdouble X = rk, Y = ik; expmult(-newn, X, Y); X += ra; Y += ia;
   find(cent + newn*per, X, Y); //c_newn
   x0 = t[4] - x; y0 = t[5] - y; expmult(t[6]*(oldn - newn), x0, y0);
   t[4] = x0 + X; t[5] = y0 + Y;
   x0 = t[0] - x; y0 = t[1] - y; expmult(t[6]*(oldn - newn), x0, y0);
   t[0] = x0 + X; t[1] = y0 + Y;
   x0 = t[2]; y0 = t[3]; expmult(t[6]*(oldn - newn), x0, y0);
   t[2] = x0; t[3] = y0; return cent + ((uint)(newn))*per;
}//set

void mndScale::expmult(mdouble t, mdouble &x, mdouble &y)
{  //multiply  x + iy  with  rho^t
   mdouble u = cos(t*ilogrho), v = sin(t*ilogrho), w;
   w = u*x - v*y; y = u*y + v*x;
   u = exp(t*rlogrho); x = u*w; y *= u;
}

int mndScale::find(uint per, mdouble &x, mdouble &y)
{  mdouble a, b, fx, fy, px, py, w; uint i, j;
   for (i = 0; i < 30; i++)
   {  a = x; b = y; fx = 0; fy = 0; px = 0; py = 0;
      for (j = 0; j < per; j++)
      {  if (px*px + py*py > 1e100) return 2;
         w = 2*(fx*px - fy*py); py = 2*(fx*py + fy*px); px = w + 1;
         w = fx*fx - fy*fy + a; fy = 2*fx*fy + b; fx = w;
      }
      w = px*px + py*py; if (w < 1e-100) return -1;
      x -= (fx*px + fy*py)/w; y += (fx*py - fy*px)/w;
   }
   return 0;
}

/* mndynamo.cpp  by Wolf Jung (C) 2007-2023.  Defines classes:
   mndynamics, mndsiegel, mndcubesiegel, mndquartsiegel, mndexposiegel,
   mndtrigosiegel, mndexpo, mndtrigo, mndmatesiegel, mndmating, mndsingpert,
   mndherman, mndnewtonsiegel, mndnewton, mndcubicnewton, mndquarticnewton.

   These classes are part of Mandel 5.18, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/

#include <cmath>
using namespace std;
#include "mndynamo.h"

void mndynamics::root(mdouble x, mdouble y, mdouble &u, mdouble &v)
{  v = sqrt(x*x + y*y);
   if (x > 0.0) { u = sqrt(0.5*(v + x)); v = 0.5*y/u; return; }
   if (x < 0.0)
   { v = sqrt(0.5*(v - x)); if (y < 0.0) v = -v; u = 0.5*y/v; return; }
   if (y >= 0.0) { u = sqrt(0.5*y); v = u; return; }
   u = sqrt(-0.5*y); v = -u;
}

void mndynamics::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) critical(A, B, x, y);
}

void mndynamics::prepare(int sg, uint nmax, uint &dm, mdouble *t)
{  //set parameters before drawing
   sign = sg; maxiter = nmax; if (maxiter > 65000) maxiter = 65000;
   if (!maxMode(sign)) dm = 0;
   else if (dm > ((uint)(maxMode(sign))) && dm <= 15) dm = 1;
   if (dm < ((uint)(minMode(sign)))) dm = minMode(sign);
   drawmode = dm; if (drawmode == 1) drawmode = 16;
   if (drawmode > 255) //renormalization
   {  temp[0] = t[0];
      maxiter = nmax - (drawmode & 255); maxiter /= (drawmode >> 8);
      if (sg < 0) //reduce preperiod (from cr.v. to cr.pt.)
      { if (!(drawmode & 255)) drawmode += (drawmode >> 8); drawmode--; }
   }
   if (!drawmode && sign < 0) //cycles
   {  mdouble X = A, Y = B; Period = period(X, Y, 1);
      if (Period > 12 || !Period) X = 1.0 + bailout;
      temp[0] = X; temp[1] = Y;
   }
   else temp[1] = *t;
}

uint mndynamics::period(mdouble &a, mdouble &b, int cycle) // = 0
{  //determine the period, if cycle then set a, b to periodic point.
   uint j; mdouble x, y, x0, y0; critical(a, b, x, y);
   for (j = 1; j <= 1000; j++)
   { if (x*x + y*y <= bailout) f(a, b, x, y); else return 0; }
   x0 = x; y0 = y;
   for (j = 1; j <= 1024; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 0;
      if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-16)
      {  if (cycle) { a = x; b = y; }
         return j;
      }
   }
   return 10000;
}

uint mndynamics::esctime(mdouble x, mdouble y)
{  uint j; mdouble a, b, x0, y0;
   if (sign > 0) { a = x; b = y; critical(a, b, x, y); }
   else { a = A; b = B; }
   if (drawmode > 255) return renormtime(a, b, x, y);
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return j;
      if (sign < 0 && !drawmode
         && (x - temp[0])*(x - temp[0]) + (y - temp[1])*(y - temp[1]) < 1e-6)
      {  int cl = 1 + j % Period;
         if (Period <= 4 && j % (2*Period) >= Period ) cl |= 8;
         return 65280 | cl;
      }
   }
   if (drawmode) return 65280 | (drawmode & 15);
   if (sign > 0)
   {  x0 = x, y0 = y;
      for (j = 0; j < 60; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y);
         else return maxiter + j;
         if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < .000001 )
           return 65281 + (j  % 12);
      }
   }
   return 65293;
}

uint mndynamics::renormtime(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; int i, per = drawmode >> 8, preper = drawmode & 255;
   mdouble c, d; critical(a, b, c, d);
   for (i = 1; i <= preper; i++)
   { if (x*x + y*y <= bailout) f(a, b, x, y); else return 0; }
   for (j = 1; j <= maxiter; j++)
   {  if ( (x - c)*(x - c) + (y - d)*(y - d) > *temp) return j;
      for (i = 1; i <= per; i++)
      { if (x*x + y*y <= bailout) f(a, b, x, y); else return 0; }
   }
   return 65293;
}

uint mndynamics::pixcolor(mdouble x, mdouble y)
{  uint j, cl; mdouble a, b;
   if (sign > 0) { a = x; b = y; critical(a, b, x, y); }
   else { a = A; b = B; }
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y);
      else
      {  if (drawmode == 2)
         { cl = ((j % 24) >> 3); if (!cl) cl = 4; if (j & 1) cl |= 8; }
         else cl = j;
         return cl;
      }
   }
   return 0;
}

////////////////////////////////////////////////////////////////////

void mndsiegel::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode <= 0) return;
   which = mode; critical(A, B, x, y);
}

//wrong for persistently attracting or parabolic != 0
uint mndsiegel::period(mdouble &a, mdouble &b, int cycle) // = 0
{  //determine period, may set a, b to per. pt.. Set which to active cr.pt.
   uint j; int point; mdouble x, y, x0, y0; critical(a, b, x, y);
   for (point = 0; point <= 1; point ++)
   {  if (point) { which ^= 3; critical(a, b, x, y); }
      for (j = 1; j <= 1000; j++)
      { if (x*x + y*y <= bailout) f(a, b, x, y); else return 0; }
      x0 = x; y0 = y;
      /*if (x*x + y*y > 0.001)*/ for (j = 1; j <= 1024; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 0;
         if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-8)
         {  if (cycle) { a = x; b = y; }
            return j;
         }
      }
   }
   return 10000;
}

uint mndsiegel::esctime(mdouble x, mdouble y)
{  if (sign < 0) return mndynamics::esctime(x, y);
   uint j; int point; mdouble x0, y0, a = x, b = y;
   critical(a, b, x, y);
   if (drawmode > 255) return renormtime(a, b, x, y);
   for (point = 0; point <= 1; point++)
   {  if (point) { which ^= 3; critical(a, b, x, y); }
      for (j = 1; j <= maxiter; j++)
      { if (x*x + y*y <= bailout) f(a, b, x, y); else return j; }
      if (point && drawmode) return 65280 | (drawmode & 15);
      x0 = x; y0 = y;
      if (! drawmode /*&& x*x + y*y > 1e-3*/) for (j = 0; j < 60; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y);
         else return maxiter + j;
         if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-6)
            return 65281 + (j  % 12);
      }
   }
   return 65293;
}

uint mndsiegel::pixcolor(mdouble x, mdouble y)
{  if (sign < 0) return mndynamics::pixcolor(x, y);
   uint j, cl, point; mdouble a = x, b = y;
   critical(a, b, x, y);
   for (point = 0; point <= 1; point++)
   {  if (point) { which ^= 3; critical(a, b, x, y); }
      for (j = 1; j <= maxiter; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y);
         else
         {  if (drawmode == 2)
            { cl = ((j % 24) >> 3); if (!cl) cl = 4; if (j & 1) cl |= 8; }
            else cl = j;
            return cl;
         }
      }
   }
   return 0;
}

////////////////////////////////////////////////////////////////////

void mndcubicsiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (subtype == 2) { f2(a, b, x, y); return; }
   mdouble u = x*x - y*y + a*x - b*y + L, v = 2*x*y + a*y + b*x + M,
   w = x*u - y*v;  y = x*v + y*u; x = w;
}  //z^3 + cz^2 + lambda z

void mndcubicsiegel::f2(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble rb, ib;
   root(a*a - b*b - a + 0.25*(1 - L), 2*a*b - b - 0.25*M, rb, ib);
   mdouble u = x*x - y*y, v = 2*x*y, X = a*x - b*y + rb, Y = a*y + b*x + ib,
     w = X*u - Y*v - (a + 1)*x + b*y - rb;
   y = X*v + Y*u - (a + 1)*y - b*x - ib; x = w;
}  //cz^3 + bz^2 - (c+1)z -b , b^2 = (c - .5)^2 - lambda/4

void mndcubicsiegel::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (subtype == 1)
   {  root(-a*a + b*b + 3*L, -2*a*b + 3*M, y, x); //factor i
      if (which & 1) x = -x; else y = -y;
      x -= a; y -= b; x /= 3.0; y /= 3.0; return;
   }
   mdouble rb, ib, u, v;
   root(a*a - b*b - a + 0.25*(1 - L), 2*a*b - b - 0.25*M, rb, ib);
   root(4*(a*a - b*b) + 2*a + 0.25*(1 - L), 8*a*b + 2*b - 0.25*M, u, v);
   if (which & 1) { u = -u; v = -v; }
   u -= rb; v -= ib; rb = 3*(a*a + b*b); if (rb < 1e-20) return;
   x = (a*u + b*v)/rb; y = (a*v - b*u)/rb;
}

void mndcubicsiegel::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { L = xmid; M = rewidth; return; }
   if (subtype == 1) { xmid = 0; rewidth = 4.2; }
   else { xmid = 0; if (sign > 0) rewidth = 1.5; else rewidth = 2.3; }
}

int mndcubicsiegel::bifurcate(mdouble t, mdouble &a, mdouble &b) const
{  if (subtype != 1) return 0;
   if (t <= -1.0) return 1;
   t *= (2*PI); mdouble l = cos(t) + L - 2.0, m = sin(t) + M, s = b;
   root(l, -m, a, b); t = sqrt(l*l + m*m); if (t <= 0) return 0;
   a /= t; b /= t; l += (L - 1.0); m += M;
   t = a*l - b*m; b = a*m + b*l; a = t; if (s*b < 0) { a = -a; b = -b; }
   return 1;
}

////////////////////////////////////////////////////////////////////

void mndquartsiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble Ar, Ai, ur, ui;
   if (subtype <= 2) { Ar = a; Ai = b; ur = subtype - 1; ui = 0; }
   if (subtype >= 3) { ur = a; ui = b; }
   if (subtype == 3) { Ar = 3*ur - L; Ai = 3*ui - M; }
   if (subtype == 4)
   {  mdouble r = ur*ur + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      mdouble m = ur/r - 3, n = -ui/r,
      w = 0.25*(L*m - M*n + 6*ur); n = 0.25*(L*n + M*m + 6*ui); m = w;
      r = (ur-0.5)*(ur-0.5) + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      Ar = ((ur - 0.5)*m + ui*n)/r; Ai = ((ur - 0.5)*n - ui*m)/r;
   }
   if (subtype >= 5)
   {  mdouble r = ur*ur + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      Ar = 2 + ((1 - L)*ur - M*ui)/r; Ai = ((L - 1)*ui - M*ur)/r;
   }
   if (subtype == 6)
   {  mdouble m = ur*ur - ui*ui, n = 2*ur*ui,
      r = (ur - 1)*m - ui*n; n = (ur - 1)*n + ui*m; m = r;
      r = m*m + n*n; if (r < 1e-10) { x = 1e100; return; }
      Ar -= m/r; Ai += n/r;
   }
   mdouble u = x*x - y*y, v = 2*x*y, m = L - 2*(Ar+ur), n = M - 2*(Ai+ui),
   w = Ar*u - Ai*v + m*x - n*y + Ar + 3*ur - 2*L;
   n = Ar*v + Ai*u + m*y + n*x + Ai + 3*ui - 2*M; m = w;
   w = m*u - n*v + L*x - M*y; y = m*v + n*u + L*y + M*x; x = w;
}  //Az^4 + (lambda-2A-2u)z^3 + (A+3u-2lambda)z^2 + lambda z

void mndquartsiegel::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble Ar, Ai, ur, ui;
   if (subtype <= 2) { Ar = a; Ai = b; ur = subtype - 1; ui = 0; }
   if (subtype >= 3) { ur = a; ui = b; }
   if (subtype == 3) { Ar = 3*ur - L; Ai = 3*ui - M; }
   if (subtype == 4)
   {  mdouble r = ur*ur + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      mdouble m = ur/r - 3, n = -ui/r,
      w = 0.25*(L*m - M*n + 6*ur); n = 0.25*(L*n + M*m + 6*ui); m = w;
      r = (ur-0.5)*(ur-0.5) + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      Ar = ((ur - 0.5)*m + ui*n)/r; Ai = ((ur - 0.5)*n - ui*m)/r;
   }
   if (subtype >= 5)
   {  mdouble r = ur*ur + ui*ui; if (r < 1e-10) { x = 1e100; return; }
      Ar = 2 + ((1 - L)*ur - M*ui)/r; Ai = ((L - 1)*ui - M*ur)/r;
   }
   if (subtype == 6)
   {  mdouble m = ur*ur - ui*ui, n = 2*ur*ui,
      r = (ur - 1)*m - ui*n; n = (ur - 1)*n + ui*m; m = r;
      r = m*m + n*n; if (r < 1e-10) { x = 1e100; return; }
      Ar -= m/r; Ai += n/r;
   }
   mdouble u = 3*ur + Ar - 1.5*L, v = 3*ui + Ai - 1.5*M; Ar *= 4; Ai *= 4;
   root(u*u - v*v + Ar*L - Ai*M, 2*u*v + Ar*M + Ai*L, x, y);
   if (which & 1) {x = -x; y = -y; }
   x += u; y += v;
   mdouble r = Ar*Ar + Ai*Ai; if (r < 1e-10) { x = 1e100; return; }
   u = (Ar*x + Ai*y)/r; y = (Ar*y - Ai*x)/r; x = u;
}

void mndquartsiegel::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { L = xmid; M = rewidth; return; }
   if (sg < 0)
   {  xmid = 0;
      if (subtype == 1) rewidth = 1.8;
      if (subtype == 2) rewidth = 1.7;
      if (subtype == 3) rewidth = 2.6;
      if (subtype == 4) rewidth = 2.3;
      if (subtype == 5) rewidth = 1.9;
      if (subtype == 6) rewidth = 2.1;
      return;
   }
   if (subtype == 1) { xmid = 8.2; rewidth = 13.2; }
   if (subtype == 2) { xmid = 1; rewidth = 11.8; }
   if (subtype == 3) { xmid = .42; rewidth = 1.06; }
   if (subtype == 4) { xmid = .3; rewidth = 2; }
   if (subtype == 5) { xmid = .4; rewidth = 2.15; }
   if (subtype == 6) { xmid = .25; rewidth = 2.15; }
}

/////////////////////////////////////////////////////////////////////

void mndexposiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{ if (x > 200) { x = 1e100; return; }
  mdouble u = a + L, v = b + M, m = u*x - v*y - a, n = u*y + v*x - b;
  x = exp(x); u = x*cos(y); v = x*sin(y);
  x = m*u - n*v + a; y = m*v + n*u + b;
}

void mndexposiegel::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (which & 1) { x = a; y = b; return; }
   x = L + a; y = M + b;
   a = x*x + y*y; if (a < 1e-10) { x = 1e100; return; }
   b = -(L*x + M*y)/a; y = (L*y - M*x)/a; x = b;
}

void mndexposiegel::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { L = xmid; M = rewidth; return; }
   xmid = 0; rewidth = 5.0; if (sg < 0) rewidth = 2.7;
}

//////////////////////////////////////////////////////////////////////

void mndtrigosiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (fabs(y) > 200) { x = 1e100; y = 1e100; return; }
   mdouble s = sin(x), c = cos(x), S = sinh(y), C = cosh(y);
   x = C*(L*s - a*c) - S*(M*c + b*s) + a;
   y = C*(M*s - b*c) + S*(L*c + a*s) + b;
}

void mndtrigosiegel::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  root(a*a - b*b + L*L - M*M, 2*(a*b + L*M), x, y);
   if (which & 1) { x = -x; y = -y; }
   x += a; y += b;
}

void mndtrigosiegel::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { L = xmid; M = rewidth; return; }
   xmid = 0; rewidth = 6.78; if (sg < 0) rewidth = 4.84;
}

/////////////////////////////////////////////////////////////////////

void mndexpo::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (x > 200) { x = 1e100; return; }
   mdouble w = exp(x), u = w*cos(y), v = w*sin(y);
   if (subtype & 2) { w = a*x - b*y; b = a*y + b*x; a = w; }
   w = a*u - b*v; y = a*v + b*u; x = w;
}

void mndexpo::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  xmid = 0; rewidth = 8.1;
   if (sg < 0 && (subtype & 1) ) rewidth = 4.8;
   if (sg > 0 && (subtype & 2) ) { xmid = 7.1; rewidth = 20.5; }
}

//////////////////////////////////////////////////////////////////////

void mndtrigo::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u, v; if (fabs(y) > 200) { x = 1e100; y = 1e100; return; }
   if (subtype == 1)
   {  u = sin(x)*cosh(y); v = cos(x)*sinh(y);
      x = a*u - b*v; y = a*v + b*u; return;
   }
   u = cos(x)*cosh(y); v = -sin(x)*sinh(y);
   if (subtype == 6)
   {  mdouble U = 1 - cos(a)*cosh(b), V = sin(a)*sinh(b), W = U*U + V*V;
      if (W < 1e-10) { x = 1e100; return; }
      u--; x = (u*U + v*V)/W + 1; v = (v*U - u*V)/W; u = x;
      x = a*u - b*v; y = a*v + b*u;
   }
   if (subtype == 3)
   { x = PI*u + a; y = PI*v + b; return; }
   if (subtype == 4) u++;
   if (subtype == 5) u--;
   x = a*u - b*v; y = a*v + b*u;
}

void mndtrigo::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  xmid = 0; rewidth = 4.84;
   if (sg > 0 && subtype == 1) rewidth = 6.0;
   if (sg > 0 && subtype == 4) rewidth = 4.0;
}

//////////////////////////////////////////////////////////////////////

void mandelMateSiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble w = x*x + y*y; if (w < 1e-10) { x = 1e100; return; }
   x += x/w; y -= y/w; w = L*x - M*y + a; y = L*y + M*x + b; x = w;
}// lambda(z + 1/z) + c , lambda -> 1/lambda before

void mandelMateSiegel::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg)                                                     //?????
   {  mdouble w = xmid*xmid + rewidth*rewidth;
      if (w > 1e-10) { L = xmid/w; M = -rewidth/w; return; }
   }
   xmid = 0; rewidth = 15.5; if (sg < 0) rewidth = 6.5;
}

//////////////////////////////////////////////////////////////////////

void mandelMating::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (mode > 0) { x = 0; y = 0; return; }
   mdouble w = x*x + y*y, a = A, b = B; uint upper = 0;
   if (0.99999 < w && w < 1.00001)
   {  if (!mode) { w = x*x - y*y; y *= 2.0*x; x = w; }
      else root(x, y, x, y);
      if (mode == -1) { x = -x; y = -y; }
      return;
   }
   if (w > 1.0) { upper = 6; a = C; b = D; w = 1.0/w; x *= w; y *= -w; }
   w = 1.0/sqrt(1.0 - w); x *= w; y *= w;
   if (!mode) { w = x*x - y*y + a; y = 2.0*x*y + b; x = w; }
   else root(x - a, y - b, x, y);
   if (mode == -1) { x = -x; y = -y; }
   w = 1.0/sqrt(1.0 + x*x + y*y); x *= w; y *= w;
   if (upper)
   {  w = x*x + y*y;
      if (w > 1e-100) { w = 1.0/w; x *= w; y *= -w; } else x = 1e100;
   }
}

void mandelMating::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { C = xmid; D = rewidth; return; }
   xmid = 0; rewidth = 2.5;
}

uint mandelMating::pixcolor(mdouble x, mdouble y)
{  if (sign > 0) return martyM(x, y, x, y, temp[1], 1);
   mdouble pw = temp[1], w = x*x + y*y, a = A, b = B;
   uint upper = 0, cl; if (w > 1.0) upper = 1;
   if (0.8 <= w && w <= 1.25) cl = 13;
   else
   {  pw /= (w*w - 2.0*w + 1.0); //compensate phi and inversion
      if (w > 1.0) { upper = 1; a = C; b = D; w = 1.0/w; x *= w; y *= -w; }
      w = 1.0/sqrt(1.0 - w); x *= w; y *= w;
      cl = martyM(a, b, x, y, pw);
   }
   if (upper) cl = (cl & 4 ? 11 : 10);
   if (drawmode == 4 && cl > 10) return 4;
   if (drawmode == 6 && cl <= 10) cl = (cl & 1 ? 13 : 11);
   return cl;
}

uint mandelMating::martyM(mdouble a, mdouble b, mdouble x, mdouble y,
   mdouble pw, int sg) //static for other classes
{  uint j; mdouble xp = 1.0, yp = 0, nz, nzp; //zp = 1
   for (j = 1; j <= /*maxiter*/150; j++)
   {  nz = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = nz; if (sg > 0) xp++;
      nz = x*x - y*y + a; y = 2*x*y + b; x = nz;
      nz = x*x + y*y; nzp = xp*xp + yp*yp;
      if (nzp > 1e40 || nz > 1e3) break;
      nz++; if (pw*nzp > 2.0*nz*nz) return 9; //before 5.0
   }
   if (nz > 16.0) return 11;
   return 9;
}

//////////////////////////////////////////////////////////////////////

void mandelBitransitive::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a - rb, v = b - ib, N = x*x - y*y + rb; y = 2.0*x*y + ib; x = N;
   N = x*x + y*y; if (N < 1e-100) { x = 1e100; return; } x /= N; y /= N;
   N = u*x + v*y; y = v*x - u*y; x = N + 1.0;
}// (z^2 + c)/(z^2 + B)

void mandelBitransitive::inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
   mdouble &x, mdouble &y) const
{  mdouble u = a - rb, v = b - ib, w; // u + iv = A - B
   X--; w = X*X + Y*Y; if (w < 1e-100) { x = 1e100; return; }
   root((u*X + v*Y)/w - rb, (v*X - u*Y)/w - ib, x, y);
}// sqrt( (A - Bz) / (z - 1) ) = sqrt( (A - Bz) / (z - 1) )

void mandelBitransitive::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0) return;
   inverse(A, B, x, y, x, y); if (mode == -2) { x = -x; y = -y; }
}

void mandelBitransitive::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (!sg) { rb = xmid; ib = rewidth; return; }
   xmid = 0; rewidth = 3.0; if (sg > 0) rewidth = 4.5;
}

int mandelBitransitive::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  if (rb*rb + ib*ib > 0.0) return 0;
   int per = 1; if (a*a + b*b >= 16.0) per = 2;
   if (t <= -1.0) return per;
   t *= (2.0*PI);
   if (per == 2) { a = 4.0*cos(t); b = -4.0*sin(t); return 2; }
   mdouble u = 2.0 + cos(t), v = -sin(t); t = 2.0/(u*u + v*v); u *= t; v *= t;
   a = u*u - v*v; b = 2.0*u*v; t = a*u - b*v - a; b = a*v + b*u - b; a = t;
   return 1; // w = 2/(2 + rho) , c = w^3 - w^2
} //bifurcate

uint mandelBitransitive::pixcolor(mdouble x, mdouble y)
{  if (sign < 0) return marty(A, B, x, y);
   uint cl1 = marty(x, y, 1.0, 0); if (rb*rb + ib*ib <= 0.0) return cl1;
   //bifu.locus of 0 blue, infinity red, intersection magenta, exterior is green
   uint cl2 = marty(x, y, 0.0, 0); if (!(cl1 & 1)) return cl2;
   if (cl2 & 1) return 5; else return 12;
}

uint mandelBitransitive::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = (sign > 0 ? 0 : 1.0), yp = 0, u, v, w;
   a -= rb; b -= ib; // c - b
   //mdouble t = 1.0; if (maxiter & 1) t *= (1.0+x*x+y*y)*(1.0+x*x+y*y)*0.25;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x - y*y + rb; v = -(2*x*y + ib); w = u*u + v*v;
      if (w > 1e100 || w < 1e-100) return 10;
      u /= w; v /= w; // 1/(z^2 + B)
      w = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = w;
      w = b*yp - a*xp; yp = -(a*yp + b*xp); xp = w; // 2(b - c)*z*zp
      x = u*u - v*v; y = 2*u*v;
      w = x*xp - y*yp; yp = x*yp + y*xp; xp= w; // "" /= (z^2 + b)^2
      if (sign > 0) { xp += u; yp += v; }
      x = a*u - b*v + 1.0; y = a*v + b*u;
      u = xp*xp + yp*yp; v = x*x + y*y + 1.0;
      if (u > 1e100 || v > 1e100) return 10;
      if (temp[1]*u > 5.0*v*v) return 9;
      //if (t*temp[1]*u > 5.0*v*v) return 9; //test spherical compensation
   }
   return 10;
}

//////////////////////////////////////////////////////////////////////

void mandelSymmetric::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u, v, N = x*x - y*y; y *= 2.0*x; x = N;
   u = a*x - b*y + 1.0; v = a*y + b*x; x += a; y += b;
   N = u*u + v*v; if (N < 1e-100) { x = 1e100; return; } x /= N; y /= N;
   N = u*x + v*y; y = u*y - v*x; x = N;
}// (z^2 + c)/(1 + cz^2)

void mandelSymmetric::inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
   mdouble &x, mdouble &y) const
{  x = a*X - b*Y - 1.0; y = a*Y + b*X; mdouble w = x*x + y*y;
   if (w < 1e-100) { x = 1e100; return; }
   a -= X; b -= Y;
   root((a*x + b*y)/w, (b*x - a*y)/w, x, y);
}// sqrt( (z - c) / (1 - cz) )

void mandelSymmetric::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0) return;
   inverse(A, B, x, y, x, y); if (mode == -2) { x = -x; y = -y; }
}

void mandelSymmetric::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 3.0; if (sg > 0) { xmid = 1.0; rewidth = 2.5; } }

int mandelSymmetric::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  int per = 1; mdouble u, v, w = a - 1.0; if (w*w + b*b >= 4.0) per = 2;
   if (t <= -1.0) return per;
   if (per == 2)
   { t *= PI; a = 1.0 + 2.0*cos(t); b = -2.0*sin(t); return 2; }
   t *= (2.0*PI); u = 1.0 + 2.0*cos(t); v = 2.0*sin(t);
   w = u*u + v*v; u /= w; v /= w; // rho / (2 + rho)
   if (a < 1.0/3.0) { a = u; b = v; }
   else { a = 1.0 - 2.0*u; b = -2.0*v; }
   return 1;
} //bifurcate

uint mandelSymmetric::pixcolor(mdouble x, mdouble y)
{ if (sign < 0) return marty(A, B, x, y); else return marty(x, y, 0.0, 0.0); }

uint mandelSymmetric::marty(mdouble a, mdouble b, mdouble x, mdouble y) //!!!!!!!!!!!
{  uint j; mdouble u = -2.0*a, v = -2.0*b, rb = a, ib = b, N; // u + iv = A - B
 
  mdouble X, Y, xp = 1.0, yp = 0.0, up = -2.0, vp = 0.0, rbp = 1.0, ibp = 0.0;
   if (sign > 0)
   {  xp = 0.0;
      if (subtype == 4) { up = -a - 1.0; vp = -b; }

      if (subtype == 8) up = -1.0;
   }
   for (j = 1; j <= maxiter; j++)
   {  X = x*x - y*y + rb; Y = -2.0*x*y - ib; N = X*X + Y*Y;
      if (N > 1e100 || N < 1e-100) return 10; X /= N; Y /= N; // 1/(z^2 + B)
      N = 2.0*(y*yp - x*xp); yp = -2.0*(x*yp + y*xp); xp = N;
      if (sign > 0) { xp -= rbp; yp -= ibp; }
      N = u*xp - v*yp; yp = u*yp + v*xp; xp = N;
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      if (sign > 0) { xp += up; yp += vp; }
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      x = 1.0 + u*X - v*Y; y = u*Y + v*X;
      X = xp*xp + yp*yp; Y = x*x + y*y + 1.0;
      if (X > 1e100 || Y > 1e100) return 10;
      if (temp[1]*X > 5.0*Y*Y) return 9;
   }
   return 10;
}

//////////////////////////////////////////////////////////////////////

void mandelMateTwo::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a + 1.0, v = b, w = x*x - y*y - 1.0; y = 2.0*x*y; x = w;
   w = x*x + y*y; if (w < 1e-100) { x = 1e100; return; } x /= w; y /= w;
   w = u*x + v*y; y = v*x - u*y; x = w + 1.0;
}// (z^2 + c)/(z^2 - 1)

void mandelMateTwo::inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
   mdouble &x, mdouble &y) const
{  a++; X--; mdouble w = X*X + Y*Y; if (w < 1e-100) { x = 1e100; return; }
   root(1.0 + (a*X + b*Y)/w, (b*X - a*Y)/w, x, y);
}// sqrt( (z + c) / (z - 1) )

void mandelMateTwo::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0) return;
   inverse(A, B, x, y, x, y); if (mode == -2) { x = -x; y = -y; }
}

void mandelMateTwo::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 3.0; if (sg > 0) rewidth = 2.5; }

int mandelMateTwo::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  if (t <= -1.0) return 1;
   t *= (2*PI); mdouble u = 1.0 + 2.0*cos(t), v = 2.0*sin(t);
   t = -1.0/(u*u + v*v); u *= t; v *= t; //w = -rho/(rho + 2), c = w^3 - w^2 - w
   a = u*u - v*v; b = 2.0*u*v;
   t = u*a - v*b - a - u; b = u*b + v*a - v - b; a = t;
   return 1;
} //bifurcate

uint mandelMateTwo::esctime(mdouble x, mdouble y) //escape locus grayscale
{  if (sign > 0 || drawmode || Period > 6) return mndynamics::esctime(x, y);
   uint j;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(A, B, x, y);
      else
      { if (j & 1) return 65280 | (drawmode & 15); else return 65288; }
      if ((x - temp[0])*(x - temp[0]) + (y - temp[1])*(y - temp[1]) < 1e-6)
      {  int cl = 1 + j % Period;
         if (Period <= 4 && j % (2*Period) >= Period ) cl |= 8;
         return 65280 | cl;
      }
   }
   return 65293;
}

uint mandelMateTwo::pixcolor(mdouble x, mdouble y)
{  if (subtype < 4 && sign < 0)
   {  if (subtype == 1) return capture(x, y); uint cl;
      if (subtype == 2) cl = mating(x, y); else cl = anti(x, y); //subtype 3
      if (cl == 9) cl = 1;
      if (drawmode == 4 && (cl == 11 || cl == 13)) return 15;
      if (drawmode == 6) { if (cl == 12) return 13; if (cl == 1) return 11; }
      return cl;
   }
   if (drawmode == 4)
   { if (sign < 0) return marty(A, B, x, y); else return marty(x, y, 0.0, 0.0); }
   uint j; mdouble a, b;
   if (sign > 0) { a = x; b = y; } else { a = A; b = B; }
   for (j = 1; j <= maxiter; j++)
   {  f(a, b, x, y);
      if (x*x + y*y > bailout)
      {  if (drawmode & 1) return j; // == 3
         return 1 + (j & 1) + 4*(j & 2);
      }
   }
   return 0;
}

uint mandelMateTwo::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = (sign > 0 ? 0 : 1.0), yp = 0, u, v, w;
   a++;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x - y*y - 1.0; v = -2*x*y; w = u*u + v*v;
      if (w > 1e100 || w < 1e-100) return 10;
      u /= w; v /= w; // 1/(z^2 + B)
      w = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = w;
      w = b*yp - a*xp; yp = -(a*yp + b*xp); xp = w; // 2(b - c)*z*zp
      x = u*u - v*v; y = 2*u*v;
      w = x*xp - y*yp; yp = x*yp + y*xp; xp= w; // "" /= (z^2 + b)^2
      if (sign > 0) { xp += u; yp += v; }
      x = a*u - b*v + 1.0; y = a*v + b*u;
      u = xp*xp + yp*yp; v = x*x + y*y + 1.0;
      if (u > 1e100 || v > 1e100) return 10;
      if (temp[1]*u > 5.0*v*v) return 1;
   }
   return 0;
}

uint mandelMateTwo::capture(mdouble x, mdouble y)
{  int j, n = pathInfo->n; if (n <= 0) return 0;
   for (j = 0; j < n; j++)
   {  f(pathInfo->rc[n-j], pathInfo->ic[n-j], x, y);
      if (x*x + y*y > bailout) return 10 - (j & 1);
   }
   for (j = n; j < maxiter; j++)
   {  f(pathInfo->rc[0], pathInfo->ic[0], x, y);
      if (x*x + y*y > bailout) return 10 - (j & 1);
   }
   return 0;
}

uint mandelMateTwo::mating(mdouble x, mdouble y)
{  if (pathInfo->n <= 0) return 0;
   uint j; mdouble u, v, w, pw = pathInfo->coeff[0]; pw *= pw;
   for (j = pathInfo->n; j > 0; j--)
   {  u = pathInfo->rc[j] + 1.0; v = pathInfo->ic[j];
      pw *= 4.0*(x*x + y*y)*(u*u + v*v);
      w = x*x - y*y - 1.0; y = 2.0*x*y; x = w;
      w = x*x + y*y; if (w < 1e-100) { return 12; } x /= w; y /= w; pw /= w*w;
      w = u*x + v*y; y = v*x - u*y; x = w + 1.0;
      w = x*x + y*y; if (w > 1e10) return 12; if (pw > 1e100) return 13;
   }
   if (w * pathInfo->coeff[0] < 1.0) //lower
   {  u = pathInfo->coeff[1]; v = pathInfo->coeff[2];
      w = u*x - v*y; v = u*y + v*x; u = w;
      x = 1.0 - pathInfo->coeff[3]*x; y *= pathInfo->coeff[3];
      w = x*x + y*y; x /= w; y /= w; w = u*x - v*y; y = u*y + v*x; x = w;
      return mandelMating::martyM(
	 pathInfo->rc[0], pathInfo->ic[0], x, y, pw*temp[1]);
   }
   w = x*x + y*y; x /= w; y /= w; //signs ignored
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y; w = u + v;
      if (w > 4.0) return 13; if (w < 0.1) return 12;
      y *= 2.0*x; x = u - v - 1.0;
   }
   return 12;
}

uint mandelMateTwo::anti(mdouble x, mdouble y)
{  int clx, j, n = pathInfo->n; if (n <= 0) return 0;
   mdouble u, v, RS = pathInfo->coeff[0], omt = pathInfo->coeff[3],
     rp = pathInfo->rc[0], ip = pathInfo->ic[0],
     w = rp*rp + ip*ip, ri = rp/w, ii = -ip/w;
   for (j = 0; j < n; j++)
   {  f(pathInfo->rc[n-j], pathInfo->ic[n-j], x, y);
      if (x*x + y*y > bailout) return 9 + 3*(j & 1);
   }
   clx = 11 + 2*(n & 1); n = 1;
   if (x*x + y*y > RS) //neglecting q^2/3
   {  n = 0; clx ^= 6;
      u = RS + omt*ip; v = omt*ip; x -= omt;
      w = x*x + y*y; if (w < 1e-100) return 0; x /= w; y /= -w;
      w = u*x - v*y; y = u*y + v*x; x = w;     
   }
   else
   {  u = rp*omt/RS; v = ip*omt/RS; w = u*x - v*y; v = u*y + v*x; u = w;
      x += u; y += v; u++; w = u*u + v*v;
      if (w < 1e-100) return 0; u /= w; v /= -w;
      w = u*x - v*y; y = u*y + v*x; x = w;
   }
   for (j = n; j < maxiter; j++)
   {  u = x*x - y*y; v = 2.0*x*y;
      if (j & 1) { u--; x = rp*u - ip*v; y = rp*v + ip*u; }
      else { x = ri*u - ii*v + 1.0; y = ri*v + ii*u; }
      if (x*x + y*y > 16.0) return clx;
   }
   return (clx & 2 ? 9 : 12);
}

//////////////////////////////////////////////////////////////////////

void mandelMateThree::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a*a - b*b, v = 2.0*a*b, N = x*x - y*y - u; y = 2.0*x*y - v; x = N;
   N = x*x + y*y; if (N < 1e-100) { x = 1e100; return; } x /= N; y /= N;
   u--; a++; N = a*u - b*v; v = a*v + b*u; u = N;
   N = u*x + v*y; y = v*x - u*y; x = N + 1.0;
}// (z^2 + c^3 - c - 1)/(z^2 - c^2)

void mandelMateThree::inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
   mdouble &x, mdouble &y) const
{  mdouble rb = a*a - b*b, ib = 2.0*a*b, u = a*rb - b*ib + rb - a - 1.0,
      v = a*ib + b*rb + ib - b, w; rb = -rb; ib = -ib;
   X--; w = X*X + Y*Y; if (w < 1e-100) { x = 1e100; return; }
   root((u*X + v*Y)/w - rb, (v*X - u*Y)/w - ib, x, y);
}// sqrt( (A - Bz) / (z - 1) ) = sqrt( (A - Bz) / (z - 1) )

void mandelMateThree::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0) return;
   inverse(A, B, x, y, x, y); if (mode == -2) { x = -x; y = -y; }
}

void mandelMateThree::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 5.0; if (sg > 0) { xmid = 0.5; rewidth = 2.5; } }

int mandelMateThree::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  if (t <= -1.0) return 2;
   t *= (2*PI); mdouble u = 4.0 + cos(t), v = -sin(t); t = 2.0/(u*u + v*v);
   u = u*t - 1.0; v *= t; //w = 2/(4 + rho) - 1 , c^2  -2wc + 1 = 0
   root(u*u - v*v -1.0, 2.0*u*v, a, b); if (b < 0) { a = -a; b = -b; }
   a += u; b += v;
   return 1;
} //bifurcate

uint mandelMateThree::esctime(mdouble x, mdouble y) //escape locus grayscale
{  if (sign > 0 || drawmode || Period > 6) return mndynamics::esctime(x, y);
   uint j;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(A, B, x, y);
      else
      { if (j & 1) return 65280 | (drawmode & 15); else return 65288; }
      if ((x - temp[0])*(x - temp[0]) + (y - temp[1])*(y - temp[1]) < 1e-6)
      {  int cl = 1 + j % Period;
         if (Period <= 4 && j % (2*Period) >= Period ) cl |= 8;
         return 65280 | cl;
      }
   }
   return 65293;
}

uint mandelMateThree::pixcolor(mdouble x, mdouble y)
{  if (subtype < 4 && sign < 0)
   {  if (subtype == 1) return capture(x, y);
      uint cl = mating(x, y);
      if (drawmode == 4 && cl > 10) return 4;
      if (drawmode == 6 && cl <= 10) cl = (cl & 1 ? 13 : 11);
      return cl;
   }
   if (drawmode == 4)
   { if (sign < 0) return marty(A, B, x, y); else return marty(x, y, 0.0, 0.0); }
   uint j, cl = 0; mdouble a, b;
   if (sign > 0) { a = x; b = y; x = 0; y = 0; } else { cl = 2; a = A; b = B; }
   for (j = cl; j <= maxiter; j++)
   {  f(a, b, x, y);
      if (x*x + y*y > bailout)
      {  if (drawmode & 1) return j; // == 3
         cl = (j - 1) % 3; if (!cl) cl = 4; return cl + 8*(j & 1);
      }
   }
   return 0;
}

uint mandelMateThree::marty(mdouble a, mdouble b, mdouble x, mdouble y) //!!!!!!!!!!
{  uint j; mdouble u = -2.0*a, v = -2.0*b, rb = a, ib = b, N; // u + iv = A - B
 
  mdouble X, Y, xp = 1.0, yp = 0.0, up = -2.0, vp = 0.0, rbp = 1.0, ibp = 0.0;
   if (sign > 0)
   {  xp = 0.0;
      if (subtype == 4) { up = -a - 1.0; vp = -b; }

      if (subtype == 8) up = -1.0;
   }
   for (j = 1; j <= maxiter; j++)
   {  X = x*x - y*y + rb; Y = -2.0*x*y - ib; N = X*X + Y*Y;
      if (N > 1e100 || N < 1e-100) return 10; X /= N; Y /= N; // 1/(z^2 + B)
      N = 2.0*(y*yp - x*xp); yp = -2.0*(x*yp + y*xp); xp = N;
      if (sign > 0) { xp -= rbp; yp -= ibp; }
      N = u*xp - v*yp; yp = u*yp + v*xp; xp = N;
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      if (sign > 0) { xp += up; yp += vp; }
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      x = 1.0 + u*X - v*Y; y = u*Y + v*X;
      X = xp*xp + yp*yp; Y = x*x + y*y + 1.0;
      if (X > 1e100 || Y > 1e100) return 10;
      if (temp[1]*X > 5.0*Y*Y) return 9;
   }
   return 10;
}
/*void mandelMateThree::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a*a - b*b, v = 2.0*a*b, N = x*x - y*y - u; y = 2.0*x*y - v; x = N;
   N = x*x + y*y; if (N < 1e-100) { x = 1e100; return; } x /= N; y /= N;
   u--; a++; N = a*u - b*v; v = a*v + b*u; u = N;
   N = u*x + v*y; y = v*x - u*y; x = N + 1.0;
}// (z^2 + c)/(z^2 - c^2)
{  uint j; mdouble xp = (sign > 0 ? 0 : 1.0), yp = 0, u, v, w;
   a++;
   for (j = 1; j <= maxiter; j++)
   {  u = x*x - y*y - 1.0; v = -2*x*y; w = u*u + v*v;
      if (w > 1e100 || w < 1e-100) return 10;
      u /= w; v /= w; // 1/(z^2 + B)
      w = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = w;
      w = b*yp - a*xp; yp = -(a*yp + b*xp); xp = w; // 2(b - c)*z*zp
      x = u*u - v*v; y = 2*u*v;
      w = x*xp - y*yp; yp = x*yp + y*xp; xp= w; // "" /= (z^2 + b)^2
      if (sign > 0) { xp += u; yp += v; }
      x = a*u - b*v + 1.0; y = a*v + b*u;
      u = xp*xp + yp*yp; v = x*x + y*y + 1;
      if (u > 1e100 || v > 1e100) return 10;
      if (temp[1]*u > 1.44*v*v) return 1;
   }
   return 0;
}//*/

uint mandelMateThree::capture(mdouble x, mdouble y)
{  uint cl; int j, n = pathInfo->n; if (n <= 0) return 0;
   for (j = 0; j < n; j++)
   {  f(pathInfo->rc[n-j], pathInfo->ic[n-j], x, y);
      if (x*x + y*y > bailout)
      { cl = (j + 1) % 3; if (!cl) cl = 4; return cl + 8; }

   }
   for (j = n; j < maxiter; j++)
   {  f(pathInfo->rc[0], pathInfo->ic[0], x, y);
      if (x*x + y*y > bailout)
      { cl = (j + 1) % 3; if (!cl) cl = 4; return cl + 8; }
   }
   return 0;
}

uint mandelMateThree::mating(mdouble x, mdouble y) //!!!!!!!!!!!!!!!!!!!!!!!
{  if (pathInfo->n <= 0) return 0;
   uint j; mdouble u, v, w, pw = pathInfo->coeff[0]; pw *= pw;
   for (j = pathInfo->n; j > 0; j--)
   {  u = pathInfo->rc[j] + 1.0; v = pathInfo->ic[j];
      pw *= 4.0*(x*x + y*y)*(u*u + v*v);
      w = x*x - y*y - 1.0; y = 2.0*x*y; x = w;
      w = x*x + y*y; if (w < 1e-100) { return 10; } x /= w; y /= w; pw /= w*w;
      w = u*x + v*y; y = v*x - u*y; x = w + 1.0;
      w = x*x + y*y; if (w > 1e10) return 10; if (pw > 1e100) return 13;
   }
   if (w * pathInfo->coeff[0] < 1.0) //lower
   {  u = pathInfo->coeff[1]; v = pathInfo->coeff[2];
      w = u*x - v*y; v = u*y + v*x; u = w;
      x = 1.0 - pathInfo->coeff[3]*x; y *= pathInfo->coeff[3];
      w = x*x + y*y; x /= w; y /= w; w = u*x - v*y; y = u*y + v*x; x = w;
      return mandelMating::martyM(
         pathInfo->rc[0], pathInfo->ic[0], x, y, pw*temp[1]);
   }
   w = x*x + y*y; x /= w; y /= w; //signs ignored
   for (j = 1; j <= maxiter; j++)
   {  u = x*x; v = y*y; w = u + v; if (w > 4.0) return 11; if (w < 0.1) return 10;
      y *= 2.0*x; x = u - v - 1.0;
   }
   return 10;
}

//////////////////////////////////////////////////////////////////////

void mandelRational::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = -2.0*a, v = -2.0*b, rb = a, ib = b, N; // u + iv = A - B
   if (subtype == 4) // A = -(c^2 + 1)/2   B = c
   { u = 0.5*(b*b - a*a) -0.5 - a; v = -a*b - b; }
   if (subtype == 5) {}
   if (subtype == 6) u -= 2.0; // A = -c - 2   B = c
   //subtype == 7: A = -c   B = c
   if (subtype == 8) { u = -1.0 - a; v = -b; } // A = -1   B = c
   N = x*x - y*y + rb; y = 2.0*x*y + ib; x = N;
   N = x*x + y*y; if (N < 1e-100) { x = 1e100; return; } x /= N; y /= N;
   N = u*x + v*y; y = v*x - u*y; x = N + 1.0;
}// (z^2 + A)/(z^2 + B)

void mandelRational::inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
   mdouble &x, mdouble &y) const
{  mdouble u = -2.0*a, v = -2.0*b, rb = a, ib = b, w; // u + iv = A - B
   if (subtype == 4) // A = -(c^2 + 1)/2   B = c
   { u = 0.5*(b*b - a*a) -0.5 - a; v = -a*b - b; }
   if (subtype == 5) {}
   if (subtype == 6) u -= 2.0; // A = -c - 2   B = c
   //subtype == 7: A = -c   B = c
   if (subtype == 8) { u = -1.0 - a; v = -b; } // A = -1   B = c
   X--; w = X*X + Y*Y; if (w < 1e-100) { x = 1e100; return; }
   root((u*X + v*Y)/w - rb, (v*X - u*Y)/w - ib, x, y);
}// sqrt( (A - Bz) / (z - 1) ) = sqrt( (A - Bz) / (z - 1) )

void mandelRational::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode > 0) { x = 0; y = 0; }
   if (mode >= 0) return;
   inverse(A, B, x, y, x, y); if (mode == -2) { x = -x; y = -y; }
}

void mandelRational::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  xmid = 0.0; rewidth = 3.0; if (sg < 0) return;
   if (subtype == 4) { xmid = -3.0; rewidth = 9.0; }
   if (subtype == 6) { xmid = -1.0; rewidth = 4.5; }
   if (subtype == 7) { xmid = -5.0; rewidth = 7.0; }
   if (subtype == 8) rewidth = 2.5;
}

int mandelRational::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  if (subtype == 5) return 0;
   int per = 2; if (subtype == 7 && a >= (1.0 - sqrt(5.0))*0.5) per = 1;
   if (t <= -1.0) return per;
   t *= (2*PI); mdouble l = cos(t), m = sin(t);
   if (subtype == 4)
   {  l = 4.0*l - 1.0; m = -4.0*m; t = a*a + b*b;
      root(l*l - m*m -2.0*l - 3.0 , 2.0*m*(l - 1.0), a, b);
      a += l; b += m;
      if ( (t > 9.0 && a*a + b*b < 9.0) || (t < 9.0 && a*a + b*b > 9.0) )
      { a = 2.0*l - a; b = 2.0*m - b; }
   }
   if (subtype == 6) { a = l + 2.0; b = m; }
   if (subtype == 7 && per == 2)
   {  root(0.25*(l*l - m*m) + 1.0, -0.5*l*m, a, b);
      a = 0.5*l - a; b = -0.5*m - b; // c^2 - 1/rho*c - 1 = 0
   }
   if (subtype == 7 && per == 1)
   {  root(0.25*(l*l - m*m) + 1.0, 0.5*l*m, a, b);
      mdouble u = -0.5*l + a, v = -0.5*m + b, uu = u*u - v*v, vv = 2.0*u*v;
      a = u - uu; b = v - vv; t = l*a + m*b; b = l*b - m*a; a = t;
      a = 2.0*a - uu; b = 2.0*b - vv;
      // w^2 + rho*w - 1 = 0 , c = 2/rho*w*(1 - w) - w^2
   }
   if (subtype == 8) { a = 1.0 + 0.25*l; b = 0.25*m; }
   return per;
} //bifurcate

uint mandelRational::pixcolor(mdouble x, mdouble y)
{ if (sign < 0) return marty(A, B, x, y); else return marty(x, y, 0.0, 0.0); }

uint mandelRational::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble u = -2.0*a, v = -2.0*b, rb = a, ib = b, N; // u + iv = A - B
   if (subtype == 4) // A = -(c^2 + 1)/2   B = c
   { u = 0.5*(b*b - a*a) - 0.5 - a; v = -a*b - b; }
   if (subtype == 5) {}
   if (subtype == 6) u -= 2.0; // A = -c - 2   B = c
   //subtype == 7: A = -c   B = c
   if (subtype == 8) { u = -1.0 - a; v = -b; } // A = -1   B = c
   mdouble X, Y, xp = 1.0, yp = 0.0, up = -2.0, vp = 0.0, rbp = 1.0, ibp = 0.0;
   if (sign > 0)
   {  xp = 0.0;
      if (subtype == 4) { up = -a - 1.0; vp = -b; }
      if (subtype == 8) up = -1.0;
   }
   for (j = 1; j <= maxiter; j++)
   {  X = x*x - y*y + rb; Y = -2.0*x*y - ib; N = X*X + Y*Y;
      if (N > 1e100 || N < 1e-100) return 10; X /= N; Y /= N; // 1/(z^2 + B)
      N = 2.0*(y*yp - x*xp); yp = -2.0*(x*yp + y*xp); xp = N;
      if (sign > 0) { xp -= rbp; yp -= ibp; }
      N = u*xp - v*yp; yp = u*yp + v*xp; xp = N;
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      if (sign > 0) { xp += up; yp += vp; }
      N = X*xp - Y*yp; yp = X*yp + Y*xp; xp = N;
      x = 1.0 + u*X - v*Y; y = u*Y + v*X;
      X = xp*xp + yp*yp; Y = x*x + y*y + 1.0;
      if (X > 1e100 || Y > 1e100) return 10;
      if (temp[1]*X > 5.0*Y*Y) return 9;
   }
   return 10;
}

//////////////////////////////////////////////////////////////////////

void mndmenage::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x, v = y*y, w = u + v; if (w < 1e-20) { x = 1e100; return; }
   u -= v; v = 2*x*y; w *= w; x += 0.5*u/w; y -= 0.5*v/w;
   w = a*x - b*y; y = a*y + b*x; x = w;
}

void mndmenage::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; if (sg > 0) rewidth = 1.1; else rewidth = 16.0; }

void mndmenage::prepare(int sg, uint nmax, uint &dm, mdouble *t)
{  uint DM = dm; mndynamics::prepare(sg, nmax, dm, t);
   if (DM == 4) { dm = 4; drawmode = 4; temp[1] = *t; } // > maxmode
 }

uint mndmenage::pixcolor(mdouble, mdouble)
{ return 0; }

/*uint mndmenage::pixcolor(mdouble x, mdouble y)
{ if (sign < 0) return marty(A, B, x, y); else return marty(x, y, 1.0, 0); }

uint mndmenage::marty(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble xp = (sign > 0 ? 0 : 1.0), yp = 0, u, v, w;
   a -= rb; b -= ib; // c - b
   for (j = 1; j <= maxiter; j++)
   {  u = x*x - y*y + rb; v = -(2*x*y + ib); w = u*u + v*v;
      if (w > 1e20 || w < 1e-20) return 0;
      u /= w; v /= w; // 1/(z^2 + B)
      w = 2*(x*xp - y*yp); yp = 2*(x*yp + y*xp); xp = w;
      w = b*yp - a*xp; yp = -(a*yp + b*xp); xp = w; // 2(b - c)*z*zp
      x = u*u - v*v; y = 2*u*v;
      w = x*xp - y*yp; yp = x*yp + y*xp; xp= w; // zp /= (z^2 + b)^2
      if (sign > 0) { xp += u; yp += v; }
      x = a*u - b*v + 1.0; y = a*v + b*u;
      u = xp*xp + yp*yp; v = x*x + y*y + 1;
      if (u > 1e60 || v > 1e20) return 1;
      if (temp[1]*u > 1.44*v*v) return 9;
   }
   return 0;
}*/

//////////////////////////////////////////////////////////////////////

void mndsingpert::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y; y *= (2*x); x = u;
   u = x*x + y*y; if (u < 1e-20) { x = 1e100; return; }
   a /= u; b /= u;
   u = x + a*x + b*y; y += (b*x - a*y); x = u;
}

void mndsingpert::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{ root(a, b, x, y); root(x, y, x, y); }

void mndsingpert::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; if (sg > 0) rewidth = 0.4; else rewidth = 1.3; }

int mndsingpert::bifurcate(mdouble t, mdouble &a, mdouble & b) const
{  if (t <= -1.0) return 1; t *= (2*PI); mdouble l = cos(t), m = sin(t);
   a = l*l - m*m; b = 2*l*m; l = a + 4*l + 4.0; m = b + 4*m;
   a = 4.0 - a; b = -b; t = a*l - b*m; b = a*m + b*l;
   a = t/256.0; b /= 256.0;
   return 1; // 256*c = (L + 2)^3 * (2 - L)
}

//////////////////////////////////////////////////////////////////////

void mandelHerman::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = a - x, v = b - y, U = 1.0 + a*x + b*y, V = a*y - b*x,
   r = U*U + V*V; if (r < 1e-10) { x = 1e100; return; }
   U /= r; V /= r; r = u*U + v*V; v = v*U - u*V; u = r; //u+iv=(c-z)/(1+_cz)
   U = x*x - y*y; V = 2.0*x*y; x = u*U - v*V; y = v*U + u*V;
}

void mandelHerman::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble rr = a*a + b*b, u, v, w = 0.25*rr - 0.75;
   if (rr < 1e-10) { x = 0.0; y = 0.0; return; }
   root(w*w + rr, 0.0, u, v); u += w;
   x = (a*u  - b*v)/rr; y = (a*v + b*u)/rr;
}

void mandelHerman::iterate(mdouble &x, mdouble &y, int mode) const // = 0
{  if (!mode) f(A, B, x, y);
   if (mode <= 0) return;
   critical(A, B, x, y); if (mode & 1) return;
   mdouble w = -A*x - B*y; y = A*y - B*x; x = w; w = x*x + y*y;
   if (w < 1e-10) { x = 0.0; y = 0.0; return; }
   x /= w; y /= w; w = A*x - B*y; y = A*y + B*x; x = w;
}

void mandelHerman::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg < 0) { xmid = 0.0; rewidth = 8.0; }
   else { xmid = 0.0; rewidth = 4.5; }
}

uint mandelHerman::pixcolor(mdouble x, mdouble y)
{  uint j; if (sign > 0) return 9;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y > bailout) return 10;
      if (x*x + y*y < 1e-8) return 12;
      f(A, B, x, y);
   }
   return 0;
}

//////////////////////////////////////////////////////////////////////

//void mndnewtonsiegel::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
void mndnewtonsiegel::f(mdouble, mdouble, mdouble &, mdouble &) const
{  /*mdouble rp, ip, u, v, w;

   rp = 4*x - 6*a + 6.0; ip = 4*y - 6*b;
   w = x*rp - y*ip - 12*a; ip = x*ip + y*rp - 12*b; rp = w;
   w = x*rp - y*ip + 10*a - 10.0; ip = x*ip + y*rp + 10*b; rp = w; // P'

   u = 3*x - 4*a + 4.0; v = 3*y - 4*b;
   w = x*u - y*v - 6*a; v = x*v + y*u - 6*b; u = w;
   w = x*u - y*v; v = x*v + y*u; u = w;
   w = x*u - y*v + 18*a - 7.0; y = x*v + y*u + 18*b; x = w; // zP' - P

   w = rp*rp + ip*ip; if (w < 1e-40) { x = 1e100; return; }
   a = (x*rp + y*ip)/w; y = (y*rp - x*ip)/w; x = a;*/
}

//void mndnewtonsiegel::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
void mndnewtonsiegel::critical(mdouble, mdouble, mdouble &, mdouble &) const
{
}

void mndnewtonsiegel::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.5; }

uint mndnewtonsiegel::esctime(mdouble x, mdouble y)
{  //if (sign < 0 || drawmode > 255) return mndnewton::esctime(x, y);
   uint j; int point; mdouble x0, y0, a = x, b = y;
   critical(a, b, x, y); x0 = x; y0 = y;
   for (point = 0; point <= 1; point++)
   {  if (point) { which ^= 3; critical(a, b, x, y); x0 = x; y0 = y; }
      for (j = 1; j <= maxiter; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 65280;
         if ( (x - x0)*(x - x0) + (y -y0)*(y -y0) < 1e-5) return j;
         x0 = x, y0 = y;
      }
      if (point && drawmode) return 65280 | (drawmode & 15);
      if (!drawmode) for (j = 0; j < 60; j++)
      {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 65280;
         if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-6 )
         { if (j) return 65281 + (j  % 12); else break; }
      }
   }
   return 65293;
}

////////////////////////////////////////////////////////////////////

void mndnewtonpara::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y + 2.0*x - a*a + b*b, v = 2.0*(x*y + y - a*b),
   w = u*u + v*v; if (w < 1.0e-50) { x = 1.0e100; return; }
   w = 2.0/w; u *= w; v *= w;
   w = x + x*u + y*v; y += y*u - x*v; x = w - 1.0;
}  // z - F/F'  with  F = (z^2 - c^2)*e^z

void mndnewtonpara::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  root(2.0 + a*a - b*b, 2.0*a*b, x, y);
   if (which & 1) { x = -x; y = -y; }
   x -= 2.0;
}

uint mndnewtonpara::singlecolor(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 0;
      if (x < -100.0) return 9; //1 + 8*(j & 1);
      if ( (x - a)*(x - a) + (y - b)*(y - b) < 1.0e-6) return 2 + 8*(j & 1);
      if ( (x + a)*(x + a) + (y + b)*(y + b) < 1.0e-6) return 4 + 8*(j & 1);
   }
   return 0;
}

uint mndnewtonpara::pixcolor(mdouble x, mdouble y)
{  if (sign < 0) return singlecolor(A, B, x, y);
   mdouble u, v; critical(x, y, u, v); uint cl = singlecolor(x, y, u, v);
   if (!(cl & 1)) return cl;
   which ^= 3; return singlecolor(x, y, -4.0 - x, -y);
}

void mndnewtonpara::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 5.0; }

////////////////////////////////////////////////////////////////////

void mndnewtonqara::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u, v, w, U, V, aa = a*a - b*b + 1.0, bb = 2.0*a*b;
   U = aa*x - bb*y - 2.0*a; V = aa*y + bb*x - 2.0*b; //F
   u = x + a; v = y + b; w = U*u - V*v + aa; v = U*v + V*u + bb; u = w; //F'
   w = u*u + v*v; if (w < 1.0e-50) { x = 1.0e100; return; }
   w = 1.0/w; u *= w; v *= w;
   x -= U*u + V*v; y += U*v - V*u;
}  // z - F/F'  with  F = [(c^2 + 1)z - 2c]*e^{z^2/2 + cz}
//corresponds to  z*e^{z^2/2 + Cz}  with  C = c + 2c/(c^2 + 1)

void mndnewtonqara::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (which == 1) { x = 0.0; y = 0.0; return; }
   mdouble w, aa = a*a - b*b, bb = 2.0*a*b;
   root(bb*bb - aa*aa - 3.0*aa - 3.0, -2.0*aa*bb - 3.0*bb, x, y);
   if (which & 1) { x = -x; y = -y; }
   x += bb*b - aa*a; y -= aa*b + bb*a;
   aa++; w = aa*aa + bb*bb; if (w < 1.0e-50) { x = 1.0e100; return; }
   w = 1.0/w; aa*= w; bb *= w; w = x*aa + y*bb; y = y*aa - x*bb; x = w;
}

uint mndnewtonqara::singlecolor(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; mdouble aa = a*a - b*b + 1.0, bb = 2.0*a*b, cc = aa*aa + bb*bb;
   if (cc < 1.0e-20) return 0;
   cc = 2.0/cc; aa *= cc; bb *= cc; cc = aa*a + bb*b; bb = aa*b - bb*a; aa=cc;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 0;
      if (y > 10.0) return 12;
      if (y < -10.0) return 9;
      if ((x - aa)*(x - aa) + (y - bb)*(y - bb) < 1.0e-6) return 2 + 8*(j & 1);
   }
   return 0;
}

uint mndnewtonqara::pixcolor(mdouble x, mdouble y)
{  if (sign < 0) return singlecolor(A, B, x, y);
   mdouble u, v; critical(x, y, u, v);
   uint cl, cl0 = singlecolor(x, y, u, v); if (!(cl0 & 5)) return cl0;
   which++; if (which > 3) which = 1; critical(x, y, u, v);
   cl = singlecolor(x, y, u, v); if (!(cl & 5)) return cl; else cl0 += cl;
   which++; if (which > 3) which = 1; critical(x, y, u, v);
   cl = singlecolor(x, y, u, v); if (!(cl & 5)) return cl; else cl0 += cl;
   return (cl0 & 1 ? 12 : 9);
}

void mndnewtonqara::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 3.5; }

////////////////////////////////////////////////////////////////////

void mndnewtonrara::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble u = x*x - y*y + a*x - b*y + 1.0, v = 2.0*x*y + a*y + b*x,
      w = a*a - b*b; b *= 2.0*a; a = w;
   w = u*u + v*v; if (w < 1.0e-50) { x = 1.0e100; return; }
   w = 1.0/w; u *= w; v *= w;
   x -= a*u + b*v; y += a*v - b*u;
}  // z - F/F'  with  F'/F = [z^2 + cz + 1]/c^2

void mndnewtonrara::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (which <= 2) { a = -a; b = -b; }
   root(a - 1.0, b, x, y); if (which & 1) { x = -x; y = -y; }
   if (which >= 3) { x -= a; y -= b; }
} // z^2 + c + 1 = 0  or  (z + c)^2 = c - 1

uint mndnewtonrara::singlecolor(mdouble a, mdouble b, mdouble x, mdouble y)
{  uint j; if (a*a + b*b < 1.0e-50) return 0;
   mdouble X = atan2(b, a), Y = -X/1.5, u = cos(Y), v = sin(Y);
   for (j = 1; j <= maxiter; j++)
   {  X = x*x + y*y; if (X <= bailout) f(a, b, x, y); else return 0;
      if (X < 1000.0) continue; X = u*x - v*y; Y = u*y + v*x;
      if (X < 2.0*Y && X < -2.0*Y) return 9; if (X < 0) continue;
      if (X < Y) return 10;
      if (X < -Y) return 12;
   }
   return 0;
}

uint mndnewtonrara::pixcolor(mdouble x, mdouble y)
{  if (sign < 0) return singlecolor(A, B, x, y);
   mdouble u, v; critical(x, y, u, v);
   uint cl, cl0 = singlecolor(x, y, u, v); if (!cl0) return 0;
   which++; if (which > 4) which = 1; critical(x, y, u, v);
   cl = singlecolor(x, y, u, v); if (!cl) return 0; else cl0 += cl;
   which++; if (which > 4) which = 1; critical(x, y, u, v);
   cl = singlecolor(x, y, u, v); if (!cl) return 0; else cl0 += cl;
   which++; if (which > 4) which = 1; critical(x, y, u, v);
   return ((singlecolor(x, y, u, v) + cl0 + 1) & 15); // cl + cl0 -31
}

void mndnewtonrara::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{  if (sg > 0) { xmid = 12.0; rewidth = 8.0; }
   else { xmid = 0; rewidth = 20.0; }
}

//////////////////////////////////////////////////////////////////////

uint mndnewton::esctime(mdouble x, mdouble y)
{  uint j; mdouble a, b, x0, y0;
   if (sign > 0) { a = x; b = y; critical(a, b, x, y); }
   else { a = A; b = B; }
   if (drawmode > 255) return renormtime(a, b, x, y);
   x0 = x; y0 = y;
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 65280;
      if ( (x - x0)*(x - x0) + (y -y0)*(y -y0) < 1e-5) return j;
      if (sign < 0 && !drawmode && Period > 1
         && (x - temp[0])*(x - temp[0]) + (y - temp[1])*(y - temp[1]) < 1e-6)
      {  int cl = 1 + j % Period;
         if (Period <= 4 && j % (2*Period) >= Period ) cl |= 8;
         return 65280 | cl;
      }
      x0 = x, y0 = y;
   }
   if (drawmode) return 65280 | (drawmode & 15);
   if (sign > 0) for (j = 0; j < 60; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 65280;
      if ( (x - x0)*(x - x0) + (y - y0)*(y - y0) < 1e-6 )
      { if (j) return 65281 + (j  % 12); else break; }
   }
   return 65293;
}

uint mndnewton::pixcolor(mdouble x, mdouble y)
{  uint j, l; mdouble a, b;
   if (sign > 0) { a = x; b = y; critical(a, b, x, y); }
   else { a = A; b = B; }
   fixed(a, b); //sets temp[] to fixed points
   for (j = 1; j <= maxiter; j++)
   {  if (x*x + y*y <= bailout) f(a, b, x, y); else return 0;
      for (l = 0; l < degree; l++) if( (x - temp[2*l])*(x - temp[2*l])
           + (y - temp[2*l+1])*(y - temp[2*l+1]) < 1e-6)
      { if (l == 4) l = 5; if (!l) l = 4; return (l + 8*(j & 1)); }
   }
   return 0;
}

//////////////////////////////////////////////////////////////////////

void mndcubicnewton::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble w = a*a - b*b + .75; b = 2*a*b; a = w; // c = lambda^2 + 3/4
   mdouble u = 2*(x*a - y*b) + 3*(1 - a), v = 2*(x*b + y*a) - 3*b; //numerator
   a = 3*(x*x - y*y) - a; b = 6*x*y - b; //denominator
   w = u*a + v*b; v = v*a - u*b; u = w;
   w = a*a + b*b; if (w < 1e-100) return;
   x = (2*x + u/w)/3.0; y = (2*y + v/w)/3.0;
}

void mndcubicnewton::fixed(mdouble a, mdouble b) const
{  temp[0] = -0.5 + a; temp[1] = b;
   temp[2] = 1.0; temp[3] = 0;
   temp[4] = -0.5 - a; temp[5] = -b;
}

void mndcubicnewton::startPlane(int sg, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.0; if (sg < 0) rewidth = 2.5; }

//////////////////////////////////////////////////////////////////////

void mndquarticnewton::f(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  if (subtype == 2) { f2(a, b, x, y); return; }
   if (subtype == 8) { f8(a, b, x, y); return; }
   mdouble rp, ip, rq, iq, u, v, w = a*a + b*b; if (w < 1e-20) return;
   if (subtype == 1)
   {  rp = (a - a/w)/sqrt(2.0); ip = (b + b/w)/sqrt(2.0);
      rq = 1.5*(rp*rp - ip*ip) + 1.0; iq = 3*rp*ip;
   }
   if (subtype == 4)
   {  rp = 3*a + 1.0; ip = 3*b;
      rq = (4*(rp*rp - ip*ip) -2*rp + 1.0)/3.0; iq = (8*rp*ip - 2*ip)/3.0;
   }
   if (subtype == 6)
   {  rp = a + a/w; ip = b - b/w;
      rq = 7.0 - sqrt(12.0)*(b + b/w); iq = sqrt(12.0)*(a - a/w);
   }
   if (subtype == 7)
   {  rp = a + a/w - 3.0; ip = b - b/w;
      rq = -6*rp - 5.0; iq = -6*ip;
   }

   u = 3*x - 4*rp; v = 3*y - 4*ip;
   w = x*u - y*v + rq - 1.0; v = x*v + y*u + iq; u = w;
   a = x*x - y*y; b = 2*x*y;
   w = a*u - b*v + rq; b = a*v + b*u  + iq; a = w; // zP' - P

   u = 4*x - 6*rp; v = 4*y - 6*ip;
   w = x*u - y*v + 2*rq - 2.0; v = x*v + y*u + 2*iq; u = w;
   rp = x*u - y*v + 2*rp; ip = x*v + y*u + 2*ip; // P'

   w = rp*rp + ip*ip; if (w < 1e-40) { x = 1e100; return; }
   x = (a*rp + b*ip)/w; y = (b*rp - a*ip)/w;
}

void mndquarticnewton::f2(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble rp, ip, u, v, w;
   w = a*a - b*b; b *= (2*a); a = w;
   u = x*x - y*y; v = 2*x*y;

   rp = 4*u - 12*a; ip = 4*v - 12*b;
   w = x*rp - y*ip; ip = x*ip + y*rp; rp = w; // P'

   x = 3*u - 6*a; y = 3*v - 6*b;
   w = x*u - y*v; y = x*v + y*u; x = w;
   w = a*a - b*b - 2.25; b *= (2*a); a = w;
   x += (a*a - b*b); y += (2*a*b); // zP' - P

   w = rp*rp + ip*ip; if (w < 1e-40) { x = 1e100; return; }
   a = (x*rp + y*ip)/w; y = (y*rp - x*ip)/w; x = a;
}

void mndquarticnewton::f8(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble rp, ip, u, v, w;

   rp = 4*x - 6*a + 6.0; ip = 4*y - 6*b;
   w = x*rp - y*ip - 12*a; ip = x*ip + y*rp - 12*b; rp = w;
   w = x*rp - y*ip + 10*a - 10.0; ip = x*ip + y*rp + 10*b; rp = w; // P'

   u = 3*x - 4*a + 4.0; v = 3*y - 4*b;
   w = x*u - y*v - 6*a; v = x*v + y*u - 6*b; u = w;
   w = x*u - y*v; v = x*v + y*u; u = w;
   w = x*u - y*v + 18*a - 7.0; y = x*v + y*u + 18*b; x = w; // zP' - P

   w = rp*rp + ip*ip; if (w < 1e-40) { x = 1e100; return; }
   a = (x*rp + y*ip)/w; y = (y*rp - x*ip)/w; x = a;
}

void mndquarticnewton::fixed(mdouble a, mdouble b) const
{  if (subtype == 2)
   {  mdouble aa = a*a - b*b; b *= (2*a); a = aa;
      temp[0] = a + 1.5; temp[1] = b;
      temp[4] = -a - 1.5; temp[5] = -b;
      temp[2] = -b; temp[3] = a - 1.5;
      temp[6] = b; temp[7] = -a + 1.5; return;
   }
   mdouble w = a*a + b*b; if (w < 1e-20) return;
   temp[0] = -1.0; temp[1] = 0;
   temp[4] = 1.0; temp[5] = 0;
   if (subtype == 1)
   {  temp[2] = (a - a/w)/sqrt(2.0) - 0.5*(b - b/w);
      temp[3] = (b + b/w)/sqrt(2.0) + 0.5*(a + a/w);
      temp[6] = (a - a/w)/sqrt(2.0) + 0.5*(b - b/w);
      temp[7] = (b + b/w)/sqrt(2.0) - 0.5*(a + a/w);
   }
   if (subtype == 4)
   {  temp[2] = 3*a + 1.0 - sqrt(3.0)*b; temp[3] = 3*b + sqrt(3.0)*a;
      temp[6] = 3*a + 1.0 + sqrt(3.0)*b; temp[7] = 3*b - sqrt(3.0)*a;
   }
   if (subtype == 6)
   {  temp[2] = 2*a; temp[3] = 2*b - sqrt(3.0);
      temp[6] = 2*a/w; temp[7] = -2*b/w + sqrt(3.0);
   }
   if (subtype == 7)
   {  temp[2] = 2*a - 3.0; temp[3] = 2*b;
      temp[6] = 2*a/w - 3.0; temp[7] = -2*b/w;
   }
}

void mndquarticnewton::critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
{  mdouble w = a*a + b*b; if (w < 1e-20) return;
   if (subtype == 1)
   { x = (a - a/w)/sqrt(8.0); y = (b + b/w)/sqrt(8.0); }
   if (subtype == 2 || subtype == 4 || subtype == 8)
   { x = a; y = b; }
   if (subtype == 6)
   {  mdouble rph = 0.5*(a + a/w), iph = 0.5*(b - b/w); //rp/2, ip/2
      x = 1.0 - (b + b/w)/sqrt(3.); y = (a - a/w)/sqrt(3.0); //(rq - 1, iq)/6
      root(rph*rph - iph*iph - x, 2*rph*iph - y, x, y);
      x += rph; y += iph;
   }
   if (subtype == 7)
   { x = a + a/w - 2.0; y = b - b/w; }

}

void mndquarticnewton::startPlane(int, mdouble &xmid, mdouble &rewidth) const
{ xmid = 0; rewidth = 2.5; }

//////////////////////////////////////////////////////////////////////

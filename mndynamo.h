/* mndynamo.h  by Wolf Jung (C) 2007-2019.  Declares classes:
   [defined in mndynamo.cpp :]
   mndynamics, mndsiegel, mndcubicsiegel, mndquartsiegel, mndexposiegel,
   mndtrigosiegel, mndexpo, mndtrigo, mndmatesiegel, mndmating, mndsingpert,
   mndherman, mndnewtonsiegel, mndnewton, mndcubicnewton, mndquarticnewton
   [defined in mndlbrot.cpp :]
   mndlbrot, mndmulti, mndbfpoly, mndcubic, mndquartic, mndsurge,
   mndrealcubic, mndtricorn, mndhenon, mndifs, mndlambda, mndScale
   [defined in mndcombi.cpp :]
   mndAngle, mndCombi, mandelPath, mandelPathMate,
   mandelPathTwo, mandelPathThree, mandelbrotPath, mandelPathExpo.
   Defines class mandelPathInfo.

   These classes are part of Mandel 5.17, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/
#ifndef MNDYNAMICS_H_INCLUDED
#define MNDYNAMICS_H_INCLUDED

typedef  unsigned int  uint;
typedef  unsigned long long int  qulonglong;
typedef  long double  mdouble;
//typedef  double  mdouble;
typedef  double  odouble;

#define  PI      3.1415926535897932385L
#define  logTwo  0.6931471805599452862L
#define  logTen  2.3025850929940459L
//(sqrt(5) - 1)/2   .618033988749894848204586834370L  //golden mean
#define  COS_GM  -.737368878078319901518240381351L  //cos(2*Pi*gm)
#define  SIN_GM  -.675490294261523642344386198138L  //sin(2*Pi*gm)

/* The library  mndynamics  provides mathematical functions related to complex
   dynamics.  It is independent of the platform and the graphics toolkit.  It
   is used with the platform-independent program Mandel,  which employs the
   GUI-toolkit Qt.  You may use it as well to write a graphics file directly,
   without displaying it on the screen.

   Most classes describe a one-parameter family of complex functions  f_c(z) ,
   c = a + bi , z = x + yi . In the dynamic z-plane,  the filled Julia set
   contains all points  z  with a bounded orbit.  In the parameter c-plane,
   the set of parameters  c  is considered,  such that the critical orbit of
   f_c  is bounded.  The standard example is the Mandelbrot set  M , which is
   obtained from the family of quadratic polynomials  f_c(z) = z^2 + c .
   Polynomial families of higher degrees have similar properties,  when their
   critical orbits satisfy persistent relations,  so that it is sufficient to
   check if one orbit escapes.  The concepts extend to rational functions with
   an attracting periodic point at infinity,  and to transcendental functions
   with finitely many critical values.  Families with a persistently neutral
   cycle (Siegel disk) require the consideration of several critical orbits,
   and for Newton mappings the convergence to finite points is discussed.

   0) Abstract base class (escape to infinity):
       mndynamics
   1) Derived classes (with critical relations and escape to infinity):
       mndlbrot   :  z^2 + c , Mandelbrot set
       mndmulti   :  z^q + c , Multibrot set
       mndbfpoly  :  cz(1 + z/q)^q  (Branner-Fagella)
       mndcubic   :  various cubic polynomial families
       mndquartic :  various quartic polynomial families
       mandelMating, mandelBitransitive, mandelSymmetric,
          mandelMateTwo, mandelmateThree, MandelRational
       mandelHerman
       mndmenage  :  c(z + 0.5/z^2) , rotationally symmetric (Henriksen)
       mndsingpert:  z^2 + c/z^2 , singular perturbation of z^2
       mndexpo    :  various exponential families
       mndtrigo   :  various trigonometric families
       mndsurge   :  piecewise polynomial,  modeling quasi-conformal surgery
       mndtricorn :  (z*)^q + c , anti-analytic,  Tricorn or Multicorn
       mndrealcubic: cubic poly.,  real-analytic para.,  2 indep. cr.pt.
       mndifs     :  iterated function system for Liouville roots
   2) Abstract derived class (persistent Siegel, two orbits, one may escape):
       mndsiegel
   3) Derived from mndsiegel (persistent Siegel, two orbits, one may escape):
       mndcubicsiegel  :  cubic polynomials (Zakeri-Buff-Henriksen & 2-per)
       mndquartsiegel  :  quartic polynomials (additional critical relation)
       mndexposiegel   :  exponential
       mndtrigosiegel  :  trigonometric
       mandelMateSiegel:  quadratic rational
       mndnewtonsiegel :  quartic Newton mappings with Siegel cycle
       mndnewtonpara   :  cubic Newton maps with 2 roots and 1 parabolic basin
       mndnewtonqara   :  cubic Newton maps with 1 root and 2 parabolic basins
       mndnewtonrara   :  cubic Newton maps with 0 roots and 3 parabolic basins
   4) Abstract derived class (cubic or quartic Newton mappings):
       mndnewton
   5) Derived from mndnewton (Newton mappings with critical relations):
       mndcubicnewton   :  Newton for cubic polynomials
       mndquarticnewton :  Newton for quartic polynomials
   6) Derived from mndynamics (special case with escape to 0 or infinity):
       mndherman :  cubic rational with Herman ring
   7) Derived from mndynamics (special case, escape to -infinity, no cr.pt.):
       mndhenon  :  Henon mapping
   8) Derived from mndynamics (special case, escape to squares, no cr.pt.):
       mndlambda  :  Henon mapping
   9) Additional classes not derived from mndynamics:
       mndScale  :  scaling of the parameter plane at Misiurewicz points
       mndAngle  :  external angles
       mndCombi  :  kneading sequences or internal addresses
       mndPath   :  spider with legs or with path---------------

   Using these classes:
   In an interactive program,  you can benefit from polymorphism by using
   a pointer to the abstract base class:
      mndynamics *f;  on the user's choice:  f = new mndlbrot(2);
   In this way,  most code will be independent of the concrete class chosen
   at runtime.  In the constructor,  the "subtype" is specified,  which may
   define a subfamily.  In the classes  mndlbrot,  mndmulti,  mndbfpoly,
   mndquartic , and  mndtricorn,  change this positive integer parameter
   with  setDegree().

   Many functions expect a parameter "sign":  positive is parameter plane,
   negative is dynamic plane.  Mandel  employs a variable
   signtype = +- subtype : its modulus remembers the chosen subtype, and
   its sign denotes the current plane;  pass its value for sign.  A single
   pointer  mndynamics *f  is used.  When a point c in the parameter plane
   is chosen by the user,  this is conveyed by  f->setParameter(a, b) , but
   it has no effect on working in the parameter plane.  Then,  when  f  is
   used in the dynamic plane (sign < 0),  this value of the parameter is
   effective.  A drawback of this approach in a multi-threaded program is
   that you cannot draw both planes simultaneously;  you must always stop
   one plane before drawing in the other one.  (mndynamics is reentrant but
   not thread-safe.)  This problem can be avoided by using two different
   pointers to mndynamics,  as in the demo of Mandel.

   Drawing: ...

   startPlane
   background on modes, min max, more in mndlbrot
   prepare, escapetime, pixcolor

   Further functions:
   iterate, period, find, bifurcate, rays, similarity, getpara


   in derived class:
   must define constructor (resets bailout), f, startPlane.
   may change: critical, iterate, minmode, maxmode, setDegree, setParameter,
      prepare, period, esctime, pixcolor
   communicate by startPlane or setDegree
*/

/* This public class is meant for short-term exchange of, e.g., a path or
   a list of parameters.  You should set  n  negative for invalid data and
   have points  rc + i*ic [0...n]  otherwise.  Delete the arrays before
   reallocating,  or set to 0 when unused.
*/

class mandelPathinfo {
private: //disabled
   mandelPathinfo(const mandelPathinfo &);
   mandelPathinfo &operator=(const mandelPathinfo &);
public:
   int n; mdouble *coeff, *rc, *ic;
   mandelPathinfo()
   { n = -1; coeff = 0; rc = 0; ic = 0; }
   virtual ~mandelPathinfo()
   { delete[] coeff; delete[] rc; delete[] ic; }
};

class mndynamics {
private: //disabled
   mndynamics(const mndynamics &);
   mndynamics &operator=(const mndynamics &);
protected:
   int subtype, sign;
   uint drawmode, maxiter, Period;
   mdouble A, B, bailout, *temp;
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const = 0;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 0; y = 0; }
   virtual uint renormtime(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mandelPathinfo *pathInfo;
   static void root(mdouble x, mdouble y, mdouble &u, mdouble &v);
   mndynamics(int subtype0)
   {  subtype = subtype0; A = 0.0L; B = 0.0L; bailout = 16.0L; pathInfo = 0;
      temp = new mdouble[5]; pathInfo = new mandelPathinfo;
   }
   virtual ~mndynamics() { delete[] temp; delete pathInfo; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 0; }
   virtual int maxMode(int) const { return 3; }
   virtual void setParameter(mdouble a, mdouble b) { A = a; B = b; }
   virtual void getParameter(mdouble &a, mdouble &b) const { a = A; b = B; }
   virtual void setDegree(int) {}
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const = 0;
   //virtual int find(int sg, uint preper, uint per, mdouble &x, mdouble &y) const
   virtual int find(int, uint, uint, mdouble &, mdouble &) const
   { return 1; }
   //virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const { return 0; }
   virtual int bifurcate(mdouble, mdouble &, mdouble &) const { return 0; }
   virtual void prepare(int sg, uint nmax, uint &dm, mdouble *t);
   virtual uint period(mdouble &a, mdouble &b, int cycle = 0);
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
   virtual int turnsign(mdouble, mdouble) { return 0; }
   virtual mdouble green(int, mdouble, mdouble) { return 0; }
   //virtual int similarity(uint preper, uint per, mdouble&a, mdouble&b, mdouble *t)
   virtual int similarity(uint, uint, mdouble &, mdouble &, mdouble *) const
   { return -1; }
}; //mndynamics

/*
   In derived classes,  critical() sets cr.pt. depending on "which",  which
   is set before by iterate(),  period(),  esctime(),  or pixcolor().
*/
class mndsiegel : public mndynamics {
protected:
   mutable int which;    //number of the active cr.pt.
   mutable mdouble L, M;  //lambda = L + iM,  set with startPlane(0, L, M)
public:
   mndsiegel(int subtype0) : mndynamics(subtype0)
   { L = COS_GM; M = SIN_GM; which = 1; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual uint period(mdouble &a, mdouble &b, int cycle = 0);
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mndcubicsiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void f2(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndcubicsiegel(int subtype0) : mndsiegel(subtype0)
   {  bailout = 100.0L; //c = 0 would have singular cr.pt.:
      if (subtype != 1) { subtype = 2; A = 0.20175L; B = -0.86235L; }
   }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
};

class mndquartsiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndquartsiegel(int subtype0) : mndsiegel(subtype0) {}
    virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndexposiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndexposiegel(int subtype0) : mndsiegel(subtype0) { bailout = 1e20; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndtrigosiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndtrigosiegel(int subtype0) : mndsiegel(subtype0) { bailout = 1e20; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndexpo : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 0; y = 0; if (subtype & 2) x = -1.0; }
public:
   mndexpo(int subtype0) : mndynamics(subtype0) { bailout = 1e20;  }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndtrigo : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 0; y = 0; if (subtype == 1) x = 0.5L*PI; if (subtype >= 5) x = PI; }
public:
   mndtrigo(int subtype0) : mndynamics(subtype0) { bailout = 1000.0L; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mandelMateSiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = -1.0; y = 0; if (which & 1) x = 1.0L; }
public:
   mandelMateSiegel(int subtype0) : mndsiegel(subtype0)
   {  bailout = 1e10; mdouble w = L*L + M*M;
      if (w > 1e-10) { L = L/w; M = -M/w; }
   }
   virtual int maxMode(int) const { return 0; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mandelMating : public mndynamics {
protected:
   mutable mdouble C, D;
   virtual void f(mdouble, mdouble, mdouble &, mdouble &) const {}
public:
   mandelMating(int subtype0) : mndynamics(subtype0)
   //{ A = -0.122561166876654; B = 0.744861766619744L; C = -1.0L; D = 0.0L; }
   { A = -1.256367930068181L; B = 0.380320963472722L;
      C = -0.615805132905564L; D = 0.687211340203275L; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 4; }
   virtual int maxMode(int sg) const { return (sg > 0 ? 4 : 6); }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
   static uint martyM(mdouble a, mdouble b, mdouble x, mdouble y,
      mdouble pw, int sg = -1);
};

class mandelBitransitive : public mndynamics {
protected:
   mutable mdouble rb, ib;
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
                        mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 1.0L; y = 0.0L; }
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mandelBitransitive(int subtype0) : mndynamics(subtype0)
   { rb = 0.0L; ib = 0.0L; bailout = 1.0e100L; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 0; }
   virtual int maxMode(int) const { return 4; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mandelSymmetric : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
                        mdouble &x, mdouble &y) const;
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mandelSymmetric(int subtype0) : mndynamics(subtype0)
   { bailout = 1e20; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 0; }
   virtual int maxMode(int) const { return 0; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mandelMateTwo : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
                        mdouble &x, mdouble &y) const;
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
   uint capture(mdouble x, mdouble y);
   uint mating(mdouble x, mdouble y);
   uint anti(mdouble x, mdouble y);
public:
   mandelMateTwo(int subtype0) : mndynamics(subtype0)
   { bailout = 1e20; pathInfo = new mandelPathinfo(); }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int sg) const
   {  if (sg > 0 || subtype == 4) return 0;
      if (subtype > 1) return 4; else return 2;
   }
   virtual int maxMode(int sg) const
   {  if (sg > 0 || subtype == 4) return 4;
      if (subtype > 1) return 6; else return 2;
   }
   virtual void setDegree(int degree) { subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mandelMateThree : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
                        mdouble &x, mdouble &y) const;
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
   uint capture(mdouble x, mdouble y);
   uint mating(mdouble x, mdouble y);
public:
   mandelMateThree(int subtype0) : mndynamics(subtype0)
   { bailout = 1e20; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int sg) const
   {  if (sg > 0 || subtype == 4) return 0;
      if (subtype > 1) return 4; else return 2;
   }
   virtual int maxMode(int sg) const
   {  if (sg > 0 || subtype == 4) return 3;
      if (subtype > 1) return 6; else return 2;
   }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mandelRational : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void inverse(mdouble a, mdouble b, mdouble X, mdouble Y,
                        mdouble &x, mdouble &y) const;
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mandelRational(int subtype0) : mndynamics(subtype0)
   { bailout = 1e100; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 0; }
   virtual int maxMode(int) const { return 4; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* Rational mappings of degree 3 with rotational symmetry. Including
   menage (Henriksen) of 3 equal polynomials: c(z + 0.5/z^2)
*/
class mndmenage : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 1.0L; y = 0.0L; }
   //uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mndmenage(int subtype0) : mndynamics(subtype0) { bailout = 1e20; }
   virtual int maxMode(int) const { return 0; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual void prepare(int sg, uint nmax, uint &dm, mdouble *t);
   virtual uint pixcolor(mdouble x, mdouble y);

};

/* Rational mappings of degree 4,  which can be considered as a singular
   perturbation of  z^2 : z^2 + c/z^2 . The critcal point 0 is going to
   the superattracting fixed point at infinity.  The four other critical
   orbits meet.
*/
class mndsingpert : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndsingpert(int subtype0) : mndynamics(subtype0) { bailout = 1e20; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
};

/* Rational maps of degree 3 with 0 and infinity superattracting. The
   unit circle is invariant:  z^2 (c - z)/(1 + _cz) . For almost all arguments
   and large modulus of  c , there is a Herman ring around the unit circle.
   The parametrization is not complex analytic,  but all maps are
   antipode-preserving.
*/
class mandelHerman : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mandelHerman(int subtype0) : mndynamics(subtype0)
   { A = -1.52197453833801561L; B = 3.91453618001555637L; bailout = 1e8; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int sg) const { return (sg > 0 ? 0 : 2); }
   virtual int maxMode(int sg) const { return (sg > 0 ? 0 : 2); }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mndnewtonsiegel : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndnewtonsiegel(int subtype0) : mndsiegel(subtype0) { bailout = 100.0L; }
   virtual int maxMode(int) const { return 1; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint esctime(mdouble x, mdouble y);
};

/* The classes  mndnewtonpara , mndnewtonqara , and  mndnewtonrara  are
   cubic Newton maps for functions of the form  p*e^q , which are rational
   with a higher degree than  p  and with a parabolic point at infinity.
   See Baranski and Mamayusupov.
*/
class mndnewtonpara : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndnewtonpara(int subtype0) : mndsiegel(subtype0)
   {  bailout = 1.0e100L; subtype = 1; A =  -0.60507L; B = 0.90361L; }
   virtual int minMode(int) const { return 2; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint singlecolor(mdouble a, mdouble b, mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mndnewtonqara : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndnewtonqara(int subtype0) : mndsiegel(subtype0)
   {  bailout = 1.0e100L; subtype = 2; A = 0.6005L; B = 1.79164L; }
   virtual int minMode(int) const { return 2; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint singlecolor(mdouble a, mdouble b, mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mndnewtonrara : public mndsiegel {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndnewtonrara(int subtype0) : mndsiegel(subtype0)
   {  bailout = 1.0e100L; subtype = 3; A = 12.3983L; B = 0.0L; }
   virtual int minMode(int) const { return 2; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint singlecolor(mdouble a, mdouble b, mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The class  mndnewton  is an abstract base class suitable for Newton
   mappings  f(z) = z - P/P'  of degree <= 5 with one free critical point.
   The additional variable  degree  is the number of fixed points,  which are
   saved in  temp[]  by the additional function  fixed() . Now  esctime()  is
   counting convergence to any fixed point as escaping,  and  pixcolor()  is
   marking convergence to the given fixed points in different colors.  When
   there is no explicit formula for the fixed points,  set  degree = 0 ,
   and drawmode 2 will be disabled.

   As always,  you must define  f()  and  startPlane()  in the derived class,
   and  critical()  if it is not 0.  If the fixed points are known,  set the
   degree  and define  fixed().  For  degree > 5  you would have to enlarge
   temp[] and modify the coloring in  pixcolor().
*/
class mndnewton : public mndynamics {
protected:
   uint degree;
   virtual void fixed(mdouble, mdouble) const {}
public:
   mndnewton(int subtype0) : mndynamics(subtype0)
   { degree = 0; bailout = 1e100; delete[] temp; temp = new mdouble[10]; }
   virtual int maxMode(int) const { if (degree) return 2; else return 1; }
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The class  mndcubicnewton  implements Newton's method for cubic polynomials
   with the roots  1 , -1/2 + c , -1/2 - c . The free critcal point is at 0.
*/
class mndcubicnewton : public mndnewton {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void fixed(mdouble a, mdouble b) const;
public:
   mndcubicnewton(int subtype0) : mndnewton(subtype0)
   { degree = 3; B = 0.866025403784438646763723170755L; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

/* The class  mndquarticnewton  implements Newton's method for various
   families of quartic polynomials.  There are two critical points (besides
   the roots), and by a critical relation,  only one is effectively free.
*/
class mndquarticnewton : public mndnewton {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void f2(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void f8(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void fixed(mdouble a, mdouble b) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndquarticnewton(int subtype0) : mndnewton(subtype0)
   { if (subtype != 3 && subtype != 5 && subtype != 8) degree = 4; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndlbrot : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   uint parabolic(mdouble x, mdouble y);
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
   int dist(mdouble a, mdouble b, mdouble x, mdouble y);
   int turn(mdouble a, mdouble b, mdouble x, mdouble y);
   int binary(mdouble a, mdouble b, mdouble x, mdouble y);
   int yoccoz(mdouble a, mdouble b, mdouble x, mdouble y);
   int newton3(mdouble a, mdouble b);
   int newton(mdouble a, mdouble b);
   int quadrant(mdouble a, mdouble b, mdouble x, mdouble y);
   int quadrantqn(mdouble a, mdouble b);
   int quadrantqnp(mdouble a, mdouble b);
public:
   mndlbrot(int subtype0) : mndynamics(subtype0)
   { subtype = 3; bailout = 16.0L; delete[] temp; temp = new mdouble[64]; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int maxMode(int sg) const
   { if (sg > 0) return 11; else return 9; }
   virtual void setDegree(int degree)
   { if (degree > 1) subtype = degree; }  //order of  q_n(c)
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int find(int sg, uint preper, uint per, mdouble &x, mdouble &y) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual void prepare(int sg, uint nmax, uint &dm, mdouble *t);
   virtual uint esctime(mdouble x, mdouble y);
   virtual uint pixcolor(mdouble x, mdouble y);
   virtual int turnsign(mdouble x, mdouble y);
   virtual mdouble green(int sg, mdouble x, mdouble y);
   virtual int similarity(uint preper, uint per, mdouble &a, mdouble &b,
     mdouble *t) const;
};

class mndmulti : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   //int dist(mdouble a, mdouble b, mdouble x, mdouble y);
   int turn(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mndmulti(int subtype0) : mndynamics(subtype0)
   {  bailout = 16.0L; if (subtype < 2) subtype = 4;
      delete[] temp; temp = new mdouble[64];
   }
   virtual int maxMode(int) const { return 6; }
   virtual void setDegree(int degree) { if (degree > 1) subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int find(int sg, uint preper, uint per, mdouble &x, mdouble &y) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual void prepare(int sg, uint nmax, uint &dm, mdouble *t);
   virtual uint pixcolor(mdouble x, mdouble y);
   virtual int turnsign(mdouble x, mdouble y);
   virtual mdouble green(int sg, mdouble x, mdouble y);
};

class mndbfpoly : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { x = 1.0L/(subtype + 1) - 1.0L; y = 0.0L; }
public:
   mndbfpoly(int subtype0) : mndynamics(subtype0)
   { if (subtype < 2) subtype = 2; bailout = 1.0e10L; A = 1.0L; }
   virtual void setDegree(int degree)
   { if (degree < 2) degree = 2; subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndcubic : public mndynamics {
protected:
   mutable mdouble rb, ib;
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   void fbasilica(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   void frabbit(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   uint marty(mdouble a, mdouble b, mdouble x, mdouble y);
public:
   mndcubic(int subtype0) : mndynamics(subtype0)
   {  A = -0.5L; bailout = 100.0L; rb = 0.0L; ib = 0.0L;
      if (subtype < 1 || subtype > 7) subtype = 1;
      if (subtype > 5) bailout = 1.0e10L;
   }
   virtual int maxMode(int) const
   { if (subtype <= 4) return 4; else return 3; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual int bifurcate(mdouble t, mdouble &a, mdouble &b) const;
   virtual void prepare(int sg, uint nmax, uint &dm, mdouble *t);
   virtual uint pixcolor(mdouble x, mdouble y);
};

class mandelQPPQ : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const
   { if (subtype & 1) { x = 0.0L; y = 0.0L; } else root(-a, -b, x, y); }   
public:
   mandelQPPQ(int subtype0) : mndynamics(subtype0)
   { A = 0.41421356237309505L; B = 0.0L; bailout = 100.0L; }
   virtual void setDegree(int degree)
   { if (degree < 3) subtype = degree; else subtype ^= 3; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndquartic : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble, mdouble, mdouble &x, mdouble &y) const
   { if (subtype == 2000) x = 0.0L; else x = 1.0L; y = 0.0L; }
public:
   mndquartic(int subtype0) : mndynamics(subtype0)
   {  A = 0.5L; if (subtype < 2000) A = 1.0L/(1 - subtype);
      bailout = 16.0L; if (subtype == 3000) bailout = 1.0e10L;
   }
   virtual void setDegree(int degree)
   { if (subtype < 2000 && degree < 2000 && degree >= 2) subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

class mndsurge : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void finv(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndsurge(int subtype0) : mndynamics(subtype0) { bailout = 6.0; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

/* The class  mndrealcubic  implements real cubic polynomials
   +-(z^3 - 3az) + b .  Note that the two families are conjugate by  z -> iz ,
   but not over the reals.  While the functions are complex analytic in z,
   they are only real analytic in the parameter  c = a + ib . The parameter
   plane is not a complex analytic subset of the two-dimensional complex
   parameter space.  The connectedness locus is not localy connected.  Since
   the two critical orbits are independent when  a > 0 , one may be escaping
   while the other stays bounded.  In this case the Julia set is disconnected
   but not totally disconnected.  In  drawmode 0,  these parameters are
   included in the connectedness locus,  and in drawmode 2,  they are marked
   gray.  Drawmode 1 should be disabled.

   The implementation relies on an unproved observation,  that e.g. for
   a, b > 0,  the smaller critical point and larger critical value is
   escaping faster.  Therefore you cannot derive another two-critical family
   from this class:  in general you should always check both critical orbits.
*/
class mndrealcubic : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void critical(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndrealcubic(int subtype0) : mndynamics(subtype0)
   { subtype = subtype0; bailout = 20.0L; }
   virtual int maxMode(int) const { return 2; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The Tricorn is the connectedness locus of the anti-analytic family
   f_c(z) = (z*)^2 + c , where  z*  is the complex conjugate of  z .  Since
   the second iterate  is a quartic polynomial,  (z^2 + c*)^2 + c , the Julia
   set is quite ordinary.  However,  the periods are unusual, e.g., there are
   4 fixed points for parameters in the main component.  The bounding curves
   consist of parabolic parameters.  The connectedness locus is not locally
   connected.  The analogous Multicorn is obtained for  f_c(z) = (z*)^q + c .
*/
class mndtricorn : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndtricorn(int subtype0) : mndynamics(subtype0)
   { subtype = subtype0; bailout = 16.0L; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual void setDegree(int degree) { if (degree > 1) subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
};

/* The class  mndhenon  implements real quadratic Henon mappings.  These are
   diffeomorphisms of  R^2 . Neglecting a decoupled case,  they are affine
   conjugate to  f_c(z) = (a + y - x^2) + i(bx) .  Now the dynamics may be
   dominated by stretching and folding,  and there are no critical orbits.
   For  |b| < 1  there may be attracting cycles or a strange attractor.
   For  b = -1  there seem to be Siegel disks.

   The forward orbit escapes when  2x + y < -(1 + 2a + 2(b/4 - 1)^2) , and
   the backwards orbit escapes when  x + 2y > 1 + 2a + 2(b - 1/4)^2 .
   The dynamic plane is colored depending on escape under forward iteration,
   backwards iteration,  or both.  The parameter plane is colored according
   to existence of periodic points,  which are attracting at most for |b| < 1:
   period 1 exists for  4a >= -(b - 1)^2 ,
   period 2 exists for  4a >= 3(b - 1)^2 ,
   period 3 exists for  4a >= 7b^2 + 10b + 7 ,
   period 4 exists for  4a >= 5b^2 - 6b + 5 .
   Here period 2 and period 4 arise from satellite bifurcations,  and it
   is not excluded that a primitive period 4 exists before the satellite one.
   The multiplier of period  n  is  +-1  on curves of the form
   a = b*k(b + 1/b) , and if it is real-analytic at  b = 0 , it must be a
   quadratic polynomial (with first and last coefficient equal).

   Note that for  a = 1.3201 , b = -0.2 , there is an attracting 2-cycle
   with large basins and at the same time an attracting 3-cycle at
   x = 0.0571573731988314 , y = 0.19599760197196 .
*/
class mndhenon : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   virtual void finv(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndhenon(int subtype0) : mndynamics(subtype0)
   { subtype = 1; bailout = 10000.0L; A = 1.4L; B = 0.3L; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 2; }
   virtual int maxMode(int) const { return 2; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The class  mndifs  computes Liouville zeros recursively in the
   parameter plane;  a zero is supected when  |P(z)| < delta*|P'(z)| ,
   where  P(z)  is a polynomial and  delta  is half pixel width.
   subtype 0 gives the Barnsley set with coefficients -1, 0, 1.
   subtype 1 gives the Bousch set with coefficients -1, 1.
   subtype 2 gives the Thurston set related to real kneading sequences.
   In the dynamic plane,  a green background is provided by  pixcolor() ,
   and the  QmnPlane  calls  iterate()  recursively to compute preimages
   for an  IFS , starting with a pre-fixed point.  Two different
   versions are used,  according to  Thierry Bousch  for  |c| < 1
   and according to  Bill Thurston  for  |c| > 1 .
*/
class mndifs : public mndynamics {
protected:
   mdouble X, Y;
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
   int recur1(mdouble x, mdouble y, mdouble xp, mdouble yp, uint n);
   int recur2(qulonglong N, mdouble x,mdouble y, mdouble xp,mdouble yp, uint n);
public:
   mndifs(int subtype0) : mndynamics(subtype0)
   { subtype = subtype0; A = 0.9052678569L; B = 1.283742172L; } //3/14
   //{ subtype = subtype0; A = 1.419643378; B = 0.6062907292; } //3/8
   //{ subtype = subtype0; A = -0.46332505824; B = 0.46153455809; }
   //{ subtype = subtype0; A = 1.083334375; B = 1.079147875; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 4; }
   virtual int maxMode(int) const { return 4; }
   virtual void setDegree(int degree) { subtype = degree; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The class  mndlambda  implements an illustration of binding entropy
   in the dynamic plane for the angle given by  A . In the parameter
   plane,  it provides a green background for drawing  lambda(theta) .
*/
class mndlambda : public mndynamics {
protected:
   virtual void f(mdouble a, mdouble b, mdouble &x, mdouble &y) const;
public:
   mndlambda(int subtype0) : mndynamics(subtype0)
   { subtype = 1; A = 1.0L/6.0L; B = 1.521379707L; }
   virtual void iterate(mdouble &x, mdouble &y, int mode = 0) const;
   virtual int minMode(int) const { return 4; }
   virtual int maxMode(int) const { return 4; }
   virtual void startPlane(int sg, mdouble &xmid, mdouble &rewidth) const;
   virtual uint pixcolor(mdouble x, mdouble y);
};

/* The Misiurewicz point  a  has multiplier  rho , preperiod  pre <= cent
   and period  per . Tan Lei's self-similarity implies that the rescaled
   Mandelbrot set  rho^n*(M - a)  has a limit.  When  newn < 0 , the
   function  set()  is reading in the parameters of a Misiurewicz point.
   When  oldn < 0 , it is setting viewport parameters for an initial
   image with  n = 0 , and  n  is changed from  oldn  to  newn  otherwise.
   Then the viewport parameters are rescaled accordingly.  In the
   application,  you may set the parameters and display an initial
   image.  The user may magnify a part of the image and change  n  to
   observe the asymptotic behavior.  The application must be able to
   display rotated images,  such that  cmid  is the center and  cmid + cwidth
   is the middle right of the viewport.  Rescaled images in the sense of
   Tan Lei are obtained with  gamma > 2 . For  gamma = 1 , 3/2, 7/4, ... 2 ,
   the images are rescaled with  rho^{gamma*n}  around the centers  c_n ,
   which have period  cent + n*per  and which are located asymptotically
   at  c_n ~ a + K*rho^{-n} . When setting parameters in the application,
   a  is determined with  mndlbrot::find() , rho is computed using explicit
   formulas,  f() , or  find() periodic, and  K  is approximately  c_0 - a .
*/
class mndScale {
protected:
   uint cent, pre, per;
   mdouble ra, ia, rlogrho, ilogrho, rk, ik;
   void expmult(mdouble t, mdouble &x, mdouble &y);
public:
   mndScale() {};
   uint set(int oldn, int newn, mdouble *t);
   static int find(uint per, mdouble &x, mdouble &y);
};

/* The class mndAngle represents an angle (in turns) by a fraction of
   unsigned long long integers , which are <= 2^64 - 1 . They are normalized
   to the denominator  2^K * (2^P - 1)  when possible,  i.e.,  K + P <= 64 .
   Otherwise  P = 0 . Many applications only need the static functions.
   normalize()   returns the period.
   twice()       doubles the angle modulo 1.
   conjugate()   computes the conjugate angle,  returns period of the char.pt.
   wake()        computes the angles of a limb.
   radians()     gives the angle in radians.
   lambda()      computes  e^{core entropy h}  for dyadic angles.
//   realSpider()  checks if the root is real and computes the center  c .
   truncatedTuning()   truncates  (u1, u2)*w  to the next  n-periodic angle.
*/
class mndAngle {
private:  //disabled
   mndAngle(const mndAngle &);
   mndAngle &operator=(const mndAngle &);
protected:
   int K, P;
   qulonglong N, D;
public:
   static qulonglong truncatedTuning(int k, qulonglong u1, qulonglong u2,
      int q, qulonglong w, int n = 0);
   static int normalize(qulonglong &n, qulonglong &d, int &k);
   static void twice(qulonglong &n, qulonglong &d);
   static int conjugate(qulonglong &n, qulonglong &d);
   static qulonglong wake(int k, int r, qulonglong &n);
   static mdouble radians(qulonglong n, qulonglong d);
// static int realSpider(qulonglong n, qulonglong d, mdouble &c);
   mndAngle() { K = 0; P = 1; N = 0LL; D = 1LL; }
   int setAngle(qulonglong n, qulonglong d, int &k);
   int getAngle(qulonglong &n, qulonglong &d)const { n = N; d = D; return P; }
   void twice() { twice(N, D); if (K) K--; }
   mdouble radians() const { return radians(N, D); }
   static mdouble lambda(qulonglong N, qulonglong D,
      mdouble eps = 1.0e-8L, int nmax = 100);
};

/* The class  mndCombi  represents a *-periodic kneading sequence with period
   P <= 64  and the corresponding internal address.  They are stored each
   in the bits of an  unsigned long long integer . QString  conversion is
   done by  QmnCombiDialog . In the address,  the first bit (2^63) is
   period 64 and the last bit (2^0) is period 1.  Example:
     AABBAA*  is  0...01100111  since the upper kneading sequence is  AABBAAA
     1-3-4-7  is  0...01001101
   The kneading sequence is always the upper one:  setKneading() ignores the
   last entry,  and the last entry from  getKneading()  shall be printed as
   a  '*' . The set-functions compute the internal address from the kneading
   sequence or vice versa.
   fromAngle()  computes the combinatorics of a periodic angle.
   renorm()  gives the lowest period of simple renormalization.
   count()  returns the number of realizations,  assuming admissibility.
   failsBS()  returns the first failure of the Bruin-Schleicher admissibility
   condition,  see  http://arXiv.org/abs/0801.4662 .
   0 means that there is no evil orbit and the combinatorial data corresponds
   to a planar Hubbard tree with an orientation-preserving mapping,  thus to
   a quadratic polynomial realizing these combinatorics.  The corresponding
   angles are computed in  toAngles()  by backwards iteration for each period,
   always choosing the  1/q-limb when  q > 2 . The simple algorithm fails for
   the first time at  1-2-6-7-13-14 , so the wake is subdivided by calling
   backAngles()  recursively.  See the explanation in  mndcombi.cpp .
*/
class mndCombi {
private:  //disabled
   mndCombi(const mndCombi &);
   mndCombi &operator=(const mndCombi &);
protected:
   int P;
   qulonglong K, A;
   int backAngles(int n, qulonglong &n1, qulonglong &n2, qulonglong d) const;
public:
   static int phi(int n); //Euler totient
   mndCombi() { P = 0; } //invalid
   int rho(int m) const;
   int setKneading(qulonglong k);
   int setAddress(qulonglong a);
   int getKneading(qulonglong &k) const { k = K; return P; }
   int getAddress(qulonglong &a) const { a = A; return P; }
   int fromAngle(qulonglong n, qulonglong d);
   int renorm() const;
   int failsBS() const;
   int count() const;
   int toAngles(qulonglong &n1, qulonglong &n2, qulonglong &d) const;
};

class mandelPath {
private: //disabled
   mandelPath(const mandelPath &);
   mandelPath &operator=(const mandelPath &);
protected:
   int M, draw;
public:
   static void root(mdouble x, mdouble y, mdouble &u, mdouble &v);
   mandelPath(int draw1 = 0, int M1 = 150) { M = M1; draw = draw1; }
   virtual ~mandelPath() { }
   virtual int capture(int, int, mandelPathinfo *) { return 0; }
   virtual int spider(qulonglong, qulonglong, mdouble logR1 = logTen,
      int upper = 0) { return upper*((int)(logR1)); }
   virtual int mating(int, int, mdouble, mdouble, mdouble logR1 = logTen,
      int upper = 0) { return upper*((int)(logR1)); }
   virtual int anti(int, int, mdouble, mdouble, mdouble logR1 = logTen,
      int upper = 0) { return upper*((int)(logR1)); }
   virtual int step(mdouble &, mdouble &) { return 0; }
   virtual int sequence(int, int, int, mandelPathinfo *) { return 0; }
};

class mandelPathMate : public mandelPath {
protected:
   int k, p, K, P;
   mdouble rp, ip, rq, iq, logR, *X, *Y, *X0, *Y0;
public:
   mdouble rD, iD;
   mandelPathMate(int draw1 = 0, int M1 = 150) : mandelPath(draw1, M1)
   { k = 0; p = 0; K = 0; P = 0; X = 0; Y = 0; X0 = 0; Y0 = 0; logR = 1.0L;
     rD = 0.0L; iD = 0.0L; }
   virtual ~mandelPathMate()
   { delete[] X0; delete[] Y0; delete[] X; delete[] Y; }
// virtual int spider(qulonglong N, qulonglong D, mdouble logR1 = logTen,
//    int dummy = 0);
   virtual int mating(int k0, int p0, mdouble rc, mdouble ic,
      mdouble logR1 = logTen, int which = 0);
   virtual int step(mdouble &rc, mdouble &ic);
// virtual int sequence(int n, int f, int fps, mandelPathinfo *mpi);
};

class mandelPathTwo : public mandelPath {
protected:
   int type, k, p;
   mdouble rp, ip, twoLogR, *X, *Y, *X0, *Y0;
public:
   mandelPathTwo(int draw1 = 0, int M1 = 150) : mandelPath(draw1, M1)
   { type = 0; X = 0; Y = 0; X0 = 0; Y0 = 0; }
   virtual ~mandelPathTwo()
   { delete[] X0; delete[] Y0; delete[] X; delete[] Y; }
   virtual int capture(int k0, int p0, mandelPathinfo *mpi);
// virtual int spider(qulonglong N, qulonglong D, mdouble logR1 = logTen,
//    int dummy = 0);
   virtual int mating(int k0, int p0, mdouble rc, mdouble ic,
      mdouble logR1 = logTen, int dummy = 0);
   virtual int anti(int k0, int p0, mdouble rQ, mdouble iQ,
      mdouble logR1 = logTen, int dummy = 0);
   virtual int step(mdouble &rc, mdouble &ic);
   virtual int sequence(int n, int f, int fps, mandelPathinfo *mpi);
};

/*
class mandelPathThree : public mandelPath {
protected:
   int type, k, p;
   mdouble rp, ip, rq, iq, twoLogR, *X, *Y, *X0, *Y0, *U, *V, *U0, *V0;
public:
   mandelPathThree(int draw1 = 0, int M1 = 150) : mandelPath(draw1, M1)
   {  type = 0; X = 0; Y = 0; X0 = 0; Y0 = 0;
      U = 0; V = 0; U0 = 0; V0 = 0;
   }
   virtual ~mandelPathThree()
   {  delete[] U0; delete[] V0; delete[] U; delete[] V;
      delete[] X0; delete[] Y0; delete[] X; delete[] Y;
   }
   virtual int capture(int k0, int p0, mandelPathinfo *mpi);
   virtual int spider(qulonglong N, qulonglong D, mdouble logR1 = logTen,
      int which = 2);
   virtual int mating(int k0, int p0, mdouble rc, mdouble ic,
      mdouble logR1 = logTen, int which = 2);
   virtual int step(mdouble &rc, mdouble &ic);
   virtual int sequence(int n, int f, int fps, mandelPathinfo *mpi);
};//*/

class mandelbrotPath : public mandelPath {
protected:
   int Ms, Mmax, steps, k, p;
   mdouble *X, *Y;
   int gait(mdouble &rc, mdouble &ic);
   int checks(mdouble ru, mdouble iu, mdouble rv, mdouble iv, mdouble rw,
      mdouble iw);
   int checkp(mdouble ra, mdouble ia, mdouble ru, mdouble iu,
      mdouble rc, mdouble ic, mdouble rv, mdouble iv);
public:
   mandelbrotPath(int draw1 = 0, int M1 = 150) : mandelPath(draw1, M1)
   { Ms = 0; Mmax = 0; steps = -1; X = 0; Y = 0; }
   virtual ~mandelbrotPath() { delete[] X; delete[] Y; }
   virtual int capture(int k0, int p0, mandelPathinfo *mpi);
   virtual int spider(qulonglong N, qulonglong D, mdouble logR = 2.0*logTen,
      int dummy = 0);
   virtual int step(mdouble &rc, mdouble &ic);
// virtual int sequence(int n, int f, int fps, mandelPathinfo *mpi);
};

class mandelPathExpo : public mandelPath {
protected:
   int Ms, Mmax, steps, k, p;
   mdouble *X, *Y;
   int gait(mdouble &rc, mdouble &ic);
   int checks(mdouble ru, mdouble iu, mdouble rv, mdouble iv, mdouble rw,
      mdouble iw);
public:
   mandelPathExpo(int draw1 = 0, int M1 = 150) : mandelPath(draw1, M1)
   { Ms = 0; Mmax = 0; steps = -1; X = 0; Y = 0; }
   virtual ~mandelPathExpo() { delete[] X; delete[] Y; }
   virtual int spider(qulonglong N, qulonglong D, mdouble R = 0.0L, int d = 0);
   virtual int step(mdouble &rc, mdouble &ic);
// virtual int sequence(int n, int f, int fps, mandelPathinfo *mpi);
};
#endif //MNDYNAMICS_H_INCLUDED

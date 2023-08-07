/* qmnplane.h by Wolf Jung (C) 2007-2019.
   Declares classes QmnPlane, QmnDraw.

   These classes are part of Mandel 5.17, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/
#ifndef QMNPLANE_H_INCLUDED
#define QMNPLANE_H_INCLUDED

#if (QT_VERSION >= 0x050000)
#include <QtWidgets>
#else
#include <QWidget>
#endif
#include <QMouseEvent>
#include <QThread>

#include "mndynamo.h" //at present only for typedef mdouble

class QMutex;
class QPainter;
class QImage;
class QSize;
//class mndynamics;
//class mndPath;

/* QmnDraw  and  QmnPlane  provide a graphical display of  mndynamics
   in a possibly rotated coordinate system,  such that the center of the
   rectangle and the midpoint of the right side are given by
   xmid + i ymid  and  rewidth + i imwidth ...

   QmnDraw is designed to draw, e.g., the Mandelbrot set in a separate
   thread. Drawing is done on an explicitly shared QImage. You may
   1) Display the QImage as a whole when finished() is emitted, or wait().
   2) Display the QImage piece-by-piece when drawn() is emitted. In this
      case you should protect its use in, e.g., PaintEvents with the mutex.
   After creating a QmnDraw object, before starting the thread with draw(),
   you must initialize the coordinate system with setRectPlane() and the
   mndynamics with prepare(). The coordinate system may imply a rectangular
   subset of the QImage (e.g., for renormalization). The mode parameter in
   draw() means: 0 standard, 1 not cleared, 2 exterior is transparent.
   Check if QmnDraw isRunning(), ask it to stop at the next line with stop().
   For its protected QmnDraw member, QmnPlane provides the signal finished()
   and the functions isRunning() and stop(). The latter in fact stops & waits.

   PaintEvents of QmnPlane would be more efficient by using a QPixmap instead
   of the QImage buffer. However, since QPixmaps must not be used outside of
   the main thread, we would have to use an additional QImage shared with
   QmnDraw, and draw it onto the QPixmap in display().

   Depending on the drawmode, ...

   In order to synchronize a QmnPlane object with its QmnDraw member in a
   clean and safe way, drawing is stopped before changing, e.g., the QImage
   buffer. Tracing and marking is disabled while the drawing thread is
   running, and drawing the buffer on the screen is protected with a mutex.
   Note that an object f of mndynamics is shared with the QmnDraw thread from
   the calling thread, either directly with QmnDraw::draw(), or indirectly
   with QmnPlane::draw(). Drawing will be disturbed by invoking f->prepare()
   or f->setParameter() from the calling thread. In particular, it is unsafe
   to draw with different QmnPlane objects at the same time, if the same
   mndynamics object is shared.

   Further functions ...
*/

class QmnDraw : public QThread
{
   Q_OBJECT
public:
   QmnDraw(QObject *parent = 0) : QThread(parent)
   { stopnow = -2; rot = 0; }
   ~QmnDraw();
   void setRectPlane(int i0, int k0, int ix, int kx,
      mdouble h0, mdouble v0, mdouble pw0, mdouble ph0, mdouble *rot0 = 0);
   void draw(mndynamics *f0, uint dm0, int mode0, QImage *b0, QMutex *m0 = 0,
      int alpha0 = 255);
   void stop();
protected:
   uint drawmode;
   int imid, kmid, imax, kmax, mode, alpha; volatile int stopnow;
   mdouble hmid, vmid, pw, ph, *rot;
   mndynamics *f; QImage *buffer; QMutex *mutex;
   virtual void run();
   unsigned long int posToPoint(int i, int k, mdouble &x, mdouble &y) const;
   virtual void drawSphere();
   virtual void drawPlane();
signals:
   void drawn(int kLast);
};

class QmnPlane : public QWidget
{
   Q_OBJECT
public:
   QmnPlane(int w = 8, int h = 8, int type0 = 0, QWidget *parent = 0);
   ~QmnPlane();
   bool framed() { return (hframe > 0); }
   void setActive(bool activate)
   {active = activate; setCursor(active ? Qt::CrossCursor : Qt::ArrowCursor);}
   bool isActive() { return active; }
   bool isBackEnabled() { return bool(oldnmax); }
   int hasPathLength()
   { if (ypath[0] == 3) return xpath[0]; else return -ypath[0]; }
   QSize sizeHint() { return QSize(2*imax, 2*kmax); }
   void setType(int type0);
   int getType() const { return type; }
   void resize(int w, int h);
   void setPlane(mdouble xmid, mdouble ymid, mdouble rewidth, mdouble imwidth,
      bool nowayBack = true);
   void getPlane(mdouble &xmid, mdouble &ymid, mdouble &rewidth,
     mdouble &imwidth) const;
   void setPoint(mdouble x0, mdouble y0);
   void getPoint(mdouble &x0, mdouble &y0) const { x0 = x; y0 = y; };
   void getUserPath(int M, mdouble *X, mdouble *Y);
   int pointToPos(mdouble x0, mdouble y0, int &i, int &k) const;
   unsigned long int posToPoint(int i, int k, mdouble &x0, mdouble &y0) const;
   void setNmax(uint n) { nmax = n; }
   void getNmax(uint &n) const { n = nmax; }
   void setCursorPix(QPixmap *pix = 0) { cursor = pix; update(); }
   void paintEvent(QPaintEvent *event);
   void move(int mode, int u = 0, int v = 0);
   void Move(int mode, mndynamics *f);
   void draw(mndynamics *f, int signtype, uint *drawmode, int mode = 0,
      int alpha = 255);
   void recur(mndynamics *f, mdouble x0, mdouble y0, uint n);
   void drawLambda(int step = 8);
   void drawOrbit(mndynamics *f, mdouble &x0, mdouble &y0,
     int preiter, int plotiter);
   void drawLine(mdouble x1, mdouble y1, mdouble x2, mdouble y2,
     QColor color = Qt::white);
   void drawEllipse(mdouble x0, mdouble y0, mdouble rx, mdouble ry,
     QColor color = Qt::white, bool filled = false);
   void drawOrtho(mdouble alpha, mdouble beta, QColor color = Qt::white);
   void drawLamination(mdouble alpha, mdouble beta, uint n);
//   void drawPathSegment(mndPath *path, QColor color = Qt::white);
   void floodFill(int i, int k, QRgb rgbold, QRgb rgbnew);
   void green(mndynamics *f, int st, mdouble g, int quality = 5,
     QColor color = Qt::white);
   int traceRay(int signtype, mdouble t, mndynamics *f,
     mdouble &x0, mdouble &y0, int quality = 5, QColor color = Qt::white);
   int backRay(qulonglong num, qulonglong denom, mdouble &a, mdouble &b,
     const int quality = 5, QColor color = Qt::white, int mode = 1);
   int newtonRay(int signtype, qulonglong num, qulonglong denom, mdouble &a,
     mdouble &b, int quality = 5, QColor color = Qt::white, int mode = 1);
   int newtonRayGeneralCubic(qulonglong N1, qulonglong N2, mdouble cr, mdouble ci, mdouble br, mdouble bi, int q);
   int rayNewtonGeneralCubic(uint n, mdouble cr, mdouble ci,
                             mdouble br, mdouble by, mdouble&x, mdouble& y,mdouble rlog, mdouble ilog);
   int rayNewton(int signtype, uint n, mdouble a, mdouble b,
     mdouble &x, mdouble &y, mdouble rlog, mdouble ilog);
   int newtonRayC(int signtype, qulonglong num, qulonglong denom, mdouble &a,
                 mdouble &b, int quality = 5, QColor color = Qt::white, int mode = 1);
   int rayNewtonC(int signtype, uint n, mdouble a, mdouble b,
                 mdouble &x, mdouble &y, mdouble rlog, mdouble ilog);
   int tricornNewton(int signtype, uint n, mdouble a, mdouble b,
     mdouble &x, mdouble &y, mdouble rlog, mdouble ilog);
   bool saveEPS(int bpp, const QString &fileName);
   bool saveTXT(const QString &fileName);
   bool savePNG(const QString &fileName, QmnPlane *plane = 0, int mark = 0);
   bool loadPNG(const QString &fileName);
   void catMap(int mode);
   void tilt(int mode);
   void replaceColor(int p, int q);
   void overlay(QmnPlane *plane, QColor *color = 0);
   void stop();
   bool isRunning() const;
public slots:
   void display(int kLast);
protected:
   const static int Mmax;
   uint nmax, oldnmax, oldmode;
   int imax, kmax, hframe, vframe, type, *xpath, *ypath;
   mdouble x, y, hmid, vmid, pw, ph,
     oldhmid, oldvmid, oldrewidth, oldimwidth, *rot;
   QMutex *mutex;
   QImage *buffer, *oldbuffer;
   QPixmap *cursor;
   QmnDraw *thread;
   bool rubberBandShown, active;
   QRect rubberBand;
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
signals:
   void moved();
   void activated();
   void backMaybeChanged();
   void finished();
   void completed();
   void drawRequest();
};

#endif //QMNPLANE_H_INCLUDED

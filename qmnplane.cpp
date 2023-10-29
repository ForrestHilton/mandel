/* qmnplane.cpp by Wolf Jung (C) 2007-2023.
   Defines classes QmnPlane, QmnDraw.

   These classes are part of Mandel 5.19, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/

#include <cmath>
#include <QtGui>
#if (QT_VERSION >= 0x050000)
#include <QtWidgets>
#endif
#include "qmnplane.h"
//#include "mndynamo.h"

QmnDraw::~QmnDraw() { stopnow = 1; wait(); delete[] rot; }

void QmnDraw::setRectPlane(int i0, int k0, int ix, int kx,
   mdouble h0, mdouble v0, mdouble pw0, mdouble ph0, mdouble *rot0) // = 0
{  if (isRunning()) { stopnow = 1; wait(); }
   if (rot0)
   {  rot = new mdouble[9];
      for (imax = 0; imax <= 8; imax++) rot[imax] = rot0[imax];
   }
   imid = i0; kmid = k0; imax = ix; kmax = kx; hmid = h0; vmid = v0;
   pw = pw0; ph = ph0;
   if (imid >= imax && kmid >= kmax && !(kmax & 3)) stopnow = -1;
}

void QmnDraw::draw(mndynamics *f0, uint dm0, int mode0, QImage *b0,
   QMutex *m0, int alpha0) // = 0, = 255
{  if (stopnow != -1) return; f = f0;
   drawmode = dm0; mode = mode0; buffer = b0; mutex = m0; alpha = alpha0;
   if (imid+imax-1 <= buffer->width() && kmid+kmax-1 <= buffer->height())
   { stopnow = 0; start(QThread::LowPriority); }
}

void QmnDraw::stop()
{ if (isRunning()) stopnow = 1; }

//mode: -1 sphere, 0 standard, 1 not cleared by caller, 2 exterior transparent
void QmnDraw::run()
{  if (mode < 0) drawSphere(); else drawPlane(); }

/*This function must not be called for rot = 0,  but this is not checked
  to save time. It maps the outside of the disk to its boundary,  to
  allow the computation at points close to the boundary of the disk.
  The return value is a gray shade with bytes < 128.
*/
unsigned long int QmnDraw::posToPoint(int i, int k, mdouble &x, mdouble &y)const
{  unsigned long int shade = 0x00010101L;
   mdouble u = pw*(i - imid), v = pw*(kmid - k), w; x = u*u + v*v;
   if (x <= 1.0)
   { x = 1.0-x; w = sqrt(x); x += w*(v - u); i = 27 + int(73*x); shade *= i; }
   else { x = sqrt(x); u /= x; v /= x; w = 0; shade = 0; }
   x = rot[0]*u + rot[1]*v + rot[2]*w;
   y = rot[3]*u + rot[4]*v + rot[5]*w;
   w = rot[6]*u + rot[7]*v + rot[8]*w; w = 1.0 - w;
   if (w < 1e-20) w = 1e-20; x /= w; y /= w; return shade;
}

void QmnDraw::drawSphere()//QColor::fromRgb(test);
{  if (!rot) return;
   unsigned long int myrgb[] = {0x000000L, 0x000080L, 0x008000L, 0x008080L,
     0x800000L, 0x800080L, 0x808000L, 0x808080L};
   QImage *hLine = new QImage(imid + imax, 1, QImage::Format_RGB32);
   QPainter *q = new QPainter(hLine); QPainter *p = new QPainter();
   int i, n, k, r =(4*kmax)/5; uint cl; unsigned long int shade; mdouble x, y;
   if (drawmode > 1 && drawmode <= 15)
   {  for (k = kmid - r; k <= kmid + r; k++)
      {  if (stopnow) break; hLine->fill(QColor(Qt::darkGray).rgba());
         x = 0.64*kmax*kmax - (k-kmid)*(k-kmid);
         n = (x > 0 ? int(floor(sqrt(x))) : 0);
         for (i = imid - n; i <= imid + n; i++)
         {  shade = posToPoint(i, k, x, y); cl = f->pixcolor(x, y);
            if (drawmode == 3 && cl)
               q->setPen(QColor::fromHsv(cl & 255,
                   255 - (shade & 127), 128 + (shade & 127)));
            else q->setPen(QColor::fromRgb(shade + myrgb[cl & 7]));
            q->drawPoint(i, 0);
         }
         if (mutex) mutex->lock(); p->begin(buffer);
         p->drawImage(imid-imax, k, *hLine, imid-imax, 0, 2*imax, 1);
         p->end(); if (mutex) mutex->unlock();
         if (!((k + 1) & 7)) emit drawn(k);
      } //for k
   }
   else
   {  int v, w; uint u, cl0 = drawmode & 7; cl = 2;
      if (!drawmode) { cl0 = 5; cl = 0; }
      unsigned long int oldshade, *shades = new unsigned long int[kmax+r+2];
      uint *last, *now, *next, *tempo;
      uint *row1 = new uint[kmax+r+2], *row2 = new uint[kmax+r+2],
         *row3 = new uint[kmax+r+2];
      last = row1; now = row2; next = row3;
      for (i = -r - 1; i <= r + 1; i++)
      {  posToPoint(imid + i, kmid - r - 1, x, y);
         last[kmax + i] = f->esctime(x, y);
      }
      for (i = -r - 1; i <= r + 1; i++)
      {  shades[kmax + i] = posToPoint(imid + i, kmid - r, x, y);
         now[kmax + i] = f->esctime(x, y);
      }
      for (k = kmid - r; k <= kmid + r; k++)
      {  if (stopnow) break; hLine->fill(QColor(Qt::darkGray).rgba());
         x = 0.64*kmax*kmax - (k-kmid)*(k-kmid);
         n = (x > 0 ? int(floor(sqrt(x))) : 0);
         posToPoint(imid - r - 1, k + 1, x, y);
         next[kmax - r - 1] = f->esctime(x, y);
         oldshade = shades[kmax - r];
         shades[kmax - r] = posToPoint(imid - r, k + 1, x, y);
         next[kmax - r] = f->esctime(x, y);
         for (i = -r; i <= r; i++)
         {  shade = oldshade; oldshade = shades[kmax + i + 1];
            shades[kmax + i + 1] = posToPoint(imid + i + 1, k + 1, x, y);
            next[kmax + i + 1] = f->esctime(x, y);
            if (i < -n || i > n) continue;
            if (now[kmax + i] > 65000) shade += myrgb[now[kmax + i] & 7];
            else
            {  w = 0; u = now[kmax + i];
               for (v = kmax + i - 1; v <= kmax + i + 1; v++)
               { if (last[v] < u) w++; if (next[v] < u) w++; }
               if (now[kmax + i - 1] < u) w++; if (now[kmax + i + 1] < u) w++;
               if (w > 4) shade += myrgb[cl0]; else shade += myrgb[cl];
            }
            q->setPen(QColor::fromRgb(shade)); q->drawPoint(imid + i, 0);
         } //for i
         tempo = last; last = now; now = next; next = tempo;
         if (mutex) mutex->lock(); p->begin(buffer);
         p->drawImage(imid-imax, k, *hLine, imid-imax, 0, 2*imax, 1);
         p->end(); if (mutex) mutex->unlock();
         if (!((k + 1) & 7)) emit drawn(k);
      } //for k
      delete[] row3; delete[] row2; delete[] row1; delete[] shades;
   }
   emit drawn(0);
   delete p; delete q; delete hLine; delete[] rot; rot = 0;
} //QmnDraw::drawSphere

void QmnDraw::drawPlane()
{  QColor mycolor[] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan,
      Qt::darkRed, Qt::darkMagenta, Qt::darkYellow, Qt::lightGray,
      Qt::darkGray, Qt::blue, Qt::green, Qt::cyan, Qt::red, Qt::magenta,
      Qt::yellow, Qt::white}; //is VGA
   QImage *hLine = new QImage(imid + imax, 1, QImage::Format_RGB32);
   QPainter *q = new QPainter(hLine); QPainter *p = new QPainter();
   int i, n, k; uint cl0, cl; mdouble x, y;
   if (drawmode > 1 && drawmode <= 15)
   {  for (k = -kmax; k < kmax; k++)
      {  if (stopnow) break;
         n = -imax; x = hmid + k*ph; y = vmid - k*pw;
         cl0 = f->pixcolor(x - imax*pw, y - imax*ph);
         for (i = 1 - imax; i < imax; i++)
         {  cl = f->pixcolor(x + i*pw, y + i*ph);
            if (cl != cl0)
            {  if (drawmode == 3 && cl0)
                  q->setPen(QColor::fromHsv(cl0 & 255, 255, 255));
                  //q->setPen(QColor::fromHsv(0, 255, cl0 & 255));
                  //q->setPen(QColor::fromRgb(cl0, cl0, cl0));
                  //q->setPen(QColor::fromRgb(cl0, 0, 255 - cl0));
               else q->setPen(mycolor[cl0]);
#if (QT_VERSION >= 0x040800) && (QT_VERSION < 0x040804)
//see  https://bugreports.qt-project.org/browse/QTBUG-26013
               for (int h = imid+n; h < imid+i; h++) q->drawPoint(h, 0);
#else
               q->drawLine(imid+n, 0, imid+i-1, 0);
#endif
               cl0 = cl; n = i;
            }
         }
         if (drawmode == 3 && cl0)
            q->setPen(QColor::fromHsv(cl0 & 255, 255, 255));
         else q->setPen(mycolor[cl0]);
#if (QT_VERSION >= 0x040800) && (QT_VERSION < 0x040804)
         for (int h = imid+n; h < imid+imax; h++) q->drawPoint(h, 0);
#else
         q->drawLine(imid+n, 0, imid+imax-1, 0);
#endif
         if (mutex) mutex->lock(); p->begin(buffer);
         p->drawImage(imid-imax, kmid+k, *hLine, imid-imax, 0, 2*imax, 1);
         p->end(); if (mutex) mutex->unlock();
         if (!((kmid + k + 1) & 7)) emit drawn(kmid+k);
      } //for k
   }
   else
   {  int v; uint u; cl0 = drawmode & 15; cl = 10;
      if (!drawmode) { cl0 = 13; cl = 0; }
      if (drawmode > 255) cl0 = 13;
      if (alpha >= 0 && alpha < 255) mycolor[cl0].setAlpha(alpha);
      uint *last, *now, *next, *tempo;
      uint *row1 = new uint[2*imax+2], *row2 = new uint[2*imax+2],
         *row3 = new uint[2*imax+2];    //no throw ...
      last = row1; now = row2; next = row3;
      x = hmid - (kmax+1)*ph; y = vmid + (kmax+1)*pw;
      for (i = -imax-1; i <= imax; i++)
        last[i+imax+1] = f->esctime(x + i*pw, y + i*ph);
      x = hmid - kmax*ph; y = vmid + kmax*pw;
      for (i = -imax-1; i <= imax; i++)
        now[i+imax+1] = f->esctime(x + i*pw, y + i*ph);
      for (k = -kmax; k < kmax; k++)
      {  if (stopnow) break;
         if (mode == 2)
            q->drawImage(imid-imax, 0, *buffer, imid-imax, kmid+k, 2*imax, 1);
         else hLine->fill(mycolor[cl].rgba());
         x = hmid + (k+1)*ph; y = vmid - (k+1)*pw;
         for (i = -imax-1; i <= imax; i++)
           next[i+imax+1] = f->esctime(x + i*pw, y + i*ph);
         for (i = -imax; i < imax; i++)
         {  if (now[i+imax+1] > 65000)
            {  q->setPen(mycolor[now[i+imax+1] & 15]);
               q->drawPoint(imid + i, 0);
            }
            else
            {  n = 0; u = now[i+imax+1];
               for (v = i+imax; v <= i+imax+2; v++)
               { if (last[v] < u) n++; if (next[v] < u) n++; }
               if (now[i+imax] < u) n++; if (now[i+imax+2] < u) n++;
               if (n > 4)
               {  q->setPen(mycolor[cl0]);
                  q->drawPoint(imid + i, 0);
               }
            }
         } //for i
         tempo = last; last = now; now = next; next = tempo;
         if (mutex) mutex->lock(); p->begin(buffer);
         p->drawImage(imid-imax, kmid+k, *hLine, imid-imax, 0, 2*imax, 1);
         p->end(); if (mutex) mutex->unlock();
         if (!((kmid + k + 1) & 7)) emit drawn(kmid+k);
      } //for k
      delete[] row3; delete[] row2; delete[] row1;
   }
   emit drawn(0);
   delete p; delete q; delete hLine;
} //QmnDraw::drawPlane

const int QmnPlane::Mmax = 1600;
//1...M, M = xpath[0], ypath[0] = 0 disabled, 1 empty, 2 partial, 3 finished

QmnPlane::QmnPlane(int w, int h, int type0, QWidget *parent)
   : QWidget(parent) // = 8 8 0 0
{  setAttribute(Qt::WA_OpaquePaintEvent);
   setFocusPolicy(Qt::NoFocus);
   buffer = 0; oldbuffer = 0;
   mutex = new QMutex(); thread = new QmnDraw();
   connect(thread, SIGNAL(drawn(int)), this, SLOT(display(int)));
   //connect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
   rubberBandShown = false; active = false; //default mouse cursor
   xpath = new int[Mmax + 1]; ypath = new int[Mmax + 1];
   imax = 4; kmax = 4; rot = 0; type = 0; setType(type0);
   x = 0; y = 0; oldnmax = 0; cursor = 0; ypath[0] = 0;
   resize(w, h);
}
//problem if setType refuses ...

QmnPlane::~QmnPlane()
{  delete thread; delete oldbuffer; delete buffer;
   delete [] xpath; delete[] ypath; delete[] rot;
}

void QmnPlane::setType(int type0)
{  if ( (type0 < 0 && imax < kmax) || (type0 == 2 && imax != kmax) ) return;
   hframe = 0; rubberBandShown = false; if (ypath[0] > 1) ypath[0] = 1;
   hmid = 0; vmid = 0; pw = 1.0; ph = 1.0;
   delete[] rot; rot = 0; type = type0; if (type >= 0) return;
   ph = 0.8*kmax; pw = 1.0/ph; rot = new mdouble[9]; setPlane(0, 0, 0, 0);
}

void QmnPlane::resize(int w, int h)
{  if (w > 20000) w = 20000; if (h > 20000) h = 20000;
   kmax = imax; if (type == 2) h = w; if (type < 0 && w < h) w = h;
   imax = w/2; if (imax < 4) imax = 4;
   imax -= (imax & 3); //w is divisible by 8 (simplifies .eps, maximal frame)
   if (type >= 0)
   { mdouble fs = mdouble(kmax)/mdouble(imax); pw *= fs; ph*= fs; }
   kmax = h/2; if (kmax < 4) kmax = 4;
   kmax -= (kmax & 3); //h is divisible by 8 (repaint in QmnDraw, max frame)
   if (type < 0) { ph = 0.8*kmax; pw = 1.0/ph; }
   hframe = 0; rubberBandShown = false; if (ypath[0] > 1) ypath[0] = 1;
   stop(); setFixedSize(2*imax, 2*kmax);
   delete oldbuffer; delete buffer;
   buffer = new QImage(2*imax, 2*kmax, QImage::Format_RGB32);
   oldbuffer = new QImage();
   updateGeometry();
}

void QmnPlane::setPlane(mdouble xmid, mdouble ymid, mdouble rewidth,
   mdouble imwidth, bool nowayBack) // = true
{  if (type < 0)
   {  for (int j = 0; j <= 8; j++) rot[j] = 0;
      rot[0] = 1.0; rot[5] = -1.0; rot[7] = 1.0; //infinity at top
      //rot[0] = 1.0; rot[4] = 1.0; rot[8] = 1.0; //infinity at center
      //rot[0] = 1.0; rot[4] = -1.0; rot[8] = -1.0; //infinity at back
   }
   else if (!type)
   { pw = rewidth/imax; ph = imwidth/imax; hmid = xmid; vmid = ymid; }
   else
   {  if (type == 2) { ymid = vmid; imwidth = rewidth; }
      hmid = xmid; pw = rewidth - xmid; vmid = imwidth; ph = imwidth - ymid;
   }
   if (nowayBack) { oldnmax = 0; emit backMaybeChanged(); }
   hframe = 0; rubberBandShown = false; if (ypath[0] > 1) ypath[0] = 1;
}

//type > 0: x1 y1 x2 y2

void QmnPlane::getPlane(mdouble &xmid, mdouble &ymid, mdouble &rewidth,
  mdouble &imwidth) const
{  if (type < 0) return;
   if (!type)
   { xmid = hmid; ymid = vmid; rewidth = pw*imax; imwidth = ph*imax; }
   else
   { xmid = hmid; ymid = vmid - ph; rewidth = hmid + pw; imwidth = vmid; }
}

void QmnPlane::setPoint(mdouble x0, mdouble y0)
{  if (x0*x0 + y0*y0 > 1e22) return; if (type == 2) y0 = x0;
   x = x0; y = y0; hframe = 0;
   update(); emit moved();
}

void QmnPlane::getUserPath(int M, mdouble *X, mdouble *Y)
{  mdouble u, v; Y[0] = (mdouble)(xpath[0]);
   if (ypath[0] < 3 || 16*M < xpath[0]) { X[0] = 0.0L; return; }
   int m, n; mdouble s = 1.0; if (M < xpath[0]) s = ((mdouble)(xpath[0]))/M;
   while (s*M > xpath[0]) M--; X[0] = (mdouble)(M);
   for (m = 1; m <= M; m++)
   {  n = (int)(s*m);
      if (posToPoint(xpath[n], ypath[n], u, v)) { X[0] = 0.0L; return; }
      X[m] = (mdouble)(u); Y[m] = (mdouble)(v);
   }
}

//returns 0 for on screen, 1 on double distance, 2 far and i, k = 0
int QmnPlane::pointToPos(mdouble x0, mdouble y0, int &i, int &k) const
{  i = 0; k = 0; //for safety when returning 2
   if (!type)
   {  mdouble pdet = pw*pw+ph*ph,
         I = floor(((x0 - hmid)*pw + (y0 - vmid)*ph)/pdet),
         K = floor(((x0 - hmid)*ph + (vmid - y0)*pw)/pdet);
      if (I < -2*imax || I >= 2*imax || K < -2*kmax || K >= 2*kmax) return 2;
      i = imax + int(I); k = kmax  + int(K);
      if (i < 0 || i >= 2*imax || k < 0 || k >= 2*kmax) return 1;
      else return 0;
   }
   if (type > 0)
   {  mdouble I = floor((x0 - hmid)*(2*imax - 1)/pw),
         K = floor((vmid - y0)*(2*kmax - 1)/ph);
      if (I < -imax || I >= 3*imax || K < -kmax || K >= 3*kmax) return 2;
      i = int(I); k = int(K);
      if (i < 0 || i >= 2*imax || k < 0 || k >= 2*kmax) return 1;
      else return 0;
   }
   mdouble w = 2.0/(1.0 + x0*x0 + y0*y0), u = x0*w, v = y0*w;
   w = 1.0 - w; x0 = u; y0 = v;
   u = rot[0]*x0 + rot[3]*y0 + rot[6]*w;
   v = rot[1]*x0 + rot[4]*y0 + rot[7]*w;
   w = rot[2]*x0 + rot[5]*y0 + rot[8]*w;
   if (w < 0) return 2;
   i = imax + int(floor(ph*u)); k = kmax - int(ceil(ph*v)); return 0;
}

//returns angles for rotation of sphere if clicked outside, otherwise 0
unsigned long int QmnPlane::posToPoint(int i, int k,
  mdouble &x0, mdouble &y0) const
{  if (!type)
   {  i -= imax; k -= kmax;
      x0 = hmid + pw*i + ph*k; y0 = vmid + ph*i - pw*k; return 0;
   }
   if (type > 0)
   {  x0 = hmid + pw*i/mdouble(2*imax-1); y0 = vmid - ph*k/mdouble(2*kmax-1);
      return 0;
   }
   mdouble u = pw*(i - imax), v = pw*(kmax - k), w = 1.0 - u*u - v*v;
   if (w < 0)
   {  unsigned long int angles = 15L; if (w < -0.1664) angles = 30L;
      if (w < -0.3456) angles = 45L; if (w < -0.5376) angles = 60L;
      if (u + v < 0) angles = 360L - angles; if (u*u < v*v) angles <<= 16;
      return angles;
   }
   w = sqrt(w); mdouble U = u, V = v;
   u = rot[0]*U + rot[1]*V + rot[2]*w;
   v = rot[3]*U + rot[4]*V + rot[5]*w;
   w = rot[6]*U + rot[7]*V + rot[8]*w; w = 1.0 - w;
   if (w < 1e-20) w = 1e-20; x0 = u/w; y0 = v/w; return 0;
}

void QmnPlane::display(int kLast)
{  if (kLast < 7) { update(); emit completed(); }
   else update(0, kLast - 7, 2*imax, 8);
}

void QmnPlane::paintEvent(QPaintEvent *event)
{  QPainter p(this);
   mutex->lock();
   p.drawImage(event->rect().topLeft(), *buffer, event->rect());
   mutex->unlock();
   p.setPen(Qt::gray);
   if (rubberBandShown) p.drawRect(rubberBand);
   if (ypath[0] > 1)
   {  int m, i1, k1, i2, k2; i1 = xpath[1]; k1 = ypath[1];
      for (m = 2; m <= xpath[0]; m++)
      {  i2 = xpath[m]; k2 = ypath[m];
         p.drawLine(i1, k1, i2, k2); i1 = i2; k1 = k2;
      }
   }
   int i, k; if (pointToPos(x, y, i, k) > 1) return;
   if (cursor != 0)
   { p.drawPixmap(QPoint(i - 8, k - 8), *cursor); if (!hframe) return; }
   p.setPen(Qt::yellow);
   if (!hframe)
   //{ p.drawLine(i - 4, k, i + 4, k); p.drawLine(i, k - 4, i,  k + 4); }
   {  p.drawLine(i - 9, k, i + 9, k); p.drawLine(i, k - 9, i,  k + 9);
      p.setPen(Qt::darkBlue);
      p.drawLine(i - 6, k, i - 5, k); p.drawLine(i, k - 6, i,  k - 5);
      p.drawLine(i + 5, k, i + 6, k); p.drawLine(i, k + 5, i,  k + 6);
   }//*/
   else p.drawRect(i - 4*hframe, k - 4*vframe, 8*hframe-1, 8*vframe-1);
}

void QmnPlane::mousePressEvent(QMouseEvent *event)
{  rubberBandShown = false; if (active) setCursor(Qt::CrossCursor);
   if (event->button() != Qt::LeftButton)
   { event->ignore(); update(); return; }
   event->accept();
   if (active)
   {  if (ypath[0]) setCursor(Qt::PointingHandCursor);
      else if (type < 0) setCursor(Qt::OpenHandCursor); //or shifting
      else setCursor(Qt::SizeAllCursor);
   }
   if (type >= 0 && !ypath[0] && active) rubberBandShown = true;
   rubberBand.setTopLeft(event->pos());
   rubberBand.setBottomRight(event->pos());
   if (ypath[0] && active)
   {  ypath[0] = 2; xpath[0] = 1;
      xpath[1] = event->x(); ypath[1] = event->y();
   }
   update();
}

void QmnPlane::mouseMoveEvent(QMouseEvent *event)
{  if (event->buttons() != Qt::LeftButton) { event->ignore(); return; }
   event->accept(); rubberBand.setBottomRight(event->pos());
   if (ypath[0] == 2)
   {  if (xpath[0] == Mmax)
      {  xpath[0] /= 2; int m;
         for (m = 1; m <= xpath[0]; m++)
         { xpath[m] = xpath[2*m-1]; ypath[m] = ypath[2*m-1]; }
      }
      else
      {  xpath[0] ++; xpath[xpath[0]] = event->x();
         ypath[xpath[0]] = event->y();
      }
   }
   update();
}

void QmnPlane::mouseReleaseEvent(QMouseEvent *event)
{  rubberBandShown = false; if (active) setCursor(Qt::CrossCursor);
   if (event->button() != Qt::LeftButton) { event->ignore(); return; }
   event->accept();
   if (event->x() < 0 || event->x() >= 2*imax
      || event->y() < 0 || event->y() >= 2*kmax)
   { if (ypath[0] == 2) ypath[0] = 1; update(); return; }
   if (!active) { update(); emit activated(); return; }
   if (ypath[0])
   {  if (ypath[0] != 2 || xpath[0] <= 2 ) ypath[0] = 1;
      else
      {  mdouble x0, y0;
         if (posToPoint(event->x(), event->y(), x0, y0)) ypath[0] = 1;
         else { ypath[0] = 3; setPoint(x0, y0); update(); return; }
      }
   }
   QRect rect = rubberBand.normalized();
   if (rect.width() <= 1 && rect.height() <= 1)
   {  int i = rect.left(), k = rect.top(); mdouble x0, y0;
      unsigned long int angles = posToPoint(i, k, x0, y0);
      /*if (!type && (x-x0)*(x-x0) + (y-y0)*(y-y0) < pw*pw + ph*ph)
      { emit drawRequest(); return; }
      if (!angles) setPoint(x0, y0);
      else
      {  move(11, int(angles & 0xFFFF), int(angles >> 16));
         emit drawRequest();
      }//*/
      if (!type || !angles) setPoint(x0, y0);
      return;
   }
   if (type < 0)
   {  if (rect.width() < 5 && rect.height() < 5) return;
      mdouble a1, a2, a3, p1, p2, p3, b1, b2, b3, c1, c2, c3, s, t;
      a1 = pw*(rubberBand.left() - imax);
      a2 = pw*(kmax - rubberBand.top()); a3 = 1.0 - a1*a1 - a2*a2;
      p1 = pw*(rubberBand.right() - imax);
      p2 = pw*(kmax - rubberBand.bottom()); p3 = 1.0 - p1*p1 - p2*p2;
      if (a3 < 0 || p3 < 0) return; a3 = sqrt(a3); p3 = sqrt(p3);
      c1 = a2*p3 - a3*p2; c2 = a3*p1 - a1*p3; c3 = a1*p2 - a2*p1;
      s = sqrt(c1*c1 + c2*c2 + c3*c3); if (s < 1.0e-10) return;
      c1 /= s; c2 /= s; c3 /= s;
      b1 = c2*a3 - c3*a2; b2 = c3*a1 - c1*a3; b3 = c1*a2 - c2*a1;
      s = rot[0]; t = rot[1]; rot[0] = s*a1 + t*a2 + rot[2]*a3;
      rot[1] = s*b1 + t*b2 + rot[2]*b3; rot[2] = s*c1 + t*c2 + rot[2]*c3;
      s = rot[3]; t = rot[4]; rot[3] = s*a1 + t*a2 + rot[5]*a3;
      rot[4] = s*b1 + t*b2 + rot[5]*b3; rot[5] = s*c1 + t*c2 + rot[5]*c3;
      s = rot[6]; t = rot[7]; rot[6] = s*a1 + t*a2 + rot[8]*a3;
      rot[7] = s*b1 + t*b2 + rot[8]*b3; rot[8] = s*c1 + t*c2 + rot[8]*c3;
      a1 = c2*p3 - c3*p2; a2 = c3*p1 - c1*p3; a3 = c1*p2 - c2*p1;
      s = rot[0]; t = rot[1]; rot[0] = s*p1 + t*a1 + rot[2]*c1;
      rot[1] = s*p2 + t*a2 + rot[2]*c2; rot[2] = s*p3 + t*a3 + rot[2]*c3;
      s = rot[3]; t = rot[4]; rot[3] = s*p1 + t*a1 + rot[5]*c1;
      rot[4] = s*p2 + t*a2 + rot[5]*c2; rot[5] = s*p3 + t*a3 + rot[5]*c3;
      s = rot[6]; t = rot[7]; rot[6] = s*p1 + t*a1 + rot[8]*c1;
      rot[7] = s*p2 + t*a2 + rot[8]*c2; rot[8] = s*p3 + t*a3 + rot[8]*c3;
      emit drawRequest(); return;
   }
   if (rect.width() < 7 || rect.height() < 7 || ypath[0])
   { update(); return; }
   hframe = rect.width()/2; vframe = rect.height()/2;
   int i = rect.left() + hframe, k = rect.top() + vframe;
   posToPoint(i, k, x, y);
   hframe += 2; hframe /= 4; if (!hframe) hframe = 1;
   vframe += 2; vframe /= 4; if (!vframe) vframe = 1;
   if (type == 2) { vframe = hframe; y = x; }
   if (!type && 3*imax*vframe < 4*kmax*hframe
      && 4*imax*vframe > 3*kmax*hframe) emit drawRequest();
   update(); emit moved(); //remove when frame independent of point
}

void QmnPlane::move(int mode, int u, int v) // = 0, 0
{  //mode: 0 move, 1 frame, 2 home, 3 kill frame, 4 kill path, 5 all,
   //6 mark point, 7 mark path, 8 both, 9 enable path, 10 disable, 11 rotate
   if (mode == 11 && type < 0)
   {  mdouble t, c, s;
      if (u)
      {  t = u*PI/mdouble(180); c = cos(t); s = sin(t);
         t = rot[0]; rot[0] = c*t + s*rot[2]; rot[2] = c*rot[2] - s*t;
         t = rot[3]; rot[3] = c*t + s*rot[5]; rot[5] = c*rot[5] - s*t;
         t = rot[6]; rot[6] = c*t + s*rot[8]; rot[8] = c*rot[8] - s*t;
      }
      if (v)
      {  t = v*PI/mdouble(180); c = cos(t); s = sin(t);
         t = rot[2]; rot[2] = c*t - s*rot[1]; rot[1] = c*rot[1] + s*t;
         t = rot[5]; rot[5] = c*t - s*rot[4]; rot[4] = c*rot[4] + s*t;
         t = rot[8]; rot[8] = c*t - s*rot[7]; rot[7] = c*rot[7] + s*t;
      }
      emit drawRequest(); return;
   }
   if (type < 0 && mode < 4) return;
   int wasmoved = 0, i, k;
   if (!mode)
   {  if (!type) { x += (pw*u - ph*v); y += (ph*u + pw*v); }
      else { x += pw*u/mdouble(2*imax-1); y += ph*v/mdouble(2*kmax-1); }
      wasmoved = 1; if (type == 2) y = x;
   }
   if (mode == 1)
   {  /*if (pointToPos(x, y, i, k) > 1 || isRunning()) return;
      floodFill(i, k, buffer->pixel(i, k), 0xFFFF0000L);//*/
      if (!hframe && v > 0) vframe = 0;
      hframe += u; vframe += v;
      if (hframe < 0) hframe = 0;
      if (hframe > 0 && vframe <= 0) vframe = 1;
      if (hframe > imax/4) hframe = imax/4;
      if (vframe > kmax/4) vframe = kmax/4;
      if (type == 2) vframe = hframe;
      if (ypath[0] > 1 && hframe) ypath[0] = 1;//*/
   }
   if (mode <= 1 && hframe)
   {  u = 0; if (pointToPos(x, y, i, k)) { i = imax; k = kmax; u = 1; }
      if (i < 4*hframe) { i = 4*hframe; u = 1; }
      if (i > 2*imax - 4*hframe) { i = 2*imax - 4*hframe; u = 1; }
      if (k < 4*vframe) { k = 4*vframe; u = 1; }
      if (k > 2*kmax - 4*vframe) { k = 2*kmax - 4*vframe; u = 1; }
      if (u) { posToPoint(i, k, x, y); wasmoved = 1; }
   }
   if ((mode == 2 || mode == 5) && !hframe && type >= 0
      && pointToPos(x, y, i, k) )
   { wasmoved = 1; x = hmid; y = vmid; if (type == 2) y = x; }
   if ((mode == 3 || mode == 5) && hframe) hframe = 0;
   if ((mode == 4 || mode == 5) && ypath[0] > 1) ypath[0] = 1;
   if ((mode == 6 || mode == 8) && ypath[0] < 2)
   {  if (pointToPos(x, y, i, k) > 1) return;
      QPainter *p = new QPainter(buffer); p->setPen(Qt::white);
      p->drawLine(i - 4, k, i + 4, k); p->drawLine(i, k - 4, i,  k + 4);
      delete p; hframe = 0;
   }
   if ((mode == 7 || mode == 8 || mode > 11) && ypath[0] > 2)
   {  QPainter *p = new QPainter(buffer); p->setPen(Qt::white);
      int m, i1, k1, i2, k2; i1 = xpath[1]; k1 = ypath[1];
      //for (m = xpath[0]; m <= xpath[0]; m++) //straight
      for (m = 2; m <= xpath[0]; m++)
      {  i2 = xpath[m]; k2 = ypath[m];
         p->drawLine(i1, k1, i2, k2); i1 = i2; k1 = k2;
      }
      delete p; if (mode < 12) ypath[0] = 1;
   }
   if (mode == 9) { ypath[0] = 1; hframe = 0; rubberBandShown = false; }
   if (mode == 10) ypath[0] = 0;
   //if (mode > 11 && ypath[0] > 2) {  } //see Move
   update(); if (wasmoved) emit moved();
}

/*void QmnPlane::Move(int mode, mndynamics *f) //to move when plane contains f
{  if (mode < 12 || ypath[0] <= 2) return; move(mode);
   int i, k, m = xpath[0]; mdouble xx, yy, XX, YY;
   posToPoint(xpath[m], ypath[m], xx, yy);
   f->iterate(xx, yy, 11 - mode); XX = xx; YY = yy;
   if (pointToPos(xx, yy, i, k) > 1) { ypath[0] = 1; return; }
   xpath[m] = i; ypath[m] = k;
   for (m = xpath[0] - 1; m > 0; m--)
   {  posToPoint(xpath[m], ypath[m], xx, yy); f->iterate(xx, yy, 11 - mode);
      if (xx*XX + yy*YY < 0.0) { xx = -xx; yy = -yy; mode = 25 - mode; }
      XX = xx; YY = yy;
      if (pointToPos(xx, yy, i, k) > 1) { ypath[0] = 1; return; }
      xpath[m] = i; ypath[m] = k;
   }
   update();
}//*/

void QmnPlane::Move(int mode, mndynamics *f) //to move when plane contains f
{  if (mode < 12 || ypath[0] <= 2) { x = 0.0; y = 0.0; return; } move(mode);
   int i, k, m; mdouble xx, yy, XX, YY;
   posToPoint(xpath[1], ypath[1], xx, yy);
   f->iterate(xx, yy, 11 - mode); XX = xx; YY = yy;
   if (pointToPos(xx, yy, i, k) > 1) { ypath[0] = 1; return; }
   xpath[1] = i; ypath[1] = k;
   for (m = 2; m <= xpath[0]; m++)
   {  posToPoint(xpath[m], ypath[m], xx, yy); f->iterate(xx, yy, 11 - mode);
      if (xx*XX + yy*YY < 0.0) { xx = -xx; yy = -yy; mode = 25 - mode; }
      XX = xx; YY = yy;
      if (pointToPos(xx, yy, i, k) > 1) { ypath[0] = 1; return; }
      xpath[m] = i; ypath[m] = k;
   }
   x = xx; y = yy; update();
}

//rewrite with posToPoint!
void QmnPlane::draw(mndynamics *f, int signtype, uint *drawmode, int mode,
   int alpha)// = 0, 0
{  stop(); mdouble t[3]; if (ypath[0] > 1) ypath[0] = 1;
   if (mode == 2)
     disconnect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
   else connect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
   if (!signtype)
   { buffer->fill(QColor(Qt::black).rgba()); update(); return; }
   if (type < 0)
   {  thread->setRectPlane(imax, kmax, imax, kmax, hmid, vmid, pw, ph, rot);
      *t = 0.25*pw*pw; f->prepare(signtype, nmax, *drawmode, t);
      buffer->fill(QColor(Qt::darkGray).rgba());
      QPainter *p = new QPainter(buffer); p->setBrush(Qt::gray);
      p->drawEllipse(imax - (kmax*4)/5 + 1, kmax/5 + 1,
        (kmax*8)/5 - 1, (kmax*8)/5 - 1);
      delete p; update(); thread->draw(f, *drawmode, -1, buffer, mutex);
   }
   if (type) return;
   if (mode < 0) //back
   {  if (!oldnmax || *drawmode > 255) return;
      nmax = oldnmax; mode = 0; hframe = 0;
      oldnmax = 0; emit backMaybeChanged();
      setPlane(oldhmid, oldvmid, oldrewidth, oldimwidth);
      if (!oldbuffer->isNull())
      {  delete buffer; buffer = oldbuffer; oldbuffer = new QImage();
         update(); *drawmode = oldmode; return;
      }
   }
   else if (*drawmode <= 255 && hframe > 0) //zoom in
   {  oldnmax = nmax; oldmode = *drawmode; emit backMaybeChanged();
      getPlane(oldhmid, oldvmid, oldrewidth, oldimwidth);
      delete oldbuffer; oldbuffer = buffer;
      buffer = new QImage(2*imax, 2*kmax, QImage::Format_RGB32);
      if (buffer->isNull())
      { delete buffer; buffer = oldbuffer; oldbuffer = new QImage(); }
      mdouble temp = 4*sqrt(hframe*vframe/mdouble(imax)/mdouble(kmax));
/*/   to rescale the previous image before drawing:
      int i1 = imax + int(((x-hmid)*pw + (y-vmid)*ph)/pdet - temp*imax),
          i2 = imax + int(((x-hmid)*pw + (y-vmid)*ph)/pdet + temp*imax),
          k1 = kmax + int(((x-hmid)*ph + (vmid-y)*pw)/pdet - temp*kmax),
          k2 = kmax + int(((x-hmid)*ph + (vmid-y)*pw)/pdet + temp*kmax);
      QImage *t = new QImage(i2 - i1 + 1, k2 - k1 + 1, QImage::Format_RGB32);
      QPainter *p = new QPainter(t);
      p->drawImage(0, 0, *oldbuffer, i1, k1, i2-i1 + 1, k2-k1 + 1);
      *buffer = t->scaled(2*imax, 2*kmax); mode = 1; update();
      delete p; delete t;//*/
      pw *= temp; ph *= temp; hmid = x; vmid = y;
   }
   if (mode > 10) //zoom in smoothly
   {  if (*drawmode > 255 || hframe > 0) return;
      mdouble temp = mdouble(mode)/256; mode = 0;
      oldnmax = 0; emit backMaybeChanged();
      int i1, i2, k1, k2; pointToPos(x, y, i1, k1);
      i1 = int(i1*(1.0 - temp)); i2 = i1 + int(2*imax*temp);
      k1 = int(k1*(1.0 - temp)); k2 = k1 + int(2*kmax*temp);
      if (i1 >= 0 && i2 < 2*imax && k1 >= 0 && k2 < 2*kmax)
      {  QImage *tmp = new QImage(i2-i1+1, k2-k1+1, QImage::Format_RGB32);
         QPainter *p = new QPainter(tmp);
         p->drawImage(0, 0, *buffer, i1, k1, i2 - i1 + 1, k2 - k1 + 1);
         *buffer = tmp->scaled(2*imax, 2*kmax); mode = 1; update();
         delete p; delete tmp;
      }
      pw *= temp; ph *= temp;
      hmid = x + temp*(hmid - x); vmid = y + temp*(vmid - y);
   }
   if (*drawmode > 255 && hframe > 0)
   {  int i, k; pointToPos(x, y, i, k);
      thread->setRectPlane(i, k, 4*hframe, 4*vframe, x, y, pw, ph);
   }
   else thread->setRectPlane(imax, kmax, imax, kmax, hmid, vmid, pw, ph);
   if (*drawmode > 255)
   {  *t = 0.25;
      if (signtype > 0) { t[1] = x; t[2] = y; }
      else f->getParameter(t[1], t[2]);
   }
   else *t = pw*pw + ph*ph; //*t *= x*x;
   hframe = 0; f->prepare(signtype, nmax, *drawmode, t);
   if (!mode) { buffer->fill(QColor(Qt::darkGray).rgba()); update(); }
   thread->draw(f, *drawmode, mode, buffer, mutex, alpha);
}//draw

void QmnPlane::recur(mndynamics *f, mdouble x0, mdouble y0, uint n)
{  int i, k; mdouble x1 = x0, y1 = y0; //caller must stop()
   if (!pointToPos(x0, y0, i, k)) buffer->setPixel(i, k, 0);
   if (n >= nmax) return; n++;
   f->iterate(x0, y0, -1); recur(f, x0, y0, n);
   f->iterate(x1, y1, -2); recur(f, x1, y1, n);
}

void QmnPlane::drawLambda(int step)// = 8
{  stop(); mdouble x0, y0, ymin, ymax, p = pw/step, scale;
   int i, j, mink, maxk, oldmin = 0, oldmax = 2*kmax - 1;
   scale = ceil(log(0.1*pw)/log(0.5));
   if (scale < 20.0) scale = 20.0; if (scale > 50.0) scale = 50.0;
   qulonglong D = 1ULL << ((int)(scale)); scale = 0.01*nmax;
   for (i = 0; i < 2*imax; i++)
   {  ymin = vmid + pw*kmax; ymax = vmid - pw*kmax; x0 = hmid + pw*(i - imax);
      for (j = 0; j < step; j++)
      {  x0 += p; y0 = x0; if (x0 < 0.0) y0 = 0.0;
         y0 *= D; if (y0 > D) y0 = 0.0;
         y0 = scale*mndAngle::lambda(((qulonglong)(y0)), D, pw);
         if (y0 < ymin) ymin = y0; if (y0 > ymax) ymax = y0;
      }
      y0 = kmax + floor((vmid - ymax)/pw);
      ymax = kmax + ceil((vmid - ymin)/pw); ymin = y0;
      if (ymax >= 2*kmax - 1.0)
      { ymax = 2*kmax - 1.0; if (ymin > ymax) ymin = ymax; }
      if (ymin < 0.0) { ymin = 0.0; if (ymax < 0.0) ymax = 0.0; }
      mink = (int)(ymin); maxk = (int)(ymax);
      for (j = mink; j <= maxk; j++) buffer->setPixel(i, j, 0x000000);
      for (j = oldmax + 1; j < (oldmax + 1 + mink)/2; j++)
         buffer->setPixel(i - 1, j, 0x0000ff);
      for (j = (oldmax + 1 + mink)/2; j < mink; j++)
         buffer->setPixel(i, j, 0x0000ff);
      for (j = maxk + 1; j < (maxk + 1 + oldmin)/2; j++)
         buffer->setPixel(i, j, 0x0000ff);
      for (j = (maxk + 1 + oldmin)/2; j < oldmin; j++)
         buffer->setPixel(i - 1, j, 0x0000ff);
      oldmin = mink; oldmax = maxk;
   }
   update();
}

void QmnPlane::drawOrbit(mndynamics *f, mdouble &x0, mdouble &y0,
  int preiter, int plotiter)
{  if (type > 0) return;
   stop(); int i, j, k;
   QPainter *p = new QPainter(buffer); p->setPen(Qt::white);
   for (j = 0; j < preiter; j++)
   { f->iterate(x0, y0); if (x0*x0 + y0*y0 > 1e6) break; }
   for (j = 0; j < plotiter; j++)
   {  f->iterate(x0, y0); if (x0*x0 + y0*y0 > 1e6) break;
      if (!pointToPos(x0, y0, i, k)) p->drawPoint(i, k);
   }
   delete p; update();
}

void QmnPlane::drawLine(mdouble x1, mdouble y1, mdouble x2, mdouble y2,
  QColor color) // = white
{  stop(); int i1, k1, i2, k2;
   /*if (!type)
   {  mdouble pdet = pw*pw + ph*ph,
      u = imax + ((x1-hmid)*pw + (y1-vmid)*ph)/pdet;
      y1 = kmax + ((x1-hmid)*ph + (vmid-y1)*pw)/pdet; x1 = u;
      u = imax + ((x2-hmid)*pw + (y2-vmid)*ph)/pdet;
      y2 = kmax + ((x2-hmid)*ph + (vmid-y2)*pw)/pdet; x2 = u;
      if (x1*x1 + y1*y1 + x2*x2 + y2*y2 > 1E10) return;
      QLineF line(x1, y1, x2, y2);
      QPainter *p = new QPainter(buffer); p->setPen(color);
      p->drawLine(line); delete p; update(); return;
   }//*/
   if(pointToPos(x1, y1, i1, k1) > 1 || pointToPos(x2, y2, i2, k2) > 1)return;
   QPainter *p = new QPainter(buffer); p->setPen(color);
   p->drawLine(i1, k1, i2, k2); delete p; update();
}

//rewrite with posToPoint?
void QmnPlane::drawEllipse(mdouble x0, mdouble y0, mdouble rx, mdouble ry,
  QColor color, bool filled) // = white, false
{  stop(); mdouble u, pdet = pw*pw + ph*ph; if (type) return;
   u = imax + ((x0-hmid)*pw + (y0-vmid)*ph)/pdet;
   y0 = kmax + ((x0-hmid)*ph + (vmid-y0)*pw)/pdet; x0 = u;
   rx /= sqrt(pdet); ry /= sqrt(pdet);
   if (x0*x0 + y0*y0 + rx*rx + ry*ry > 1E10) return;
   QPainter *p = new QPainter(buffer);
   if (filled) p->setBrush(QBrush(color)); else p->setPen(color);
#if QT_VERSION >= 0x040400
   QPointF point(x0, y0); p->drawEllipse(point, rx, ry);
#else
   QRectF rect(x0 - rx, y0 - ry, 2*rx, 2*ry); p->drawEllipse(rect);
#endif
   delete p; update();
}

void QmnPlane::drawOrtho(mdouble alpha, mdouble beta, QColor color) // = white
{  mdouble u, r, x0, y0; stop(); if (type || ph != 0.0) return;
   if (alpha > beta) { u = alpha; alpha = beta; beta = u; }
   beta -= alpha; if (beta > 0.5) { alpha += beta; beta = 1.0 - beta; }
   QPainter *p = new QPainter(buffer); p->setPen(color);
   if (beta > 0.45) //span angle inaccurate, center far out
   {  x0 = imax + (cos(2.0*PI*alpha)-hmid)/pw;
      y0 = kmax + (vmid-sin(2.0*PI*alpha))/pw;
      QPainterPath path; path.moveTo(x0, y0); beta += alpha;
      x0 = imax + (cos(2.0*PI*beta)-hmid)/pw;
      y0 = kmax + (vmid-sin(2.0*PI*beta))/pw;
      path.quadTo(imax - hmid/pw, kmax + vmid/pw, x0, y0);
      p->drawPath(path);
   }
   else
   {  r = tan(PI*beta); x0 = cos(2.0*PI*alpha); y0 = sin(2.0*PI*alpha);
      u = x0 - r*y0; y0 += r*x0; x0 = u; r /= pw;
      u = imax + (x0-hmid)/pw; y0 = kmax + (vmid-y0)/pw; x0 = u;
      QRectF rect(x0 - r, y0 - r, 2*r, 2*r);
      alpha += (beta + 0.25); if (alpha >= 1.0) alpha -= 1.0; //start, span
      beta = 0.5 - beta; p->drawArc(rect, int(5760*alpha), int(5760*beta));
   }
   delete p; update(); //full circle: 1.0 turns, 2*PI, here 5760 = 360*16
}

void QmnPlane::drawLamination(mdouble alpha, mdouble beta, uint n)
{  drawOrtho(alpha, beta);
   n--; if (!n) return;
   alpha *= 0.5; if (alpha < x) alpha += 0.5;
   beta *= 0.5; if (beta < x) beta += 0.5;
   drawLamination(alpha, beta, n);
   if (alpha < 0.5) alpha += 0.5; else alpha -= 0.5;
   if (beta < 0.5) beta += 0.5; else beta -= 0.5;
   drawLamination(alpha, beta, n);  
}

//void QmnPlane::drawPathSegment(mndPath *path, QColor color) // = white
//{  int j, m, s, M = path->M, S = path->S; if (!M) return; stop();
   /*if (S <= 0 && path->p == 13 && !path->k && M == 100)
   {  buffer->fill(QColor(Qt::black).rgba());
      drawLine(0.0, 0.0, 0.0, 0.0, color);
      mdouble u[13], v[13]; //z[j][100]
      u[0] = path->y[11][0]*path->y[11][0] - path->x[11][0]*path->x[11][0];
      v[0] = -2.0L*path->x[11][0]*path->y[11][0];
      for (j = 1; j<= 12; j++)
      {  u[j] = path->x[j-1][0]*path->x[j-1][0]
         - path->y[j-1][0]*path->y[j-1][0] + u[0];
         v[j] = 2.0L*path->x[j-1][0]*path->y[j-1][0] + v[0];
      }
      j = (25 + S) % 13; s = (24 + S) % 13;
      for (m = 1; m < 100; m++)
         drawLine(((mdouble)(path->x[j][m])) - ((mdouble)(path->x[s][m])),
                  ((mdouble)(path->y[j][m])) - ((mdouble)(path->y[s][m])),
                  ((mdouble)(path->x[j][m-1])) - ((mdouble)(path->x[s][m-1])),
                  ((mdouble)(path->y[j][m-1])) - ((mdouble)(path->y[s][m-1])) );
      drawLine(((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(path->x[j][99])) - ((mdouble)(path->x[s][99])),
               ((mdouble)(path->y[j][99])) - ((mdouble)(path->y[s][99])) );
      for (m = 25; m <= 75; m += 25)
         drawLine(((mdouble)(path->x[j][m])) - ((mdouble)(path->x[s][m])),
                  ((mdouble)(path->y[j][m])) - ((mdouble)(path->y[s][m])),
                  ((mdouble)(path->x[j][m-25])) - ((mdouble)(path->x[s][m-25])),
                  ((mdouble)(path->y[j][m-25])) - ((mdouble)(path->y[s][m-25])),
                  Qt::green);
      drawLine(((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(path->x[j][75])) - ((mdouble)(path->x[s][75])),
               ((mdouble)(path->y[j][75])) - ((mdouble)(path->y[s][75])),
               Qt::green);
      drawLine(((mdouble)(path->x[j][50])) - ((mdouble)(path->x[s][50])),
                  ((mdouble)(path->y[j][50])) - ((mdouble)(path->y[s][50])),
                  ((mdouble)(path->x[j][0])) - ((mdouble)(path->x[s][0])),
                  ((mdouble)(path->y[j][0])) - ((mdouble)(path->y[s][0])),
                  Qt::red);
      drawLine(((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(u[j])) - ((mdouble)(u[s])),
               ((mdouble)(path->x[j][50])) - ((mdouble)(path->x[s][50])),
               ((mdouble)(path->y[j][50])) - ((mdouble)(path->y[s][50])),
               Qt::red);
      update(); return;
   }//*/
   /*if (S <= 0 && nmax)
   {  buffer->fill(QColor(Qt::black).rgba());
      QColor c[] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan,
      Qt::darkRed, Qt::darkMagenta, Qt::darkYellow, Qt::lightGray,
      Qt::darkGray, Qt::blue, Qt::green, Qt::cyan, Qt::red, Qt::magenta,
      Qt::yellow, Qt::white};
      for (j = path->k + path->p - 1; j >= 0; j--)
      {  if (!path->k && j == path->p - 1)
         { drawLine(0.0, 0.0, 0.0, 0.0, color); continue; }
         for (m = M - 1; m > 0; m--)
            drawLine(((mdouble)(path->x[j][m])), ((mdouble)(path->y[j][m])),
            ((mdouble)(path->x[j][m-1])), ((mdouble)(path->y[j][m-1])),
              c[(j+1)&15]);
      }
      update(); return;
   }//*/
   /*if (S <= 0 && (nmax & 1) && !path->k)
   {  drawLine(0.0, 0.0, 0.0, 0.0, color);
      drawLine(1.0, 0.0, 1.0, 0.0, color);
      mdouble x1, y1, x2, y2, u, v, w;
      for (j = path->p - 2; j > 0; j--) for (m = M - 1; m > 0; m--)
      {  u = (mdouble)(path->x[0][m]); v = (mdouble)(path->y[0][m]);
         w = u*u + v*v; u /= w; v /= w;
         x1 = (mdouble)(path->x[j][m]); y1 = (mdouble)(path->y[j][m-1]);
         w = x1*u + y1*v; y1 = y1*u - x1*v; x1 = w;
         u = (mdouble)(path->x[0][m-1]); v = (mdouble)(path->y[0][m-1]);
         w = u*u + v*v; u /= w; v /= w;
         x2 = (mdouble)(path->x[j][m]); y2 = (mdouble)(path->y[j][m-1]);
         w = x2*u + y2*v; y2 = y2*u - x2*v; x2 = w;
         drawLine(x1, y1, x2, y2, color);
      }
      update(); return;
   }//*/
/*   if (S <= 0)
   {  for (j = path->k + path->p - 1; j >= 0; j--)
      {  if (!path->k && j == path->p - 1)
         { drawLine(0.0, 0.0, 0.0, 0.0, color); continue; }
         for (m = M - 1; m > 0; m--)
            drawLine(((mdouble)(path->x[j][m])), ((mdouble)(path->y[j][m])),
            ((mdouble)(path->x[j][m-1])), ((mdouble)(path->y[j][m-1])), color);
      }
      update(); return;
   }
   buffer->fill(QColor(Qt::black).rgba()); //improve on sphere
   for (j = path->k + path->p - 1; j >= 0; j--)
   {  for (m = M - 2; m >= 0; m--)
      {  drawLine(((mdouble)(path->x[j*S][m+1])),
            ((mdouble)(path->y[j*S][m+1])), ((mdouble)(path->x[j*S+S-1][m])),
            ((mdouble)(path->y[j*S+S-1][m])), color);
         for (s = S - 1; s > 0; s--)
            drawLine(((mdouble)(path->x[j*S+s][m])),
               ((mdouble)(path->y[j*S+s][m])), ((mdouble)(path->x[j*S+s-1][m])),
               ((mdouble)(path->y[j*S+s-1][m])), color);
      }
   }
   update();
}//*/

//rewrite without recursion to avoid stack overflow
void QmnPlane::floodFill(int i, int k, QRgb rgbold, QRgb rgbnew)
{  //if (isRunning()) return;  //en.wikipedia.org/wiki/Flood_fill
   if (buffer->pixel(i, k) != rgbold) return;
   buffer->setPixel(i, k, rgbnew);
   floodFill(i + 1, k, rgbold, rgbnew);
   floodFill(i - 1, k, rgbold, rgbnew);
   floodFill(i, k + 1, rgbold, rgbnew);
   floodFill(i, k - 1, rgbold, rgbnew);
   update();
}

int mabs(int i) { return (i >= 0 ? i : -i); }
//the following code will be rewritten completely!!!
void QmnPlane::green(mndynamics *f, int st, mdouble g, int quality,
  QColor color) //5, white
{ //trace the boundary ...
  if (g <= 0 || type) return;
  uint m = 1; f->prepare(st, nmax, m, &g); //just to set nmax
  int i, k, i0, k0, i1, k1, i2, k2, j, vert, side, sg, start, T; stop();
  QPainter *p = new QPainter(buffer); p->setPen(color);
for (T = 20 - kmax; T < kmax - 20; T += imax*2/quality)
{ for (vert = 0; vert <= 1; vert++)
  { for (side = -1; side <= 1; side += 2)
    { start = 10000; //no startpoint
      if (vert) //vertical lines from above and below
      { if (f->green(st, hmid - ph*side*kmax + pw*T, vmid - pw*side*kmax + ph*T) <= g)
           continue;
        for (j = kmax-1; j >= -kmax/2; j--)
        { if (f->green(st, hmid + ph*side*j + pw*T, vmid - pw*side*j + ph*T) <= g)
          {start = side*j; break;} }
      }
      else//vert: horizontal lines from left and right
      { if (f->green(st, hmid + pw*side*imax - ph*T, vmid + ph*side*imax - pw*T) <= g)
          continue;
        for (j = imax-1; j >= -imax/2; j--)
        { if (f->green(st, hmid + pw*side*j - ph*T, vmid + ph*side*j - pw*T) <= g)
          {start = side*j; break;} }
      }//vert
      if (start == 10000) continue; //no startpoint
      for (sg = -1; sg <= 1; sg += 2) //go in both directions
      { if (vert)
        { k0 = start; k1 = k0 + side; k2 = k1; i0 = T; i1 = T; i2 = i1 + sg;}
        else
        { i0 = start; i1 = i0 + side; i2 = i1; k0 = T; k1 = T; k2 = k1 + sg;}
        for (j = 1; j < 8*imax; j++)
        { if (i0 < -imax || i0 >= imax || k0 < -kmax || k0 >= kmax) break;
          else p->drawLine(imax + i0, kmax + k0, imax + i0, kmax + k0);
          if (f->green(st, hmid + pw*i2 + ph*k2, vmid + ph*i2 - pw*k2) <= g)
          {i = i0; k = k0; i0 = i2; k0 = k2; i2 += (i1 - i); k2 += (k1 - k);}
          else
          {i = i1; k = k1; i1 = i2; k1 = k2; i2 += (i0 - i); k2 += (k0 - k);}
        }//for j
      }//for sg
    }//for side
  }//for vert
}//for T
  delete p; update();
} //green

//rewrite with posToPoint?
int QmnPlane::traceRay(int signtype, mdouble t, mndynamics *f,
  mdouble &x0, mdouble &y0, int quality, QColor color) //5, white
{ //Draw the external ray for the turn t with the argument tracing method.
  //Return 0 if the endpoint was found, 1 no startpoint, 2 or 3 no endpoint.
  //f->turnsign() checks if the turn is in a small intervall around t,
  //returns +-1 for the side and 0 otherwise, may adjust jumps.
  if (type) return 4;
  int i, i0, i1, i2, k, k0, k1, k2, j, sign, draw = 0, iold, kold, u, v;
  //Set signtype and t in mndynamics,  disable adjusting of jumps:
  uint m = 201; f->prepare(signtype, 0, m, &t);
  //First,  search the startpoint on the boundary enlarged by  quality >= 1:
  i = quality*imax; k = -quality*kmax - 1; i2 = -2; k2 = 0; //go left on top
  j = f->turnsign(hmid + pw*i + ph*k, vmid + ph*i - pw*k);
  while (1)
  {  i += i2; k += k2;
     sign = f->turnsign(hmid + pw*i + ph*k, vmid + ph*i - pw*k);
     if (j < 0 && sign > 0)
     {  iold = i2/2; kold = k2/2;
        i1 = i; k1 = k;
        i0 = i - iold; k0 = k - kold;
        if (f->turnsign(hmid + pw*i0 + ph*k0, vmid + ph*i0 - pw*k0) > 0)
        { i1 = i0; k1 = k0; i0 -= iold; k0 -= kold; }
        i2 = i0 + kold; k2 = k0 - iold; u = i1; v = k1;
        break; //found startpoint
     }
     j = sign;
     if (i < -quality*imax && i2 < 0) { i2 = 0; k2 = 2; } //down on left line
     if (k >= quality*kmax && k2 > 0) { i2 = 2; k2 = 0; } //right on bottom
     if (i >= quality*imax && i2 > 0) { i2 = 0; k2 = -2; } //up on right line
     if (k < -quality*kmax && k2 < 0) return 1;
  }
  //Second, trace the ray by triangles with sign(z0) < 0, sign(z1) > 0:
  stop(); QPainter *p = new QPainter(buffer); p->setPen(color);
  m = 202; f->prepare(signtype, 0, m, &t); //adjusting jumps enabled
  for (j = 1; j < quality*3*(imax + kmax); j++)
  {  sign = f->turnsign(hmid + pw*i2 + ph*k2, vmid + ph*i2 - pw*k2);
     if (sign > 0) { i = i1; k = k1; i1 = i2; k1 = k2; }
     else { i = i0; k = k0; i0 = i2; k0 = k2; }
     //New reflected point z2:
     i2 = i0 + i1 - i; k2 = k0 + k1 - k;
     //If not reflected at diagonal edge, take maximal distance to (u, v) :
     if (i0 == i1) { if (mabs(k1-v) > mabs(k0-v)) k2 = k1; else k2 = k0; }
     if (k0 == k1) { if (mabs(i1-u) > mabs(i0-u)) i2 = i1; else i2 = i0; }
     u = i; v = k;
     //Check viewport and draw at z1 (t = 0 | 1/2 too low with z0; z2 rough):
     if (-imax <= i1 && i1 < imax && -kmax <= k1 && k1 < kmax)
     {  if (draw)
        {  // >= 8 is less smooth but not going in circles at 399/992:
           if ((i1-iold)*(i1-iold)+(k1-kold)*(k1-kold) >= 5)
           {  p->drawLine(imax+iold, kmax+kold, imax+i1, kmax+k1);
              iold = i1; kold = k1;
           }
           if (!sign)
           {  x0 = hmid + pw*i1 + ph*k1; y0 = vmid + ph*i1 - pw*k1;
              delete p; update(); return 0;
           }
        }
        else
        { draw = 1; iold = i1; kold = k1; }
     }
     else if (draw) //has left the viewport, no endpoint
     { delete p; update(); return 2; }
  }
  delete p; update(); return 3;
} //traceRay

//rewrite mdouble array allocation as in mndPath
int QmnPlane::backRay(qulonglong num, qulonglong denom, mdouble &a, mdouble &b,
  const int quality, QColor color, int mode) // = 5, white, 1
{  //Draw a dynamic ray with angle  num/denom  by backwards iteration with
   //quality points per step.  At present only for quadratic polynomials.
   //mode : 0 all rays, 1 one ray, 2 return endpoint in a, b.
   if (type > 0) return 2; //works on sphere and on plane
   if (quality <= 1) return 3;
   int i, j, k, preper = 0, pppp = 0; //pppp = preper + per
   mndAngle t; pppp = t.setAngle(num, denom, preper); if (!pppp) return 4;
   //pppp += preper; mdouble c, s, X[quality][], Y[quality][];
   //X = new mdouble[quality][pppp + 1]; Y = new mdouble[quality][pppp + 1];
   pppp += preper; mdouble c, s,
   X[quality][pppp + 1], Y[quality][pppp + 1];

   //The second index corresponds to the i-th iterate of the angle. Initial
   //radii between 2^12 and 2^24 : 2^(24*2^(-k/quality))
   s = log(2); c = 24*s; s = exp(-s/mdouble(quality));
   for (k = 0; k < quality; k++)
   { c *= s; X[k][pppp] = exp(c); Y[k][pppp] = 0.5/X[k][pppp]; }
   //Using the approximation Phi_c^{-1}(w) ~ w - 0.5*c/w :
   for (i = 0; i < pppp;i++)
   {  s = t.radians(); c = cos(s); s = sin(s);
      for (k = 0; k < quality; k++)
      {  X[k][i] = c*X[k][pppp] - (b*s + a*c)*Y[k][pppp];
         Y[k][i] = s*X[k][pppp] + (a*s - b*c)*Y[k][pppp];
      }
      t.twice();
   }
   for (k = 0; k < quality; k++)
   { X[k][pppp] = X[k][preper]; Y[k][pppp] = Y[k][preper]; }
   stop(); QPainter *p = new QPainter(buffer); p->setPen(color);
   //2 more for bailout |z| = 4 :
   for (j = nmax + 2; j; j--) for (k = 0; k < quality; k++)
   {  for (i = 0; i < pppp; i++)
      {  c = X[(k ? k - 1 : quality - 1)][i];
         s = Y[(k ? k - 1 : quality - 1)][i];
         mndynamics::root(X[k][i + 1] - a, Y[k][i + 1] - b, X[k][i], Y[k][i]);
         if (c*X[k][i] + s*Y[k][i] < 0) //choose closest preimage
         { X[k][i] = -X[k][i]; Y[k][i] = -Y[k][i]; }
         //if (k & 1) p->setPen(Qt::red); else p->setPen(Qt::blue);
         int i1, k1, i2, k2;
         if ( (!i || !mode) && pointToPos(c, s, i1, k1) <= 1
            && pointToPos(X[k][i], Y[k][i], i2, k2) <= 1)
            p->drawLine(i1, k1, i2, k2);
      }
      X[k][pppp] = X[k][preper]; Y[k][pppp] = Y[k][preper];
   }
   if (mode == 2) { a = X[quality - 1][0]; b = Y[quality - 1][0]; }
   delete p; /*delete[] Y; delete[] X;*/ update(); return 0;
} //backRay

void QmnPlane::mandelRay(int sg, mndynamics *f, qulonglong N, qulonglong D,
   mdouble &a, mdouble &b, QColor color)
{  int i0, k0, i, k, points, n = (int)(nmax);
   if (n > 5000) n = 5000; n += 10; //limited time ~ nmax^2
   //now up to n iterations with 6 segments each
   mdouble *xlist = new mdouble[6*n + 1], *ylist = new mdouble[6*n + 1];
   points = f->mandelRay(sg, n, N, D, xlist, ylist);
   a = xlist[0]; b = ylist[0];
   if (points < 0)
   { a = 0.0L; b = 0.0L; delete[] ylist; delete[] xlist; return; }
   stop(); QPainter *p = new QPainter(buffer); p->setPen(color);
   for (n = 1; n < points; n++)
   {  if(pointToPos(xlist[n], ylist[n], i0, k0) > 1
         || pointToPos(xlist[n + 1], ylist[n + 1], i, k) > 1) continue;
      p->drawLine(i0, k0, i, k);
   }
   delete p; update(); delete[] ylist; delete[] xlist;
}
  
int QmnPlane::newtonRay(int signtype, qulonglong N, qulonglong D,
   mdouble &a, mdouble &b, int q, QColor color, int mode) //5, white, 1
{  uint n; int j, i, k, i0, k0; mndAngle t; t.setAngle(N, D, j);
   mdouble logR = 14.0, x, y, u;
   u = exp(0.5*logR); y = t.radians();
   x = u*cos(y); y = u*sin(y);
   if (pointToPos(x, y, i0, k0) > 1) i0 = -10;
   stop(); QPainter *p = new QPainter(buffer); p->setPen(color);
   for (n = 1; n <= (nmax > 5000u ? 5000u : nmax + 2); n++)
   {  t.twice();
      for (j = 1; j <= q; j++)
      {  if (mode & 4 ? tricornNewton(signtype, n, a, b, x, y,
                exp(-j*0.69315/q)*logR, t.radians())
           : rayNewton(signtype, n, a, b, x, y,
                exp(-j*0.69315/q)*logR, t.radians()) )
         { n = (n <= 20 ? 65020u : 65010u); break; }
         if (pointToPos(x, y, i, k) > 1) i = -10;
         if (i0 > -10)
         {  if (i > -10) { if (!(mode & 8)) p->drawLine(i0, k0, i, k); }
            else if (!mode & 2) { n = 65020u; break; }
         }
         i0 = i; k0 = k;
      }
   }
   //if rayNewton fails after > 20 iterations, endpoint may be accepted
   delete p; update(); if (n >= 65020u) return 2;
   if (mode & 2) { a = x; b = y; }
   if (n >= 65010u) return 1; else return 0;
} //newtonRay

int QmnPlane::rayNewton(int signtype, uint n, mdouble a, mdouble b,
   mdouble &x, mdouble &y, mdouble rlog, mdouble ilog)
{  uint k, l; mdouble fx, fy, px, py, u, v = 0.0, d = 1.0 + x*x + y*y, t0, t1;
   for (k = 1; k <= 60; k++)
   {  if (signtype > 0) { a = x; b = y; }
      fx = cos(ilog); fy = sin(ilog);
      t0 = exp(rlog)*fx - 0.5*exp(-rlog)*(a*fx + b*fy);
      t1 = exp(rlog)*fy + 0.5*exp(-rlog)*(a*fy - b*fx);
      fx = x; fy = y; px = 1.0; py = 0.0;
      for (l = 1; l <= n; l++)
      {  u = 2.0*(fx*px - fy*py); py = 2.0*(fx*py + fy*px);
         px = u; if (signtype > 0) px++;
         u = fx*fx; v = fy*fy; fy = 2.0*fx*fy + b; fx = u - v + a;
         u += v; v = px*px + py*py; if (u + v > 1.0e100) return 1;
      }
      fx -= t0; fy -= t1; if (v < 1.0e-50) return 2;
      u = (fx*px + fy*py)/v; v = (fx*py - fy*px)/v;
      px = u*u + v*v; if (px > 9.0*d) return -1;
      x -= u; y += v; d = px; if (px < 1.0e-28 && k >= 5) break;
   }
   return 0;
} //rayNewton

int QmnPlane::tricornNewton(int signtype, uint n, mdouble a, mdouble b,
   mdouble &x, mdouble &y, mdouble rlog, mdouble ilog)
{  uint k, l; // Iterating  z^2 + c  or  + _c  instead of  _z^2 + c .
   mdouble fx, fy, px, py, qx, qy, u, v, d = 1.0 + x*x + y*y, t0, t1;
   for (k = 1; k <= 60; k++)
   {  if (signtype > 0) { a = x; b = y; }
      fx = cos(ilog); fy = sin(ilog);
      t0 = exp(rlog)*fx;// - 0.5*exp(-rlog)*(a*fx - b*fy);
      t1 = exp(rlog)*fy;// + 0.5*exp(-rlog)*(b*fx + a*fy);
      fx = x; fy = y; px = 1.0; py = 0.0; qx = 0.0; qy = 0.0;
      for (l = 1; l <= n; l++)
      {  u = 2.0*(fx*px - fy*py); py = 2.0*(fx*py + fy*px); px = u;
         if (signtype > 0)
         {  u = 2.0*(fx*qx - fy*qy); qy = 2.0*(fx*qy + fy*qx); qx = u;
            if (l & 1) qx++; else px++;
         }
         u = fx*fx; v = fy*fy; fy *= 2.0*fx; fx = u - v + a;
         if (l & 1) fy -= b; else fy += b;
         u += v; v = px*px + py*py + qx*qx + qy*qy;
         if (u + v > 1.0e100) return 1;
      }
      v = px*px + py*py - qx*qx - qy*qy;
      if (v > -1.0e-50 && v < 1.0e-50) return 2;
      fx -= t0; fy -= t1;
      t0 = (px - qx)*fx + (py - qy)*fy; t1 = (px + qx)*fy - (py + qy)*fx;
      u = t0/v; v = t1/v;
      px = u*u + v*v; if (px > 9.0*d) return -1;
      x -= u; y -= v; d = px; if (px < 1.0e-28 && k >= 5) break;
   }
   return 0;
} //tricornNewton

bool QmnPlane::saveEPS(int bpp, const QString &fileName)
{  //Save the whole buffer,  or the content of the frame,
   //in an EPS-file with 1, 2, or 4 bits-per-pixel.  If possible,  the
   //image is shrunk to fit on one page (does not matter when included).
   int i1 = 0, k1 = 0, w = 2*imax, h = 2*kmax;
   if (hframe > 0)
   {  pointToPos(x, y, i1, k1); i1 -= 4*hframe; k1 -= 4*vframe;
      w = 8*hframe; h = 8*vframe;
   }
   int i2 = i1 + w - 1, k2 = k1 + h - 1;
   if (i1 < 0 || i2 >= 2*imax || k1 < 0 || k2 >= 2*kmax) return false;
   int i = w, k = h, cl;
   while ((i > 500 || k > 700) && !(i & 1) && !(k & 1)) { i /= 2; k /= 2; }
   stop(); QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
   QTextStream outputfile(&file);
   outputfile
   << "%!PS-Adobe-3.0 EPSF-3.0\n"
   << "%%Creator: Mandel 5.9 by Wolf Jung\n"
   << "%%BoundingBox: 50 " << 750 - k << ' ' << 50 + i << " 750" << endl;
   if (bpp == 1) { cl = 1; outputfile
   << "/DeviceGray setcolorspace" << endl; }
   if (bpp == 2) { cl = 3; outputfile //(11*r + 16*g + 5*b)/32,old 00 4d 96 1c
   << "[/Indexed /DeviceRGB 3 <000000 ff0000 00ff00 0000ff>] setcolorspace\n"
   << "%[/Indexed /DeviceGray 3 <00 58 7f 28>] setcolorspace" << endl; }
   if (bpp == 3 || bpp == 4) { cl = 15; outputfile
   << "[/Indexed /DeviceRGB 15\n"
   << "<000000 000080 008000 008080 800000 800080 808000 c0c0c0\n"
   << " 808080 0000ff 00ff00 00ffff ff0000 ff00ff ffff00 ffffff>\n"
   << "] setcolorspace" << endl; }
   outputfile
   << "50 " << 750 - k << " translate\n"
   << i << ".0 " << k << ".0 scale\n"
   << "<<\n"
   << "/ImageType 1\n"
   << "/Width " << w << '\n'
   << "/Height " << h << '\n'
   << "/BitsPerComponent " << (bpp == 3 ? 4 : bpp) << '\n'
   << "/Decode [0 " << cl << "]\n"
   << "/ImageMatrix [" << w << " 0 0 " << -h << " 0 " << h <<"]\n"
   << "/DataSource currentfile /ASCIIHexDecode filter\n"
   << ">>\n"
   << "image" << endl;
   int j = 0; QRgb rgb, myrgb[16];
   if (bpp == 3 || bpp == 4)
   {  QColor mycolor[] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan,
      Qt::darkRed, Qt::darkMagenta, Qt::darkYellow, Qt::lightGray,
      Qt::darkGray, Qt::blue, Qt::green, Qt::cyan, Qt::red, Qt::magenta,
      Qt::yellow, Qt::white}; //is VGA, below with white / black interchanged
      if (bpp == 3)
      { bpp = 4; mycolor[0] = Qt::white; mycolor[15] = Qt::black; }
      for (cl = 0; cl < 16; cl++) myrgb[cl] = mycolor[cl].rgb();
   }
   QImage *image = new QImage(w, 1, QImage::Format_RGB32);
   QPainter *p = new QPainter(image);
   for (k = k1; k <= k2; k++)
   {  p->drawImage(0, 0, *buffer, i1, k, w, 1);
      if (bpp == 1) for (i = 0; i < w; i += 4)
      {  cl = 0; //green and cyan become white, red and magenta as well:
         rgb = image->pixel(i, 0);
         if (((rgb & 0x00ff00) && !(rgb & 0xff0000)) ||
             ((rgb & 0xff0000) && !(rgb & 0x00ff00))) cl = 8;
         rgb = image->pixel(i + 1, 0);
         if (((rgb & 0x00ff00) && !(rgb & 0xff0000)) ||
             ((rgb & 0xff0000) && !(rgb & 0x00ff00))) cl |= 4;
         rgb = image->pixel(i + 2, 0);
         if (((rgb & 0x00ff00) && !(rgb & 0xff0000)) ||
             ((rgb & 0xff0000) && !(rgb & 0x00ff00))) cl |= 2;
         rgb = image->pixel(i + 3, 0);
         if (((rgb & 0x00ff00) && !(rgb & 0xff0000)) ||
             ((rgb & 0xff0000) && !(rgb & 0x00ff00))) cl |= 1;
         outputfile << hex << cl; j++;
         if (j == 72) { outputfile << endl; j = 0; }
      }
      if (bpp == 2) for (i = 0; i < w; i += 2)
      {  cl = 0; //violet -> red, cyan -> blue, yellow -> green, w -> b:
         rgb = image->pixel(i, 0);
         if ((rgb & 0xff0000) && !(rgb & 0x00ff00)) cl = 4;
         if ((rgb & 0x00ff00) && !(rgb & 0x0000ff)) cl = 8;
         if ((rgb & 0x0000ff) && !(rgb & 0xff0000)) cl = 12;
         rgb = image->pixel(i + 1, 0);
         if ((rgb & 0xff0000) && !(rgb & 0x00ff00)) cl |= 1;
         if ((rgb & 0x00ff00) && !(rgb & 0x0000ff)) cl |= 2;
         if ((rgb & 0x0000ff) && !(rgb & 0xff0000)) cl |= 3;
         outputfile << hex << cl; j++;
         if (j == 72) { outputfile << endl; j = 0; }
      }
      if (bpp == 4) for (i = 0; i < w; i++)
      {  rgb = image->pixel(i, 0); cl = 9; //for ^p
         for (h = 0; h < 16; h++) if (myrgb[h] == rgb) { cl = h; break; }
         outputfile << hex << cl; j++;
         if (j == 72) { outputfile << endl; j = 0; }
      }
      if (j) { outputfile << endl; j = 0; }
   }
   outputfile
   << ">\n"
   << "showpage" << endl;
   file.close(); delete p; delete image; return true;
}//saveEPS

bool QmnPlane::saveTXT(const QString &fileName)
{  stop(); QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
   QTextStream outputfile(&file);
   int i, k, r; QRgb rgb;
   for (i = 0; i < 2*imax; i++) for (k = 0; k < 2*kmax; k++)
   {  rgb = buffer->pixel(i, k) & 0xffffff; //truncate argb
      r = (int)(rgb >> 16);
      if (r) outputfile << i << ' ' << k << ' ' << r << endl;
   }
    file.close(); return true;
}//saveTXT

bool QmnPlane::savePNG(const QString &fileName, QmnPlane *plane, int mark)// =00
{  //Save the whole buffer,  or the content of the frame,  as a PNG-file.
   //Or save together with plane,  not checking the latter's size.
   stop(); QImage *pix = buffer;
   if (mark && !plane) //four pics
   {  QPainter *p = new QPainter(buffer); int i = 0, k = 0;
      if (mark == 1 || mark == 4) i = imax; if (mark > 2) k = kmax;
      pix = new QImage(); pix->load(fileName, "PNG");
      p->drawImage(i, k, *pix, 0, 0, imax, kmax); p->setPen(Qt::darkGray);
      p->drawLine(imax-1, 0, imax-1, 2*kmax-1);
      p->drawLine(imax, 0, imax, 2*kmax-1);
      p->drawLine(0, kmax-1, 2*imax-1, kmax-1);
      p->drawLine(0, kmax, 2*imax-1, kmax);
      delete pix; delete p; return true;
   }
   if (plane)
   {  pix = new QImage(4*imax + 32, 2*kmax, QImage::Format_RGB32);
      plane->stop(); QPainter *p = new QPainter(pix); int i, k;
      pix->fill(QColor(Qt::gray).rgba());
      p->setPen(Qt::darkGray); p->drawLine(2*imax+31, 0, 2*imax+31, 2*kmax - 1);
      p->drawLine(2*imax, 2*kmax - 1, 2*imax + 31, 2*kmax - 1);
      p->setPen(Qt::lightGray); p->drawLine(2*imax, 0, 2*imax, 2*kmax - 1);
      p->drawLine(2*imax, 0, 2*imax + 31, 0);
      p->drawImage(0, 0, *buffer, 0, 0, 2*imax, 2*kmax);
      p->drawImage(2*imax + 32, 0, *(plane->buffer), 0, 0, 2*imax, 2*kmax);
      p->setPen(Qt::white);
      if ((mark & 1) && !(pointToPos(x, y, i, k)))
      { p->drawLine(i - 4, k, i + 4, k); p->drawLine(i, k - 4, i, k + 4); }
      if ((mark & 3) && !(plane->pointToPos(plane->x, plane->y, i, k)))
      {  p->drawLine(2*imax + 28 + i, k, 2*imax + 36 + i, k);
	 p->drawLine(2*imax + 32 + i, k - 4, 2*imax + 32 + i, k + 4);
      }
      delete p;
   }
   else if (hframe > 0)
   {  int i, k; pointToPos(x, y, i, k);
      pix = new QImage(8*hframe, 8*vframe, QImage::Format_RGB32);
      QPainter *p = new QPainter(pix);
      p->drawImage(0, 0, *buffer, i-4*hframe, k-4*vframe, 8*hframe, 8*vframe);
      delete p;
   }
   bool ok = pix->save(fileName, "PNG");
   if (hframe > 0 || plane) delete pix;
   return ok;
}

bool QmnPlane::loadPNG(const QString &fileName)
{  //Loads a PNG-file into the buffer if the sizes agree.
   stop(); QImage *newbuffer = new QImage();
   if (!newbuffer->load(fileName, "PNG")) { delete newbuffer; return false; }
   if (newbuffer->width() == 2*imax && newbuffer->height() == 2*kmax)
   { delete buffer; buffer = newbuffer; update(); return true; }
   delete newbuffer; return false;
}//problem with external files, maybe when grayscale*/

void QmnPlane::catMap(int mode) //1 forward, 0 backward
{  if (isRunning() || imax != kmax) return;

   /*unsigned int i, k, j, l, n;
   for (k = 0; k < kmax/4; k++) for (i = 0; i < imax/4; i++)
   {  n = 0;
      for (j = 0; j < 8; j++) for (l = 0; l < 8; l++)
         if (buffer->pixel(8*i + j, 8*k + l) & 0x00ff00) n++; //exclude white?
      if (n & 64) n = 0x00ff00;
      else if (n > 50) n = 0x00ffff;
      else if (n > 36) n = 0x0000ff;
      else if (n) n = 0x000080;
      buffer->setPixel(i, k, n);
   }
   update(); return; //test of supersampling*/

   /*for (int k = 0; k < 2*kmax; k++) //test scanline to replace QPainter
   { QRgb *sl = (QRgb*)(buffer->scanLine(k)); sl[100] = qRgb(255, 0, 0); }//*/

   if (type < 0) oldnmax = 0;
   else
   {  oldnmax = nmax; //oldmode = *drawmode;
      getPlane(oldhmid, oldvmid, oldrewidth, oldimwidth);
   }
   delete oldbuffer; oldbuffer = buffer; emit backMaybeChanged();
   buffer = new QImage(2*imax, 2*kmax, QImage::Format_RGB32);
   if (buffer->isNull())
   { delete buffer; buffer = oldbuffer; oldbuffer = new QImage(); return; }
   int i, k, I, K; QRgb rgb;
   for (k = 0; k < 2*kmax; k++)
   {  for (i = 0; i < 2*imax; i++)
      {  if (mode)
         {  I = 2*i + k; K = i + k;
            while (I >= 2*imax) I -= 2*imax; //twice
            if (K >= 2*kmax) K -= 2*kmax;
         }
         else
         {  I = i - k; K = 2*k - i;
            if (I < 0) I += 2*imax;
            if (K >= 2*kmax) K -= 2*kmax;
            if (K < 0) K += 2*kmax;
         }
         rgb = oldbuffer->pixel(I, K); buffer->setPixel(i, k, rgb);
      }
   } //*/
   update();
}

void QmnPlane::tilt(int mode) //1 color shows height, 0 uniform
{  if (isRunning()) return;
   int i, k, j, j0, u, tkm = 2*kmax - 1; int *r; QRgb rgb; r = new int[2*kmax];
   for (i = 0; i < 2*imax; i++)
   {  for (k = 0; k < 2*kmax; k++)
      {  rgb = buffer->pixel(i, tkm - k) & 0xffffff; //truncate argb
         r[k] = (int)(rgb >> 16);
         rgb = 0xffffff; buffer->setPixel(i, tkm - k, rgb);
      }
      j0 = 0; for (k = 0; k < 2*kmax; k++) for (j = j0; j < 2*kmax; j++)
      {  //parallel projection with slope -7/8 , r = 255 is 2*kmax/8
         u = 255*(8*k - 7*j); if (u < 0) { j0 = j; break; }
         if (u <= 2*kmax*r[j])
         {  u /= (2*kmax); if (!mode) u = r[j];
            rgb = (QRgb)(u); rgb <<= 16; rgb |= 255 - u;
            if (r[j]) buffer->setPixel(i, tkm - k, rgb);
            j0 = j; break;
         }
         if (j == 2*kmax - 1 && !r[j]) k = 2*kmax;
      }
   }
   delete[] r; update();
}

/*QRgb QmnPlane::getRGB(mdouble x0, mdouble y0)
{  int i, k; if (pointToPos(x0, y0, i, k)) return 0u;
   return buffer->pixel(i, k) & 0xffffff;
}//*/

void QmnPlane::replaceColor(int p, int q)
{  if (isRunning()) stop(); int i, i1 = 0, i2 = 2*imax, k, k1 = 0, k2 = 2*kmax;
   if (hframe > 0)
   {  pointToPos(x, y, i1, k1); i1 -= 4*hframe; k1 -= 4*vframe;
      i2 = i1 + 8*hframe; k2 = k1 + 8*vframe;
   }
   QRgb rgb, rgb1, rgb2;
   QColor mycolor[] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::darkCyan,
      Qt::darkRed, Qt::darkMagenta, Qt::darkYellow, Qt::lightGray,
      Qt::darkGray, Qt::blue, Qt::green, Qt::cyan, Qt::red, Qt::magenta,
      Qt::yellow, Qt::white}; //is VGA
   rgb1 = mycolor[p].rgb() & 0xffffff; rgb2 = mycolor[q].rgb() & 0xffffff;
   //rgb1 = 0xd0ffd0; rgb2 = 0xc8c8ff;
   for (k = k1; k < k2; k++) for (i = i1; i < i2; i++)
   {  rgb = buffer->pixel(i, k) & 0xffffff; //truncate argb
      if (rgb == rgb1) buffer->setPixel(i, k, rgb2);
      //else if (rgb == rgb2) buffer->setPixel(i, k, rgb1);
   }
   update();
}

void QmnPlane::overlay(QmnPlane *plane, QColor *color) // = 0
{  if (isRunning() || plane->isRunning()) return; int i, k; QRgb cl;
   /*for (k = 1; k < 2*kmax; k++) for (i = 0; i < 2*imax; i++)
   { cl = plane->buffer->pixel(i, k); buffer->setPixel(i, 2*kmax-k, cl); }
   update(); return;//*/
   /*for (k = 1; k < kmax; k++) for (i = 0; i < 2*imax; i++)
   { cl = buffer->pixel(i, k); buffer->setPixel(i, 2*kmax-k, cl); }
   update(); return;//*/
   if (color)
   {  cl = color->rgb();
      for (k = 0; k < 2*kmax; k++) for (i = 0; i < 2*imax; i++)
      if (!(plane->buffer->pixel(i, k) & 0x00ff00)) buffer->setPixel(i, k, cl);
   }
   else
   {  bool b;
      for (k = 0; k < 2*kmax; k++) for (i = 0; i < 2*imax; i++)
      {  b = (plane->buffer->pixel(i, k) & 0x00ff00);
         if (buffer->pixel(i, k) & 0x00ff00)
         { if (b) cl = 0x00ff00; else cl = 0xff0000; }
         else
         { if (b) cl = 0x0000ff; else cl = 0xff00ff; }
         buffer->setPixel(i, k, cl);
      }
   }
   update();
}

void QmnPlane::Overlay(QmnPlane *plane)
{  if (isRunning() || plane->isRunning()) return; int i, k; QRgb cl;
   for (k = 0; k < 2*kmax; k++) for (i = 0; i < 2*imax; i++)
      if ((plane->buffer->pixel(i, k) & 0xffffff) == 0xff0000)
      {  cl = buffer->pixel(i, k) & 0xffffff;
         if (cl == 0xff00ff) continue;
         if (cl == 0xff0000) cl = 0xff00ff; else cl = 0x0000ff;
         buffer->setPixel(i, k, cl);
      }
   update();
}

void QmnPlane::stop()
{  if (!thread->isRunning()) return;
   disconnect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
   thread->stop(); thread->wait();
   connect(thread, SIGNAL(finished()), this, SIGNAL(finished()));
}

bool QmnPlane::isRunning() const
{ return thread->isRunning(); }

/////////////////////////////////////////////////////////////////

void mandelSlow::init(int *ilist, mdouble *mdlist, qulonglong *qulist)
{  P = ilist[0]; sym = ilist[1]; imgno = ilist[2]; imax = ilist[3];
   xmid = mdlist[1]; ymid = mdlist[2]; invf = ((mdouble)(imax))/mdlist[0];
   D = qulist[0]; N1 = qulist[1]; N2 = qulist[2];
   if (!P) mpm = new mandelMating();
   if (P == 1) mpm = new mandelMate1();
   if (P == 2)
   { mpm = new mandelMating(); mpm->setMating(0, 2, -1.0L, 0.0L, 1); } 
   if (P == 3)
   {  mpm = new mandelMating();
      mpm->setMating(0, 3, -1.754877666246693L, 0.0L, 1);
      if (sym < 0)
      {  sym = 0;
         mpm->setMating(0, 3, -0.122561166876654L, 0.744861766619744L, 1);
      }
   } 
   if (sym) N2 = D >> 1;
   for (int j = 0; j < jmax; j++)
   {  image[j] = new QImage(2*imax, 2*imax, QImage::Format_RGB32);
      p[j] = new QPainter(image[j]); p[j]->setPen(Qt::red);
   }
   start(QThread::LowPriority);
}

//      mndynamics *F = new mndlbrot(2);

void mandelSlow::run()
{  int img, j, k, i1, k1, i0[jmax], k0[jmax];
   mdouble rc, ic, rd, id; qulonglong N, N0, D0; QString name;
/*   QImage *image[jmax]; QPainter *p[jmax];
   for (j = 0; j < jmax; j++)
   {  image[j] = new QImage(2*imax, 2*imax, QImage::Format_RGB32);
      p[j] = new QPainter(image[j]); p[j]->setPen(Qt::red);
   }//*/
   for (img = imgno; img <= imgno + 4; img++)
   {  name = QString("1025.png");
      for (j = 0; j < jmax; j++) image[j]->load(name, "PNG");
     /* for (N = N1; N <= N2; N++)
      {  N0 = N; D0 = D; k = 0;
         while (!(N0 & 1ULL) && !(D0 & 1ULL)) { N0 >>= 1; D0 >>= 1; }
         while (!(D0 & 1ULL)) { k++; D0 >>= 1; }
        // if (!k || dplane->newtonRay(1, N, D, rc, ic, 5, Qt::white, 10) > 1
        //  || F->find(1, k, 6, rc, ic) ) continue;
  rc = 0.0L; ic = 1.0L; k = 1;
         mpm->setMating(k, 6, rc, ic, (P ? 1 : 0));
         if (!P)
         {  if (k <= 1) continue; //here N/D <= 1/2 :
        //    if (dplane->newtonRay(1, N, D>>1, rc, ic, 5, Qt::white, 10) > 1
        //      || F->find(1, k - 1, 6, rc, ic) ) continue;
            mpm->setMating(k - 1, 6, rc, -ic, 0);
         }
         mpm->init((img - 1025)*0.040001L);
         for (j = 0; j < jmax; j++)
         {  if (j) mpm->step(rc, ic);
            mpm->getParameters(rc, ic, rd, id, P);
            if (sym < 0) { rd = rc*rc + ic*ic; rc /= rd; ic = -ic/rd; }
            //i1 = imax + (int)(invf*(rc - xmid));
            //k1 = imax + (int)(invf*(ymid - ic));
            rc = invf*(rc - xmid); ic = invf*(ymid - ic);
            if (rc*rc + ic*ic > 1.0e15L) continue;
            i1 = imax + (int)(rc); k1 = imax + (int)(ic);
            if (N > N1)
            {  p[j]->drawLine(i0[j], k0[j], i1, k1);
               if (sym)
                p[j]->drawLine(i0[j], 2*imax - k0[j] - 1, i1, 2*imax - k1 - 1);
            }
            i0[j] = i1; k0[j] = k1;
            if (sym && P && j >= (P == 2 ? 12 : 8) && 3ULL*N >= D) break;
         }
      } //*/
      for (j = 0; j < jmax; j++)
      { name = QString("%1.png").arg(img + 25*j); image[j]->save(name, "PNG");}
   }
}
/*
mandelSlow::~mandelSlow()
{  terminate(); wait(); delete mpm; //delete F;
   for (int j = 0; j < jmax; j++) { delete p[j]; delete image[j]; }
}//*/

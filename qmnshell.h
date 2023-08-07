/* qmnshell.h  by Wolf Jung (C) 2007-2019.
   Defines icons and declares classes:
   [defined in qmnshell.cpp :] QmnShell
   [defined in qmndialg.cpp :] QmnHelp, QmnUIntDialog, QmnDoubleDialog,
   QmnCombiDialog, QmnRayDialog.
   [defined in qmndemos.cpp :] QmnDemo, QmnDemoDS, QmnDemoPB, QmnDemoER,
   QmnDemoCC, QmnDemoRN, QmnDemoAS, QmnDemoLS, QmnDemoQC, QmnDemoCZ.

   These classes are part of Mandel 5.17, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/
#ifndef QMNSHELL_H_INCLUDED
#define QMNSHELL_H_INCLUDED

#include <QApplication>
#include <QMainWindow>
#include <QDialog>
#include <QTabWidget>
#include <QUrl>
//#include "mndynamo.h"
#include "qmnplane.h"

class QChar;
class QLabel;
class QLineEdit;
class QPixmap;
class QRadioButton;
class QScrollArea;
class QSpinBox;
class QTextBrowser;
class QmnHelp;

/* Compilation instructions:
   You need Qt 4.x or 5.x and the following files in a directory "mandel":
   mndynamo.h  mndynamo.cpp  mndlbrot.cpp  mndcombi.cpp
   qmnplane.h  qmnplane.cpp
   qmnshell.h  qmnshell.cpp  qmndemos.cpp qmndialg.cpp
   qmnicons.rc qmnicon32.ico
   qt_de.qm  mandel_de.qm  qt_pl.qm  mandel_pl.qm  qt_pt.qm  mandel_pt.qm
   From a command shell in this directory,  run  "qmake -project"  to
   build the project file  mandel.pro . On Windows,  edit this file
   to add the line  "RC_FILE = qmnicons.rc"  and move *.qm to
   mandel\release.  Build the  makefile  with  "qmake" , then  "make" .
*/

/* The documentation of the source code is still under construction ...
   In short: for the structure of the menus, actions, and slots,  please
   compare  createActions()  and  updateActions()  to the behavior of
   the executable and look for the if-statements in the various slots.
   The variables are initialized by functions in the order
   main -> constructor -> setF -> updateActions -> draw(homeAct) .
   The most tricky part is the synchronization of the two drawing threads
   by  pMoved()  and  pFinished() : these must not be drawing at the
   same time,  and sometimes  dplane  must wait for  pplane  without
   blocking the user interface.

   pMoved() is called when the point c has changed.  It is resetting the
   coordinate display,  and scheduling the drawing of the Julia set in the
   right window if appropriate.  Note that drawing is done in a separate
   thread.  Since mndynamics is not thread-safe,  stop() is needed to ensure
   that only one thread is drawing,  either on the pplane or on the dplane.
*/

class QmnShell : public QMainWindow
{
   Q_OBJECT
public:
   QmnShell(int argc, char *argv[], QWidget *parent = 0);
protected slots:
   void updateBack();
   void pMoved();
   void dMoved();
   void pFinished();
   void dFinished();
   void dCompleted();
   void paraDyn();
   void resizeWin(QAction *act);
   void draw(QAction *act);
   void drawing(QAction *act);
   void drawRequested();
   void move(QAction *act);
   void map(QAction *act);
   void find(QAction *act);
   void setRay(QAction *act);
   void setPath(QAction *act);
   void setAS(QAction *act);
   void setLS(QAction *act);
   void setF(QAction *act);
   void setFile(QAction *act);
   void help(QAction *act);
protected:
   uint pmode, dmode, *themode;
   uint lsppp; //assumed to be at least 32bit
   int thesize, hskip, vskip, ftype, signtype, asn, drawLater, imgno;
   mdouble gamma;
   bool updateRegion, psizeChanged;
   mndynamics *f;
   mndScale *as;
   mandelPath *mpath;
   QWidget *thespace;
   QLineEdit *pcoords, *dcoords;
   QmnHelp *helptabs;
   QPixmap *spiderPix;
   QmnPlane *pplane, *dplane, *theplane;
   QScrollArea *pscroll, *dscroll;
   void createActions();
   void updateActions();
   void contextMenuEvent(QContextMenuEvent *event);
   void closeEvent(QCloseEvent *e) {QMainWindow::closeEvent(e); qApp->quit();}
   QMenu *fileMenu,
      *drawMenu,
      *pointMenu,
      *rayMenu,
      *pathMenu,
      *scaleMenu,
      *newMenu,
      *helpMenu,
      *epsMenu,
      *algoMenu,
      *chooseMenu,
      *levelMenu,
      *f3Menu,
      *f4Menu,
      *f5Menu,
      *f6Menu,
      *f7Menu,
      *f8Menu;
   QAction*paraDynAct,
      *keepRegionAct,
      *sphereAct,
      *sizeAct,
      *colorAct,
      *epsActs[4],
      *epsSaveAct,
      *epsMenuAct,
      *pngSaveAct,
      *bothSaveAct,
      *pngLoadAct,
      *overlayAct,
      *videoAct,
      *catmapAct,
      *catinvAct,
      *exitAct,
      *stepAct,
      *backAct,
      *zoominAct,
      *zoomoutAct,
      *homeAct,
      *redrawAct,
      *algoActs[12],
      *nmaxAct,
      *algoAct,
      *renormAct,
      *entroAct,
      *killAct,
      *killframeAct,
      *drawPathAct,
      *rturnAct,
      *lturnAct,
      *uturnAct,
      *dturnAct,
      *rightAct,
      *leftAct,
      *upAct,
      *downAct,
      *plusAct,
      *minusAct,
      *greaterAct,
      *lessAct,
      *tiltAct,
      *textAct,
      *coordsAct,
      *traceAct,
      *markAct,
      *orbitAct,
      *fAct,
      *inv1Act,
      *inv2Act,
      *pointAct,
      *bifurcateAct,
      *bifucentAct,
      *greenAct,
      *rayAct,
      *rayPointAct,
      *portraitAct,
      *laminatAct,
      *wakeAct,
      *kneadAct,
      *spider1Act,
      *spider2Act,
      *mate1Act,
      *mate2Act,
      *pathAct,
      *captAct,
      *levelaAct,
      *level0Act,
      *level1Act,
      *level2Act,
      *level3Act,
      *levelmAct,
      *chooseActs[10],
      *FeigenbaumAct,
      *customAct,
      *chooseAct,
      *levelAct,
      *orderAct,
      *locsimAct,
      *degreeAct,
      *f0Act,
      *f1Act,
      *f2Act,
      *f3Act,
      *f3Acts[10],
      *f4Act,
      *f4Acts[10],
      *f5Act,
      *f5Acts[14],
      *f6Act,
      *f6Acts[13],
      *f7Act,
      *f7Acts[10],
      *f8Act,
      *f8Acts[10],
      *helpAct,
      *aboutAct,
      *demoActs[11];
};

/* The help browser  QmnHelp  provides help pages in a tab widget.  It
   is just hidden when closed,  and invoked either on its current page as
   a source of general help,  or invoked on a specific page to provide
   context help.  There are two buttons for closing and changing the font.
*/
class QmnHelp : public QDialog
{
   Q_OBJECT
public:
   QmnHelp(QWidget *parent = 0);
public slots:
   void showPage(int index);
protected slots:
   void changeFont();
protected:
   QTabWidget *tabs;
};

/* All input dialogs contain three buttons.  The help-button will be disabled
   when page < 0.  Otherwise it request a help page via the connection
   connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
   The input dialog  QmnUIntDialog  serves four purposes:
   1) If  mode == 0 , show the text,  return  accept()  for Return and
      reject()  for Esc.
      If  step <= 0 , Esc will be disabled (only in this first case).
      If  step < 0, automatic wordWrap will be enabled.
   2) If  0 < mode <= 65000 , ask for an unsigned integer *x between
      *y and mode. *x is the default value.  Spin corrects the intermediate
      value to the last valid one -- I would rather disable Return.
   3) If  mode == 65001 , ask for a quotient *x / *y with numbers <= 9999.
   4) If  mode == 65002 , ask for *y or *x,*y with *x = 0 in the first case.
      *y is always non-zero.
*/
class QmnUIntDialog : public QDialog
{
   Q_OBJECT
public:
   QmnUIntDialog(QString text, uint *x, uint *y, uint mode,
      int page, QWidget *parent, int step = 1);
protected slots:
   void inputChanged();
   void inputFinished();
   void getHelp();
protected:
   uint Mode; int Page;
   uint *X, *Y;
   QSpinBox *spin;
   QLineEdit *edit;
   QPushButton *button1;
signals:
   void needHelp(int page);
};

/* The input dialog  QmnDoubleDialog  asks to enter one or two mdouble
   numbers with dot or comma as decimal separator,  or as a fraction.
   *x  and  *y  are not changed when input is canceled or invalid.
   After editing  *x , hit Tab to edit  *y .  If you hit Return at
   this point,  *y  remains unchanged.  When  y == 0 , only one number
   is requested.
*/
class QmnDoubleDialog : public QDialog
{
   Q_OBJECT
public:
   QmnDoubleDialog(QString text, mdouble *x, mdouble *y,
      int page, QWidget *parent);
protected slots:
   void inputChanged();
   void inputFinished();
   void getHelp();
protected:
   int Page;
   mdouble *X, *Y;
   QLineEdit *edit1, *edit2;
   QPushButton *button1;
signals:
   void needHelp(int page);
};

/* The input dialog  QmnCombiDialog  asks to enter an angle as a fraction or
   in  binary notation,  when  *x == 0 , or to enter a kneading sequence or
   internal address.  It returns numerator and denominator in the first case.
   You can apply the static functions from  mndAngle  to them or construct an
   mndAngle . In the second case,  either *x returns a binary encoding of the
   kneading sequence and  *y = 0 , or  *x = 0  and  *y  encodes the internal
   address.  These encodings are compatible with  mndCombi .
*/
class QmnCombiDialog : public QDialog
{
   Q_OBJECT
public:
   static bool fractionToNumbers(QString text, qulonglong &n, qulonglong &d);
   static void binaryToNumbers(QString text, qulonglong &n, qulonglong &d);
   static bool numbersToBinary(qulonglong n, qulonglong d, QString &text);
   static int addressToCode(QString text, qulonglong &a);
   static int codeToAddress(qulonglong a, QString &text);
   static int kneadingToCode(QString text, qulonglong &k);
   static int codeToKneading(qulonglong k, QString &text);
   QmnCombiDialog(QString text, qulonglong *x, qulonglong *y, int page,
      QWidget *parent);
protected slots:
   void inputChanged();
   void inputFinished();
   void getHelp();
protected:
   static QChar A,B;
   int Page;
   qulonglong *X, *Y;
   QLineEdit *edit;
   QPushButton *button1;
   QRadioButton *radio[3];
signals:
   void needHelp(int page);
};

/* The input dialog  QmnRayDialog  asks to enter an angle as a fraction or in
   binary notation,  to choose the method,  and to adjust the quality.  When
   *qp > 0 , the period of rescaling is asked.  The dialog returns  accept()
   for Return and  reject()  for Esc.
   The smallest and largest value of  method  is given in *x and *y.  The
   angle is *x / *y . This dialog is used for the commands e and Ctrl+e.
   It contains some strings,  which are not given as arguments,  therefore
   it is not as flexible as the previous input dialogs.
*/
class QmnRayDialog : public QDialog
{
   Q_OBJECT
public:
   QmnRayDialog(QString text1, QString text2, qulonglong *x, qulonglong *y,
      uint *method, uint *qp, int page, QWidget *parent);
protected slots:
   void inputChanged();
   void inputFinished();
   void getHelp();
protected:
   uint *Method, *QP;
   int Page;
   qulonglong *X, *Y;
   QSpinBox *spin;
   QLineEdit *edit;
   QPushButton *button1;
   QRadioButton *radio[3];
signals:
   void needHelp(int page);
};

/* The base class  QmnDemo  provides a demo in a dialog window.  Its
   pages consist of a text,  two images,  and some buttons for animation,
   navigation through the pages,  closing,  getting help,  and changing the
   font.  In addition to the flexible main program,  here only a single action
   is available at most points of time.  This feature serves two purposes:
   * The novice user may follow the animation without previous training in
     the usage of the main program.
   * All users benefit from animations that are available in the main program
     only through combinations of many commands.  E.g., external rays may be
     drawn and periodic points be marked while the parameter is changing.  Or
     the parameter may be moved along the boundary of a hyperbolic component.

   Derived classes set  pageMax , implement  showPage() , go() , drawRight() .
   They may implement  move() , pFinished() , and  dFinished()   as well.
   The goButton can invoke mapping,  zooming,  movement,  changing the order
   of something,  or the next step of drawing.  Depending on the page,  moving
   the point by clicking or with the arrow keys may be enabled.  Since the
   arrow keys and page keys are used here,  they are no longer available for
   scrolling the text,  only Home and End remain.  These are coded explicitly,
   because the scroll area lost focus.  Hyperlinks are opened externally,  but
   for the scheme "javascript,"  the text will be displayed in a message box.

   The function  showPage()  is setting most variables,  displaying the text
   and doing some of the drawing.  The dynamic plane is drawn usually by
   drawRight()  when the parameter has changed.  The functions  pFinished()
   and  dFinished()  may add, e.g., external rays.  The function  go()
   performs the animation according to the current page.  When it does
   nothing,  the button  shall be disabled to inform the user.  move()  is
   invoked by the arrow keys;  it may be moving the point when the plane is
   enabled.  Two instances of  mndlbrot  are used to avoid synchronizing two
   drawing threads.  An active  QmnPlane  may be clicked and framed with the
   mouse.  The latter feature is not intended,  but it does little harm.
*/
class QmnDemo : public QDialog
{
   Q_OBJECT
public:
   QmnDemo(const QString title, QWidget *parent = 0);
   ~QmnDemo() { pplane->stop(); delete pf; dplane->stop(); delete df; }
protected slots:
   virtual void go() {};
   virtual void drawRight() {};
   virtual void pFinished() {};
   virtual void dFinished() {};
   virtual void polyline();
   virtual void move(QAction *act);
   virtual void navi(QAction *act);
   virtual void back();
   virtual void next();
   virtual void getHelp();
   virtual void changeFont();
   virtual void openLink(const QUrl link);
protected:
   uint mode;
   int page, pageMax, n, steps, points;
   mdouble *xp, *yp;
   mndynamics *pf, *df;
   QTextBrowser *text;
   QmnPlane *pplane, *dplane;
   QLabel *label;
   QAction *homeAct, *endAct, *rightAct, *leftAct, *upAct, *downAct;
   QPushButton *goButton, *backButton, *nextButton;
   virtual void showPage(bool) {};
signals:
   void needHelp(int page);
};

class QmnDemoCZ : public QmnDemo  //Celia & Zach
{
   Q_OBJECT
public:
   QmnDemoCZ(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent)
      { string = new QString(); pageMax = 9; next(); }
   ~QmnDemoCZ() { delete rabbit; delete witch; }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
   virtual void showCoords();
protected:
   QString *string;
   QPixmap *witch, *rabbit;
   virtual void showPage(bool backwards);
};

class QmnDemoDS : public QmnDemo  //dynamics and sets
{
   Q_OBJECT
public:
   QmnDemoDS(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 9; next(); }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   virtual void showPage(bool backwards);
};

class QmnDemoPB : public QmnDemo  //points and bifurcations
{
   Q_OBJECT
public:
   QmnDemoPB(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent)
   {  connect(pplane, SIGNAL(activated()), this, SLOT(nextLimb()));
      pageMax = 10; next();
   }
protected slots:
   virtual void go();
   virtual void drawRight();
//   virtual void pFinished();
   virtual void dFinished();
   virtual void move(QAction *act);
   virtual void nextLimb();
protected:
   virtual void showPage(bool backwards);
};

class QmnDemoER : public QmnDemo  //external rays
{
   Q_OBJECT
public:
   QmnDemoER(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent)
      { string = new QString(); pageMax = 12; next(); }
   ~QmnDemoER() { delete xp; delete yp; }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   QString *string;
   virtual void showPage(bool backwards);
};

class QmnDemoCC : public QmnDemo  //combinatorics
{
   Q_OBJECT
public:
   QmnDemoCC(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 10; next(); }
   ~QmnDemoCC() { delete spider; delete[] xp; delete[] yp; }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   QPixmap *spider;
   virtual void showPage(bool backwards);
};

class QmnDemoRN : public QmnDemo  //renormalization
{
   Q_OBJECT
public:
   QmnDemoRN(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 10; next(); }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   virtual void showPage(bool backwards);
};

class QmnDemoAS : public QmnDemo  //asymptotic similarity
{
   Q_OBJECT
public:
   QmnDemoAS(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 10; next(); }
   ~QmnDemoAS() { delete as; delete[] t; }
protected slots:
   virtual void go();
//   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   mdouble *t, a, b;
   mndScale *as;
   virtual void showPage(bool backwards);
};

class QmnDemoLS : public QmnDemo  //local similarity
{
   Q_OBJECT
public:
   QmnDemoLS(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 7; next(); }
protected slots:
   virtual void go();
   virtual void drawRight();
//   virtual void pFinished();
   virtual void dFinished();
protected:
   virtual void showPage(bool backwards);
};

class QmnDemoQC : public QmnDemo  //qc surgery
{
   Q_OBJECT
public:
   QmnDemoQC(const QString title, QWidget *parent = 0) :
      QmnDemo(title, parent) { pageMax = 10; pg = 0; dg = 0; next(); }
   ~QmnDemoQC() { delete pg; delete dg; delete xp; delete yp; }
protected slots:
   virtual void go();
   virtual void drawRight();
   virtual void pFinished();
   virtual void dFinished();
protected:
   mndynamics *pg, *dg;
   virtual void showPage(bool backwards);
};

#ifdef USING_QmnSpider16
static const char *const QmnSpider16[]={
"16 16 4 1",
". c None",
"b c #ff8000",
"a c #ffff00",
"# c #800000",
"................",
"................",
"................",
"................",
"......b...b.....",
".......b.b......",
"....bb.b.b.bb...",
"......bbabb.....",
".......aaa......",
"......bbabb.....",
"....bb.b.b.bb...",
".......b.b......",
"......b...b.....",
"................",
"................",
"................"};
#endif

#ifdef USING_QmnWitch16
static const char *const QmnWitch16[]={
"16 16 4 1",
". c None",
"b c #800000",
"a c #ff0000",
"# c #800000",
".......#........",
"......###.......",
"......bab.......",
".....aaaaa......",
"......a#a.......",
".......a.....b..",
".....aaaaa...b..",
"....a.aaa.a.b...",
"...a..aaa..aa...",
"..a...aaa.......",
"......###.......",
".....#####......",
".....#####......",
"......a.a.......",
"......a.a.......",
"......#.#......."};
#endif

#ifdef USING_QmnRabbit16
static const char *const QmnRabbit16[]={
"16 16 4 1",
". c None",
"b c #000000",
"a c #ff0000",
"# c #800000",
".........#......",
"........##......",
".......##.......",
"......##........",
"....###.........",
"..##aa#.........",
".#aaba#.........",
".##aaaa##.......",
"...#aaaaa###....",
"...#aaaaaaaa##..",
"...#aaaaaaaaaa#.",
"...#aaa####aaaa#",
"....#a#....#aaa#",
"....#a#.....#a#.",
"..##a#....##a#..",
"..###.....####.."};
#endif

#ifdef USING_QmnIcon_xpm16
static const char *const QmnIcon_xpm16[]={
"16 16 5 1",
"x c None",
"b c #000000",
"a c #000080",
". c #00ff00",
"# c #ffffff",
"....#...........",
".....#.#####....",
"......##..a##...",
"....###...b.##..",
"...##.#.abb.a##.",
"..##...#bbbba.#.",
".##...b#bbbbb.#.",
".#bbbbbbbbbb..#.",
".##...b#bbbbb.#.",
"..##...#bbbba.#.",
"...##.#.abb.a##.",
"....###...b.##..",
"......##..a##...",
".....#.#####....",
"....#...........",
"....#..........."};
#endif

#ifdef USING_QmnIcon_xpm24
static const char *const QmnIcon_xpm24[]={
"24 24 5 1",
"x c None",
"b c #000000",
"a c #000080",
". c #00ff00",
"# c #ffffff",
".......#................",
".......#................",
".......#....####........",
"........#.###..###......",
"........###...a..##.....",
"......####....aa..##....",
".....##..#....bb..a##...",
"....##....#...bb.aa.#...",
"...##..a..#abbbbb...##..",
"...#...aa.#bbbbbbba..#..",
"..#.....bbbbbbbbbb...#..",
"..#bbbbbbbbbbbbbb....#..",
"..#.....bbbbbbbbbb...#..",
"...#...aa.#bbbbbbba..#..",
"...##..a..#abbbbb...##..",
"....##....#...bb.aa.#...",
".....##..#....bb..a##...",
"......####....aa..##....",
"........###...a..##.....",
"........#.###..###......",
".......#....####........",
"......##................",
"......#.................",
"......#................."};
#endif

#ifdef USING_QmnIcon_xpm32
static const char *const QmnIcon_xpm32[]={
"32 32 5 1",
"x c None",
"b c #000000",
"a c #000080",
". c #00ff00",
"# c #ffffff",
".......#........................",
"........#.......................",
"........#........#####..........",
".........#....####...###........",
".........#..###........##.......",
"..........###......a....##......",
".........##.........aa...##.....",
".......###.#........b.....##....",
"......##...#.......bb......##...",
".....##.....#..aa.bbb...a...#...",
"....##.......#..bbbbbbbaa...##..",
"...##....a...#.bbbbbbbbb.....#..",
"..##......a...#bbbbbbbbbba...#..",
".##.......abb.#bbbbbbbbbb....#..",
".#......a.bbbb#bbbbbbbbbb....#..",
".#.bbbbbbbbbbbbbbbbbbbbb.....#..",
".#......a.bbbb#bbbbbbbbbb....#..",
".##.......abb.#bbbbbbbbbb....#..",
"..##......a...#bbbbbbbbbba...#..",
"...##....a...#.bbbbbbbbb.....#..",
"....##.......#..bbbbbbbaa...##..",
".....##.....#..aa.bbb...a...#...",
"......##...#.......bb......##...",
".......###.#........b.....##....",
".........##.........aa...##.....",
"..........###......a....##......",
".........#..###........##.......",
"........#.....####...###........",
"........#........#####..........",
".......#........................",
".......#........................",
"......#........................."};
#endif

#ifdef USING_QmnIcon_xpm48
static const char *const QmnIcon_xpm48[]={
"48 48 5 1",
"x c None",
"b c #000000",
"a c #000080",
". c #00ff00",
"# c #ffffff",
".........#......................................",
".........#......................................",
"..........#.....................................",
"...........#....................................",
"...........#....................................",
"............#...................................",
"............#..........#########................",
".............#.......###.......###..............",
".............#.....###...........###............",
"..............#..###...............##...........",
"...............###.........aa.......##..........",
".............###............a.aa.....##.........",
"............##..#............a........##........",
"..........###...#..........a.b.........##.......",
".........##......#..........bbb.........#.......",
"........##.......#....a.....bbb....a....##......",
".......##.........#....a...bbbbbb..a.....#......",
"......##...........#....bbbbbbbbb.ba.....##.....",
".....##............##.a.bbbbbbbbbb........#.....",
"....##........a.....#..bbbbbbbbbbbb.a.....#.....",
"....#..........a....#.bbbbbbbbbbbbbb......#.....",
"...##..........abbb..#bbbbbbbbbbbbbba.....#.....",
"...#...........bbbbbb#bbbbbbbbbbbbbb......##....",
"...#.........a.bbbbbb#bbbbbbbbbbbbb........#....",
"..##.abbbbbbbbbbbbbbbbbbbbbbbbbbbb.........#....",
"...#.........a.bbbbbb#bbbbbbbbbbbbb........#....",
"...#...........bbbbbb#bbbbbbbbbbbbbb......##....",
"...##..........abbb..#bbbbbbbbbbbbbba.....#.....",
"....#..........a....#.bbbbbbbbbbbbbb......#.....",
"....##........a.....#..bbbbbbbbbbbb.a.....#.....",
".....##............##.a.bbbbbbbbbb........#.....",
"......##...........#....bbbbbbbbb.ba.....##.....",
".......##.........#....a...bbbbbb..a.....#......",
"........##.......#....a.....bbb....a....##......",
".........##......#..........bbb.........#.......",
"..........###...#..........a.b.........##.......",
"............##..#............a........##........",
".............###............a.aa.....##.........",
"...............###.........aa.......##..........",
"...............#.###...............##...........",
"..............#....###...........###............",
"..............#......###.......###..............",
".............#.........#########................",
".............#..................................",
"............#...................................",
"............#...................................",
"...........#....................................",
"..........#....................................."};
#endif

#ifdef USING_QmnIcon_xpm64
static const char *const QmnIcon_xpm64[]={
"64 64 5 1",
"x c None",
"b c #000000",
"a c #000080",
". c #00ff00",
"# c #ffffff",
"..............#.................................................",
"...............#................................................",
"...............#................................................",
"................#...............................................",
"................#...............................................",
".................#................#########.....................",
"..................#............####.......###...................",
"..................#.........####............###.................",
"...................#......###.................###...............",
"...................#....###.....................##..............",
"....................#.###........................##.............",
"....................###...............a...........##............",
"..................###..................a.aa........##...........",
".................##..#..................aa..........##..........",
"...............###...#..................a............##.........",
"..............##......#..............a..b.a...........##........",
"............###.......#..............abbb.a............#........",
"...........##..........#.............abbbba............##.......",
"..........##...........#........a.....bbbb......a.......#.......",
".........##.............#.....aa....b.bbb..aa...a.......##......",
"........##..............#.......b.bbbbbbbbbbb..aaa.......#......",
".......##................#......bbbbbbbbbbbbbb.ba........##.....",
"......##.................#...aabbbbbbbbbbbbbbbbb..........#.....",
".....##...........a.......#...bbbbbbbbbbbbbbbbbb..........#.....",
"....##.............a...a..#...bbbbbbbbbbbbbbbbbbbaa.......#.....",
"....#..............aa..a...#..bbbbbbbbbbbbbbbbbbbb........##....",
"...##...............aa.b.a.#.bbbbbbbbbbbbbbbbbbbb..........#....",
"..##.................bbbbb.#.bbbbbbbbbbbbbbbbbbbbba........#....",
"..#................abbbbbbb.#bbbbbbbbbbbbbbbbbbbb..........#....",
"..#.............a..bbbbbbbbb#bbbbbbbbbbbbbbbbbbbb..........#....",
"..#......a.......bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbba..........#....",
"..#..bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb............#....",
"..#......a.......bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbba..........#....",
"..#.............a..bbbbbbbbb#bbbbbbbbbbbbbbbbbbbb..........#....",
"..#................abbbbbbb.#bbbbbbbbbbbbbbbbbbbb..........#....",
"..##.................bbbbb..#bbbbbbbbbbbbbbbbbbbbba........#....",
"...##...............aa.b.a.#.bbbbbbbbbbbbbbbbbbbb..........#....",
"....#..............aa..a...#..bbbbbbbbbbbbbbbbbbbb........##....",
"....##.............a...a..#...bbbbbbbbbbbbbbbbbbbaa.......#.....",
".....##...........a.......#...bbbbbbbbbbbbbbbbbb..........#.....",
"......##.................#...aabbbbbbbbbbbbbbbbb..........#.....",
".......##................#......bbbbbbbbbbbbbb.ba........##.....",
"........##..............#.......b.bbbbbbbbbbb..aaa.......#......",
".........##.............#.....aa....b.bbb..aa...a.......##......",
"..........##...........#........a.....bbbb......a.......#.......",
"...........##.........#..............abbbba............##.......",
"............###.......#..............abbb.a............#........",
"..............##.....#...............a..b.a...........##........",
"...............###..#...................a............##.........",
".................##.#...................aa..........##..........",
"..................###..................a.aa........##...........",
"..................#.###...............a...........##............",
"..................#...###........................##.............",
".................#......###.....................##..............",
".................#........###.................###...............",
"................#...........####............###.................",
"................#..............####.......###...................",
"...............#..................#########.....................",
"...............#................................................",
"..............#.................................................",
"..............#.................................................",
".............#..................................................",
".............#..................................................",
"............#..................................................."};
#endif

#endif //QMNSHELL_H_INCLUDED

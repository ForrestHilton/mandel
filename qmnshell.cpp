/* qmnshell.cpp  by Wolf Jung (C) 2007-2023.  Defines class QmnShell.

   These classes are part of Mandel 5.19, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.

&alpha;  \xce\xb1     \u03b1
&beta;   \xce\xb2     \u03b2
&gamma;  \xce\xb3     \u03b3
&theta;  \xce\xb8     \u03b8
&lambda; \xce\xbb     \u03bb
&mu;     \xce\xbc     \u03bc
&pi;     \xcf\x80     \u03c0
&rho;    \xcf\x81     \u03c1
&sigma;  \xcf\x83     \u03c3
&phi;    \xcf\x86     \u03c6
&Phi;    \xce\xa6     \u03a6
&le;     \xe2\x89\xa4 \u2264
&ge;     \xe2\x89\xa5 \u2265
&radic;  \xe2\x88\x9a \u221a
&ldquo;  \xe2\x80\x9c \u201c
&rdquo;  \xe2\x80\x9d \u201d
&infin;  \xe2\x88\x9e \u221e
&rarr;   \xe2\x86\x92
*/

#include <QtGui>
#if (QT_VERSION >= 0x050000)
#include <QtWidgets>
#endif
#include <cmath>
#define USING_QmnSpider16
#define USING_QmnIcon_xpm16
#define USING_QmnIcon_xpm64
#include "qmnshell.h"
//undefine the following to speed up computation of initial images
//#define SaveFramesOfVeinsLimbsX

const mdouble cFb = -1.40115518909205060052L, dFb = 4.66920160910299067185L,
   aFb = 2.50290787509589282228L;  //Fibonacci -1.8705286321646448888906L

void QmnShell::help(QAction *act)
{  if (act == helpAct) { helptabs->show(); helptabs->raise(); }
   if (act == aboutAct)
   {  QMessageBox mb(QMessageBox::NoIcon, trUtf8("About Mandel"), trUtf8(
      "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
      "<b>Mandel 5.19</b> of October 15, 2023.<br>"
      "Copyright &copy; Wolf Jung"
      "<p>This program comes with no warranty.<br>"
      "It may be redistributed and modified under<br>"
      "the terms of the <a href=\"http://www.gnu.org/copyleft/gpl.html\">"
      "GNU General Public Licence</a>."
      "<p>The latest version and the source code are<br>"
      "available from "
      "<a href=\"http://www.mndynamics.com\">www.mndynamics.com</a> ."
      "<p>This program employs the cross-platform C++<br>"
      "toolkit Qt from Qt Group. See "
      "<a href=\"http://www.qt.io/download-open-source/\">"
      "www.qt.io/download-open-source/</a> ."),
      QMessageBox::Ok, this);
      mb.setIconPixmap(QPixmap((const char **)(QmnIcon_xpm64)));
      mb.exec();
   }
   if (act == demoActs[0]) //see updateActions and F1
   {  QmnDemo *demo = new QmnDemoDS(trUtf8(
      "Mandel demo 0: real iteration"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[1])
   {  QmnDemo *demo = new QmnDemoDS(trUtf8(
      "Mandel demo 1: dynamics and sets"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[2])
   {  QmnDemo *demo = new QmnDemoPB(trUtf8(
      "Mandel demo 2: periodic points and bifurcations"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[3])
   {  QmnDemo *demo = new QmnDemoER(trUtf8(
      "Mandel demo 3: external rays"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[4])
   {  QmnDemo *demo = new QmnDemoCC(trUtf8(
      "Mandel demo 4: combinatorics"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[5])
   {  QmnDemo *demo = new QmnDemoRN(trUtf8(
      "Mandel demo 5: renormalization"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[6])
   {  QmnDemo *demo = new QmnDemoAS(trUtf8(
      "Mandel demo 6: asymptotic similarity at Misiurewicz points"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[7])
   {  QmnDemo *demo = new QmnDemoLS(trUtf8(
      "Mandel demo 7: local similarity"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[8])
   {  QmnDemo *demo = new QmnDemoQC(trUtf8(
      "Mandel demo 8: quasiconformal surgery"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[9]) //see updateActions and F1
   {  QmnDemo *demo = new QmnDemo(trUtf8(
      "Mandel demo 9: new families of functions"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
   if (act == demoActs[10])
   {  QmnDemo *demo = new QmnDemoCZ(trUtf8(
      "Mandel demo: Celia and Zach, a fairy tale"), 0);
      connect(demo, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      demo->showMaximized();
   }
}

int main(int argc, char *argv[])
{  QApplication app(argc, argv);
   QString locale = QLocale::system().name();
   QLocale::setDefault(QLocale::c()); //output decimal separator is dot
   if (argc > 1)
   {  bool ok; QString arg = QString(argv[argc - 1]);
      arg.toDouble(&ok); if (!ok) { locale = arg; argc--; }
   }
   QTranslator myTranslator, qtTranslator;
   if (!locale.contains("en")
       && myTranslator.load("mandel_" + locale, qApp->applicationDirPath()))
   {  app.installTranslator(&myTranslator);
      qtTranslator.load("qt_" + locale, qApp->applicationDirPath());
      app.installTranslator(&qtTranslator);
   }
   QmnShell *shell = new QmnShell(argc, argv);
   shell->show();
   return app.exec();
}

QmnShell::QmnShell(int argc, char *argv[], QWidget *parent)
   : QMainWindow(parent)
{  if (QApplication::desktop()->width() >= 1312) thesize = 640;
   else thesize = 480;
   pmode = 1; dmode = 1; themode = &pmode;
   as = new mndScale(); mpath = 0; imgno = 0;
   f = 0; //for delete in setF()
   setWindowIcon(QPixmap((const char **)(QmnIcon_xpm16)));
   spiderPix = new QPixmap((const char **)(QmnSpider16));
   QWidget *dummy = new QWidget(this); setCentralWidget(dummy);
   pcoords = new QLineEdit(QString(" "), 0);
   pcoords->setReadOnly(true); pcoords->setFocusPolicy(Qt::ClickFocus);
   dcoords = new QLineEdit(QString(" "), 0);
   dcoords->setReadOnly(true); dcoords->setFocusPolicy(Qt::ClickFocus);
   hskip = 20; vskip = pcoords->height();
   pplane = new QmnPlane(thesize, thesize, 0, 0);
   dplane = new QmnPlane(thesize, thesize, 0, 0);
   pscroll = new QScrollArea(0); pscroll->setFrameStyle(QFrame::NoFrame);
   dscroll = new QScrollArea(0); dscroll->setFrameStyle(QFrame::NoFrame);
   pscroll->setWidget(pplane); pscroll->setMaximumSize(thesize, thesize);
   dscroll->setWidget(dplane); dscroll->setMaximumSize(thesize, thesize);
   pscroll->setMinimumSize(thesize, thesize);
   dscroll->setMinimumSize(thesize, thesize);
   setMaximumWidth(2*thesize + hskip); setMaximumHeight(thesize + vskip);

   thespace = new QWidget(0);
   thespace->setFixedWidth(hskip); thespace->setMinimumHeight(10);
   thespace->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
   QGridLayout *grid = new QGridLayout(0);
   grid->setSpacing(0); grid->setMargin(0);
   grid->addWidget(pcoords, 0, 0);
   grid->addWidget(dcoords, 0, 2);
   grid->addWidget(pscroll, 1, 0);
   grid->addWidget(dscroll, 1, 2);
   grid->addWidget(thespace, 0, 1, 3, 1);
   dummy->setLayout(grid);

   theplane = pplane; createActions();
   connect(pplane, SIGNAL(backMaybeChanged()), this, SLOT(updateBack()));
   connect(dplane, SIGNAL(backMaybeChanged()), this, SLOT(updateBack()));
   connect(pplane, SIGNAL(activated()), this, SLOT(paraDyn()));
   connect(dplane, SIGNAL(activated()), this, SLOT(paraDyn()));
   connect(pplane, SIGNAL(moved()), this, SLOT(pMoved()));
   connect(dplane, SIGNAL(moved()), this, SLOT(dMoved()));
   connect(pplane, SIGNAL(finished()), this, SLOT(pFinished()));
   connect(dplane, SIGNAL(finished()), this, SLOT(dFinished()));
   connect(dplane, SIGNAL(completed()), this, SLOT(dCompleted()));
   connect(pplane, SIGNAL(drawRequest()), this,
     SLOT(drawRequested()));
   connect(dplane, SIGNAL(drawRequest()), this,
     SLOT(drawRequested()));
   helptabs = new QmnHelp(); helptabs->hide();

   setF(f0Act); //sets parameters and calls updateActions();
   if (argc == 3)
   {  bool ok; QString arg = QString(argv[1]);
      mdouble y, x = arg.toDouble(&ok); if (!ok) return;
      arg = QString(argv[2]); y = arg.toDouble(&ok); if (!ok) return;
      pplane->setPoint(x, y);
   }
   if (argc == 5)
   {  bool ok; QString arg = QString(argv[3]);
      mdouble y, x = arg.toDouble(&ok); if (!ok) return;
      arg = QString(argv[4]); y = arg.toDouble(&ok); if (!ok) return;
      setF(f5Acts[0]); pplane->stop(); f->startPlane(0, x, y);
      *themode = 4; draw(redrawAct);
      arg = QString(argv[1]); x = arg.toDouble(&ok); if (!ok) return;
      arg = QString(argv[2]); y = arg.toDouble(&ok); if (!ok) return;
      pplane->setPoint(x, y);
   }
}

void QmnShell::createActions()
{  QActionGroup *winAG = new QActionGroup(this);
   paraDynAct = new QAction(QString("&$"), winAG);
   paraDynAct->setShortcut(Qt::Key_F2);
   keepRegionAct = new QAction(QString("&$"), winAG);
   keepRegionAct->setShortcut(Qt::SHIFT+Qt::Key_F2);
   //do not use ctrl+F2, alt+F2, ctrl+alt+F2, *+F3
   sphereAct = new QAction(QString("&$"), winAG);
   sphereAct->setShortcut(Qt::Key_F3);
   sizeAct = new QAction(trUtf8("&Resize ..."), winAG);
   sizeAct->setShortcut(Qt::Key_F4);
   connect(winAG,SIGNAL(triggered(QAction*)),this, SLOT(resizeWin(QAction*)));
   this->addActions(winAG->actions());

   QActionGroup *epsAG = new QActionGroup(this);
   epsActs[1] = new QAction(trUtf8("&b/w ..."), epsAG);
   epsActs[2] = new QAction(trUtf8("&4 colors ..."), epsAG);
   epsActs[3] = new QAction(trUtf8("16 &colors, b <-> w ..."), epsAG);
   epsActs[0] = new QAction(trUtf8("&16 colors ..."), epsAG);
   connect(epsAG, SIGNAL(triggered(QAction*)), this, SLOT(setFile(QAction*)));
   //this->addActions(epsAG->actions());
   epsMenu = new QMenu(this); epsMenu->addActions(epsAG->actions());

   QActionGroup *vidAG = new QActionGroup(this);
   vidAct1 = new QAction(trUtf8("&1: Overlay ..."), vidAG);
   vidAct2 = new QAction(trUtf8("&2: Combine two ..."), vidAG);
   vidAct3 = new QAction(trUtf8("&3: Combine three ..."), vidAG);
   vidAct4 = new QAction(trUtf8("&4: Combine four..."), vidAG);
   vidAct5 = new QAction(trUtf8("&5: Change colors ..."), vidAG);
   QAction *vvSeparator = new QAction(this);
   vvSeparator->setSeparator(true); vidAG->addAction(vvSeparator);
   slowmateAct = new QAction(trUtf8("&6: Slow mating ..."), vidAG);
   slowmateAct->setShortcut(QString("d"));
   slowantiAct = new QAction(trUtf8("&7: Slow anti-mating ..."), vidAG);
   slowantiAct->setShortcut(QString("Ctrl+d"));
   veinAct2 = new QAction(trUtf8("&8: 1/2-vein ..."), vidAG);
   veinAct3 = new QAction(trUtf8("&9: 1/4-vein ..."), vidAG);
   limbAct2 = new QAction(trUtf8("&m: 1/2-limb ..."), vidAG);
   limbAct3 = new QAction(trUtf8("&n: 1/3-limb ..."), vidAG);
   limbAct4 = new QAction(trUtf8("&o: other limbs ..."), vidAG);
   connect(vidAG, SIGNAL(triggered(QAction*)), this, SLOT(setFile(QAction*)));
   vidMenu = new QMenu(this); vidMenu->addActions(vidAG->actions());

   QActionGroup *fileAG = new QActionGroup(this);
   colorAct = new QAction(trUtf8("&Change colors ..."), fileAG);
   colorAct->setShortcut(Qt::Key_F5);
   epsSaveAct = new QAction(trUtf8("Save .&eps ..."), fileAG);
   epsSaveAct->setShortcut(Qt::Key_F6);
   epsMenuAct = new QAction(trUtf8("Sa&ve .eps"), fileAG);
   epsMenuAct->setMenu(epsMenu);
   epsMenuAct->setShortcut(Qt::SHIFT+Qt::Key_F6);
   pngSaveAct = new QAction(trUtf8("Save .pn&g ..."), fileAG);
   pngSaveAct->setShortcut(Qt::Key_F7);
   bothSaveAct = new QAction(trUtf8("Save &both .png ..."), fileAG);
   bothSaveAct->setShortcut(Qt::SHIFT+Qt::Key_F7);
   pngLoadAct = new QAction(trUtf8("Load .p&ng ..."), fileAG);
   pngLoadAct->setShortcut(Qt::Key_F8);
   overlayAct = new QAction(trUtf8("Overlay for com&parison"), fileAG);
   overlayAct->setShortcut(Qt::SHIFT+Qt::Key_F8);
   QAction *fSeparator = new QAction(this);
   fSeparator->setSeparator(true); fileAG->addAction(fSeparator);
   videoAct = new QAction(trUtf8("&Video frames ..."), fileAG);
   videoAct->setShortcut(Qt::Key_F9);
   vidMenuAct = new QAction(trUtf8("Video &utilities"), fileAG);
   vidMenuAct->setMenu(vidMenu);
   vidMenuAct->setShortcut(Qt::SHIFT+Qt::Key_F9);
   QAction *ffSeparator = new QAction(this);
   ffSeparator->setSeparator(true); fileAG->addAction(ffSeparator);
   catmapAct = new QAction(trUtf8("Arnold cat &map"), fileAG);
   catmapAct->setShortcut(Qt::Key_F10);
   catinvAct = new QAction(trUtf8("&Inverse cat map"), fileAG);
   catinvAct->setShortcut(Qt::SHIFT+Qt::Key_F10);
   connect(fileAG, SIGNAL(triggered(QAction*)), this, SLOT(setFile(QAction*)));
   this->addActions(fileAG->actions());

   exitAct = new QAction(trUtf8("E&xit"), this);
   exitAct->setShortcut(Qt::ALT+Qt::Key_F4);
   connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
   this->addAction(exitAct);
   //Actually,  Alt+F4  may be intercepted by the OS,  which closes the window.
   //The  closeEvent  was reimplemented to quit in addition.

   QActionGroup *drawAG = new QActionGroup(this);
   stepAct = new QAction(trUtf8("&Step or zoom"), drawAG);
   stepAct->setShortcut(Qt::Key_Return);
   backAct = new QAction(trUtf8("&Back"), drawAG);
   backAct->setShortcut(Qt::Key_Backspace);
   zoominAct = new QAction(trUtf8("&Zoom in"), drawAG);
   zoominAct->setShortcut(QString("z"));
   zoomoutAct = new QAction(trUtf8("Zoom o&ut"), drawAG);
   zoomoutAct->setShortcut(QString("v"));
   homeAct = new QAction(trUtf8("Restar&t"), drawAG);
   homeAct->setShortcut(Qt::Key_Home);
   redrawAct = new QAction(trUtf8("Re&draw"), drawAG);
   redrawAct->setShortcut(Qt::CTRL+Qt::Key_Return);
   connect(drawAG, SIGNAL(triggered(QAction*)), this, SLOT(draw(QAction*)));
   this->addActions(drawAG->actions());

   QActionGroup *algoAG = new QActionGroup(this);
   algoActs[0] = new QAction(QString("&0"), algoAG);
   algoActs[0]->setShortcut(QString("0"));
   algoActs[1] = new QAction(trUtf8("&1: Comparing neighbors"), algoAG);
   algoActs[1]->setShortcut(QString("1"));
   algoActs[2] = new QAction(trUtf8("&2: Escape time"), algoAG);
   algoActs[2]->setShortcut(QString("2"));
   algoActs[3] = new QAction(trUtf8("&3: Escape time, rainbow colors"),algoAG);
   algoActs[3]->setShortcut(QString("3"));
   algoActs[4] = new QAction(trUtf8("&4: Marty normality"), algoAG);
   algoActs[4]->setShortcut(QString("4"));
   algoActs[5] = new QAction(trUtf8("&5: Distance estimate"), algoAG);
   algoActs[5]->setShortcut(QString("5"));
   algoActs[6] = new QAction(trUtf8("&6: Argument of \xce\xa6"), algoAG);
   algoActs[6]->setShortcut(QString("6"));
   algoActs[7] = new QAction(trUtf8("&7: Binary decomposition"), algoAG);
   algoActs[7]->setShortcut(QString("7"));
   algoActs[8] = new QAction(trUtf8("&8: Yoccoz puzzle 1/2 1/3 2/3"), algoAG);
   algoActs[8]->setShortcut(QString("8"));
   algoActs[9] = new QAction(trUtf8("&9: Zeros of q_n(c) [Q]"), algoAG);
   algoActs[9]->setShortcut(QString("9"));
   algoActs[10] = new QAction(trUtf8("&c: Cr. points of q_n(c) [Q]"), algoAG);
   algoActs[11] = new QAction(trUtf8("&n: Newton for q_n(c) [Q]"), algoAG);
   connect(algoAG, SIGNAL(triggered(QAction*)), this, SLOT(drawing(QAction*)));
   this->addActions(algoAG->actions());
   algoMenu = new QMenu(this); algoMenu->addActions(algoAG->actions());

   QActionGroup *drawingAG = new QActionGroup(this);
   nmaxAct = new QAction(trUtf8("&Iterations ..."), drawingAG);
   nmaxAct->setShortcut(QString("n"));
   algoAct = new QAction(trUtf8("&Algorithm"), drawingAG);
   algoAct->setShortcut(Qt::Key_Delete);
   algoAct->setMenu(algoMenu);
   renormAct = new QAction(trUtf8("&Renormalize ..."), drawingAG);
   renormAct->setShortcut(QString("r"));
   entroAct = new QAction(trUtf8("&Entropy"), drawingAG);
   entroAct->setShortcut(QString("h"));
   connect(drawingAG, SIGNAL(triggered(QAction*)),
      this, SLOT(drawing(QAction*)));
   this->addActions(drawingAG->actions());

   QActionGroup *moveAG = new QActionGroup(this);
   killAct = new QAction(QString("&$"), moveAG);
   killAct->setShortcut(Qt::Key_Escape);
   killframeAct = new QAction(QString("&$"), moveAG); //ctx menu
   drawPathAct = new QAction(QString("&$"), moveAG); //ctx menu
   rturnAct = new QAction(QString("&$"), moveAG);
   rturnAct->setShortcut(Qt::CTRL+Qt::Key_Right);
   lturnAct = new QAction(QString("&$"), moveAG);
   lturnAct->setShortcut(Qt::CTRL+Qt::Key_Left);
   uturnAct = new QAction(QString("&$"), moveAG);
   uturnAct->setShortcut(Qt::CTRL+Qt::Key_Up);
   dturnAct = new QAction(QString("&$"), moveAG);
   dturnAct->setShortcut(Qt::CTRL+Qt::Key_Down);
   rightAct = new QAction(QString("&$"), moveAG);
   rightAct->setShortcut(Qt::Key_Right);
   leftAct = new QAction(QString("&$"), moveAG);
   leftAct->setShortcut(Qt::Key_Left);
   upAct = new QAction(QString("&$"), moveAG);
   upAct->setShortcut(Qt::Key_Up);
   downAct = new QAction(QString("&$"), moveAG);
   downAct->setShortcut(Qt::Key_Down);
   plusAct = new QAction(QString("&$"), moveAG);
   plusAct->setShortcut(Qt::Key_Plus);
   minusAct = new QAction(QString("&$"), moveAG);
   minusAct->setShortcut(Qt::Key_Minus);
   greaterAct = new QAction(QString("&$"), moveAG);
   greaterAct->setShortcut(Qt::Key_Greater);
   lessAct = new QAction(QString("&$"), moveAG);
   lessAct->setShortcut(Qt::Key_Less);
   tiltAct = new QAction(QString("&$"), moveAG);
   tiltAct->setShortcut(QString("Ctrl+h"));
   textAct = new QAction(QString("&$"), moveAG);
   textAct->setShortcut(QString("Shift+h"));
   connect(moveAG, SIGNAL(triggered(QAction*)), this, SLOT(move(QAction*)));
   this->addActions(moveAG->actions()); //not visible in menus

   QActionGroup *movingAG = new QActionGroup(this);
   coordsAct = new QAction(trUtf8("&Coordinates ..."), movingAG);
   coordsAct->setShortcut(Qt::Key_Insert);
   markAct = new QAction(trUtf8("Mar&k"), movingAG);
   markAct->setShortcut(Qt::Key_NumberSign);
   connect(movingAG, SIGNAL(triggered(QAction*)), this, SLOT(move(QAction*)));
   this->addActions(movingAG->actions());

   QActionGroup *mapAG = new QActionGroup(this);
   orbitAct = new QAction(trUtf8("&Orbit"), mapAG);
   orbitAct->setShortcut(QString("Ctrl+f"));
   fAct = new QAction(trUtf8("map &f"), mapAG);
   fAct->setShortcut(QString("f"));
   inv1Act = new QAction(trUtf8("&1st inverse"), mapAG);
   inv1Act->setShortcut(QString("a"));
   inv2Act = new QAction(trUtf8("&2nd inverse"), mapAG);
   inv2Act->setShortcut(QString("b"));
   connect(mapAG, SIGNAL(triggered(QAction*)), this, SLOT(map(QAction*)));
   this->addActions(mapAG->actions());

   QActionGroup *findAG = new QActionGroup(this);
   pointAct = new QAction(trUtf8("Find &point ..."), findAG);
   pointAct->setShortcut(QString("x"));
   bifurcateAct = new QAction(trUtf8("&Bifurcate ..."), findAG);
   bifurcateAct->setShortcut(QString("c"));
   bifucentAct = new QAction(trUtf8("Bif&u. to center ..."), findAG);
   bifucentAct->setShortcut(QString("Ctrl+c"));
   connect(findAG, SIGNAL(triggered(QAction*)), this, SLOT(find(QAction*)));
   this->addActions(findAG->actions());

   QActionGroup *rayAG = new QActionGroup(this);
   greenAct = new QAction(trUtf8("E&quipotential ..."), rayAG);
   greenAct->setShortcut(QString("g"));
   rayAct = new QAction(trUtf8("&External ray ..."), rayAG);
   rayAct->setShortcut(QString("e"));
   rayPointAct = new QAction(trUtf8("Ra&y to point ..."), rayAG);
   rayPointAct->setShortcut(QString("Ctrl+x"));
   portraitAct = new QAction(trUtf8("&Orbit portrait ..."), rayAG);
   portraitAct->setShortcut(QString("o"));
   laminatAct = new QAction(trUtf8("&Lamination ..."), rayAG);
   laminatAct->setShortcut(QString("Ctrl+o"));
   QAction *rSeparator = new QAction(this);
   rSeparator->setSeparator(true); rayAG->addAction(rSeparator);
   wakeAct = new QAction(trUtf8("Angles of the &wake ..."), rayAG);
   wakeAct->setShortcut(QString("Ctrl+k"));
   kneadAct = new QAction(trUtf8("&Kneading ..."), rayAG);
   kneadAct->setShortcut(QString("k"));
   connect(rayAG, SIGNAL(triggered(QAction*)), this, SLOT(setRay(QAction*)));
   this->addActions(rayAG->actions());

   QActionGroup *pathAG = new QActionGroup(this);
   spider1Act = new QAction(QString("&$"), pathAG);
   spider1Act->setShortcut(QString("s"));
   spider2Act = new QAction(QString("&$"), pathAG);
   spider2Act->setShortcut(QString("Ctrl+s"));
   mate1Act = new QAction(QString("&$"), pathAG);
   mate1Act->setShortcut(QString("m"));
   mate2Act = new QAction(QString("&$"), pathAG);
   mate2Act->setShortcut(QString("Ctrl+m"));
   pathAct = new QAction(QString("&$"), pathAG);
   pathAct->setShortcut(QString("w"));
   captAct = new QAction(QString("&$"), pathAG);
   captAct->setShortcut(QString("Ctrl+w"));
   connect(pathAG, SIGNAL(triggered(QAction*)), this, SLOT(setPath(QAction*)));
   this->addActions(pathAG->actions());

   QActionGroup *chooseAG = new QActionGroup(this);
   chooseActs[0] = new QAction(trUtf8(
      "&0: a = \xce\xb3(1/6) = i , per 2 , 1/3 limb."), chooseAG);
   chooseActs[1] = new QAction(trUtf8(
      "&1: a = \xce\xb3(1/2) = -2, \xce\xb2 in 1/2 limb."), chooseAG);
   chooseActs[2] = new QAction(trUtf8(
      "&2: a = \xce\xb3(1/4) , \xce\xb2 in 1/3 limb."), chooseAG);
   chooseActs[3] = new QAction(trUtf8(
      "&3: a = \xce\xb3(1/16) , \xce\xb2 in 1/5 limb."), chooseAG);
   chooseActs[4] = new QAction(trUtf8(
      "&4: a = \xce\xb3(5/16) , \xce\xb2 in 2/5 limb."), chooseAG);
   chooseActs[5] = new QAction(trUtf8(
      "&5: a = \xce\xb3(5/12) , \xce\xb1 in 1/2 limb."), chooseAG);
   chooseActs[6] = new QAction(trUtf8(
      "&6: a = \xce\xb3(9/56) , \xce\xb1 in 1/3 limb."), chooseAG);
   chooseActs[7] = new QAction(trUtf8(
      "&7: a = \xce\xb3(23/112) , \xce\xb1 in 1/3 limb."), chooseAG);
   chooseActs[8] = new QAction(trUtf8(
      "&8: a = \xce\xb3(9/40) , primitive period 4."), chooseAG);
   chooseActs[9] = new QAction(trUtf8(
      "&9: a = the same,  c_n  on other branch."), chooseAG);
   connect(chooseAG, SIGNAL(triggered(QAction*)), this, SLOT(setAS(QAction*)));
   chooseMenu = new QMenu(this); chooseMenu->addActions(chooseAG->actions());

   QActionGroup *levelAG = new QActionGroup(this);
   levelaAct = new QAction(trUtf8("&a: Self-similarity around  a"), levelAG);
   level0Act = new QAction(trUtf8("&1: Similarity around  c_n , \xce\xb3 = 1"),
      levelAG);
   level1Act = new QAction(trUtf8(
      "&2: Similarity around  c_n , \xce\xb3 = 3/2"), levelAG);
   level2Act = new QAction(trUtf8(
      "&3: Similarity around  c_n , \xce\xb3 = 7/4"), levelAG);
   level3Act = new QAction(trUtf8(
      "&4: Similarity around  c_n , \xce\xb3 = 15/8"), levelAG);
   levelmAct = new QAction(trUtf8("&m: Small M-sets at  c_n , \xce\xb3 = 2"),
      levelAG);
   connect(levelAG, SIGNAL(triggered(QAction*)), this, SLOT(setAS(QAction*)));
   levelMenu = new QMenu(this); levelMenu->addActions(levelAG->actions());

   QActionGroup *ASAG = new QActionGroup(this);
   FeigenbaumAct = new QAction(trUtf8("&Feigenbaum scaling"), ASAG);
   FeigenbaumAct->setShortcut(QString("Shift+f"));
   QAction *sSeparator = new QAction(this);
   sSeparator->setSeparator(true); ASAG->addAction(sSeparator);
   customAct = new QAction(trUtf8("Find &point ..."), ASAG);
   customAct->setShortcut(QString("t"));
   chooseAct = new QAction(trUtf8("&Choose point"), ASAG);
   chooseAct->setShortcut(QString("Ctrl+t"));
   chooseAct->setMenu(chooseMenu);
   levelAct = new QAction(trUtf8("&Level of rescaling"), ASAG);
   levelAct->setShortcut(QString("l"));
   levelAct->setMenu(levelMenu);
   orderAct = new QAction(trUtf8("Set &order ..."), ASAG);
   orderAct->setShortcut(QString("Ctrl+l"));
   connect(ASAG, SIGNAL(triggered(QAction*)), this, SLOT(setAS(QAction*)));
   this->addActions(ASAG->actions());

   QActionGroup *locsimAG = new QActionGroup(this);
   locsimAct = new QAction(trUtf8("Local &similarity ..."), locsimAG);
   locsimAct->setShortcut(QString("p"));
   //locsim2Act = new QAction(trUtf8("Similarity, com&parison"), locsimAG);
   //locsim2Act->setShortcut(QString("Ctrl+p"));
   connect(locsimAG, SIGNAL(triggered(QAction*)), this,
      SLOT(setLS(QAction*)));
   this->addActions(locsimAG->actions());

   QActionGroup *ftypeAG = new QActionGroup(this);
   degreeAct = new QAction(QString("&$"), ftypeAG);
   degreeAct->setShortcut(QString("q"));
   QAction *fffSeparator = new QAction(this);
   fffSeparator->setSeparator(true); ftypeAG->addAction(fffSeparator);
   f0Act = new QAction(trUtf8("&0: Mandelbrot set z^2 + c"), ftypeAG);
   f0Act->setShortcut(QString("Ctrl+0"));
   f1Act = new QAction(trUtf8("&1: Multibrot set z^q + c [Q]"), ftypeAG);
   f1Act->setShortcut(QString("Ctrl+1"));
   f2Act = new QAction(trUtf8(
      "&2: Branner-Fagella cz(1 + z/q)^q [Q]"), ftypeAG);
   f2Act->setShortcut(QString("Ctrl+2"));

   QActionGroup *f3AG = new QActionGroup(this);
   f3Acts[1] = new QAction(
      trUtf8("&0: c(z^3 - 3z) , semi-conj. to 2.2 [Q]"), f3AG);
   f3Acts[4] = new QAction(
      trUtf8("&1: c(z^3 - 3z - 2) - 1 , superattracting fixed point"), f3AG);
   f3Acts[5] = new QAction(trUtf8(
      "&2: A(z^3 - 3z - 2) + c , superattracting 2-cycle"), f3AG);
   f3Acts[6] = new QAction(
      trUtf8("&3: A(z^3 - 3z - 2) + b , superattr. 3-cycle"), f3AG);
   f3Acts[2] = new QAction(
      trUtf8("&5: c(z^3 - 3z - 2) + 1 , cr.pt. -> cr.pt.: M_2 , M_4"), f3AG);
   f3Acts[3] = new QAction(
      trUtf8("&6: c(z^3 - 3z - 2) - 2 , cr. orbits meet"), f3AG);
   f3Acts[7] = new QAction(
      trUtf8("&7: A(z^3 - 3z - 2) + c , cr.pt. preperiodic"), f3AG);
   f3Acts[8] = new QAction(trUtf8(
      "&8: z^3 + cz^2 + \xce\xbb z Siegel disk [Q]"),
      f3AG);
   f3Acts[9] = new QAction(
      trUtf8("&9: 2-cycle of Siegel disks [Q]"), f3AG);
   connect(f3AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f3Menu = new QMenu(this); f3Menu->addActions(f3AG->actions());
   f3Act = new QAction(trUtf8("&3: Cubic polynomials"), ftypeAG);
   f3Act->setShortcut(QString("Ctrl+3"));
   f3Act->setMenu(f3Menu);

   QActionGroup *f4AG = new QActionGroup(this);
   f4Acts[0] = new QAction(
      trUtf8("&0: c(z^4 - 4z) , commutes w. rotation, s-c to 2.3 [Q]"), f4AG);
   f4Acts[1] = new QAction(
      trUtf8("&1: (z^2 + c)^2 , semi-conjugate to z^4 + c"), f4AG);
   f4Acts[2] = new QAction(
      trUtf8("&2: PQ and QP for antimating [Q, Ctrl+M]"), f4AG);
   f4Acts[3] = new QAction(
      trUtf8("&3: Cr.pts. -1 -> c -> 1 : M_2 , M_4 , M_8"), f4AG);
   f4Acts[4] = new QAction(
      trUtf8("&4: One degenerate cr.pt.: M_2 , M_3 [Q] Siegel"), f4AG);
   f4Acts[5] = new QAction(
      trUtf8("&5: Cr.pt. -> cr.pt. as 3.2 [Q] Siegel"), f4AG);
   f4Acts[6] = new QAction(
      trUtf8("&6: Cr.pt. pre-fixed, as 2.2 [Q] Siegel"), f4AG);
   f4Acts[7] = new QAction(
      trUtf8("&7: Superattracting fixed point, as 3.4 [Q] Siegel"), f4AG);
   f4Acts[8] = new QAction(
      trUtf8("&8: Superattracting 2-cycle, as 3.5 [Q] Siegel"), f4AG);
   f4Acts[9] = new QAction(
      trUtf8("&9: One cr.pt. -> indifferent f.p. [Q] Siegel"), f4AG);
   connect(f4AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f4Menu = new QMenu(this); f4Menu->addActions(f4AG->actions());
   f4Act = new QAction(trUtf8("&4: Quartic polynomials"), ftypeAG);
   f4Act->setShortcut(QString("Ctrl+4"));
   f4Act->setMenu(f4Menu);

   QActionGroup *f5AG = new QActionGroup(this);
   f5Acts[13] = new QAction(trUtf8("&m: Mating"), f5AG);
   f5Acts[0] = new QAction(trUtf8("&0: 1 + c/z^2, 0 \xe2\x86\x92 \xe2\x88\x9e [Q]"), f5AG);
   f5Acts[1] = new QAction(trUtf8(
      "&1: (z^2 + c)/(1 + cz^2), symmetric under inversion [Ctrl+Q]"), f5AG);
   f5Acts[2] = new QAction(
      trUtf8("&2: (z^2 + c)/(1 - z^2), \xe2\x88\x9e is 2-periodic"), f5AG);
   f5Acts[3] = new QAction(trUtf8("&3: (z^2 + A)/(c^2 - z^2), \xe2\x88\x9e is 3-periodic [Ctrl+Q]"), f5AG);
   f5Acts[14] = new QAction(trUtf8("&4: \xe2\x88\x9e is 4-periodic"), f5AG);
   f5Acts[15] = new QAction(trUtf8("&5: \xe2\x88\x9e is 5-periodic"), f5AG);
   f5Acts[6] = new QAction(
      trUtf8("&6: \xe2\x88\x9e has preperiod 1, period 1 [Ctrl+Q]"), f5AG);
   f5Acts[4] = new QAction(
      trUtf8("&7: \xe2\x88\x9e has preperiod 1, period 2 [Ctrl+Q]"), f5AG);
   f5Acts[5] = new QAction(trUtf8(
      "&?: \xe2\x88\x9e has preperiod 2, period 1 [Ctrl+Q]"), f5AG);
   f5Acts[7] = new QAction(
      trUtf8("&8: (z^2 - c)/(z^2 + c), critical orbits meet [Ctrl+Q]"), f5AG);
   f5Acts[8] = new QAction(
      trUtf8("&9: (z^2 - 1)/(z^2 + c), \xe2\x88\x9e \xe2\x86\x92 1 "
         "\xe2\x86\x92 0 [Ctrl+Q]"), f5AG);
   f5Acts[9] = new QAction(
      trUtf8("&s: Persistent Siegel disk [Q, Ctrl+Q]"), f5AG);
   f5Acts[10] = new QAction(
      trUtf8("&h: Cubic rational with Herman ring, antipode preserving [Q]"), f5AG);
   f5Acts[11] = new QAction(
      trUtf8("&r: c(z + 0.5/z^2), cubic with rotational symmetry"), f5AG);
   f5Acts[12] = new QAction(
      trUtf8("&p: z^2 + c/z^2, quartic, critical orbits meet"), f5AG);
   connect(f5AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f5Menu = new QMenu(this); f5Menu->addActions(f5AG->actions());
   f5Act = new QAction(trUtf8("&5: Rational maps"), ftypeAG);
   f5Act->setShortcut(QString("Ctrl+5"));
   f5Act->setMenu(f5Menu);

   QActionGroup *f6AG = new QActionGroup(this);
   f6Acts[0] = new QAction(trUtf8("&0: Newton for cubic polynomials"), f6AG);
   f6Acts[1] = new QAction(trUtf8(
      "&1: Quartic Newton, degenerate cr.pt."), f6AG);
   f6Acts[2] = new QAction(trUtf8("&2: Odd quartic Newton map"), f6AG);
   f6Acts[3] = new QAction(trUtf8(
      "&3: Quartic, cr.pt. -> cr.pt. as 3.2"), f6AG);
   f6Acts[4] = new QAction(
                   trUtf8("&4: Quartic, cr.pt. -> s.attr. fixed pt."), f6AG);
   f6Acts[5] = new QAction(
                   trUtf8("&5: Quartic, cr.pt. -> repelling fixed pt."), f6AG);
   f6Acts[6] = new QAction(trUtf8("&6: Quartic, critical orbits meet"), f6AG);
   f6Acts[7] = new QAction(trUtf8(
      "&7: Quartic, degenerate fixed cr.pt."), f6AG);
   f6Acts[8] = new QAction(trUtf8(
      "&8: Quartic, superattracting 2-cycle"), f6AG);
   f6Acts[9] = new QAction(
                   trUtf8("&9: Quartic, persistent Siegel cycle [Q]"), f6AG);
   f6Acts[10] = new QAction(
             trUtf8("&p: Parabolic at \xe2\x88\x9e for (z^2 - c^2)e^z"), f6AG);
   f6Acts[11] = new QAction(
             trUtf8("&q: Parabolic at \xe2\x88\x9e with two basins"), f6AG);
   f6Acts[12] = new QAction(
             trUtf8("&r: Parabolic at \xe2\x88\x9e with three basins"), f6AG);
   connect(f6AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f6Menu = new QMenu(this); f6Menu->addActions(f6AG->actions());
   f6Act = new QAction(trUtf8("&6: Newton maps"), ftypeAG);
   f6Act->setShortcut(QString("Ctrl+6"));
   f6Act->setMenu(f6Menu);

   QActionGroup *f7AG = new QActionGroup(this);
   f7Acts[1] = new QAction(trUtf8("&1: c exp(z)"), f7AG);
   f7Acts[2] = new QAction(trUtf8("&2: cz exp(z) (Fagella, Kremer)"), f7AG);
   f7Acts[3] = new QAction(
      trUtf8("&3: (az - c)exp(z) + c , a = \xce\xbb + c [Q] Siegel"), f7AG);
   f7Acts[4] = new QAction(trUtf8("&4: c sin(z)"), f7AG);
   f7Acts[5] = new QAction(trUtf8("&5: c cos(z)"), f7AG);
   f7Acts[6] = new QAction(trUtf8("&6: \xcf\x80 cos(z) + c"), f7AG);
   f7Acts[7] = new QAction(trUtf8(
      "&7: c[cos(z) + 1] (M_2 , M_4 as 3.2)"), f7AG);
   f7Acts[8] = new QAction(
      trUtf8("&8: c[cos(z) - 1] , superattracting fixed point"), f7AG);
   f7Acts[9] = new QAction(trUtf8(
      "&9: a cos(z) + c , cr.pt. 2-periodic"), f7AG);
   f7Acts[0] = new QAction(
      trUtf8("&0: \xce\xbb sin(z) + c(1 - cos(z)) [Q] Siegel"), f7AG);
   connect(f7AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f7Menu = new QMenu(this); f7Menu->addActions(f7AG->actions());
   f7Act = new QAction(trUtf8("&7: Transcendental"), ftypeAG);
   f7Act->setShortcut(QString("Ctrl+7"));
   f7Act->setMenu(f7Menu);

   QActionGroup *f8AG = new QActionGroup(this);
   f8Acts[1] = new QAction(trUtf8("&1: Branner-Douady surgery"), f8AG);
   f8Acts[2] = new QAction(trUtf8("&2: Modified Branner-Douady"), f8AG);
   f8Acts[3] = new QAction(trUtf8("&3: Inverse Branner-Douady"), f8AG);
   f8Acts[4] = new QAction(trUtf8("&4: +z^3 - 3az + b , real cubic"), f8AG);
   f8Acts[5] = new QAction(trUtf8("&5: -z^3 + 3az + b , real cubic"), f8AG);
   f8Acts[6] = new QAction(trUtf8("&6: Tricorn [Q]"), f8AG);
   f8Acts[7] = new QAction(trUtf8("&7: Henon map"), f8AG);
   f8Acts[8] = new QAction(trUtf8(
       "&8: Bousch-Thurston IFS, n \xe2\x89\xa4 32  [Q]"), f8AG);
   f8Acts[9] = new QAction(trUtf8(
       "&9: Entropy \xce\xbb(\xce\xb8) = e^h = 2^B  [N]"), f8AG);
   connect(f8AG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   f8Menu = new QMenu(this); f8Menu->addActions(f8AG->actions());
   f8Act = new QAction(trUtf8("&8: Non-analytic maps"), ftypeAG);
   f8Act->setShortcut(QString("Ctrl+8"));
   f8Act->setMenu(f8Menu);

   connect(ftypeAG, SIGNAL(triggered(QAction*)), this, SLOT(setF(QAction*)));
   this->addActions(ftypeAG->actions());

   QActionGroup *helpAG = new QActionGroup(this);
   helpAct = new QAction(trUtf8("&Help ..."), helpAG);
   helpAct->setShortcut(Qt::Key_F1);
   aboutAct = new QAction(trUtf8("&About ..."), helpAG);
   QAction *hSeparator = new QAction(this);
   hSeparator->setSeparator(true); helpAG->addAction(hSeparator);
   demoActs[10] = new QAction(trUtf8("C. and Z., a &fairy tale ..."), helpAG);
   demoActs[0] = new QAction(trUtf8("Demo &0: real iteration ..."), helpAG);
   demoActs[0]->setShortcut(QString("Alt+0"));
   demoActs[1] = new QAction(trUtf8("Demo &1: dynamics and sets ..."), helpAG);
   demoActs[1]->setShortcut(QString("Alt+1"));
   demoActs[2] = new QAction(
                     trUtf8("Demo &2: points and bifurcations ..."), helpAG);
   demoActs[2]->setShortcut(QString("Alt+2"));
   demoActs[3] = new QAction(trUtf8("Demo &3: external rays ..."), helpAG);
   demoActs[3]->setShortcut(QString("Alt+3"));
   demoActs[4] = new QAction(trUtf8("Demo &4: combinatorics ..."), helpAG);
   demoActs[4]->setShortcut(QString("Alt+4"));
   demoActs[5] = new QAction(trUtf8("Demo &5: renormalization ..."), helpAG);
   demoActs[5]->setShortcut(QString("Alt+5"));
   demoActs[6] = new QAction(
                     trUtf8("Demo &6: asymptotic similarity ..."), helpAG);
   demoActs[6]->setShortcut(QString("Alt+6"));
   demoActs[7] = new QAction(trUtf8("Demo &7: local similarity ..."), helpAG);
   demoActs[7]->setShortcut(QString("Alt+7"));
   demoActs[8] = new QAction(trUtf8("Demo &8: qc surgery ..."), helpAG);
   demoActs[8]->setShortcut(QString("Alt+8"));
   demoActs[9] = new QAction(trUtf8("Demo &9: new families ..."), helpAG);
   demoActs[9]->setShortcut(QString("Alt+9"));
   connect(helpAG, SIGNAL(triggered(QAction*)), this, SLOT(help(QAction*)));
   this->addAction(helpAct);

   fileMenu = menuBar()->addMenu(trUtf8("Fil&e"));
   fileMenu->addActions(winAG->actions());
   fileMenu->addSeparator();
   fileMenu->addActions(fileAG->actions());
   fileMenu->addSeparator();
   fileMenu->addAction(exitAct);

   drawMenu = menuBar()->addMenu(trUtf8("&Draw"));
   drawMenu->addActions(drawAG->actions());
   drawMenu->addSeparator();
   drawMenu->addActions(drawingAG->actions());

   pointMenu = menuBar()->addMenu(trUtf8("&Points"));
   pointMenu->addActions(movingAG->actions());
   pointMenu->addSeparator();
   pointMenu->addActions(mapAG->actions());
   pointMenu->addSeparator();
   pointMenu->addActions(findAG->actions());

   rayMenu = menuBar()->addMenu(trUtf8("&Rays"));
   rayMenu->addActions(rayAG->actions());
   
   pathMenu = menuBar()->addMenu(trUtf8("Pa&th"));
   pathMenu->addActions(pathAG->actions());

   scaleMenu = menuBar()->addMenu(trUtf8("&Scaling"));
   scaleMenu->addActions(ASAG->actions());
   scaleMenu->addSeparator();
   scaleMenu->addActions(locsimAG->actions());

   newMenu = menuBar()->addMenu(trUtf8("&Function"));
   newMenu->addActions(ftypeAG->actions());

   helpMenu = menuBar()->addMenu(trUtf8("&Help"));
   helpMenu->addActions(helpAG->actions());
}//createActions

void QmnShell::updateActions()
{  int ft = ftype % 10; QString fname = trUtf8("Mandelbrot set");
   if (ft == 1) fname = trUtf8("Multibrot set");
   if (ft == 2) fname = trUtf8("Branner-Fagella");
   if (ft == 3) fname = trUtf8("Cubic polynomials");
   if (ft == 4) fname = trUtf8("Quartic polynomials");
   if (ft == 5) fname = trUtf8("Rational maps");
   if (ft == 6) fname = trUtf8("Newton maps");
   if (ft == 7) fname = trUtf8("Transcendental");
   if (ft == 8) fname = trUtf8("Non-analytic maps");
   if (signtype > 0)
        setWindowTitle(trUtf8("Mandel: %1, parameter plane").arg(fname));
   else setWindowTitle(trUtf8("Mandel: %1, dynamic plane").arg(fname));
   if (signtype > 0) paraDynAct->setText(trUtf8("&To dynamics"));
   else paraDynAct->setText(trUtf8("&To parameters"));
   if (signtype < 0) keepRegionAct->setText(trUtf8("To para., &keep region"));
   else keepRegionAct->setText( (updateRegion ?
      trUtf8("Keep &Julia region") : trUtf8("Reset &Julia region") ) );
   if (dplane->getType() < 0) sphereAct->setText(trUtf8("Sphere to pl&ane"));
   else sphereAct->setText(trUtf8("Pl&ane to sphere"));
   sphereAct->setEnabled(ft != 7 && ftype != 38 && ftype != 48 && ftype != 58);
   bool sphere = signtype < 0 && dplane->getType() < 0;
   colorAct->setEnabled(*themode != 3 && !sphere);
   epsMenuAct->setEnabled(*themode != 3 && !sphere);
   epsSaveAct->setEnabled(*themode != 3 && !sphere);
   videoAct->setEnabled(signtype > 0);
   limbAct2->setEnabled(!ftype || ftype == 35);
   veinAct2->setEnabled(!ftype || ftype == 35 || (ftype == 45 && signtype == 6));
   limbAct3->setEnabled(!ftype || ftype == 15 || ftype == 25 || ftype == 35);
   veinAct3->setEnabled(!ftype || ftype == 15 || ftype == 25 || ftype == 35 ||
                        (ftype == 45 && signtype == 6));
   limbAct4->setEnabled(ftype == 15 || ftype == 35);
   vidMenuAct->setEnabled(signtype > 0);
   zoominAct->setEnabled(!sphere);
   zoomoutAct->setEnabled(!sphere);
   updateBack();
   if (signtype > 0) algoActs[0]->setText(trUtf8("&0: Period (hyp. comp.)"));
   else algoActs[0]->setText(trUtf8("&0: Attracting basins"));
   for (int i = 0; i <= 11; i++)
      algoActs[i]->setEnabled(i >= f->minMode(signtype)
         && i <= f->maxMode(signtype));
   algoAct->setEnabled(f->minMode(signtype) < f->maxMode(signtype));
   //nmaxAct->setEnabled(ftype != 58 || signtype < 0); //????????????????
   if (ftype == 5 || ftype == 15 || ftype == 45 || ftype == 105) 
      for (int i = 1; i <= 3; i++) algoActs[i]->setEnabled(false);
   renormAct->setEnabled(f->minMode(signtype) < 2 && ftype != 38 && !sphere);
   entroAct->setEnabled(!ftype && signtype > 0);
   rturnAct->setEnabled(sphere);
   lturnAct->setEnabled(sphere);
   uturnAct->setEnabled(sphere);
   dturnAct->setEnabled(sphere);
   orbitAct->setEnabled(signtype < 0 && gamma >= 0.0
      && ftype != 48 && ftype != 58);
   fAct->setEnabled(ftype != 48);
   inv2Act->setEnabled(!ftype || (ftype == 28 && signtype*signtype == 4)
      || ((ftype % 10) == 5 && ftype < 115) || ftype == 48 || ftype == 58);
   inv1Act->setEnabled(inv2Act->isEnabled() || ftype == 38);
   pointAct->setEnabled(ftype <= 1 ||
      (signtype > 0 && (ftype == 25 || ftype == 35)) );
   mdouble x = 0, y = 0;
   bifurcateAct->setEnabled(signtype > 0 && f->bifurcate(-1.0, x, y));
   bifucentAct->setEnabled(signtype > 0 && !ftype);
   greenAct->setEnabled(ftype <= 1 && !sphere);
   rayAct->setEnabled(!ftype || (ftype == 1 && !sphere) || ftype == 24
      || (ftype == 28 && (signtype == 2 || signtype == -2) )
      || (signtype < 0 && (ftype == 95 || ftype == 25 || ftype == 35) ) );
   rayPointAct->setEnabled(!ftype || ftype == 58);
   portraitAct->setEnabled((!ftype && gamma >= 0.0) || ftype == 58);
   laminatAct->setEnabled((!ftype && gamma >= 0.0) || ftype == 58);
   wakeAct->setEnabled((signtype > 0 && !ftype) || ftype == 58);
   kneadAct->setEnabled((signtype > 0 && !ftype) || ftype == 58);
   spider1Act->setText(trUtf8("&Mating by angle ..."));
   spider2Act->setText(trUtf8("&Anti-mating by angle ..."));
   mate1Act->setText(trUtf8("Mating &by parameter ..."));
   mate2Act->setText(trUtf8("Anti-mating by &parameter ..."));
   pathAct->setText(trUtf8("(P-)recapt&ure ..."));
   captAct->setText(trUtf8("&Capture ..."));
   if (ftype == 95)
   {  spider2Act->setText(trUtf8("Upper &angle ..."));
      mate2Act->setText(trUtf8("Upper &parameter ..."));
   }
   if (!ftype)
   {  spider1Act->setText(trUtf8("&Spider with path ..."));
      spider2Act->setText(trUtf8("S&pider with legs ..."));
      captAct->setText(trUtf8("Ex&amples ..."));
   }
   sphere = !ftype || ftype == 25 || ftype == 35;
   pathAct->setEnabled(signtype > 0 && sphere);
   captAct->setEnabled(signtype > 0 && sphere);
   sphere = sphere || ftype == 5 || ftype == 15 || ftype == 95;
   spider1Act->setEnabled(signtype > 0 && sphere && ftype != 5);
   spider2Act->setEnabled(signtype > 0 && sphere
      && ftype != 15 && ftype != 35);
   mate1Act->setEnabled(signtype > 0 && sphere && ftype != 5 && ftype);
   mate2Act->setEnabled(signtype > 0 && sphere
      && ftype != 15 && ftype != 35 && ftype);
   if (ftype == 24 && signtype > 0) mate2Act->setEnabled(true);
   rayMenu->setEnabled(rayAct->isEnabled());
   pathMenu->setEnabled(spider1Act->isEnabled() || mate2Act->isEnabled() );
   scaleMenu->setEnabled(!ftype);
   FeigenbaumAct->setEnabled(signtype > 0 && !ftype && gamma == 0.0);
   customAct->setEnabled(signtype > 0 && !ftype);
   chooseAct->setEnabled(signtype > 0 && !ftype);
   orderAct->setEnabled(signtype > 0 && gamma > 0.0);
   levelAct->setEnabled(signtype > 0 && gamma > 0.0 && gamma < 4.0);
   locsimAct->setEnabled(signtype > 0 && !ftype);
   degreeAct->setEnabled(ftype == 1 || ftype == 2 || ftype == 28);
   if (ftype == 4 && signtype < 2000 && signtype > -2000)
      degreeAct->setEnabled(true);
   if (ftype) degreeAct->setText(trUtf8("&Change degree ..."));
   else
   {  degreeAct->setText(trUtf8("&Order of q_n ..."));
      degreeAct->setEnabled(*themode >= 9 && *themode <= 11);
   }
   if (ftype == 3)
   {  degreeAct->setEnabled(true);
      if (signtype == 1 || signtype == -1)
      degreeAct->setText(trUtf8("&Change coefficients ..."));
      else degreeAct->setText(trUtf8("&To c(z^3 - 3z) + b ..."));
   }
   if (ftype == 24 && signtype > 0)
   {  degreeAct->setEnabled(true);
      degreeAct->setText(trUtf8("&Toggle  PQ  and  QP"));
   }
   if (ftype == 5 && signtype >= -5 && signtype <= 5)
   {  degreeAct->setEnabled(true);
      if (signtype == 2 || signtype == -2)
      degreeAct->setText(trUtf8("&Change coefficients ..."));
      else degreeAct->setText(trUtf8("&To (z^2 + c)/(z^2 + b) ..."));
   }
   if (ftype == 48 && signtype > 0)
   {  degreeAct->setEnabled(true);
      degreeAct->setText(trUtf8("&Change coefficients ..."));
   }
   if (ftype > 100)
   {  degreeAct->setEnabled(true);
      degreeAct->setText(trUtf8("&Change multiplier ..."));
   }
////////////////////////////////////////////// not written yet ///////////////
   vidAct3->setEnabled(false);
   if (ftype == 1) algoActs[4]->setEnabled(false);
   if (ftype == 1) algoActs[5]->setEnabled(false);
   if (ftype == 18 && signtype > 0) algoActs[1]->setEnabled(false); //not gray
   if (!ftype)
   {  spider1Act->setEnabled(false); spider2Act->setEnabled(false);
      pathAct->setEnabled(false); captAct->setEnabled(false);
   }
   if (ftype % 10 == 5) pathMenu->setEnabled(false);
   if (ftype == 25 && signtype > 0) pathMenu->setEnabled(true);
   if (ftype == 25 && signtype > 0) spider2Act->setEnabled(false);
   if ((ftype == 5 || ftype == 15 || ftype == 35) && signtype > 0)
   {  pathMenu->setEnabled(true);
      spider1Act->setEnabled(true);
      spider2Act->setEnabled(ftype == 15);
      mate1Act->setEnabled(false);
      mate2Act->setEnabled(false);
      pathAct->setEnabled(false);
      captAct->setEnabled(false);
   }
   slowmateAct->setEnabled(signtype > 0 &&
      (ftype == 5 || ftype == 15 || ftype == 25 || ftype == 35));
   slowantiAct->setEnabled(signtype > 0 && (ftype == 5 || ftype == 25));
   f5Acts[13]->setEnabled(signtype > 0);
   f5Acts[5]->setEnabled(false);
   f6Acts[3]->setEnabled(false);
   f6Acts[5]->setEnabled(false);
   f6Acts[9]->setEnabled(false);
   demoActs[0]->setEnabled(false);
   demoActs[9]->setEnabled(false);
}//updateActions

void QmnShell::contextMenuEvent(QContextMenuEvent *event)
{  if (!QRect(theplane->mapToGlobal(QPoint(0, 0)),
              theplane->size()).contains(event->globalPos()))
   { event->ignore(); return; }
   QMenu *contextMenu = new QMenu(this); event->accept();
   if (theplane->hasPathLength()) drawPathAct->setText(trUtf8("Path &off"));
   else drawPathAct->setText(trUtf8("&Set path"));
   if (theplane->framed())
   {  killframeAct->setText(trUtf8("Cancel &frame"));
      contextMenu->addAction(zoominAct);
      contextMenu->addAction(renormAct);
      contextMenu->addSeparator();
      contextMenu->addAction(epsSaveAct);
      contextMenu->addAction(pngSaveAct);
      contextMenu->addSeparator();
      contextMenu->addAction(killframeAct); //not killAct: ESC closes ctxMenu
   }
   else
   {  killframeAct->setText(trUtf8("Cancel &path"));
      if (theplane->hasPathLength() > 0)
      {  killframeAct->setEnabled(true);
         markAct->setText(trUtf8("Mar&k path"));
      }
      else
      {  killframeAct->setEnabled(false);
         markAct->setText(trUtf8("Mar&k point"));
      }
      contextMenu->addAction(zoominAct);
      contextMenu->addAction(paraDynAct);
      if (signtype < 0) contextMenu->addAction(keepRegionAct);
      if (ftype <= 1)
      {  contextMenu->addAction(greenAct);
         contextMenu->addSeparator();
         contextMenu->addAction(pointAct);
         contextMenu->addAction(drawPathAct);
         contextMenu->addAction(markAct);
         contextMenu->addAction(killframeAct);
         if (signtype > 0 && !ftype)
         {  contextMenu->addSeparator();
            contextMenu->addAction(locsimAct);
         }
      }
      else
      {  contextMenu->addAction(drawPathAct);
         contextMenu->addAction(markAct);
         contextMenu->addAction(killframeAct);
      }
      if (signtype < 0)
      {  contextMenu->addSeparator();
         contextMenu->addAction(fAct);
         contextMenu->addAction(inv1Act);
         contextMenu->addAction(inv2Act);
      }
   }
   contextMenu->exec(event->globalPos());
   markAct->setText(trUtf8("Mar&k"));
   killframeAct->setEnabled(true); delete contextMenu;
}//contextMenuEvent

void QmnShell::updateBack()
{ backAct->setEnabled(theplane->isBackEnabled()); }

void QmnShell::pMoved()
{  mdouble x, y; dplane->stop(); pplane->getPoint(x, y);
   uint p = f->period(x, y); if (y*y < 1.0e-26) y = 0.0;
   if (p == 1 && !ftype && x < -0.75) p = 10000; //c = -2
   if (p == 2 && !ftype && x >= -0.75) p = 1; //c ~ -0.75
   if (p == 2 && !ftype && (y > 0.25 || y < -0.25)) p = 10000; //c = i, -i
   if (ftype == 48 || ftype == 58) p = 0u;
   if (ftype == 58) { uint n; pplane->getNmax(n); y *= 100.0/n; }
   pcoords->setText(trUtf8("c = %1  %2%3 i    period = %4").arg(
      QString::number(x, 'f', 15)).arg(
      (y < 0.0 ? QString("") : QString("+"))).arg(
      QString::number(y, 'f', 15)).arg(QString::number(p)));
   f->setParameter(x, y);
   if (updateRegion)
   {  if (lsppp && !dplane->getType())
      {  uint preper = uint(lsppp >> 16), per = uint(lsppp & 0xFFF);
         mdouble t0, t1, t2, t3; mdouble t[4]; pplane->getPlane(t0, t1, t2, t3);
         t[0] = t0; t[1] = t1; t[2] = t2; t[3] = t3;
         if (f->similarity(preper, per, x, y, t))
            dplane->setPlane(0.0, 0.0, 2.0, 0.0);
         else dplane->setPlane(t[0], t[1], t[2], t[3]);
         dplane->setPoint(x, y); pplane->getNmax(per); dplane->setNmax(per);
      }
      else
      {  f->startPlane(-1, x, y); dplane->setPlane(x, 0.0, y, 0.0);
         if (ftype == 58) dplane->setPlane(0.5, 0.5, 0.5, 0.0);
         f->iterate(x, y, 1); dplane->setPoint(x, y);
         dplane->setNmax(ftype == 48 ? 12 : 100);
      }
   }
   if (pplane->isRunning() || drawLater == 1)
   { drawLater = 1; dplane->draw(f, 0, themode); }
   else dplane->draw(f, -signtype, &dmode);
}

void QmnShell::dMoved()
{  mdouble x, y; dplane->getPoint(x, y); if (y*y < 1.0e-26) y = 0.0;
   dcoords->setText(QString("z = %1  %2%3 i").arg(
      QString::number(x, 'f', 15)).arg(
      (y < 0.0 ? QString("") : QString("+"))).arg(
      QString::number(y, 'f', 15)));
   //if (gamma == -2.0 && signtype < 0 && dplane->hasPathLength() > 0)
      //setRay(redrawAct);
}

void QmnShell::pFinished()
{  if (gamma == -1.0 || gamma == -2.0 || signtype < 0 || pplane->isRunning())
      return;
   if (ftype == 58) pplane->drawLambda();
   //if () { mdouble x, y; pplane->newtonRay(1, 9ULL, 56ULL, x, y); }
   if (!drawLater) return;
   drawLater = 0; dplane->draw(f, -signtype, &dmode);
}

void QmnShell::dFinished()
{  mdouble x, y;
   /*if (!ftype)
   {  pplane->getPoint(x, y); mndynamics::root(0.25 - x, -y, x, y);
      dplane->setNmax(16u); dplane->stop(); dplane->recur(f, -0.5-x, -y, 0u);
   }//*/
   if (ftype == 48)
   {  uint n; dplane->stop();
      dplane->getNmax(n); if (n > 20u) dplane->setNmax(20u);
      f->iterate(x, y, 1); dplane->recur(f, x, y, 0u);
      f->iterate(x, y, 2); dplane->recur(f, x, y, 0u);
      dplane->update(); return;
   }
   //if () { f->getParameter(x, y); dplane->backRay(4ULL, 7ULL, x, y); } else
   if (!drawLater && !dplane->isRunning() && !lsppp
      && gamma >= 0.0 && ftype != 58 && ftype != 95)
   {  f->iterate(x, y, 1); dplane->drawOrbit(f, x, y, 0, 4000);
      if (ftype > 100 || ftype == 18)
      { f->iterate(x, y, 2); dplane->drawOrbit(f, x, y, 0, 4000); }
      if (ftype == 116 && signtype > 1)
      { f->iterate(x, y, 3); dplane->drawOrbit(f, x, y, 0, 4000); }
      if (ftype == 116 && signtype == 3)
      { f->iterate(x, y, 4); dplane->drawOrbit(f, x, y, 0, 4000); }
   }
   //if (gamma == -1.0) dplane->drawPathSegment(path);
}

void QmnShell::dCompleted() //signal is more reliable than  finished()  ???!!!
{  if (imgno >= 10000 && gamma == -3.0)
   {  mdouble x, y; QString name = QString("%1.png").arg(imgno - 9000);
      setWindowTitle(name);
      //pplane->getPoint(x, y); dplane->setPoint(x, y); dplane->move(8);//jumps
      if (imgno > 10000) dplane->savePNG(name);
      if (imgno >= 11000) { imgno -= 10000; return; } //see killAct
      //imgno++; int fr = imgno % 25; if (!fr) fr = 25;
      //mpath->sequence((24 - 10000 + imgno)/25, fr, 25, f->pathInfo);
      //mpath->step(x, y); pplane->setPoint(x, y);
      imgno++; int n = (49-10000+imgno)/25, fr = imgno % 25; if (!fr) fr = 25;
      mpath->sequence(n, fr, 25, f->pathInfo);
      x = f->pathInfo->rc[n]; y = f->pathInfo->ic[n]; f->pathInfo->n --;
      pplane->setPoint(x, y);
   }
   if (imgno >= 10000 && gamma >= 0.0L && lsppp)
   {  QString name = QString("%1.png").arg(imgno - 9000);
      setWindowTitle(name);
      if (imgno > 10000) pplane->savePNG(name, dplane);
      if (imgno >= 11000) { imgno = 0; return; }
      imgno++; pplane->draw(f, signtype, themode, 249); pMoved();
   }
}

void QmnShell::paraDyn()
{ resizeWin(paraDynAct); }

void QmnShell::resizeWin(QAction *act)
{  if (act == paraDynAct)
   {  signtype = -signtype;
      dcoords->setEnabled(signtype < 0); pcoords->setEnabled(signtype > 0);
      dplane->setActive(signtype < 0); pplane->setActive(signtype > 0);
      if (signtype < 0) //p->d
      {  psizeChanged = false; updateRegion = true;
         themode = &dmode; theplane = dplane;
         if (drawLater && gamma >= 0.0)
         { drawLater = 0; pplane->stop(); draw(redrawAct); }
      }
      else //d->p
      {  drawLater = 0; theplane = pplane; themode = &pmode;
         updateBack(); dplane->stop();
         if (psizeChanged) draw(redrawAct);
      }
      updateActions();
   }
   if (act == keepRegionAct)
   {  if (signtype < 0) { updateRegion = false; resizeWin(paraDynAct); }
      else { updateRegion = !updateRegion; updateActions(); }
   }
   if (act == sphereAct)
   {  updateRegion = true; pplane->stop();
      if (dplane->getType() < 0)
      {  dplane->setType(0); mdouble x, y; f->startPlane(-1, x, y);
         dplane->setPlane(x, 0, y, 0); dplane->draw(f, -1, &dmode);
      }
      else { dplane->setType(-1); dplane->draw(f, -1, &dmode); }
      updateActions();
   }
   if (act != sizeAct) return;
   uint n = uint(thesize), n1 = 240u, n2 = 8000u, size0 = n;
   mdouble x = 0.0L, y = 0.0L, u = 0.0L, v = 0.0L;
   if (theplane->getType() >= 0) theplane->getPlane(x, y, u, v);
   QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
      "The center is (%3, %4) and\n"
      "the half width is (%5, %6).\n"
      "Adjust the image size in pixels (%1 ... %2) :").arg(
          QString::number(n1)).arg(QString::number(n2)).arg(
          QString::number(x, 'f', 15)).arg(QString::number(y, 'f', 15)).
          arg(QString::number(u, 'f', 15)).arg(QString::number(v, 'f', 15)),
       &n, &n1, n2, 0, this, 40);
   connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
   if (!dialog->exec() || n == size0) return;
   while (n & 7) n--;
   thesize = int(n); size0 = uint(QApplication::desktop()->height() - 100);
   if (n <= size0)
   {  pplane->resize(thesize, thesize);
      pscroll->setFixedSize(thesize, thesize);
      dplane->resize(thesize, thesize);
      dscroll->setFixedSize(thesize, thesize);
      pscroll->setMinimumSize(thesize, thesize);
      dscroll->setMinimumSize(thesize, thesize);
      setMaximumWidth(2*thesize + hskip); setMaximumHeight(thesize + vskip);
   }
   else
   {  pplane->resize(thesize, thesize);
      pscroll->setMaximumSize(thesize, thesize);
      dplane->resize(thesize, thesize);
      dscroll->setMaximumSize(thesize, thesize);
      int size = 480;
      pscroll->setMinimumSize(size, size);
      dscroll->setMinimumSize(size, size);
      setMaximumWidth(2*size0 + hskip); setMaximumHeight(size0 + vskip);
   }
   if (signtype > 0)
   {  drawLater = 1; pMoved(); draw(redrawAct);
      //if (gamma == -1.0) { drawLater = 0; dplane->drawPathSegment(path); }
   }
   else
   { psizeChanged = true; pplane->draw(f, 0, themode); draw(redrawAct); }
   //why no blackening before path? why in pplane (new QImage is what?)
   adjustSize(); updateGeometry(); // ?
}

void QmnShell::draw(QAction *act)
{  mdouble x, y; int mode = 0; //for drawing
   if (act == stepAct)
   {  if (theplane->framed()) { draw(zoominAct); return; }
      if (signtype > 0 && gamma > 0.0) setAS(stepAct);
      if (signtype > 0 && gamma < 0.0) setPath(stepAct);
      return;
   }
   if (act == homeAct)
   {  if (gamma < 0.0) setPath(0);
      if (signtype < 0 && dplane->getType() < 0)
      { dplane->setPlane(0, 0, 0, 0); draw(redrawAct); return; }
      if (!updateRegion && signtype > 0) updateRegion = true;
      dplane->setNmax(ftype == 48 ? 12 : 100); f->startPlane(signtype, x, y);
      if (ftype == 8 && signtype == 3)
         theplane->setPlane(-.113, .888, .343, 0);
      else theplane->setPlane(x, 0, y, 0);
      if (ftype == 58)
      {  if (signtype > 0) pplane->setPlane(0.5, 1.5, 0.5, 0);
         else dplane->setPlane(0.5, 0.5, 0.5, 0.0);
      }
      if (signtype > 0)
      {  lsppp = 0; gamma = 0.0; updateActions();
         pplane->setNmax(ftype == 48 ? 24 : 100); drawLater = 1;
         f->getParameter(x, y); pplane->setPoint(x, y);
      }
   }
   if (act == redrawAct && theplane->framed()) theplane->move(3);
   if (act == zoominAct && !theplane->framed()) mode = 210;
   if (act == backAct) mode = -1;
   if (act == zoomoutAct)
   {  mdouble u, v; theplane->getPlane(x, y, u, v);
      if (u*u + v*v < 1e6) theplane->setPlane(x, y, 2*u, 2*v);
      else return;
   }
   if (signtype > 0) dplane->stop(); else { drawLater = 0; pplane->stop(); }
   theplane->draw(f, signtype, themode, mode); //below unnec. when mode changed
   if (lsppp && signtype > 0 && updateRegion && !dplane->getType()) pMoved();
}

void QmnShell::drawing(QAction *act)
{  /*if (act == nmaxAct)
   {  uint dd = 1u, mm = 1u; QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the maximal number of iterations (1 ... 65000) :"),
          &dd, &mm, 65000u, 1, this, 10);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || dd > 100U) return;
      const int M = 1000; const int N = 1000;
      mdouble centerx = -1.0, centery = 0.0,
         radx = 0.625, rady = 0.330718913883073824, u, v, w, X, Y;
      int d = dd, m, n; mdouble nearx, neary, XX, YY;
      mdouble *x = new mdouble[M], *y = new mdouble[M];
      for (m = 0; m < M; m++)
      {  neary = m*(d*2.0*PI/M); nearx = cos(neary); neary = sin(neary);
         x[m] = centerx + radx*nearx - rady*neary;
         y[m] = centery + radx*neary + rady*nearx;
      }
      XX = x[0]; YY = y[0];
      for (n = 1; n <= N; n++)
      {  for (m = M - 1; m >= 0; m--)
         {  nearx = x[(m+1) % M]; neary = y[(m+1) % M]; X = x[m]; Y = y[m];
            w = 2.0*(X*X + Y*Y);
            if (w < 1.0E-30) { n = 2*N; break; }
            u = 0.5 + X/w; v = -Y/w;
            mndynamics::root(X, Y, X, Y); w = X*u - Y*v; Y = X*v + Y*u; X = w;
            if (X*X + Y*Y < 1.0e-30) {n = 2*N; break; }
            if (nearx*X + neary*Y < 0.0) { X = -X; Y = -Y; }
            x[m] = X; y[m] = Y; dplane->drawLine(nearx, neary, X, Y);
         }
         //dplane->drawLine(x[0], y[0], XX, YY); XX = x[0]; YY = y[0];
      }
      return;
   }//twist 2222c*/
   if (act == nmaxAct)
   {  uint now, n, m = 1u; theplane->getNmax(n); now = n;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the maximal number of iterations (1 ... 65000) :"),
          &n, &m, 65000u, 1, this, 10);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != now)
      {  theplane->setNmax(n); if (theplane->framed()) theplane->move(3);
         if (ftype == 58 && signtype > 0)
         {  mdouble t1, t2, t3, t4, y; pplane->getPlane(t1, t2, t3, t4);
            pplane->getPoint(t4, y); t2 -= y;
            y *= ((mdouble)(n))/((mdouble)(now)); pplane->setPoint(t4, y);
            pplane->setPlane(t1, t2 + y, t3, 0);
         }//*/
         draw(redrawAct);
      }
   }
   if (act == algoAct) { algoMenu->exec(QCursor::pos()); return; }
   for (uint i = 0; i <= 11; i++) if (act == algoActs[i] && i != *themode)
   {  *themode = i; if (theplane->framed()) theplane->move(3);
      draw(redrawAct); updateActions();
   }
   if (act == degreeAct) //forwarded from setF()
   {  uint n = uint(signtype > 0 ? signtype : -signtype), m = 2;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the order  n  of  q_n(c) := f_c^n(0)  (2 ... 1024) :"),
          &n, &m, 1024u, 1, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && int(n) != signtype)
      {  signtype = int(n); f->setDegree(signtype);
         if (dplane->isActive()) signtype = -signtype;
         if (theplane->framed()) theplane->move(3); draw(redrawAct);
      }
   }
   if (act == renormAct)
   {  uint n1, n2 = 0;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Enter the renormalization period \xe2\x89\xa4 255.\n"
         "For embedded Julia sets, enter the preperiod\n"
         "and the period, separated with a comma:"),
         &n1, &n2, 65002u, 1, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n1 <= 255 && n2 <= 255) n1 |= (n2 << 8);
      else return;
      if (signtype > 0) { drawLater = 0; dplane->stop(); }
      else pplane->stop();
      theplane->draw(f, signtype, &n1, 2);
   }
   if (act == entroAct)
   {  uint n = pmode, m = 1u; int k; mdouble x = 0.0, y = 0.0, lambda;
      pmode = 18; draw(redrawAct); pmode = n; n = 0u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Mark the boundary according to the core entropy:\n"
	 "Blue is low, magenta is medium, red is high.\n"
	 "Afterwards, you may get a coordinate text-file\n"
	 "with Shift+h or a 3d-chart with Ctrl+h.\n"
	 "Now set the preperiod of the dyadic angles used.\n"
	 "Caution: the user interface will be frozen for a\n"
	 "few seconds with preperiod 15, a few minutes\n"
	 "with 20, and some hours with preperiod 25."),
          &n, &m, 30u, 1, this, 1);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || !n) return;
      qulonglong M, N = 1ULL; N <<= n;
      //adapted
      qulonglong M1 = 1ULL, M2 = (N >> 1); int i;
      if (n > 10)
      {  for (M1 = 1ULL; M1 < 1024ULL; M1++)
         {  x=10.0; pplane->newtonRay(1, M1, 1024ULL, x, y, 5, Qt::darkMagenta, 10);
            if (!pplane->pointToPos(x, y, i, k)) break;
         }
         for (M2 = 1023ULL; M2 > M1; M2--)
         {  x=10.0; pplane->newtonRay(1, M2, 1024ULL, x, y, 5, Qt::darkMagenta, 10);
            if (!pplane->pointToPos(x, y, i, k)) break;
         }
         M1 = ((M1 - 1ULL) << (n - 10)) + 1ULL;
         M2 = ((M2 + 1ULL) << (n - 10)) - 1ULL;
         if (M2 <= M1) return;
      }
      for (M = M1; M < M2; M++)
      {  lambda = mndAngle::lambda(M, N, 1.0e-6, 500);
         if (lambda > 2.0) continue;
         k = (int)(367.0*log(lambda));
         pplane->newtonRay(1, M, N, x, y, 5, Qt::darkMagenta, 10);
         pplane->drawLine(x, y, x, y, QColor(k, 0, 255 - k));
         pplane->drawLine(x, -y, x, -y, QColor(k, 0, 255 - k));
      }//adapted*/
    /*/vein
    int j; qulonglong K, L, D = (1ULL << 35); mdouble X, Y;
    for (j = 4; j <= n; j++)
    { N = 1ULL; N <<= j; N--; L = (N*4498065ULL)/16777215ULL;
      for (M = L; M > N/4; M--)
      {  L = M; K = N; mndAngle::conjugate(L, K);
	 if (K < N || L > K/4) continue;
	 lambda = mndAngle::lambda((M*D)/N, D, 1.0e-6, 500);
         if (lambda > 2.0) continue;
         k = (int)(367.0*log(lambda));
         pplane->newtonRay(1, M, N, x, y, 5, Qt::white, 10);
         pplane->newtonRay(1, M*N + L, N*(N + 1ULL), X, Y, 5, Qt::white, 10);
         pplane->drawLine(x, y, X, Y, QColor(k, 0, 255 - k));
      }
    }//vein*/
    /*/graph
    int j; qulonglong K, L, D = (1ULL << 35); mdouble X, Y; mndCombi C;
    pplane->setPlane(-0.5, 0, 0.6, 0); pplane->draw(f, 0, 0);
    pplane->drawLine(-1.0, 0.0, 0.0, 0.0);
    for (j = 4; j <= n; j++)
    { N = 1ULL; N <<= j; N--; L = (N*4498065ULL)/16777215ULL; Y = (mdouble)(N);
      for (M = L; M > N/4; M--)
      {  L = M; K = N; mndAngle::conjugate(L, K);
	 if (K < N || L > K/4) continue;
	 lambda = mndAngle::lambda((M*D)/N, D, 1.0e-6, 500);
         if (lambda > 2.0) continue; y = log(lambda);
	 C.fromAngle(M, N); C.getKneading(K); L = K ^ 1ULL; X = (mdouble)(L);
	 x = X/Y - 0.75; X = (((mdouble)(K))*Y + X)/(Y*(Y + 1.0)) - 0.75;
         pplane->drawLine(-4.0*x, y, -4.0*X, y);
      }
    }//graph*/
   }
}

void QmnShell::drawRequested()
{ draw(zoominAct); } //works as redraw on sphere

void QmnShell::move(QAction *act)
{  if (act == killAct && imgno > 10000) { imgno -=10000; return; }
   if (act == killAct || act == killframeAct) theplane->move(5);
   if (act == drawPathAct)
   {  if (theplane->hasPathLength()) theplane->move(10);
      else theplane->move(9);
   }
   if (act == rturnAct) dplane->move(11, 30, 0);
   if (act == lturnAct) dplane->move(11, 330, 0);
   if (act == uturnAct) dplane->move(11, 0, 30);
   if (act == dturnAct) dplane->move(11, 0, 330);
   if (act == rightAct) theplane->move(0, 1, 0);
   if (act == leftAct) theplane->move(0,-1, 0);
   if (act == upAct) theplane->move(0, 0, 1);
   if (act == downAct) theplane->move(0, 0, -1);
   if (act == plusAct) theplane->move(1, 1, 1);
   if (act == minusAct) theplane->move(1, -1, -1);
   if (act == greaterAct) theplane->move(1, 0, 1);
   if (act == lessAct) theplane->move(1, 0, -1);
   if (act == coordsAct)
   {  mdouble x, y, s = 1.0; theplane->getPoint(x, y);
      if (ftype == 58 && signtype > 0)
      { uint n; pplane->getNmax(n); s = 100.0/n; y *= s; }
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "Adjust the real part and the imaginary part of %1:").arg(
         (signtype > 0 ? QString("c") : QString("z"))), &x, &y, 2, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec()) theplane->setPoint(x, y/s);
   }
   if (act == markAct) theplane->move(8);
   if (act == tiltAct) theplane->tilt(1);
   if (act == textAct) setFile(textAct);
}

void QmnShell::map(QAction *act)
{  mdouble x, y, X, Y; dplane->getPoint(x, y);
   if (act == fAct) f->iterate(x, y);
   //{mdouble x0=x, y0=y; f->iterate(x, y); dplane->drawLine(x0, y0, x, y);}
   if (act == inv1Act)
   {  f->iterate(x, y, -1); dplane->Move(12, f);
      dplane->getPoint(X, Y); if (x*X + y*Y < 0.0) { x = -x; y = -y; }
   }
   if (act == inv2Act)
   {  f->iterate(x, y, -2); dplane->Move(13, f);
      dplane->getPoint(X, Y); if (x*X + y*Y < 0.0) { x = -x; y = -y; }
   }
   if (act == orbitAct) dplane->drawOrbit(f, x, y, 0, 4000);
   dplane->setPoint(x, y);
}

void QmnShell::find(QAction *act)
{  mdouble x, y, a, b; theplane->getPoint(x, y);
   pplane->getPoint(a, b); f->setParameter(a, b);
   uint per = f->period(a, b);
   if (act == pointAct && (ftype % 10 == 5))
   {  x = 0.0; y = 0.0;
      if (ftype == 35)
      {  x = 1.324717957244746;
         if (a < 0)
         { x = -0.662358978622373; y = 0.562279512062301; if (b < 0) y = -y; }
      }
       pplane->setPoint(x, y); return;
   }
   if (act == pointAct)
   {  uint n1 = 0, n2 = 0; if (signtype > 0 && per <= 1024) n2 = per;
      QmnUIntDialog *dialog;
      if (signtype > 0) dialog = new QmnUIntDialog(trUtf8(
         "Find the center of a hyperbolic component\n"
         "corresponding to an attracting period p:\n"
         "f_c^p (0) = 0 . Enter p (1...1024) . Or\n"
         "find a Misiurewicz point with preperiod k and period p:\n"
         "f_c^{k+p}(c) = f_c^k(c) .\n"
         "Enter k and p (separated with a comma, 1...512) :"),
         &n1, &n2, 65002u, 4, this);
      else dialog = new QmnUIntDialog(trUtf8(
         "Find a p-periodic point:  f_c^p (z) = z .\n"
         "Enter p (1...1024) . Or\n"
         "find a preperiodic point with preperiod k and period p.\n"
         "Enter k and p (separated with a comma, 1...512) :"),
         &n1, &n2, 65002u, 2, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && (n1 ? (n1 <= 512 && n2 <= 512) : n2 <= 1024)
         && !f->find(signtype, n1, n2, x, y)) theplane->setPoint(x, y);
   }
   if (act == bifurcateAct)
   {  per = f->bifurcate(-1.0, x, y); if (!per) return;
      mdouble t = .61803398874989484820L;
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "Go to a point of bifurcation from period %1. Enter a\n"
         "decimal number t or a quotient t = k/r with "
         "0 \xe2\x89\xa4 t < 1\n"
         "for the multiplier \xce\xbb = e^{i 2\xcf\x80 t} . The default\n"
         "value is the Golden Mean t = (\xe2\x88\x9a\x35-1)/2 :" //&radic;5
         ).arg(QString::number(per)), &t, 0, 2, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && t >= 0 && t < 1.0)
      { f->bifurcate(t, x, y); theplane->setPoint(x, y); }
   }
   if (act == bifucentAct)
   {  if (!per || per > 512) return;
      uint n1 = 1, n2 = 2;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Go to a center by a satellite bifurcation\n"
         "from period %1. Enter a quotient t = k/r\n"
         "for the multiplier \xce\xbb = e^{i 2\xcf\x80 t} , in\n"
         "lowest terms, with 1 \xe2\x89\xa4 k < r \xe2\x89\xa4 %2 :"
         ).arg(QString::number(per)).arg(QString::number(1024/per)),
         &n1, &n2, 65001u, 2, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || !n1 || n1 >= n2 || n2*per > 1024) return;
      for (uint j = 2; j <= n1; j++) if (!(n1 % j) && !(n2 % j)) return;
      if (per <= 3) f->bifurcate(mdouble(n1)/mdouble(n2), x, y);
      else
      {  f->find(signtype, 0, per, x, y);
         mdouble fx = x, fy = y, zx = 1, zy = 0, px = 0, py = 0, u;
         for (uint i = 1; i < per; i++)
         {  u = 2*(fx*zx - fy*zy); zy = 2*(fx*zy + fy*zx); zx = u;
            u = 2*(fx*px - fy*py) + 1; py = 2*(fx*py + fy*px); px = u;
            u = fx*fx - fy*fy + x; fy = 2*fx*fy + y; fx = u;
         }
         px += zx; py += zy; fx = 2*(zx*px - zy*py); fy = 2*(zx*py + zy*px);
         u = fx*fx + fy*fy;  //f = dlambda/dc
         if (u < 1e-10) return;
         zx = cos(2*PI*mdouble(n1)/mdouble(n2));
         zy = sin(2*PI*mdouble(n1)/mdouble(n2));
         x += (zx*fx + zy*fy)/u; y += (zy*fx - zx*fy)/u;
      }
      per *= n2; f->find(signtype, 0, per, x, y);
      if (f->period(x, y) == per) theplane->setPoint(x, y);
   }
} //find

void QmnShell::setRay(QAction *act)
{  QString enterAngleString = trUtf8(
   "<b></b>Enter the angle &theta; "
   "with 0 &le; &theta; &lt; 1. Notation :<br>"
   "\"1/7\" or \"p001\" for 1/7 = "
   ".<nobr style=\"text-decoration:overline\">001</nobr> "
   "(periodic angles),<br>"
   "\"9/56\" or \"001p010\" for 9/56 = "
   ".001<nobr style=\"text-decoration:overline\">010</nobr> "
   "(preperiodic angles),<br>"
   "\"1/4\" or \"01\" for 1/4 = .01 (dyadic angles).");
   if (act == greenAct)
   {  mdouble x, y; theplane->getPoint(x, y); y = f->green(signtype, x, y);
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "To draw an equipotential line, enter the\n"
         "potential  log|\xce\xa6(%1)| > 0 :\n"
         "The suggested value is the potential of the\n"
         "current point. Just hit Return to accept it."
         ).arg((signtype > 0 ? QChar('c') : QChar('z'))), &y, 0, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || y <= 0 || y > 10.0) return;
      if (signtype > 0) { drawLater = 0; dplane->stop(); }
      else pplane->stop();
      theplane->green(f, signtype, y, 10);
   }
   /*if (act == rayAct && ftype == 24 && signtype > 0 && dplane->getType() < 0)
   {  int k = 6, l; qulonglong N1, N2 = 3ULL << 2*k;
      mdouble x = 0.0L, y = 0.0L, x0, y0;
      for (N1 = 1ULL; N1 <= N2/6ULL; N1 += 2ULL)
      {  x0 = x; y0 = y;
         if (f->mandelRay(1, -150, N1, N2, &x, &y) < 0
             || f->find(1, k, 1, x, y) || x*x + y*y < 0.54L)
         { x = x0; y = y0; continue; }
         if (N1 == 1ULL) continue; pplane->drawLine(x0, y0, x, y, Qt::red);
      }
      return;
   }//*/
   /*if (act == rayAct && ftype == 1 && signtype == 4 && dplane->getType() < 0)
   {  int k = 6, l; qulonglong N1, N2 = 3ULL << (2*k - 1);
      mdouble x = 0.0L, y = 0.0L, x0, y0;
      for (N1 = 1ULL; N1 <= N2/6ULL; N1 += 2ULL)
      {  x0 = x; y0 = y;
         if (f->mandelRay(1, -150, N1, N2, &x, &y) < 0
             || f->find(1, k, 1, x, y) || x*x + y*y < 0.22L)
         { x = x0; y = y0; continue; }
         if (N1 == 1ULL) continue; pplane->drawLine(x0, y0, x, y, Qt::red);
      }
      return;
   }//*/
   if (act == rayAct)
   {  uint method, q = 0; mdouble x, y; f->getParameter(x, y);
      qulonglong N1 = 1LL, N2 = 2ULL;
      if (signtype < 0) { N1 = 0ULL; if (dplane->getType() < 0) N2 = 1ULL; }
      if (ftype == 1) { N1 = 2ULL; N2 = 2ULL; }
      if (ftype == 28) { N1 = 1ULL; N2 = 1ULL; }
      QmnRayDialog *dialog = new QmnRayDialog(enterAngleString, trUtf8(
         "Adjust the quality, 2...10. It is the number of intermediate\n"
         "points, or it concerns the distance to the starting point."),
         &N1, &N2, &method, &q, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return; if (ftype == 28) method = 5;
      if (ftype == 24 || (ftype == 1 && signtype*signtype == 16)) method = 1;
      if (!method && dplane->backRay(N1, N2, x, y, q, Qt::darkMagenta, 1))
         method = 1;
      if (method & 1)
         theplane->mandelRay(signtype, f, N1, N2, x, y); //quality = 6
      if (method & 2)
      {  if (signtype > 0) { drawLater = 0; dplane->stop(); }
         else pplane->stop();
         theplane->traceRay(signtype, mdouble(N1)/mdouble(N2), f, x, y, q);
      }
   }
   if (act == rayPointAct)
   {  mdouble x, y; f->getParameter(x, y);
      qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialog = new QmnCombiDialog(enterAngleString,
         &N1, &N2, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      int k, p = mndAngle::normalize(N1, N2, k), q = 0; qulonglong n1 = N1;
      if (!p)
      {  QmnUIntDialog *dialog1 = new QmnUIntDialog(trUtf8(
            "The angle  %1/%2\n"
            "has  preperiod + period > 64.").arg(N1).arg(N2),
            0, 0, 0u, 3, this, 0);
         connect(dialog1,SIGNAL(needHelp(int)), helptabs,SLOT(showPage(int)));
         dialog1->exec(); return;
      }
      QString bin; QmnCombiDialog::numbersToBinary(N1, N2, bin); mdouble l;
      if (p == 1) l = mndAngle::lambda(N1, N2, 1.0e-10, 1000);
      else
      {  qulonglong L = 1ULL; L <<= 60;
         l = ((mdouble)(L))*((mdouble)(N1))/((mdouble)(N2));
         l = -mndAngle::lambda(((qulonglong)(l)), L, 1.0e-10, 1000);
      }
      QString text = trUtf8(
         "The angle  %1/%2  or  %3\n"
         "has  preperiod = %4  and  period = %5.\n"
         ).arg(N1).arg(N2).arg(bin).arg(k).arg(p);
      if (l > 0.0 && signtype > 0) text += trUtf8(
        "Entropy: e^h = 2^B = \xce\xbb = %1\n").arg(
           QString::number(l, 'f', 8));
      if (k && signtype < 0) text += trUtf8(
         "The corresponding dynamic ray lands\n"
         "at a preperiodic point of preperiod %1 and\n"
         "period dividing %2.\n"
         "Do you want to draw the ray and to shift z\n"
         "to the landing point?").arg(k).arg(p);
      if (k && signtype > 0) text += trUtf8(
         "The corresponding parameter ray lands\n"
         "at a Misiurewicz point of preperiod %1 and\n"
         "period dividing %2.\n"
         "Do you want to draw the ray and to shift c\n"
         "to the landing point?").arg(k).arg(p);
      if (!k && signtype < 0) text += trUtf8(
         "The dynamic ray lands at a repelling\n"
         "or parabolic point of period dividing %1.\n"
         "Do you want to draw the ray and to shift\n"
         "z to the landing point?").arg(p);
      if (!k && signtype > 0)
      {  q = mndAngle::conjugate(n1, N2);
         QmnCombiDialog::numbersToBinary(n1, N2, bin); text += QString(trUtf8(
         "The conjugate angle is  %1/%2  or  %3 .\n"
         )).arg(n1).arg(N2).arg(bin);
         if (q < p) bin = trUtf8("satellite"); else bin = trUtf8("primitive");
         QString t1, t2; mndCombi c; c.fromAngle(N1, N2); qulonglong b;
         c.getKneading(b); QmnCombiDialog::codeToKneading(b, t1);
         c.getAddress(b); QmnCombiDialog::codeToAddress(b, t2);
         text += trUtf8(
            "The kneading sequence is  %1  and\n"
            "the internal address is  %2 .\n").arg(t1).arg(t2);
         text += trUtf8(
            "The corresponding parameter rays land\n"
            "at the root of a %1 component of period %2.\n").arg(bin).arg(p);
         if (q && q < p) text += trUtf8(
            "It bifurcates from period %1.\n").arg(q);
         text += trUtf8(
            "Do you want to draw the rays and to shift c\n"
            "to the corresponding center?");
      }
      QmnUIntDialog *dialog1 = new QmnUIntDialog(text, 0, 0, 0u, 3, this);
      connect(dialog1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog1->exec()) return;
      if (ftype == 58)
      {  x = ((mdouble)(N1))/((mdouble)(N2)); if (l < 0.0) l = -l;
         uint n; pplane->getNmax(n);
         pplane->setPoint(x, l*0.01*n); return;
      }
      if (q) pplane->mandelRay(signtype, f, n1, N2, x, y);
      theplane->mandelRay(signtype, f, N1, N2, x, y);
      if (!f->find(signtype, k, p, x, y)) theplane->setPoint(x, y);
   }
   if (act == portraitAct)
   {  mdouble x, y; f->getParameter(x, y);
      qulonglong n1, N1 = 0ULL, N2;
      QmnCombiDialog *dialog = new QmnCombiDialog(trUtf8(
         "Enter the characteristic angle \xce\xb8 with 0 < \xce\xb8 < 1,\n"
         "2 \xe2\x89\xa4 period \xe2\x89\xa4 64. "
         "Notation : \"2/7\" or \"p010\" ."),
         &N1, &N2, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      int k, p = mndAngle::normalize(N1, N2, k); if (k || p <= 1) return;
      n1 = N1; int q = mndAngle::conjugate(n1, N2);
      if (signtype < 0)
      {  for (k = 0; k < p; k++)
         { dplane->backRay(N1, N2, x, y); mndAngle::twice(N1, N2); }
         if (q == p) for (k = 0; k < p; k++)
         { dplane->backRay(n1, N2, x, y); mndAngle::twice(n1, N2); }
         return;
      }
      bool sphere = (dplane->getType() < 0); if (sphere) dplane->setType(0);
      dplane->setPlane(0, 0, 1.2, 0); dplane->draw(f, 0, themode);
      dplane->drawEllipse(0, 0, 1.0, 1.0, Qt::green); x = mdouble(N2);
      for (k = 0; k < p; k++)
      {  dplane->drawOrtho(mdouble(N1)/x, mdouble(n1)/x);
         mndAngle::twice(N1, N2); mndAngle::twice(n1, N2);
      }
      updateRegion = true; drawLater = 0; dplane->setPoint(0, 0);
      if (sphere) dplane->setType(-1); else dplane->setPlane(0, 0, 2.0, 0);
   }
   if (act == laminatAct)
   {  mdouble x, y, u, v; dplane->getPoint(x, y);
      qulonglong N = 0ULL, N1 = 0ULL, N2;
      QmnCombiDialog *dialog = new QmnCombiDialog(enterAngleString,
         &N1, &N2, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || !N1) return;
      uint n; int k, p = mndAngle::normalize(N1, N2, k); u = (mdouble)(N2);
      if (!k && p)
      { N = N1; mndAngle::conjugate(N, N2); v = ((mdouble)(N))/u; }
      u = ((mdouble)(N1))/u; dplane->setPoint(0.5*u, 0.0);
      if (N && (N1 & 1)) dplane->setPoint(0.5*v, 0.0);
      //problem 1/7 avoided, why? new problem 41/127, need rational angles?
      pplane->getNmax(n); if (n > 20u) n = 12u;
      if (signtype < 0) //no crash when n + k + p ~ 64
      {  dplane->setPoint(x, y); f->getParameter(x, y); qulonglong K, D = 1ULL;
	 for (k = 0; k <= n; k++)
         {  for (K = 0ULL; K < D; K++)
	    {  dplane->backRay(N1 + K*N2, D*N2, x, y);
	       if (N) dplane->backRay(N + K*N2, D*N2, x, y);
	    }
	    D <<= 1;
	 }
         return;
      }
      bool sphere = (dplane->getType() < 0); if (sphere) dplane->setType(0);
      dplane->setPlane(0, 0, 1.2, 0); dplane->draw(f, 0, themode);
      dplane->drawEllipse(0, 0, 1.0, 1.0, Qt::green);
      if (N)
      {  dplane->drawOrtho(u, v);
	 dplane->drawLamination(0.5*u, 0.5*v + 0.5, n);
	 dplane->drawLamination(0.5*v, 0.5*u + 0.5, n);
      }
      else if (N2 == 56ULL && (N1 == 9ULL || N1 == 11ULL || N1 == 15ULL))
      {  dplane->drawOrtho(9.0/56.0, 11.0/56.0);
         dplane->drawOrtho(11.0/56.0, 15.0/56.0);
         dplane->drawOrtho(9.0/56.0, 15.0/56.0);
	 dplane->drawLamination(9.0/112.0, 11.0/112.0, n);
	 dplane->drawLamination(11.0/112.0, 15.0/112.0, n);
	 dplane->drawLamination(15.0/112.0, 65.0/112.0, n);
	 dplane->drawLamination(65.0/112.0, 67.0/112.0, n);
	 dplane->drawLamination(67.0/112.0, 71.0/112.0, n);
	 dplane->drawLamination(71.0/112.0, 9.0/112.0, n);
      }
      else dplane->drawLamination(0.5*u, 0.5*u + 0.5, n);
      updateRegion = true; drawLater = 0; dplane->setPoint(0, 0);
      if (sphere) dplane->setType(-1); else dplane->setPlane(0, 0, 2.0, 0);
   }
   if (act == wakeAct)
   {  uint k = 1, r = 2; qulonglong n, d; mdouble x, y;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Determine the wake of a limb at the main cardioid.\n"
         "Enter a fraction  k/r  for the rotation number,\n"
         "in lowest terms, with  1 \xe2\x89\xa4 k < r \xe2\x89\xa4 64 :"),
         &k, &r, 65001u, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      d = mndAngle::wake(((int) (k)), ((int) (r)), n);
      if (!d) return;
      QString b1, b2; QmnCombiDialog::numbersToBinary(n, d, b1);
      QmnCombiDialog::numbersToBinary(n + 1LL, d, b2);
      QmnUIntDialog *dialog1 = new QmnUIntDialog(trUtf8(
         "The %1/%2-wake of the main cardioid is\n"
         "bounded by the parameter rays with the angles\n"
         "%3/%4  or  %5  and\n"
         "%6/%4  or  %7 .\n"
         "Do you want to draw the rays and to shift c\n"
         "to the center of the satellite component?").arg(k).arg(r).arg(
         n).arg(d).arg(b1).arg(n + 1LL).arg(b2), 0, 0, 0u, 3, this);
      connect(dialog1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog1->exec()) return;
      if (ftype == 58)
      {  uint nn; pplane->getNmax(nn);
         pplane->setPoint(((mdouble)(n))/((mdouble)(d)), 0.01*nn); return;
      }
      pplane->newtonRay(1, n, d, x, y, 5, Qt::darkMagenta, 1); n++;
      if (pplane->newtonRay(1, n, d, x, y, 5, Qt::darkMagenta, 2) <= 1
          && !f->find(1, 0, r, x, y)) pplane->setPoint(x, y);
   }
   if (act == kneadAct)
   {  qulonglong N1 = 1LL, N2, n1, n2, d;
      QmnCombiDialog *dialog = new QmnCombiDialog(trUtf8(
         "Enter a *-periodic kneading sequence,\n"
         "e.g, \"ABAAB*\" or \"10110*\".\n"
         "Or enter an internal address,\n"
         "e.g., \"1-2-4-5-6\".\n"
         "(The periods \xe2\x89\xa4 64 are increasing.)"), &N1, &N2, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      int p, q; QString t1, t2, text; mndCombi c;
      if (!N2) { p = c.setKneading(N1); c.getAddress(N2); }
      else { p = c.setAddress(N2); c.getKneading(N1); }
      QmnCombiDialog::codeToKneading(N1, t1);
      QmnCombiDialog::codeToAddress(N2, t2);
      text = trUtf8(
         "The kneading sequence  %1  corresponds\n"
         "to the internal address  %2 .\n"
         "The period is %3.\n").arg(t1).arg(t2).arg(p);
      q = c.renorm();
      if (q <= 1) text += trUtf8(
         "It is not simply renormalizable.\n");
      else text += trUtf8(
         "It is simply renormalizable with lowest period %1.\n").arg(q);
      q = c.failsBS();
      if (q) text += trUtf8(
         "This combinatorics is not realized by quadratic polynomials,\n"
         "since it fails the Bruin-Schleicher admissibility condition:\n"
         "the abstract Hubbard tree has an evil branch point of period %1."
         ).arg(q);
      else
      {  q = c.count();
         if (q == 1)
         {  text += trUtf8(
            "This combinatorics is realized once on the real axis.\n");
            t1 = trUtf8("external");
         }
         else
         {  text += trUtf8(
            "This combinatorics is realized for %1 complex parameters.\n"
            ).arg(q);
            t1 = trUtf8("smallest");
         }
         q = c.toAngles(n1, n2, d);
         if (q) text += QString("Angles not computed: Error %1").arg(q);
         else text += trUtf8(
         "The %4 angles are %1/%3 and %2/%3 .\n"
         "Do you want to draw the rays and to shift c\n"
         "to the corresponding center?").arg(n1).arg(n2).arg(d).arg(t1);
      }
      QmnUIntDialog *dialog1
         = new QmnUIntDialog(text, 0, 0, 0u, 3, this, (q ? 0 : 1) );
      connect(dialog1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog1->exec() || q) return;
      if (ftype == 58)
      {  mdouble l; qulonglong L = 1ULL; L <<= 60;
         l = ((mdouble)(L))*((mdouble)(n1))/((mdouble)(d));
         l = mndAngle::lambda(((qulonglong)(l)), L, 1.0e-12, 500);
         uint n; pplane->getNmax(n);
         pplane->setPoint(((mdouble)(n1))/((mdouble)(d)), l*0.01*n); return;
      }
      q = 0; mdouble x, y; pplane->newtonRay(1, n1, d, x, y, 5, Qt::darkMagenta, 1);
      if (pplane->newtonRay(1, n2, d, x, y, 5, Qt::darkMagenta, 2) <= 1
          && !f->find(1, 0, p, x, y)) pplane->setPoint(x, y);
      while (1)
      {  q++; N2 -= 1LL << (p - 1); p = c.setAddress(N2); if (p <= 1) break;
         c.toAngles(n1, n2, d);
         pplane->newtonRay(1, n1, d, x, y, 5,
            (q & 1 ? Qt::yellow : Qt::darkMagenta), 1);
         pplane->newtonRay(1, n2, d, x, y, 5,
            (q & 1 ? Qt::yellow : Qt::darkMagenta), 1);
      }
      pplane->newtonRay(1, 0ULL, 1ULL, x, y, 5,
         (q & 1 ? Qt::yellow : Qt::darkMagenta), 1);
   }
} //setRay
   
void QmnShell::setPath(QAction *act)
{  if (act == 0 && gamma < 0.0)
   {  gamma = 0.0; imgno = 0; //dplane->move(10);
      pplane->setCursorPix(0); dplane->setCursorPix(0);
      if (ftype) f->setDegree(4);
      updateActions(); pMoved(); //twice when called from homeAct?
   }
   QString enterAngleString = trUtf8(
   "<b></b>Enter the angle &theta; "
   "with 0 &le; &theta; &lt; 1. Notation :<br>"
   "\"1/7\" or \"p001\" for 1/7 = "
   ".<nobr style=\"text-decoration:overline\">001</nobr> "
   "(periodic angles),<br>"
   "\"9/56\" or \"001p010\" for 9/56 = "
   ".001<nobr style=\"text-decoration:overline\">010</nobr> "
   "(preperiodic angles),<br>"
   "\"1/4\" or \"01\" for 1/4 = .01 (dyadic angles).");
   QString pullBackStringHtml = trUtf8(
   "<br>Then hit Return repeatedly to perform the iteration<br>"
   "of the Thurston Algorithm. A connecting path between<br>"
   "point configurations is used instead of spider legs.<br>"
   "Use Home or enter 0 to quit.");
   QString pullBackString = trUtf8(
   "\nThen hit Return repeatedly to perform the iteration\n"
   "of the Thurston Algorithm. A connecting path between\n"
   "point configurations is used instead of spider legs.\n"
   "Use Home to quit.");
   /*if (act == stepAct && gamma == -1.0)
   {  ldouble x, y; path->pathStep(x, y);
      dplane->setPoint(x, y); pplane->setPoint(x, y);
      f->setParameter(x, y); dplane->drawPathSegment(path);
   }//*/
   if (act == stepAct && gamma == -3.0 && imgno < 10000)
   {  mdouble x, y; imgno += 25; int fr = imgno % 25; if (!fr) fr = 25;
      mpath->sequence((imgno + 24)/25, fr, 25, f->pathInfo);
      mpath->step(x, y); pplane->setPoint(x, y);     
   }
   if (act == pathAct)
   {  QmnUIntDialog *dialogxx = new QmnUIntDialog(trUtf8(
      "There is only one built-in example. Other \n"
      "examples can be made available by re-compiling.\n"
      "Interactive features will be available in Mandel 5.14.\n"
      ) + pullBackString, 0, 0, 0u, 4, this);
      connect(dialogxx, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogxx->exec()) return;
      pplane->stop(); drawLater = 0; lsppp = 0; dplane->move(10);
      updateRegion = true; dplane->draw(f, 0, themode);
      pplane->setCursorPix(spiderPix); dplane->setCursorPix(spiderPix);
      gamma = -3.0; imgno = 25; int k, p;
      /*/precapture12
      k = 1; p = 2; mdouble t; f->pathInfo->n = 150;
      delete[] f->pathInfo->coeff; f->pathInfo->coeff = 0;
      delete[] f->pathInfo->rc; delete[] f->pathInfo->ic;
      f->pathInfo->rc = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->ic = new mdouble[1 + f->pathInfo->n];
      for (int j = 0; j <= f->pathInfo->n; j++)
      {  t = M_PI*sqrt(j/150.0); f->pathInfo->rc[j] = -0.80901699437495*(1.0 - cos(t));
         f->pathInfo->ic[j] = -0.80901699437495*sin(t);
      }//*/
      //precapture14
      k = 1; p = 4; mdouble t; f->pathInfo->n = 150;
      delete[] f->pathInfo->coeff; f->pathInfo->coeff = 0;
      delete[] f->pathInfo->rc; delete[] f->pathInfo->ic;
      f->pathInfo->rc = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->ic = new mdouble[1 + f->pathInfo->n];
      for (int j = 0; j <= f->pathInfo->n; j++)
      {  t = M_PI*sqrt(j/150.0); f->pathInfo->rc[j]
            = -0.434231667927833*(1.0 - cos(t)) + 0.0714001124957945*sin(t);
         f->pathInfo->ic[j]
            = -0.0714001124957945*(1.0 - cos(t)) - 0.434231667927833*sin(t);
      }//*/
      /*/regluing
      mdouble t, rc, ic, rz, iz; f->pathInfo->n = 150;
      k = 1; p = 2; rc = -1.0; ic = 0.0; rz = -1.839286755214161; iz = 0.0;
      //k = 1; p = 4; rc = -1.0; ic = 0.0; rz = -0.739225539751518; iz = -0.324792174541382;
      //k = 1; p = 0; rc = -0.725420495660914; ic = -0.240290714066185; rz = -1.0; iz = 0.0;
      //k = 1; p = 0; rc = -2.0; ic = 0.0; rz = -1.0; iz = 0.0;
      delete[] f->pathInfo->coeff; f->pathInfo->coeff = 0;
      delete[] f->pathInfo->rc; delete[] f->pathInfo->ic;
      f->pathInfo->rc = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->ic = new mdouble[1 + f->pathInfo->n];
      for (int j = 0; j <= f->pathInfo->n; j++)
      {  t = j/150.0; f->pathInfo->rc[j] = (1.0 - t)*rc + t*rz;
         f->pathInfo->ic[j] = (1.0 - t)*ic + t*iz;
      }//*/
      /*/dehn
      k = 0; p = 3; f->pathInfo->n = 25;
      delete[] f->pathInfo->coeff; f->pathInfo->coeff = 0;
      delete[] f->pathInfo->rc; delete[] f->pathInfo->ic;
      f->pathInfo->rc = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->ic = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->rc[0] = -0.5; f->pathInfo->ic[0] = -0.866025403784438647;
      f->pathInfo->rc[1] = -0.4257; f->pathInfo->ic[1] = -0.8374;
      f->pathInfo->rc[2] = -0.3532; f->pathInfo->ic[2] = -0.7939;
      f->pathInfo->rc[3] = -0.2554; f->pathInfo->ic[3] = -0.7504;
      f->pathInfo->rc[4] = -0.1575; f->pathInfo->ic[4] = -0.7033;
      f->pathInfo->rc[5] = -0.037060726586655;
         f->pathInfo->ic[5] = -0.646452300365239;
      f->pathInfo->rc[6] = 0.0781; f->pathInfo->ic[6] = -0.7142;
      f->pathInfo->rc[7] = 0.1687; f->pathInfo->ic[7] = -0.7468;
      f->pathInfo->rc[8] = 0.2630; f->pathInfo->ic[8] = -0.7867;
      f->pathInfo->rc[9] = 0.3391; f->pathInfo->ic[9] = -0.8084;
      f->pathInfo->rc[10]= 0.4116; f->pathInfo->ic[10] = -0.8338;
      f->pathInfo->rc[11]= 0.4406; f->pathInfo->ic[11] = -0.9172;
      f->pathInfo->rc[12]= 0.5457; f->pathInfo->ic[12] = -0.9172;
      f->pathInfo->rc[13]= 0.5675; f->pathInfo->ic[13] = -0.8338;
      f->pathInfo->rc[14]= 0.4950; f->pathInfo->ic[14] = -0.7867;
      for (int j = 15; j <= 25; j++)
      {  f->pathInfo->rc[j] = f->pathInfo->rc[25 - j];
         f->pathInfo->ic[j] = f->pathInfo->ic[25 - j];
      }//*/
      mpath->capture(k, p, f->pathInfo); f->setDegree(1);
      updateActions(); //pMoved();
      imgno = 0; setPath(stepAct);
   }
   if (act == captAct)
   {  QmnUIntDialog *dialogx = new QmnUIntDialog(trUtf8(
      "There is only one built-in example. Other \n"
      "examples can be made available by re-compiling.\n"
      "Interactive features will be available in Mandel 5.14.\n"
      ) + pullBackString, 0, 0, 0u, 4, this);
      connect(dialogx, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogx->exec()) return;
      pplane->stop(); drawLater = 0; lsppp = 0; dplane->move(10);
      updateRegion = true; dplane->draw(f, 0, themode);
      pplane->setCursorPix(spiderPix); dplane->setCursorPix(spiderPix);
      gamma = -3.0; imgno = 25; int k, p;
      k = 4; p = 0; mdouble t; f->pathInfo->n = 150;
      delete[] f->pathInfo->coeff; f->pathInfo->coeff = 0;
      delete[] f->pathInfo->rc; delete[] f->pathInfo->ic;
      f->pathInfo->rc = new mdouble[1 + f->pathInfo->n];
      f->pathInfo->ic = new mdouble[1 + f->pathInfo->n];
      for (int j = 0; j <= f->pathInfo->n; j++)
      {  t = M_PI*sqrt(j/150.0); f->pathInfo->rc[j] = -0.6718986*sin(t);
         f->pathInfo->ic[j] = 0.6718986*(cos(t) - 1.0);
      }
      mpath->capture(k, p, f->pathInfo); f->setDegree(1);
      updateActions(); //pMoved();
      imgno = 0; setPath(stepAct);
   }

//search drawPathSegment when implementing spider and check eps !!!!!!!!!!!!!!

/*dialog = new QmnUIntDialog(trUtf8(
         "Find a p-periodic point:  f_c^p (z) = z .\n"
         "Enter p (1...1024) . Or\n"
         "find a preperiodic point with preperiod k and period p.\n"
         "Enter k and p (separated with a comma, 1...512) :"),
         &n1, &n2, 65002u, 3, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && (n1 ? (n1 <= 512 && n2 <= 512) : n2 <= 1024)
         && !f->find(signtype, n1, n2, x, y)) theplane->setPoint(x, y);
*/
   if (act == spider1Act && ftype == 25)
   {  qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialogs = new QmnCombiDialog(enterAngleString
         + pullBackStringHtml, &N1, &N2, 4, this);
      connect(dialogs, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogs->exec()) return;
      mndlbrot *F = new mndlbrot(2);
      mdouble x, y; int k, p = mndAngle::normalize(N1, N2, k);
      if (!p || (!k && p == 1)
           || dplane->newtonRay(1, N1, N2, x, y, 5, Qt::darkMagenta, 10) > 1
           || F->find(1, k, p, x, y) ) { delete F; setPath(0); return; }
      delete F; pplane->stop(); drawLater = 0; lsppp = 0; dplane->move(10);
      updateRegion = true; dplane->draw(f, 0, themode);
      pplane->setCursorPix(spiderPix); dplane->setCursorPix(spiderPix);
      gamma = -3.0; imgno = 25;
      if (mpath->mating(k, p, x, y)) { setPath(0); return; }
      f->setDegree(2); updateActions(); //pMoved();
      setPath(stepAct);
   }
   if (act == mate1Act)
   {  mdouble x = -0.181318938783978, y = 0.819671390557520;
      QmnDoubleDialog *dialogm = new QmnDoubleDialog(trUtf8(
         "Enter the real and imaginary parts of the parameter c\n"
         "for mating. A high preperiod will be assumed, so this shall\n"
         "work for hyperbolic, parabolic, and Siegel parameters.\n"
         "Use [s] for Misiurewicz parameters (and centers).")
         + pullBackString, &x, &y, 4, this);
      connect(dialogm, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogm->exec()) return;
      pplane->stop(); drawLater = 0; lsppp = 0; dplane->move(10);
      updateRegion = true; dplane->draw(f, 0, themode);
      pplane->setCursorPix(spiderPix); dplane->setCursorPix(spiderPix);
      gamma = -3.0; imgno = 25; int k = 80, p = 1;
      if (mpath->mating(k, p, x, y)) { setPath(0); return; }
      f->setDegree(2); updateActions(); //pMoved();
      setPath(stepAct);
   }
   if (act == mate2Act)
   {  mdouble x = 0.339093, y = -0.446630; enterAngleString = QString();
      if (ftype == 24)
      {  f->getParameter(x, y);
         mdouble u = x*(x*x - 3.0L*y*y); y *= 3.0L*x*x - y*y; x = u;
      }
      else enterAngleString = trUtf8(
         "\nOr start with the quartic polynomial family 4.2.");
         QmnDoubleDialog *dialoga = new QmnDoubleDialog(trUtf8(
         "For the anti-mating of  z^2 - q^2  and  z^2 + q ,\n"
         "enter the real and imaginary parts of  q^3 .\n"
         "A high preperiod will be assumed, so this shall work\n"
         "for hyperbolic, parabolic, and Siegel parameters.\n"
         "For capture components, the center is found.\n"
         "In later versions of Mandel, use [Ctrl+s] for\n"
         "Misiurewicz parameters (and centers).")
         + enterAngleString + pullBackString, &x, &y, 4, this);
      connect(dialoga, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialoga->exec()) return;
      if (ftype == 24) setF(f5Acts[2]); else pplane->stop();
      drawLater = 0; lsppp = 0; dplane->move(10);
      updateRegion = true; dplane->draw(f, 0, themode);
      pplane->setCursorPix(spiderPix); dplane->setCursorPix(spiderPix);
      gamma = -3.0; imgno = 25; int k = 80, p = 1;
      //k = 0; p = 4; x = 0.339093; y = -0.446630; //p4, c4 0.426035-i*0.538591
      if (mpath->anti(k, p, x, y)) { setPath(0); return; }
      f->setDegree(3); updateActions(); //pMoved();
      setPath(stepAct);
   }
   if (act == spider1Act && ftype == 5)
   {  qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialogs = new QmnCombiDialog(enterAngleString,
         &N1, &N2, 4, this);
      connect(dialogs, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogs->exec()) return;
      mndlbrot *F = new mndlbrot(2);
      mdouble x, y, rc, ic; int k, p = mndAngle::normalize(N1, N2, k);
      if (!p || k < 2
           || dplane->newtonRay(1, N1, N2, x, y, 5, Qt::darkMagenta, 10) > 1
           || F->find(1, k, p, x, y) ) { delete F; return; }
      mandelPath *mpm = new mandelMating(); mpm->setMating(k, p, x, y, 1);
      k--; N2 >>= 1; if (N1 > N2) N1 -= N2; N1 = N2 - N1;
      if (dplane->newtonRay(1, N1, N2, x, y, 5, Qt::darkMagenta, 10) > 1
           || F->find(1, k, p, x, y) ) { delete F; delete mpm; return; }
      delete F; mpm->setMating(k, p, x, y, 0);
      for (k = 1; k <= 1000; k++) mpm->step(rc, ic);
      x = rc*rc - ic*ic; y = 2.0L*rc*ic; rc--;
      pplane->setPoint(rc*x - ic*y, ic*x + rc*y); delete mpm;
   }
   if ((act == spider1Act || act == spider2Act) && ftype == 15)
   {  qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialogs = new QmnCombiDialog(enterAngleString,
         &N1, &N2, 4, this);
      connect(dialogs, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogs->exec()) return;
      mndlbrot *F = new mndlbrot(2);
      mdouble x, y, rc, ic; int k, p = mndAngle::normalize(N1, N2, k);
      if (!p || (!k && p == 1)
           || dplane->newtonRay(1, N1, N2, x, y, 5, Qt::darkMagenta, 10) > 1
           || F->find(1, k, p, x, y) ) { delete F; return; }
      delete F; mandelPath *mpm = new mandelMate1();
      mpm->setMating(k, p, x, y, 0);
      for (k = 1; k <= 1000; k++) mpm->step(rc, ic);
      if (act == spider2Act)
      { x = rc*rc + ic*ic; rc /= x; ic /= -x; }
      pplane->setPoint(rc, ic); delete mpm;
   }
   if (act == spider1Act && ftype == 35)
   {  qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialogs = new QmnCombiDialog(enterAngleString,
         &N1, &N2, 4, this);
      connect(dialogs, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialogs->exec()) return;
      mndlbrot *F = new mndlbrot(2);
      mdouble x, y, rc, ic; int k, p = mndAngle::normalize(N1, N2, k);
      if (!p || (!k && p == 1)
           || dplane->newtonRay(1, N1, N2, x, y, 5, Qt::darkMagenta, 10) > 1
           || F->find(1, k, p, x, y) ) { delete F; return; }
      delete F; mandelPath *mpm = new mandelMating();
      mpm->setMating(k, p, x, y, 0); pplane->getPoint(rc, ic);
      x = -0.122561166876654L; y = 0.744861766619744L; if (ic < 0) y = -y;
      if (rc > 0) { x = -1.754877666246693L; y = 0.0L; }
      mpm->setMating(0, 3, x, y, 1);
      for (k = 1; k <= 1000; k++) mpm->step(rc, ic);
      mpm->getParameters(rc, ic, x, y, 3);
      pplane->setPoint(rc, ic); delete mpm;
   }
   if (act == f5Act) //draw segments, later as function 5.m
   {  mdouble a, b, A, B, *x, *y, *X, *Y; QColor cl;
      int i, k, p, K, P; QString name = QString(
      "Slow mating from two angles; only the path of marked points is shown.\n"
      "Save video frames 1026.png to 1800.png in the current directory\n%1 .\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1026 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8.\n\n"
      "Enter the first angle:"
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      qulonglong N1 = 0ULL, N2;
      QmnCombiDialog *dialog = new QmnCombiDialog(name, &N1, &N2, 4, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return; p = mndAngle::normalize(N1, N2, k);
      mndynamics *F = new mndlbrot(2); mandelPath *mpm = new mandelMating();
      if (F->mandelRay(1, -150, N1, N2, &a, &b) < 0 || F->find(1, k, p, a, b) ) return;
      mpm->setMating(k, p, a, b, 0); N1 = 0ULL;
      name = QString("Enter the second angle, then expect some freezing:");
      QmnCombiDialog *dialog1 = new QmnCombiDialog(name, &N1, &N2, 4, this);
      connect(dialog1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog1->exec()) return; P = mndAngle::normalize(N1, N2, K);
      if (F->mandelRay(1, -150, N1, N2, &A, &B) < 0 || F->find(1, K, P, A, B) ) return;
      mpm->setMating(K, P, A, B, 1); mpm->init(0.0L);
      k += p; x = new mdouble[26*k]; y = new mdouble[26*k];
      K += P; X = new mdouble[26*K]; Y = new mdouble[26*K];
      dplane->draw(f, 0, themode); dplane->replaceColor(0, 15);
      for (p = 1026; p < 1800; p += 25)
      {  mpm->getSegments(x, y, 0); mpm->getSegments(X, Y, 1);
         for (P = 0; P < 25; P++)
         {  cl = (p & 1 ? Qt::blue : Qt::darkBlue);
            for (i = 0; i < k; i++)
              dplane->drawLine(x[P*k+i], y[P*k+i], x[P*k+k+i], y[P*k+k+i], cl);
            cl = (p & 1 ? Qt::red : Qt::darkRed);
            for (i = 0; i < K; i++)
              dplane->drawLine(X[P*K+i], Y[P*K+i], X[P*K+K+i], Y[P*K+K+i], cl);
            name = QString("%1.png").arg(p + P); dplane->savePNG(name);
         }
         mpm->step(a, b); dplane->setPoint(a, b);
      }
      delete[] X; delete[] Y; delete[] x; delete[] y;
      delete mpm; delete F; return;
   }
} //setPath

void QmnShell::setAS(QAction *act)
{  if (act == FeigenbaumAct)
   {  mdouble x, y, u, v;
      pplane->getPlane(x, y, u, v);
      pplane->setPlane(cFb + (x - cFb)/dFb, y/dFb, u/dFb, v/dFb);
      dplane->stop(); pplane->draw(f, signtype, themode, 0);
      pplane->getPoint(x, y);
      pplane->setPoint(cFb + (x - cFb)/dFb, y/dFb); //calls pMoved()
   }
   if (act == customAct)
   {  uint j, q, n1, n2 = 0; QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Illustrate asymptotic similarity at a Misiurewicz point\n"
         "a nearby. Enter the preperiod k and the period p\n"
         "(separated with a comma, 1...512).\n"
         "Afterwards you can use Return and Ctrl+l to adjust the order\n"
         "n of rescaling in the parameter plane. Hit Home to quit.\n"
         "When you have put the current parameter inside a\n"
         "hyperbolic component before opening the present dialog box,\n"
         "the higher levels of rescaling will be available with\n"
         "the key l, and the current hyperbolic component will be\n"
         "the first in the sequence spiraling towards a."),
         &n1, &n2, 65002u, 5, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      mdouble x, y, px = 1.0, py = 0.0, w;
      pplane->getPoint(x, y); q = f->period(x, y);
      mdouble t[6]; t[4] = x; t[5] = y;
      if (!dialog->exec() || !n1 || n1 > 512 || n2 > 512
         || f->find(1, n1, n2, x, y)) return;
      if (q <= n1 || q > 512) q = 0;
      t[0] = x; t[1] = y; t[4] -= x; t[5] -= y; //multiplier:
      for (j = 1; j < n1 + n2; j++)
      {  w = x*x - y*y; if (x*x + y*y > 1.0e100) return;
         y = 2.0*x*y + t[1]; x = w + t[0];
         if (j >= n1)
         { w = 2.0*(x*px - y*py); py = 2.0*(x*py + y*px); px = w; }
      }
      w = px*px + py*py; if (w <= 1.0) return;
      t[2] = sqrt(w); t[3] = atan2(py, px);
      if (gamma < 0.0) setPath(0);
      asn = 0; gamma = (q > 0 ? 3.0 : 4.0); q <<= 16;
      lsppp = as->set(q | n1, -n2, t);
      if (dplane->getType() < 0) dplane->setType(0);
      updateRegion = true; updateActions();
      pplane->stop(); pplane->setNmax(500);
      pplane->setPoint(t[0], t[1]); //calls pMoved()
   }
   if (act == chooseAct) { chooseMenu->exec(QCursor::pos()); return; }
   for (int action = 0; action <= 9; action++) if (act == chooseActs[action])
   {  if (gamma < 0.0) setRay(0);
      asn = 0; gamma = 3.0; uint q = 5, n1 = 1, n2 = 2;
      mdouble u, v, t[6]; //re a, im a, abs rho, arg rho, re K, im K
      if (action == 0)
      {  t[0]= 0; t[1] = 1; t[2] = sqrt(32); t[3] = PI/4;
         t[4] = -0.042; t[5] = -0.014;
      }
      if (action == 1)
      {  t[0]= -2; t[1] = 0; t[2] = 4; t[3] = 0;
         t[4] = 3*PI*PI/128.0; t[5] = 0; q = 3; n1 = 1; n2 = 1;
      }
      if (action == 2)
      {  u = -0.23; v = 1.1; f->find(1, 2, 1, u, v);
         t[0]= u; t[1] = v; mndynamics::root(1 - 4*u, -4*v, u, v);
         u++; t[2] = sqrt(u*u + v*v); t[3] = atan2(v, u);
         t[4] = 0.064; t[5] = -0.078; q = 4; n1 = 2; n2 = 1;
      }
      if (action == 3)
      {  u = 0.44; v = 0.41; f->find(1, 4, 1, u, v);
         t[0]= u; t[1] = v; mndynamics::root(1 - 4*u, -4*v, u, v);
         u++; t[2] = sqrt(u*u + v*v); t[3] = atan2(v, u);
         t[4] = 0.0068; t[5] = -0.014; q = 7; n1 = 4; n2 = 1;
      }
      if (action == 4)
      {  u = -0.64; v = 0.69; f->find(1, 4, 1, u, v);
         t[0]= u; t[1] = v; mndynamics::root(1 - 4*u, -4*v, u, v);
         u++; t[2] = sqrt(u*u + v*v); t[3] = atan2(v, u);
         t[4] = 0.036; t[5] = -0.021; q = 6; n1 = 4; n2 = 1;
      }
      if (action == 5)
      {  t[0]= -1.543689012692076L; t[1] = 0;
         t[2] = sqrt(1-4*t[0]) - 1; t[3] = PI;
         t[4] = -0.096; t[5] = 0; q = 5; n1 = 2; n2 = 1;
      }
      if (action == 6)
      {  t[0]= -0.101096363845622L; t[1] = 0.956286510809142L;
         t[2] = 1.328330115300301L; t[3] = 2.086595344738708L;
         t[4] = -0.035; t[5] = 0.03; q = 7; n1 = 3; n2 = 1;
      }
      if (action == 7)
      {  u = -0.176; v = 1.087; f->find(1, 4, 1, u, v);
         t[0]= u; t[1] = v; mndynamics::root(1 - 4*u, -4*v, u, v);
         u = 1 - u; v = -v; t[2] = sqrt(u*u + v*v); t[3] = atan2(v, u);
         t[4] = -0.026; t[5] = 0.01; q = 5; n1 = 4; n2 = 1;
      }
      if (action == 8 || action == 9)
      {  u = -0.19; v = 1.097; f->find(1, 3, 4, u, v);
         t[0]= u; t[1] = v;
         t[2] = 6.461583158563137L; t[3] = -0.302512878511506L;
         if (action == 8)
         { t[4] = -0.0085; t[5] = 0.0032; q = 5; n1 = 3; n2 = 4; }
         else
         { t[4] = 0.006; t[5] = -0.0053; q = 8; n1 = 3; n2 = 4; }
      }
      q <<= 16; as->set(q | n1, -n2, t);
      t[4] = gamma; lsppp = as->set(-1, 0, t);
      pplane->stop(); dplane->stop(); pplane->setNmax(500);
      pplane->setPlane(t[0], t[1], t[2], t[3]);
      pplane->draw(f, 1, themode, 0);
      if (dplane->getType() < 0) dplane->setType(0);
      updateRegion = true; updateActions(); levelAct->setEnabled(true);
      pplane->setPoint(t[0], t[1]); //calls pMoved()
   }
   if (act == stepAct || act == orderAct)
   {  mdouble x, y, u, v = -1, t[7]; uint asnew = uint(asn);
      if (act == orderAct)
      {  uint m = 0; QString text = trUtf8("\xcf\x81^{-n} around a");
         if (gamma >= 1.0 && gamma <= 2.0) text = trUtf8(
            "\xcf\x81^{-\xce\xb3 n}  around  c_n , \xce\xb3 = %1"
            ).arg(QString::number((odouble)(gamma)));
         QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Set the order n \xe2\x89\xa5 0 that defines the factor for "
         "rescaling\n"
         "with %1 . (Use the Return key for n+1.)").arg(text),
         &asnew, &m, 100u, 5, this);
         connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         if (!dialog->exec()) return;
      }
      else asnew++;
      pplane->getPlane(x, y, u, v);
      t[6] = gamma; t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      pplane->getPoint(u, v); t[4] = u; t[5] = v;
      lsppp = as->set(asn, int(asnew), t);
      if (t[2]*t[2] + t[3]*t[3] < 1e-32) return;
      asn = int(asnew); pplane->setPlane(t[0], t[1], t[2], t[3]);
      dplane->stop(); pplane->draw(f, signtype, themode, 0);
      pplane->setPoint(t[4], t[5]); //calls pMoved()
   }
   if (act == levelAct) { levelMenu->exec(QCursor::pos()); return; }
   mdouble g = 0.0;
   if (act == levelaAct) g = 3.0;
   if (act == level0Act) g = 1.0;
   if (act == level1Act) g = 1.5;
   if (act == level2Act) g = 1.75;
   if (act == level3Act) g = 1.875;
   if (act == levelmAct) g = 2.0;
   if (g == 0.0) return;
   gamma = g; asn = 0;
   mdouble t[5]; t[4] = gamma; lsppp = as->set(-1, 0, t);
   pplane->setPlane(t[0], t[1], t[2], t[3]);
   dplane->stop(); pplane->draw(f, signtype, themode, 0);
   updateRegion = true; updateActions();
   pplane->setPoint(t[0], t[1]); //calls pMoved()
} //setAS

void QmnShell::setLS(QAction *act)
{  mdouble x, y; pplane->getPoint(x, y);
   uint preper = lsppp >> 16, per = lsppp & 0xFFF;
   if (!per) { per = f->period(x, y); if (per < 3 || per > 1024) per = 3; }
   if (act == locsimAct)
   {  QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Enter the period, 3...1024, of a nearby primitive small\n"
         "Mandelbrot set. The dynamic plane will be rescaled\n"
         "according to local similarity (demo 7).\n"
         "Or, to rescale according to asymptotic similarity (demo 6),\n"
         "enter the preperiod and the period of a nearby Misiurewicz\n"
         "point (separated with a comma, 1...512).\n"
         "Use Home or enter 1 to quit."),
         &preper, &per, 65002u, 5, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      if (preper ? (preper > 512 || per > 512) : (per < 3 || per > 1024) )
      { lsppp = 0; return; }
      lsppp = preper; lsppp <<= 16; lsppp |= per;
      updateRegion = true; if (dplane->getType() < 0) dplane->setType(0);
      if (gamma < 0.0) setPath(0);
      else { updateActions(); pMoved(); }
   }
} //setLS

void QmnShell::setF(QAction *act)
{  if (act == f5Acts[13]) { setPath(f5Act); return; }
   int i, st = 0;
   if (act == degreeAct && !ftype) { drawing(degreeAct); return; }
   if (act == degreeAct && ftype == 1)
   {  uint q = uint(signtype > 0 ? signtype : -signtype), n = q, m = 3u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the degree q of z^q + c, 3...64 :"),
          &n, &m, 64u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != q)
      {  pplane->stop(); dplane->stop();
         st = n; f->setDegree(st); f->setParameter(0, 0);
      }
   }
   if (act == degreeAct && ftype == 2)
   {  uint q = uint(signtype > 0 ? signtype : -signtype), n = q, m = 2u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the \"degree\" q of cz(1 + z/q)^q, 2...64 :"),
          &n, &m, 64u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != q)
      {  pplane->stop(); dplane->stop();
         st = n; f->setDegree(st); f->setParameter(1.0, 0);
      }
   }
   if (act == degreeAct && ftype == 4 && signtype < 2000 && signtype > -2000)
   {  uint q = uint(signtype > 0 ? signtype : -signtype), n = q, m = 2u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the degree q of c(z^q - qz), 2...64 :"),
          &n, &m, 64u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != q)
      {  pplane->stop(); dplane->stop();
         st = n; f->setDegree(st); f->setParameter(1.0/(1 - st), 0);
      }
   }
   if (act == degreeAct && ftype == 28)
   {  uint q = uint(signtype > 0 ? signtype : -signtype), n = q, m = 2u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Adjust the degree q of (z*)^q + c, 2...64 :"),
          &n, &m, 64u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != q)
      {  pplane->stop(); dplane->stop();
         st = n; f->setDegree(st); f->setParameter(0, 0);
      }
   }
   if (act == degreeAct && ftype == 48) //signtype > 0
   {  uint q = (signtype == 100 ? 0u : (uint)(signtype)), n = q, m = 0u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "Enter 0, 1, or 2 to choose a closure of polynomial root sets:\n"
         "0:  Barnsley set,  coefficients -1, 1, 0.\n"
         "1:  Bousch   set,  coefficients -1, 1.\n"
         "2:  Thurston set,  coefficients from real kneading sequences."),
          &n, &m, 64u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (dialog->exec() && n != q)
      {  pplane->stop(); dplane->stop(); f->setDegree(n); mdouble x, y, r, i;
         pplane->getPlane(x, y, r, i); pplane->setPlane(x, y, r, i); //no Back
         draw(redrawAct); signtype = (!n ? 100 : n); return;
      }
   }
   if (act == degreeAct && ftype == 3 && (signtype == 1 || signtype == -1) )
   {  mdouble x = 0, y = 0; f->iterate(x, y);
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "c(z^3 - 3z) + b , adjust the real part "
         "and the imaginary part of b:"), &x, &y, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      pplane->stop(); dplane->stop();
      st = 1; f->startPlane(0, x, y); //f->setParameter(-0.5, 0);
   }
   if (act == degreeAct && ( (ftype == 3 && signtype != 1 && signtype != -1)
      /*|| (ftype == 5 && signtype != 2 && signtype != -2)*/ ) )
   {  QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "%1 are a two-parameter family.\n"
         "The current map belongs to a one-parameter family\n"
         "defined by a persistent relation between the critical orbits.\n"
         "Now show it in a more arbitrary slice of parameter space.").arg(
         (ftype == 3 ? trUtf8("Cubic polynomials") : trUtf8(
            "Quadratic rational maps"))),
         0, 0, 0u, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
   }
   if (act == degreeAct && ftype == 3 && signtype != 1 && signtype != -1)
   {  mdouble a = 1.0, b = 0, x = 0, y = 0;
      f->iterate(x, y); f->iterate(a, b); a = 0.5*(x - a); b = 0.5*(y - b);
      pplane->stop(); dplane->stop();
      st = 1; delete f; f = new mndcubic(st);
      f->setParameter(a, b); f->startPlane(0, x, y);
   }
   if (act == degreeAct && ftype == 5 && signtype != 2 && signtype != -2)
   {  mdouble a = 0, b = 0, u, v, x = 1.0, y = 0, z;
      f->iterate(a, b); f->iterate(x, y);
      u = a - x; v = b - y; x--; z = u*u + v*v;
      if (z < 1e-20 || z > 1e20) { x = -1.0; y = 0; }
      else { u /= z; v /= z; z = x*u + y*v; y = y*u - x*v; x = z; }
      z = a*x - b*y; b = a*y + b*x; a = z;
      pplane->stop(); dplane->stop();
      st = 1; delete f; f = new mandelV0(st);
      f->setParameter(a, b); f->startPlane(0, x, y);
   }
   if (act == degreeAct && ftype == 5)
   {  mdouble u = 0, v = 0, x = 1.0, y = 0, z;
      f->iterate(u, v); f->iterate(x, y); u -= x; v -= y; x--; z = u*u + v*v;
      if (z < 1e-20 || z > 1e20) { x = -1.0; y = 0; }
      else { u /= z; v /= z; z = x*u + y*v; y = y*u - x*v; x = z; }
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "(z^2 + c)/(z^2 + b) , adjust the real part "
         "and the imaginary part of b:"), &x, &y, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      pplane->stop(); dplane->stop(); st = 1; f->startPlane(0, x, y);
   }
   if (act == degreeAct && ftype == 24)
   {  pplane->stop(); dplane->stop(); drawLater = 0;
      f->setDegree(3); pMoved(); return;
   }
   if (act == degreeAct && ftype > 100)
   {  mdouble t = .61803398874989484820L;
      QmnDoubleDialog *dialog = new QmnDoubleDialog(trUtf8(
         "Adjust the argument t, 0 \xe2\x89\xa4 t < 1, of the\n"
         "multiplier \xce\xbb = e^{i 2\xcf\x80 t} for the neutral\n"
         "cycle. The default value is the Golden Mean:"), &t, 0, 6, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || t <= -1.0 || t >= 1.0) return;
      if (ftype == 115)
      {  t *= PI; mdouble x = cos(t), y = sin(t); t = 4.2;
         pplane->setPoint(t*x, t*y); return;
      }
      st = (signtype > 0 ? signtype : -signtype);
      t *= (2.0*PI); mdouble x = cos(t), y = sin(t);
      pplane->stop(); dplane->stop(); f->startPlane(0, x, y);
      pplane->setPoint(0, 0);
   }
   pplane->stop(); dplane->stop(); setPath(0);
   if (act == f0Act)
   { ftype = 0; st = 3; delete f; f = new mndlbrot(st);
     delete mpath; mpath = new mandelbrotPath(); }
   if (act == f1Act)
   { ftype = 1; st = 4; delete f; f = new mndmulti(st); }
   if (act == f2Act)
   { ftype = 2; st = 3; delete f; f = new mndbfpoly(st); }
   if (act == f3Act) { f3Menu->exec(QCursor::pos()); return; }
   for (i = 1; i <= 7; i++) if (act == f3Acts[i])
   { ftype = 3; st = i; delete f; f = new mndcubic(st); }
   for (i = 8; i <= 9; i++) if (act == f3Acts[i])
   { ftype = 103; st = i - 7; f = new mndcubicsiegel(st); }
   if (act == f4Act) { f4Menu->exec(QCursor::pos()); return; }

   if (act == f4Acts[0])
   { ftype = 4; st = 4; delete f; f = new mndquartic(st); }

   if (act == f4Acts[1])
   { ftype = 4; st = 2000; delete f; f = new mndquartic(st); }

   if (act == f4Acts[2])
   { ftype = 24; st = 1; delete f; f = new mandelQPPQ(st); }

   if (act == f4Acts[3])
   { ftype = 4; st = 3000; delete f; f = new mndquartic(st); }

   for (i = 4; i <= 9; i++) if (act == f4Acts[i])
   {  ftype = 104; st = i - 1;
      if (st == 6) st = 2; if (st == 7) st = 6; if (st == 8) st = 1;
      f = new mndquartsiegel(st);
   }
   if (act == f5Act) { f5Menu->exec(QCursor::pos()); return; }
   //if (act == f5Acts[13]) //above
   //{ ftype = 95; st = 1; delete f; f = new mndMating(st); }
   if (act == f5Acts[0])
   { ftype = 5; st = 1; delete f; f = new mandelV0(st); }//path also q
   if (act == f5Acts[1])
   { ftype = 15; st = 1; delete f; f = new mandelV1(st); }
   if (act == f5Acts[2])
   { ftype = 25; st = 4; delete f; f = new mandelV2(st);
     delete mpath; mpath = new mndPathTwo(1); }
   if (act == f5Acts[3])
   { ftype = 35; st = 4; delete f; f = new mandelV3(st); }
     //delete mpath; mpath = new mndPathThree(1); }
   if (act == f5Acts[14])
   { ftype = 145; st = 4; delete f; f = new mandelV45(st); }
   if (act == f5Acts[15])
   { ftype = 155; st = 5; delete f; f = new mandelV45(st); }
   for (i = 4; i <= 8; i++) if (act == f5Acts[i])
   { ftype = 45; st = i; delete f; f = new mandelRational(st); }
   if (act == f5Acts[9])
   { ftype = 105; st = 1; delete f; f = new mandelMateSiegel(st); }
   if (act == f5Acts[10])
   { ftype = 115; st = 1; delete f; f = new mandelHerman(st); }
   if (act == f5Acts[11])
   { ftype = 65; st = 1; delete f; f = new mndmenage(st); }
   if (act == f5Acts[12])
   { ftype = 75; st = 1; delete f; f = new mndsingpert(st); }

   if (act == f6Act) { f6Menu->exec(QCursor::pos()); return; }
   if (act == f6Acts[0])
   { ftype = 6; st = 1; delete f; f = new mndcubicnewton(st); }
   for (i = 1; i <= 8; i++) if (act == f6Acts[i])
   { ftype = 16; st = i; delete f; f = new mndquarticnewton(st); }
   if (act == f6Acts[10])
   { ftype = 116; st = 1; delete f; f = new mndnewtonpara(st); }
   if (act == f6Acts[11])
   { ftype = 116; st = 2; delete f; f = new mndnewtonqara(st); }
   if (act == f6Acts[12])
   { ftype = 116; st = 3; delete f; f = new mndnewtonrara(st); }
   if (act == f7Act) { f7Menu->exec(QCursor::pos()); return; }
   if (act == f7Acts[0])
   { ftype = 117; st = 1; delete f; f = new mndtrigosiegel(st); }
   for (i = 1; i <= 2; i++) if (act == f7Acts[i])
   { ftype = 7; st = i; delete f; f = new mndexpo(st); }
   if (act == f7Acts[3])
   { ftype = 107; st = 1; delete f; f = new mndexposiegel(st); }
   for (i = 4; i <= 9; i++) if (act == f7Acts[i])
   { ftype = 17; st = i - 3; delete f; f = new mndtrigo(st); }
   if (act == f8Act) { f8Menu->exec(QCursor::pos()); return; }
   for (i = 1; i <= 3; i++) if (act == f8Acts[i])
   { ftype = 8; st = i; delete f; f = new mndsurge(st); }
   if (act == f8Acts[4])
   { ftype = 18; st = 1; delete f; f = new mndrealcubic(st); }
   if (act == f8Acts[5])
   { ftype = 18; st = 2; delete f; f = new mndrealcubic(st); }
   if (act == f8Acts[6])
   { ftype = 28; st = 2; delete f; f = new mndtricorn(st); }
   if (act == f8Acts[7])
   { ftype = 38; st = 1; delete f; f = new mndhenon(st); }
   if (act == f8Acts[8])
   { ftype = 48; st = 1; delete f; f = new mndifs(st); }
   if (act == f8Acts[9])
   { ftype = 58; st = 1; delete f; f = new mndlambda(st); }
   if (!st) return;
   signtype = st; theplane = pplane; themode = &pmode; setPath(0);
   pmode = f->minMode(1); if (!pmode && f->maxMode(1)) pmode = 1;
   dmode = f->minMode(-1); if (!dmode && f->maxMode(-1)) dmode = 1;
   if (ftype == 5 || ftype == 15 || ftype == 45 || ftype == 105)
   { pmode = 4; dmode = 4; } 
   if (ftype == 25 || ftype == 35)
   { pmode = 2; dmode = 2; } 
   if (ftype == 6 || ftype == 16) { pmode = 2; dmode = 2; }
   if (ftype == 18) pmode = 2;
   pcoords->setEnabled(true); dcoords->setEnabled(false);
   pplane->setActive(true); dplane->setActive(false); dplane->setType(0);
   updateRegion = true; updateActions(); draw(homeAct);
}

void QmnShell::setFile(QAction *act)
{  if (act == colorAct)
   {  uint p = 13u, q = 14u;
      QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
         "<b></b>Replace a VGA-color by entering the numbers,<br>"
	 "separated with a comma:<br>"
	 "<font color=\"#000080\"> 1</font>"
	 "&nbsp;&nbsp;<font color=\"#008000\"> 2</font>"
	 "&nbsp;&nbsp;<font color=\"#008080\"> 3</font>"
	 "&nbsp;&nbsp;<font color=\"#800000\"> 4</font>"
	 "&nbsp;&nbsp;<font color=\"#800080\"> 5</font>"
	 "&nbsp;&nbsp;<font color=\"#808000\"> 6</font>"
	 "&nbsp;&nbsp;<font color=\"#c0c0c0\"> 7</font>"
	 "&nbsp;&nbsp;<font color=\"#808080\"> 8</font><br>"
	 "<font color=\"#0000ff\"> 9</font>"
	 " <font color=\"#00ff00\">10</font>"
	 " <font color=\"#00ffff\">11</font>"
	 " <font color=\"#ff0000\">12</font>"
	 " <font color=\"#ff00ff\">13</font>"
	 " <font color=\"#ffff00\">14</font>"
	 " <font color=\"#ffffff\">15</font>"
	 " <font color=\"#000000\">16</font>"),
         &p, &q, 65002u, 7, this);//*/
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || p > 16u || q > 16u || p == q) return;
      if (p == 16u) p = 0u; if (q == 16u) q = 0u;
      theplane->replaceColor(p, q);
   }
   if (act == overlayAct)
   { if (signtype > 0) pplane->overlay(dplane); else dplane->overlay(pplane); }
   if (act == epsMenuAct) { epsMenu->exec(QCursor::pos()); return; }
   int bpp, BPP = -1;
   for (bpp = 0; bpp <= 3; bpp++) if (act == epsActs[bpp]) BPP = bpp;
   if (!BPP) BPP = 4;
   if (act == epsSaveAct || BPP > 0) //mode 3 and sphere are excluded
   {  bpp = 4;
      if (*themode >= 2 && *themode <= 8) bpp = 2;
      if (ftype == 16 && *themode == 2) bpp = 4;
      if (ftype == 18 && signtype > 0 && *themode == 2) bpp = 4;
      if (*themode == 1 || *themode == 5) bpp = 1;
      if ((*themode == 4 && ftype <= 3) || ftype == 48) bpp = 1;
      if (ftype == 58) bpp = 2;
      if (!*themode || (gamma == -1.0 && signtype < 0)) bpp = 3;
      if (BPP > 0) bpp = BPP;
      QString name = trUtf8("Mandel: save EPS b/w");
      if (bpp == 2) name = trUtf8("Mandel: save EPS in 4 colors");
      if (bpp > 2) name = trUtf8("Mandel: save EPS in 16 colors");
      name = QFileDialog::getSaveFileName(this, name, ".eps",
         "PostScript (*.eps)");
      if (name.isEmpty()) return;
      if (!name.contains(".eps", Qt::CaseInsensitive)) name += ".eps";
      theplane->saveEPS(bpp, name);
      QFileInfo fi(name); QDir::setCurrent(fi.absolutePath());
   }
   if (act == pngSaveAct || act == bothSaveAct)
   {  QString name = QFileDialog::getSaveFileName(this,
         trUtf8("Mandel: save PNG"), ".png", trUtf8("Images (*.png)"));
      if (name.isEmpty()) return;
      if (!name.contains(".png", Qt::CaseInsensitive)) name += ".png";
      if (act == pngSaveAct) theplane->savePNG(name);
      else pplane->savePNG(name, dplane);
      QFileInfo fi(name); QDir::setCurrent(fi.absolutePath());
   }
   if (act == pngLoadAct)
   {  QString name = QFileDialog::getOpenFileName(this,
         trUtf8("Mandel: load PNG"), ".png", trUtf8("Images (*.png)"));
      if (!name.isEmpty() && !theplane->loadPNG(name))
      {  QmnUIntDialog *dialog = new QmnUIntDialog(trUtf8(
           "Cannot load the image - maybe wrong size."), 0, 0, 0u, 7, this, 0);
         connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         dialog->exec();
      }
      QFileInfo fi(name); QDir::setCurrent(fi.absolutePath());
   }
   if (act == catmapAct) theplane->catMap(1);
   if (act == catinvAct) theplane->catMap(0);
   if (act == textAct)
   {  QString name = QFileDialog::getSaveFileName(this,
         trUtf8("Mandel: save TXT"), ".txt", "text (*.txt)");
      if (name.isEmpty()) return;
      if (!name.contains(".txt", Qt::CaseInsensitive)) name += ".txt";
      theplane->saveTXT(name);
      QFileInfo fi(name); QDir::setCurrent(fi.absolutePath());
   }
   if (act == videoAct)
   {  if (gamma >= 0.0L && !lsppp) return;
      if (gamma < 0.0L && imgno <= 0) return;
      QString text = trUtf8(
      "Save video frames 1001.png to 2000.png in the current directory\n%1\n"
      "You can stop with ESC. Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(text, 0, 0, 0u, 7, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      imgno = 10000; pplane->stop(); drawLater = 0; dplane->stop(); pMoved();
      /* repeat one frame 25 times per second
      (last frame will be lost,  without fps the first may be lost):
      ffmpeg -framerate 1 -i %02d.png -c:v libx264 -pix_fmt yuv420p
      -vf fps=25 out.mp4  */
   }
   if (act == vidMenuAct) { vidMenu->exec(QCursor::pos()); return; }
   if (act == vidAct1)
   {  QString name = trUtf8(
      "Save video frames 1001.png to 2000.png in the current directory\n%1\n"
      "by overlaying _red_ pixels of images from the subdirectories p and q.\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(name, 0, 0, 0u, 7, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      for (int img = 1001; img <= 2000; img++)
      {  name = QString("p/%1.png").arg(img);
         if (!pplane->loadPNG(name)) continue;
         name = QString("d/%1.png").arg(img);
         if (!dplane->loadPNG(name)) continue;
         pplane->Overlay(dplane);
         name = QString("%1.png").arg(img);
         pplane->savePNG(name);
      }
   }
   if (act == vidAct2)
   {  QString name = trUtf8(
      "Save video frames 1001.png to 2000.png in the current directory\n%1\n"
      "by combining images from the subdirectories p and q.\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(name, 0, 0, 0u, 7, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      for (int img = 1001; img <= 2000; img++)
      {  name = QString("p/%1.png").arg(img);
         if (!pplane->loadPNG(name)) continue;
         name = QString("d/%1.png").arg(img);
         if (!dplane->loadPNG(name)) continue;
         name = QString("%1.png").arg(img);
         pplane->savePNG(name, dplane);
      }//*/
      /*for (int img = 1001; img <= 2000; img++)
      {  name = QString("p/%1.png").arg(img);
         if (!pplane->loadPNG(name)) ;//continue;
         name = QString("d/%1.png").arg(img);
         if (!dplane->loadPNG(name)) ;//continue;
         name = QString("%1.png").arg(img);
         pplane->savePNG(name, dplane);
      }//*/
      /*for (int img = 1001; img <= 2000; img++)
      {  name = QString("p/%1.png").arg(img > 1025 ? img + 25 : img);
         if (!pplane->loadPNG(name)) ;//continue;
         name = QString("d/%1.png").arg(img);
         if (!dplane->loadPNG(name)) ;//continue;
         name = QString("%1.png").arg(img);
         pplane->savePNG(name, dplane);
      }//*/
   }
   if (act == vidAct4)
   {  QString name = trUtf8(
      "Save video frames 1001.png to 2000.png in the current directory\n%1\n"
      "by combining images from the subdirectories rt, lt, lb, rb.\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(name, 0, 0, 0u, 7, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      for (int img = 1001; img <= 2000; img++)
      {  name = QString("rt/%1.png").arg(img); pplane->savePNG(name, 0, 1);
         name = QString("lt/%1.png").arg(img); pplane->savePNG(name, 0, 2);
         name = QString("lb/%1.png").arg(img); pplane->savePNG(name, 0, 3);
         name = QString("rb/%1.png").arg(img); pplane->savePNG(name, 0, 4);
         name = QString("%1.png").arg(img); pplane->savePNG(name);
      }
   }
   if (act == vidAct5)
   {  uint p = 13u, q = 14u;
      QString name = trUtf8(
         "<b></b>For the files 1001.png to 2000.png in the current "
         "directory<br>%1 ,<br>"
         "replace a VGA-color by entering the numbers,<br>"
         "separated with a comma:<br>"
         "<font color=\"#000080\"> 1</font>"
         "&nbsp;&nbsp;<font color=\"#008000\"> 2</font>"
         "&nbsp;&nbsp;<font color=\"#008080\"> 3</font>"
         "&nbsp;&nbsp;<font color=\"#800000\"> 4</font>"
         "&nbsp;&nbsp;<font color=\"#800080\"> 5</font>"
         "&nbsp;&nbsp;<font color=\"#808000\"> 6</font>"
         "&nbsp;&nbsp;<font color=\"#c0c0c0\"> 7</font>"
         "&nbsp;&nbsp;<font color=\"#808080\"> 8</font><br>"
         "<font color=\"#0000ff\"> 9</font>"
         " <font color=\"#00ff00\">10</font>"
         " <font color=\"#00ffff\">11</font>"
         " <font color=\"#ff0000\">12</font>"
         " <font color=\"#ff00ff\">13</font>"
         " <font color=\"#ffff00\">14</font>"
         " <font color=\"#ffffff\">15</font>"
         " <font color=\"#000000\">16</font>"
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(name, &p, &q, 65002u, 7, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec() || p > 16u || q > 16u || p == q) return;
      if (p == 16u) p = 0u; if (q == 16u) q = 0u;
      for (int img = 1001; img <= 2000; img++)
      {  name = QString("%1.png").arg(img);
         if (!pplane->loadPNG(name)) continue;
         pplane->replaceColor(p, q); pplane->savePNG(name);
      }
   }

   /*/temporary for spine: set n, load 1025 from other vids, hit shift F9 6
   if (act == veinAct2 || act == limbAct2)
   {  QString name; mndynamics *F = new mndlbrot(2);
      mndPathMate *Mpath = new mndPathMate(0, 50);
      Mpath->mating(0, 3, -1.754877666246693L, 0.0L, logTen, 1);
      mdouble x, y, x0 = 0.8445944976806L, y0 = 0.8921881866455L;
      uint img = 1026, n, pp; qulonglong M, N, Nmin, Nmax, S, D = 7ULL;
      pplane->getNmax(pp); pplane->setNmax(100);
      if (pp > 30 || pp < 15) pp = 15; D <<= pp;
      //pplane->setPlane(0.10557L, 1.09785L, 0.9336L, 0.0L);
      //Nmin = D/7ULL + 1ULL; Nmax = 3ULL*D/14ULL; S = (Nmax - Nmin)/599ULL;
      pplane->setPlane(0.69153L, 0.38915L, 0.55240L, 0.0L);
      Nmin = 2ULL*D/7ULL + 1ULL; Nmax = D/3ULL; S = (Nmax - Nmin)/598ULL;
      for (N = Nmin; N <= Nmax; N++)
      {  if (!(N % S))
         { name = QString("%1.png").arg(img); pplane->savePNG(name); img++; }
         if (!(N % 7ULL)) continue; M = N;
         for (n = 1; n < pp; n++)
         {  M <<= 1; if (M >= D) M -= D;
            if (7ULL*M < D || 7ULL*M > 6ULL*D) { M = 0ULL; break; }
         }
         if (!M) continue;
         M = N; n = pp;
         while (1) { if (M & 1ULL) break; M >>= 1; n--; }
         if (pplane->newtonRay(1, N, D, x, y, 5, Qt::white, 10) > 1
             || F->find(1, n, 3, x, y) ) continue;
         Mpath->mating(n, 3, x, y, logTen, 0);
         for (n = 1; n <= 60; n++) if (Mpath->step(x, y)) { n = 0; break; }
         //if (img < 1030 && y < x + 0.0475936889649L) continue;
         //normalize  (z^2 + u+iv) / (z^2 + X+iY)
         mdouble rc = x, ic = y, rd = Mpath->rD, id = Mpath->iD,
         u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
         u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
         u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
         X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
         w = rd*rd + id*id; u = rc/w; v = ic/w;
         w = u*rd + v*id; v = v*rd - u*id; u = w;
         w = u*X - v*Y; v = u*Y + v*X; u = w;
         //for V_3
         u++; X++; w = X*X + Y*Y; x = -(u*X + v*Y)/w; y = (u*Y - v*X)/w;
         if (n) { pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
      }
      if (y0 > 0.2) { x = -0.39512338225965L; y = 0.506843901805983L; }
      else { x = 1.0; y = 0.0; }
      pplane->drawLine(x0, y0, x, y, Qt::red);
      name = QString("%1.png").arg(img); pplane->savePNG(name);
      return;
   } // temporary for spine */

   if (act == veinAct2 || act == limbAct2)
   {  QString name, text = trUtf8(
      "Mate the 1/2-vein or 1/2-limb with:\n"
      "the Rabbit in family 5.3,\n"
      "-1/6 in family 5.6 with n_max even,\n"
      "3/14 in family 5.6 with n_max odd.\n"
      "Video frames are saved in the current directory\n%1\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      if (ftype == 45)
      { uint m; pplane->getNmax(m); if (m & 1) ftype = 65; }
      mndynamics *F = new mndlbrot(2); pplane->setNmax(500);
      mndPathMate *Mpath = new mndPathMate(0, 50);
      int img = 1000, k, p, n, j = 1, K, npf;
      qulonglong N0, D0, N, D, N1;
      if (act == limbAct2)
      {  p = 6; K = 14; N0 = 21ULL << K; D0 = 63ULL << K; N1 = 42ULL << K;
         npf = (1 << K)/37 + 1; //steps per frame, about 800 frames
      }
      else
      { k = 0; K = 0; N0 = 6919830ULL; D0 = 16777215ULL; } 
      mdouble a, b, x, y, x0, y0, x1, y1;
      //first image (x, 0, a, 0), last (x1, y1, b, 0), mating start x0, y0 
      if (!ftype)
      {  x = -0.5L; x1 = -1.38L; y1 = 0.0L; a = 1.55L; b = 0.67L;
         *themode = 4; x0 = -0.75L; y0 = 0.0L;
      }
      if (ftype == 35)
      {  x = 0.5703L; x1 = 0.10557L; y1 = 1.09785L; a = 2.2465L; b = 0.9336L;
         *themode = 1; x0 = -0.60023707L; y0 = 0.8001647L; //Rabbit
         Mpath->mating(0, 3, -0.122561166876654L, 0.744861766619744L, logTen, 1);
      }
      if (ftype >= 45) //5.6
      {  x = -1.0L; x1 = 0.546511L; y1 = 1.499118L; a = 4.3L; b = 2.573231L;
         *themode = 4; x0 = 3.0L; y0 = 0.0L; //-i
         Mpath->mating(1, 2, 0.0L, -1.0L, logTen, 1);
      }
      if (ftype == 65) //5.6
      {  x0 = 1.0L; y0 = sqrt(12.0L); //3/14
         Mpath->mating(1, 3, -0.155788496687127L, 1.112217114596038L, logTen, 1);
      }
      while (img < 1025)
      {  img++; y = ((mdouble)(img - 1001))/24.0L;
         pplane->setPlane(x + y*(x1 - x), y*y1, a + y*(b - a), 0.0L);
         pplane->draw(f, 1, themode);
         text = trUtf8("Please confirm that the image is drawn completely.");
         if (img == 1025) text += trUtf8("\n\nExpect a short freezing ...");
         dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
         connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         if (!dialog->exec()) return;
         if (ftype == 15)
         {  pplane->replaceColor(13, 16);
            for (j = 1; j <= 12; j++) pplane->replaceColor(j, 7);
         }
         else
         {  pplane->replaceColor(10, 8); pplane->replaceColor(0, 7);
            pplane->replaceColor(9, 1); pplane->replaceColor(1, 16);
         }
#ifdef SaveFramesOfVeinsLimbsX
         name = QString("%1.png").arg(img); pplane->savePNG(name);
#endif
      }
      n = 0; while (1)
      {  n++;
         if (K)
         {  N0++; if (N0 >= N1) break; 
            k = K; N = N0; D = D0;
            while (!(N & 1ULL) && !(D & 1ULL))
            { k--; N >>= 1; D >>= 1; }
            if (!k) continue;
         }
         else
         {  if (2ULL*N0 >= D0) break;
            if (n == 1) { p = 2; N = 1ULL; D = 3ULL; }
            if (n == 2) { p = 4; N = 6ULL; D = 15ULL; }
            if (n == 3) { p = 8; N = 105ULL; D = 255ULL; }
            if (n == 4) { p = 24; N = N0; D = D0; }
            if (n >= 5)
            {  if (N0 == 7190234ULL)
               { p = 3; N = 3ULL; D = 7ULL; N0 = 7456540ULL; }
               else if (N0 == 7456540ULL)
               { p = 6; N = 28ULL; D = 63ULL; N0 = 7489828ULL; }
               else if (N0 == 7489828ULL)
               { p = 12; N = 1828ULL; D = 4095ULL; N0 = 0ULL; }
               else if (N0 == 0ULL)
               { p = 16; N = 29257ULL; D = 65535ULL; N0 = N; D0 = D; }
               else while (2ULL*N0 < D0)
               {  N0++; N = N0;
                  for (j = 1; j <= 24; j++)
                  {  N <<= 1; if (2ULL*N > D0) N = D0 - N;
                     if (N > N0) { N = 0ULL; break; }
                     if (N == N0) { p = j; break; }
                  }
                  if (N) { N = N0; break; } else N0++; //two angles invalid
               }
            }
         }
         if (pplane->newtonRay(1, N, D, x1, y1, 5, Qt::darkMagenta, 10) > 1
             || F->find(1, k, p, x1, y1)) continue;
         if (ftype == 35)
         {  if (3ULL*D0 == 7ULL*N0 || 4ULL*D0 == 7ULL*N0
                || (K && 7ULL*(N0 - 1ULL) == 3ULL*D0)
                || (K && 7ULL*(N0 - 1ULL) == 4ULL*D0))
            { x = 0.844727L; y = 0.893066L; pplane->drawLine(x0, y0, x, y, Qt::green); x0 = x; y0 = y; }
            Mpath->mating(k, p, x1, y1, logTen, 0);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            //normalize  (z^2 + u+iv) / (z^2 + X+iY)
            mdouble rc = x1, ic = y1, rd = Mpath->rD, id = Mpath->iD,
            u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
            u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
            u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
            X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
            w = rd*rd + id*id; u = rc/w; v = ic/w;
            w = u*rd + v*id; v = v*rd - u*id; u = w;
            w = u*X - v*Y; v = u*Y + v*X; u = w;
            //for V_3
            u++; X++; w = X*X + Y*Y; x1 = -(u*X + v*Y)/w; y1 = (u*Y - v*X)/w;
         }
         if (ftype == 45 || (ftype == 65 && x1 <= -1.75))
         {  if (ftype == 45 && (3ULL*D0 == 7ULL*N0 || 4ULL*D0 == 7ULL*N0))
            { x = -0.502134L; y = 0.865849L; pplane->drawLine(x0, y0, x, y, Qt::green); x0 = x; y0 = y; }
            if (ftype == 65 && (7ULL*D0 == 15ULL*N0 || 8ULL*D0 == 15ULL*N0))
            { x = 1.0L; y = 0.0L; pplane->drawLine(x0, y0, x, y, Qt::green); x0 = x; y0 = y; }
            Mpath->mating(k, p, x1, y1, logTen, 0);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            //normalize  (z^2 + u+iv) / (z^2 + X+iY)
            mdouble rc = x1, ic = y1, rd = Mpath->rD, id = Mpath->iD,
            u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
            u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
            u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
            X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
            w = rd*rd + id*id; u = rc/w; v = ic/w;
            w = u*rd + v*id; v = v*rd - u*id; u = w;
            w = u*X - v*Y; v = u*Y + v*X; u = w;
            //for Ch
            x1 = X; y1 = Y;
         }
         else if (ftype == 65) { x1 = x0; y1 = y0; }
         if (j)
         { pplane->drawLine(x0, y0, x1, y1, Qt::green); x0 = x1; y0 = y1; }
         if (!K) npf = (n <= 1850 ? 5 : 15);
         if (!(n % npf))
         { img++; name = QString("%1.png").arg(img); pplane->savePNG(name); }
      }
      img++; name = QString("%1.png").arg(img); setWindowTitle(name);
#ifdef SaveFramesOfVeinsLimbs
      pplane->savePNG(name);
#endif
      delete F; delete Mpath; if (ftype == 65) ftype = 45;
   }//veinAct2

   if (act == veinAct3 || act == limbAct3)
   {  QString name, text = trUtf8(
      "Mate the 1/4-vein or 1/3-limb with:\n"
      "   itself in family 5.1 with n_max even,\n"
      "anti-self in family 5.1 with n_max odd, and conjugated,\n"
      "the Basilica in family 5.2,\n"
      "the Airplane in family 5.3,\n"
      " 1/2 in family 5.6 with n_max even,\n"
      "5/14 in family 5.6 with n_max odd.\n"
      "Video frames are saved in the current directory\n%1\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      if (ftype == 15)
      { uint m; pplane->getNmax(m); if (m & 1) ftype = 5; }
      if (ftype == 45)
      { uint m; pplane->getNmax(m); if (m & 1) ftype = 65; }
      mndynamics *F = new mndlbrot(2);
      pplane->setNmax((ftype == 5 || ftype == 15) ? 2000 : 500);
      mndPathMate *Mpath = new mndPathMate(0, 60);
      mndPathTwo *MMpath = new mndPathTwo(0, 60);
      int img = 1000, k, p, n, j = 1, K, npf;
      qulonglong N0, D0, N, D, N1;
      if (act == limbAct3)
      {  p = 6; K = 15; N0 = 9ULL << K; D0 = 63ULL << K; N1 = 18ULL << K;
         npf = (1 << K)/86 + 1; //steps per frame, about 800 frames
      }
      else
      { k = 0; K = 0; N0 = 4498058ULL; D0 = 16777215ULL; } 
      mdouble a, b, x, y, x0, y0, x1, y1;
      //first image (x, 0, a, 0), last (x1, y1, b, 0), mating start x0, y0 
      if (!ftype)
      {  x = -0.5L; x1 = -0.131L; y1 = 0.879L; a = 1.55L; b = 0.246L;
         *themode = 4; x0 = -0.125L; y0 = 0.649519052838329L;
      }
      if (ftype == 5 || ftype == 15)
      {  x = 1.0L; x1 = -0.1L; y1 = 1.05L; a = 2.2L; b = 1.1L;
         *themode = 0; x0 = 0.0L; y0 = 0.5766L; if (ftype == 5) y0 = 1.0L/y0;
      }
      if (ftype == 25)
      {  x = 0.28125L; x1 = 1.05088L; y1 = 1.04004L; a = 2.20488L; b = 1.07488L;
         *themode = 4; x0 = 1.0L/3.0L; y0 = 0.769800358919501L;
      }
      if (ftype == 35)
      {  x = 0.5703L; x1 = 0.10557L; y1 = 1.09785L; a = 2.2465L; b = 0.9336L;
         *themode = 1; x0 = 0.8445944976806L; y0 = 0.8921881866455L;
         Mpath->mating(0, 3, -1.754877666246693L, 0.0L, logTen, 1);
      }
      if (ftype >= 45) //5.6
      {  x = -1.0L; x1 = 0.546511L; y1 = 1.499118L; a = 4.3L; b = 2.573231L;
         *themode = 4; x0 = -0.5029302; y0 = 0.8655227L;
         Mpath->mating(1, 1, -2.0L, 0.0L, logTen, 1);
      }
      if (ftype == 65) //5.6
      {  x0 = 1.0L; y0 = sqrt(12.0L); // 3/14 5/14 13/14
         //Mpath->mating(1, 3, -0.155788496687127L, 1.112217114596038L, logTen, 1);
         Mpath->mating(1, 3, -1.239225555389567L, 0.412602181602004L, logTen, 1);
         //Mpath->mating(1, 3, 0.395014052076695L, -0.555624571005996L, logTen, 1);
      }
      while (img < 1025)
      {
#ifdef SaveFramesOfVeinsLimbsX
         img++;
#else
         img += 25;
#endif
         y = ((mdouble)(img - 1001))/24.0L;
         pplane->setPlane(x + y*(x1 - x), y*y1, a + y*(b - a), 0.0L);
         pplane->draw(f, 1, themode);
         text = trUtf8("Please confirm that the image is drawn completely.");
         if (img == 1025) text += trUtf8("\n\nExpect a short freezing ...");
         dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
         connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         if (!dialog->exec()) return;
         if (ftype == 5 || ftype == 15)
         {  pplane->replaceColor(13, 16);
            for (j = 1; j <= 12; j++) pplane->replaceColor(j, 7);
         }
         else
         {  pplane->replaceColor(10, 8); pplane->replaceColor(0, 7);
            pplane->replaceColor(9, 1); pplane->replaceColor(1, 16);
         }
#ifdef SaveFramesOfVeinsLimbsX
         name = QString("%1.png").arg(img); pplane->savePNG(name);
#endif
      }
      n = 0; while (1)
      {  n++;
         if (K)
         {  N0++; if (N0 >= N1) break; 
            k = K; N = N0; D = D0;
            while (!(N & 1ULL) && !(D & 1ULL))
            { k--; N >>= 1; D >>= 1; }
            if (!k) continue;
         }
         else
         {  if (4ULL*N0 < D0) break;
            if (n == 1) { p = 3; N = 2ULL; D = 7ULL; }
            if (n == 2) { p = 6; N = 17ULL; D = 63ULL; }
            if (n == 3) { p = 12; N = 1098ULL; D = 4095ULL; }
            if (n == 4) { p = 24; N = 4498065ULL; D = D0; }
            if (n == 5) { p = 24; N = 4498058ULL; D = D0; }
            if (n >= 6)
            {  if (N0 == 4493897ULL && D0 == 16777215ULL)
               { N0 = 8987795ULL; D0 = 33554431ULL; } //p = 25;
               if (N0 == 8950345ULL && D0 == 33554431ULL)
               { N0 = 4475173ULL; D0 = 16777215ULL; } //p = 24;*/
               while ((N0 << 2) > D0)
               {  N0--; N = N0; D = D0; mndAngle::conjugate(N, D);
                  if (4ULL*N < D) break; else N0--; //two angles invalid
               }
               N1 = N; p = 0;
               while (p <= 60)
               { p++; N1 <<= 1; if (N1 >= D) N1 -= D; if (N1 == N) break; }
            }
         }
         if (pplane->newtonRay(1, N, D, x1, y1, 5, Qt::darkMagenta, 10) > 1
             || F->find(1, k, p, x1, y1)) continue;
         if (ftype == 5 || ftype == 15)
         {  if (3ULL*D0 == 15ULL*N0 || 4ULL*D0 == 15ULL*N0)
            { x = 0.333370L; y = 0.942789L; pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
            for (j = 0; j <= 1; j++)
               Mpath->mating(k, p, x1, y1, logTen, j);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            if (ftype == 5) { x = x1*x1 + y1*y1; x1 /= x; y1 /= x; }
         }
         if (ftype == 25)
         {  if (3ULL*D0 == 15ULL*N0 || 4ULL*D0 == 15ULL*N0
                || (K && 15ULL*(N0 - 1ULL) == 3ULL*D0) //ineffective
                || (K && 15ULL*(N0 - 1ULL) == 4ULL*D0))
            { x = 1.074122L; y = 1.676149L; pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
            MMpath->mating(k, p, x1, y1, logTen, 0);
            for (j = 1; j <= 60; j++)
            if (MMpath->step(x1, y1)) { j = 0; break; }
         }
         if (ftype == 35)
         {  if (3ULL*D0 == 15ULL*N0 || 4ULL*D0 == 15ULL*N0
                || (K && 15ULL*(N0 - 1ULL) == 3ULL*D0) //ineffective
                || (K && 15ULL*(N0 - 1ULL) == 4ULL*D0))
            { x = -0.333291L; y = 0.9428418L; pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
            Mpath->mating(k, p, x1, y1, logTen, 0);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            //normalize  (z^2 + u+iv) / (z^2 + X+iY)
            mdouble rc = x1, ic = y1, rd = Mpath->rD, id = Mpath->iD,
            u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
            u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
            u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
            X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
            w = rd*rd + id*id; u = rc/w; v = ic/w;
            w = u*rd + v*id; v = v*rd - u*id; u = w;
            w = u*X - v*Y; v = u*Y + v*X; u = w;
            //for V_3
            u++; X++; w = X*X + Y*Y; x1 = -(u*X + v*Y)/w; y1 = (u*Y - v*X)/w;
         }
         if (ftype >= 45)
         {  if ((3ULL*D0 == 15ULL*N0 || 4ULL*D0 == 15ULL*N0) && x0 < 0.0L)
            { x = -1.00326L; y = 1.99533L; pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
            if ((3ULL*D0 == 15ULL*N0 || 4ULL*D0 == 15ULL*N0) && y0 < 0.35L)
            { x = 1.0L; y = 0.0L; pplane->drawLine(x0, y0, x, y, Qt::red); x0 = x; y0 = y; }
            Mpath->mating(k, p, x1, (ftype == 45 ? -y1 : y1), logTen, 0);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            //normalize  (z^2 + u+iv) / (z^2 + X+iY)
            mdouble rc = x1, ic = y1, rd = Mpath->rD, id = Mpath->iD,
            u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
            u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
            u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
            X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
            w = rd*rd + id*id; u = rc/w; v = ic/w;
            w = u*rd + v*id; v = v*rd - u*id; u = w;
            w = u*X - v*Y; v = u*Y + v*X; u = w;
            //for Ch
            x1 = X; y1 = Y;
         }
         if (j) { pplane->drawLine(x0, y0, x1, y1, Qt::red); x0 = x1; y0 = y1; }
         if (!K) npf = (n <= 1614 ? 6 : 20);
         if (!(n % npf))
         { img++; name = QString("%1.png").arg(img);  pplane->savePNG(name); }
      }
      img++; name = QString("%1.png").arg(img); pplane->savePNG(name);
      delete F; delete MMpath; delete Mpath;
      if (ftype == 65) ftype = 45; if (ftype == 5) ftype = 15;
   }//veinAct3

   /*if (act == slowmateAct && ftype == 15) //5 steps from nmax, and both planes
   {  int img, i, k, p = 6, P, sym = 0; const int imax = 32;
      mdouble rc, ic, rd, id, x[imax], y[imax]; QmnPlane plane[2*imax];
      qulonglong N1, N2, N, D, N0, D0; QString name = QString("1025.png");
      if (ftype == 5)
      { P = 0; rc = 0.0L; ic = 0.0L; rd = 4.5L; id = 0.0L; sym = 1; } 
      if (ftype == 15)
      { P = 1; rc = 1.0L; ic = 0.0L; rd = 2.05L; id = 0.0L; sym = 1; } 
      if (ftype == 25)
      {  P = 2; sym = 1; x[0] = -1.0L; y[0] = 0.0L;
         rc = 0.0L; ic = 0.0L; rd = 2.5L; id = 0.0L;
      }
      if (ftype == 35)
      {  P = 3; if (dplane->getType() < 0) //Rabbit
         {  x[0] = -0.122561166876654L; y[0] = 0.744861766619744L; //R
            rc = -0.33L; ic = 0.7L; rd = 1.41L; id = 0.0L;
         }
         else
         {   x[0] = -1.754877666246693L; y[0] = 0.0L; //A
             rc = 0.781L; ic = 0.0L; rd = 2.0L; id = 0.0L; sym = 1;
             //rc = 0.6612L; ic = 0.2265L; rd = 0.0965L; id = 0.0L; //3/8-limb
             //rc = 0.7178L; ic = 0.0273L; rd = 0.1843L; id = 0.0L; //2/5-limb
         }
      }
      pplane->setPlane(rc, ic, rd, id);
      if (!pplane->loadPNG(name))
      {  draw(redrawAct); QmnUIntDialog *d0 = new QmnUIntDialog(trUtf8(
         "Please check nmax and set grayscale, save as 1025.png, hit d again."
         ), 0, 0, 0u, 4, this, 0);
         connect(d0, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         d0->exec(); return;
      }
      for (i = 0; i < 2*imax; i++)
      { plane[i].resize(thesize, thesize); plane[i].setPlane(rc, ic, rd, id); }
      uint n = 10u, m = 5u; QmnUIntDialog *d1 = new QmnUIntDialog(trUtf8(
         "Enter the preperiod, 5 ... 25 :"), &n, &m, 25u, 4, this, 1);
      connect(d1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!d1->exec()) return; k = n;
      D = 63ULL << k; N1 = 1ULL; N2 = D - N1; //full
      //N1 = (73ULL*D)/255ULL + 1ULL; N2 = (74ULL*D)/255ULL; //3/8
      //N1 = (9ULL*D)/31ULL + 1ULL; N2 = (10ULL*D)/31ULL; //2/5
      if (sym) N2 = D >> 1;
      mndynamics *F = new mndlbrot(2); mandelPath *mpm;
      if (P == 1) mpm = new mandelMate1();
      else mpm = new mandelMating();
      if (P > 1) mpm->setMating(0, P, x[0], y[0], 1); //mpm->setR(exp(1.0L));
      uint IMG; pplane->getNmax(IMG);
      for (img = IMG; img <= 1050; img += 5)
      {  name = QString("1025.png");
         for (i = 0; i < 2*imax; i++) plane[i].loadPNG(name);
         for (N = N1; N <= N2; N++)
         {  N0 = N; D0 = D; k = 0;
            while (!(N0 & 1ULL) && !(D0 & 1ULL)) { N0 >>= 1; D0 >>= 1; }
            while (!(D0 & 1ULL)) { k++; D0 >>= 1; }
            if (!k || dplane->newtonRay(1, N, D, rc, ic, 5, Qt::white, 10) > 1
              || F->find(1, k, p, rc, ic) ) continue;
            if (P) mpm->setMating(k, p, rc, ic, 0);
            else mpm->setMating(k, p, rc, ic, 1);
            if (!P)
            {  if (k <= 1) continue; //here N/D <= 1/2 :
               if (dplane->newtonRay(1, N, D>>1, rc, ic, 5, Qt::white, 10) > 1
                 || F->find(1, k - 1, p, rc, ic) ) continue;
               mpm->setMating(k - 1, p, rc, -ic, 0);
            }
            mpm->init((img - 1025)*0.040001L);
            for (i = 0; i < imax; i++)
            {  mpm->getParameters(rc, ic, rd, id, P);
               if (P == 1 && dplane->getType() < 0)
               { rd = rc*rc + ic*ic; rc /= rd; ic = -ic/rd; }
               if (N > N1)
               {  plane[i].drawLine(x[i], y[i], rc, ic, Qt::red);
                  if (sym) plane[i].drawLine(x[i], -y[i], rc, -ic, Qt::red);
                  double w = x[i]*x[i] + y[i]*y[i]; x[i] /= w; y[i] /= -w;
                  w = rc*rc + ic*ic;
                  plane[imax  + i].drawLine(x[i], y[i], rc/w, -ic/w, Qt::red);
                  plane[imax  + i].drawLine(x[i], -y[i], rc/w, ic/w, Qt::red);
               }
               x[i] = rc; y[i] = ic; if (i < imax - 1) mpm->step(rc, ic);
               if (sym && P && i >= (P == 2 ? 12 : 8) && 3ULL*N >= D) break;
            }
         }
         for (i = 0; i < imax; i++)
         {  name = QString("p/%1.png").arg(img + 25*i); plane[i].savePNG(name);
            name = QString("d/%1.png").arg(img + 25*i);
            plane[i+imax].savePNG(name);
         }
      }
      delete mpm; delete F; return;
   }//modified 15*/

   if (act == limbAct4)
   {  QString name, text = trUtf8(
      "Mate several limbs with:\n"
      "  themselves in family 5.1 with n_max even,\n"
      "anti-self in family 5.1 with n_max odd,\n"
      "the Airplane in family 5.3 with different\n"
      "  parts for n_max even or odd.\n"
      "Video frames are saved in the current directory\n%1\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1001 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Note: the current directory is changed with F6 ... F8."
      ).arg(QDir::toNativeSeparators(QDir::currentPath()));
      QmnUIntDialog *dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
      connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!dialog->exec()) return;
      if (ftype == 15)
      { uint m; pplane->getNmax(m); if (m & 1) ftype = 5; }
      if (ftype == 35)
      { uint m; pplane->getNmax(m); if (m & 1) ftype = 30; }
      mndynamics *F = new mndlbrot(2);
      pplane->setNmax((ftype == 5 || ftype == 15) ? 2000 : 500);
      mndPathMate *Mpath = new mndPathMate(0, 60);
      int img = 1000, k, p, n, j = 1, K, npf;
      qulonglong N0, D0, N, D, N1;
      p = 6; K = 18; N0 = 0ULL; D0 = 63ULL << K; N1 = 21ULL << K;
         npf = (1 << K)/37 + 1; //steps per frame, about 800 frames
      mdouble a, b, x, y, x0, y0, x1, y1;
      //first image (x, 0, a, 0), last (x1, y1, b, 0), mating start x0, y0 
      if (!ftype)
      {  x = -0.5L; x1 = -0.2L; y1 = 0.0L; a = 1.55L; b = 0.335L;
         *themode = 4; x0 = 0.25L; y0 = 0.0L;
      }
      if (ftype == 5 || ftype == 15)
      {  x = 1.0L; x1 = 1.0L; y1 = 0.0L; a = 2.2L; b = 2.05L;
         *themode = 0; x0 = 3.0L; y0 = 0.0L; if (ftype == 15) x0 = 1.0L/x0;
      }
      if (ftype == 35)
      {  x = 0.5703L; x1 = 0.69153L; y1 = 0.38915L; a = 2.2465L; b = 0.55240L;
         *themode = 1; x0 = 0.8445944976806L; y0 = 0.8921881866455L;
         Mpath->mating(0, 3, -1.754877666246693L, 0.0L, logTen, 1);
         K = 21; N0 = 18ULL << K; D0 = 63ULL << K; N1 = 21ULL << K;
         npf = (1 << K)/257 + 1;
         /*/for zoom around indirect capture component
         x1 = -0.248828125L; y1= 0.6552734375L; b = 0.2L;
         K = 20; N0 = 805ULL << (K - 6); D0 = 63ULL << K;
         N1 = 1045ULL << (K - 6); npf = (1 << K)/207 + 1;//*/
         /*/for zoom around one-sided capture component
         x1 = 0.6879L; y1= 0.2040L; b = 0.125L;
         K = 21; N0 = 18ULL << K; D0 = 63ULL << K;
         N1 = N0 + (63ULL << (K - 7)); npf = (1 << K)/1638 + 1;//*/
      }
      if (ftype == 30)
      {  x = 0.5703L; x1 = 1.79846L; y1 = 0.31250L; a = 2.2465L; b = 0.96892L;
         *themode = 1; x0 = 1.474L; y0 = 0.0L;
         Mpath->mating(0, 3, -1.754877666246693L, 0.0L, logTen, 1);
         K = 21;
         D0 = 63ULL << K; N0 = D0/60ULL; N1 = 9ULL << K; npf = (1 << K)/85 + 1;
         /*/for zoom around 3/8-limb
         x0 = 0.74536L; y0 = 0.27374L; x1 = 0.6879L; y1= 0.2040L; b = 0.125L;
         p = 8; K = 20; N0 = 73ULL << K; D0 = 255ULL << K;
         N1 = 74ULL << K; npf = (1 << K)/756 + 1;//*/
      }
      while (img < 1025)
      {
#ifdef SaveFramesOfVeinsLimbsX
         img++;
#else
         img += 25;
#endif
         y = ((mdouble)(img - 1001))/24.0L;
         pplane->setPlane(x + y*(x1 - x), y*y1, a + y*(b - a), 0.0L);
         pplane->draw(f, 1, themode);
         text = trUtf8("Please confirm that the image is drawn completely.");
         if (img == 1025) text += trUtf8("\n\nExpect a short freezing ...");
         dialog = new QmnUIntDialog(text, 0, 0, 0u, 4, this);
         connect(dialog, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         if (!dialog->exec()) return;
         if (ftype == 5 || ftype == 15)
         {  pplane->replaceColor(13, 16);
            for (j = 1; j <= 12; j++) pplane->replaceColor(j, 7);
         }
         else
         {  pplane->replaceColor(10, 8); pplane->replaceColor(0, 7);
            pplane->replaceColor(9, 1); pplane->replaceColor(1, 16);
         }
#ifdef SaveFramesOfVeinsLimbsX
         name = QString("%1.png").arg(img); pplane->savePNG(name);
#endif
      }
      n = 0; while (1)
      {  n++; N0++; if (N0 >= N1) break; 
         k = K; N = N0; D = D0;
         while (!(N & 1ULL) && !(D & 1ULL))
         { k--; N >>= 1; D >>= 1; }
         if (!k) continue;
         if (pplane->newtonRay(1, N, D, x1, y1, 5, Qt::darkMagenta, 10) > 1
             || F->find(1, k, p, x1, y1)) continue;
         if ((ftype == 5 && img <= 30) || (ftype == 15 && img <= 30)) { x1 = x0; y1 = y0; }
         else if (ftype == 5 || ftype == 15)
         {  for (j = 0; j <= 1; j++) Mpath->mating(k, p, x1, y1, logTen, j);
            for (j = 1; j <= 60; j++)
               if (Mpath->step(x1, y1)) { j = 0; break; }
            if (ftype == 5) { x = x1*x1 + y1*y1; x1 /= x; y1 /= x; }
         }
         if (ftype >= 30)
         {  Mpath->mating(k, p, x1, y1, logTen, 0);
            for (j = 1; j <= 60; j++)
            if (Mpath->step(x1, y1)) { j = 0; break; }
            //normalize  (z^2 + u+iv) / (z^2 + X+iY)
            mdouble rc = x1, ic = y1, rd = Mpath->rD, id = Mpath->iD,
            u = rd - 1.0L, v = id, X = 1.0L - rc, Y = ic, w = X*X + Y*Y;
            u /= w; v /= w; w = u*X - v*Y; Y = u*Y + v*X; X = w;
            u = rd*rd - id*id; v = 2.0L*rd*id; w=u*u + v*v;
            X /= w; Y /= w; w = X*u + Y*v; Y = Y*u - X*v; X = w;
            w = rd*rd + id*id; u = rc/w; v = ic/w;
            w = u*rd + v*id; v = v*rd - u*id; u = w;
            w = u*X - v*Y; v = u*Y + v*X; u = w;
            //for V_3
            u++; X++; w = X*X + Y*Y; x1 = -(u*X + v*Y)/w; y1 = (u*Y - v*X)/w;
            //if (img >= 1151 && y0 > 0.075L) break;
         }
         if (j)
         {  pplane->drawLine(x0, y0, x1, y1, Qt::red); 
            if (ftype < 30) pplane->drawLine(x0, -y0, x1, -y1, Qt::red);
            x0 = x1; y0 = y1;
         }
         if (!(n % npf))
         { img++; name = QString("%1.png").arg(img); pplane->savePNG(name); }
      }
      //pplane->drawLine(x0, y0, 0.74536L, 0.27374L, Qt::red);
      img++; name = QString("%1.png").arg(img); pplane->savePNG(name);
      delete F; delete Mpath;
      if (ftype == 30) ftype = 35; if (ftype == 5) ftype = 15;
   }//limbAct4

   if (act == slowantiAct)
   {  int img, i, k, P = -1, sym = 0; const int imax = 32;
      mdouble rc, ic, rd, id, x[imax], y[imax]; QmnPlane plane[imax];
      qulonglong N1, N2, N, D; QString name = QString("1025.png");
      if (ftype == 5)
      {  P = 0; sym = 1; rc = 0.0L; ic = 0.0L; rd = 4.5L; id = 0.0L;
         if (dplane->getType() < 0) { rc = 0.0L; rd = 8.0L; }
      }
      if (ftype == 25)
      {  P = 2; sym = 1; rc = 0.0L; ic = 0.0L; rd = 2.5L; id = 0.0L;
         if (dplane->getType() < 0) { rc = 0.8L; rd = 3.8L; }
      }
      if (P < 0) return;
      pplane->setPlane(rc, ic, rd, id);
      if (!pplane->loadPNG(name))
      {  draw(redrawAct); QmnUIntDialog *d0 = new QmnUIntDialog(trUtf8(
         "Video frames of slow parameter anti-mating in family 5.0 or 5.2:\n\n"
         "There is no 1025.png in the current directory\n%1 .\n"
         "Redraw the given part with appropriate nmax and set grayscale,\n"
         "save as 1025.png in an appropriate directory, then hit Ctrl+d again."
         ).arg(QDir::toNativeSeparators(QDir::currentPath())),
         0, 0, 0u, 4, this, 0);
         connect(d0, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         d0->exec(); return;
      }
      for (i = 0; i < imax; i++)
      { plane[i].resize(thesize, thesize); plane[i].setPlane(rc, ic, rd, id); }
      uint n = 8u, m = 5u; QmnUIntDialog *d1 = new QmnUIntDialog(trUtf8(
      "Video frames of slow parameter anti-mating in family 5.0 or 5.2:\n"
      "1026.png ... 1825.png are saved in the current directory\n%1 .\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1026 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Enter the preperiod, 5 ... 15, and expect some freezing:"
      ).arg(QDir::toNativeSeparators(QDir::currentPath())),
         &n, &m, 15u, 4, this, 1);
      connect(d1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!d1->exec()) return; k = n; mndynamics *F; mandelPath *mpm;
      if (!P)
      {  D = 3ULL << (2*k - 1); N1 = 1ULL; N2 = D/6ULL;
         F = new mndmulti(4); mpm = new mandelMate0();
      }
      if (P == 2)
      {  D = 3ULL << 2*k; N1 = 1ULL; N2 = D/6ULL; //limb from 1/12 to 1/6
         F = new mandelQPPQ(2); mpm = new mandelMate2();
      }
      /*for (N = N1; N <= N2; N += 2ULL)
      {  rd = rc; id = ic;
         if (F->mandelRay(1, -150, N, D, &rc, &ic) < 0
             || F->find(1, k, p, rc, ic) || rc*rc + ic*ic < 0.54L)
         { rc = rd; ic = id; dplane->setPoint(0, N);continue; }
         if (mpm->setAnti(2*k - 1, 2*p,
            rc*(rc*rc - 3.0L*ic*ic), (3.0L*rc*rc - ic*ic)*ic))
         { rc = rd; ic = id; dplane->setPoint(N, 0);continue; }
         for (i = 1; i <= imax; i++) mpm->step(rc, ic);
         if (rc < -1.60L) dplane->setPoint(rc, N);
         if (N == N1) continue;
         pplane->drawLine(rd, id, rc, ic, Qt::red);
         if (sym) pplane->drawLine(rd, -id, rc, -ic, Qt::red);
      }//*/
      for (img = 1026; img <= 1050; img++)
      {  name = QString("1025.png");
         for (i = 0; i < imax; i++) plane[i].loadPNG(name);
         for (N = N1; N <= N2; N += 2ULL)
         {  if (F->mandelRay(1, -150, N, D, &rc, &ic) < 0
             || F->find(1, k, 1, rc, ic) || rc*rc + ic*ic < 0.22L
             || mpm->setAnti(2*k - P/2, 2,
                  rc*(rc*rc - 3.0L*ic*ic), (3.0L*rc*rc - ic*ic)*ic))
               continue;
            mpm->init((img - 1025)*0.040001L);
            for (i = 0; i < imax; i++)
            {  mpm->getParameters(rc, ic, rd, id, P);
               if (N > N1)
               {  plane[i].drawLine(x[i], y[i], rc, ic, Qt::red);
                  if (sym) plane[i].drawLine(x[i], -y[i], rc, -ic, Qt::red);
               }
               x[i] = rc; y[i] = ic; if (i < imax - 1) mpm->step(rc, ic);
            }
         }
         for (i = 0; i < imax; i++)
         { name = QString("%1.png").arg(img + 25*i); plane[i].savePNG(name); }
      }
      delete mpm; delete F;
   }

   if (act == slowmateAct)
   {  int img, i, k, p = 6, P = -1, sym = 0; const int imax = 32;
      mdouble rc, ic, rd, id, x[imax], y[imax]; QmnPlane plane[imax];
      qulonglong N1, N2, N, D, N0, D0; QString name = QString("1025.png");
      if (ftype == 5)
      { P = 0; rc = 0.0L; ic = 0.0L; rd = 4.5L; id = 0.0L; sym = 1; } 
      if (ftype == 15)
      { P = 1; rc = 1.0L; ic = 0.0L; rd = 2.05L; id = 0.0L; sym = 1; } 
      if (ftype == 25)
      {  P = 2; sym = 1; x[0] = -1.0L; y[0] = 0.0L;
         rc = 0.0L; ic = 0.0L; rd = 2.5L; id = 0.0L;
      }
      if (ftype == 35)
      {  P = 3; if (dplane->getType() < 0) //Rabbit
         {  x[0] = -0.122561166876654L; y[0] = 0.744861766619744L; //R
            rc = -0.33L; ic = 0.7L; rd = 1.41L; id = 0.0L;
         }
         else
         {   x[0] = -1.754877666246693L; y[0] = 0.0L; //A
             rc = 0.781L; ic = 0.0L; rd = 2.0L; id = 0.0L; sym = 1;
             //rc = 0.6612L; ic = 0.2265L; rd = 0.0965L; id = 0.0L; //3/8-limb
             //rc = 0.7178L; ic = 0.0273L; rd = 0.1843L; id = 0.0L; //2/5-limb
         }
      }
      if (P < 0) return;
      pplane->setPlane(rc, ic, rd, id);
      if (!pplane->loadPNG(name))
      {  draw(redrawAct); QmnUIntDialog *d0 = new QmnUIntDialog(trUtf8(
         "Video frames of slow parameter mating in family\n"
         "5.0 with \xce\xb8 v -2\xce\xb8,\n"
         "self-mating in family 5.1, self anti-mating with sphere F3,\n"
         "mating with the Basilica in 5.2, and\n"
         "Airplane in family 5.3, Rabbit with sphere F3.\n\n"
         "There is no 1025.png in the current directory\n%1 .\n"
         "Redraw the given part with appropriate nmax and set grayscale,\n"
         "save as 1025.png in an appropriate directory, then hit d again."
         ).arg(QDir::toNativeSeparators(QDir::currentPath())),
         0, 0, 0u, 4, this, 0);
         connect(d0, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         d0->exec(); return;
      }
      for (i = 0; i < imax; i++)
      { plane[i].resize(thesize, thesize); plane[i].setPlane(rc, ic, rd, id); }
      uint n = 10u, m = 5u; QmnUIntDialog *d1 = new QmnUIntDialog(trUtf8(
      "Video frames of slow parameter mating in family\n"
      "5.0 with \xce\xb8 v -2\xce\xb8,\n"
      "self-mating in family 5.1, self anti-mating with sphere F3,\n"
      "mating with the Basilica in 5.2, and\n"
      "Airplane in family 5.3, Rabbit with sphere F3.\n\n"
      "1026.png ... 1825.png are saved in the current directory\n%1 .\n"
      "Make the video with the shell command\n"
      "ffmpeg -start_number 1026 -i %04d.png -c:v libx264 "
      "-pix_fmt yuv420p out.mp4\n"
      "Enter the preperiod, 5 ... 25, and expect some freezing:"
      ).arg(QDir::toNativeSeparators(QDir::currentPath())),
         &n, &m, 25u, 4, this, 1);
      connect(d1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!d1->exec()) return; k = n;
      D = 63ULL << k; N1 = 1ULL; N2 = D - N1; //full
      //N1 = (73ULL*D)/255ULL + 1ULL; N2 = (74ULL*D)/255ULL; //3/8
      //N1 = (9ULL*D)/31ULL + 1ULL; N2 = (10ULL*D)/31ULL; //2/5
      if (sym) N2 = D >> 1;
      mndynamics *F = new mndlbrot(2); mandelPath *mpm;
      if (P == 0) mpm = new mandelMating();
      if (P == 1) mpm = new mandelMate1();
      if (P == 2) mpm = new mandelMate2();
      if (P == 3)
      { mpm = new mandelMating(); mpm->setMating(0, P, x[0], y[0], 1); }
      for (img = 1026; img <= 1050; img++)
      {  name = QString("1025.png");
         for (i = 0; i < imax; i++) plane[i].loadPNG(name);
         for (N = N1; N <= N2; N++)
         {  N0 = N; D0 = D; k = 0;
            while (!(N0 & 1ULL) && !(D0 & 1ULL)) { N0 >>= 1; D0 >>= 1; }
            while (!(D0 & 1ULL)) { k++; D0 >>= 1; }
            if (!k || F->mandelRay(1, -150, N, D, &rc, &ic) < 0
                || F->find(1, k, p, rc, ic) ) continue;
            if (P) mpm->setMating(k, p, rc, ic, 0);
            else
            {  mpm->setMating(k, p, rc, ic, 1); //here N/D <= 1/2 :
               if (k <= 1 || F->mandelRay(1, -150, N, D >> 1, &rc, &ic) < 0
                   || F->find(1, k - 1, p, rc, ic) ) continue;
               mpm->setMating(k - 1, p, rc, -ic, 0);
            }
            mpm->init((img - 1025)*0.040001L);
            for (i = 0; i < imax; i++)
            {  mpm->getParameters(rc, ic, rd, id, P);
               if (P == 1 && dplane->getType() < 0)
               { rd = rc*rc + ic*ic; rc /= rd; ic = -ic/rd; }
               if (N > N1)
               {  plane[i].drawLine(x[i], y[i], rc, ic, Qt::red);
                  if (sym) plane[i].drawLine(x[i], -y[i], rc, -ic, Qt::red);
               }
               x[i] = rc; y[i] = ic; if (i < imax - 1) mpm->step(rc, ic);
               if (sym && P && i >= (P == 2 ? 12 : 8) && 3ULL*N >= D) break;
            }
         }
         for (i = 0; i < imax; i++)
         { name = QString("%1.png").arg(img + 25*i); plane[i].savePNG(name); }
      }
      delete mpm; delete F;
   }//*/

   /*if (act == slowmateAct) //segmentation fault ...
   {  int ilist[4]; ilist[3] = thesize; //P, sym, imgno, size
      mdouble mdlist[3]; qulonglong qulist[3]; //D, N1, N2
      QString name = QString("1025.png");
      if (ftype == 5)
      {  ilist[0] = 0; ilist[1] = 1;
         mdlist[1] = 0.0L; mdlist[2] = 0.0L; mdlist[0] = 4.5L;
      } 
      if (ftype == 15)
      {  ilist[0] = 1; ilist[1] = (dplane->getType() < 0 ? -1 : 1); //anti
         mdlist[1] = 1.0L; mdlist[2] = 0.0L; mdlist[0] = 2.05L;
      } 
      if (ftype == 25)
      {  ilist[0] = 2; ilist[1] = 1;
         mdlist[1] = 0.0L; mdlist[2] = 0.0L; mdlist[0] = 2.5L;
      }
      if (ftype == 35)
      {  ilist[0] = 3; if (dplane->getType() < 0) //Rabbit
         {  ilist[1] = -1;
            mdlist[1] = -0.33L; mdlist[2] = 0.7L; mdlist[0] = 1.41L;
         }
         else
         {  ilist[1] = 0; //Airplane
            mdlist[1] = 0.781L; mdlist[2] = 0.0L; mdlist[0] = 2.0L; ilist[1]=1;
          //mdlist[1] = 0.6612L; mdlist[2] = 0.2265L; mdlist[0] = 0.0965L;//3/8
          //mdlist[1] = 0.7178L; mdlist[2] = 0.0273L; mdlist[0] = 0.1843L;//2/5
         }
      }
      pplane->setPlane(mdlist[1], mdlist[2], mdlist[0], 0.0L);
      if (!pplane->loadPNG(name))
      {  draw(redrawAct); QmnUIntDialog *d0 = new QmnUIntDialog(trUtf8(
         "Please check nmax and set grayscale, save as 1025.png, hit d again."
         ), 0, 0, 0u, 4, this, 0);
         connect(d0, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
         d0->exec(); return;
      }
      uint n = 10u, m = 5u; QmnUIntDialog *d1 = new QmnUIntDialog(trUtf8(
         "Enter the preperiod, 5 ... 25 :"), &n, &m, 25u, 4, this, 1);
      connect(d1, SIGNAL(needHelp(int)), helptabs, SLOT(showPage(int)));
      if (!d1->exec()) return;
      qulist[0] = 63ULL << n; qulist[1] = 1ULL; qulist[2] = qulist[0] - 1ULL;
      //N1 = (73ULL*D)/255ULL + 1ULL; N2 = (74ULL*D)/255ULL; //3/8
      //N1 = (9ULL*D)/31ULL + 1ULL; N2 = (10ULL*D)/31ULL; //2/5
      for (m = 0; m <= 4; m++)
      {  ilist[2] = 1026 + 5*m; //if (mani[m]) delete mani[m];
         mani[m] = new mandelSlow(this); mani[m]->init(ilist, mdlist, qulist);
      }
   }//*/
}

/*
1/12 v 5/12 fix -(1+i)(1+sqrt0.5) = -1.7071067811865475244(1+i), d ~ 0.0017
1/6 v 1/6 c = 0.25 +0.661437827766148 i  z = -0.5 +1.322875655532295 i, d ~ 0.014
3/14 v 3/14: c = .5L + 1.3228756555322952953L i , cr fix = -c, d ~0.005
53/60 v 29/60: a = (-1-i-sqrt(-4+2i)/2 = -0.742934135878323 -1.529085513635746 i, d ~ 0.025 
*/

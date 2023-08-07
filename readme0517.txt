Mandel 5.17 of July 22, 2019. Copyright (C) 2007-2019 Wolf Jung.
jung@mndynamics.com   www.mndynamics.com

This program is free software;  you can redistribute it and / or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation;  either version 3 of
the License,  or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but without any warranty;  without even the implied warranty of
merchantability or fitness for a particular purpose.  See the
GNU Lesser General Public License for more details,  www.fsf.org .

Mandel serves to draw and to research the Mandelbrot set and related
sets from complex dynamics.  It is based on the platform-independent
c++ toolkit Qt by The Qt Company,  see  www.qt.io/download-open-source/ .
Mandel is distributed from  www.mndynamics.com  in two ways:

A) Compilation from the source code (Linux,  Windows,  Mac):

   0) Download a free edition of  Qt  4.x  or  5.x  for Linux,  Windows,
   or Mac from  http://www.qt.io/download-open-source/ . Linux users with KDE 4
   will have it already.  Use the binary installer or the online installer.
   Or unpack the source code and follow the instructions in the file INSTALL.

   1) Download  www.mndynamics.com/mandel0517src.zip  and unzip it to
   a directory,  e.g.,  "mandel" .  You will have the following files:
   readme0517.txt                               (this file)
   copying.txt                                  (the GNU GPL v. 3)
   mandel.pro                                   (Qt project file)
   mndynamo.h  mndynamo.cpp  mndlbrot.cpp  mndcombi.cpp
   qmnplane.h  qmnplane.cpp
   qmnshell.h  qmnshell.cpp  qmndemos.cpp  qmndialg.cpp
   qmnicons.rc qmnicon32.ico                    (Windows icon)
   mandel_de.ts   mandel_pl.ts   mandel_pt.ts   (translation source)
   mandel_de.qm   mandel_pl.qm   mandel_pt.qm   (translation binary)
   qt_de.qm       qt_pl.qm       qt_pt.qm       (translation binary)

   2a) In a shell,  create the Makefile with  "qmake" , then use  "make" .
   For MinGW on Windows,  move  *.qm  to the directory  mandel\release .
   On MacOS X,  qmake  may be configured to generate an Xcode project
   file instead of a Makefile.  Open  mandel.xcodeproj  from the Finder,
   which will start Xcode,  and select "Build" from the Build-menu.

   2b) Or,  if you are using QtCreator,  open  mandel.pro , adjust the
   project settings (e.g., from debug to release),  and build it.

   3) Run "mandel" , "./mandel" , "release\mandel" or "open mandel.app".
   You may click or double-click it in  Konqueror / Explorer / Finder
   as well,  or put a shortcut onto the desktop.

B) Installation of the binary distribution (Windows):

   1) Download  www.mndynamics.com/mandel0517win.zip  and unzip it to
   a directory, e.g.,  "mandel" .  You will have the following files:
   readme0517.txt                               (this file)
   copying.txt                                  (the GNU GPL v. 3)
   mandel.exe                                   (the executable)
   mandel_de.qm   mandel_pl.qm   mandel_pt.qm   (translation binary)
   qt_de.qm       qt_pl.qm       qt_pt.qm       (translation binary)
   QtCore4.dll    QtGui4.dll                (dynamic link libraries)
   mingwm10.dll   libgcc_s_dw2-1.dll        (dynamic link libraries)

   2) Start the executable  "mandel.exe"  by double-clicking it in the
   Explorer.  Or put a shortcut onto your desktop:  single-click
   mandel.exe , create a shortcut from the context menu,  drag it onto
   the desktop.  You may edit the properties of the shortcut to set
   the language,  e.g.   "C:\...\mandel.exe" en   for English.

Redistribution and modification:

If you redistribute  Mandel  or have it on a local network,  please
accompany it with this file and with  copying.txt . If you distribute
a modified version,  please observe the licensing conditions (GPL):
*  Distribute the complete source code under the GPL.
*  In the top of each source code file,  and in  QmnShell::about() ,
   state that it is based on my work,  and that you have modified it.
*  Say that there is no warranty.

To update the translations,  when the English strings have changed:
Run  "lupdate mandel.pro"  to update the  *.ts  files,  update the
translations in these files manually with  Qt Linguist ,
and run  "lrelease mandel.pro"  to update the  *.qm  files.

The source code is explained in the three header-files.
Note that the program consists of three layers:
1) The mathematical class library  mndynamics .
2) The graphical representation by  QmnPlane  and  QmnDraw .
3) The user interface  QmnShell , QmnHelp , and  QmnDemo .
You might modify  QmnShell , or write your own user interface using
QmnPlane  or  QmnDraw , or translate these to a different graphics
toolkit,  or even use  mndynamics  in a console application that is
writing graphics files.  Note that by the GPL you must distribute
the complete source code in these cases as well.

Known bugs:
* Often a window needs to be resized manually to see all of the contents
  without scrolling,  or to remove wide gray borders.
* The command g for equipotential lines is producing artefacts sometimes.
  It was not working with Qt 4.8.0 - 4.8.3,  probably due to QTBUG-26013.
* Sometimes hitting the Go-button of a demo animation in fast repetition
  results in black background.
* Laminations may be unclean.
* Saving png does not ask to confirm overwriting,  if the name is given
  without suffix and the directory changed.
* The pixel size on the sphere is not adjusted for distance estimate and Marty
  normality.  The Julia set gets thinner at the boundary of the visible diks,
  which may be ok,  but it is thinner at infinity as well.  This should be
  compensated by multiplying  temp[1] = pw  with  (1 + |z|^2)^2 / 4 .
* The stripping algorithm does not work for
   1-3-6-7-10-11-14-15-18-19-22-23-26-27-30-31-34-35-39

Plans for Mandel 6:
* Better documentation.
* The two planes contain different copies of mndynamics,  avoiding
  synchronization and allowing more automatic drawing.
* Synchronize local similarity by signals.
* Use int instead of uint,  remove complex.
* New sleeping thread,  including multi-pass drawing without background erasing
  and permanent rays / equipotentials.  Rewrite  green().
* Remove comparison,  smooth coloring,  combine distance estimate and Marty.
  Sphere reports adapted pixel size. But maybe comparison is needed for
  renormalization and transcendental functions.
  Add drawmode 10, 11 in the dynamics,  and drawmodes 4, 5 in mndmulti.
* Background 0 white,  rays and spiders black,  Thurston path red / magenta.
* Rays,  finding,  and ^q for standard slice in more families.
* 1 is graph, 2 Mandel and Multi,  3 and 4 contains Mulitbrot and BF as well.
* Include graph types of the plane,  add maybe distorted sphere.
* Improve sphere by shaded lines and changing colors,  maybe cache all sides.
* Mouse modes include shifting and polygonal path.  Path with long double may
  be mapped and doubled before pulling it back.  Circular path is specified.
  Mouse-drawn frame is independent of cursor position.  Cursor 9 x 9 or more.
  Context menu only for mouse properties and drawing.
* Filling,  movable color eraser,  magnified pixels,  maybe blinking cursor.
* Improved animations.
* Free resizing.
* Make functions in  mndsurge  continuous,  add Newton with Siegel cycle.
* Extend  mndynamics::bifurcate()  to  more functions.  For  mndlbrot ,
  improve the code for finding satellite centers by the next order
  of the multiplier map,  and move it from  QmnShell  to  mndynamics .
* Add real iteration (road map,  cobweb graphics) from logistic.exe ,
  and display the mapping of angles according to combinatorial surgery.
* Add Hubbard trees,  inverse spider,  and Lavaurs' algorithm.
* Rewrite demos,  maybe with scripting and glossary.

Major changes in version 5.18 of ..., 2020:
* Mating of arbitrary quadratic polynomials.
* Update translations and documentation.
* Re-activate Thurston for quadratic polynomials,  remove examples.
* Implemented more rational families,  mating,  and captures by path.
* Split commands Q and Ctrl+Q.

Major changes in version 5.17 of July 22, 2019:
* Changed colors of mating once again.
* Removed Feigenbaum tuning,  renamed shortcuts.

Major changes in version 5.16 of December 1, 2018:
* F9 gives a zoom video now when lsppp > 0 and gamma >= 0.
* Renamed shortcuts for levels in asymptotic similarity.
* Combined  close  of main window and  exit .

Major changes in version 5.15 of December 20, 2017:
* Replaced  double  with  mdouble=long double . Still need to check whether
  QmnDoubleDialog  does not round the given value upon Cancel.
* Renamed shortcuts,  split menu and help page.  Note that
  Ctrl+Fi  may change the workspace instead on Linux.  Alt+Fi  or
  Ctrl+Alt+Fi  would be worse. (The latter is mended with  Ctrl+Alt+F7.)
* Removed affine renormalization.

Major changes in version 5.14 of December 30, 2016:
* Corrected a command line for ffmpeg.
* Changed colors of mating.
* Preimage of path is chosen from the starting point.

Major changes in version 5.13 of August 18, 2015:
* Save both planes in one image.  Remember current directory.
* Automatic images for video frames.  https://www.ffmpeg.org/
ffmpeg -start_number 1001 -i %04d.png -c:v libx264 -pix_fmt yuv420p out.mp4
* Mating and anti-mating in family 5.2 ,  added corresponding quartic family.
* Rearranged rational maps.

Major changes in version 5.12 of October 29, 2014:
* Enable zooming into a frame when step is active.
* Removed rescaled rays,  added affine renormalization.
* Laminations.  Renamed shortcuts.

Major changes in version 5.11 of July 30, 2014:
* Command for changing colors.
* Menu for saving *.eps.
* Draw entropy graph over M.
* Drawing precritical points with mode 9.
* New families mndnewtonpara, -qara, -rara.
* Corrected a bug for pages 6-10 of Demo 6.
* Removed zooming in at double-clicks and turning by clicking besides.

Major changes in version 5.10 of January 12, 2014:
* Corrected a bug in local similarity during asymptotic scaling.
* Corrected a bug for pages 7-8 of Demo 4.

Major changes in version 5.9 of July 30, 2013:
* Compute and draw the core entropy / biaccessibility dimension.
* Thurston-Bousch-Barnsley IFS related to Liouville zeros.
* Standard Spider Algorithm.
* Thurston Algorithm with twists and recapture.
* Drawing of a path.  Modified mouse cursor.
* External rays of the tricorn.
* Adapted to Qt 5.

Major changes in version 5.8 of April 5, 2012:
* Local similarity is performed automatically when zooming, e.g.,
  and together with asymptotic similarity.
* Modified Spider Algorithm with a path in moduli space.
* Zoom in and rotate more conveniently with the mouse alone.
* Removed doubling and tracing,  renamed shortcuts.

Major changes in version 5.7 of August 21, 2011:
* Set parameters with command-line arguments.
* Simplified failure detection in the Stripping Algorithm.

Major changes in version 5.6 of February 28, 2011:
* Compute external angles from combinatorics with the Stripping Algorithm.
* Computation of the conjugate angle now works for periods <= 64.
* Angles etc. may be copied from the label of  QmnUIntDialog .
  Strings pasted into a dialog may contain a leading or trailing whitespace.
* Introduced the orbit portrait,  extended QmnPlane::drawOrtho() .
* In  QmnShell , use UTF-8 encoding of  gamma  etc. with escape sequences.

Major changes in version 5.5 of August 15, 2010:
* Introduced the cat map.
* Removed one-window and fullscreen modes,  renamed shortcuts.
* Equipotential lines are drawn for a given potential level.
* Special parabolic drawing for  c = 0.25  and  c = -0.75 .
* Corrected a bug in  mndlbrot::prepare() .
* Enable  backAct  with signal from  QmnPlane .

Major changes in version 5.4 of July 30, 2010:
* In the rational quadratic family 5.2 , an additional parameter may be
  set with q.  From some families,  you may move to this slice with q.
* Changed the coloring of Marty normality.  In 3.1 and 5.2 it represents
  both critical orbits.
* Added the family 5.9 of rotationally symmetric rational mappings.
* Added QmnCombiDialog and discussion of combinatorics with the commands
  s, w, and k,  using  mndAngle  and  mndCombi.
* Added a third method of drawing external rays,  QmnPlane::newtonRay() .
  Enabled the binary notation of angles in  QmnRayDialog.
* Improved dialogs  QmnDoubleDialog  and  QmnUIntDialog  with context help.
  Decimal numbers may be entered with dot,  comma,  or as fractions.
  Returned to displaying numbers with dot even when the locale is comma.
* Renormalization no longer insists on a frame.  Always use a disk instead
  of a rectangle,  compute the radius.
* Local similarity always asks for the period now.  The same commands
  p and Ctrl+p work for asymptotic similarity when a preperiod is entered.
* Removed affine renormalization,  added affinely renormalized rays.
* In asymptotic similarity added custom point,  changed initialization.
* Commands for leaving full-screen mode added to the context menu,
  keep region appears in the main menu as well.
* One command for finding periodic and preperiodic points,  removed
  center command,  renamed shortcuts.
* Added shortcuts for the demos.

Major changes in version 5.3 of October 3, 2009:
* Introduced the demos 1 - 8 and a fairy tale.
* Added quartic Newton mappings with critical relation,
  and the singular perturbation of quadratic polynomials.
* Added non-analytic mappings:  real cubic,  Tricorn,  Henon.
* Renormalization with preperiod displays embedded Julia sets,  now magenta.
* Dynamics on the Riemann sphere,  external rays with backwards iteration.
  QmnPlane  has sphere,  complex plane,  real plane,  or graph.
* Added the command s,  combining e and . or , .
* Added the command q in 3.2 - 3.7 .
* Added escape time with rainbow colors,  and Marty normality,  renumbered.
* The full-screen mode works for different aspect ratios,  preferably with
  sizes divisible by 8.  Enabled F5 in 2-window and F4 in full-screen mode.
* Local similarity with two windows does not go to the dynamic plane now.
* Affine renormalization in the dynamics does change the parameter only
  in  f , not in the parameter plane.
* Polish translation (by Adam Majewski,  fraktal.republika.pl).
* Added shortcuts for the new-menu.
* Use comma as the decimal separator if that is the system locale.
  I would prefer a period,  but this is useful to exchange data via
  the clipboard with,  e.g.,  a calculator.
* Replaced  crpv  with  critical() , removed  Iterate()  from  mndsiegel.
* Added  QmnPlane::drawLine()  and  QmnPlane::drawEllipse() .
* Added transparency to  QmnDraw  and a QPixmap cursor to  QmnPlane .
* QmnPlane no longer emits  finished()  when it was stopped.

Major changes in version 5.2 of May 17, 2008:
* Resizing with F3 is available in two-window-mode as well.
* Translation files are loaded according to the system locale or
  the command-line argument.  Translations into German and into
  Portuguese (by Atractor,  www.atractor.pt).
* Added writing of 16-color *.eps files.  The depth of colors is
  chosen according to the algorithm of drawing.  Renamed shortcuts.

Major changes in version 5.1 of March 14, 2008:
* Added the command Ctrl+f for Feigenbaum rescaling,  renamed shortcuts.
* Added the command z for smooth zooming.
* In QmnShell::setTL(),  moved  draw()  before  setPoint() .
* In QmnDraw,  close the painter p in-between,  use the signal  drawn()
  and the slot  display()  in QmnPlane.  Replaced QPixmap with QImage.
* In QmnPlane,  changed the window attribute to disable implicit
  double buffering (PaintOnScreen was not sufficient).
* In QmnPlane,  corrected the size when marking a rectangle,
  removed updateRubberBand().

Major changes in version 5.0 of September 9, 2007:
* First GUI version,  based on a former DOS-program.

The program and its documentation have benefitted from inspiring
discussions and useful suggestions from several people.  I wish to thank
the Atractor team,  Krzysztof Baranski,  Laurent Bartholdi,  Arnaud Cheritat,
Evgeny Demidov,  Christian Henriksen,  Hiroyuki Inou,  Khudoyor Mamayusupov,
Adam Majewski,  Michael Mertens,  Sabyasachi Mukherjee,  Robert Munafo,
David Pfrang,  Bernhard Reinke,  Johannes Riedl,  Miguel Romera,
Dierk Schleicher,  Marshall Stoner,  Tan Lei,  and Giulio Tiozzo.

I hope that you will enjoy  Mandel  and find it useful.  Your feedback to
jung@mndynamics.com  is appreciated.  Wolf Jung

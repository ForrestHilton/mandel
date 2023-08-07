/* qmndialg.cpp  by Wolf Jung (C) 2007-2019.  Defines classes:
   QmnHelp, QmnUIntDialog, QmnDoubleDialog, QmnCombiDialog, QmnRayDialog.

   These classes are part of Mandel 5.17, which is free software; you can
   redistribute and / or modify them under the terms of the GNU General
   Public License as published by the Free Software Foundation; either
   version 3, or (at your option) any later version. In short: there is
   no warranty of any kind; you must redistribute the source code as well.
*/

#include <QtGui>
#if (QT_VERSION >= 0x050000)
#include <QtWidgets>
#endif
#include <cmath>
#include "qmnshell.h"

//note: when inserting more pages, check all calls to showPage()
QmnHelp::QmnHelp(QWidget *parent) : QDialog(parent)
{  setWindowTitle(tr("Mandel: help"));
   setMinimumWidth(560); setMaximumWidth(760); setMinimumHeight(580);
   QFont Font = font();
   if (Font.pointSize() > 0 && Font.pointSize() < 12) Font.setPointSize(12);
   setFont(Font);
   tabs = new QTabWidget(this);
   QTextEdit *p0 = new QTextEdit(this); p0->setHtml(tr(
 "Most actions of the program Mandel can be invoked both via the menu and "
 "via shortcut keys, which are indicated in the menu and in these pages. "
 "As usual, the menu is activated with a mouse click or with the keyboard "
 "(press Alt and the accelerator key). Some actions are available via "
 "mouse moves or in the context menu (right click) as well."
 "<p>"
 "This program serves to draw the Mandelbrot set and Julia sets, and to "
 "explore their mathematical properties, e.g., external rays. See the "
 "Demos in the Help-menu for an introduction to the theory. Consider the "
 "dynamics of quadratic polynomials f<sub>c</sub>(z) = z<sup>2</sup> + c : "
 "the filled Julia set K<sub>c</sub> contains all points z with a bounded "
 "orbit, and the Mandelbrot set M contains those parameters c, such that "
 "K<sub>c</sub> is connected."
 "<p>"
 "The program shows the parameter plane with M in the left image, and the "
 "dynamic plane with the Julia set K<sub>c</sub> for the current parameter "
 "c in the "
 "right image. To go to the dynamic plane or back, hit F2 or click into "
 "the inactive window. (When you have magnified a part of the Julia set, "
 "go back and change the parameter, then the new Julia set in the right "
 "window will be shown in full size. Use Shift+F2 to keep the small region, "
 "to compare parts of different Julia sets.) Hit F3 to see the Julia set on "
 "the Riemann sphere, turn it with Ctrl+Arrow keys or drag it with the mouse."
 "<p>"
 "The current point is indicated by a yellow cross. Its coordinates may be "
 "copied to the clipboard. For the parameter plane, the attracting period "
 "is displayed as well (0 means outside of M, 10000 means &gt; 1024). "
 "The current point is meaningful to find special "
 "points, and to zoom in around them. In the parameter plane, it gives "
 "the current parameter c, which defines the corresponding map "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c and the Julia set K<sub>c</sub> in "
 "the dynamic plane. There, the current point z can be mapped to "
 "illustrate the dynamics."
 "<p>"
 "Hit F4 to resize the image. You may need to resize the application window "
 "in addition, by dragging its corner with the mouse. Hit Alt+F4 to quit."
 "<p>"
 "The program tries to use your preferred language. You may specify "
 "another language as a command line parameter: en for English, "
 "de for German, pl for Polish, or pt for Portuguese."));
   p0->setReadOnly(true); tabs->addTab(p0, tr("&Basic"));

   QTextEdit *p1 = new QTextEdit(this); p1->setHtml(tr(
 "Create a frame by dragging the mouse diagonally (left button pressed). "
 "When it is approximately square, the program zooms in immediately. "
 "Besides dragging the mouse, you may create the frame and change its size "
 "with the keys + and -. When the frame is created with +, it will be "
 "square automatically, and its center remains unchanged. This is useful "
 "to zoom in around a Misiurewicz point, e.g., repeatedly. In addition, "
 "you may use the keys &lt; and &gt; to adjust the vertical size, and the "
 "Arrow keys to translate the frame. To zoom in, hit Return, or use the context "
 "menu (right button). Or kill the frame by hitting the Esc key, by creating "
 "another frame, or by setting a point instead (click the left button "
 "without dragging). The frame can be used for saving a part of the image, "
 "and it is used for renormalization as well. Alternatively, "
 "hit the key z repeatedly to zoom in smoothly around the current point."
 "<p>"
 "Before drawing is completed, you may already create a new frame "
 "or give a command invoking some dialog box. Go back once with the "
 "Back key, or go back to the first image with Home. Redraw, e.g., "
 "when drawing was interrupted, or to clear marks, equipotential "
 "lines or external rays. Hit the key v to zoom out by a factor of 2."
 "<p>"
 "After zooming in for a few times, you will see smooth bands instead of "
 "fractal trees. Then increase the maximal number N<sub>max</sub> of "
 "iterations with the key n. (When you see large rectangular pixels, the "
 "limit of possible magnifications is reached.) Hit the Del key to choose "
 "an algorithm for drawing, or use the shortcut keys 0...9. The algorithm "
 "may emphasize the boundary of the Mandelbrot set or Julia set, "
 "respectively, or it may illustrate properties of their interiors or "
 "exteriors. In the algorithms 9, 10, and 11, properties of the polynomial "
 "q<sub>n</sub>(c) or f<sub>c</sub><sup>n</sup>(z) are "
 "shown in addition. Hit the key q to change the order n."
 "<p>"
 "Renormalization means that M contains small copies of itself, and "
 "K<sub>c</sub> may contain copies of another Julia set. See demo 5. "
 "These copies can be illustrated by hitting the key r: the points not "
 "escaping from a disk with some period are marked magenta. For the "
 "parameter plane, the critical orbit is checked. You may restrict drawing "
 "to a frame to gain speed. To mark embedded Julia sets, specify a "
 "preperiod, such that the copy at the critical value has preperiod 0. "
 "Use h to mark the boundary of M according to the core entropy."));
   p1->setReadOnly(true); tabs->addTab(p1, tr("&Draw"));

   QTextEdit *p2 = new QTextEdit(this); p2->setHtml(tr(
 "The current point is set by left-clicking with the mouse, or moved with "
 "the Arrow keys, or its coordinates are typed in after hitting the Ins key "
 "(or pasted from the clipboard). You may use dot or comma as the "
 "decimal separator, or enter a fraction. "
 "Hit the key f to map z &rarr; z<sup>2</sup> + c, hit it repeatedly to "
 "observe the orbit. Use # repeatedly to mark a periodic orbit. Hit the key "
 "Ctrl+f to trace the critical orbit at once, to show convergence to an "
 "attracting or parabolic cycle, or to mark the boundary of a Siegel disk. "
 "Use the keys a and b for the inverse map. (The two branches map "
 "approximately to the parts A and B of the itinerary.) "
 "When the point is outside of the image, you can bring it back with Esc."
 "<p>"
 "In the dynamic plane, periodic and preperiodic points are found with the "
 "key x. These are possible branch points of the Julia set and landing "
 "points of dynamic rays with rational angles. Analogously, centers and "
 "Misiurewicz points in the parameter plane are found with x: these are "
 "the parameters, for which the critical point is periodic or preperiodic. "
 "Each hyperbolic component has a unique center, and when the current point "
 "is within a hyperbolic component, you can move it to the center with the "
 "keys x and Return."
 "<p>"
 "Note that you may use the drawing algorithms 9 and 11 to illustrate "
 "these commands: 11 shows the attracting basins of Newton's "
 "method, which is used to find centers / periodic points, and 9 shows "
 "the location of centers / p.p. The period is set with q. "
 "(Use 9 also to check the displayed period, which might be inaccurate.)"
 "<p>"
 "Bifurcation points on the boundary of a hyperbolic component of period "
 "1, 2, or 3 are found with c. When the argument of the multiplier "
 "is a rational multiple of 2&pi;, the bifurcation point is the root of a "
 "satellite component. Otherwise, it denotes a Siegel or Cremer polynomial."
 "<p>"
 "The bifurcation to the center of a satellite component is obtained with "
 "Ctrl+c (starting from period &le; 512), e.g., period doubling."));
   p2->setReadOnly(true); tabs->addTab(p2, tr("&Points"));

   QTextEdit *p3 = new QTextEdit(this); p3->setHtml(tr(
 "The Boettcher map &Phi;<sub>c</sub>(z) conjugates "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c to F(z) = z<sup>2</sup> "
 "in a neighborhood of infinity. Preimages of circles and "
 "straight rays are called equipotential lines and external rays, "
 "respectively. Analogous curves are defined in the parameter plane "
 "with the map &Phi;<sub>M</sub>(c) := &Phi;<sub>c</sub>(c), i.e., by "
 "evaluating the Boettcher map at the critical value z = c. When the "
 "angle of an external ray is a rational multiple of 2&pi;, the ray "
 "lands at a distinguished boundary point of M or K<sub>c</sub> , "
 "respectively. See demo 3."
 "<p>"
 "Hit the key g to draw the equipotential line through the current "
 "point z (outside of K<sub>c</sub>) or c (outside of M), or to specify the "
 "potential level. Hit e and enter the rational angle as a fraction or in "
 "binary notation to draw an external ray. The dialog box allows to choose "
 "between three methods and to set the accuracy. Normally you can ignore "
 "this and, e.g., hit \"e 1 / 7 Return\"."
 "<p>"
 "Hit the key Ctrl+x to discuss the combinatorics of an angle, to draw the ray, "
 "and to shift the current point to its landing point (or to the center "
 "corresponding to the root, where the ray lands). Hit o to draw "
 "an orbit portrait, which is shown as a lamination when you give the "
 "command from the parameter plane, and Ctrl+o for the precritical lamination. "
 "Use Ctrl+k to determine the angles of the parameter rays bounding the wake "
 "of a limb at the main cardioid. Hit k to discuss the combinatorics of a "
 "kneading sequence or an internal address. See demo 4."));
   p3->setReadOnly(true); tabs->addTab(p3, tr("&Rays"));

   QTextEdit *p4 = new QTextEdit(this); p4->setHtml(tr(
 "The Thurston Algorithm iterates the critical orbit backwards to "
 "compute the parameter c corresponding to a given external angle. "
 "To choose the correct preimages, a connecting path between point "
 "configurations is used by Ch&eacute;ritat for slow mating. The Spider "
 "Algorithm employs rays to infinity. Use s or Ctrl+s to illustrate these "
 "algorithms. With w you may move the critical value along some "
 "path to define a Dehn twist or a recapture. Special examples are "
 "available with Ctrl+w. Hit Return repeatedly to observe the iteration. "
 "Use Home or enter 0 to quit."));
   p4->setReadOnly(true); tabs->addTab(p4, tr("Pa&th"));

   QTextEdit *p5 = new QTextEdit(this); p5->setHtml(tr(
 "The tuning operation corresponding to period doubling is asymptotically "
 "linear at the Feigenbaum point c<sub>F</sub> = -1.401155 with contraction "
 "by &delta; = 4.6692 . Hit the key Shift+f to rescale the image around "
 "c<sub>F</sub> by this factor "
 "(you will have to increase N<sub>max</sub> with the key n as well)."
 "<p>"
 "Consider a Misiurewicz point a with the multiplier &rho;. According to "
 "Tan Lei, the Mandelbrot set M is asymptotically self-similar at a with "
 "the scale &rho;, i.e., &rho;<sup>n</sup>(M - a) converges to an "
 "asymptotic model. Moreover, there is a sequence of centers c<sub>n</sub> "
 "spiraling towards a, such that the corresponding small Mandelbrot sets "
 "M<sub>n</sub> are scaled with &rho;<sup>-2n</sup>. On the intermediate "
 "scales &rho;<sup>-&gamma; n</sup>, there is an asymptotic model for "
 "M - c<sub>n</sub> as well, when &gamma; = 1, 3/2, 7/4 ..."
 "<p>"
 "Hit t to find any Misiurewicz point a, or hit Ctrl+t to choose "
 "a from a menu. Then display rescaled and rotated images according to "
 "the asymptotic self-similarity: hit Return to increase the order by one, "
 "ctrl+l to enter it, and the key l to change the level of rescaling "
 "around small Mandelbrot sets. In the first level, you will see "
 "&rho;<sup>n</sup>(M - a), and in the higher levels, "
 "&rho;<sup>&gamma; n</sup>(M - c<sub>n</sub>) is shown. Hit Home to "
 "turn off rescaling."
 "<p>"
 "In a relatively large neighborhood of the small Mandelbrot sets, H.-O. "
 "Peitgen has observed a local similarity to the dynamic plane, i.e., the "
 "decorations of the small Mandelbrot set are close to the decorations "
 "of the small Julia set after an affine rescaling. Hit the key p to " 
 "see the corresponding part of the Julia set in the right window, or "
 "Ctrl+F8 to see it as a background image in the left one. When you "
 "specify a preperiod, the dynamic plane will be rescaled according "
 "to asymptotic similarity at a Misiurewicz point."
 "<p>"
// "With the command Ctrl+Alt+r, an affine approximation to the renormalization "
// "map is computed, which is not valid for every small Mandelbrot set. "
 "See the demos 6 and 7 for an explanation of these scaling phenomena. "
 "The self-similarity at parabolic parameters is illustrated in demo 2."));
   p5->setReadOnly(true); tabs->addTab(p5, tr("&Scale"));

   QTextEdit *p6 = new QTextEdit(this); p6->setHtml(tr(
 "The Mandelbrot set is based on the one-parameter family of quadratic "
 "polynomials f<sub>c</sub>(z) = z<sup>2</sup> + c. Various other "
 "one-parameter families f<sub>c</sub>(z) are available in the Function-menu. "
 "Most of these families f<sub>c</sub>(z) are constructed such that only "
 "one critical point or critical value is active, which is achieved by "
 "persistent relations between the critical orbits. Some families are "
 "constructed such that there is a persistently neutral cycle, which captures "
 "at least one critical orbit. The command q serves various "
 "aims:<ul>"
 "<li>For the families 1, 2, 4.1, and 8.6 you can change the degree.</li>"
 "<li>When there is a persistently neutral cycle, change the argument of "
 "the multiplier.</li>"
 "<li>In 3.1, the coefficients of a cubic polynomial may be set by choosing "
 "a one-dimensional parameter slice with q. The critical orbits are "
 "independent. Either work with a single map, or observe the escape "
 "locus of a single critical point. The other critical point is used for "
 "odd values of N<sub>max</sub> . Algorithm 4 shows both escape loci.</li>"
 "<li>In 3.2 - 3.7, show the current map in the family 3.1 with "
 "independent critical orbits.</li>"
 "</ul>Analogously to cubic polynomials, quadratic rational maps form "
 "a two-parameter family. One-dimensional slices are chosen from "
 "the family 5.2 with the key q. The current map from family 5.1 or "
 "5.3-5.5 defines such a slice when you hit q. Menu 8 contains simulations "
 "of quasiconformal surgery, and various real-analytic maps."
 "<p>"
 "The commands for drawing and the mapping commands f and Ctrl+f are "
 "available for all families. Some commands for finding special points or "
 "drawing equipotential lines and external rays are available in family 1 "
 "as well. With the possible exception of c, these commands are missing "
 "in most families. At the present time, the scaling commands are available "
 "only for the quadratic family 0, i.e., the Mandelbrot set. You can "
 "always see from the menu, which commands are available."));
   p6->setReadOnly(true); tabs->addTab(p6, tr("&Function"));

   QTextEdit *p7 = new QTextEdit(this); p7->setHtml(tr(
 "Hit F6 or Ctrl+F6 to save the current image as a small EPS file "
 "(Encapsulated "
 "PostScript). It can be included in documents produced with LaTeX and "
 "dvips. You may convert it to a single-page PDF file using epstopdf, e.g., "
 "and include it in a PDF file produced with pdfLaTeX. Depending on the "
 "algorithm of drawing, the EPS file may be black and white, have 4 colors, "
 "or have 16 colors. For algorithm 0, black and white are interchanged. "
 "Change colors by editing the EPS file, by modifying a PNG file, or "
 "with the key F5."
 "<p>"
 "With the key F7, you can save the image in the format PNG, which is "
 "recommended for inclusion in HTML documents on the www, or in ODT files. "
 "Use Ctrl+F7 to save both planes together in one image. "
 "PNG images can be loaded with F8. A serious application may be to draw "
 "puzzle pieces: save an image, use an external drawing program to erase "
 "parts of the external rays and equipotential lines, reload the image, "
 "and save it as EPS."
 "<p>"
 "Save video frames for the Thurston algorithm with F9, stop with ESC. "
 "Convert them to a video with the following shell command:<br>"
 "ffmpeg -start_number 1001 -i %04d.png -c:v libx264"
 " -pix_fmt yuv420p out.mp4<br>"
 "You can add initial images, start later, or delete later images as well."
 "<p>"
 "When you create a frame, F5, F6 and F7 work on the region specified by the "
 "frame. To print an image, please save it and use an external program. "
 "Setting a path with the mouse is turned on or off in the context menu. "
 "The path is cleared with Esc and drawn permanently with #. Use a and b "
 "for its preimages. Hit F10 or Ctrl+F10 to transform the image according "
 "to the Arnold cat map."));
   p7->setReadOnly(true); tabs->addTab(p7, tr("&Image"));

   QTextEdit *p8 = new QTextEdit(this); p8->setHtml(tr(
 "While this help browser explains the commands of the program, the "
 "demos give an introduction to the mathematical background. On "
 "each page they show a text and two images of the parameter plane and the "
 "dynamic plane, respectively. You can perform a single kind of animation "
 "like zooming in, mapping, increasing the order of rescaling. Usage:<br>"
 "Go-button (Return) : perform one step of the animation.<br>"
 "Back-button (PageUp) : go back to the previous page.<br>"
 "Next-button (PageDown) : show the next page of the chapter.<br>"
 "Close-button (Esc) : close the chapter.<br>"
 "Help-button (F1) : display this help page.<br>"
 "Font-button : change the font size, type etc.<br>"
 "On some pages you may set the current point with a mouse click or move "
 "it with the Arrow keys. Note that the Arrow keys and Page keys are not "
 "available for scrolling the text; use Home and End instead."
 "<p>"
 "There are ten chapters available:<br>"
 "Demo 0: introduction to the iteration of real maps.<br>"
 "Demo 1: iteration and attracting cycles, the filled Julia set "
 "K<sub>c</sub> , definition of the Mandelbrot set M.<br>"
 "Demo 2: self-similarity of K<sub>c</sub> at repelling periodic points, "
 "satellite bifurcation and primitive bifurcation, limbs and rotation "
 "numbers, Siegel disks, parabolic implosion.<br>"
 "Demo 3: periodic angles, definition and landing properties of external "
 "rays, bifurcation of landing patterns, correspondence.<br>"
 "Demo 4: external angles, kneading sequences and Hubbard trees.<br>"
 "Demo 5: quadratic-like maps and renormalization, small Julia sets "
 "and small Mandelbrot sets, decorations, embedded Julia sets, "
 "Feigenbaum scaling.<br>"
 "Demo 6: self-similarity of the Mandelbrot set at Misiurewicz points, "
 "and its generalization on multiple scales.<br>"
 "Demo 7: examples and explanation of a local similarity between the "
 "decorations of small Mandelbrot sets and corresponding small Julia "
 "sets.<br>"
 "Demo 8: quasiconformal surgery.<br>"
 "Demo 9: other families of functions.<br>"
 "There is an introductory fairy tale as well.")
 +QString("<br>[Demos 0 and 9 are not available yet.]"));
   p8->setReadOnly(true); tabs->addTab(p8, tr("Dem&o"));

   QPushButton *okButton = new QPushButton(tr("Ok"));
   QPushButton *fontButton = new QPushButton(tr("Font ..."));
   connect(okButton, SIGNAL(clicked()), this, SLOT(hide()));
   connect(fontButton, SIGNAL(clicked()), this, SLOT(changeFont()));
   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addStretch(1);
   hLayout->addWidget(okButton);
   hLayout->addWidget(fontButton);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(tabs);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
}

void QmnHelp::showPage(int index)
{ tabs->setCurrentIndex(index); show(); raise(); }

void QmnHelp::changeFont()
{ setFont(QFontDialog::getFont(0, font())); raise(); }


QmnUIntDialog::QmnUIntDialog(QString text, uint *x, uint *y, uint mode,
   int page, QWidget *parent, int step) : QDialog(parent) // step = 1
{  setAttribute(Qt::WA_DeleteOnClose); setWindowTitle(QString("Mandel"));
#if QT_VERSION >= 0x040100
   setWindowModality(Qt::WindowModal);
#endif
   X = x; Y = y; Mode = mode; Page = page;
   QLabel *label = new QLabel(); label->setFocusPolicy(Qt::NoFocus);
   if (step < 0) label->setWordWrap(true);
   label->setText(text);
#if QT_VERSION >= 0x040200
   label->setTextInteractionFlags(Qt::TextSelectableByMouse);
#endif
   if (Mode && Mode <= 65000u)
   {  spin = new QSpinBox();
      if (*X > Mode) *X = Mode; if (*Y > *X) *Y = *X;
      spin->setRange(qint32(*Y), qint32(Mode)); spin->setSingleStep(step);
      spin->setValue(qint32(*X)); spin->selectAll();
   }
   if (Mode > 65000u)
   {  if (Mode & 1)
         text = QString("^\\s?[0-9]{1,4}/[1-9]{1,1}[0-9]{0,4}\\s?$");
      else text = QString("^\\s?(?:[0-9]{1,4},|)[1-9]{1,1}[0-9]{0,4}\\s?$");
      QRegExpValidator *validator = new QRegExpValidator(QRegExp(text), this);
      edit = new QLineEdit(); edit->setValidator(validator);
      if (Mode & 1) text = QString::number(*X)
                         + QString("/") + QString::number(*Y);
      else if (*X) text = QString::number(*X)
                         + QString(",") + QString::number(*Y);
      else text = QString::number(*Y);
      edit->setText(text);
      if (!edit->hasAcceptableInput()) edit->setText(QString(""));
      connect(edit, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
      connect(edit, SIGNAL(returnPressed()), this, SLOT(inputFinished()));
      edit->selectAll();
   }
   QPushButton *button2, *button3;
   button1 = new QPushButton(tr("Ok"));
   button1->setShortcut(Qt::Key_Return);
   connect(button1, SIGNAL(clicked()), this, SLOT(inputFinished()));
   button1->setEnabled(Mode <= 65000u || edit->hasAcceptableInput());
   button2 = new QPushButton(tr("Cancel"));
   button2->setShortcut(Qt::Key_Escape);
   button2->setEnabled(Mode || step > 0);
   connect(button2, SIGNAL(clicked()), this, SLOT(reject()));
   button3 = new QPushButton(tr("Help ..."));
   button3->setShortcut(Qt::Key_F1);
   connect(button3, SIGNAL(clicked()), this, SLOT(getHelp()));
   button3->setEnabled(Page >= 0);

   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addWidget(button1);
   hLayout->addWidget(button2);
   hLayout->addWidget(button3);
   hLayout->addStretch(1);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(label);
   if (Mode && Mode <= 65000u) vLayout->addWidget(spin);
   if (Mode > 65000u) vLayout->addWidget(edit);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
   show();
}

void QmnUIntDialog::inputChanged()
{ button1->setEnabled(edit->hasAcceptableInput()); }

void QmnUIntDialog::inputFinished()
{  if (Mode && Mode <= 65000u) *X = uint(spin->value());
   if (Mode > 65000u)
   {  QString text = edit->text();
      text = text.replace(QChar(','), QChar('/'));
      if (text.contains(QChar('/')))
      {  QStringList numbers = text.split(QChar('/'));
         *X = numbers.first().toUInt(); *Y = numbers.last().toUInt();
      }
      else { *X = 0; *Y = text.toUInt(); }
   }
   accept();
}

void QmnUIntDialog::getHelp()
{ emit needHelp(Page); }


QmnDoubleDialog::QmnDoubleDialog(QString text, mdouble *x, mdouble *y,
   int page, QWidget *parent) : QDialog(parent)
{  setAttribute(Qt::WA_DeleteOnClose); setWindowTitle(QString("Mandel"));
#if QT_VERSION >= 0x040100
   setWindowModality(Qt::WindowModal);
#endif
   X = x; Y = y; Page = page;
   QLabel *label = new QLabel(); label->setFocusPolicy(Qt::NoFocus);
   label->setText(text);
   QRegExpValidator *validator = new QRegExpValidator(QRegExp(QString(
      "^\\s?-?(?:[0-9]{1,5}"
      "|[0-9]{0,4}\\.[0-9]{1,30}"
      "|[0-9]{0,4},[0-9]{1,30}"
      "|[0-9]{0,4}\\.[0-9]{1,30}e-[0-9]{1,3}"
      "|[0-9]{0,4},[0-9]{1,30}e-[0-9]{1,3}"
      "|[0-9]{1,15}/[1-9]{1,1}[0-9]{0,14})\\s?$")), this);
   //Normally I avoid the exponential notation in the coordinate display,
   //but here it is needed for small potential levels.  The 30 digits cannot
   //be stored accurately, but they allow to paste results from a calculator.
   edit1 = new QLineEdit(); edit1->setValidator(validator);
   if (*X < 1e-5 && *X > -1e-5 && *X != 0.0)
        edit1->setText(QString::number(*X, 'e', 15));
   else edit1->setText(QString::number(*X, 'f', 15));
   if (!edit1->hasAcceptableInput()) edit1->setText(QString("0"));
   if (Y)
   {  edit2 = new QLineEdit(); edit2->setValidator(validator);
      if (*Y < 1e-5 && *Y > -1e-5 && *Y != 0.0)
           edit2->setText(QString::number(*Y, 'e', 15));
      else edit2->setText(QString::number(*Y, 'f', 15));
      if (!edit2->hasAcceptableInput()) edit2->setText(QString("0"));
   }
   connect(edit1, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
   connect(edit1, SIGNAL(returnPressed()), this, SLOT(inputFinished()));
   if (Y)
   {  connect(edit2, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
      connect(edit2, SIGNAL(returnPressed()), this, SLOT(inputFinished()));
   }
   edit1->selectAll();

   QPushButton *button2, *button3;
   button1 = new QPushButton(tr("Ok"));
   button1->setShortcut(Qt::Key_Return);
   connect(button1, SIGNAL(clicked()), this, SLOT(inputFinished()));
   button2 = new QPushButton(tr("Cancel"));
   button2->setShortcut(Qt::Key_Escape);
   connect(button2, SIGNAL(clicked()), this, SLOT(reject()));
   button3 = new QPushButton(tr("Help ..."));
   button3->setShortcut(Qt::Key_F1);
   connect(button3, SIGNAL(clicked()), this, SLOT(getHelp()));
   button3->setEnabled(Page >= 0);

   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addWidget(button1);
   hLayout->addWidget(button2);
   hLayout->addWidget(button3);
   hLayout->addStretch(1);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(label);
   vLayout->addWidget(edit1);
   if (Y) vLayout->addWidget(edit2);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
   show();
}

void QmnDoubleDialog::inputChanged()
{  bool ok = edit1->hasAcceptableInput();
   if (Y) { edit2->setEnabled(ok); if (ok) ok = edit2->hasAcceptableInput(); }
   button1->setEnabled(ok);
}

void QmnDoubleDialog::inputFinished()
{  mdouble x, y; QString text = edit1->text();
   if (text.contains(QChar('/')))
   {  QStringList numbers = text.split(QChar('/'));
      x = numbers.first().toDouble(); y = numbers.last().toDouble();
      x /= y;
   }
   else
   { text = text.replace(QChar(','), QChar('.')); x = text.toDouble(); }
   *X = x;
   if (Y)
   {  text = edit2->text();
      if (text.contains(QChar('/')))
      {  QStringList numbers = text.split(QChar('/'));
         x = numbers.first().toDouble(); y = numbers.last().toDouble();
         x /= y;
      }
      else
      { text = text.replace(QChar(','), QChar('.')); x = text.toDouble(); }
      *Y = x;
   }
   accept();
}

void QmnDoubleDialog::getHelp()
{ emit needHelp(Page); }


//numbersToFraction is just  QString("%1/%2").arg(n).arg(d)
bool QmnCombiDialog::fractionToNumbers(QString text, qulonglong &n,
   qulonglong &d)
{  bool ok; //assumes validation
   QStringList numbers = text.split(QChar('/'));
   n = numbers.first().toULongLong(&ok); if (!ok) return false;
   d = numbers.last().toULongLong(&ok); if (!ok) return false;
   n %= d; while (!(n & 1) && !(d & 1)) { n >>= 1; d >>= 1; }
   return true;
}

void QmnCombiDialog::binaryToNumbers(QString text,
   qulonglong &n, qulonglong &d)
{  bool ok; //assumes validation
   if (!text.contains(QChar('1'))
      || (!text.contains(QChar('0')) && text.contains(QChar('p')) ) )
   { n = 0LL; d = 1LL; return; }
   int k = 0, p = 1; qulonglong K = 0LL, P = 0LL;
   if (text.contains(QChar('p')))
   {  QStringList numbers = text.split(QChar('p'));
      P = numbers.last().toULongLong(&ok, 2); p = numbers.last().length();
      if (!text.startsWith(QChar('p')))
      {K = numbers.first().toULongLong(&ok, 2); k = numbers.first().length();}
   }
   else
   { K = text.toULongLong(&ok, 2); k = text.length(); }
   qulonglong N = 1LL; N <<= p - 1; N--; N <<= 1; N++; // 2^p - 1 , p <= 64
   n = N*K + P; d = N << k;
}

bool QmnCombiDialog::numbersToBinary(qulonglong n, qulonglong d,
   QString &text)
{  int k, p = mndAngle::normalize(n, d, k); if (!p) return false;
   d >>= k; QString t; text.setNum(n / d, 2); t.setNum(n % d, 2);
   if (k) { while (text.length() < k) text.prepend(QChar('0')); }
   else text = QString("");
   while (t.length() < p) t.prepend(QChar('0'));
   t.prepend(QChar('p')); if (p > 1 || !k) text.append(t);
   return true;
}

int QmnCombiDialog::addressToCode(QString text, qulonglong &a)
{  qulonglong u = 1LL; int i, j, m = 1; a = u; //assumes validation
   QStringList numbers = text.split(QChar('-'));
   for (i = 1; i < numbers.size(); i++)
   {  j = m; m = numbers.at(i).toInt();
      if (m <= j || m > 64) return 0;
      a |= u << (m - 1);
   }
   return m;
}

int QmnCombiDialog::codeToAddress(qulonglong a, QString &text)
{  int p; a >>= 1; if (!a) return 0;
   text = QString(QChar('1'));
   for (p = 2; p <= 64; p++)
   {  if (a & 1LL) text.append(QString("-%1").arg(p));
      a >>= 1; if (!a) break;
   }
   return p;
}

QChar QmnCombiDialog::A = QChar('A');
QChar QmnCombiDialog::B = QChar('B');
//The resulting encoded kneading sequence is always ending on 0, so sometimes
//it is the upper, sometimes the lower kneading sequence. This does not matter
//for  mndCombi,  because the upper kneading sequence is determined there.
int QmnCombiDialog::kneadingToCode(QString text, qulonglong &k)
{  k = 1LL; int j, p = text.length(); if (p <= 1) return 0;
   if (text.contains(QChar('1'))) { A = QChar('1'); B = QChar('0'); }
   else { A = QChar('A'); B = QChar('B'); }
   for (j = 1; j < p; j++)
   { k <<= 1; if (QString("aA1").contains(text.at(j))) k |= 1; }
   return p;
}

int QmnCombiDialog::codeToKneading(qulonglong k, QString &text)
{  int p; k >>= 1; if (!k) return 0;
   text = QString(QChar('*'));
   for (p = 2; p <= 64; p++)
   {  if (k & 1LL) text.prepend(A); else text.prepend(B);
      k >>= 1; if (!k) break;
   }
   return p;
}

QmnCombiDialog::QmnCombiDialog(QString text, qulonglong *x,
   qulonglong *y, int page, QWidget *parent)
    : QDialog(parent)
{  setAttribute(Qt::WA_DeleteOnClose); setWindowTitle(QString("Mandel"));
#if QT_VERSION >= 0x040100
   setWindowModality(Qt::WindowModal);
#endif
   X = x; Y = y; Page = page;
   QLabel *label = new QLabel(); label->setFocusPolicy(Qt::NoFocus);
   label->setText(text);

   QRegExpValidator *validator; edit = new QLineEdit();
   if (*X)
   {  validator = new QRegExpValidator(QRegExp(QString(
      "^\\s?(?:[aA1]{1,1}[aA1bB0]{0,62}\\*"
      "|1(\\-[0-9]{1,2}){1,29})\\s?$")), this);
      edit->setMaxLength(80);
   }
   else
   {  validator = new QRegExpValidator(QRegExp(QString(
      "^\\s?(?:[0-9]{1,20}/[1-9]{1,1}[0-9]{0,19}"
      "|p[0-1]{1,64}"
      "|[0-1]{1,63}p[0-1]{1,63}" //at most 64 digits altogether
      "|[0-1]{1,63})\\s?$")), this);
      edit->setMaxLength(65); edit->setText(QString("0/1"));
   }
   edit->setValidator(validator);
   connect(edit, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
   connect(edit, SIGNAL(returnPressed()), this, SLOT(inputFinished()));
   edit->selectAll();

   QPushButton *button2, *button3;
   button1 = new QPushButton(tr("Ok"));
   button1->setShortcut(Qt::Key_Return);
   connect(button1, SIGNAL(clicked()), this, SLOT(inputFinished()));
   button2 = new QPushButton(tr("Cancel"));
   button2->setShortcut(Qt::Key_Escape);
   connect(button2, SIGNAL(clicked()), this, SLOT(reject()));
   button3 = new QPushButton(tr("Help ..."));
   button3->setShortcut(Qt::Key_F1);
   connect(button3, SIGNAL(clicked()), this, SLOT(getHelp()));
   button3->setEnabled(Page >= 0);

   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addWidget(button1);
   hLayout->addWidget(button2);
   hLayout->addWidget(button3);
   hLayout->addStretch(1);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(label);
   vLayout->addWidget(edit);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
   show();
}

void QmnCombiDialog::inputChanged()
{ button1->setEnabled(edit->hasAcceptableInput()); }

void QmnCombiDialog::inputFinished()
{  QString text = edit->text();
   if (text.contains(QChar('*')))
   { kneadingToCode(text, *X); *Y = 0LL; accept(); return; }
   if (text.contains(QChar('-')))
   { *X = 0LL; if(addressToCode(text, *Y)) accept(); else reject(); return; }
   if (!text.contains(QChar('/'))) binaryToNumbers(text, *X, *Y);
   else if (!fractionToNumbers(text, *X, *Y)) { reject(); return; }
   accept();
}

void QmnCombiDialog::getHelp()
{ emit needHelp(Page); }


QmnRayDialog::QmnRayDialog(QString text1, QString text2, qulonglong *x,
   qulonglong *y, uint *method, uint *qp, int page, QWidget *parent)
    : QDialog(parent)
{  setAttribute(Qt::WA_DeleteOnClose); setWindowTitle(QString("Mandel"));
#if QT_VERSION >= 0x040100
   setWindowModality(Qt::WindowModal);
#endif
   X = x; Y = y; Method = method; QP = qp; Page = page;
   if (*X > *Y || *Y > 2) { *X = 2; *Y = 2; }
   spin = new QSpinBox();
   QLabel *label1 = new QLabel(), *label2 = new QLabel();
   label1->setFocusPolicy(Qt::NoFocus); label2->setFocusPolicy(Qt::NoFocus);
   label1->setText(text1); label2->setText(text2);
   if (*QP) { spin->setRange(3, 1024); spin->setValue(qint32(*QP)); }
   else { spin->setRange(2, 10); spin->setValue(5); }

   QRegExpValidator *validator = new QRegExpValidator(QRegExp(QString(
      "^\\s?(?:[0-9]{1,20}/[1-9]{1,1}[0-9]{0,19}"
      "|p[0-1]{1,64}"
      "|[0-1]{1,63}p[0-1]{1,63}" //at most 64 digits altogether
      "|[0-1]{1,63})\\s?$")), this);
   edit = new QLineEdit(); edit->setValidator(validator);
   edit->setMaxLength(65); edit->setText(QString("0/1"));
   connect(edit, SIGNAL(textEdited(QString)), this, SLOT(inputChanged()));
   connect(edit, SIGNAL(returnPressed()), this, SLOT(inputFinished()));
   edit->selectAll();

   QGroupBox *groupBox = new QGroupBox(tr("Method of drawing"));
   radio[0] = new QRadioButton(tr("Backwards iteration"));
   radio[1] = new QRadioButton(tr("Newton method"));
   radio[2] = new QRadioButton(tr("Argument tracing"));
   for (uint i = 0; i <= 2; i++) radio[i]->setEnabled(*X <= i && i <= *Y);
   radio[*X]->setChecked(true);
   QVBoxLayout *vbox = new QVBoxLayout;
   vbox->addWidget(radio[0]);
   vbox->addWidget(radio[1]);
   vbox->addWidget(radio[2]);
   //vbox->addStretch(1);
   groupBox->setLayout(vbox);

   QPushButton *button2, *button3;
   button1 = new QPushButton(tr("Ok"));
   button1->setShortcut(Qt::Key_Return);
   connect(button1, SIGNAL(clicked()), this, SLOT(inputFinished()));
   button2 = new QPushButton(tr("Cancel"));
   button2->setShortcut(Qt::Key_Escape);
   connect(button2, SIGNAL(clicked()), this, SLOT(reject()));
   button3 = new QPushButton(tr("Help ..."));
   button3->setShortcut(Qt::Key_F1);
   connect(button3, SIGNAL(clicked()), this, SLOT(getHelp()));
   button3->setEnabled(Page >= 0);

   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addWidget(button1);
   hLayout->addWidget(button2);
   hLayout->addWidget(button3);
   hLayout->addStretch(1);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(label1);
   vLayout->addWidget(edit);
   if (! *QP) vLayout->addWidget(groupBox);
   vLayout->addWidget(label2);
   vLayout->addWidget(spin);
   if (*QP) vLayout->addWidget(groupBox);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
   show();
}

void QmnRayDialog::inputChanged()
{ button1->setEnabled(edit->hasAcceptableInput()); }

void QmnRayDialog::inputFinished()
{  *QP = uint(spin->value());
   for (uint i = 0; i <= 2; i++) if (radio[i]->isChecked()) *Method = i;
   QString text = edit->text();
   if (!text.contains(QChar('/')))
      QmnCombiDialog::binaryToNumbers(text, *X, *Y);
   else if (!QmnCombiDialog::fractionToNumbers(text, *X, *Y))
   { reject(); return; }
   accept();
}

void QmnRayDialog::getHelp()
{ emit needHelp(Page); }


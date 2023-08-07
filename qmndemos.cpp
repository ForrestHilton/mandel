/* qmndemos.cpp  by Wolf Jung (C) 2007-2023.  Defines classes:
   QmnDemo, QmnDemoDS, QmnDemoPB, QmnDemoER, QmnDemoCC,
   QmnDemoRN, QmnDemoAS, QmnDemoLS, QmnDemoQC, QmnDemoCZ.

   These classes are part of Mandel 5.18, which is free software; you can
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
#define USING_QmnSpider16
#define USING_QmnWitch16
#define USING_QmnRabbit16
#include "qmnshell.h"

const mdouble cFb = -1.40115518909205060052L, dFb = 4.66920160910299067185L,
   aFb = 2.50290787509589282228L;

/*
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<a href=\"javascript:alert('.')\"> </a> "
 "<a href=\"http:// \"> </a> "
*/

QmnDemo::QmnDemo(const QString title, QWidget *parent) : QDialog(parent)
{  setAttribute(Qt::WA_DeleteOnClose);
   setMinimumWidth(780); setMaximumWidth(800); setMinimumHeight(560);
//   setMinimumWidth(1000); setMaximumWidth(1000); setMinimumHeight(700);
//   setMinimumWidth(980); setMaximumWidth(1010); setMinimumHeight(650);
   setWindowTitle(title);
   QFont Font = font();
   if (Font.pointSize() > 0 && Font.pointSize() < 12) Font.setPointSize(12);
   setFont(Font);
   pf = new mndlbrot(3); df = new mndlbrot(3);
   page = 0; points = 0; xp = 0; yp = 0;

   text = new QTextBrowser();
#if QT_VERSION >= 0x040200
   text->setTextInteractionFlags(
      Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
   text->setOpenExternalLinks(false);
#endif
#if QT_VERSION >= 0x040300
   text->setOpenLinks(false);
#endif
   connect(text, SIGNAL(anchorClicked(QUrl)), this, SLOT(openLink(QUrl)));

   pplane = new QmnPlane(360, 360); dplane = new QmnPlane(360, 360, 0);
//   pplane = new QmnPlane(480, 480, 0); dplane = new QmnPlane(480, 480, 0);
   connect(pplane, SIGNAL(moved()), this, SLOT(drawRight()));
   connect(pplane, SIGNAL(finished()), this, SLOT(pFinished()));
   connect(dplane, SIGNAL(finished()), this, SLOT(dFinished()));

   QHBoxLayout *imageLayout = new QHBoxLayout;
   imageLayout->addWidget(pplane);
   imageLayout->addStretch(1);
   imageLayout->addWidget(dplane);

   QActionGroup *moveAG = new QActionGroup(this);
   rightAct = new QAction("&$", moveAG);
   rightAct->setShortcut(Qt::Key_Right);
   leftAct = new QAction("&$", moveAG);
   leftAct->setShortcut(Qt::Key_Left);
   upAct = new QAction("&$", moveAG);
   upAct->setShortcut(Qt::Key_Up);
   downAct = new QAction("&$", moveAG);
   downAct->setShortcut(Qt::Key_Down);
   connect(moveAG, SIGNAL(triggered(QAction*)), this, SLOT(move(QAction*)));
   this->addActions(moveAG->actions());

   QActionGroup *naviAG = new QActionGroup(this);
   homeAct = new QAction("&$", naviAG);
   homeAct->setShortcut(Qt::Key_Home);
   endAct = new QAction("&$", naviAG);
   endAct->setShortcut(Qt::Key_End);
   connect(naviAG, SIGNAL(triggered(QAction*)), this, SLOT(navi(QAction*)));
   this->addActions(naviAG->actions());
   label = new QLabel;
   goButton = new QPushButton(tr("Go!")); goButton->setDefault(true);
   connect(goButton, SIGNAL(clicked()), this, SLOT(go())); //Return key
   backButton = new QPushButton(tr("Back"));
   backButton->setAutoDefault(false); //when focused not Return, only Space
   backButton->setShortcut(Qt::Key_PageUp); //Qt::CTRL+
   connect(backButton, SIGNAL(clicked()), this, SLOT(back()));
   nextButton = new QPushButton(tr("Next"));
   nextButton->setAutoDefault(false);
   nextButton->setShortcut(Qt::Key_PageDown);
   connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
   QPushButton *closeButton = new QPushButton(tr("Close"));
   closeButton->setAutoDefault(false);
   connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));
   QPushButton *helpButton = new QPushButton(tr("Help ..."));
   helpButton->setAutoDefault(false);
   helpButton->setShortcut(Qt::Key_F1);
   connect(helpButton, SIGNAL(clicked()), this, SLOT(getHelp()));
   QPushButton *fontButton = new QPushButton(tr("Font ..."));
   fontButton->setAutoDefault(false);
   connect(fontButton, SIGNAL(clicked()), this, SLOT(changeFont()));
   QHBoxLayout *hLayout = new QHBoxLayout;
   hLayout->addWidget(goButton);
   hLayout->addStretch(1);
   hLayout->addWidget(backButton);
   hLayout->addWidget(label);
   hLayout->addWidget(nextButton);
   hLayout->addStretch(1);
   hLayout->addWidget(closeButton);
   hLayout->addWidget(helpButton);
   hLayout->addWidget(fontButton);
   QVBoxLayout *vLayout = new QVBoxLayout;
   vLayout->addWidget(text);
   vLayout->addLayout(imageLayout);
   vLayout->addLayout(hLayout);
   setLayout(vLayout);
}

void QmnDemo::polyline()
{  if (!points) return;
   if (!n) points--; n++; mdouble t = n/mdouble(steps);
   pplane->setPoint(t*xp[points-1] + (1-t)*xp[points],
      t*yp[points-1] + (1-t)*yp[points]);
   if (n == steps) { n = 0; if (points == 1) goButton->setEnabled(false); }
}

void QmnDemo::move(QAction *act)
{  QmnPlane *theplane = 0;
   if (dplane->isActive()) theplane = dplane;
   if (pplane->isActive()) theplane = pplane;
   if (!theplane) return;
   if (act == rightAct) theplane->move(0, 1, 0);
   if (act == leftAct) theplane->move(0,-1, 0);
   if (act == upAct) theplane->move(0, 0, 1);
   if (act == downAct) theplane->move(0, 0, -1);
}

void QmnDemo::navi(QAction *act)
{  if (act == homeAct) text->verticalScrollBar()->setValue(0);
   if (act == endAct) text->verticalScrollBar()->setValue(
       text->verticalScrollBar()->maximum());
}

void QmnDemo::back()
{  if (page <= 1) return;
   page--; backButton->setEnabled(page > 1);
   nextButton->setEnabled(page < pageMax);
   label->setText(tr("page %1 of %2").arg(QString::number(page)).arg(
      QString::number(pageMax)));
   showPage(true);
}

void QmnDemo::next()
{  if (page >= pageMax) return;
   page++; backButton->setEnabled(page > 1);
   nextButton->setEnabled(page < pageMax);
   label->setText(tr("page %1 of %2").arg(QString::number(page)).arg(
      QString::number(pageMax)));
   showPage(false);
}

void QmnDemo::getHelp()
{ emit needHelp(8); }

void QmnDemo::changeFont()
{ setFont(QFontDialog::getFont(0, font())); raise(); }

void QmnDemo::openLink(const QUrl link)
{  QString string = link.toString();
   if (string.contains("javascript"))
   {  QmnUIntDialog *dialog = new QmnUIntDialog(
         string.remove("javascript:alert('").remove("')"),
         0, 0, 0u, -1, this, -1); //automatic wordWrap
      dialog->exec(); return;
   }
   QmnUIntDialog *dialog = new QmnUIntDialog(tr(
      "<style type=\"text/css\">a:link{text-decoration: none;}</style>"
      "Do you want to open an external link:<br>"
      "<a href=\"%1\">%1</a>").arg(string), 0, 0, 0u, -1, this);
   if (dialog->exec()) QDesktopServices::openUrl(link);
}

/*void QmnDemo::openLink(const QUrl link)
{  QString string = link.toString();
#if QT_VERSION >= 0x040200
   if (string.contains("javascript"))
      QMessageBox::information(this, "Mandel",
         string.remove("javascript:alert('").remove("')"));
   else if (QMessageBox::question(this, "Mandel", tr(
      "<style type=\"text/css\">a:link{text-decoration: none;}</style>"
      "Do you want to open an external link:<br>"
      "<a href=\"%1\">%1</a>").arg(string),
      QMessageBox::Yes | QMessageBox::Default,
      QMessageBox::No | QMessageBox::Escape)
         == QMessageBox::Yes) QDesktopServices::openUrl(link);
#endif
}//*/

////////////////////////////////////////////////////////////////////////

void QmnDemoCZ::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   pplane->move(3); dplane->move(3);
   goButton->setEnabled(true); mode = 16; n = 0;
if (page == 1)
{  text->setHtml(tr(
 "Once upon a time, there was a young witch, Celia. The rabbit Zach lived "
 "in the garden of her house. She loved him very much, but from time "
 "to time, he took a few jumps and ran away. She was poor and could not "
 "afford a fence. So she relied on her witchcraft, raised her wand, and "
 "spoke a spell: \"Functio!\""
 "<p>"
 "Celia watched Zach from the house. He could still jump whenever he liked "
 "to, but he would land at a point determined by the spell. After a few "
 "jumps he would usually come to rest at some place, but when he started "
 "somewhere else, he might still escape."
 "<p>"
 "Celia remembered her teacher Almondo speaking about the geometry of "
 "nature. In her mind, she colored a part of the garden black. When Zach "
 "started his jumps in the black region, he would remain trapped. When he "
 "started from the green region, he could escape."
 "<p>"
 "<i>By clicking into the right window, put Zach at different places in the "
 "garden. Push the Go-Button a few times (or hit the Return key) to watch "
 "Zach jump. Check if Celia imagines the black and green regions "
 "correctly.</i>"));
   if (!backwards)
   {  witch = new QPixmap((const char **)(QmnWitch16));
      rabbit = new QPixmap((const char **)(QmnRabbit16));
      pplane->setCursorPix(witch); dplane->setCursorPix(rabbit);
      connect(dplane, SIGNAL(moved()), this, SLOT(showCoords()));
      pplane->setPlane(-0.8, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
      pplane->setNmax(200); dplane->setNmax(200);
   }
   dplane->setActive(true); pplane->draw(pf, 1, &mode, 0, 15);
   pplane->setPoint(-0.125, 0.5); dplane->draw(df, -1, &mode, 0, 15);
}
if (page == 2)
{  text->setHtml(tr(
 "When Celia stood at a different place in her house, she noticed that Zach "
 "was jumping in a different way now. Often he would jump in a kind of "
 "circle between three places in the garden. Celia watched him to find the "
 "region in the garden, from which he could not escape, and colored it "
 "black in her mind. It looked symmetric again: you could turn it around by "
 "180<sup>o</sup> and it would still be the same. The region had a funny "
 "shape, with many parts attached to one another, looking like a rabbit's "
 "face with two long ears."
 "<p>"
 "Celia wondered, why the spell worked in a different way now."
 "<p>"
 "<i>Again, click into the right window to put Zach at different places in "
 "the garden. Push the Go-Button a few times (or hit the Return key) to make "
 "him jump.</i>"));
   pplane->setPoint(-0.1226, 0.7449); dplane->draw(df, -1, &mode, 0, 15);
   dplane->setActive(true);
}
if (page == 3)
{  text->setHtml(tr(
 "Celia suspected that the spell worked according to her position in "
 "the house: when she went to a different place in the house, Zach would be "
 "jumping in a different way in the garden. She tried a new position and "
 "watched him. Celia was shocked to find that the spell no longer worked as "
 "a fence. Zach was escaping easily, and she could find almost no black "
 "points in the garden, from which he could not escape."
 "<p>"
 "<i>Again, put Zach at different places in the garden, and watch him "
 "jump. (He will move away slowly from the black points as well, because you "
 "cannot hit them precisely.)</i>"));
   pplane->setPoint(-0.85, 1.0); dplane->setActive(true);
}
if (page == 4)
{  text->setHtml(tr(
 "Celia decided to try different positions in her house. She watched Zach "
 "jump around and colored her own position black, if he could not escape "
 "easily. A symmetric figure emerged in the house. When Celia was within a "
 "bulb of this figure, usually Zach was jumping around between some "
 "points that seemed to attract him."
 "<p>"
 "Now Celia understood how the spell Functio worked. In a way she was "
 "trapped herself: she had to stay within the black part of her house, "
 "because otherwise Zach could run away."
 "<p>"
 "<i>Put the witch at different positions by clicking into the left window. "
 "Observe the black region in the garden and make the rabbit jump "
 "(Go-button or Return-key). Check that Zach cannot escape, if Celia is in "
 "the black part of her house.</i>"));
   if (backwards)
   { string->clear(); pplane->draw(pf, 1, &mode, 0, 15); }
   pplane->setPoint(-1, 0); pplane->setActive(true);
}
if (page == 5)
{  *string = tr(
 "Celia was visited by her friend Kartesio, a mathematician. He analysed "
 "her situation:<p>"
 "\"The house is a plane and the garden is a plane, too. You are at the "
 "point c in one plane, and Zach is at the point z in the other plane. "
 "Functio seems to mean that he can jump to the new point z*z + c .\"<p>"
 "\"How do you multiply a point z with itself?\"<p>"
 "\"Let us introduce coordinates x and y, so that the point z is x units "
 "to the right and y units up from 0. Then Zach can jump to the point "
 "with the new coordinates x*x - y*y - 0.1226 and 2*x*y + 0.7449 . Here "
 "-0.1226 and 0.7449 are the coordinates of your position c in the "
 "house.\"<p>"
 "\"I like the formula z*z + c better, it seems easier to analyze.\"<p>"
 "\"Yes, but the formula with x and y is useful, too. A computer can "
 "draw the black figures by computing this formula millions of times.\""
 "<p>"
 "<i>Put the rabbit at different positions and read the coordinates. Or "
 "try to guess the coordinates before clicking into the right window. "
 "Note what happens to x and y, when you move the point z to the right, "
 "left, up, or down.</i>"
 "<p>Now the rabbit sits at x = 0 and y = 0 . "
 "Zach can jump to x = -0.1226 and y = 0.7449 .");
   text->setHtml(*string);
   if (backwards)
   {  pplane->setNmax(200); dplane->setNmax(200);
      pplane->setPlane(-0.8, 0, 1.6, 0);
   }
   pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.1226, 0.7449); dplane->setActive(true);
}
if (page == 6)
{  text->setHtml(tr(
 "With their new formulas, they could compute small details of the black "
 "figure. Celia used the spell \"Reduccio!\" to let herself shrink to the "
 "size of a bacterium. She found a small copy of the whole black figure, "
 "decorated by many branches. Standing in a little black bulb, she noted "
 "that Zach would be jumping around between 48 points that attracted him. "
 "Should something like this be happening for every bulb?"
 "<p>"
 "<i>Push the Go-button repeatedly or hit the Return-key to watch the "
 "details as Celia is shrinking.</i>"));
   if (backwards)
   { pplane->setPlane(-0.8, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0); }
   else { string->clear(); pplane->setNmax(2000); dplane->setNmax(2000); }
   pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.758060762686005, 0.0720118702356124);
}
if (page == 7)
{  text->setHtml(tr(
 "Celia was fascinated by the details of the black figure in the house. She "
 "made Zach shrink as well to watch the details of the black region in the "
 "garden. The spirals in the branches looked similar. This observation "
 "reminded her of her friend Leila, who used to speak of the \"similarity "
 "between the house and the garden\". The whole images would look similar "
 "in fact, when one was rotated by 90<sup>o</sup>."
 "<p>"
 "<i>Push the Go-button repeatedly or hit the Return-key to watch the "
 "details as Celia and Zach are shrinking. Note the similarity.</i>"));
   pplane->setPlane(-0.706310517981, 0.067104747198, 0.13652, 0);
   if (backwards)
   { string->clear(); pplane->setNmax(2000); dplane->setNmax(2000); }
   pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.7580556573488, 0.0720209199120);
   dplane->setPoint(-0.7580556573488, 0.0720209199120);
}
if (page == 8)
{  *string = tr(
 "Celia took a new position in the house, close to the largest bulb in "
 "the black figure. They watched Zach jump around between five points that "
 "seemed to attract him. He always went one step further. So Celia and "
 "Kartesio said that Zach rotated by <sup>1</sup>/<sub>5</sub> . "
 "The number 5 (the denominator) is the total number of jumps before "
 "the same point is reached again, and the number 1 (the numerator) is "
 "the number of steps in every jump (in a counterclockwise direction)."
 "<p>"
 "There are several good reasons to describe this rotation by a fraction. "
 "One reason is that the jumps are similar to a rotation by a fraction of "
 "a full circle, here <sup>1</sup>/<sub>5</sub> of 360<sup>o</sup> or "
 "72<sup>o</sup>. Another reason is that such a rotation happens only when "
 "Celia is in a bulb attached to the biggest bulb, and the fraction would "
 "grow from 0 to 1 when she walked around the biggest bulb."
 "<p>"
 "<i>Click anywhere into the left window to make Celia walk around the big "
 "bulb. Check the rotation by pushing the Go-button repeatedly or "
 "by hiting the Return-key, count the number of steps and of jumps.</i>");
   text->setHtml(*string);
   if (!backwards)
   {  pplane->setNmax(200); dplane->setNmax(200);
      pplane->setPlane(-0.8, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   }
   pplane->draw(pf, 1, &mode); pplane->setActive(true); n = -2; drawRight();
}
if (page == 9)
{  string->clear(); text->setHtml(tr(
 "Celia returned to her favorite position in the house. Kartesio spoke "
 "of his friend Adriano, who worked with rays shining onto the black region "
 "in the garden, coming from a definite angle. They noted that three rays "
 "landed together. These rays had the angles "
 "<sup>1</sup>/<sub>7</sub> of 360<sup>o</sup>, "
 "<sup>2</sup>/<sub>7</sub> of 360<sup>o</sup>, and "
 "<sup>4</sup>/<sub>7</sub> of 360<sup>o</sup>. Somehow a jump of the rabbit "
 "seemed to be connected to doubling the angle, since "
 "2*<sup>1</sup>/<sub>7</sub> = <sup>2</sup>/<sub>7</sub> and "
 "2*<sup>2</sup>/<sub>7</sub> = <sup>4</sup>/<sub>7</sub> . (Note that "
 "2*<sup>4</sup>/<sub>7</sub> = <sup>8</sup>/<sub>7</sub> , but "
 "<sup>8</sup>/<sub>7</sub> of 360<sup>o</sup> is the same direction as "
 "<sup>1</sup>/<sub>7</sub> of 360<sup>o</sup>.)"
 "<p>"
 "Celia and Kartesio drew the corresponding three rays shining onto the "
 "black figure in the house from these angles. Two of these landed "
 "close to Celia's position.<br>"
 "<i>Push the Go-button once or hit the Return-key to draw these rays.</i>"
 "<br>So they had different descriptions of this point:<ul>"
 "<li>The rabbit Zach is jumping from the point z = 0 to another point z "
 "with the coordinates x = -0.1226 and y = 0.7449 in the garden. Celia's "
 "position c in the house has the same coordinates.</li>"
 "<li>Zach is jumping around like a rotation by "
 "<sup>1</sup>/<sub>3</sub> . Celia walked around the big bulb to the "
 "position marked as <sup>1</sup>/<sub>3</sub> .</li>"
 "<li>In the garden, the rays with the angles <sup>1</sup>/<sub>7</sub> and "
 "<sup>2</sup>/<sub>7</sub> of 360<sup>o</sup> land close to Zach's "
 "point z. In the house, the rays with these angles land close to "
 "Celia's position c.</li>"
 "</ul>Since the house was described by looking at the garden, finally they "
 "understood Adriano's famous saying: "
 "\"What you grow in the garden, you eat in the house.\""
 "<p>"
 "The black figure in the house is called the <b>Mandelbrot set</b> by "
 "mathematicians, and the black regions in the garden are called "
 "<b>Julia sets</b>. We have found out many facts about these sets, but "
 "we still cannot answer Celia's question: \"Is there a special bulb in "
 "the Mandelbrot set, such that z can jump around without being attracted "
 "while c is within that bulb?\" Maybe you will become a mathematician "
 "some day, and help us to find out."));
   pplane->setPoint(-0.1226, 0.7449); dplane->setPoint(-0.1226, 0.7449);
}
} //showPage CZ

void QmnDemoCZ::go()
{  if (page <= 6 || page == 8)
   {  mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      df->iterate(x, y); dplane->setPoint(x, y);
      if (page <= 2 && n < 48)
      { n += page; dplane->draw(df, -1, &mode, 1, 5*n + 15); }
   }
   if (page == 6 || page == 7) pplane->draw(pf, 1, &mode, 210);
   if (page == 7) dplane->draw(df, -1, &mode, 210);
   if (page == 9)
   {  mdouble x, y; goButton->setEnabled(false);
      pplane->traceRay(1, 1.0/7.0, pf, x, y, 5);
      pplane->traceRay(1, 2.0/7.0, pf, x, y, 5);
      pplane->traceRay(1, 4.0/7.0, pf, x, y, 5);
   }
}

void QmnDemoCZ::drawRight()
{  mdouble x = 0, y = 0;
   if (page == 4 && n < 24)
   { n++; pplane->move(3); pplane->draw(pf, 1, &mode, 1, 10*n + 15); }
   if (page == 8)
   {  int k = 1, r = 5; n++; if (n == 19) n = 1;
      if (n == 3) r = 4;
      if (n == 5) r = 3;
      if (n == 7) k = 2;
      if (n == 9) r = 2;
      if (n == 11) k = 3;
      if (n == 13) { k = 2; r = 3; }
      if (n == 15) { k = 3; r = 4; }
      if (n == 17) k = 4;
      if (n & 1)
      {  pf->bifurcate(mdouble(k)/mdouble(r), x, y);
         pf->find(1, 0, r, x, y); pplane->setPoint(x, y);
         (*string).append(tr(
            "<p>The rotation number is <sup>%1</sup>/<sub>%2</sub> .").arg(
            k).arg(r));
         text->setHtml(*string);
         if (!n) n = 2; else text->verticalScrollBar()->setValue(
            text->verticalScrollBar()->maximum());
         return;
      }
   }
   pplane->getPoint(x, y); df->setParameter(x, y);
   dplane->setPoint(0, 0); if (page > 2) dplane->draw(df, -1, &mode);
}

void QmnDemoCZ::pFinished()
{  if (page != 5) return;
   pplane->drawLine(-2.2, 0, 0.6, 0);
   pplane->drawLine(0.6, 0, 0.5, 0.1);
   pplane->drawLine(0.6, 0, 0.5, -0.1);
   pplane->drawLine(-2.0, 0, -2.0, -0.04);
   pplane->drawLine(-1.0, 0, -1.0, -0.04);
   pplane->drawLine(0, -1.4, 0, 1.4);
   pplane->drawLine(0, 1.4, 0.1, 1.3);
   pplane->drawLine(0, 1.4, -0.1, 1.3);
   pplane->drawLine(0, 1.0, -0.04, 1.0);
   pplane->drawLine(0, -1.0, -0.04, -1.0);
}

void QmnDemoCZ::dFinished()
{  if (page == 9)
   {  mdouble x, y;
      dplane->traceRay(-1, 1.0/7.0, df, x, y, 5);
      dplane->traceRay(-1, 2.0/7.0, df, x, y, 5);
      dplane->traceRay(-1, 4.0/7.0, df, x, y, 5);
   }
   if (page != 5) return;
   dplane->drawLine(-1.75, 0, 1.75, 0);
   dplane->drawLine(1.75, 0, 1.63, 0.12);
   dplane->drawLine(1.75, 0, 1.63, -0.12);
   dplane->drawLine(1.0, 0, 1.0, -0.05);
   dplane->drawLine(-1.0, 0, -1.0, -0.05);
   dplane->drawLine(0, -1.75, 0, 1.75);
   dplane->drawLine(0, 1.75, 0.12, 1.63);
   dplane->drawLine(0, 1.75, -0.12, 1.63);
   dplane->drawLine(0, 1.0, -0.05, 1.0);
   dplane->drawLine(0, -1.0, -0.05, -1.0);
}

void QmnDemoCZ::showCoords()
{  if (page != 5) return; if (!n) { n = 1; return; }
   mdouble x, y; dplane->getPoint(x, y);
   if (x*x < 2e-4) x = 0; if (y*y < 2e-4) y = 0;
   mdouble u = x, v = y; df->iterate(u, v);
   if (u*u < 2e-4) u = 0; if (v*v < 2e-4) v = 0;
   QLocale locale = QLocale::system();
#if QT_VERSION >= 0x040200
   locale.setNumberOptions(QLocale::OmitGroupSeparator);
#endif
   (*string).append(tr("<p>You put the rabbit at x = %1 and y = %2 . "
      "Zach can jump to x = %3 and y = %4 .").arg(
      locale.toString((odouble)(x), 'g', 4)).arg(
         locale.toString((odouble)(y), 'g', 4)).arg(
      locale.toString((odouble)(u), 'g', 4)).arg(
         locale.toString((odouble)(v), 'g', 4)));
   text->setHtml(*string); text->verticalScrollBar()->setValue(
      text->verticalScrollBar()->maximum());
}

////////////////////////////////////////////////////////////////////////

void QmnDemoDS::showPage(bool backwards)
{/*Change the default initializations on the pages if neccessary,  take
   care of a possible going back. Usually the image of the parameter plane may
   stay the same for a few pages (restore it on the last one when going back),
   but on every page the parameter is reset,  and the Julia set is redrawn by
   drawRight().  This function resets z as well.
 */
   pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1;
if (page == 1)
{  text->setHtml(tr(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The left image shows the Mandelbrot set M, which will be discussed later. "
 "The unit disk in the right image is related to the iteration of a simple "
 "map: when a "
 "<a href=\"http://en.wikipedia.org/wiki/Complex_number\">complex number</a> "
 "z is squared, z &rarr; z<sup>2</sup>, then "
 "its modulus |z| (the distance to 0) is squared, and its argument "
 "(the angle to the positive real axis) is doubled. A discrete dynamic "
 "system is given by iterating this map, i.e., applying the same map "
 "again and again, starting with some initial value."
 "<p>"
 "When the initial value is within the unit disk, i.e., |z| &lt; 1, then it "
 "will converge to 0 under iteration, since the modulus goes to 0. If "
 "|z| &gt; 1, it will go to &infin;, the point at infinity (see page 8). "
 "Points on the unit circle, |z| = 1, will stay there forever. Here the "
 "dynamics is chaotic: repelling periodic points are dense on the unit "
 "circle, there is a dense orbit, and the distance of nearby points is "
 "doubled under iteration."
 "<p>"
 "<i>Observe the changes of modulus and angle, and the qualitative "
 "dynamics, by repeating the following steps:<br>"
 "1) Choose an initial value for z by clicking into the right window, or by "
 "moving the cross with the Arrow keys.<br>"
 "2) Iterate the initial value, i.e., map it several times "
 "z &rarr; z<sup>2</sup>, by hitting Return or pushing the Go-button "
 "repeatedly.<br>"
 "Do this for initial values inside the disk, outside, and on the circle. "
 "(Since the point will not be on the circle exactly, it "
 "will move away from it after a few iterations.)</i>"));
   pplane->setPlane(-0.83, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   pplane->setNmax(150); dplane->setNmax(150);
   if (!backwards) pplane->draw(pf, 1, &mode);
   pplane->setPoint(0, 0); dplane->setActive(true);
}
if (page == 2)
{  text->setHtml(tr(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Now the map is given by z &rarr; z<sup>2</sup> + c with "
 "c = -0.125 + 0.5*i : first the modulus of z is squared and its angle "
 "doubled. Then it is translated by 0.125 to the left and by 0.5 upwards. "
 "The dynamics is qualitatively similar to that of z &rarr; z<sup>2</sup> : "
 "any point z inside the black region will converge to a unique point "
 "&alpha;<sub>c</sub> &asymp; -0.2071 + 0.3536*i, which is called a fixed "
 "point, since it is not moved itself by the map. Starting with a point "
 "z in the green exterior, iterating the map lets z go to &infin;. "
 "A point z on the boundary of the black region will stay there forever. "
 "<p>"
 "The black region is the filled Julia set K<sub>c</sub>. It is the "
 "<a href=\"javascript:alert('A compact set is bounded and closed, i.e., "
 "containing all boundary points.')\">compact</a> "
 "set of points not going to &infin; under iteration. Its boundary "
 "&part;K<sub>c</sub> is the Julia set J<sub>c</sub> , where the "
 "dynamics is always chaotic."
 "<p>"
 "<i>Again, choose an initial value z by clicking into the right window, or "
 "with the Arrow keys. Map it several times z &rarr; z<sup>2</sup> + c, by "
 "hitting Return or pushing the Go-button. Do this for intial values inside "
 "the filled Julia set, outside, and on the Julia set.</i>"));
   pplane->setPoint(-0.125, 0.5); dplane->setActive(true);
}
if (page == 3)
{  text->setHtml(tr(
 "Now the map is z &rarr; z<sup>2</sup> - 1. We shall always consider "
 "quadratic polynomials of the form f<sub>c</sub>(z) = z<sup>2</sup> + c "
 "with a fixed complex parameter c, and now c = -1. The filled Julia set "
 "K<sub>c</sub> , called the \"basilica,\" has a different shape: its "
 "boundary contains pinching points, whose removal would disconnect "
 "K<sub>c</sub> , and the interior of K<sub>c</sub> consists of "
 "countably many components. Two of these are mapped onto each other, and "
 "they contain an attracting 2-cycle, i.e., two points that are mapped to "
 "each other by f<sub>c</sub>(z), and thus onto themselves by the second "
 "iterate of f<sub>c</sub>(z). These two components are connected by the "
 "repelling fixed point &alpha;<sub>c</sub> . All components are mapped "
 "onto the 2-periodic components after a finite number of iterations, and "
 "every point z in the interior of the filled Julia set K<sub>c</sub> "
 "converges to the 2-cycle under the iteration."
 "<p>"
 "<i>Again, choose an initial value by clicking or with the Arrow keys. "
 "Map it several times z &rarr; z<sup>2</sup> + c.</i><br>"
 "In the main program, you can choose the initial value also by hitting "
 "Ins and giving its coordinates. Map it by hitting the key f."));
   pplane->setPoint(-1.0, 0); dplane->setActive(true);
}
if (page == 4)
{  text->setHtml(tr(
 "Again, consider a quadratic polynomial of the form "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c with the new parameter "
 "c = -0.125 + 0.736*i. The filled Julia set K<sub>c</sub> , "
 "called the \"rabbit,\" has three components at every pinching point. "
 "There is an attracting 3-cycle of points in the three components "
 "attached to the repelling fixed point &alpha;<sub>c</sub> . Every "
 "component is mapped to these three components after a finite number of "
 "iterations, and every point z in the interior of the filled Julia set "
 "K<sub>c</sub> converges to the 3-cycle under the iteration. "
 "Starting with an initial value z outside of K<sub>c</sub> , the orbit "
 "always escapes to &infin;, and the Julia set &part;K<sub>c</sub> "
 "is always invariant under the iteration of f<sub>c</sub>(z)."
 "<p>"
 "<i>Again, choose an initial value and map it several times "
 "z &rarr; z<sup>2</sup> + c.</i><br>"
 "Note that the current value of the parameter c is indicated by the cross "
 "in the left window, since it is a point in the parameter plane. In the "
 "main program, you can set this point by clicking into the left window, "
 "e.g., activate the right window by hitting F2, set the point z there, "
 "and map it by hitting the key f." ));
   pplane->setPoint(-0.125, 0.736); dplane->setActive(true);
}
if (page == 5)
{  text->setHtml(tr(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "In the previous examples of quadratic polynomials "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c, the filled Julia set K<sub>c</sub> "
 "was connected, and the critical point z = 0 belonged to K<sub>c</sub> . "
 "At the critical point, f<sub>c</sub>(z) is not locally "
 "<a href=\"javascript:alert('A function is injective, if it cannot map "
 "two different points to the same value.')\">injective</a> but 2:1. The "
 "critcal value is f<sub>c</sub>(0) = c, which is the same complex number "
 "as the parameter c. If z = 0 and z = c do not belong to K<sub>c</sub> , "
 "i.e., the critcal orbit goes to &infin;, then the filled Julia set "
 "K<sub>c</sub> will be <a href=\"javascript:alert('A set is totally "
 "disconnected, if its connected components consist of single points.')\">"
 "totally disconnected</a>."
 "<p>"
 "The right image shows the filled Julia set K<sub>c</sub> for the "
 "parameter c = -0.85 + 1.0*i. The inner white curve passes through the "
 "critical value z = c. Its preimage is a figure-eight, since "
 "f<sub>c</sub>(z) is 2:1 at the critical point z = 0. The preimage of this "
 "curve consists of two figure-eights, and every component of K<sub>c</sub> "
 "is contained in a nested sequence of such curves. Thus there are "
 "infinitely many connected components, which can be shown to be single "
 "points."
 "<p>"
 "The left image shows the parameter plane. Each point c represents a "
 "dynamic plane, where a quadratic polynomial "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c and its filled Julia set "
 "K<sub>c</sub> live. If K<sub>c</sub> is connected, i.e., the critical "
 "orbit stays bounded, then the parameter c belongs to the Mandelbrot set "
 "M. If K<sub>c</sub> is disconnected, i.e., the critical orbit escapes to "
 "&infin;, then c does not belong to M. This defines the Mandelbrot set as "
 "a subset of the parameter plane."
 "<p>"
 "<i>Hit Return or push the Go-button to move z from the critical value c "
 "to the critical point 0 and to its preimages, and to draw the "
 "corresponding curves.</i><br>"
 "The curves shown here are equipotential lines. In the main program, the "
 "equipotential line through the current point, or for a given potential, "
 "is drawn with the key g. Preimages of z are obtained with the keys "
 "a and b."));
   pplane->setPoint(-0.85, 1.0); dplane->setPoint(-0.85, 1.0);
}
if (page == 6)
{  text->setHtml(tr(
 "Each parameter c in the left image represents a quadratic polynomial "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c and its filled Julia set "
 "K<sub>c</sub> . E.g., c = 0 represents f<sub>0</sub>(z) = z<sup>2</sup> "
 "and K<sub>0</sub> , the unit disk. K<sub>c</sub> is a connected set, when "
 "the parameter c belongs to the Mandelbrot set M. It is conjectured that "
 "all interior components of M consist of hyperbolic parameters, i.e., the "
 "maps f<sub>c</sub>(z) corresponding to these parameters have an "
 "attracting cycle. As the parameter c varies within a hyperbolic "
 "component, the corresponding Julia sets vary continuously and keep their "
 "qualitative structure. At the boundary &part;M of the Mandelbrot set, "
 "various bifurcations of the qualitative dynamics of f<sub>c</sub>(z) "
 "and of the structure of K<sub>c</sub> happen, see Chapters 2-3."
 "<p>"
 "<i>Choose different parameters and observe the changes of the Julia set: "
 "click into the left window, or move the cross with the Arrow keys.</i><br>"
 "The main program shows the parameter plane in the left window and the "
 "dynamic plane in the right window, too. Activate either window by hitting "
 "F2 or clicking into it. Resize the windows with F4. You cannot change the "
 "image size by resizing the application window."));
   if (backwards)
   {  dplane->setNmax(150); pplane->setNmax(150);
      pplane->setPlane(-0.83, 0, 1.6, 0); pplane->draw(pf, 1, &mode);
   }
   pplane->setActive(true); goButton->setEnabled(false);
   pplane->setPoint(0, 0);
}
if (page == 7)
{  text->setHtml(tr(
 "In Chapter 2, you shall see that Julia sets are self-similar at many "
 "points. Chapters 3 and 4 explain our main tool, external rays. Since the "
 "pioneering work of Adrien Douady and John Hamal Hubbard, the "
 "Mandelbrot set M is understood by an interplay between the parameter "
 "plane and the dynamic planes for different parameters c. M shows a "
 "combination of various kinds of self-similarity, see Chapters 5-8, but "
 "usually different parts are not similar to each other. When zooming into "
 "the Mandelbrot set, you may encounter different patterns of branch "
 "points, and small copies of M with infinitely many decorations."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in.</i><br>"
 "In the main program, smooth zooming is obtained by a mdouble-click or "
 "with the key z. Or specify a frame with the mouse or with the keys +-. "
 "Increase the number of iterations with n, change the coloring with "
 "Del. Go back with the Back key or zoom out with v, get the first image "
 "again with Home."));
   pplane->setNmax(1500); dplane->setNmax(500);
   if (backwards) { dplane->setType(0); dplane->setPlane(0, 0, 2.0, 0); }
   pplane->setPoint(-0.758060762686005, 0.0720118702356124);
}
if (page == 8)
{  text->setHtml(tr(
 "External rays are curves in the exterior of a connected filled Julia set "
 "K<sub>c</sub> , see Chapter 3 for their precise definition. They come "
 "from &infin; at a definite angle, and they may land at a "
 "boundary point of K<sub>c</sub> . The right image shows three rays "
 "landing together at a branch point of K<sub>c</sub> ."
 "<p>"
 "Complex numbers can be visualized as a plane (due to Gauss) or as a "
 "sphere (due to Riemann). The plane corresponds to the sphere without its "
 "north pole. This point represents &infin;: when a point z in the "
 "plane has a very large modulus, the corresponding point on the sphere will "
 "be close to the north pole &infin;. While the distinction between +&infin; "
 "and -&infin; is useful for real numbers, the direction of z is neglected "
 "when a complex number z goes to &infin;. When a rational map is "
 "considered on the Riemann sphere, it will be continuous at its poles as "
 "well. The quadratic polynomial f<sub>c</sub>(z) = z<sup>2</sup> + c "
 "has a superattracting fixed point at &infin;."
 "<p>"
 "<i>Hit Return or push the Go-button repeatedly to see the Julia set on "
 "the Riemann sphere and to turn it around.</i><br>"
 "In the main program, show the sphere by hitting F3. Turn it "
 "around the vertical or horizontal axis by clicking beside it or with "
 "Ctrl+Arrow keys. Or drag it with the mouse."));
   if (!backwards) { pplane->setNmax(150); pplane->setNmax(150); }
   pplane->setPlane(-0.83, 0, 1.6, 0); pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.101096363845622, 0.956286510809142);
   pplane->draw(pf, 1, &mode); n = 0;
}
if (page == 9)
{  text->setHtml(tr(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "There are external rays in the exterior of the Mandelbrot set M as well."
 "The left image shows three parameter rays landing at a branch point in M "
 "with three branches. A pinching point is a point in a compact connected "
 "set K, whose removal would leave the set disconnected into several "
 "branches. If there are at least three branches, it is called a branch "
 "point. Pinching points and branch points are found as the common landing "
 "points of external rays, with exactly one ray landing between two "
 "consecutive branches. They are used to cut M or K<sub>c</sub> into "
 "well-defined components, and to build topological models for these sets "
 "in a combinatorial way. "
 "<p>"
 "The right figure shows a standard example of a set K that is not locally "
 "connected: since there are infinitely many vertical lines accumulating at "
 "the middle line, a point on that line does not have an arbitrarily small "
 "connected neighborhood. Although every external ray of this set will land "
 "somewhere, there is no ray landing at the center point, and the "
 "map of angle &rarr; landing point is not continuous at the angle "
 "&pi;/2. (Note also that the center point is not a branch point of K: "
 "although it looks like its removal would leave four branches, the "
 "vertical segments are not relatively open. The set would still be "
 "connected, although not pathwise connected.) For other examples K, "
 "there may be rays accumulating at the "
 "boundary &part;K without landing. By a Theorem of Carath&eacute;odory, "
 "K is locally connected if and only if all rays land, and the "
 "landing point depends continuously on the angle."
 "<p>"
 "It is known for many classes of filled Julia sets, if K<sub>c</sub> is "
 "locally connected or not. The main conjecture about the Mandelbrot set, "
 "which has not been proved yet, says that it is locally connected. This "
 "would imply that M is described completely by topological models based on "
 "the common landing points of certain external rays. Otherwise M would "
 "contain non-trivial "
 "<a href=\"http://arXiv.org/abs/math/9902155\">fibers</a>, i.e., "
 "subsets that are not disconnected by these landing points, and which "
 "might contain non-hyperbolic components."
 "<p>"
 "<i>Hit Return or push the Go-button repeatedly to zoom in around the "
 "branch point.</i>"));
   dplane->setType(0); dplane->setPlane(5.0, 0, 2.0, 0); //green backgr.
   pplane->setPlane(-0.101096363845622, 0.956286510809142, 2.15, 0);
   pplane->setPoint(-0.101096363845622, 0.956286510809142);
   pplane->draw(pf, 1, &mode);
}
} //showPage DS

void QmnDemoDS::go()
{  if (page <= 4)
   {  mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      df->iterate(x, y); dplane->setPoint(x, y);
   }
   if (page == 5)
   {  mdouble x, y; dplane->getPoint(x, y); df->iterate(x, y, -1);
      dplane->setPoint(x, y); y = df->green(-1, x, y);
      dplane->green(df, -1, y, 32);
   }
   if (page == 7 || page == 9) pplane->draw(pf, 1, &mode, 210);
   if (page == 8)
   {  if (!n)
      {  n = 1; dplane->setType(-1);
         dplane->move(11, 0, -90); dplane->move(11, 90, 0);
      }
      dplane->move(11, 15, 0); dplane->draw(df, -1, &mode);
   }
}

void QmnDemoDS::drawRight()
{  mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   dplane->setPoint(0, 0); dplane->draw(df, -1, &mode);
}

void QmnDemoDS::pFinished()
{  if (page != 9) return; mdouble x, y;
   pplane->traceRay(1, 9.0/56.0, pf, x, y, 10);
   pplane->traceRay(1, 11.0/56.0, pf, x, y, 10);
   pplane->traceRay(1, 15.0/56.0, pf, x, y, 10);
}

void QmnDemoDS::dFinished()
{  if (page == 5)
   {  dplane->green(df, -1, df->green(-1, -1.1275, -0.7), 4);
      dplane->green(df, -1, df->green(-1, -0.85, 1.0), 4);
   }
   if (page == 8)
   {  mdouble x, y; df->getParameter(x, y);
      dplane->backRay(9LL, 56LL, x, y, 3);
      dplane->backRay(11LL, 56LL, x, y, 3);
      dplane->backRay(15LL, 56LL, x, y, 3);
   }
   if (page != 9) return;
   dplane->setPlane(0, 0, 2.0, 0);
   dplane->drawLine(-1.7, 0, 1.7, 0, Qt::darkBlue);
   dplane->drawLine(0, -1.0, 0, 1.0, Qt::darkBlue);
   mdouble t;
   for (n = 0, t = 1.45; n <= 11; n++, t *= 0.68)
   {  dplane->drawLine(t, -1.0, t, 1.0, Qt::darkBlue);
      dplane->drawLine(-t, -1.0, -t, 1.0, Qt::darkBlue);
   }
}

void QmnDemoPB::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1;
if (page == 1)
{  text->setHtml(QString(
 "Recall from Chapter 1 that K<sub>c</sub> is the filled Julia set of the "
 "quadratic polynomial f<sub>c</sub>(z) = z<sup>2</sup> + c : points z in "
 "K<sub>c</sub> stay bounded under the iteration, and points in the "
 "exterior go to &infin;. K<sub>c</sub> contains periodic points of almost "
 "all periods p: z<sub>0</sub> is p-periodic, if it is mapped to itself by "
 "p iterations of f<sub>c</sub>(z), and if p is the smallest number with "
 "this property. z<sub>0</sub> and its images form a p-cycle. Fixed points "
 "are 1-periodic."
 "<p>"
 "The multiplier &rho; associated to the p-cycle is the derivative of the "
 "p-th iterate of f<sub>c</sub>(z) at a point of the cycle. If "
 "|&rho;| &lt; 1, the cycle will be attracting: every z in a neighborhood is "
 "attracted to the cycle, which belongs to the interior of K<sub>c</sub> . "
 "Thus K<sub>c</sub> is not totally disconnected, and the parameter c "
 "belongs to the Mandelbrot set M. At z<sub>0</sub> , the p-th iterate of "
 "f<sub>c</sub>(z) is conjugate to multiplication with &rho;, "
 "g(z) = &rho;z. If the cycle is superattracting, i.e., &rho; = 0, the "
 "p-th iterate will be conjugate to g(z) = z<sup>2</sup>. A conjugation "
 "&phi;(z) from f<sub>c</sub><sup>p</sup>(z) to g(z) means that "
 "f<sub>c</sub><sup>p</sup>(z) = &phi;<sup>-1</sup>(g(&phi;(z))). Then "
 "&phi;(z) is a kind of coordinate change, and the dynamics of "
 "f<sub>c</sub>(z) and g(z) is qualitatively the same."
 "<p>"
 "Since every attracting cycle attracts the critical point z = 0, at "
 "most one cycle can be attracting.The right image shows K<sub>c</sub> for "
 "the parameter c &asymp; -0.1539 + 1.0377*i, for which f<sub>c</sub>(z) "
 "has an attracting 12-cycle."
 "<p>"
 "If |&rho;| &gt; 1, the cycle will be repelling and belongs to the Julia "
 "set J<sub>c</sub> = &part;K<sub>c</sub> . At z<sub>0</sub> , K<sub>c</sub> "
 "is locally invariant under the p-th iterate of f<sub>c</sub>(z), which "
 "is conjugate to the multiplication with &rho; again. Therefore the Julia "
 "set has a spiraling shape, since this multiplication is a combination of "
 "a scaling by the factor |&rho;| and a rotation by the argument of &rho;. "
 "In the image, the cross indicates the repelling fixed point "
 "&alpha;<sub>c</sub> . Neutral cycles, i.e., |&rho;| = 1, will be "
 "discussed on pages 7-10."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in around "
 "the repelling fixed point &alpha;<sub>c</sub>.<br>"
 "The spiral appears as a slow rotation of the branches, since the argument "
 "of &rho;<sup>3</sup> is very small here.</i>"));
   pplane->setPlane(-0.83, 0, 1.6, 0); pplane->setNmax(250);
   dplane->setPlane(0, 0, 2.0, 0); dplane->setNmax(250);
   pplane->draw(pf, 1, &mode); pplane->setPoint(-0.1539466011, 1.037693778);
}
if (page == 2)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The filled Julia set K<sub>c</sub> shows various notions of "
 "self-similarity:<ol>"
 "<li>Any small subset of K<sub>c</sub> , not containing the critical point "
 "z = 0, is mapped <a href=\"javascript:alert('A conformal map is "
    "complex differentiable with non-zero derivative. Locally, it is "
    "approximately a rotation and rescaling without shearing. See Chapter "
    "8')\">conformally</a> onto a subset of K<sub>c</sub> by "
 "f<sub>c</sub>(z).</li>"
 "<li>A small neighborhood of a repelling p-periodic point z<sub>0</sub> "
 "is mapped conformally onto a larger neighborhood by the p-th iterate of "
 "f<sub>c</sub>(z).</li>"
 "<li>This map is asymptotically linear at z<sub>0</sub> : in a "
 "sufficiently small neighborhood of z<sub>0</sub> , K<sub>c</sub> is "
 "approximately self-similar by a linear map, which is multiplication "
 "with the multiplier &rho;. See also page 2 of Chapter 6.</li></ol>"
 "You have seen an example of this approximately linear self-similarity on "
 "the previous page. Now the right image shows K<sub>c</sub> for the "
 "parameter c = -0.5 . The cross in the middle is the attracting fixed "
 "point &alpha;<sub>c</sub> , and the other two crosses denote the "
 "repelling 2-cycle."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in around "
 "a repelling 2-periodic point.</i><br>"
 "In the main program, periodic points and preperiodic points are found "
 "with the key x. They can be marked with #."));
   dplane->setPlane(0, 0, 2.0, 0); pplane->setPoint(-0.5, 0);
}
if (page == 3)
{  text->setHtml(QString(
 "If f<sub>c</sub>(z) has an attracting p-cycle, this behavior will be "
 "stable under a perturbation of the parameter: for all parameters c in a "
 "hyperbolic component of the Mandelbrot set, the attracting cycle and "
 "the Julia set move holomorphically. A bifurcation, i.e., a change in the "
 "qualitative behavior, happens when an attracting cycle becomes repelling, "
 "e.g., or vice versa."
 "<p>"
 "When the parameter c moves from c = -0.5 to c = -1.0 , e.g., a period "
 "doubling bifurcation happens at c = -0.75 , where two cycles coincide: "
 "the attracting fixed point &alpha;<sub>c</sub> becomes repelling, and "
 "the repelling 2-cycle becomes attracting. Now &alpha;<sub>c</sub> is a "
 "pinching point of K<sub>c</sub> , and the 2-cycle is in the interior of "
 "K<sub>c</sub> . The filled Julia sets for c &gt; -0.75 and for "
 "c &lt; -0.75 are not mutually homeomorphic (they cannot be mapped one "
 "onto another continuously), and the corresponding polynomials "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c are not conjugate."
 "<p>"
 "<i>Hit the Return key or push the Go-button to move the parameter c in "
 "the left image; observe the changes in the right image.</i>"));
   dplane->setPlane(0, 0, 2.0, 0); pplane->setPoint(-0.5, 0);
}
if (page == 4)
{  text->setHtml(QString(
 "The polynomial f<sub>c</sub>(z) = z<sup>2</sup> + c with c = -1.0 has a "
 "superattracting 2-cycle. The filled Julia set K<sub>c</sub> , the "
 "\"basilica,\" is again self-similar at the repelling fixed point "
 "&alpha;<sub>c</sub> , which connects the two interior components "
 "of K<sub>c</sub> containing the 2-cycle. The multiplier is "
 "&rho; = 1 - &radic;5 &lt; 0, and f<sub>c</sub>(z) interchanges the "
 "two components. From the second iterate we see that the image will be "
 "approximately invariant under a scaling with "
 "&rho;<sup>2</sup> = 6 - 2&radic;5 &asymp; 1.528 ."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in around "
 "the repelling fixed point &alpha;<sub>c</sub> .</i>"));
   pplane->setPoint(-1.0, 0); dplane->setPoint(0.5 - sqrt(1.25), 0);
}
if (page == 5)
{  text->setHtml(QString(
 "The period tripling bifurcation is similar to the period doubling "
 "bifurcation of page 3: as the parameter c approaches the point of "
 "bifurcation, the repelling 3-cycle moves towards the attracting "
 "fixed point &alpha;<sub>c</sub> . The latter becomes repelling, and "
 "the 3-cycle becomes attracting. Again, &alpha;<sub>c</sub> will be a "
 "pinching point of the filled Julia set K<sub>c</sub> , joining the three "
 "interior components around the attracting 3-cycle. K<sub>c</sub> becomes "
 "the \"rabbit.\""
 "<p>"
 "<i>Hit the Return key or push the Go-button to move the parameter c in "
 "the left image; observe the changes in the right image.</i>"));
   dplane->setPlane(0, 0, 2.0, 0); pplane->setPoint(-0.125, 0.5010815528383);
   if (backwards) pplane->draw(pf, 1, &mode);
}
if (page == 6)
{  text->setHtml(QString(
 "For parameters c in a hyperbolic component, denote by &rho; the "
 "multiplier of the attracting p-cycle. The map c &rarr; &rho; is a "
 "conformal map from the hyperbolic component onto the unit disk. "
 "The center of the component is the parameter with &rho; = 0, i.e., the "
 "polynomial f<sub>c</sub>(z) = z<sup>2</sup> + c has a superattracting "
 "p-cycle, and the root of the component is characterized by &rho; = 1. "
 "The boundary of the hyperbolic component is parametrized by the "
 "internal angle, which is the argument of the neutral multiplier. If the "
 "argument is 2&pi;*k/r, a period r-tupling bifurcation occurs. A "
 "sublimb is attached to the hyperbolic component here, and for all "
 "parameters c in this sublimb, the newly repelling p-cycle has the "
 "combinatorial rotation number k/r: the r components of K<sub>c</sub> "
 "at one of the p-periodic points are permuted cyclically by the p-th "
 "iterate of f<sub>c</sub>(z), going k steps counterclockwise."
 "<p>"
 "The main cardioid of the Mandelbrot set M is the hyperbolic component "
 "of period p = 1, and the fixed point &alpha;<sub>c</sub> is attracting "
 "when the parameter c is in the main cardioid. The boundary is "
 "parametrized by the argument of the neutral multiplier. The k/r-limb is "
 "attached to the main cardioid at the parameter with "
 "&rho; = exp(i2&pi;k/r). For "
 "parameters c in this limb, f<sub>c</sub>(z) permutes the r "
 "components of K<sub>c</sub> at &alpha;<sub>c</sub> with combinatorial "
 "rotation number k/r. The right image shows K<sub>c</sub> for a parameter "
 "c in the 1/5-limb, such that f<sub>c</sub>(z) has an attracting 5-cycle. "
 "Observe the rotation numbers 1/5, 2/5, 3/5, and 4/5 as follows:<br>"
 "<i>Hit Return or push the Go-button to follow the attracting 5-cycle. "
 "Note its rotation number.<br>"
 "Hit any Arrow key, or click into the left window, to choose "
 "the next limb of period 5.</i><br>"
 "In the main program, bifurcate to the center for some rotation number "
 "with the key Ctrl+c."));
   mode = 0; pplane->draw(pf, 1, &mode); pplane->setPoint(0.3795, 0.3349);
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For parameters c in the boundary of the main cardioid, f<sub>c</sub>(z) "
 "has a neutral fixed point &alpha;<sub>c</sub>. If the argument of the "
 "multiplier &rho; is a rational multiple of 2&pi;, the fixed point is "
 "called rationally neutral or parabolic. This happens at r-tupling "
 "bifurcation points and at the root of the component. Otherwise, the "
 "fixed point is called irrationally neutral. Then either there is a "
 "Siegel disk around the fixed point, where f<sub>c</sub>(z) is conjugate "
 "to a rotation. Or there is no conjugation, and the fixed point is called "
 "a Cremer point. If the irrational rotation number is approximated badly "
 "by rational numbers, there will be a Siegel disk, otherwise not. The work "
 "of Siegel, Cremer, Brjuno, Cherry, Yoccoz has led to a precise "
 "condition in terms of the "
 "<a href=\"http://en.wikipedia.org/wiki/Continued_fraction\">continued "
 "fraction</a> expansion of the rotation "
 "number. If the critical orbit is on the boundary of the Siegel disk, then "
 "K<sub>c</sub> may be locally connected (i.e., there are arbitrarily small "
 "connected neighborhoods of all points, see page 9 of Chapter 1). Or the "
 "critical orbit may accumulate at the boundary without belonging to it, and "
 "K<sub>c</sub> will be non-locally connected, although the boundary of the "
 "disk may be <a href=\"http://www.math.univ-toulouse.fr/~buff/Preprints/"
 "Boundaries/Boundaries.pdf\">smooth</a> in fact. When &alpha;<sub>c</sub> "
 "is a Cremer point, K<sub>c</sub> will not be locally connected. The "
 "dynamics is not understood completely."
 "<p>"
 "The right image shows the filled Julia set of K<sub>c</sub>, where the "
 "parameter c is located on the boundary of the main cardioid, such that the "
 "rotation number of the fixed point &alpha;<sub>c</sub> is the "
 "<a href=\"http://en.wikipedia.org/wiki/Golden_ratio\">Golden Mean</a> "
 "(&radic;5 - 1)/2. Although K<sub>c</sub> is homeomorphic to the "
 "\"basilica,\" the dynamics is quite different. The white points mark the "
 "critical orbit, which shows the boundary of the Siegel disk. Orbits within "
 "the Siegel disk would be on closed curves, which are approximately "
 "circular near to the neutral fixed point &alpha;<sub>c</sub>."
 "<p>"
 "Note that for other parameters on the boundary of the main cardioid, the "
 "image may show a white curve as well, even if there is no Siegel disk but "
 "a parabolic point with many petals, and the critical orbit converges "
 "to it only after many more iterations.<br>"
 "<i>Hit Return or push the Go-button to move the parameter c around "
 "the boundary of the main cardioid.</i><br>"
 "In the main program, go to a boundary point with the key c. You "
 "cannot walk along the boundary, however. Draw the orbit of the current "
 "point with the key Ctrl+f. In this way, e.g., you may draw "
 "many orbits inside a Siegel disk."));
   if (backwards)
   {  pplane->setNmax(250); dplane->setNmax(250);
      pplane->setPlane(-0.83, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   }
   pplane->draw(pf, 1, &mode); mdouble x = 0, y = 0;
   pf->bifurcate(.61803398874989484820L, x, y); pplane->setPoint(x, y);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "On pages 3 and 5 you have seen examples of a satellite bifurcation: as "
 "the parameter c in the main cardioid approaches the boundary, such "
 "that the multiplier &rho;<sub>c</sub> = 2&alpha;<sub>c</sub> becomes "
 "exp(i2&pi;k/r), the fixed point &alpha;<sub>c</sub> meets an r-cycle, "
 "which becomes attracting. This is a pitchfork bifurcation of the r-th "
 "iterate. A hyperbolic component of period r is attached to the main "
 "cardioid at this parameter. Analogously, hyperbolic components of "
 "period r*p are attached to those of period p."
 "<p>"
 "There are satellite components and primitive components. The former look "
 "like disks, the latter like cardioids. At the root point of a primitive "
 "hyperbolic component, a different kind of bifurcation happens: a "
 "primitive bifurcation, which is a low-dimensional version of a "
 "saddle-node bifurcation. The left image shows the primitive hyperbolic "
 "component of period 3. As the parameter c approaches the root "
 "a = -1.75 from the right, the two repelling 3-cycles of f<sub>c</sub> "
 "meet to form one parabolic 3-cycle. A \"cauliflower,\" a parabolic basin "
 "appears with the critical value z = c inside, and the parabolic "
 "3-cycle on its boundary. Afterwards, when the parameter c is inside the "
 "hyperbolic component, the two 3-cycles separate again. One cycle is "
 "attracting and inside K<sub>c</sub>, the other one consists of repelling "
 "pinching points."
 "<p>"
 "The right image shows a subset of K<sub>c</sub> at the critical value "
 "z = c. The structure of K<sub>c</sub> is a combination of spirals at "
 "the repelling 3-periodic points, and preimages joined like the branches "
 "of a cauliflower. The spirals are more prominent as c &rarr; a and the "
 "multiplier satisfies &rho; &rarr; 1, see also page 9 of Chapter 5. At "
 "c = a there is a "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:1062.37044\">parabolic implosion</a>: neither the Julia set "
 "J<sub>c</sub> "
 "nor the filled Julia set K<sub>c</sub> move continuously. K<sub>c</sub> "
 "gets bigger, and J<sub>c</sub> gets smaller in the sense that "
 "points of J<sub>c</sub> may converge to points in the interior of "
 "K<sub>a</sub> , which do not belong to J<sub>a</sub> = &part;K<sub>a</sub>."
 "<p>"
 "<i>Hit Return or push the Go-button to move the parameter c in the left "
 "image; observe the changes of the Julia set in the right image.</i><br>"
 "In the main program, hit Shift+F2 to observe a fixed subset of the dynamic "
 "plane while changing the parameter c."));
   if (!backwards) { pplane->setNmax(500); dplane->setNmax(500); }
   pplane->setPlane(-1.7548776662467, 0, 0.047, 0);
   dplane->setPlane(-1.7548776662467, 0, 0.033, 0);
   pplane->setPoint(-1.7373046875, 0);
   pplane->draw(pf, 1, &mode);
}
if (page == 9)
{  text->setHtml(QString(
 "The parameter c = 0.25 is the root of the main cardioid, and "
 "f<sub>c</sub>(z) has a parabolic fixed point &alpha;<sub>c</sub> = 0.5 . "
 "There it is conjugate to a translation at &infin;, z &rarr; z + 1, "
 "and the orbits are "
 "approximately circles touching the real axis at &alpha;<sub>c</sub> . A "
 "point z close to &alpha;<sub>c</sub> moves very slowly towards it or "
 "away from it under the iteration of f<sub>c</sub>(z). The filled Julia "
 "set K<sub>c</sub> is the closure of the parabolic basin, it is called the "
 "\"cauliflower.\" For parameters c > 0.25 , the two fixed points are "
 "repelling, and the Julia set is totally disconnected. The Fatou "
 "coordinates are two partial conjugations to a translation at &infin;, "
 "which show that for c = 0.25 + &epsilon; with small &epsilon; &gt; 0, an "
 "orbit will take approximately n &asymp; &pi;/&radic;&epsilon; iterations "
 "to pass through between the two fixed points. E.g., this explains "
 "Bott's observation that the escape time of the critical orbit for "
 "c = 0.2501 , c = 0.250001 ... is approximately 100&pi;, 1000&pi; ... The "
 "right image shows the \"imploded cauliflower\" for c = 0.251 ."
 "<p>"
 "<i>Hit Return or push the Go-button to map z with f<sub>c</sub>(z), "
 "observe the orbit going slowly between the fixed points. Change the "
 "initial value of z with the Arrow keys or by clicking into the "
 "right window.</i><br>"
 "To check the escape time in the main program, set c with the key Ins "
 "and adjust the maximal number of iterations with the key n, "
 "until the color of c changes."));
   if (backwards) { pplane->setNmax(500); dplane->setNmax(500); }
   pplane->setPlane(-0.83, 0, 1.6, 0); pplane->draw(pf, 1, &mode);
   dplane->setPlane(0, 0, 1.15, 0); dplane->setActive(true);
   pplane->setPoint(0.255, 0);
}
if (page == 10)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The method of <a href=\"http://www.math.kyoto-u.ac.jp/~mitsu/pararenorm/\">"
 "parabolic renormalization</a> was used to show that the boundary of the "
 "Mandelbrot set has <a href=\"http://arxiv.org/abs/math/9201282\">"
 "Hausdorff dimension 2</a>, and recently to construct a Julia set of "
 "<a href=\"http://arxiv.org/abs/math/0605514\">positive area</a>. For "
 "another application of parabolic implosion, recall that the 1/r-limb "
 "is attached to the main cardioid at a parameter c, "
 "such that the fixed point &alpha;<sub>c</sub> is parabolic with "
 "combinatorial rotation number 1/r. These limbs converge to the root "
 "c = 0.25 of the main cardioid. Pierre Lavaurs has studied these limbs "
 "using the Fatou coordinates and the related horn map of Ecalle cylinders. "
 "It is <a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0934.30023\">conjectured</a> that the limbs converge to a "
 "limiting \"elephant\" under a suitable rescaling."
 "<p>"
 "The left image shows the 1/2-limb rotated by 90<sup>o</sup>. All limbs "
 "will be shown with increasing magnification and appropriate rotation, "
 "such that they point upwards from the boundary of the main "
 "cardioid. (Which is rotated such that it is horizontal at the root of "
 "the satellite component.) The right image shows the filled Julia set "
 "for the corresponding center of period r.<br>"
 "<i>Hit Return or push the Go-button to increase the period r of "
 "the limbs.</i><br>"
 "This kind of scaling is not available in the main program. See page 9 "
 "of Chapter 5 for another scaling phenomenon at parabolic parameters."));
   pplane->setNmax(5000); dplane->setNmax(5000);
   n = 1; dplane->setPlane(0, 0, 2.0, 0); go();
}
} //showPage PB

void QmnDemoPB::go()
{  if (page == 1 || page == 2 || page == 4) dplane->draw(df, -1, &mode, 210);
   if (page == 3)
   {  mdouble x, y; pplane->getPoint(x, y);
      x -= 0.0078125; pplane->setPoint(x, y);
      if (x < -0.99) goButton->setEnabled(false);
   }
   if (page == 5)
   {  mdouble x, y; pplane->getPoint(x, y);
      y += 0.00390625; pplane->setPoint(x, y);
      if (y > 0.745) goButton->setEnabled(false);
   }
   if (page == 6)
   {  mdouble x, y; dplane->getPoint(x, y);
      df->iterate(x, y); dplane->setPoint(x, y);
   }
   if (page == 7)
   {  mdouble x, y; pplane->getPoint(x, y); // 2alpha :
      mndynamics::root(1 - 4*x, -4*y, x, y); x = 1 - x; y = -y;
      y = atan2(y, x) + 0.01;
      x = 0.5*cos(y) - 0.25*cos(2*y); y = 0.5*sin(y) - 0.25*sin(2*y);
      pplane->setPoint(x, y);
   }
   if (page == 8)
   {  mdouble x, y; pplane->getPoint(x, y);
      x -= 0.00048828125; pplane->setPoint(x, y);
      if (x < -1.7545) goButton->setEnabled(false);
   }
   if (page == 9)
   {  mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      df->iterate(x, y); dplane->setPoint(x, y);
   }
   if (page == 10)
   {  n++; mdouble t = 2*PI/n,
         x = 0.5*cos(t) - 0.25*cos(2*t), y = 0.5*sin(t) - 0.25*sin(2*t),
         u = sin(t) - sin(2*t), v = -cos(t) + cos(2*t);
      t = 2.5/(n*n); u *= t; v *= t; //move root below the center of the image:
      pplane->setPlane(x - 0.6*v, y + 0.6*u, u, v);
      //pplane->setPlane(x - 0.8*v - 0.2*u, y + 0.8*u - 0.2*v, 0.1*u, 0.1*v);
      pplane->draw(pf, 1, &mode);
      pf->find(1, 0, n, x, y); pplane->setPoint(x, y);
   }
} //go PB

void QmnDemoPB::drawRight()
{  mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   if (page == 1) dplane->setPoint(-0.37106, 0.59565);
   if (page == 5) //alpha is redrawn below after drawing and marking 3 points
   {  mdouble x, y; pplane->getPoint(x, y);
      mndynamics::root(0.25 - x, -y, x, y);
      dplane->setPoint(0.5 - x , -y);
   }
   if (page == 6 || page == 7 || page == 9) dplane->setPoint(0, 0);
   if (page == 8 || page == 10) dplane->setPoint(x, y);
   dplane->draw(df, -1, &mode);
}

void QmnDemoPB::dFinished()
{  if (page == 2)
   {  mdouble x, y; pplane->getPoint(x, y);
      dplane->setPoint(0.5 - sqrt(0.75), 0); dplane->move(5);
      dplane->setPoint(-0.5, -0.5); dplane->move(5);
      dplane->setPoint(-0.5, 0.5);
   }
   if (page == 3)
   {  mdouble x, y; pplane->getPoint(x, y);
      if (x >= -0.75)
      {  y = sqrt(x + 0.75);
         dplane->setPoint(-0.5, y); dplane->move(5);
         dplane->setPoint(-0.5, -y); dplane->move(5);
      }
      else
      {  y = sqrt(-x - 0.75);
         dplane->setPoint(-0.5 + y, 0); dplane->move(5);
         dplane->setPoint(-0.5 - y, 0); dplane->move(5);
      }
      dplane->setPoint(0.5 - sqrt(0.25 - x), 0);
   }
   if (page == 5)     //add 3 points, then draw alpha again !!!
   {  mdouble x, y; pplane->getPoint(x, y);
      mndynamics::root(0.25 - x, -y, x, y);
      dplane->setPoint(0.5 - x , -y);
   }
   if (page == 7)
   { mdouble x = 0, y = 0; dplane->drawOrbit(df, x, y, 2000, 4000); }
} //dFinished PB

void QmnDemoPB::move(QAction *act)
{ if (page == 6) nextLimb(); else QmnDemo::move(act); }

void QmnDemoPB::nextLimb()
{  if (page != 6) return;
   mdouble x, y; pplane->getPoint(x, y);
   if (x > 0 && y > 0) { pplane->setPoint(-0.5043, 0.5628); return; }
   if (x < 0 && y > 0) { pplane->setPoint(x, -y); return; }
   if (x < 0 && y < 0) { pplane->setPoint(0.3795, -0.3349); return; }
   if (x > 0 && y < 0) { pplane->setPoint(x, -y); return; }
}

////////////////////////////////////////////////////////////////////////

void QmnDemoER::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1;
if (page == 1)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The filled Julia set K<sub>c</sub> of the quadratic polynomial "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c contains all points z with a "
 "bounded orbit, and points z in the exterior escape to &infin; under the "
 "iteration of f<sub>c</sub>(z). When the parameter c belongs to the "
 "Mandelbrot set M, the critical orbit does not escape, and K<sub>c</sub> "
 "is a connected set. The <a href=\"javascript:alert('A conformal map "
    "is complex differentiable with non-zero derivative. Locally it is "
    "approximately a rotation and rescaling without shearing. "
    "See Chapter 8.')\">conformal</a> map &Phi;<sub>c</sub>(z) from "
 "its exterior to the exterior of the unit disk conjugates "
 "f<sub>c</sub>(z) to F(z) = z<sup>2</sup>, i.e., "
 "f<sub>c</sub>(z) = &Phi;<sub>c</sub><sup>-1</sup>(F(&Phi;<sub>c</sub>(z)))."
 " External rays and equipotential lines in the dynamic plane are "
 "defined as preimages of straight rays and circles, respectively."
 "<p>"
 "When the parameter c does not belong to the Mandelbrot set, the critical "
 "orbit escapes to &infin;, and K<sub>c</sub> is not connected. The "
 "Boettcher conjugation &Phi;<sub>c</sub>(z) is defined for large |z|. It can "
 "be extended to the critical value z = c, e.g., but not to the critical "
 "point z = 0. A map &Phi;<sub>M</sub>(c) from the exterior of the "
 "Mandelbrot set to the exterior of the unit disk is defined by evaluating "
 "the Boettcher conjugation at the critical value: "
 "&Phi;<sub>M</sub>(c) = &Phi;<sub>c</sub>(c). Adrien Douady has shown that "
 "this map is conformal, and thus, the Mandelbrot set is connected. "
 "By the Maximum Principle, M is full, i.e., its complement is connected as "
 "well. Using the map &Phi;<sub>M</sub>(c), external rays and "
 "equipotential lines in the parameter plane are defined as preimages of "
 "straight rays and circles, respectively."
 "<p>"
 "<i>Click into the left window, or move the cross with the Arrow keys, to "
 "choose different parameters and observe the external rays and "
 "equipotential lines around the Julia set. (Drawing is optimized for "
 "closed outer equipotential lines. Figure-eights are visible better on "
 "page 5 of Chapter 1.)</i><br>"
 "In the main program, draw external rays with the key e and equipotential "
 "lines with the key g."));
   pplane->setPlane(-0.83, 0, 1.6, 0); pplane->setNmax(250);
   dplane->setPlane(0, 0, 2.0, 0); dplane->setNmax(250);
   pplane->draw(pf, 1, &mode); pplane->setPoint(0, 0);
   pplane->setActive(true);
}
if (page == 2)
{  text->setHtml(QString(
 "For c = 0, f<sub>c</sub>(z) conincides with F(z) = z<sup>2</sup>, and "
 "the external rays are straight rays. They are described by their argument, "
 "i.e., the angle to the positive real axis. This angle is doubled by F(z), "
 "since the argument of z<sup>2</sup> is twice that of z. Consider the ray "
 "with angle 1/7*2&pi;. It is mapped to the ray with angle 2/7*2&pi;, which "
 "is mapped to 4/7*2&pi;, and then to 8/7*2&pi;. The latter angle "
 "describes the same ray as 1/7*2&pi;, i.e., the initial ray. The angle "
 "1/7*2&pi; is 3-periodic under doubling modulo 2&pi;, and the ray is "
 "3-periodic under the map F(z)."
 "<p>"
 "For a point z on the ray with angle 1/7*2&pi;, the third iterate will be "
 "further out on the same ray. If z is the landing point of the ray on the "
 "unit circle, it will be mapped to itself by the third iterate, i.e., it "
 "is 3-periodic. Normally, angles will be measured in full turns instead of "
 "radians. E.g., we shall say 1/7 instead of 1/7*2&pi;. Then rational "
 "angles with odd denominators are periodic under doubling modulo 1. "
 "The period is 2 for denominator 3, 3 for denominator 7, and generally, the "
 "period is dividing p when the denominator is 2<sup>p</sup> - 1. The "
 "landing point z has the same period here. Since angle doubling is a "
 "chaotic map, so is F(z) on the unit circle."
 "<p>"
 "<i>Choose a point z on a ray with the Arrow keys, or by clicking into the "
 "right window. Map it a few times z &rarr; z<sup>2</sup>, by "
 "hitting Return or pushing the Go-button. Note that you will not hit the "
 "circle or the ray exactly, and so the point will go away after a few "
 "iterations. If you start further out, it may be lost already "
 "after one iteration.</i>"));
   if (!backwards) pplane->draw(pf, 1, &mode);
   pplane->setPoint(0, 0); dplane->setActive(true);
}
if (page == 3)
{  text->setHtml(QString(
 "Now consider the dynamics of f<sub>c</sub>(z) = z<sup>2</sup> + c with "
 "c = -0.125 + 0.5*i. The right image shows three dynamic rays with the "
 "angles 1/7 turns, 2/7 turns, and 4/7 turns. They are preimages of straight "
 "rays under the Boettcher conjugation &Phi;<sub>c</sub>(z). If z belongs "
 "to the dynamic ray with angle 1/7, then f<sub>c</sub>(z) belongs to the "
 "dynamic ray with angle 2/7. This is explained by the conjugation "
 "&Phi;<sub>c</sub>(z) from f<sub>c</sub>(z) to F(z) = z<sup>2</sup>: the "
 "dynamic ray of angle 1/7 is mapped to the straight ray with angle 1/7 by "
 "&Phi;<sub>c</sub>(z), which is mapped to the straight ray with the doubled "
 "angle 2/7 by F(z), which is mapped to the dynamic ray with angle 2/7 by "
 "&Phi;<sub>c</sub><sup>-1</sup>(z)."
 "<p>"
 "Again, the three rays are 3-periodic under f<sub>c</sub>(z), and their "
 "landing points on the Julia set &part;K<sub>c</sub> are 3-periodic as "
 "well. Although the p-periodic points in the Julia set can be computed "
 "only to some precision, the chaotic dynamics is described exactly by "
 "the doubling of rational numbers (with denominator 2<sup>p</sup> - 1). The "
 "fixed point &beta;<sub>c</sub> is always the landing point of the dynamic "
 "ray with angle 0 = 0/1."
 "<p>"
 "<i>Choose a point z with the Arrow keys, or by clicking into the right "
 "window. Map it a few times z &rarr; z<sup>2</sup> + c, by "
 "hitting Return or pushing the Go-button.</i><br>"
 "In the main program, map z with the key f."));
   if (backwards) pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.125, 0.5); dplane->setActive(true);
}
if (page == 4)
{  text->setHtml(QString(
 "Recall the doubling bifurcation from Chapter 2, page 3: for parameters "
 "c in the main cardioid of the Mandelbrot set, the fixed point "
 "&alpha;<sub>c</sub> is attracting and the 2-cycle is repelling. As c moves "
 "through the parabolic parameter -0.75 into the hyperbolic component of "
 "period 2, the three points meet and separate again. The fixed point is now "
 "repelling, and the 2-periodic points are attracting."
 "<p>"
 "The angles 1/3 and 2/3 are 2-periodic under doubling modulo 1: "
 "1/3 &rarr; 2/3 &rarr; 4/3 = 1/3. For c in the main cardioid, the dynamic "
 "rays of angles 1/3 and 2/3 land at the 2-periodic points. In the "
 "doubling bifurcation, the fixed point &alpha;<sub>c</sub> inherits "
 "these rays. It becomes a pinching point in &part;K<sub>c</sub> , while "
 "the 2-periodic points are inside K<sub>c</sub> . When c goes into a "
 "sublimb of the period-2 component, the 2-periodic points will inherit "
 "other rays. But for all c in the 1/2-limb, &alpha;<sub>c</sub> has the "
 "external angles 1/3 and 2/3, i.e., it is the common landing point of the "
 "corresponding dynamic rays."
 "<p>"
 "<i>Hit the Return key or push the Go-button to move the parameter c in "
 "the left image; observe the 2-periodic landing points meet in the right "
 "image. The cross is at z = c.</i>"));
   if (backwards) pplane->draw(pf, 1, &mode); else pFinished();
   pplane->setPoint(-0.5, 0);
}
if (page == 5)
{  text->setHtml(QString(
 "The tripling bifurcation of Chapter 2, page 5, is analogous: as the "
 "parameter c moves from the main cardioid into the 1/3-limb, one of the "
 "two repelling 3-cycles meets with the fixed point "
 "&alpha;<sub>c</sub> , which becomes repelling. The 3-cycle goes inside "
 "K<sub>c</sub> and is attracting. Now &alpha;<sub>c</sub> is a pinching "
 "point of K<sub>c</sub> with the three external angles 1/7, 2/7, and 4/7, "
 "since it has inherited the corresponding dynamic rays. This landing "
 "pattern remains the same for all parameters c in the 1/3-limb. In the "
 "sublimbs of the period-3 component, the 3-cycle has inherited other "
 "angles. Note that the second 3-cycle with the angles 6/7, 5/7, and 3/7 "
 "does not take part in this tripling bifurcation, but stays repelling in "
 "the 1/3-limb. This situation is reversed in the 2/3-limb."
 "<p>"
 "The ray period of a periodic point is the period of the dynamic rays "
 "landing there. Here &alpha;<sub>c</sub> has the period 1 but the ray "
 "period 3. When a p-periodic point has undergone an r-tupling bifurcation, "
 "it will have the ray period rp. The r rays landing at it are permuted "
 "cyclically by the p-th iterate of f<sub>c</sub>(z), and the corresponding "
 "rotation number k/r characterizes the sublimb in the parameter plane. "
 "At the bifurcation point, the argument of the multiplier is 2&pi;*k/r."
 "<p>"
 "<i>Hit the Return key or push the Go-button to move the parameter c in "
 "the left image; observe the 3-periodic landing points meet in the right "
 "image.</i>"));
   if (backwards)
   {  pplane->setNmax(250); dplane->setNmax(250);
      pplane->setPlane(-0.83, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   }
   pplane->draw(pf, 1, &mode); pplane->setPoint(-0.125, 0.5010815528383);
}
if (page == 6)
{  text->setHtml(QString(
 "As the parameter c approaches the root -1.75 of the primitive "
 "period-3 component from the right, the two 3-cycles meet in one parabolic "
 "3-cycle. The parabolic 3-periodic points have two external angles each: "
 "3/7 and 4/7, 6/7 and 1/7, 5/7 and 2/7. As c moves into the hyperbolic "
 "component, there are two 3-cycles again. One is repelling with two "
 "external angles at each point, the other one is attracting and inside "
 "K<sub>c</sub> . As the parameter c moves into a sublimb, the former cycle "
 "keeps its external angles, while the latter cycle inherits other "
 "angles. The common landing point of the dynamic rays with the angles "
 "3/7 and 4/7, e.g., has the period 3 and also the ray period 3, since the "
 "3rd iterate of f<sub>c</sub>(z) sends both rays to themselves, and "
 "does not permute them cyclically. "
 "<p>"
 "<i>Hit Return or push the Go-button to move the parameter c in "
 "the left image; observe the dynamic rays with the angles 3/7 and 4/7 and "
 "their landing points in the right image.</i>"));
   if (!backwards)
   {  pplane->setPlane(-1.7548776662467, 0, 0.047, 0);
      dplane->setPlane(-1.7548776662467, 0, 0.033, 0);
      pplane->setNmax(500); dplane->setNmax(500);
      pplane->draw(pf, 1, &mode);
   }
   pplane->setPoint(-1.7373046875, 0);
}
if (page == 7)
{  text->setHtml(QString(
 "When the parameter c does not belong to the Mandelbrot set, the filled "
 "Julia set K<sub>c</sub> will be totally disconnected. The Boettcher "
 "conjugation &Phi;<sub>c</sub> is not defined everywhere in its exterior. "
 "By taking preimages under f<sub>c</sub>(z), equipotential lines can be "
 "defined everywhere, but they will be a figure-eight, when they go "
 "through the critcal point z = 0 or through one of its preimages. External "
 "rays through 0 or its preimages branch as well."
 "<p>"
 "Now we shall approach the root of the primitive period-3 component from "
 "the exterior of the Mandelbrot set. More specifically, the parameter c "
 "shall belong to the parameter ray with angle 3/7. By the definition of "
 "&Phi;<sub>M</sub>(c), the critical value z = c belongs to the dynamic ray "
 "of angle 3/7. But this ray must be branched, because 3/7 is 3-periodic: "
 "the ray contains a preimage of c under the 3rd iterate of "
 "f<sub>c</sub>(z), which is a preimage of 0 under the 2nd iterate."
 "<p>"
 "The colors in the right image indicate escape lines, which are close to "
 "equipotential lines, and the cross is at a preimage of 0. The dynamic ray "
 "of angle 4/7 goes to a 3-periodic point without branching. The ray of "
 "angle 3/7 branches at an infinite sequence of preimages of 0. By "
 "always taking the branch turning right, we arrive at a 3-periodic point "
 "from the other cycle."
 "<p>"
 "<i>Hit Return or push the Go-button to move z to a preimage of 0, and to "
 "show branches of the dynamic ray with angle 3/7 turns.</i><br>"
 "In the main program, you cannot draw branched dynamic rays, but you can "
 "illustrate them in a few cases: with the drawing algorithm 8 for "
 "parameters c on the parameter rays with angles 1/3, 2/3, 1/7, 2/7, 5/7, "
 "or 6/7, and with algorithm 7 for real c outside of M."));
   if (backwards)
   {  pplane->setPlane(-1.7548776662467, 0, 0.047, 0);
      dplane->setPlane(-1.7548776662467, 0, 0.033, 0);
      pplane->setNmax(500); dplane->setNmax(500);
      pplane->draw(pf, 1, &mode);
   }
   mode = 2; pplane->setPoint(-1.74059862586412, 0.00395966862382012);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For a p-periodic angle, the corresponding dynamic ray of a connected "
 "Julia set lands at a repelling or parabolic point with period "
 "dividing p. The corresponding parameter ray lands at the root of a "
 "hyperbolic component with period p. To show this, consider a parameter "
 "c<sub>0</sub> where the parameter ray accumulates. For parameters c "
 "on this ray, the corresponding dynamic ray branches. If the dynamic "
 "ray for c<sub>0</sub> landed at a repelling periodic point, it "
 "would land without branching also for c &asymp; c<sub>0</sub> by "
 "the Implicit Function Theorem and a pullback argument. This contradiction "
 "shows that the dynamic ray for the parameter c<sub>0</sub> must land "
 "at a parabolic point, and c<sub>0</sub> is a root."
 "<p>"
 "In fact, exactly two parameter rays with periodic angles land at "
 "every root. This was shown by Adrien Douady and John Hamal Hubbard, using "
 "Fatou coordinates for the parabolic implosion (Chapter 2). Combinatorial "
 "proofs were given by "
 "<a href=\"http://arXiv.org/abs/math/9711213\">Dierk Schleicher</a> "
 "and <a href=\"http://arXiv.org/abs/math/9905169\">John Milnor</a>. "
 "For parameters c in "
 "the wake between the two parameter rays, the two dynamic rays with the "
 "same angles land together, and the critical value z = c is always "
 "in the wake between them, even if there are more dynamic rays landing."
 "<p>"
 "The left image shows the parameter rays with the 2-periodic angles 1/3 and "
 "2/3, which land at the root of the period-2 component. The cross "
 "indicates the center c = -1. The right image shows the corresponding "
 "filled Julia set, the \"basilica,\" and the two dynamic rays landing at "
 "the repelling fixed point &alpha;<sub>c</sub> . The cross is at the "
 "critical value z = c. Now you shall see the Julia sets for the centers "
 "corresponding to the p-periodic angles 1/(2<sup>p</sup>-1) and "
 "2/(2<sup>p</sup>-1), and all parameter rays with these denomiantors:<br>"
 "<i>Hit Return or push the Go-button to increase the period (2...9).</i>"));
   if (!backwards)
   {  pplane->setNmax(250); dplane->setNmax(250);
      dplane->setPlane(0, 0, 2.0, 0);
   }
   else string->clear();
   pplane->setPlane(-0.83, 0, 1.6, 0); pplane->draw(pf, 1, &mode);
   pplane->setPoint(-1.0, 0); n = 2;
}
if (page == 9)
{  *string = QString(
 "The parameter rays with the angles 1/7 and 2/7 land at the root of "
 "a period-3 component, which is of satellite type with rotation number 1/3. "
 "For all parameters c in the wake between the rays, in particular for c in "
 "the 1/3-limb of the Mandelbrot set, the dynamic rays with the angles 1/7, "
 "2/7, and 4/7 land together at the repelling fixed point "
 "&alpha;<sub>c</sub> , and the critical value z = c is between the first "
 "two rays. We shall compute the external angles of certain preimages of "
 "&alpha;<sub>c</sub> under f<sub>c</sub>(z). Note that an angle &theta; has "
 "two preimages under doubling modulo 1, &theta;/2 and (&theta;+1)/2."
 "<p>"
 "<i>Hit Return or push the Go-button to move z to a preimage and to "
 "read more (three times).</i>");
   text->setHtml(*string);
   if (backwards) { points = 0; delete[] xp; delete[] yp; xp = 0; yp = 0; }
   pplane->setPlane(-0.122561166876654, 0.744861766619744, 0.55, 0);
   pplane->draw(pf, 1, &mode); dplane->setPlane(0, 0, 2.0, 0); n = 0;
   //pplane->setPoint(-0.122561166876654, 0.744861766619744);
   mode = 16; pplane->setPoint(-0.101096363845622, 0.956286510809142);
}
if (page == 10)
{ text->setHtml(QString(
 "For a Misiurewicz point a in the Mandelbrot set, the critical value is "
 "preperiodic. The parameter rays landing at a have the same "
 "preperiodic angles as the critcal value z = a in K<sub>a</sub> . We have "
 "seen that for all c in the 1/3-limb, the dynamic rays with the angles "
 "9/56, 11/56, and 15/56 land together at a point z, which is mapped "
 "to the repelling fixed point &alpha;<sub>c</sub> by 3 iterations of "
 "f<sub>c</sub>(z). Therefore the corresponding parameter rays land "
 "together at a Misiurewicz point c = a, which has preperiod 3, period 1, "
 "and ray period 3."
 "<p>"
 "For c = a, z = 0 is the only preimage of z = a, and there are six rays "
 "landing at this point. For c &ne; a, the three dynamic rays of above "
 "still land together, but their six preimages land at two "
 "different points. The landing pattern is different for parameters c in "
 "the three branches of the Mandelbrot set at the Misiurewicz point a."
 "<p>"
 "<i>Hit Return or push the Go-button to move the parameter c in "
 "the left image; observe the bifurcation of the landing pattern in the "
 "right image.</i><br>"
 "In the main program, you can observe a small subset of the dynamic plane "
 "for different parameters by choosing it once and going back to the "
 "parameter plane with the command Shift+F2."));
   if (!backwards) string->clear();
   pplane->setPlane(-0.101096363845622, 0.956286510809142, 0.04748, 0);
   pplane->draw(pf, 1, &mode); dplane->setPlane(0, 0, 0.44, 0); n = 0;
   steps = 3; points = 19; xp = new mdouble[points]; yp = new mdouble[points];
   xp[18] = -0.105469938167404; yp[18] = 0.923999523775334;//9
   xp[17] = -0.104576687926597; yp[17] = 0.930115182253905;//24
   xp[16] = -0.103861963046748; yp[16] = 0.935088520375947;//15
   xp[15] = -0.103235009375587; yp[15] = 0.939587436356624;//18
   xp[14] = -0.102609566724054; yp[14] = 0.943968328764448;//12
   xp[13] = -0.102024633532868; yp[13] = 0.9481335500415;//18
   xp[12] = -0.101612280985603; yp[12] = 0.951309080796937;//15
   xp[11] = -0.101266278101407; yp[11] = 0.954218437502389;//18
   xp[10] = -0.101096363845622; yp[10] = 0.956286510809142;//a
   xp[9] = -0.103853330800487; yp[9] = 0.958429644267402;//16
   xp[8] = -0.107158915356992; yp[8] = 0.961550141409282;//13
   xp[7] = -0.110039035747744; yp[7] = 0.964810229598355;//16
   xp[6] = -0.112300573738324; yp[6] = 0.968259715088608;//23~10m
//   xp[6] = -0.111692620380104; yp[6] = 0.968005264187231;//10m
   xp[5] = -0.116556170244492; yp[5] = 0.970790597296549;//25~10m
//   xp[5] = -0.116541650655251; yp[5] = 0.970487916302835;//10m
   xp[4] = -0.118337917300509; yp[4] = 0.976070357122559;//16m
   xp[3] = -0.120158972902325; yp[3] = 0.977704201088661;//16m
   xp[2] = -0.121075678471729; yp[2] = 0.982790073968401;//7m
   xp[1] = -0.124499338581119; yp[1] = 0.985362411279769;//23~7m
//   xp[1] = -0.124691510009524; yp[1] = 0.985896880770954;//7m
   xp[0] = -0.127499973546363; yp[0] = 0.987460909489457;//7
   pplane->setPoint(xp[18], yp[18]);
}
if (page == 11)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The 1/3-limb of the Mandelbrot set is contained in the wake bounded by "
 "the parameter rays with the angles 1/7 and 2/7. For all parameters c in "
 "this wake, the dynamic rays with the angles 1/7, 2/7, and 4/7 land "
 "together at the fixed point &alpha;<sub>c</sub> , which is repelling with "
 "combinatorial rotation number 1/3. On the previous pages, you have seen "
 "that some preimages z<sub>c</sub> of &alpha;<sub>c</sub> are stable in "
 "the sense that they depend "
 "<a href=\"javascript:alert('Analytic or holomorphic means complex "
    "differentiable, see page 1 of Chapter 8.')\">analytically</a> "
 "on c and have the same external angles for all parameters c in "
 "the wake. This is the case when they are never iterated to the dynamic "
 "wake between the rays with the angles 1/7 and 2/7. Otherwise the critical "
 "value will be at an image of the point z<sub>c</sub> for a suitable "
 "Misiurewicz parameter c, and z<sub>c</sub> exchanges external rays "
 "with another preimage of &alpha;<sub>c</sub> (in fact, both are not "
 "distinguishable everywhere)."
 "<p>"
 "The stable preimages of &alpha;<sub>c</sub> form a "
 "graph, whose edges are connected components of the filled Julia set "
 "K<sub>c</sub> without these points, with three edges at every vertex. The "
 "central edge connects &alpha;<sub>c</sub> and -&alpha;<sub>c</sub> . "
 "It is mapped to the critcal wake by f<sub>c</sub>(z), which shows that it "
 "does not contain another stable preimage of &alpha;<sub>c</sub> . On the "
 "other hand, every edge is mapped to this edge after a finite number of "
 "iterations without meeting z = 0, thus its vertices are mapped to "
 "&plusmn;&alpha;<sub>c</sub> without passing through the critical wake. The "
 "mapping of dynamic edges is discussed further on page 9 of Chapter 4. You "
 "may explore it for the \"rabbit\" on page 4 of Chapter 1 as well."
 "<p>"
 "There is a unique correspondence between the stable preimages of "
 "&alpha;<sub>c</sub> in the critical wake and certain &alpha;-type "
 "Misiurewicz points in the 1/3-limb. Again, these form the vertices of a "
 "graph, joined by "
 "<a href=\"http://www.mndynamics.com/indexm.html#edge\">parameter edges</a>. "
 "A dynamic edge has the order n, when it is mapped 1:1 to the central edge "
 "with n - 1 iterations, thus mapped 2:1 to the critical wake with n "
 "iterations. The correponding parameter edge of order n contains a maximal "
 "primitve copy of the Mandelbrot set with period n, and no hyperbolic "
 "component of a smaller period."
 "<p>"
 "In both planes, the external angles of the vertices are determined from a "
 "recursive subdivision: in each step, a sector is divided into a strip and "
 "two smaller sectors by three external rays landing at one vertex. The "
 "strip marks an edge, and the sectors are divided further. The "
 "interval of angles is subdivided in the ratios 1:2:4:1. In the images "
 "below, the white sectors are subdivided by the red rays already.<br>"
 "<i>Hit the Return key or push the Go-button twice to see the subdivision "
 "of sectors by more external rays. The new strips mark parameter "
 "edges or dynamic edges, respectively.</i>"));
   if (!backwards) { points = 0; delete[] xp; delete[] yp; xp = 0; yp = 0; }
   pplane->setPlane(-0.1137, 0.8880, 0.2562, 0); n = 1;
   dplane->setPlane(-0.19554, 0.85701, 0.28626, 0); mode = 16;
   pplane->setPoint(-0.153940779, 1.037700196); pplane->draw(pf, 1, &mode);
}
if (page == 12)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "On page 10, you have observed two preimages of the stable vertex with the "
 "external angles 9/56, 11/56, and 15/56. When the "
 "parameter c is behind the corresponding Misiurewicz point a, these two "
 "points separate &alpha;<sub>c</sub> and -&alpha;<sub>c</sub> from "
 "each other. They mark the central frame on the central edge of "
 "order 1. The left image shows the parameter edge of order 4 attached to a, "
 "and the right image shows the corresponding dynamic edge for some "
 "parameter c. The six red rays mark a strip and two sectors "
 "containing a dynamic frame of order 4. It consists of the part between the "
 "two landing points and two additional arms, whose direction would be "
 "reversed for parameters c in the right branch behind a."
 "<p>"
 "By n - 1 iterations, a dynamic frame of order n is mapped 1:1 to the "
 "central frame without meeting the critical sector, and by the n-th "
 "iterate, it is mapped 2:1 to the branches behind the stable dynamic vertex "
 "with the external angles 9/56, 11/56, and 15/56. Thus "
 "the landing points at a dynamic frame are stable for parameters c in a "
 "branch behind a, but not for all c in the 1/3-limb. Again, "
 "<a href=\"http://www.mndynamics.com/indexm.html#edge\">parameter frames</a> "
 "correspond to dynamic frames, and a parameter frame of order 4 is "
 "marked in the left image."
 "<p>"
 "The central edge consists of the central frame of order 1 and two "
 "additional components, which are mapped 1:1 onto the whole edge by the "
 "third iterate of f<sub>c</sub>(z). Thus each of these contains a frame of "
 "order 4. There are four more components between these three frames and two "
 "vertices, each of which contains a frame of order 7 ... See also page 8 of "
 "Chapter 8. The hierarchy of frames fills out the edge except for a subset "
 "of a Cantor set. On the dynamic edge or the parameter edge of order 4 "
 "shown here, there is one frame of order 4, two of order 7, 4 of order "
 "10 ... Here the strip between two frames, or between a frame and a vertex "
 "of the edge, is subdivided into three strips and two sectors. The strip in "
 "the middle and two sectors contain the new frame, and the remaining two "
 "strips may be subdivided further. On both sides, the interval of angles is "
 "subdivided in the ratios 1:2:4:1 again."
 "<p>"
 "<i>Hit the Return key or push the Go-button twice to see the subdivision "
 "of strips by more external rays, marking parameter frames or "
 "dynamic frames, respectively.</i>"));
   pplane->setPlane(-0.14216, 1.02128, 0.0726, 0); n = 1;
   dplane->setPlane(-0.16174078, 1.02030020, 0.066864, 0); mode = 16;
   pplane->setPoint(-0.153940779, 1.037700196); pplane->draw(pf, 1, &mode);
}
} //showPage ER

void QmnDemoER::go()
{  if (page == 2 || page == 3)
   {  mdouble x, y; dplane->getPoint(x, y);
      df->iterate(x, y); dplane->setPoint(x, y);
   }
   if (page == 4)
   {  mdouble x, y; pplane->getPoint(x, y);
      x -= 0.0078125; if (x < -0.99) goButton->setEnabled(false);
      pplane->setPoint(x, y);
   }
   if (page == 5)
   {  mdouble x, y; pplane->getPoint(x, y);
      y += 0.00390625; if (y > 0.745) goButton->setEnabled(false);
      pplane->setPoint(x, y);
   }
   if (page == 6)
   {  mdouble x, y; pplane->getPoint(x, y);
      x -= 0.00048828125; if (x < -1.7545) goButton->setEnabled(false);
      pplane->setPoint(x, y); dplane->setPoint(x, y);
   }
   if (page == 7)
   {  mdouble x, y, x0, y0, x1, y1; dplane->getPoint(x, y); x0 = x; y0 = y;
      df->iterate(x, y, -1); df->iterate(x, y, -2); df->iterate(x, y, -1);
      x1 = 2*x0 - x; y1 = 2*y0 - y;
      if (y0 < 0.002)
      {  x = -1.75082776241; y = 0.01168496152;
         x1 = -1.74802433; y1 = -0.00865548;
      }
      if (x0 < -1.750827) { x1 = -1.75592056004; y1 = 0.01268654863; }
      if (y0 > 0.013337) { x1 = -1.74590779882; y1 = 0.01562722351; }
      if (x0 > -1.743635) { x1 = -1.74275535205; y1 = 0.01210847673; }
      dplane->drawLine(x0, y0, x, y); dplane->drawLine(x0, y0, x1, y1);
      dplane->setPoint(x, y);
   }
   if (page == 8)
   {  n++; if (n == 9) goButton->setEnabled(false);
      int j; mdouble x = 0, y = 0, u = pow(2.0, n) - 1.0;
      pf->bifurcate(1.0/n, x, y); pf->find(1, 0, n, x, y);
      pplane->setPoint(x, y); dplane->setPoint(x, y);
      for (j = 1; j < u; j++)
         pplane->traceRay(1, mdouble(j)/u, pf, x, y, 1);
   }
   if (page == 9)
   {  n++; if (n == 3) goButton->setEnabled(false);
      mdouble t1 = 9.0/14.0, t2 = 11.0/14.0, t3 = 1.0/14.0, x, y;
      dplane->getPoint(x, y);
      if (n == 1) df->iterate(x, y, -2); else df->iterate(x, y, -1);
      dplane->setPoint(x, y); QColor color = Qt::blue;
      if (n == 1)
      {  *string += QString(
 "<p>z = -&alpha;<sub>c</sub> is the only preimage of &alpha;<sub>c</sub> "
 "different from the fixed point itself. The angle 1/7 has the preimages "
 "(1/7)/2 = 1/14 and (1/7 + 1)/2 = 4/7. The latter angle belongs to "
 "&alpha;<sub>c</sub> , so 1/14 is an external angle of "
 "-&alpha;<sub>c</sub> . In the same way, the other angles 9/14 and 11/14 "
 "are obtained. The rays are drawn blue.");
         text->setHtml(*string); text->verticalScrollBar()->setValue(10000);
      }
      if (n == 2)
      {  *string += QString(
 "<p>Move z to that preimage of -&alpha;<sub>c</sub> between the rays for "
 "2/7 and 4/7. The angle 1/14 has the preimages (1/14)/2 = 1/28 and "
 "(1/14 + 1)/2 = 15/28. Only the latter angle is in the chosen interval. "
 "The other two external angles of z are 9/28 and 11/28. The rays are "
 "drawn magenta.");
         text->setHtml(*string); text->verticalScrollBar()->setValue(10000);
         t1 = 9.0/28.0; t2 = 11.0/28.0; t3 = 15.0/28.0; color = Qt::magenta;
      }
      if (n == 3)
      {  *string += QString(
 "<p>Now z is the preimage between the rays for 1/7 and 2/7. By taking "
 "preimages in this interval, the external angles 9/56, 11/56, and 15/56 "
 "are obtained. The rays are drawn red. Rays with preperiodic angles, i.e., "
 "even denominators, land at preperiodic points in the dynamic "
 "plane, or at Misiurewicz points in the parameter plane. For these "
 "parameters, the critcal value is preperiodic under the iteration of "
 "f<sub>c</sub>(z).");
         text->setHtml(*string); text->verticalScrollBar()->setValue(10000);
         t1 = 9.0/56.0; t2 = 11.0/56.0; t3 = 15.0/56.0; color = Qt::red;
      }
      dplane->traceRay(-1, t1, df, x, y, 2, color);
      dplane->traceRay(-1, t2, df, x, y, 2, color);
      dplane->traceRay(-1, t3, df, x, y, 2, color);
      if (n == 3)
      {  pplane->traceRay(1, t1, pf, x, y, 2, color);
         pplane->traceRay(1, t2, pf, x, y, 2, color);
         pplane->traceRay(1, t3, pf, x, y, 2, color);
      }
   }
   if (page == 10) polyline();
   if (page == 11)
   {  int j; mdouble x, y, N = 2.0, D = 14.0; QColor color = Qt::white;
      while (1)
      {  if (n == 1) { N = 9.0; D = 56.0; color = Qt::red;}
         if (n == 2) { N = 23.0; D = 112.0; color = Qt::blue; }
         if (n == 3) { N = 37.0; D = 224.0; }
         if (n == 4) { N = 51.0; D = 224.0; color = Qt::magenta; }
         if (n == 5) { N = 93.0; D = 448.0; }
         if (n == 6) N = 79.0;
         if (n == 7) { N = 149.0; D = 896.0; }
         for (j =0; j <= 4; j += 2)
         {  N += j; pplane->traceRay(1, N/D, pf, x, y, 3, color);
            dplane->traceRay(-1, N/D, df, x, y, 3, color);
         }
         n++; if (n == 8) goButton->setEnabled(false);
         if (n == 2 || n == 4 || n == 8) return;
      }
   }
   if (page == 12)
   {  int j; mdouble x, y, M = 18.0, N = 23.0, D = 112.0;
      QColor color = Qt::white;
      while (1)
      {  if (n == 1) { M = 177.0; N = 233.0; D = 896.0; color = Qt::red; }
         if (n == 2) { M = 1409.0; N = 1913.0; D = 7168.0; color = Qt::blue; }
         if (n == 3) { M = 1465.0; N = 1857; }
         if (n == 4)
         { M = 11265.0; N = 15353.0; D = 57344.0; color = Qt::magenta; }
         if (n == 5) { M = 11321.0; N = 15297.0; }
         if (n == 6) { M = 11713.0; N = 14905.0; }
         if (n == 7) { M = 11769.0; N = 14849.0; }
         for (j =0; j <= 4; j += 2)
         {  M += j; N += j; if (!n) M += j; //D is really 56.0
            pplane->traceRay(1, M/D, pf, x, y, 3, color);
            dplane->traceRay(-1, M/D, df, x, y, 3, color);
            pplane->traceRay(1, N/D, pf, x, y, 3, color);
            dplane->traceRay(-1, N/D, df, x, y, 3, color);
         }
         n++; if (n == 8) goButton->setEnabled(false);
         if (n == 2 || n == 4 || n == 8) return;
      }
   }
} //go ER

void QmnDemoER::drawRight()
{  mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   if (page <= 3 || page == 7) dplane->setPoint(0, 0);
   if ((page >= 4 && page <= 6) || page == 8|| page >= 11)
      dplane->setPoint(x, y);
   if (page == 9)
   { x = -0.3; y = 0.5; df->find(-1, 0, 1, x, y); dplane->setPoint(x, y); }
   dplane->draw(df, -1, &mode, (page == 1 || page == 10 ? 1 : 0) );
}

void QmnDemoER::pFinished()
{  int j; mdouble x, y;
   if (page == 1)
   {  for (j = 0; j < 16; j++) pplane->traceRay(1, mdouble(j)/16.0, pf, x, y, 2);
      for (y = 1.35; y > 0.39; y *= 0.78)
         pplane->green(pf, 1, pf->green(1, -1.0, y), 3);
   }
   if (page == 4 || page == 8)
   {  pplane->traceRay(1, 1.0/3.0, pf, x, y, 2);
      pplane->traceRay(1, 2.0/3.0, pf, x, y, 2);
   }
   if (page == 5 || page == 9)
   {  pplane->traceRay(1, 1.0/7.0, pf, x, y, 2);
      pplane->traceRay(1, 2.0/7.0, pf, x, y, 2);
   }
   if (page == 6 || page == 7)
   {  pplane->traceRay(1, 3.0/7.0, pf, x, y, 2);
      pplane->traceRay(1, 4.0/7.0, pf, x, y, 2);
   }
   if (page == 8) pplane->traceRay(1, 0.0, pf, x, y, 2);
   if (page == 10)
   {  pplane->traceRay(1, 9.0/56.0, pf, x, y, 2, Qt::red);
      pplane->traceRay(1, 11.0/56.0, pf, x, y, 2, Qt::red);
      pplane->traceRay(1, 15.0/56.0, pf, x, y, 2, Qt::red);
   }
   if (page >= 11) { if (!n) go(); else n = 0; }
}

void QmnDemoER::dFinished()
{  int j; mdouble x, y;
   if (page == 1)
   {  df->getParameter(x, y);
      for (j = 0; j < 16; j++)
         dplane->backRay((unsigned long long int)(j), 16LL, x, y, 2);
      for (y = 1.95; y > 0.8; y *= 0.862)
         dplane->green(df, -1, df->green(-1, 0, y), 2);
   }
   if (page == 2 || page == 3 || page == 5 || page == 9)
   {  dplane->traceRay(-1, 1.0/7.0, df, x, y, 2);
      dplane->traceRay(-1, 2.0/7.0, df, x, y, 2);
      dplane->traceRay(-1, 4.0/7.0, df, x, y, 2);
   }
   if (page == 4)
   {  dplane->traceRay(-1, 1.0/3.0, df, x, y, 2);
      dplane->traceRay(-1, 2.0/3.0, df, x, y, 2);
   }
   if (page == 6 || page == 7)
   {  df->getParameter(x, y);
      dplane->backRay(3LL, 7LL, x, y, 2);
      dplane->backRay(4LL, 7LL, x, y, 2);
   }
   if (page == 7)
   {  //dplane->setPoint(-1.74158910421, -0.00703803701); dplane->move(5);
      //dplane->setPoint(-1.74508559227, 0.01011069536); dplane->move(5);
      dplane->setPoint(-1.74926253035, 0.00156074859);
   }
   if (page == 8)
   {  mdouble u = pow(2.0, n) - 1.0;
      dplane->traceRay(-1, 1.0/u, df, x, y, 2);
      dplane->traceRay(-1, 2.0/u, df, x, y, 2);
   }
   if (page == 10)
   {  dplane->traceRay(-1,  9.0/112.0, df, x, y, 2);
      dplane->traceRay(-1, 11.0/112.0, df, x, y, 2);
      dplane->traceRay(-1, 15.0/112.0, df, x, y, 2);
      dplane->traceRay(-1, 65.0/112.0, df, x, y, 2);
      dplane->traceRay(-1, 67.0/112.0, df, x, y, 2);
      dplane->traceRay(-1, 71.0/112.0, df, x, y, 2);
   }
   if (page >= 11) { if (!n) go(); else n = 0; }
} //dFinished ER

////////////////////////////////////////////////////////////////////////

void QmnDemoCC::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1; n = 0;
/* Three alternative ways of encoding overline:
   <style type=\"text/css\"> *.per { text-decoration: overline } </style>
   1/7 = .<nobr class=\"per\">001</nobr>
   <style type=\"text/css\"> em { font-style:normal;
          text-decoration: overline } </style>
   1/7 = .<em>001</em>
   1/7 = .<nobr style=\"text-decoration:overline\">001</nobr>
*/
if (page == 1)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "The external angle 11/31 turns is 5-periodic under doubling modulo 1. Its "
 "binary expansion is given by 11/31 = .<nobr class=\"per\">01011</nobr>, "
 "since 11 = 0*16 + 1*8 + 0*4 + 1*2 + 1*1 and 31 = 2<sup>5</sup> - 1. "
 "Doubling means that the digits are shifted to the left, e.g., "
 "2*11/31 = 22/31 = .<nobr class=\"per\">10110</nobr>. There are two kinds "
 "of rational angles:<ul>"
 "<li>When the denominator is odd, the sequence of binary digits will be "
 "periodic, and the angle is periodic under doubling. The dynamic ray with "
 "this angle lands at a periodic point of the Julia set. The parameter "
 "ray lands at the root of a hyperbolic component.</li>"
 "<li>When the denominator is even, the sequence of binary digits will be "
 "preperiodic, and the angle is preperiodic under doubling. The dynamic ray "
 "lands at a preperiodic point of the Julia set, and the parameter ray "
 "lands at a Misiurewicz point.</li>"
 "</ul>This chapter concentrates on combinatorial descriptions of Julia "
 "sets with attracting periodic points and of hyperbolic components. The "
 "main example will be the primitive component of period 5 in the "
 "1/3-sublimb of the period-2 component. The parameter c is the center of "
 "this hyperbolic component. The dynamic rays with the angles 11/31 (red) "
 "and 12/31 (magenta) land at a repelling 5-periodic point on the "
 "boundary of the component around the superattracting 5-periodic critical "
 "value z = c. The right image shows two yellow rays as well, with the "
 "angles 0 and 1/2, landing at the fixed point &beta;<sub>c</sub> and at its "
 "preimage -&beta;<sub>c</sub> . Dynamic rays in the upper part between "
 "these two rays have an angle between 0 and 1/2, the first binary digit "
 "is 0. Angles in the lower part are between 1/2 and 1, with first digit 1. "
 "When the red ray is mapped with the quadratic polynomial f<sub>c</sub>(z), "
 "the angle is doubled and the sequence of binary digits is shifted. The "
 "new ray is in the lower part between the yellow rays, thus the first digit "
 "of the new angle, and the second digit of the original angle, is 1. "
 "Iterating the mapping of the ray, the digits of "
 "11/31 = .<nobr class=\"per\">01011</nobr> are recovered. For the "
 "magenta ray, some images are on the opposite side, and the angle is "
 "12/31 = .<nobr class=\"per\">01100</nobr>. The corresponding parameter "
 "rays land at the root of the period-5 hyperbolic component."
 "<p>"
 "When the external angles of a hyperbolic component are not known, they "
 "can be determined from the following "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0603.30030\">algorithm</a>: draw the filled Julia set K<sub>c</sub> , "
 "locate &plusmn;&beta;<sub>c</sub> and the two accesses to the 5-periodic "
 "point. Follow the orbit of these accesses and note the digits 0 or 1 "
 "according to the upper or lower sides of the Julia set."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to follow the "
 "critical orbit in the right window. Observe the red and magenta "
 "rays.</i>"));
   if (!backwards)
   {  spider = new QPixmap((const char **)(QmnSpider16));
      pplane->setNmax(250); dplane->setNmax(250);
      dplane->setPlane(0, 0, 1.9, 0);
      df->setParameter(-1.2563679300, 0.3803209635);
   }
   pplane->setPlane(-0.83, 0, 1.6, 0);
   pplane->setPoint(-1.2563679300, 0.3803209635); dplane->setPoint(0, 0);
   pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
if (page == 2)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "John Hamal Hubbard showed that the combinatorics of a hyperbolic component "
 "are encoded in a tree (a simply connected graph). Join the periodic "
 "critical orbit z<sub>0</sub> = 0, z<sub>1</sub> = c, z<sub>2</sub> , "
 "z<sub>3</sub> , z<sub>4</sub> by arcs within the filled Julia set "
 "K<sub>c</sub> , representing a planar graph H. It has the following "
 "properties:<ul>"
 "<li>Every endpoint belongs to the critical orbit. "
 "Additional branch points are marked as well.</li>"
 "<li>The map f : H &rarr; H is orientation-preserving at branch points. "
 "It is at most 2:1, and injective (1:1) on each branch of H at "
 "z<sub>0</sub> . f and f<sup>-1</sup> (on a branch) are continuous.</li>"
 "<li>The map f is expanding: iterating the arc between two marked "
 "points yields an arc through or at the critical point z<sub>0</sub> .</li>"
 "</ul>Any Hubbard tree characterizes a pair of periodic angles and a "
 "hyperbolic component uniquely. There are several "
 "<a href=\"https://www.mittag-leffler.se/preprints/\">algorithms</a> "
 "relating different representations:<ul>"
 "<li>Starting from a periodic angle &theta;, the conjugate angle and the "
 "Hubbard tree H are constructed combinatorially. The center c is obtained "
 "from the spider algorithm, see page 6.</li>"
 "<li>Starting from the Hubbard tree H, the angles are obtained analogously "
 "to the previous page: first you can figure out the location of "
 "&plusmn;&beta;, i.e., behind which marked point there is a fixed point. "
 "Then you know which \"side\" of H corresponds to the digit 0 or 1. Follow "
 "the orbit and note the digits. From the angles, c is obtained with the "
 "spider algorithm again.</li>"
 "<li>When the parameter c of a center is given, draw the filled Julia set "
 "K<sub>c</sub> and read off the angles or the Hubbard tree. You may need "
 "to magnify subsets carefully. Or use the inverse spider algorithm.</li>"
 "</ul><i>Hit the Return key or push the Go-button repeatedly to observe "
 "the critical orbit on the Hubbard tree in the left window, and on the "
 "filled Julia set in the right window.</i><br>"
 "In the main program, you can mark the critical orbit with f and # or "
 "by drawing the orbit portrait of periodic dynamic rays with o."
 "(When you give this command from the parameter plane, the orbit "
 "portrait will be symbolized as a lamination by circular arcs.)"));
   if (!backwards)
   {  pplane->setPlane(10.0, 0, 1.0, 0);
      pplane->draw(pf, -1, &mode); //green
   }
   pplane->setPoint(0, 0); dplane->setPoint(0, 0);
   dplane->draw(df, -1, &mode);
}
if (page == 3)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The number of arms at a point of the Hubbard tree can decrease with the "
 "map f at the critcal point z<sub>0</sub> only, where f is locally "
 "2:1. Thus z<sub>1</sub> ... have one arm, and ... z<sub>0</sub> have two "
 "arms. (When c is in a satellite component of the main cardioid, all points "
 "z<sub>i</sub> have only one arm.) In the filled Julia set K<sub>c</sub> , "
 "the points of the critcal orbit are contained in components with "
 "infinitely many branches attached. For the component containing the "
 "critical value z<sub>1</sub> = c, only the branch before this component "
 "(the one containing 0) "
 "meets the other points of the critical orbit. At most two "
 "branches at the component containing the critical point z<sub>0</sub> = 0 "
 "meet the critical orbit. A similar description applies to the repelling "
 "periodic points w<sub>0</sub> , w<sub>1</sub> ... on the boundary of these "
 "components, where w<sub>1</sub> is before z<sub>1</sub> and "
 "&plusmn;w<sub>0</sub> are on the arc joining &plusmn;&alpha;<sub>c</sub> . "
 "The external angles of w<sub>1</sub> are the same as those of the "
 "corresponding root in the parameter plane. For all parameters behind this "
 "root, the <a href=\"javascript:alert('The periodic point can be continued "
 "analytically for all parameters c in the wake between the two parameter "
 "rays landing at the root. Equivalently, it does not undergo a bifurcation "
 "by meeting another periodic point. (An analytic map is complex "
 "differentiable, see Chapter 8.)')\">analytic</a> continuation of "
 "w<sub>1</sub> keeps its external angles, and the critical value is behind "
 "it. (A point x is \"before\" y, or y \"behind\" x, if x is on an arc from "
 "0 to y.)"
 "<p>"
 "More generally, a periodic pinching point w<sub>1</sub> on the arc from "
 "&alpha;<sub>c</sub> to c in K<sub>c</sub> is called "
 "characteristic, if none of its iterates is behind it. There is a unique "
 "correspondence between these points and the roots or hyperbolic components "
 "before c. Now Lavaurs' Lemma is obtained as follows: when there are two "
 "characteristic points of the same period, join them by an arc and iterate "
 "it. By the expanding property, some iterate contains 0, and the next "
 "iterate covers the original arc. Thus there is a characteristic point of "
 "lower period between them. In the parameter plane, there will be a "
 "hyperbolic component of lower period between two components of equal "
 "periods."
 "<p>"
 "William Thurston has <a href=\"http://arxiv.org/abs/math/9711213\">"
 "shown</a> that every branch point of a filled Julia set is (pre-)periodic "
 "or a preimage of the critical value. The corresponding Branch Theorem in "
 "the parameter plane applies to two centers or Misiurewicz points, where "
 "none is behind the other one. Then they are in different sublimbs of a "
 "hyperbolic component, or in different branches of a Misiurewicz point. In "
 "particular, every branch point of the Mandelbrot set is a Misiurewicz "
 "point. Moreover, a non-hyperbolic component would have at most two "
 "parameter rays accumulating at its boundary, and it cannot exist if M "
 "is locally connected."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to observe "
 "the critical orbit on the Hubbard tree in the left window.</i>"));
   pplane->setPoint(0, 0); dplane->setPoint(3.0, 0); //invisible
   dplane->draw(df, -1, &mode);
}
if (page == 4)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "For real quadratic polynomials, the combinatorics is encoded in the "
 "kneading sequence: the symbols A, B, * denote that the corresponding "
 "iterate of the critical value z = c is left or right of the critical point "
 "z = 0, or coincides with it. In the complex case, the arms of the Hubbard "
 "tree at z<sub>0</sub> are labeled A and B, such that A contains "
 "z<sub>1</sub> . The kneading sequence in our example is "
 "<nobr class=\"per\">ABAB*</nobr>, since z<sub>1</sub> and z<sub>3</sub> "
 "belong to arm A, z<sub>2</sub> and z<sub>4</sub> belong to arm B. The "
 "kneading sequence can be obtained from the angle &theta; = 11/31 as well: "
 "its preimages 11/62 and 21/31 divide the circle into two intervals, and "
 "the symbols A and B denote which interval contains an iterate of &theta; "
 "under doubling. The kneading sequence can be obtained directly from the "
 "critical orbit in the dynamic plane, but we may need to magnify a "
 "neighborhood of z = 0 carefully, when the filled Julia set K<sub>c</sub> "
 "spirals around z = 0 or branching close to it. Note that only the two "
 "large arms can contain points of the critical orbit. For the same reason, "
 "we could use the preimages of the other angle 12/31 as well: the critical "
 "orbit (except 0) and the periodic rays will not meet the strip between the "
 "four rays at the central component."
 "<p>"
 "The internal address is a \"human-readable\" encoding of the kneading "
 "sequence, consisting of an increasing sequence of periods, here 1-2-5. "
 "These correspond to the hyperbolic components of lowest periods on the arc "
 "from the main cardioid to the period 5 considered here. In the dynamics, "
 "it is a sequence of cutting times: iterate the arc from z<sub>0</sub> to "
 "z<sub>1</sub> , chop off the tail at z<sub>0</sub> when the arc contains "
 "the critical point. There are two simple "
 "<a href=\"http://arXiv.org/abs/math/9411238\">algorithms</a>, since the "
 "n-th entry of the kneading sequence will change at a hyperbolic "
 "component of period n:<ul>"
 "<li>When the kneading sequence <nobr class=\"per\">ABAB*</nobr> is given, "
 "note that it differs from <nobr class=\"per\">A</nobr> at the second place "
 "and from the 2-periodic <nobr class=\"per\">AB</nobr> at the fifth place, "
 "thus the internal address is 1-2-5.</li>"
 "<li>When the internal address 1-2-5 is given, start with the 1-periodic "
 "kneading sequence <nobr class=\"per\">A</nobr>. Modify it at the second "
 "place, which gives <nobr class=\"per\">AB</nobr>, and at the fifth place, "
 "which gives <nobr class=\"per\">ABABB</nobr>. This is the upper kneading "
 "sequence, while <nobr class=\"per\">ABABA</nobr> is the lower kneading "
 "sequence, and <nobr class=\"per\">ABAB*</nobr> is the kneading sequence "
 "for the center.</li>"
 "</ul>The <a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:03409670\">algorithm</a> of Metropolis-Stein-Stein gives the "
 "hyperbolic component of smallest period between two roots or Misiurewicz "
 "points: take the upper kneading sequence of the lower parameter and the "
 "lower kneading sequence of the upper parameter. Suppose they differ at "
 "position n for the first time. Then n will be the smallest period on the "
 "arc, and putting a * at that position gives the kneading sequence of the "
 "corresponding center."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to observe "
 "the critical orbit on the Hubbard tree in the left window, and on the "
 "filled Julia set in the right window.</i><br>"
 "In the main program, the commands a and b for the branches of "
 "the inverse map shall map to the parts A and B. This will not always "
 "work, since the border is taken as a straight line orthogonal to c. Use "
 "the key Ctrl+x to compute the kneading sequence and the internal address "
 "from a given angle."));
   if (backwards)
   {  dplane->setPlane(0, 0, 1.9, 0); pplane->setPlane(10.0, 0, 1.0, 0);
      pplane->draw(pf, -1, &mode); //green
      df->setParameter(-1.2563679300, 0.3803209635);
   }
   pplane->setPoint(-1.0, 1.0); dplane->setPoint(-1.2563679300, 0.3803209635);
   dplane->draw(df, -1, &mode);
}
if (page == 5)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "If a kneading sequence or internal address is realized by a parameter c on "
 "the real line, then c will be unique. Otherwise, it may be realized by "
 "different complex polynomials, or not at all. Henk Bruin and Dierk "
 "Schleicher have given an intuitive "
 "<a href=\"http://arXiv.org/abs/0801.4662\">criterion</a>: there is a "
 "unique correspondence between *-periodic kneading sequences or finite "
 "internal addresses on the one hand, and abstract Hubbard trees with "
 "periodic critcal orbit on the other hand. These are not imbedded into "
 "the plane. There are two cases:<ul>"
 "<li>If the arms at every periodic branch point are permuted "
 "<a href=\"javascript:alert('A transitive permutation of r objects means "
 "that each one returns to its place in r iterations, after visiting all "
 "the other places.')\">transitively</a> "
 "by the first iterate of f returning, then the abstract Hubbard tree can be "
 "embedded into the plane in various ways, such that f becomes "
 "orientation-preserving. Each embedding corresponds to a unique hyperbolic "
 "component realizing these combinatorics.</li>"
 "<li>If the lower arm at a characteristic branch point is mapped to itself "
 "by the first-return map, there will be no suitable embedding of the "
 "abstract Hubbard tree, and the combinatorics is not realized by a "
 "quadratic polynomial.</li>"
 "</ul>The simplest example is given by the kneading sequence "
 "<nobr class=\"per\">ABAAB*</nobr> or the internal address 1-2-4-5-6. The "
 "abstract Hubbard tree is shown in the left window. The 3-cycle of branch "
 "points is \"evil,\" since it fails the admissibility condition of period "
 "3. To check this condition on the kneading sequence, it is not necessary "
 "to construct the abstract Hubbard tree. We only need to evaluate the "
 "function &rho;, where &rho;(k) is the first integer n, where the kneading "
 "sequence differs from the k-periodic continuation of the first k entries:"
 "<ul><li>3 is not contained in the internal address 1-2-4-5-6.</li>"
 "<li>For all smaller divisors k of 3, &rho;(k) &le; 3. Here "
 "&rho;(1) = 2 &le; 3.</li>"
 "<li>When r &le; 3 is congruent to &rho;(3) modulo 3, its &rho;-orbit "
 "must contain 3. This applies to r = 3.</li>"
 "</ul>Without constructing the abstract Hubbard tree, we know that it "
 "cannot be embedded into the plane, such that f will be "
 "orientation-preserving at the 3-cycle of branch points."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to observe the "
 "critical orbit on the abstract Hubbard tree in the left window.</i><br>"
 "In the main program, you can convert kneading sequences and internal "
 "addresses with the key k, check the admissibility condition, and "
 "<a href=\"http://www.mndynamics.com/indexm.html#combi\">determine</a> "
 "external angles. The construction of Hubbard trees will be "
 "implemented later."));
   if (backwards)
   { delete[] xp; delete[] yp; xp = 0; yp = 0; pplane->setCursorPix(0); }
   dplane->setPlane(10.0, 0, 1.0, 0); pplane->setPlane(10.0, 0, 1.0, 0);
   pplane->draw(pf, -1, &mode); dplane->draw(df, -1, &mode); //green
   pplane->setPoint(0, 0);
}
if (page == 6)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For a center c of period n, the critical orbit z<sub>0</sub> = 0, "
 "z<sub>1</sub> = c, ... satisfies the system of equations<br>"
 "z<sub>n-1</sub> = &plusmn;&radic;(0 - z<sub>1</sub>) , "
 "z<sub>n-2</sub> = &plusmn;&radic;(z<sub>n-1</sub> - z<sub>1</sub>) , ... , "
 "z<sub>1</sub> = &plusmn;&radic;(z<sub>2</sub> - z<sub>1</sub>) .<br>"
 "Considering real quadratic polynomials, there is a unique correspondence "
 "between the pairs of periodic external angles at roots, the kneading "
 "sequences or internal addresses, and the linear order of the periodic "
 "critical orbit. When the initial values follow this order, iterating the "
 "system of equations (with the appropriate choice of signs &plusmn; "
 "according to the kneading sequence) will converge to the superattracting "
 "n-cycle, with z<sub>1</sub> converging to the center parameter c."
 "<p>"
 "In the complex case, finding the center c corresponding to a periodic "
 "angle &theta; is more involved, since the choice of the correct branch of "
 "the square-roots is not obvious. The "
 "<a href=\"http://www.math.cornell.edu/~hubbard/SpidersFinal.pdf\">spider "
 "algorithm</a> employs curves from the points z<sub>i</sub> to &infin; "
 "with the required angles 2<sup>i-1</sup>&theta; of dynamic rays. In each "
 "step of the iteration, the correct branches are chosen either such that "
 "the circular order of these curves is preserved, or equivalently, such "
 "that they keep their angles at &infin;. Again, z<sub>1</sub> will "
 "converge to the center c. (For a Misiurewicz point corresponding to a "
 "preperiodic angle &theta;, the algorithm may fail to converge due to a "
 "<a href=\"http://www.math.cornell.edu/~hubbard/ThurstonRatMaps.pdf\">"
 "Thurston obstruction</a>.)"
 "<p>"
 "The inverse spider algorithm yields the periodic angles corresponding to "
 "a center c. The initial curves at the periodic points are iterated "
 "backwards until their circular order becomes stable."
 "<p>"
 "The simulation illustrates convergence of the spider algorithm for "
 "&theta; = 11/31. (The points z<sub>i</sub> in the right window are "
 "computed by prescribing the left or right halfplanes for the square-roots, "
 "which works in this example. In general the spider legs cannot be omitted, "
 "but they are required to determine the correct branches.)<br>"
 "<i>Hit the Return key or push the Go-button repeatedly to iterate "
 "the points z<sub>i</sub> in the dynamic plane. Observe the convergence "
 "of the parameter c in the left window. (It will enter the window after "
 "three iterations.)</i><br>"
 "The spider algorithm is available in the main program with the key Ctrl+s. "
 "With s an alternative algorithm is used. Further applications of the "
 "Thurston algorithm are obtained with d and Ctrl+d. To find the "
 "parameter c corresponding to a periodic or preperiodic angle &theta; "
 "fast, you may draw the parameter ray with the key e, shift the current "
 "point to its endpoint, and find the parameter c with the key x. These "
 "steps are performed together with the single key Ctrl+x."));
   xp = new mdouble[6]; yp = new mdouble[6]; pplane->setCursorPix(spider);
   pplane->setPlane(-1.2563679, 0.3803210, 0.074, 0);
   pplane->draw(pf, 1, &mode); dplane->setPlane(0, 0, 1.9, 0); go();
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The parameter rays with periodic angles land in pairs at the "
 "roots of hyperbolic components. (Only one ray lands at the root "
 "c = 0.25 of period 1. It makes sense to count the angle "
 "&theta; = 0 = 1 twice.) There is a simple combinatorial "
 "<a href=\"https://www.mittag-leffler.se/preprints/\">algorithm</a> "
 "to determine the second angle, conjugate to a given angle. To find all "
 "pairs of conjugate angles, Lavaurs' algorithm is straightforward. It "
 "works because there is a hyperbolic component of lower period between "
 "two hyperbolic components of equal periods. For periods 1, 2, 3, ... all "
 "pairs of angles with this period are constructed as "
 "follows: always join the smallest free angle with the first one that is "
 "available, i.e, they can be joined without crossing rays of a lower "
 "period. This is a condition on the order of angles 0 &le; &theta; &lt; 1 "
 "that can be checked without drawing external rays. It is visualized as "
 "follows: represent external angles by points on the unit circle. Join "
 "pairs of conjugate angles by the circular arc orthogonal to the unit "
 "circle."
 "<p>"
 "This representation serves as a topological "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0801.58025\">model</a> of the Mandelbrot set M in fact: in the disk, "
 "identify each arc with a single point, and take the closure of this "
 "equivalence relation. In general, an arc is corresponding to the common "
 "landing point of two rays. All polygons of arcs are identified with single "
 "points, corresponding to branch points. The remaining gaps are "
 "corresponding to hyperbolic components. This model set is "
 "locally connected, and it will be homeomorphic to M if M is locally "
 "connected, which is not known. There is a continuous projection from M "
 "onto the model set. The preimages of single points are called fibers. "
 "These subsets of M are either points in the closure of a hyperbolic "
 "component, or subsets disjoint from hyperbolic components that cannot be "
 "separated by landing points of periodic rays. M will be locally connected, "
 "if and only if all fibers are single points."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to see the pairs "
 "of conjugate angles for periods 1 to 6, represented by parameter rays in "
 "the left window and by circular arcs in the right window. Note that, e.g., "
 "the 4-periodic angles 6/15 and 7/15 are adjacent but not conjugate, since "
 "they are separated by the 3-periodic pair 3/7 and 4/7. So 6/15 is joined "
 "with 9/15 and 7/15 is joined with 8/15.</i><br>"
 "Lavaurs' algorithm is not yet available in the main program. Conjugate "
 "angles are determined with the key Ctrl+x. Orbit portraits and "
 "dynamic laminations are obtained with o and Ctrl+o."));
   if (!backwards)
   { delete[] xp; delete[] yp; xp = 0; yp = 0; pplane->setCursorPix(0); }
   dplane->setPlane(0, 0, 1.2, 0); pplane->setPlane(-0.83, 0, 1.6, 0);
   dplane->draw(df, 0, &mode); dplane->drawEllipse(0, 0, 1.0, 1.0, Qt::green);
   pplane->setPoint(0, 0); pplane->draw(pf, 1, &mode);
   dplane->setPoint(3.0, 0);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "Each limb of the Mandelbrot set is characterized by the rotation number "
 "k/r. For parameters c in the k/r-limb, the filled Julia set K<sub>c</sub> "
 "has r components at the fixed point &alpha;<sub>c</sub> . These are "
 "permuted cyclically by the quadratic polynomial f<sub>c</sub>(z), going "
 "k steps counterclockwise (see page 6 of Chapter 2). &alpha;<sub>c</sub> "
 "has r external angles. Two of these belong to dynamic rays that are "
 "adjacent to the component containing the critical value z = c. The "
 "corresponding parameter rays land at the root of the k/r-limb, they "
 "bound the k/r-wake. To find these angles, draw the Hubbard tree "
 "for the center of period r. It consists of r arcs attached to &alpha;. "
 "The other marked points are endpoints. z<sub>1</sub> is k steps from "
 "z<sub>0</sub> ... Now &beta; is behind z<sub>0</sub> and its preimage "
 "-&beta; is behind the preimage z<sub>r-1</sub> of z<sub>0</sub> . The "
 "angles are obtained by following the orbit of the ray and noting its side, "
 "as explained on pages 1 and 2. The numerators of the angles at the root "
 "differ by 1, and it is "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0603.30030\">shown</a> that the sum of the intervals of "
 "angles for all wakes is 1. Thus the angles of rays landing at the main "
 "cardioid form a set of measure 0, and the Mandelbrot set has no further "
 "components besides the main cardioid and its limbs."
 "<p>"
 "The angles of the roots can be determined recursively as well. The "
 "<a href=\"http://www.math.cornell.edu/Research/Abstracts/"
 "dynamical_systems.html\">algorithm</a> by Dierk Schleicher employs "
 "Farey addition of rotation numbers. All fractions between 0 and 1 are "
 "obtained recursively by adding numerators and denominators separately:<br>"
 "0/1 (+) 1/1 = 1/2 , 0/1 (+) 1/2 = 1/3 , 1/3 (+) 1/2 = 2/5 , "
 "1/3 (+) 2/5 = 3/8 ...<br>"
 "Now the external angles of the 3/8-limb are obtained by combining the "
 "binary digits: the larger angle of the 1/3-limb is "
 "2/7 = .<nobr class=\"per\">010</nobr> and the smaller angle of the "
 "2/5-limb is 9/31 = .<nobr class=\"per\">01001</nobr> . Thus the external "
 "angles of the 3/8-limb are 73/255 = .<nobr class=\"per\">010 01001</nobr> "
 "and 74/255 = .<nobr class=\"per\">01001 010</nobr> . This algorithm is "
 "proved as follows: when the larger angle of m/p and the smaller angle of "
 "n/q, u/(2<sup>p</sup>-1) = .<nobr class=\"per\">u</nobr> and "
 "v/(2<sup>q</sup>-1) = .<nobr class=\"per\">v</nobr> are known,<ul>"
 "<li>.<nobr class=\"per\">u</nobr> &lt; .<nobr class=\"per\">v</nobr> "
 "implies .<nobr class=\"per\">u</nobr> &lt; .<nobr class=\"per\">uv</nobr> "
 "&lt; .<nobr class=\"per\">vu</nobr> &lt; .<nobr class=\"per\">v</nobr> . "
 "The new angles have periods dividing r = p + q, and the parameter rays "
 "need not land together.</li>"
 "<li>By a property of Farey addition, r is the smallest denominator of "
 "all fractions between m/p and n/q, and there are exactly 2 angles of "
 "period r and no smaller periods between .<nobr class=\"per\">u</nobr> and "
 ".<nobr class=\"per\">v</nobr> . Thus the angles "
 ".<nobr class=\"per\">uv</nobr> and .<nobr class=\"per\">vu</nobr> have "
 "period r, and the rays land together."
 "</ul>"
 "<i>Hit the Return key or push the Go-button repeatedly to compute the "
 "angles of the limbs for periods &le; 15, draw the parameter rays in the "
 "left window, and mark the rotation numbers in the right window.</i><br>"
 "In the main program, compute the angles of a wake with the key Ctrl+k. You "
 "can convert fractions and binary digits with the key Ctrl+x."));
   dplane->setPlane(0, 0, 1.2, 0); pplane->setPlane(-0.2553, 0, 1.1981, 0);
   dplane->draw(df, 0, &mode); dplane->drawEllipse(0, 0, 1.0, 1.0);
   pplane->setPoint(0, 0); pplane->draw(pf, 1, &mode); n = 1;
   dplane->setPoint(3.0, 0);
}
if (page == 9)
{  text->setHtml(QString(
 "On pages 11 and 12 of Chapter 3, the notion of edges and frames was "
 "introduced. These are useful to sketch the critical orbit immediately in "
 "certain cases, from which you can deduce, e.g., the Hubbard tree or the "
 "external angles. The left image shows the parameter edge of order 4 at the "
 "prinicipal Misiurewicz point a in the 1/3-limb. The corresponding dynamic "
 "edge in the right image is quite small. The edges form a graph, and the "
 "mapping of a dynamic edge under the iteration of f<sub>c</sub>(z) is "
 "understood from the rotation number 1/3 at the repelling fixed point "
 "&alpha;<sub>c</sub> together with the fact that f<sub>c</sub>(z) is an "
 "even function, i.e., f<sub>c</sub>(-z) = f<sub>c</sub>(z). Only the "
 "central edge is not mapped to another edge: the central frame is mapped "
 "2:1 to the branches behind the preperiodic point corresponding to a, and "
 "the two remaining parts are mapped to an edge at &alpha;<sub>c</sub> each."
 "<p>"
 "Here the parameter c is in an arm at a parameter frame of order 7 in the "
 "parameter edge of order 4. The critical value z<sub>1</sub> = c will be "
 "mapped as follows:<ul>"
 "<li>The edge containing z<sub>1</sub> = c is mapped to the central edge in "
 "three iterations. z<sub>4</sub> is in an arm at a non-central frame.</li>"
 "<li>This non-central frame is in the middle of the part between "
 "&alpha;<sub>c</sub> and the central frame. Therefore it is mapped to the "
 "central frame in three more iterations, and z<sub>7</sub> is in an arm "
 "at the central frame.</li>"
 "<li>This arm is mapped to the right branch behind the preperiodic point "
 "corresponding to a, and z<sub>8</sub> is in the edge attached to this "
 "point.</li>"
 "<li>This edge is mapped to the central edge in four more iterations. The "
 "parameter c has been chosen such that z<sub>12</sub> = 0, thus c is a "
 "center of period 12. This is the lowest period appearing in the "
 "initial arm.</li>"
 "</ul>"
 "<i>Hit the Return key or push the Go-button repeatedly to mark the "
 "critical orbit, try to predict the mapping from the description and the "
 "principles above.</i>"));
   dplane->setPlane(0, 0, 1.4, 0);
   pplane->setPlane(-0.14216, 1.02128, 0.0726, 0);
   pplane->setPoint(-0.115926243710, 0.985763493054);
   pplane->draw(pf, 1, &mode);
}
if (page == 10)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Consider a neighborhood of the filled Julia set K<sub>c</sub> bounded by "
 "an equipotential line. It is cut into three pieces by the dynamic rays "
 "landing at the repelling fixed point &alpha;<sub>c</sub> . These form the "
 "Yoccoz puzzle of depth 0. Preimages under the n-th iterate of "
 "f<sub>c</sub>(z) build the puzzle of depth n. Since the initial rays are "
 "mapped to one another, increasing n results in a subdivision of "
 "puzzle-pieces, and every point z of K<sub>c</sub> is contained in a unique "
 "nested sequence of pieces. In the right image, the different colors "
 "indicate neighboring pieces, which are mapped to different pieces of depth "
 "0, and the alternating light and dark annuli mark the bounding "
 "equipotential lines. The left images shows the corresponding "
 "para-puzzle-pieces, bounded by certain equipotential lines and by the ends "
 "of all parameter rays landing at &alpha;-type Misiurewicz points."
 "<p>"
 "Each puzzle-piece intersects K<sub>c</sub> in a connected set. If "
 "the diameter of the pieces containing any point z goes to 0, if it is the "
 "only point in their intersection, then K<sub>c</sub> will be "
 "<a href=\"javascript:alert('A set K is locally connected, if every "
 "point z in K has arbitrarily small connected (relative) neighborhoods, "
 "i.e., open sets whose intersection with K is connected. See page 9 of "
 "Chapter 1.')\">locally connected</a>. The nested sequence of puzzle-pieces "
 "may define a sequence of annuli around z. If their images do not surround "
 "the critical point 0 too often, their <a href=\"javascript:alert('"
 "The modulus of an annulus is a number measuring its thickness. It is "
 "invariant under a conformal map, but it will be doubled by a 2:1 "
 "map.')\">moduli</a> can be estimated below, showing that their union "
 "is a thick annulus surrounding a single point. Thus Jean-Christophe Yoccoz "
 "<a href=\"http://arxiv.org/abs/math/9207220\">showed</a> that "
 "K<sub>c</sub> is locally connected, when f<sub>c</sub>(z) does not have a "
 "neutral cycle, and is not renormalizable or only finitely often "
 "renormalizable (see Chapter 5). The estimate of moduli extends to the "
 "parameter plane, "
 "<a href=\"http://www.math.cornell.edu/~hubbard/Yoccoz.pdf\">showing</a> "
 "also that the Mandelbrot set M is locally connected at the corresponding "
 "parameters c."
 "<p>"
 "These results have been extended to infinitely renormalizable parameters "
 "with high renormalization periods by "
 "<a href=\"http://arxiv.org/abs/math/9503215\">Mikhail Lyubich</a> and "
 "<a href=\"http://arxiv.org/abs/0712.2444\">Jeremy Kahn</a>. When "
 "f<sub>c</sub>(z) has a neutral cycle, the filled Julia set K<sub>c</sub> "
 "may be non-locally connected, but M is locally connected at these "
 "parameters. This can be shown by the landing properties of parameter rays, "
 "or with the Pommerenke-Levin-Yoccoz "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0802.30022\">inequality</a>, which bounds the multiplier of a "
 "repelling periodic point, and thus the size of sublimbs of a hyperbolic "
 "component. Local connectivity of M would imply pathwise connectivity. "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0801.58025\">Jeremy Kahn</a> and "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:1121.37314\">Johannes Riedl</a> have shown that at least all "
 "centers and Misiurewicz points can be joined by arcs within M."
 "<p>"
 "<i>You may change the parameter c by clicking into the left window, "
 "or move it with the Arrow keys.</i><br>"
 "In the main program, choose algorithm 8 to color puzzle-pieces or "
 "para-puzzle pieces in the 1/2-, 1/3-, or 2/3-limbs."));
   dplane->setPlane(0, 0, 1.79, 0); mode = 8;
   pplane->setPlane(-0.1172, 0.9125, 0.26298, 0);
   pplane->setActive(true); goButton->setEnabled(false);
   pplane->setPoint(-0.156520, 1.032247); pplane->draw(pf, 1, &mode);
}
} //showPage CC

void QmnDemoCC::go()
{  if (page == 1 && n <= 5)
   {  n++; mdouble x, y, u = 11.0/31.0, v = 12.0/31.0;
      if (n == 2) { u = 22.0/31.0; v = 24.0/31.0; }
      if (n == 3) { u = 13.0/31.0; v = 17.0/31.0; }
      if (n == 4) { u = 26.0/31.0; v = 3.0/31.0; }
      if (n == 5) { u = 21.0/31.0; v = 6.0/31.0; }
      if (n == 6)
      {  pplane->traceRay(1, u, pf, x, y, 1, Qt::red);
         pplane->traceRay(1, v, pf, x, y, 1, Qt::magenta);
      }
      else
      {  dplane->traceRay(-1, u, df, x, y, 1, Qt::red);
         dplane->traceRay(-1, v, df, x, y, 1, Qt::magenta);
      }
   }
   if (page <= 2 || page == 4 || page == 9)
   {  if (page == 9) dplane->move(5);
      mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      df->iterate(x, y); dplane->setPoint(x, y);
   }
   if (page >= 2 && page <= 4)
   {  mdouble x, y; pplane->getPoint(x, y);
      if (y > 0) { x = 1.0; y = -1.0; }
      else if (y < 0) { x = -2.0; y = 0; }
      else if (x < 0) { x = 2.0; y = 0.0; }
      else if (x > 0) { x = 0; y = 0; }
      else { x = -1.0; y = 1.0; }
      pplane->setPoint(x, y);
   }
   if (page == 5)
   {  mdouble x, y; pplane->getPoint(x, y);
      if (x < -1.0) { x = -x; y = -y; }
      else if (x < 0) { x = -2.0; y = -1.0; }
      else if (y > 0) { x = 0; y = 0; }
      else if (y < 0) x = -0.7;
      else { x = -2.0; y = 1.0; }
      pplane->setPoint(x, y);
   }
   /*if (page == 6)
   {  int j;
      if (!n)
      {  pplane->setPlane(-0.83, 0, 1.6, 0); pplane->draw(pf, 1, &mode);
         xp[1] = 2*PI*1.0/31.0;
         for (j = 1; j <= 4; j++)
         { xp[j+1] = 2*xp[j]; yp[j] = sin(xp[j]); xp[j] = cos(xp[j]); }
         //yp[5] = sin(xp[5]); xp[5] = cos(xp[5]); n = 1;
         xp[5] = 0; yp[5] = 0; n = 1;
      }
      else for (j = 4; j >= 1; j--)
      {  mndynamics::root(xp[j+1] - xp[1], yp[j+1] - yp[1], xp[j], yp[j]);
         if (yp[j] < 0) { xp[j] = -xp[j]; yp[j]= -yp[j]; }
         //xp[5] = 0; yp[5] = 0;
      }
      dplane->draw(df, 0, &mode); pplane->setPoint(xp[1], yp[1]);
      for (j = 5; j >= 1; j--)
      { dplane->setPoint(xp[j], yp[j]); dplane->move(5); }
   }//test 1/5 */
   if (page == 6)
   {  int j;
      if (!n)
      {  xp[1] = 2*PI*11.0/31.0;
         for (j = 1; j <= 4; j++)
         { xp[j+1] = 2*xp[j]; yp[j] = sin(xp[j]); xp[j] = cos(xp[j]); }
         //yp[5] = sin(xp[5]); xp[5] = cos(xp[5]); n = 1;
         xp[5] = 0; yp[5] = 0; n = 1;
      }
      else for (j = 4; j >= 1; j--)
      {  mndynamics::root(xp[j+1] - xp[1], yp[j+1] - yp[1], xp[j], yp[j]);
         if (j & 1) { xp[j] = -xp[j]; yp[j]= -yp[j]; }
         //xp[5] = 0; yp[5] = 0;
      }
      dplane->draw(df, 0, &mode); pplane->setPoint(xp[1], yp[1]);
      for (j = 5; j >= 1; j--)
      { dplane->setPoint(xp[j], yp[j]); dplane->move(5); }
   }//*/
   if (page == 7)
   {  mdouble x, y, M = 1.0, N = 2.0, D = 3.0;
      QColor color = Qt::blue;
      while (1)
      {  if (n == 1) { M = 1.0; N = 2.0; D = 7.0; color = Qt::darkCyan; }
         if (n == 2) { M = 3.0; N = 4.0; }
         if (n == 3) { M = 5.0; N = 6.0; }
         if (n == 4)
         { M = 1.0; N = 2.0; D = 15.0; color = Qt::red; }
         if (n == 5) { M = 3.0; N = 4.0; }
         if (n == 6) { M = 6.0; N = 9.0; }
         if (n == 7) { M = 7.0; N = 8.0; }
         if (n == 8) { M = 11.0; N = 12.0; }
         if (n == 9) { M = 13.0; N = 14.0; }
         if (n == 10) { M = 1.0; N = 2.0; D = 31.0; color = Qt::magenta; }
         if (n == 11) { M = 3.0; N = 4.0; }
         if (n == 12) { M = 5.0; N = 6.0; }
         if (n == 13) { M = 7.0; N = 8.0; }
         if (n == 14) { M = 9.0; N = 10.0; }
         if (n == 15) { M = 11.0; N = 12.0; }
         if (n == 16) { M = 13.0; N = 18.0; }
         if (n == 17) { M = 14.0; N = 17.0; }
         if (n == 18) { M = 15.0; N = 16.0; }
         if (n == 19) { M = 19.0; N = 20.0; }
         if (n == 20) { M = 21.0; N = 22.0; }
         if (n == 21) { M = 23.0; N = 24.0; }
         if (n == 22) { M = 25.0; N = 26.0; }
         if (n == 23) { M = 27.0; N = 28.0; }
         if (n == 24) { M = 29.0; N = 30.0; }
         if (n == 25) { M = 1.0; N = 2.0; D = 63.0; color = Qt::yellow; }
         if (n == 26) { M = 3.0; N = 4.0; }
         if (n == 27) { M = 5.0; N = 6.0; }
         if (n == 28) { M = 7.0; N = 8.0; }
         if (n == 29) { M = 10.0; N = 17.0; }
         if (n == 30) { M = 11.0; N = 12.0; }
         if (n == 31) { M = 13.0; N = 14.0; }
         if (n == 32) { M = 15.0; N = 16.0; }
         if (n == 33) { M = 19.0; N = 20.0; }
         if (n == 34) { M = 22.0; N = 25.0; }
         if (n == 35) { M = 23.0; N = 24.0; }
         if (n == 36) { M = 26.0; N = 37.0; }
         if (n == 37) { M = 28.0; N = 35.0; }
         if (n == 38) { M = 29.0; N = 34.0; }
         if (n == 39) { M = 30.0; N = 33.0; }
         if (n == 40) { M = 31.0; N = 32.0; }
         if (n == 41) { M = 38.0; N = 41.0; }
         if (n == 42) { M = 39.0; N = 40.0; }
         if (n == 43) { M = 43.0; N = 44.0; }
         if (n == 44) { M = 46.0; N = 53.0; }
         if (n == 45) { M = 47.0; N = 48.0; }
         if (n == 46) { M = 49.0; N = 50.0; }
         if (n == 47) { M = 51.0; N = 52.0; }
         if (n == 48) { M = 55.0; N = 56.0; }
         if (n == 49) { M = 57.0; N = 58.0; }
         if (n == 50) { M = 59.0; N = 60.0; }
         if (n == 51) { M = 61.0; N = 62.0; }
         dplane->drawOrtho(M/D, N/D, color);
         pplane->traceRay(1, M/D, pf, x, y, 1, color);
         pplane->traceRay(1, N/D, pf, x, y, 1, color);
         n++; if (n == 52) goButton->setEnabled(false);
         if (n == 1 || n == 4 || n == 10 || n == 25 || n == 52) return;
      }
   }
   if (page == 8)
   {  n++; mdouble x, y; int k, pos, power, num;
      for (k = 1; k < n; k++)
      {  if (!(n & 1) && !(k & 1)) continue;
         if (!(n % 3) && !(k % 3)) continue;
         if (!(n % 5) && !(k % 5)) continue;
         if (!(n % 7) && !(k % 7)) continue;
         pos = n; power = 1; num = 0;
         while (1)
         {  if (pos > n - k) num |= power;
            pos -= k; if (pos <= 0) pos += n;
            power <<= 1;
            if (pos == n) break;
         }
         x = 2*PI*mdouble(k)/mdouble(n); QColor cl(255 - 17*n, 0, 17*n);
         dplane->drawLine(0, 0, cos(x), sin(x), cl); power--;
         pplane->traceRay(1, mdouble(num)/mdouble(power), pf, x, y, 2, cl);
         pplane->traceRay(1, mdouble(num + 1)/mdouble(power), pf, x, y, 2, cl);
      }
      if (n == 15) goButton->setEnabled(false);
   }
} //go CC

void QmnDemoCC::drawRight()
{  if (page <= 8) return;
   mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   dplane->setPoint(x, y); dplane->draw(df, -1, &mode);
}

void QmnDemoCC::pFinished()
{  if (page >= 2 && page <= 4)
   {  pplane->setPlane(0, 0, 2.4, 0);
      pplane->drawLine(-2.0, 0, 2.0, 0, Qt::darkBlue);
      pplane->drawLine(-1.0, 0, -1.0, 1.0, Qt::darkBlue);
      pplane->drawLine(1.0, 0, 1.0, -1.0, Qt::darkBlue);
      pplane->drawEllipse(0, 0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(-2.0, 0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(2.0, 0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(-1.0, 1.0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(1.0, -1.0, 0.04, 0.04, Qt::black, true);
   }
   if (page == 5)
   {  pplane->setPlane(0, 0, 2.4, 0);
      pplane->drawLine(-1.4, 0, 1.4, 0, Qt::darkBlue);
      pplane->drawLine(-0.7, 0, -0.7, -1.0, Qt::darkBlue);
      pplane->drawLine(-2.0, 1.0, -1.4, 0, Qt::darkBlue);
      pplane->drawLine(-2.0, -1.0, -1.4, 0, Qt::darkBlue);
      pplane->drawLine(2.0, 1.0, 1.4, 0, Qt::darkBlue);
      pplane->drawLine(2.0, -1.0, 1.4, 0, Qt::darkBlue);
      pplane->drawEllipse(0, 0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(-2.0, 1.0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(-2.0, -1.0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(-0.7, -1.0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(2.0, 1.0, 0.04, 0.04, Qt::black, true);
      pplane->drawEllipse(2.0, -1.0, 0.04, 0.04, Qt::black, true);
   }
   if (page == 7) { pplane->drawLine(0.25, 0, 1.0, 0, Qt::white); }
   //if (page == 8) go(); //7 and 8 did not work with  go()  here ...?
}

void QmnDemoCC::dFinished()
{  if (page == 1)
   {  mdouble x, y; dplane->traceRay(-1, 0.5, df, x, y, 2, Qt::yellow);
      dplane->traceRay(-1, 0, df, x, y, 2, Qt::yellow); go();
   }
   if (page == 2)
   {  int j; mdouble x = 0, y = 0;
      for (j = 1; j <= 5; j++)
      { df->iterate(x, y); dplane->setPoint(x, y); dplane->move(5); }
   }
   if (page == 4)
   {  mdouble x, y; df->getParameter(x, y);
      dplane->backRay(21LL, 31LL, x, y, 3, Qt::blue);
      dplane->backRay(11LL, 62LL, x, y, 3, Qt::blue);
      dplane->backRay(6LL, 31LL, x, y, 3, Qt::darkGreen);
      dplane->backRay(43LL, 62LL, x, y, 3, Qt::darkGreen);
   }
}

////////////////////////////////////////////////////////////////////////

void QmnDemoRN::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1; n = 0;
if (page == 1)
{  text->setHtml(QString(
 "The left image shows a small Mandelbrot set M<sub>4</sub> contained in "
 "the 1/3-limb of the Mandelbrot set M. Its main cardioid is a primitive "
 "hyperbolic component of period 4. A satellite component of period 12 is "
 "attached to it, which is obtained by a period tripling bifurcation, and "
 "the current parameter c is the center of that component. The right image "
 "shows the filled Julia set K<sub>c</sub> of the corresponding quadratic "
 "polynomial f<sub>c</sub>(z) = z<sup>2</sup> + c, i.e., the set of points "
 "z not escaping to &infin; under the iteration. Its branches are filled "
 "with small Julia sets, which look like the \"rabbit,\" the filled Julia "
 "set of satellite type and period 3. The attracting critical orbit is "
 "contained in four small Julia sets. The fourth iterate "
 "f<sub>c</sub><sup>4</sup>(z) of f<sub>c</sub>(z) maps each of "
 "these small Julia sets to itself. Here \"small Julia set\" is a "
 "shorthand of \"small copy of a filled Julia set.\"<br>"
 "The appearance of small Mandelbrot sets and small Julia sets will be "
 "explained on pages 2-5."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to map the point "
 "z with the quadratic polynomial f<sub>c</sub>(z). You may move the "
 "initial value z with the Arrow keys or by clicking into the right window "
 "as well.</i>"));
   if (!backwards) { pplane->setNmax(500); dplane->setNmax(500); }
   pplane->setPlane(-0.1118, 1.0, 0.097 ,0); //(-0.1575, 1.0341, 0.2021, 0);
   dplane->setPlane(0, 0, 1.45, 0);
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   pplane->draw(pf, 1, &mode); dplane->setActive(true);
}
if (page == 2)
{  text->setHtml(QString(
 "The left image shows a magnification of the small Mandelbrot set "
 "M<sub>4</sub> , and the right image shows a small Julia set "
 "K<sub>c</sub><sup>4</sup> contained in the filled Julia set "
 "K<sub>c</sub> . The cross is initially at a critical point of the "
 "fourth iterate f<sub>c</sub><sup>4</sup>(z), which will be mapped to the "
 "critical value z = c. The following cases occur:<ul>"
 "<li>Points z in the small Julia set (black) will be mapped to the same "
 "small Julia set by f<sub>c</sub><sup>4</sup>(z). This map "
 "acts like the quadratic polynomial of the \"rabbit.\"</li>"
 "<li>Points z in the exterior (green) leave the window after a few "
 "iterations of f<sub>c</sub><sup>4</sup>(z) and go to &infin; under "
 "iteration of f<sub>c</sub>(z).</li>"
 "<li>The decorations (mostly blue) belong to the filled Julia set "
 "K<sub>c</sub> but not to the small Julia set K<sub>c</sub><sup>4</sup> . "
 "An initial value z in a decoration will leave the window after a few "
 "iterations of f<sub>c</sub><sup>4</sup>(z), but it will stay within the "
 "filled Julia set K<sub>c</sub> and not go to &infin;. It may or may not "
 "return to the window later.</li></ul>"
 "The map g<sub>c</sub>(z) = f<sub>c</sub><sup>4</sup>(z) in a "
 "neighborhood of the small Julia set is called a renormalization of "
 "f<sub>c</sub>(z). From the point of view of g<sub>c</sub>(z), the filled "
 "Julia set is K<sub>c</sub><sup>4</sup>, and points z in the decorations "
 "escape from its domain. But from the point of view of the original "
 "quadratic polynomial f<sub>c</sub>(z), the filled Julia set is "
 "K<sub>c</sub> , which contains both the decorations and the small "
 "Julia set K<sub>c</sub><sup>4</sup> ."
  "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to map z with the "
 "fourth iterate f<sub>c</sub><sup>4</sup>(z) of f<sub>c</sub>(z). Change "
 "the initial value z with the Arrow keys or by clicking into the right "
 "window. Take your time to explore the different cases described "
 "above.</i><br>"
 "In the main program, you can hit the key f four times for this."));
   if (!backwards)
   {  pplane->setPlane(-0.1592, 1.0345, 0.037, 0);
      dplane->setPlane(-0.156520, 1.0322471, 0.027644, 0);
      pplane->draw(pf, 1, &mode);
   }
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   dplane->setActive(true);
}
if (page == 3)
{  text->setHtml(QString(
 "The map g<sub>c</sub>(z) = f<sub>c</sub><sup>4</sup>(z) can be "
 "discussed precisely after specifying its domain. It is a 2:1 "
 "map from the region U<sub>c</sub>' to the disk U<sub>c</sub> , "
 "where U<sub>c</sub>' is bounded by the cyan curve, and U<sub>c</sub> "
 "is bounded by the red circle. g<sub>c</sub>(z) is called a "
 "quadratic-like map, a polynomial-like map of degree 2. "
 "Although g<sub>c</sub>(z) is a polynomial of degree 16 (in this case), "
 "on U<sub>c</sub>' it behaves like a quadratic polynomial on a large "
 "disk. Its filled Julia set is defined as the set "
 "K<sub>c</sub><sup>4</sup> of points z, which do not leave "
 "U<sub>c</sub>' under the iteration. It is marked magenta. The cross is "
 "initially at the critical point z = &omega;<sub>c</sub> of "
 "g<sub>c</sub>(z), and the critical value is z = c."
 "<p>"
 "Note that only finitely many decorations reach beyond U<sub>c</sub>' . "
 "Bounding U<sub>c</sub> with parts of equipotential lines and external "
 "rays instead of a circle, we can arrange that only one or two "
 "decorations intersect the boundary of U<sub>c</sub> and the point "
 "of intersection is preperiodic under f<sub>c</sub>(z). Then all "
 "decorations can be described by preimages of these points, since every "
 "escaping point z passes once through the fundamental annulus "
 "U<sub>c</sub> \\ U<sub>c</sub>'. The decoration attached to the small "
 "&beta;-fixed point is constructed, and all other decorations are attached "
 "to preimages of this point. These points are preperiodic with period 4 "
 "under f<sub>c</sub>(z) and period 1 under g<sub>c</sub>(z)."
 "<p>"
 "<i>Choose an initial value for z by clicking into the right window or by "
 "moving the cross with the Arrow keys. Hit the Return key or push the "
 "Go-button repeatedly to map z with the quadratic-like map "
 "g<sub>c</sub>(z) = f<sub>c</sub><sup>4</sup>(z). Take your time to do "
 "this for z in K<sub>c</sub><sup>4</sup> , on the decorations, or on the "
 "cyan boundary of U<sub>c</sub>'.</i><br>"
 "Drawing the boundaries is not available in the main program. To mark "
 "the small Julia set,  hit the key r and enter the renormalization period "
 "(here 4). The program checks that after 3 initial iterations of z, "
 "every 4th iterate must stay within some disk around z = 0. (You may "
 "choose a frame around the small Julia set to reduce computation time.)"));
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   dplane->setActive(true);
}
if (page == 4)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The <a href=\"http://www.math.cornell.edu/~hubbard/PolyLikeMaps.pdf\">"
 "Straightening Theorem</a> of Douady and Hubbard gives a conjugation "
 "&psi;<sub>c</sub>(z) from the quadratic-like map "
 "g<sub>c</sub>(z) = f<sub>c</sub><sup>4</sup>(z) to a quadratic polynomial "
 "f<sub>d</sub>(z) = z<sup>2</sup> + d. The relation g<sub>c</sub>(z) = "
 "&psi;<sub>c</sub><sup>-1</sup>(f<sub>d</sub>(&psi;<sub>c</sub>(z))) shows "
 "that the dynamics of these two maps is qualitatively the same. "
 "&psi;<sub>c</sub>(z) maps the critical point &omega;<sub>c</sub> of "
 "g<sub>c</sub>(z) to the critical point 0 of f<sub>d</sub>(z), the "
 "critical value c to d, and the small Julia set K<sub>c</sub><sup>4</sup> "
 "to the filled Julia set K<sub>d</sub> of f<sub>d</sub>(z), which is the "
 "\"rabbit\" in this case."
 "<p>"
 "The conjugation &psi;<sub>c</sub>(z) is a "
 "<a href=\"javascript:alert('The linear approximation of a conformal "
    "map is multiplication with a complex number, thus a rotation and "
    "rescaling. The linear approximation of a quasiconformal map is "
    "multiplication with a real matrix, which may include shearing.')\">"
 "quasiconformal</a> map, which is conformal almost everywhere on the "
 "small Julia set K<sub>c</sub><sup>4</sup>. See Chapter 8 for a more "
 "extensive discussion of these notions and of the construction. "
 "A main step in the construction of &psi;<sub>c</sub>(z) is to choose a "
 "quasiconformal map &xi;<sub>c</sub>(z) from the fundamental annulus "
 "U<sub>c</sub> \\ U<sub>c</sub>' to a round annulus. "
 "When the critical orbit of g<sub>c</sub>(z) does not escape from "
 "U<sub>c</sub>', or equivalently, when the small Julia set "
 "K<sub>c</sub><sup>4</sup> is connected, then the new parameter d belongs "
 "to the Mandelbrot set M. It will depend on g<sub>c</sub>(z) only, not "
 "on the choice of &xi;<sub>c</sub>(z). Otherwise the value of d does "
 "depend on the choice of &xi;<sub>c</sub>(z)."
 "<p>"
 "<i>To change the parameter c, click into the left window, or move the "
 "cross with the Arrow keys. Watch the small Julia set for parameters in "
 "M<sub>4</sub> , on its decorations, and in the exterior. When "
 "U<sub>c</sub>' becomes disconnected, g<sub>c</sub>(z) will no longer be "
 "quadratic-like.</i>"));
   if (backwards) pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   pplane->setActive(true);
}
if (page == 5)
{  text->setHtml(QString(
 "The quasiconformal map &psi;<sub>c</sub>(z) constructed with the "
 "Straightening Theorem conjugates the quadratic-like restriction "
 "g<sub>c</sub>(z) : U<sub>c</sub>' &rarr; U<sub>c</sub> of "
 "f<sub>c</sub><sup>4</sup>(z) to a quadratic polynomial f<sub>d</sub>(z) "
 "= z<sup>2</sup> + d. It maps the critical value z = c of "
 "g<sub>c</sub>(z) to the critical value z = d of f<sub>d</sub>(z). "
 "A map in the parameter plane is given by h(c) = d: for parameters c "
 "in a neighborhood U<sub>M</sub> of the small Mandelbrot set "
 "M<sub>4</sub> , such that z = c is in U<sub>c</sub> , the corresponding "
 "map g<sub>c</sub>(z) is straightened and the parameter d of the new "
 "map f<sub>d</sub>(z) is taken as the value of h(c). This map is "
 "shown to be a homeomorphism from U<sub>M</sub> to a neighborhood of the "
 "Mandelbrot set M, i.e., h(c) and h<sup>-1</sup>(d) are continuous."
 "<p>"
 "The small Mandelbrot set M<sub>4</sub> , which is marked magenta in the "
 "left image, is defined as the set of parameters c, such that the small "
 "Julia set K<sub>c</sub><sup>4</sup> is connected, or equivalently, such "
 "that the critical orbit of g<sub>c</sub>(z) does not escape from "
 "U<sub>c</sub>'. Then the filled Julia set K<sub>d</sub> of "
 "f<sub>d</sub>(z) will be connected as well, and the "
 "new parameter d belongs to the Mandelbrot set M. Thus h(c) maps the "
 "small Mandelbrot set M<sub>4</sub> to the Mandelbrot set M. The "
 "decorations of the small Mandelbrot set, which are attached to "
 "Misiurewicz points of period 4, are mapped to the exterior of M. Their "
 "images under h(c) are attached to &beta;-type Misiurewicz points. The "
 "map in the exterior depends on the choice of &xi;<sub>c</sub>(z) "
 "in the fundamental annulus. If &xi;<sub>c</sub><sup>-1</sup>(z) depends "
 "holomorphically on the parameter c, then the map h(c) in the parameter "
 "plane will be quasiconformal. This was shown by Mikhail Lyubich."
 "<p>"
 "<i>To change the parameter c, click into the left window, or move the "
 "cross with the Arrow keys. Watch the small Julia set for parameters in "
 "M<sub>4</sub> , on its decorations, and in the exterior.</i><br>"
 "To mark a small Mandelbrot set in the main program, hit the key r and "
 "enter the renormalization period (here 4). The program checks, if every "
 "4th iterate of z = 0 stays in some disk around z = 0. (You may choose "
 "a frame around the small Mandelbrot set to reduce computation time.)"));
   if (backwards)
   {  pplane->draw(pf, 1, &mode);
      dplane->setPlane(-0.156520, 1.0322471, 0.027644, 0);
   }
   else pFinished(); //ren
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   pplane->setActive(true);
}
if (page == 6)
{  text->setHtml(QString(
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "External angles of the small Julia set or small Mandelbrot set can be "
 "computed by a composition of binary expansions. The left image "
 "shows the parameter rays landing at the root of M<sub>4</sub> , with the "
 "angles 3/15 = .<nobr class=\"per\">0011</nobr> (red) and "
 "4/15 = .<nobr class=\"per\">0100</nobr> (magenta). The right image shows "
 "the corresponding dynamic rays landing at the small "
 "&beta;<sub>c</sub><sup>4</sup> and their 4-cycles of dynamic rays. The "
 "yellow rays have the angles 0 and 1/2. To determine the angle of the white "
 "ray of period 12, which lands at the small "
 "&alpha;<sub>c</sub><sup>4</sup> of the small K<sub>c</sub><sup>4</sup>, "
 "recall the algorithm from page 1 of Chapter 4: follow the orbit of this "
 "ray, and note the binary digit 0 or 1, when the ray is in the upper or "
 "lower part between the yellow rays. You will find that the first four "
 "rays are close to the red ones, again close to the red ones for four "
 "more iterations, and then close to the magenta ones for four iterations. "
 "Thus the binary digits are "
 "<nobr>820/4095 = .<nobr class=\"per\">0011 0011 0100</nobr> .</nobr>"
 "<p>"
 "Renormalizing this ray of period 12 at K<sub>c</sub><sup>4</sup> gives "
 "the 3-periodic ray at K<sub>d</sub> with angle "
 "1/7 = .<nobr class=\"per\">001</nobr> . Its binary digits show that the "
 "ray and its first image are on the upper side of the \"rabbit,\" while the "
 "second image is on the lower side. Now the two sides of the "
 "\"small rabbit\" are represented by the red ray with angle "
 ".<nobr class=\"per\">0011</nobr> and the magenta ray with angle "
 ".<nobr class=\"per\">0100</nobr> . This explains the behavior of the white "
 "ray, since four iterations correspond to one iteration of the renormalized "
 "ray. We have the following algorithm: take the binary expansion of the "
 "renormalized angle and replace every 0 with 0011 and every 1 with 0100 to "
 "compute the angle at the small Julia set. E.g., the other external angle "
 "at the component of K<sub>c</sub><sup>4</sup> containing z = c, or at the "
 "satellite component of period 12 in M<sub>4</sub> , is obtained by "
 "replacing the digits of 2/7 = .<nobr class=\"per\">010</nobr> , resulting "
 "in 835/4095 = .<nobr class=\"per\">0011 0100 0011</nobr> ."
 "<p>"
 "The same algorithm applies to preperiodic angles. When the renormalized "
 "angle is pre-1-periodic, there will be two different representations, "
 "e.g., "
 "1/4 = .00<nobr class=\"per\">1</nobr> = .01<nobr class=\"per\">0</nobr> ."
 "The algorithm gives two different angles at the small Julia set or small "
 "Mandelbrot set, "
 "769/3840 = .0011 0011 <nobr class=\"per\">0100</nobr> and "
 "<nobr>783/3840 = .0011 0100 <nobr class=\"per\">0011</nobr> .</nobr> "
 "A decoration is attached to the preperiodic point or Misiurewicz point "
 "between these two rays. If the small Mandelbrot set was not primitive, "
 "but of satellite type from an r-tupling bifurcation, the decoration would "
 "have r - 1 branches, and there would be r - 2 additional rays between "
 "the ones computed from the algorithm."
 "<p>"
 "<i>Hit the Return key or push the Go-button twelve times to map the "
 "critical value z = c with f<sub>c</sub>(z) and to draw the corresponding "
 "dynamic rays, note the side.</i><br>"
 "In the main program, you can specify angles in binary notation when "
 "drawing external rays with e, and convert fractions and binary notation "
 "with the key Ctrl+x."));
   if (backwards) pplane->setPlane(-0.1592, 1.0345, 0.037, 0);
   dplane->setPlane(0, 0, 1.55, 0); pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.153940779149429, 1.03770019646018); n = 820;
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "The left image shows the 1/3-sublimb of the period-4 component. The part "
 "belonging to the small Mandelbrot set M<sub>4</sub> is marked magenta and "
 "the decorations are blue. The principal branch point of the 1/3-limb of "
 "the full Mandelbrot set M is a Misiurewicz point with the external angles "
 "9/56 = .001<nobr class=\"per\">010</nobr>, "
 "11/56 = .001<nobr class=\"per\">100</nobr>, and "
 "15/56 = .010<nobr class=\"per\">001</nobr> . By the algorithm from the "
 "previous page, the corresponding Misiurewicz point a in M<sub>4</sub> "
 "has the external angles 3358735/16773120 = "
 ".0011 0011 0100 <nobr class=\"per\">0011 0100 0011</nobr> , "
 "3358975/16773120 = "
 ".0011 0011 0100 <nobr class=\"per\">0100 0011 0011</nobr> , "
 "and 3420145/16773120 = "
 ".0011 0100 0011 <nobr class=\"per\">0011 0011 0100</nobr> . "
 "<p>"
 "The small Julia set K<sub>c</sub><sup>4</sup> is marked magenta in the "
 "right image. For c = a it is a quasiconformal copy of the filled Julia "
 "set for the branch point in the 1/3-limb of M. On the following page you "
 "will see that the Mandelbrot set contains a similar embedded Julia set. "
 "The explanation relies on renormalization together with the fact that the "
 "small Julia set K<sub>c</sub><sup>4</sup> depends continuously on the "
 "parameter c at the Misiurewicz point c = a: for c sufficiently close to "
 "a, the small Julia sets K<sub>c</sub><sup>4</sup> and "
 "K<sub>a</sub><sup>4</sup> are close to one another. They will not be "
 "homeomorphic, however, because K<sub>c</sub><sup>4</sup> may have "
 "non-empty interior or, e.g., be totally disconnected."
 "<p>"
 "<i>Change the parameter c with the Arrow keys or by clicking into the "
 "left window. Observe the changes of the small Julia set "
 "K<sub>c</sub><sup>4</sup> and its continuity for c &rarr; a.</i>"));
   if (backwards) pplane->setNmax(500);
   pplane->setPlane(-0.152888578707896, 1.03893533032734, 0.0022518, 0);
   dplane->setPlane(-0.156520, 1.0322471, 0.027644, 0);
   pplane->draw(pf, 1, &mode); pplane->setActive(true);
   pplane->setPoint(-0.152888578707896, 1.03893533032734);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Now the parameter c = c<sub>0</sub> is the center of a small Mandelbrot "
 "set M<sub>75</sub> of period 75, which is close to the Misiurewicz point "
 "a in M<sub>4</sub> but which is disjoint from M<sub>4</sub> . The right "
 "image shows a subset of the filled Julia set K<sub>c</sub> with the small "
 "Julia set K<sub>c</sub><sup>4</sup> for c = c<sub>0</sub> . Embedded Julia "
 "sets will become visible by zooming in around c<sub>0</sub> in "
 "both planes. The "
 "<a href=\"http://www.mndynamics.com/indexm.html#embed\">explanation</a> "
 "is given by relating several sets to one another:<ul>"
 "<li>The restriction f<sub>c</sub><sup>4</sup>(z) : "
 "U<sub>c</sub>' &rarr; U<sub>c</sub> is quadratic-like for parameters c in "
 "a neighborhood U<sub>M</sub> of M<sub>4</sub> , such that the critical "
 "value z = c belongs to U<sub>c</sub> . The small Julia set "
 "K<sub>c</sub><sup>4</sup> is connected, if c belongs to "
 "M<sub>4</sub> .</li>"
 "<li>The restriction f<sub>c</sub><sup>75</sup>(z) : "
 "V<sub>c</sub> &rarr; U<sub>c</sub> is quadratic-like for parameters c in a "
 "neighborhood V<sub>M</sub> of M<sub>75</sub> , such that the critical "
 "value z = c belongs to V<sub>c</sub> . The small Julia set "
 "K<sub>c</sub><sup>75</sup> is connected, if c belongs to M<sub>75</sub> . "
 "Here V<sub>c</sub> is much smaller than U<sub>c</sub>'.</li>"
 "<li>For c in V<sub>M</sub> , the composition "
 "f<sub>c</sub><sup>-3</sup>(f<sub>c</sub><sup>74</sup>(z)) maps "
 "V<sub>c</sub> conformally to U<sub>c</sub>'. The preimage of the "
 "small Julia set K<sub>c</sub><sup>4</sup> , which is totally "
 "disconnected, defines the embedded Julia set K<sub>c</sub><sup>em</sup> "
 "in the dynamic plane. It is close to a conformal copy of "
 "K<sub>a</sub><sup>4</sup> by the continuity of "
 "K<sub>c</sub><sup>4</sup>.</li>"
 "<li>The embedded Julia set K<sub>M</sub><sup>em</sup> in the parameter "
 "plane is defined as the set of parameters c, such that the critical value "
 "z = c belongs to the embedded Julia set K<sub>c</sub><sup>em</sup> in "
 "the dynamic plane for this parameter. Since the latter sets are moving "
 "holomorphically for parameters c in V<sub>M</sub> , a standard argument "
 "shows that K<sub>M</sub><sup>em</sup> is a quasiconformal copy of any "
 "K<sub>c</sub><sup>em</sup> with c in V<sub>M</sub> , e.g., "
 "c = c<sub>0</sub> .</li></ul>"
 "So K<sub>M</sub><sup>em</sup> is close to a quasiconformal copy of "
 "K<sub>a</sub> . Although the former set is totally disconnected, these "
 "sets will look similar, especially when V<sub>M</sub> is close to a "
 "but its diameter is relatively small compared to its distance  to "
 "M<sub>4</sub>. The embedded Julia set K<sub>M</sub><sup>em</sup> is "
 "visible well, because certain Misiurewicz points of period 4 are dense "
 "in it, and there are decorations attached to all of them."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in to the "
 "embedded Julia set around the small Mandelbrot set of period 75. When "
 "</i><nobr>K<sub>M</sub><sup>em</sup></nobr> <i>appears, go slowly to see "
 "it marked magenta. Go further to see </i>M<sub>75</sub> .<br>"
 "In the main program, mark these embedded Julia sets by hitting the key r "
 "and entering 75,4 for preperiod,period: the small Julia set "
 "K<sub>c</sub><sup>4</sup> has preperiod 0 and by this convention, the "
 "embedded Julia sets have the preperiod 75. (In fact, the program "
 "does 74 iterations to a neighborhood of z = 0, before checking that "
 "every 4th iterate stays there.)"));
   if (backwards)
   {  pplane->setPlane(-0.152888578707896, 1.03893533032734, 0.0022518, 0);
      dplane->setPlane(-0.156520, 1.0322471, 0.027644, 0);
      dplane->setNmax(500);
   }
   pplane->setNmax(2000); pplane->draw(pf, 1, &mode);
   pplane->setPoint(-0.152928554293307, 1.03895330374394);
}
if (page == 9)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "On the previous page, you have seen an example of an embedded Julia set "
 "close to a Misiurewicz point a of M<sub>4</sub> . Here you can see an "
 "embedded Julia set around a small Mandelbrot set of period 139, which "
 "is close to a Siegel parameter a in M<sub>4</sub> . The embedded Julia set "
 "K<sub>M</sub><sup>em</sup> is similar to a Siegel filled Julia set (with "
 "narrow cuts) now. Again, the small Julia set K<sub>c</sub><sup>4</sup> is "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0934.30023\">continuous</a> at c = a (since it is actually a small "
 "filled Julia set, but the small \"strict\" Julia set "
 "&part;K<sub>c</sub><sup>4</sup> is discontinuous)."
 "<p>"
 "You shall see another example, where c<sub>0</sub> of period 55 is close "
 "to the root a of period-doubling <nobr>4 &rarr; 8.</nobr> The embedded "
 "Julia set K<sub>M</sub><sup>em</sup> is a quasiconformal copy of an "
 "\"imploded basilica,\" but it is not similar to the \"fat basilica\" "
 "K<sub>a</sub><sup>4</sup>, since the small Julia set "
 "K<sub>c</sub><sup>4</sup> is discontinuous at the parabolic parameter "
 "c = a."
 "<p>"
 "Douady et al. have "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:1107.37303\">obtained</a> a sequence of embedded \"cauliflowers\" "
 "approaching the root <nobr>a &asymp; -0.1547246 + 1.0310472*i</nobr> of "
 "M<sub>4</sub> with definite scaling properties: e.g., the center "
 "c<sub>0</sub> has period 4n - 1, c<sub>0</sub> - a is of the order "
 "1/n<sup>2</sup>, and both the small Mandelbrot set and the surrounding "
 "\"cauliflower\" have diameters of the order 1/n<sup>3</sup>. A "
 "quasiconformal model of the collection of small Mandelbrot sets and "
 "embedded Julia sets is constructed by employing Fatou coordinates and "
 "choosing the parameters of the \"imploded cauliflowers\" and the "
 "distances appropriately. Here the images will have a width of "
 "0.0176/n<sup>3</sup> in the parameter plane and 0.00008 in the "
 "dynamic plane."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to observe an "
 "embedded Julia set close to the root of period doubling 4 &rarr; 8, and "
 "the series of \"cauliflowers\" for n = 6, 7, 8 ... 64 approaching the "
 "root of M<sub>4</sub> .</i><br>"
 "The latter kind of rescaling is not available in the main program. To "
 "mark embedded Julia sets, you must first zoom in further to determine "
 "the period of the small Mandelbrot set surrounded by the embedded Julia "
 "set. Zoom out again and take this period as the preperiod of the "
 "renormalization."));
   pplane->setNmax(5000); dplane->setNmax(5000);
   mdouble x = -0.16234443219491, y = 1.03041793343988;
   pplane->setPlane(x, y, 0.00000065, 0); pplane->draw(pf, 1, &mode);
   dplane->setPlane(x, y, 0.0000016, 0); pplane->setPoint(x, y);
   //root -0.154724605511925156L + i 1.031047227748952040L
   //a = -0.154718127301096   b = 1.0310433432646   period = 431
}
if (page == 10)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Suppose that f<sub>c</sub>(z) is p-renormalizable, i.e., there is a "
 "p-cycle of connected small Julia sets, and "
 "f<sub>c</sub><sup>p</sup>(z) is conjugate to f<sub>d</sub>(z) for a "
 "unique parameter d in M. The periodic points corresponding to the "
 "fixed points &alpha;<sub>d</sub> and &beta;<sub>d</sub> are the small "
 "&alpha;<sub>c</sub><sup>p</sup> and &beta;<sub>c</sub><sup>p</sup>."
 "There are <a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0822.30002\">three</a> possible cases:<ul>"
 "<li>The p small Julia sets are disjoint, &alpha;<sub>c</sub><sup>p</sup> "
 "and &beta;<sub>c</sub><sup>p</sup> are p-periodic. Then the parameter c "
 "belongs to a primitive small Mandelbrot set.</li>"
 "<li>Some of the p small Julia sets touch at their "
 "&beta;<sub>c</sub><sup>p</sup>, whose period divides p. "
 "&alpha;<sub>c</sub><sup>p</sup> is p-periodic. Then the parameter c "
 "belongs to a satellite small Mandelbrot set.</li>"
 "<li>Some of the p small Julia sets cross at their "
 "&alpha;<sub>c</sub><sup>p</sup>, whose period divides p. "
 "&beta;<sub>c</sub><sup>p</sup> is p-periodic. The set of these parameters "
 "was <a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
  "q=an:0942.37029\">described</a> by Johannes Riedl and Dierk "
  "Schleicher.</li>"
 "</ul>For a primitive or satellite center c<sub>0</sub> , the inverse "
 "of the straightening map c &rarr; d = h(c) is the tuning map "
 "<nobr>d &rarr; c = c<sub>0</sub> * d.</nobr> Small Mandelbrot sets "
 "may be nested, and we have <nobr>"
 "c<sub>1</sub> * (c<sub>2</sub> * d) = (c<sub>1</sub> * c<sub>2</sub>) * d."
 "</nobr> An infinitely renormalizable parameter c is contained in an "
 "infinite sequence of nested small Mandelbrot sets. These parameters are "
 "studied extensively with respect to questions of local connectivity or "
 "measure."
 "<p>"
 "The sequence of repeated period doubling leads to the infinitely "
 "renormalizable Feigenbaum parameter c<sub>F</sub> &asymp; -1.401155189, "
 "the nested small Mandelbrot sets are scaled asymptotically by "
 "<nobr>&delta;<sub>F</sub> &asymp; 4.669201609,</nobr> and the small "
 "Julia sets around z = 0 (not z = c) by "
 "&alpha;<sub>F</sub> &asymp; 2.502907875 . The decorations in the parameter "
 "plane become more hairy, and converge to a dense subset of the "
 "plane. This <a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0676.58036\">conjecture</a> by John Milnor was "
 "<a href=\"http://arxiv.org/abs/math/9903201\">proved</a> by Mikhail "
 "Lyubich, and Curt McMullen has "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0860.58002\">shown</a> convergence of blow-ups of the Feigenbaum "
 "Julia set.<p>"
 "<i>Hit the Return key or push the Go-button a few times to rescale "
 "the parameter plane by &delta;<sub>F</sub> around "
 "<nobr>c = c<sub>F</sub></nobr> and the dynamic plane by "
 "&alpha;<sub>F</sub> around z = 0.</i><br>"
 "In the main program, Feigenbaum scaling is available in the parameter "
 "plane with the key Shift+f. You will have to increase the number of "
 "iterations with n. Renormalization with the key r works "
 "best in the primitve case."));
   pplane->setPlane(-1.0, 0, 1.5, 0); dplane->setPlane(0, 0, 2.1, 0);
   dplane->setNmax(125); pplane->setNmax(125);
   pplane->draw(pf, 1, &mode); pplane->setPoint(-1.0, 0);
}
} //showPage RN

void QmnDemoRN::go()
{  if (page <= 7 && page != 6)
   {  mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      for (int i = 1; i <= (page == 1 ? 1 : 4); i++) df->iterate(x, y);
      dplane->setPoint(x, y);
   }
   if (page == 6)
   {  mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
      df->iterate(x, y); dplane->setPoint(x, y);
      n <<= 1; if (n >= 4095) n -= 4095;
      if (n == 820)
      {  pplane->traceRay(1, mdouble(n)/4095.0, pf, x, y, 2);
         goButton->setEnabled(false);
      }
      else dplane->traceRay(-1, mdouble(n)/4095.0, df, x, y, 2);
   }
   if (page == 8)
   {  n = 0; mdouble x, y, u, v; dplane->getPlane(x, y, u, v);
      pplane->draw(pf, 1, &mode, 210);
      dplane->draw(df, -1, &mode, (u > 1e-6 ? 195 : 209) );
   }
   if (page == 9)
   {  mdouble x, y; n &= 2047; n++; if (n == 2) n = 6;
      if (n == 64) goButton->setEnabled(false);
      if (n == 1)
      {  pplane->setNmax(2000); dplane->setNmax(2000);
         x = -0.160424611936285; y = 1.03765097380051;
         pplane->setPlane(x, y, 0.0000116, 0); pplane->draw(pf, 1, &mode);
         dplane->setPlane(x, y, 0.000022, 0); pplane->setPoint(x, y); return;
      }
      x = -0.154724605511925156L + 0.0757/(n*n);
      y = 1.031047227748952040L - 0.0450/(n*n);
      pf->find(1, 0, 4*n - 1, x, y);
      pplane->setPlane(x, y, 0.0176/(n*n*n), 0); pplane->draw(pf, 1, &mode, 1);
      dplane->setPlane(x, y, 0.00008, 0); pplane->setPoint(x, y);
   }
   /*if (page == 10)
   {  uint N; pplane->getNmax(N); if (N <= 32500) N <<= 1;
      pplane->setNmax(N); dplane->setNmax(N);
      mdouble x, y, u, v, X, U; dplane->getPlane(X, y, U, v);
      pplane->getPlane(x, y, u, v); x = cFb + (x - cFb)/dFb;
      pplane->setPlane(x, 0, u/dFb, 0); pplane->draw(pf, 1, &mode);
      dplane->setPlane(x, 0, U/(aFb*aFb), 0); pplane->setPoint(x, 0);
   }// z = c */
   if (page == 10)
   {  uint N; pplane->getNmax(N); if (N <= 32500) N <<= 1;
      pplane->setNmax(N); dplane->setNmax(N);
      mdouble x, y, u, v; dplane->getPlane(x, y, u, v);
      dplane->setPlane(0, 0, u/aFb, 0);
      pplane->getPlane(x, y, u, v); x = cFb + (x - cFb)/dFb;
      pplane->setPlane(x, 0, u/dFb, 0); pplane->draw(pf, 1, &mode);
      pplane->setPoint(x, 0);
   }// z = 0 */
} //go RN

void QmnDemoRN::drawRight()
{  mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   if (page == 1 || page == 10) { x = 0; y = 0; }
   if (page >= 2 && page <= 7 && page != 6)
   {  x = 0; y = 0; //omega:
      df->iterate(x, y, -2); df->iterate(x, y, -1); df->iterate(x, y, -1);
   }
   dplane->setPoint(x, y); n &= 4095; mode = 1;
   dplane->draw(df, -1, &mode,
      (page == 4 || page == 5 || page == 7 || page == 9 ? 1 : 0) );
}

void QmnDemoRN::pFinished()
{  if (page == 5 && !(n & 2048) )
   {  //pplane->drawEllipse(-0.156520, 1.0322471, 0.027, 0.027, Qt::red);
      n |= 2048; uint n1 = 1028; pplane->draw(pf, 1, &n1, 2);
   }
   if (page == 6)
   {  mdouble x, y;
      pplane->traceRay(1, 3.0/15.0, pf, x, y, 2, Qt::red);
      pplane->traceRay(1, 4.0/15.0, pf, x, y, 2, Qt::magenta);
   }
   if (page == 7 && !(n & 2048))
   {  n |= 2048; mdouble x, y;
      pplane->traceRay(1, 3358735.0/16773120.0, pf, x, y, 10, Qt::darkGreen);
      pplane->traceRay(1, 3358975.0/16773120.0, pf, x, y, 10, Qt::darkGreen);
      pplane->traceRay(1, 3420145.0/16773120.0, pf, x, y, 10, Qt::darkGreen);
      uint n1 = 1028; pplane->draw(pf, 1, &n1, 2);
   }
   if (page == 8 && !(n & 2048) )
   {  n |= 2048; mdouble x, y, u, v; pplane->getPlane(x, y, u, v);
      if (u > 0.000005 || u < 0.00000001) return;
      uint n1 = 1099; pplane->draw(pf, 1, &n1, 2);
   }
   if (page == 9 && !(n & 2048) )
   {  n |= 2048; uint n1 = uint(n) & 2047; n1 *= 4;
      if (!n1) n1 = 139; else if (n1 == 4) n1 = 55; else n1--;
      n1 |= 1024; pplane->draw(pf, 1, &n1, 2);
   }
}

void QmnDemoRN::dFinished()
{  if (page >= 3 && page <= 7 && page != 6 && !(n & 4096) )
   {  const mdouble xomega = -0.156520, yomega = 1.0322471, R = 0.027;
      mdouble t, a, b, x = xomega + R, y = yomega, x0, y0, u, v;
      df->getParameter(a, b);
      dplane->drawEllipse(xomega, yomega, R, R, Qt::red);
      for (int j = 0; j <= 1; j++)
      {  mndynamics::root(xomega + R - a, yomega - b, u, v);
         if (j) { u = -u; v = -v; }
         for (t = 0; t < 6.31; t += 0.02)
         {  x0 = u; y0 = v;
            mndynamics::root(xomega+R*cos(t) - a, yomega+R*sin(t) - b, u, v);
            if (u*x0 + v*y0 < 0) { u = -u; v = -v; } //closest for continuity
            x0 = x; y0 = y; mndynamics::root(u - a, v - b, x, y);
            mndynamics::root(x - a, y - b, x, y); x = -x; y = -y;
            mndynamics::root(x - a, y - b, x, y); x = -x; y = -y;
            if (t > 0) dplane->drawLine(x0, y0, x, y, Qt::darkCyan);
         }
      }
   }
   if (page == 7 && !(n & 4096))
   {  mdouble x, y;
      dplane->traceRay(-1, 3358735.0/16773120.0, df, x, y, 10, Qt::darkGreen);
      dplane->traceRay(-1, 3358975.0/16773120.0, df, x, y, 10, Qt::darkGreen);
      dplane->traceRay(-1, 3420145.0/16773120.0, df, x, y, 10, Qt::darkGreen);
   }
   if (page >= 3 && page <= 7 && page != 6 && !(n & 4096) )
   { n |= 4096; uint n1 = 1028; dplane->draw(df, -1, &n1, 2); }
   if (page == 6)
   {  mdouble x, y;
      dplane->traceRay(-1, 0.0, df, x, y, 2, Qt::yellow);
      dplane->traceRay(-1, 0.5, df, x, y, 2, Qt::yellow);
      dplane->traceRay(-1, 3.0/15.0, df, x, y, 2, Qt::red);
      dplane->traceRay(-1, 4.0/15.0, df, x, y, 2, Qt::magenta);
      dplane->traceRay(-1, 6.0/15.0, df, x, y, 2, Qt::red);
      dplane->traceRay(-1, 8.0/15.0, df, x, y, 2, Qt::magenta);
      dplane->traceRay(-1, 12.0/15.0, df, x, y, 2, Qt::red);
      dplane->traceRay(-1, 1.0/15.0, df, x, y, 2, Qt::magenta);
      dplane->traceRay(-1, 9.0/15.0, df, x, y, 2, Qt::red);
      dplane->traceRay(-1, 2.0/15.0, df, x, y, 2, Qt::magenta);
      dplane->traceRay(-1, 820.0/4095.0, df, x, y, 2);
   }
   if (page == 8 && !(n & 4096) )
   {  n |= 4096; mdouble x, y, u, v; pplane->getPlane(x, y, u, v); //not d
      if (u > 0.000005 || u < 0.00000001) return;
      uint n1 = 1099; dplane->draw(df, -1, &n1, 2);
   }
   if (page == 9 && !(n & 4096) )
   {  n |= 4096; uint n1 = uint(n) & 2047; n1 *= 4;
      if (!n1) n1 = 139; else if (n1 == 4) n1 = 55; else n1--;
      n1 |= 1024; dplane->draw(df, -1, &n1, 2);
   }
}

////////////////////////////////////////////////////////////////////////

void QmnDemoAS::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1;
   if ((page == 1 && !backwards) || (page == 9 && backwards))
   {  if (page == 1) { as = new mndScale(); t = new mdouble[7]; }
      pplane->setNmax(500); dplane->setNmax(500);
      a = -0.101096363845622L; b = 0.956286510809142L;
      df->setParameter(a, b); t[0]= a; t[1] = b;
      t[2] = 1.328330115300301L; t[3] = 2.086595344738708L;
      t[4] = -0.035; t[5] = 0.03; as->set((7 << 16) | 3, -1, t);
   }
if (page == 1)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "A Misiurewicz point is a parameter c, such that the critical point "
 "z = 0 is strictly preperiodic under the iteration of "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c. These parameters are dense in the "
 "boundary &part;M of the Mandelbrot set M. For all c in the 1/3-limb of M, "
 "the fixed point &alpha;<sub>c</sub> has the combinatorial rotation number "
 "1/3, and it is repelling except at the root of period tripling. Our "
 "example will be the &alpha;-type Misiurewicz point of lowest preperiod "
 "in this limb: for c = a, the critical value z = a is mapped to the "
 "repelling fixed point &alpha;<sub>a</sub> in 3 iterations. The critical "
 "orbit is marked in the right window. The filled Julia set K<sub>a</sub> "
 "is a dendrite, i.e., a locally connected branched curve. It has three "
 "branches at z = a, since a neighborhood of z = a is mapped 1:1 to a "
 "neighborhood of z = &alpha;<sub>a</sub> by f<sub>a</sub><sup>3</sup>(z), "
 "and K<sub>a</sub> has three branches at z = &alpha;<sub>a</sub> . "
 "Therefore it has six branches at the critical point z = 0, and preimages "
 "of 0 with six branches are dense on K<sub>a</sub> ."
 "<p>"
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0726.58026\">Tan Lei</a> has shown that the Mandelbrot set is "
 "asymptotically self-similar at c = a, and similar to the filled Julia set "
 "K<sub>a</sub> . Her proof will be explained on page 4, after introducing "
 "the Schroeder conjugation &phi;<sub>a</sub>(z) and the asymptotic model "
 "set X<sub>a</sub> . The scaling factor of self-similarity is given by the "
 "multiplier &rho;<sub>a</sub> = f<sub>a</sub>'(&alpha;<sub>a</sub>) = "
 "2&alpha;<sub>a</sub> . Since the argument of &rho;<sub>a</sub><sup>3</sup> "
 "is small, each of the three branches of K<sub>a</sub> at z = "
 "&alpha;<sub>a</sub> or at z = a spirals very slowly. The same applies "
 "to the three branches of M at c = a. Note that for K<sub>c</sub> with "
 "c &ne; a, in general the branch points with six branches "
 "are replaced with two branch points having three branches, see page 10 of "
 "Chapter 3. The corresponding pairs of branch points in M accompany small "
 "Mandelbrot sets. Close to c = a, these small Mandelbrot sets, and the "
 "distance of the two branch points, get relatively small. Thus it "
 "looks like M has branch points with six branches here as well."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in "
 "smoothly around the Misurewicz point c = a and the preperiodic point "
 "z = a, respectively.</i><br>"
 "In the main program, hit the key x to find centers and Misiurewicz points "
 "in the parameter plane, or periodic and preperiodic points in the dynamic "
 "plane. Smooth zooming is done with the key z."));
   pplane->setPlane(-0.83, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   pplane->setPoint(a, b); dplane->setPoint(a, b);
   n = 0; pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
if (page == 2)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "At the repelling fixed point &alpha;<sub>a</sub> with the multiplier "
 "&rho;<sub>a</sub> = f<sub>a</sub>'(&alpha;<sub>a</sub>) = "
 "2&alpha;<sub>a</sub> , the quadratic polynomial "
 "f<sub>a</sub>(z) = z<sup>2</sup> + a is conjugate to a linear map "
 "g(x) = &rho;<sub>a</sub>x :<br>"
 "&phi;<sub>a</sub>(f<sub>a</sub>(&phi;<sub>a</sub><sup>-1</sup>(x))) = "
 "&rho;<sub>a</sub>x and f<sub>a</sub>(z) = "
 "&phi;<sub>a</sub><sup>-1</sup>(&rho;<sub>a</sub>&phi;<sub>a</sub>(z)) .<br>"
 "The Schroeder conjugation &phi;<sub>a</sub>(z) satisfies "
 "&phi;<sub>a</sub>(&alpha;<sub>a</sub>) = 0 and "
 "&phi;<sub>a</sub>'(&alpha;<sub>a</sub>) = 1. It can be constructed with an "
 "argument from <a href=\"javascript:alert('When an electron flies "
 "through an electromagnetic field vanishing at &infin;, its motion is "
 "asymptotically free. The wave operators conjugate its dynamics "
 "to the free motion, analogously to the Schroeder conjugation. The "
 "Boettcher conjugation is constructed in the same way (see page 1 of "
 "Chapter 3), but here the dynamics is 2:1, and iterating backwards "
 "requires to choose between two preimages.')\">scattering</a> theory, by "
 "iterating backwards n times at the fixed point &alpha;<sub>a</sub> , "
 "iterating forward by the linear map, and taking the limit as "
 "n &rarr; &infin; :<br>"
 "&phi;<sub>a</sub>(z) = lim<sub>n&rarr;&infin;</sub> "
 "&rho;<sub>a</sub><sup>n</sup> <big>(</big> "
 "f<sub>a</sub><sup>-n</sup>(z) - &alpha;<sub>a</sub> <big>)</big> .<br>"
 "The asymptotic model set X<sub>a</sub> is an unbounded set and "
 "linearly self-similar with the complex scaling factor &rho;<sub>a</sub> , "
 "i.e., it is invariant under multiplication with &rho;<sub>a</sub> . "
 "Locally, the Schroeder conjugation &phi;<sub>a</sub>(z) maps "
 "K<sub>a</sub> around z = &alpha;<sub>a</sub> to X<sub>a</sub> around "
 "x = 0. Its inverse &phi;<sub>a</sub><sup>-1</sup>(x) extends to an entire "
 "function, which maps X<sub>a</sub> to K<sub>a</sub> , folding the "
 "unbounded set an infinite number of times. X<sub>a</sub> can be "
 "constructed by blowing up the filled Julia set K<sub>a</sub> at the fixed "
 "point &alpha;<sub>a</sub> repeatedly with &rho;<sub>a</sub> :<br>"
 "X<sub>a</sub> = lim<sub>n&rarr;&infin;</sub> "
 "&rho;<sub>a</sub><sup>n</sup> <big>(</big> "
 "K<sub>a</sub> - &alpha;<sub>a</sub> <big>)</big> .<br>"
 "For parameters c close to the Misiurewicz point a, the filled Julia set "
 "K<sub>c</sub> contains a preimage z<sub>c</sub> of the fixed point "
 "&alpha;<sub>c</sub> , which moves "
 "<a href=\"javascript:alert('Analytic or holomorphic means complex "
    "differentiable, see page 1 of Chapter 8.')\">analytically</a> "
 "with c and satisfies z<sub>a</sub> = a but z<sub>c</sub> &ne; c for "
 "c &ne; a. Locally at &alpha;<sub>c</sub> , the Schroeder conjugation "
 "&phi;<sub>c</sub>(z) maps K<sub>c</sub> to X<sub>c</sub> , which is "
 "linearly self-similar with the scaling factor &rho;<sub>c</sub> = "
 "f<sub>c</sub>'(&alpha;<sub>c</sub>) = 2&alpha;<sub>c</sub> . At the "
 "Misiurewicz point, both families of sets depend continuously on the "
 "parameter: for c &rarr; a we have K<sub>c</sub> &rarr; K<sub>a</sub> and "
 "X<sub>c</sub> &rarr; X<sub>a</sub> ."
 "<p>"
 "Observe the convergence of "
 "&rho;<sub>a</sub><sup>n</sup> ( K<sub>a</sub> - &alpha;<sub>a</sub> ) "
 "to the asymptotic model X<sub>a</sub> as follows:<br>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in around "
 "the repelling fixed point &alpha;<sub>a</sub> by the complex scaling "
 "factor &rho;<sub>a</sub> = f<sub>a</sub>'(&alpha;<sub>a</sub>) = "
 "2&alpha;<sub>a</sub> .</i>"));
   pplane->setPlane(-0.83, 0, 1.6, 0); dplane->setPlane(0, 0, 2.0, 0);
   pplane->setPoint(a, b);
   dplane->setPoint(-0.327586178699357L, 0.577756453298949L);
   n = 0; pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
if (page == 3)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The distance of two closed sets, which may be unbounded, is measured with "
 "the Hausdorff-Chabauty metric: for a radius r &gt; 0, the distance "
 "of A and B is the smallest &epsilon; &gt; 0, such that A (restricted to "
 "the disk of radius r - &epsilon;) is contained in an "
 "&epsilon;-neighborhood of B and vice versa. Recall from the previous page "
 "that the model set X<sub>a</sub> is obtained by blowing up the filled "
 "Julia set K<sub>a</sub> at the repelling fixed point &alpha;<sub>a</sub> : "
 "&rho;<sub>a</sub><sup>n</sup> ( K<sub>a</sub> - &alpha;<sub>a</sub> ) "
 "&rarr; X<sub>a</sub> , i.e., the Hausdorff-Chabauty distance of these sets "
 "goes to 0 for any r &gt; 0. Now f<sub>a</sub><sup>3</sup>(z) maps a "
 "neighborhood of z = a to a neighborhood of z = &alpha;<sub>a</sub> , and "
 "thus <br>"
 "&rho;<sub>a</sub><sup>n</sup> ( K<sub>a</sub> - a ) "
 "&rarr; X<sub>a</sub> / (f<sub>a</sub><sup>3</sup>)'(a) .<br>"
 "On the following page, we shall introduce a function u(c) and obtain an "
 "analogous result for blowing up the Mandelbrot set M in the parameter "
 "plane:<br>"
 "&rho;<sub>a</sub><sup>n</sup> ( M - a ) &rarr; X<sub>a</sub> / u'(a) .<br>"
 "Combining both results shows that the Hausdorff-Chabauty distance of "
 "&lambda;(M - a) and (K<sub>a</sub> - a) on the disk of radius r "
 "goes to 0 faster than r. Here the similarity factor is defined by "
 "&lambda; = u'(a) / (f<sub>a</sub><sup>3</sup>)'(a) . Juan Rivera-Letelier "
 "has <a href=\"http://www.math.sunysb.edu/~rivera/mypapers/semi.ps\">"
 "shown</a> that the distance is of the relative order &radic;r in fact."
 "<p>"
 "Note that blowing up by the complex scaling factor entails a rotation by "
 "approximately 120<sup>o</sup>.<br>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in "
 "around the Misiurewicz point c = a and the preperiodic point z = a, "
 "respectively, by the complex scaling factor &rho;<sub>a</sub> .</i>"));
   pplane->setPlane(a, b, 2.15, 0); dplane->setPlane(a, b, 2.15, 0);
   pplane->setPoint(a, b); dplane->setPoint(a, b);
   pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
if (page == 4)
{  text->setHtml(QString(
 "The function u(c) = &phi;<sub>c</sub>(f<sub>c</sub><sup>3</sup>(c)) maps "
 "the parameter plane to the model plane, with u(a) = 0 and "
 "u'(a) &ne; 0. The convergence of "
 "&rho;<sub>a</sub><sup>n</sup> u'(a) ( M - a ) &rarr; X<sub>a</sub> is "
 "shown in two steps:<ul><li>"
 "A parameter c belongs to the Mandelbrot set M, if the point z = c belongs "
 "to the filled Julia set K<sub>c</sub> . For c close to a, this means that "
 "u(c) = &phi;<sub>c</sub>(f<sub>c</sub><sup>3</sup>(c)) belongs to the "
 "asymptotic model set X<sub>c</sub> . When c - a is of the order "
 "|&rho;<sub>a</sub>|<sup>-n</sup>, then "
 "&rho;<sub>a</sub><sup>n</sup> u'(a) (c - a) &asymp; "
 "&rho;<sub>c</sub><sup>n</sup> u(c). Thus it is close to X<sub>c</sub> and "
 "to X<sub>a</sub> .</li><li>"
 "Suitable repelling periodic points x<sub>c</sub> in X<sub>c</sub> "
 "move holomorphically with the parameter c. There are corresponding "
 "sequences of Misiurewicz points c<sub>n</sub> satisfying "
 "&rho;<sub>c</sub><sup>n</sup> u(c) = x<sub>c</sub> . Then for large n, "
 "x<sub>a</sub> is close to "
 "&rho;<sub>a</sub><sup>n</sup> u'(a) (c<sub>n</sub> - a)."
 "</li></ul>"
 "On the disk of radius |&rho;<sub>a</sub>|<sup>-n</sup>, the distance of "
 "u'(a) (M - a) and the model set X<sub>a</sub> is of the order "
 "|&rho;<sub>a</sub>|<sup>-3/2 n</sup>."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in "
 "around the Misiurewicz point c = a and the preperiodic point z = a, "
 "respectively, by the complex scaling factor &rho;<sub>a</sub> . The "
 "different scales will be superimposed in different colors.</i><br>"
 "In the main program, specify a Misiurewicz point with the key t or "
 "choose it from a menu with Ctrl+t. Rescale both planes by "
 "&rho;<sub>a</sub> with the Return key. Use ctrl+l to set the order of "
 "rescaling, i.e., to scale by a power of &rho;<sub>a</sub> , or to return "
 "to a lesser magnification. Hit Home to turn off rescaling."));
   mode = 17; if (backwards) df->setParameter(a, b);
   pplane->setPlane(a, b, 0.183, 0); dplane->setPlane(a, b, 0.141, 0);
   pplane->setPoint(a, b); dplane->setPoint(a, b);
   pplane->draw(pf, 0, &mode); dplane->draw(df, 0, &mode);
   pplane->draw(pf, 1, &mode, 2); dplane->draw(df, -1, &mode, 2);
}
if (page == 5)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The dynamics of the quadratic polynomial f<sub>c</sub>(z) = "
 "z<sup>2</sup> + c at the repelling fixed point &alpha;<sub>c</sub> can be "
 "described in terms of fundamental domains: choose a preperiodic pinching "
 "point in the filled Julia set K<sub>c</sub> and consider its preimages "
 "that spiral towards &alpha;<sub>c</sub> . The part of K<sub>c</sub> "
 "between two adjacent pinching points is a fundamental domain. It will be "
 "mapped to the next one by three iterations of f<sub>c</sub>(z). The "
 "preimages of these fundamental domains at the preimage z<sub>c</sub> of "
 "&alpha;<sub>c</sub> are denoted by S<sub>c</sub><sup>n</sup>, and the "
 "corresponding subsets of the Mandelbrot set M are labeled "
 "S<sub>M</sub><sup>n</sup>. When the parameter c belongs to "
 "S<sub>M</sub><sup>n</sup>, then the critical value z = c will be in "
 "S<sub>c</sub><sup>n</sup>. It is mapped to a fundamental domain at "
 "&alpha;<sub>c</sub> in three iterations, to a neighborhood of z = 0 in n "
 "iterations, and close to z = c again in one iteration. The sets "
 "S<sub>M</sub><sup>n</sup> are similar to each other in several ways:"
 "<ul><li>"
 "Since S<sub>c</sub><sup>n</sup> is mapped 2:1 to a larger set in n + 4 "
 "iterations, we have a quadratic-like map, and each "
 "S<sub>M</sub><sup>n</sup> contains a primitive small Mandelbrot set "
 "of period n + 4. The periods are 4, 7, 10, ... on the left branch at a, "
 "5, 8, 11, ... on the right branch, and 6, 9, 12, ... on the lower branch. "
 "On each branch, two Misiurewicz points in S<sub>M</sub><sup>n</sup> "
 "form a typical pattern, see also pages 10 and 12 of Chapter 3. The "
 "centers c<sub>n</sub> of period n + 4 satisfy "
 "&rho;<sub>c</sub><sup>n</sup> u(c) = &phi;<sub>c</sub>(0), so they "
 "converge to a with the asymptotics "
 "c<sub>n</sub> &asymp; a + K &rho;<sub>a</sub><sup>-n</sup>, where "
 "K = &phi;<sub>a</sub>(0) / u'(a)."
 "</li><li>"
 "The fundamental domains S<sub>M</sub><sup>n</sup> in the Mandelbrot set "
 "have an asymptotic model set "
 "S = lim<sub>n&rarr;&infin;</sub> &rho;<sub>a</sub><sup>n</sup> "
 "( S<sub>M</sub><sup>n</sup> - a ), and the Hausdorff-Chabauty distance of "
 "(S<sub>M</sub><sup>n</sup> - a) and &rho;<sub>a</sub><sup>-n</sup> S is of "
 "the order |&rho;<sub>a</sub>|<sup>-3/2 n</sup>."
 "</li><li>"
 "The fundamental domains S<sub>M</sub><sup>n</sup> on the same branch are "
 "mutually homeomorphic in fact, i.e., they are mapped to each other "
 "continuously. The homeomorphism is "
 "<a href=\"http://www.mndynamics.com/indexm.html#surge\">constructed</a> "
 "by quasiconformal surgery, see pages 8-10 of Chapter 8."
 "</li></ul>"
 "Note that some statements refer to all branches simultaneously, while "
 "others are restricted to some branch, i.e., n is increased by 3."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to move the "
 "parameter c to the next center c<sub>n</sub> and z to a preimage of 0 "
 "in K<sub>a</sub> . Fundamental domains on the left branch are marked with "
 "external rays.</i>"));
   pplane->setPlane(-0.118833, 0.985108, 0.0833109, 0);
   dplane->setPlane(-0.101096, 0.981060, 0.080210, 0);
   pplane->setPoint(-0.1565201668L, 1.0322471089L);
   dplane->setPoint(-0.1236653660L, 1.0314293788L);
   pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
   n = -3; t[6] = 1.0;
}
if (page == 6)
{  text->setHtml(QString(
 "The distance of the centers c<sub>n</sub> to the Misurewicz point a is of "
 "the order |&rho;<sub>a</sub>|<sup>-n</sup>, and the Mandelbrot set has "
 "four arms of this size there, which are attached to two branch points in "
 "three alternating patterns. Combining the asymptotics from pages 4 and 5 "
 "gives<br>"
 "&rho;<sub>a</sub><sup>n</sup> ( M - c<sub>n</sub> ) "
 "&rarr; ( X<sub>a</sub> - &phi;<sub>a</sub>(0) ) / u'(a) .<br>"
 "On the scale &rho;<sub>a</sub><sup>-n</sup>, the Mandelbrot set at the "
 "centers c<sub>n</sub> is asymptotic to a subset of the asymptotic model "
 "X<sub>a</sub> around a branch point with six branches. While M contains "
 "two branch points with a distance of the order "
 "|&rho;<sub>a</sub>|<sup>-3/2 n</sup>, and small Mandelbrot sets with "
 "diameter |&rho;<sub>a</sub>|<sup>-2n</sup>, these shrink to a point on "
 "the scale |&rho;<sub>a</sub>|<sup>-n</sup>."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to increase the "
 "order n. The left image shows a subset of M around c<sub>n</sub> , and "
 "the right image shows the corresponding subset of K<sub>c</sub> for "
 "c = c<sub>n</sub> . Both sets are shown on the scale "
 "&rho;<sub>a</sub><sup>-n</sup>.</i>"));
   t[6] = 1.0;
   if (backwards) { pplane->setNmax(500); dplane->setNmax(500); }
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For the center parameters c = c<sub>n</sub> , the iterate "
 "f<sub>c</sub><sup>n+4</sup>(z) is approximately c<sub>n</sub> + "
 "A &rho;<sub>a</sub><sup>2n</sup>(z - c<sub>n</sub>)<sup>2</sup> for some "
 "A, since the orbit travels n times through a neigborhood of "
 "z = &alpha;<sub>c</sub> , where the dynamics is conjugate to a "
 "multiplication with &rho;<sub>c</sub> , before it reaches the critical "
 "point z = 0. Thus the small Julia set has a diameter of the order "
 "|&rho;<sub>a</sub>|<sup>-2n</sup>, and the higher powers of "
 "(z - c<sub>n</sub>) are negligible on this scale. For parameters c close "
 "to c<sub>n</sub> , the critical value of the quadratic-like restriction of "
 "f<sub>c</sub><sup>n+4</sup>(z) is c but the critical point "
 "&omega;<sub>c</sub> is different. Since c - &omega;<sub>c</sub> is of the "
 "order |&rho;<sub>a</sub>|<sup>-2n</sup> as well, both the dynamic plane "
 "and the parameter plane are rescaled affinely by "
 "&rho;<sub>a</sub><sup>2n</sup>. For large n, the higher powers in the "
 "rescaled quadratic-like map are negligible, and the quasiconformal "
 "conjugation to a quadratic polynomial is close to the identity. (See "
 "Chapters 5 and 8 for details of the Straightening Theorem.) The "
 "sequence of small Mandelbrot sets at a Misiurewicz point was "
 "obtained independently by "
 "<a href=\"http://www.math.cornell.edu/~hubbard/PolyLikeMaps.pdf\">"
 "Douady and Hubbard</a> and "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0587.30027\">Eckmann and Epstein</a>. This result was generalized by "
 "<a href=\"http://www.math.harvard.edu/~ctm/papers/home/text/papers/"
 "muniv/muniv.pdf\">Curt McMullen</a> to generic families of rational "
 "maps."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to increase the "
 "order n. The left image shows a subset of M around c<sub>n</sub> , and "
 "the right image shows the corresponding subset of K<sub>c</sub> for "
 "c = c<sub>n</sub> . Both sets are shown on the scale "
 "&rho;<sub>a</sub><sup>-2n</sup>.</i>"));
   t[6] = 2.0; pplane->setNmax(1000); dplane->setNmax(1000);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For parameters c close to the centers c<sub>n</sub> , the quadratic-like "
 "restriction of the iterate f<sub>c</sub><sup>n+4</sup>(z) is approximately "
 "c + A &rho;<sub>a</sub><sup>2n</sup>(z - &omega;<sub>c</sub>)<sup>2</sup>. "
 "Close to the small Julia set of diameter "
 "|&rho;<sub>a</sub>|<sup>-2n</sup>, the filled Julia set K<sub>c</sub> has "
 "two large arms and four arms of length |&rho;<sub>a</sub>|<sup>-n</sup>. "
 "Their preimages are eight arms of length "
 "|&rho;<sub>a</sub>|<sup>-3/2 n</sup>, sixteen arms of length "
 "|&rho;<sub>a</sub>|<sup>-7/4 n</sup> ... The techniques from page 4 "
 "<a href=\"http://www.mndynamics.com/indexm.html#misi\">show</a> that there "
 "are asymptotic models in the parameter plane as well, approximated by the "
 "Mandelbrot set around the centers c<sub>n</sub> on multiple scales:<br>"
 "&rho;<sub>a</sub><sup>3/2 n</sup> ( M - c<sub>n</sub> ) &rarr; "
 "A<sub>1</sub> ( X<sub>a</sub> - &phi;<sub>a</sub>(0) )<sup>1/2</sup><br>"
 "&rho;<sub>a</sub><sup>7/4 n</sup> ( M - c<sub>n</sub> ) &rarr; "
 "A<sub>2</sub> ( X<sub>a</sub> - &phi;<sub>a</sub>(0) )<sup>1/4</sup>...<br>"
 "On the higher scales, the convergence to the asymptotic models is slower, "
 "i.e., n must be chosen much larger. There are similar asymptotics for the "
 "decorations of the small Julia sets."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to increase the "
 "order n. The left image shows a subset of M around c<sub>n</sub> , and "
 "the right image shows the corresponding subset of K<sub>c</sub> for "
 "c = c<sub>n</sub> . Both sets are shown on the scale "
 "&rho;<sub>a</sub><sup>-3/2 n</sup>.</i><br>"
 "In the main program, hit the key l to choose the level of rescaling, then "
 "use Return and Ctrl+l to change the order n."));
   t[6] = 1.5;
   if (!backwards) { pplane->setNmax(500); dplane->setNmax(500); }
}
if (page == 9)
{  text->setHtml(QString(
 "These asymptotic models on multiple scales provide an explanation of a "
 "frequent observation: when zooming in around a small Mandelbrot set, the "
 "number of visible decorations doubles from time to time, and their shape "
 "looks rotationally symmetric. For a given order n, this applies only to "
 "finitely many levels and finitely many decorations. There are infinitely "
 "many decorations close to the small Mandelbrot set, which would approach "
 "the asymptotic models of higher levels only for much larger values of the "
 "order n."
 "<p>"
 "On each level, the asymptotic models of (M - c<sub>n</sub>) and of "
 "(K<sub>c</sub> - &omega;<sub>c</sub>) differ by the same similarity factor "
 "&lambda; = u'(a) / (f<sub>a</sub><sup>3</sup>)'(a) . See Chapter 7 for the "
 "formulation and explanation of a local similarity between the decorations "
 "in the parameter plane and those in the dynamic plane, which is "
 "independent of these levels."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to increase the "
 "order n. The left image shows a subset of M around c<sub>n</sub> , and "
 "the right image shows the corresponding subset of K<sub>c</sub> for "
 "c = c<sub>n</sub> . Both sets are shown on the scale "
 "&rho;<sub>a</sub><sup>-7/4 n</sup>.</i>"));
   t[6] = 1.75;
}
if (6 <= page && page <= 9)
{  n = 0; t[4] = t[6]; as->set(-1, 0, t);
   pplane->setPlane(t[0], t[1], t[2], t[3]);
   dplane->setPlane(t[0], t[1], 0.846*t[2], 0.846*t[3]);
   pplane->setPoint(t[0], t[1]); dplane->setPoint(t[0], t[1]);
   df->setParameter(t[0], t[1]);
   pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
if (page == 10)
{  text->setHtml(QString(
  "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "When the small Mandelbrot sets at the centers c<sub>n</sub> are magnified "
 "by &rho;<sub>a</sub><sup>2n</sup>, they have some decorations of length "
 "|&rho;<sub>a</sub>|<sup>n</sup>, twice as many of length "
 "|&rho;<sub>a</sub>|<sup>n/2</sup>, four times as many of length "
 "|&rho;<sub>a</sub>|<sup>n/4</sup> ... As the order n is increased, "
 "decorations of higher levels slowly get longer and thinner. Each "
 "decoration converges to a curve, which has a simple description when "
 "the multiplier &rho;<sub>a</sub> is real: the rescaled small Mandelbrot "
 "set is close to the full Mandelbrot set, and the curve is a dyadic ray of "
 "the latter. In the limit n &rarr; &infin;, the decorations "
 "<a href=\"http://www.mndynamics.com/indexm.html#locsim\">get</a> "
 "dense in the plane, while their area goes to 0. They get "
 "thinner, and not more hairy as in the Feigenbaum case "
 "(cf. page 10 of Chapter 5)."
 "<p>"
 "The images show small Mandelbrot sets and small Julia sets with rescaled "
 "dyadic rays. The centers c<sub>n</sub> approach an &alpha;-type "
 "Misiurewicz point on the real line now.<br>"
 "<i>Hit the Return key or push the Go-button repeatedly to increase the "
 "order n.</i>"));
   pplane->setNmax(1000); dplane->setNmax(1000);
   a = -1.543689012692076L; b = -1.5410968L; t[0]= a; t[1] = 0;
   t[2] = sqrt(1 - 4*a) - 1; t[3] = PI;
   t[4] = -0.096; t[5] = 0; t[6] = 0.09;
   as->set((5 << 16) | 1, -1, t); n = 7; t[6] = 2.0;
   pplane->setPlane(b, 0, 0.000022, 0);
   dplane->setPlane(b + 0.09*0.000022, 0, 0.637*0.000022, 0);
   pplane->setPoint(b, 0); dplane->setPoint(b, 0);
   df->setParameter(b, 0);
   pplane->draw(pf, 1, &mode); dplane->draw(df, -1, &mode);
}
} //showPage AS

void QmnDemoAS::go()
{  if (page == 1)
   {  pplane->draw(pf, 1, &mode, 210); dplane->draw(df, -1, &mode, 210);
      n++;
   }
   if (page == 2)
   {  mdouble x, y, u, v; t[6] = 3.0; t[4] = 0; t[5] = 0; n = 11;
      dplane->getPlane(x, y, u, v);
      x += 0.226489814853735L; y += 0.378530057510193L;
      t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      as->set(0, 1, t);
      t[0] -= 0.226489814853735L; t[1] -= 0.378530057510193L;
      dplane->setPlane(t[0], t[1], t[2], t[3]);
      dplane->draw(df, -1, &mode, 1);
   }
   if (page == 3)
   {  mdouble x, y, u, v; t[6] = 3.0; t[4] = 0; t[5] = 0;
      pplane->getPlane(x, y, u, v);
      t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      as->set(0, 1, t);
      pplane->setPlane(t[0], t[1], t[2], t[3]);
      pplane->draw(pf, 1, &mode, 1);
      dplane->setPlane(t[0], t[1], t[2], t[3]);
      dplane->draw(df, -1, &mode, 1);
   }
   if (page == 4)
   {  mode++; if (mode == 31) goButton->setEnabled(false);
      mdouble x, y, u, v; t[6] = 3.0; t[4] = 0; t[5] = 0;
      pplane->getPlane(x, y, u, v);
      t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      as->set(0, 3, t);
      pplane->setPlane(t[0], t[1], t[2], t[3]);
      pplane->draw(pf, 1, &mode, 2);
      dplane->getPlane(x, y, u, v);
      t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      as->set(0, 3, t);
      dplane->setPlane(t[0], t[1], t[2], t[3]);
      dplane->draw(df, -1, &mode, 2);
   }
   if (page == 5)
   {  n += 3; mdouble x, y, u, v;
      x = 11.0/56.0 + pow(0.5, mdouble(n))/8064.0;
      u = 15.0/56.0 - pow(0.5, mdouble(n))/8064.0;
      pplane->traceRay(1, x, pf, y, v, 2);
      pplane->traceRay(1, u, pf, y, v, 2);
      dplane->traceRay(-1, x, df, y, v, 2);
      dplane->traceRay(-1, u, df, y, v, 2);
      if (!n) { as->set(-1, 0, t); x = t[0]; y = t[1]; }
      else
      {  pplane->getPoint(x, y); dplane->getPoint(u, v);
         t[0] = x; t[1] = y; t[4] = u; t[5] = v;
         as->set(n - 3, n, t); x = t[4]; y = t[5];
      }
      pplane->setPoint(t[0], t[1]); pplane->move(5);
      df->find(-1, n + 10, 1, x, y);
      dplane->setPoint(x, y); dplane->move(5);
   }
   if (page >= 6)
   {  n++; mdouble x, y, u, v; pplane->getPlane(x, y, u, v);
      t[0] = x; t[1] = y; t[2] = u; t[3] = v;
      t[4] = x; t[5] = y;
      as->set(n - 1, n, t);
      pplane->setPlane(t[0], t[1], t[2], t[3]);
      if (page <= 9)
         dplane->setPlane(t[0], t[1], 0.846*t[2], 0.846*t[3]);
      else dplane->setPlane(t[0] + 0.09*t[2], 0, 0.637*t[2], 0);
      pplane->setPoint(t[0], t[1]); dplane->setPoint(t[0], t[1]);
      df->setParameter(t[0], t[1]);
      pplane->draw(pf, 1, &mode, 1); dplane->draw(df, -1, &mode, 1);
   }
} //go AS

void QmnDemoAS::pFinished()
{  if(page == 5)
   {  mdouble y, v; pplane->move(5);
      pplane->traceRay(1, 103.0/504.0, pf, y, v, 2);
      pplane->traceRay(1, 131.0/504.0, pf, y, v, 2);
      pplane->traceRay(1, 199.0/1008.0, pf, y, v, 2);
      pplane->traceRay(1, 269.0/1008.0, pf, y, v, 2);
   }
   if (page == 10)
   {  mdouble x, y, u, v; pplane->getPlane(x, y, u, v);
      pplane->setPlane(-0.4766, 0, 3.917, 0);
      pplane->traceRay(1, 0.125, pf, y, v, 2);
      pplane->traceRay(1, 0.25, pf, y, v, 2);
      pplane->traceRay(1, 0.375, pf, y, v, 2);
      pplane->traceRay(1, 0.625, pf, y, v, 2);
      pplane->traceRay(1, 0.75, pf, y, v, 2);
      pplane->traceRay(1, 0.875, pf, y, v, 2);
      pplane->setPlane(x, 0, u, 0);
   }
}

void QmnDemoAS::dFinished()
{  if (page <= 2 && n <= 10)
   {  mdouble x = a, y = b;
      dplane->setPoint(x, y); dplane->move(5); df->iterate(x, y);
      dplane->setPoint(x, y); dplane->move(5);
      x = -0.327586178699357L; y = 0.577756453298949L;
      dplane->setPoint(-x, -y); dplane->move(5);
      dplane->setPoint(x, y); dplane->move(5);
      if (page == 1) dplane->setPoint(a, b);
   }
   if (page == 5)
   {  mdouble y, v; dplane->move(5);
      dplane->traceRay(-1, 103.0/504.0, df, y, v, 2);
      dplane->traceRay(-1, 131.0/504.0, df, y, v, 2);
      dplane->traceRay(-1, 199.0/1008.0, df, y, v, 2);
      dplane->traceRay(-1, 269.0/1008.0, df, y, v, 2);
   }
   if (page == 10)
   {  mdouble x, y, u, v; dplane->getPlane(x, y, u, v);
      dplane->setPlane(0.1615, 0, 4.672, 0);
      df->setParameter(-0.4986, 0);
      dplane->traceRay(-1, 0.125, df, y, v, 2);
      dplane->traceRay(-1, 0.25, df, y, v, 2);
      dplane->traceRay(-1, 0.375, df, y, v, 2);
      dplane->traceRay(-1, 0.625, df, y, v, 2);
      dplane->traceRay(-1, 0.75, df, y, v, 2);
      dplane->traceRay(-1, 0.875, df, y, v, 2);
      df->setParameter(x, 0); dplane->setPlane(x, 0, u, 0);
   /*
   dplane->getPoint(x, y);
   mdouble t0, t1, t2, t3; mdouble t[4]; dplane->getPlane(t0, t1, t2, t3);
   t[0] = t0; t[1] = t1; t[2] = t2; t[3] = t3;
   mndlbrot::similar(-1, n + 5, x, y, x, y, t);
   dplane->setPlane(t[0], t[1], t[2], t[3]);
   df->setParameter(x, y); dplane->setPoint(x, y);
   setWindowTitle(QString("%1 +i %2, parameter %3").arg(
   QString::number(t[0])).arg(QString::number(t[2])).arg(QString::number(x)));
   //*/
   }
}

////////////////////////////////////////////////////////////////////////

void QmnDemoLS::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1; n = 0;
if (page == 1)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The current parameter c<sub>0</sub> in the left image is close to a "
 "primitve small Mandelbrot set M<sub>93</sub> of period 93. The right "
 "image shows the filled Julia set K<sub>c</sub> of the quadratic polynomial "
 "f<sub>c</sub>(z) = z<sup>2</sup> + c for c = c<sub>0</sub> , rescaled and "
 "rotated in some way. When zooming in, you will see that the decorations "
 "of the small Mandelbrot set M<sub>93</sub> and of the small Julia set "
 "K<sub>c</sub><sup>93</sup> look similar to each other. The scale "
 "and rotation of the dynamic plane is chosen such that they look almost "
 "identical. The parameter c<sub>0</sub> has been set in a satellite "
 "component, so that the small Julia set will be a \"rabbit.\" The choice "
 "of rescaling, however, is related to the approximately affine "
 "straightening of the primitive M<sub>93</sub> , not to the satellite "
 "component of period 279 at c<sub>0</sub> ."
 "<p>"
 "This kind of local similarity was "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0683.58003\">observed</a> by Heinz-Otto Peitgen. It will be "
 "explained on the following pages."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to zoom in "
 "smoothly by the same factor in both planes simultaneously, down to "
 "the small Mandelbrot set and the small \"rabbit.\"</i>"));
   if (!backwards) { pplane->setNmax(5000); dplane->setNmax(5000); }
   pplane->setPlane(-0.7487811661403, -0.0585140260676, 1.274, 0);
   pplane->setPoint(-0.7487811661403, -0.0585140260676);
   pplane->draw(pf, 1, &mode); go();
}
if (page == 2)
{  text->setHtml(QString(
 "The similarity between the Mandelbrot set and the rescaled Julia set "
 "can happen only in a region around the small Mandelbrot set and the "
 "small Julia set, which is very small compared to the full Mandelbrot set "
 "or the full Julia set, respectively, but which may be large compared to "
 "the small Mandelbrot set and the small Julia set. The similarity will be "
 "lost again very close to these sets, since the decorations are attached "
 "to the boundaries of M<sub>93</sub> and of K<sub>c</sub><sup>93</sup>, "
 "respectively, and these sets are not similar at all."
 "<p>"
 "Recall from Chapter 5 that the iterate f<sub>c</sub><sup>93</sup>(z) "
 "behaves like a quadratic polynomial f<sub>d</sub>(z) in a neighborhood "
 "of K<sub>c</sub><sup>93</sup>. We shall assume that this neighborhood "
 "is relatively large and that the straightening maps &psi;<sub>c</sub>(z) "
 "and h(c) are approximately affine, which happens when the higher powers "
 "in f<sub>c</sub><sup>93</sup>(z) are negligible after an affine rescaling."
 "<p>"
 "In the dynamic plane, the decorations are attached to preimages of the "
 "small &beta;<sub>c</sub><sup>93</sup>, which is a periodic point in the "
 "small Julia set. In the parameter plane, the "
 "corresponding decorations are attached to Misiurewicz points with the "
 "period 93. A parameter c belongs to a decoration, if the critical value "
 "z = c belongs to a decoration of the corresponding small Julia set. The "
 "second assumption is that these decorations move only small in the "
 "fundamental annulus, when the parameter c is varied. The control of the "
 "dynamic decorations in the fundamental annulus is pulled back, and "
 "transfered to the parameter plane."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to see 42 "
 "examples of local similarity, at 14 small Mandelbrot sets with 3 "
 "different zoom levels each.</i><br>"
 "In the main program, choose a parameter c close to a primitive hyperbolic "
 "component and hit the key p to see the rescaled Julia set. You will be "
 "asked to adjust the period. If c is in a satellite component of the "
 "primitive one, the program will suggest the larger period, but you "
 "should enter the period of the primitive component."));
   if (backwards) { pplane->setNmax(5000); dplane->setNmax(5000); }
   go();
}
if (page == 3)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The decorations of the small Mandelbrot set contain smaller small "
 "Mandelbrot sets, and the decorations of the small Julia set contain "
 "smaller Julia sets. These smaller subsets are so small in fact, "
 "that they are not visible on the scale used here, and thus the "
 "decorations in the parameter plane and in the dynamic plane look similar. "
 "The part of the decorations considered here is very small compared to the "
 "whole Julia set, and the smaller Julia sets are mapped to a bigger "
 "Julia set by a high iterate of f<sub>c</sub>(z), which is strongly "
 "expanding. Therefore the smaller Julia sets  are negligible, and "
 "the small branches of the decorations do not change much either with the "
 "parameter."
 "<p>"
 "This intuition can be "
 "<a href=\"http://www.mndynamics.com/indexm.html#locsim\">proved</a> when "
 "the small Mandelbrot set belongs to a geometric sequence at a "
 "Misiurewicz point according to Chapter 6. We have noted already that the "
 "same scaling factor &lambda; appeared on different levels, and it will be "
 "used again here. The renormalization at the small Mandelbrot set is "
 "approximately affine on a large disk, and the decorations are close to "
 "their asymptotic models on the fundamental annulus. Thus both assumptions "
 "from the previous page are satisfied here. The proof will be presented as "
 "a series of coordinate changes on the following pages, illustrated with a "
 "small Mandelbrot set of period 16 at the principal Misiurewicz point of "
 "the 1/3-limb."
 "<p>"
 "<i>Again, hit the Return key or push the Go-button repeatedly to zoom in "
 "smoothly by the same factor in both planes simultaneously, down to "
 "the small Mandelbrot set and the small \"rabbit.\"</i><br>"
 "In the main program, you can do this by using p once and hitting z "
 "repeatedly. Stop rescaling with Home."));
   if (!backwards) { pplane->setNmax(1000); dplane->setNmax(1000); }
   pplane->setPlane(-0.103861, 0.958427, 2.585, 0);
   pplane->setPoint(-0.103858979246, 0.958441423578);
   pplane->draw(pf, 1, &mode); go();
}
if (page == 4)
{  text->setHtml(QString(
 "To control the shape of parameter decorations, we must control dynamic "
 "decorations around the critical value z = c for all parameters c in the "
 "parameter decorations. The left image shows a subset of the "
 "parameter plane, and the right image shows a fixed subset of the dynamic "
 "plane. As the parameter c varies in the left image, you will notice three "
 "kinds of changes in the right image:<ul>"
 "<li>The decorations are moved by a translation. This can be compensated "
 "by the affine approximate renormalization from the quadratic approximation "
 "to the polynomial f<sub>c</sub><sup>16</sup>(z).</li>"
 "<li>The decorations bend close to the small Julia set. This "
 "effect can be compensated by introducing the Boettcher conjugation of the "
 "approximate quadratic polynomial.</li>"
 "<li>The small Julia set itself changes wildly. This does not affect "
 "the decorations any further, because its preimages are negligible.</li>"
 "</ul>"
 "<i>Move the current parameter c with the Arrow keys or by clicking into "
 "the left window. Observe the changes of the Julia set in the right window, "
 "displayed on a fixed region in the complex plane independent of c. The "
 "translation will be most prominent.</i><br>"
 "In the main program, choose the subset of the dynamic plane once and go "
 "back to the parameter plane with the command Shift+F2 instead of F2."));
   if (!backwards)
   {  pplane->setPlane(-0.103861, 0.958427, 0.000195, 0);
      pplane->draw(pf, 1, &mode);
   }
   dplane->setPlane(-0.103861, 0.9584270, 0.000174, 0);
   pplane->setPoint(-0.103858979246, 0.958441423578);
   pplane->setActive(true); goButton->setEnabled(false);
}
if (page == 5)
{  text->setHtml(QString(
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "By the affine coordinate change z &rarr; <nobr class=\"per\">z</nobr> and "
 "c &rarr; <nobr class=\"per\">c</nobr>, the polynomial "
 "f<sub>c</sub><sup>16</sup>(z) of degree 2<sup>16</sup> is approximated by "
 "a quadratic polynomial "
 "<nobr class=\"per\">z</nobr><sup>2</sup> + <nobr class=\"per\">c</nobr>. "
 "In the new coordinates, the small Mandelbrot set is close to M and the "
 "small Julia set is close to K<sub><nobr class=\"per\">c</nobr></sub> . "
 "You shall see that by this affine rescaling, the dynamic decorations are "
 "no longer translated when the parameter changes. The bending of "
 "decorations close to the small Julia set will  still be visible."
 "<p>"
 "<i>Move the current parameter c with the Arrow keys or by clicking into "
 "the left window. Observe the changes of the Julia set in the right window, "
 "which is rescaled according to the current value of c.</i><br>"
 "In the main program, you can get the rescaled dynamic plane with the "
 "key p."));
   pplane->setPoint(-0.103858979246, 0.958441423578);
   pplane->setActive(true); goButton->setEnabled(false);
}
if (page == 6)
{  text->setHtml(QString(
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "The bending of decorations is understood as follows: the inner parts of "
 "the decorations are mapped to the outer parts by a high iterate of the "
 "quadratic-like map, which is approximately "
 "<nobr class=\"per\">z</nobr><sup>2</sup> + <nobr class=\"per\">c</nobr>. "
 "In the exterior of the small Julia set, this map is conjugate to "
 "F(<nobr class=\"per\">z</nobr>) = <nobr class=\"per\">z</nobr><sup>2</sup> "
 "by a map, which is approximately the Boettcher conjugation "
 "&Phi;<sub><nobr class=\"per\">c</nobr></sub>(<nobr class=\"per\">z</nobr>). "
 "The outer parts of the decorations do not change much, and the "
 "Boettcher conjugation is almost the identity there. So the inner parts of "
 "the decorations are mapped by the Boettcher conjugation to a preimage of "
 "the outer decorations under a high iterate of "
 "F(<nobr class=\"per\">z</nobr>), and they depend on the parameter "
 "<nobr class=\"per\">c</nobr> only through the parameter of "
 "&Phi;<sub><nobr class=\"per\">c</nobr></sub>(<nobr class=\"per\">z</nobr>)."
 "<p>"
 "Thus the Boettcher conjugation maps the dynamic decorations to a set "
 "in the exterior of the unit disk, which changes very little when the "
 "parameter <nobr class=\"per\">c</nobr> is varied. The simulation below "
 "illustrates the Boettcher map by the drawing algorithm called "
 "binary decomposition: the curves between the red and green regions are "
 "mapped to straight rays and circles. If the background and the decorations "
 "move together, this will show that the image of the decorations under "
 "&Phi;<sub><nobr class=\"per\">c</nobr></sub>(<nobr class=\"per\">z</nobr>) "
 "will not depend on the parameter."
 "<p>"
 "<i>Move the current parameter c with the Arrow keys or by clicking into "
 "the left window. Observe the changes of the Julia set in the right window, "
 "rescaled according to the value of c and with a moving Boettcher "
 "background. Look out for parts of the decorations that move, and "
 "check if the background moves with them. This may take some "
 "concentration, since the moving background makes the movement of the "
 "decorations less obvious.</i><br>"
 "You cannot get this background in the main program."));
   if (backwards)
   {  pplane->setNmax(1000); dplane->setNmax(1000);
      pplane->setPlane(-0.103861, 0.958427, 0.000195, 0);
      pplane->draw(pf, 1, &mode);
   }
   pplane->setPoint(-0.103858979246, 0.958441423578);
   pplane->setActive(true); goButton->setEnabled(false);
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "After an almost affine straightening, all dynamic decorations are mapped "
 "approximately to the same set in the exterior of the unit disk by the "
 "Boettcher conjugation "
 "&Phi;<sub><nobr class=\"per\">c</nobr></sub>(<nobr class=\"per\">z</nobr>)."
 "Thus the parameter decorations are mapped approximately to the same set by "
 "the Douady <a href=\"javascript:alert('By evaluating the Boettcher "
 "conjugation at the critical value, this conformal map from the "
 "exterior of M to the exterior of the unit disk is obtained.'"
 ")\">map</a> &Phi;<sub>M</sub>(<nobr class=\"per\">c</nobr>). "
 "If we are not too close to the small Mandelbrot set and Julia sets, the "
 "approximations "
 "&Phi;<sub><nobr class=\"per\">c</nobr></sub>(<nobr class=\"per\">z</nobr>)"
 "&asymp; <nobr class=\"per\">z</nobr> and "
 "&Phi;<sub>M</sub>(<nobr class=\"per\">c</nobr>) &asymp; "
 "<nobr class=\"per\">c</nobr> + 1/2 can be used. Returning to the original "
 "coordinates, the factor &lambda; appears from the affine rescaling. Thus "
 "the sets &lambda;(M - c<sub>1</sub>) and (K<sub>c</sub> - c) are close to "
 "each other."
// "hypotheses: large R, affine, outside slowly, intuitively from small/large "
// "and expansion, at Misiurewicz ok from renormalization and "
// "combined asymptotic models. steps of proof, cf local and asymptotic"
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to see the same 42 "
 "examples as on page 2, at 14 small Mandelbrot sets with 3 different "
 "zoom levels each, with the Mandelbrot set (red) drawn on top of the "
 "rescaled Julia set (blue) to compare them.</i><br>"
 "In the main program, hit the key Shift+F8 to see the rescaled Julia set as "
 "a background image in the parameter plane."));
   pplane->setNmax(500); dplane->setNmax(5000);
   pplane->setPlane(-0.75, 0, 1.25, 0); pplane->draw(pf, 1, &mode); go();
}
} //showPage LS

void QmnDemoLS::go()
{  if ( (page == 1 || page == 3) && n)
   {  pplane->draw(pf, 1, &mode, 200); dplane->draw(df, -1, &mode, 200);
      return;
   }
   n &= 1023; n++; uint per = 16; mdouble x, y, t0, t1, t2, t3 = 0, t[4];
   if (page == 1) per = 93;
   if (page == 2 || page == 7)
   {  if (n >= 1 && n <= 3) //nice surrounding spirals
      {  per = 66; x = -0.757983486586; y = 0.07288858531;
         t0 = -0.757983551; t1 = 0.072888523; t2 = 7.00e-5;
         if (n == 2) t2 = 1.84e-5; if (n == 3) t2 = 1.49e-6;
      }
      if (n >= 4 && n <= 6) //in branch of cauliflower
      {  per = 171; x = -0.15427496221783; y = 1.0307527717116;
         t0 = -0.1542749622216; t1 = 1.0307527717096 ; t2 = 1.76e-7;
         if (n == 5) t2 = 1.64e-9; if (n == 6) t2 = 7.38e-11;
      }
      if (n >= 7 && n <= 9) //thick embedded basilica
      {  per = 113; x = -1.250961368574; y = 0.013932721645;
         t0 = -1.2509614245; t1 = 0.0139325610; t2 = 9.41e-5;
         if (n == 8) t2 = 4.43e-6; if (n == 9) t2 = 1.07e-6;
      }
      if (n >= 10 && n <= 12) //thin embedded basilica
      {  per = 44; x = -1.768867928187; y = 0.003926018883;
         t0 = -1.7688679289; t1 = 0.0039260267; t2 = 1.85e-5;
         if (n == 11) t2 = 4.30e-7; if (n == 12) t2 = 7.51e-8;
      }
      if (n >= 13 && n <= 15) //frame in 2/5-limb
      {  per = 25; x = -0.559694691135; y = 0.639147182003;
         t0 = -0.559728568; t1 = 0.639143561; t2 = 0.126;
         if (n == 14) t2 = 2.04e-3; if (n == 15) t2 = 2.96e-4;
      }
      if (n >= 16 && n <= 18) //subframe in 2/5-limb
      {  per = 220; x = -0.55970872757867; y = 0.639112841173122;
         t0 = -0.55970872758493; t1 = 0.63911284117263; t2 = 4.19e-7;
         if (n == 17) t2 = 1.08e-8; if (n == 18) t2 = 1.23e-10;
      }
      if (n >= 19 && n <= 21) //curved frame in 5/11-limb
      {  per = 33; x = -0.708530224385; y = 0.308568064152;
         t0 = -0.708530219785; t1 = 0.308568054307; t2 = 8.09e-5;
         if (n == 20) t2 = 1.11e-6; if (n == 21) t2 = 9.90e-8;
      }
      if (n >= 22 && n <= 24) //straight frame in 5/11-limb
      {  per = 21; x = -0.742930566854283; y = 0.290571198734347;
         t0 = -0.74293056685418; t1 = 0.29057119873412; t2 = 9.60e-7;
         if (n == 23) t2 = 4.87e-10; if (n == 24) t2 = 1.02e-11;
      }
      if (n >= 25 && n <= 27) //cross in 1/15-sublimb of period 2
      {  per = 86; x = -0.767337583476; y = 0.105341112069;
         t0 = -0.767337580973; t1 = 0.105341112782; t2 = 6.25e-4;
         if (n == 26) t2 = 1.35e-5; if (n == 27) t2 = 7.85e-8;
      }
      if (n >= 28 && n <= 30) //spirals at sides, in 1/44-sublimb of period 2
      {  per = 87; x = -0.751832425605; y = 0.036228189173;
         t0 = -0.7518346436; t1 = 0.0362279762; t2 = 1.06e-3;
         if (n == 29) t2 = 3.62e-4; if (n == 30) t2 = 3.31e-5;
      }
      if (n >= 31 && n <= 33) //in 5/8-sublimb of period 3
      {  per = 34; x = -0.186158110159; y = 0.825565575632;
         t0 = -0.186158677795; t1 = 0.825565116881; t2 = 7.81e-4;
         if (n == 32) t2 = 3.86e-5; if (n == 33) t2 = 5.64e-6;
      }
      if (n >= 34 && n <= 36) //thin square in 1/3-sublimb of period 2
      {  per = 23; x = -1.219838652544; y = 0.317328970338;
         t0 = -1.219838652715; t1 = 0.317328970420; t2 = 1.22e-4;
         if (n == 35) t2 = 1.12e-7; if (n == 36) t2 = 5.26e-9;
      }
      if (n >= 37 && n <= 39) //wiggly square in 1/3-sublimb of period 2
      {  per = 33; x = -1.14220985422088; y = 0.30741071167811;
         t0 = -1.1422098542134; t1 = 0.3074107116808; t2 = 3.73e-5;
         if (n == 38) t2 = 1.71e-8; if (n == 39) t2 = 3.52e-10;
      }
      if (n >= 40 && n <= 42) //embedded Cantor in subframe, right branch
      {  per = 47; x = -0.09871024750064; y = 0.957494397584;
         t0 = -0.0987102474608; t1 = 0.9574943977393; t2 = 1.52e-4;
         if (n == 41) t2 = 1.69e-7; if (n == 42) t2 = 5.73e-9;
      }
      pplane->setPoint(x, y); if (n == 42) goButton->setEnabled(false);
      if (page == 2)
      { pplane->setPlane(t0, t1, t2, t3); pplane->draw(pf, 1, &mode); }
      else mode = 20;
   } //page == 2 || == 7
   else { pplane->getPoint(x, y); pplane->getPlane(t0, t1, t2, t3); }
   dplane->setPoint(x, y); df->setParameter(x, y);
   t[0] = t0; t[1] = t1; t[2] = t2; t[3] = t3;
   if (pf->similarity(0, per, x, y, t)) return; //t: dyn rescaled
   if (page == 6)
   {  mode = 7; t0 = t[0]; t1 = t[1]; t2 = t[2]; t3 = t[3];
      if (df->similarity(65002u, per, x, y, t)) return; //t: backgr. dyn.
      df->setParameter(x, y);
   }
   dplane->setPlane(t[0], t[1], t[2], t[3]);
   dplane->draw(df, -1, &mode, 0);
   if (page >= 6) dplane->setPlane(t0, t1, t2, t3); //small dyn. / para.
}//go LS

void QmnDemoLS::drawRight()
{  if (page == 5 || page == 6) go(); //rescale
   if (page != 4) return;
   mdouble x, y; pplane->getPoint(x, y); dplane->setPoint(x, y);
   df->setParameter(x, y); dplane->draw(df, -1, &mode, 0);
}

void QmnDemoLS::dFinished()
{  /*if (page == 5)
   {  mdouble x, y; pplane->getPoint(x, y);
      dplane->backRay(12873LL, 65535LL, x, y, 3);
   }//*/
   if (page == 6 && !(n & 1024)) //small dyn. on background dyn.
   {  n |= 1024; mode = 16; mdouble x, y;
      pplane->getPoint(x, y); df->setParameter(x, y);
      dplane->draw(df, -1, &mode, 2);
   }
   if (page == 7 && !(n & 1024)) //para. on dyn.
   { n |= 1024; mode = 17; dplane->draw(pf, 1, &mode, 2, 204); }
}

////////////////////////////////////////////////////////////////////////

void QmnDemoQC::showPage(bool backwards)
{  pplane->setActive(false); dplane->setActive(false);
   goButton->setEnabled(true); mode = 1; n = 0;
if (page == 1)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "A function f(z) is called holomorphic, analytic, or complex "
 "differentiable, if it is approximated well by affine maps. The "
 "simplest example is given by f(z) = z<sup>2</sup> : by the Binomial "
 "Theorem,<br>"
 "f(z+h) = (z+h)<sup>2</sup> = z<sup>2</sup> + 2zh + h<sup>2</sup> "
 "&asymp; z<sup>2</sup> + 2zh<br>"
 "for small |h|, since |h<sup>2</sup>| is much smaller. In the linear part, "
 "h is multiplied with f'(z) = 2z . In the one-dimensional real case, the "
 "derivative f'(z) is interpreted as the slope of the tangent line. Here it "
 "is a complex number, and the multiplication f'(z)*h means that h is "
 "rescaled by the factor |f'(z)| and rotated by the argument of f'(z). When "
 "a complex function f(z) is differentiable in the real sense, i.e., as a "
 "map in the real plane, the affine part can be written with complex "
 "multiplication as<br>"
 "f(z+h) &asymp; f(z) + &part;f(z)*h + "
 "<nobr class=\"per\">&part;</nobr>f(z)*<nobr class=\"per\">h</nobr> ,<br>"
 "where <nobr class=\"per\">h</nobr> denotes the complex conjugate of h. The "
 "Wirtinger derivatives &part;f(z) and <nobr class=\"per\">&part;</nobr>f(z) "
 "are interpreted as partial derivatives with respect to z and to "
 "<nobr class=\"per\">z</nobr>, respectively. Now f(z) is complex "
 "differentiable, if the linear part is a rotation and rescaling without "
 "shearing, which means <nobr class=\"per\">&part;</nobr>f(z) = 0. Then "
 "f'(z) = &part;f(z)."
 "<p>"
 "A map &psi;(z) between open subsets of the plane is a diffeomorphism, "
 "when both &psi;(z) and the inverse map &psi;<sup>-1</sup>(z) are "
 "differentiable in the real sense (with continuous derivatives). An "
 "orientation-preserving diffeomorphism &psi;(z) is called conformal, if "
 "curves intersecting at right angles are mapped to curves at right angles. "
 "Equivalently, &psi;(z) is complex differentiable (with &psi;'(z) &ne; 0). "
 "In general, a diffeomorphism will change the angles between orthogonal "
 "curves, and the angle may go to 0 at the boundary of the domain. The "
 "notion of a quasiconformal map is less restrictive in one sense, more "
 "restrictive in another sense: &psi;(z) may be differentiable in a weak "
 "sense only, such that the derivative may exist only almost everywhere and "
 "need not be continuous. But the shearing or distortion must be bounded "
 "globally. This condition can be expressed as "
 "|<nobr class=\"per\">&part;</nobr>&psi;(z)| &le; m|&part;&psi;(z)| with "
 "<nobr>m &lt; 1.</nobr>"
 "<p>"
 "A holomorphic motion of a set S means that the points z of S move in "
 "the complex plane without colliding, as a parameter c varies in a simply "
 "connected domain U, and that for each z the motion depends holomorphically "
 "on c. More precisely, it is a family of maps "
 "&psi;<sub>c</sub>(z) : S &rarr; <i>C</i>, injective in z and holomorphic "
 "in c, which is the identity for c = c<sub>0</sub> . These conditions are "
 "strong enough to determine that &psi;<sub>c</sub>(z) must be "
 "quasiconformal. By a "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0741.32009\">theorem</a> of Zbigniew S&#x0142;odkowski, "
 "&psi;<sub>c</sub>(z) extends from the initial set S to a holomorphic "
 "motion of the whole plane. By the "
 "<a href=\"http://archive.numdam.org/article/"
 "ASENS_1983_4_16_2_193_0.pdf\">work</a> of Ma&ntilde;&eacute;, Sad, "
 "and Sullivan, the Julia set &part;K<sub>c</sub> moves holomorphically "
 "unless there is a bifurcation, i.e, the parameter c belongs to the "
 "boundary of the Mandelbrot set M."
 "<p>"
 "A simple example of a holomorphic motion is given by the linear map "
 "&psi;<sub>c</sub>(z) = z + c<nobr class=\"per\">z</nobr>, where the "
 "parameter c belongs to the unit disk U in the left image. The right "
 "image shows where some orthogonal lines are mapped.<br>"
 "<i>Note both the dependence of individual nodes on the parameter c, and "
 "the increased distortion for |c| &rarr; 1: change the parameter c by "
 "clicking into the left window, or move it with the Arrow keys.</i>"));
  if (!backwards)
   {  pplane->setNmax(500); dplane->setNmax(500);
      dplane->setPlane(0, 0, 2.2, 0); pplane->setPlane(0, 0, 1.2, 0);
      pplane->draw(pf, 0, &mode);
      pplane->drawEllipse(0, 0, 1.0, 1.0, Qt::red);
   }
   pplane->setPoint(0, 0); pplane->setActive(true);
   goButton->setEnabled(false);
}
if (page == 2)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "<style type=\"text/css\"> *.per { text-decoration: overline } </style>"
 "The linear approximation of a conformal map sends a circle to a "
 "circle again. The linear approximation of a quasiconformal map "
 "&psi;(z), which exists almost everywhere, maps some ellipse to a "
 "circle. The direction and the length of the axes varies with z, but the "
 "ratio of the axes is bounded independently of z. The Beltrami coefficient "
 "<nobr>&mu;(z) = "
 "<nobr class=\"per\">&part;&psi;</nobr>(z) / &part;&psi;(z)</nobr> "
 "describes the field of ellipses (up to scaling): its argument is twice "
 "that of the minor axis, and its modulus "
 "|&mu;(z)| &le; m &lt; 1 is related to the distortion ratio."
 "<p>"
 "Conversely, suppose that an ellipse field is defined by a "
 "Lebesgue-measurable function &mu;(z) with "
 "|&mu;(z)| &le; m &lt; 1, vanishing at &infin;. Then there is "
 "a unique quasiconformal map &psi;(z) with &psi;(z) &asymp; z as "
 "z &rarr; &infin;, satisfying the partial differential equation of Beltrami "
 "<nobr class=\"per\">&part;</nobr>&psi;(z) = &mu;(z)*&part;&psi;(z). It has "
 "the Beltrami coefficient &mu;(z), thus &psi;(z) maps the given "
 "ellipses to circles. By the Mourrey-Ahlfors-Bers Theorem, &psi;(z) depends "
 "continuously on &mu;(z)."
 "<p>"
 "This is the main tool of quasiconformal surgery: suppose a map g(z) "
 "is constructed piecewise to obtain interesting dynamics. It equals "
 "different holomorphic maps f<sub>k</sub>(z) in different parts of "
 "the plane. In sectors or annuli between these parts, it is differentiable "
 "in the real sense only, since complex differentiability rules out a "
 "piecewise definition. But when an ellipse field &mu;(z) can be "
 "constructed, such that it is invariant under the linear approximation of "
 "g(z), then solve the Beltrami equation to obtain a quasiconformal map "
 "&psi;(z) sending the ellipses &mu;(z) to circles. Now the composite "
 "map &psi;(g(&psi;<sup>-1</sup>(z))) sends circles to circles, in "
 "the linear approximation, thus it is holomorphic, e.g, a polynomial. It "
 "has the desired dynamics, since it is conjugate to g(z). Famous "
 "applications include the theorem by Dennis Sullivan, that every "
 "<a href=\"javascript:alert('The Fatou set is the complement of the "
 "Julia set. Here the dynamics is stable.')\">Fatou</a> "
 "component of a rational function is (pre-)periodic, and the construction "
 "of a Herman ring from a Siegel disk by Mitsuhiro Shishikura. In the "
 "context of the Mandelbrot set, quasiconformal surgery shows that the "
 "multiplier map of a hyperbolic component is injective, and it is used to "
 "construct conjugations &psi;<sub>c</sub>(z) and homeomorphisms h(c) in, "
 "e.g., the Straightening Theorem."
 "<p>"
 "Again, consider the family of linear quasiconformal maps "
 "&psi;<sub>c</sub>(z) = z + c<nobr class=\"per\">z</nobr>. The Beltrami "
 "coefficient &mu;<sub>c</sub>(z) = c is independent of z. In the right "
 "image, the field of ellipses in the tangent space is represented by "
 "a single ellipse, which is the preimage of the unit circle.<br>"
 "<i>Ignore its size, but watch the direction and the ratio of the axes: "
 "change the parameter c by clicking into the left window, or move it "
 "with the Arrow keys.</i>"));
   if (backwards)
   {  dplane->setPlane(0, 0, 2.2, 0); pplane->setPlane(0, 0, 1.2, 0);
      pplane->draw(pf, 0, &mode);
      pplane->drawEllipse(0, 0, 1.0, 1.0, Qt::red);
   }
   pplane->setPoint(0, 0); pplane->setActive(true);
   goButton->setEnabled(false);
}
if (page == 3)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Recall the example of renormalization from Chapter 5: the left image shows "
 "a small Mandelbrot set M<sub>4</sub> of period 4, and the right image "
 "shows a small Julia set K<sub>c</sub><sup>4</sup>. The cyan and red "
 "curves are equipotential lines (vertical) and external rays (horizontal). "
 "The domain U<sub>c</sub>' is bounded by parts of the cyan curves, and "
 "U<sub>c</sub> is bounded by parts of the red curves. The restriction "
 "g<sub>c</sub>(z) of f<sub>c</sub><sup>4</sup>(z) is a 2:1 map from "
 "U<sub>c</sub>' to U<sub>c</sub> . Adrien Douady and John Hamal Hubbard "
 "have <a href=\"http://www.math.cornell.edu/~hubbard/PolyLikeMaps.pdf\">"
 "constructed</a> a conjugation &psi;<sub>c</sub>(z) from the "
 "quadratic-like map g<sub>c</sub>(z) to a quadratic polynomial "
 "f<sub>d</sub>(z) = z<sup>2</sup> + d."
 "<p>"
 "The first step is to choose a quasiconformal map &xi;<sub>c</sub>(z) "
 "from the fundamental annulus U<sub>c</sub> \\ U<sub>c</sub>' to a round "
 "annulus with inner radius R &gt; 1 and outer radius R<sup>2</sup>, "
 "conjugating g<sub>c</sub>(z) to F(z) = z<sup>2</sup> on the inner "
 "boundary. Extend this map to the inner component and define a 2:1 "
 "map j<sub>c</sub>(z) of the plane by F(z) for |z| &ge; R and by "
 "&xi;<sub>c</sub>(g<sub>c</sub>(&xi;<sub>c</sub><sup>-1</sup>(z))) for "
 "|z| &lt; R. An invariant ellipse field &nu;<sub>c</sub>(z) is constructed "
 "piecewise by a pullback with j<sub>c</sub>(z). Let &zeta;<sub>c</sub>(z) "
 "solve the corresponding Beltrami equation, and define "
 "&psi;<sub>c</sub>(z) = &zeta;<sub>c</sub>(&xi;<sub>c</sub>(z)). Then "
 "&zeta;<sub>c</sub>(z) conjugates j<sub>c</sub>(z) to an analytic 2:1 "
 "map, which is a quadratic plynomial of the form z<sup>2</sup> + d, "
 "and &psi;<sub>c</sub>(z) conjugates g<sub>c</sub>(z) to the same "
 "polynomial."
 "<p>"
 "We have &zeta;<sub>c</sub>(z) =  &Phi;<sub>d</sub><sup>-1</sup>(z) for "
 "|z| &gt; R and thus &psi;<sub>c</sub>(z) = "
 "&Phi;<sub>d</sub><sup>-1</sup>(&xi;<sub>c</sub>(z)) in the fundamental "
 "annulus. A pullback argument shows that &psi;<sub>c</sub>(z) and d depend "
 "only on the choice of &xi;<sub>c</sub>(z) in the annulus. When the "
 "critical value c of g<sub>c</sub>(z) is in the fundamental annulus, then "
 "d = &psi;<sub>c</sub>(c) satisfies "
 "&Phi;<sub>M</sub>(d) = &xi;<sub>c</sub>(c)."
 "<p>"
 "<i>Choose an initial value for z by clicking into the right window or by "
 "moving the cross with the Arrow keys. Hit the Return key or push the "
 "Go-button to map z with the quadratic-like map "
 "g<sub>c</sub>(z) = f<sub>c</sub><sup>4</sup>(z). Check that the cyan "
 "boundary of U<sub>c</sub>' is mapped to the red boundary of "
 "U<sub>c</sub> .</i>"));
   if (!backwards)
   {  pplane->setPlane(-0.1592, 1.0345, 0.037, 0);
      dplane->setPlane(-0.156520, 1.0322471, 0.0414, 0);
      pplane->draw(pf, 1, &mode);
   }
   pplane->setPoint(-0.153940779149429, 1.03770019646018);
   dplane->setActive(true);
}
if (page == 4)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Two more details shall be explained. The conjugation &psi;<sub>c</sub>(z) "
 "maps the small Julia set K<sub>c</sub><sup>4</sup> to the filled "
 "Julia set K<sub>d</sub> of f<sub>d</sub>(z), and the map h(c) = d in "
 "parameter space maps the small Mandelbrot set M<sub>4</sub> to the "
 "usual Mandelbrot set M. The ellipse field &nu;<sub>c</sub>(z) should be "
 "constructed such that &psi;<sub>c</sub>(z) is a hybrid-equivalence, i.e., "
 "conformal almost everywhere on K<sub>c</sub><sup>4</sup>. If c "
 "belongs to M<sub>4</sub> , or K<sub>c</sub><sup>4</sup> is connected, "
 "then d will be determined uniquely from g<sub>c</sub>(z). For parameters "
 "in the exterior of M<sub>4</sub> , the map d = h(c) depends on the "
 "choice of &xi;<sub>c</sub>(z) in the fundamental annulus."
 "<p>"
 "The rational dynamic rays and equipotential lines used to define the "
 "fundamental annulus move holomorphically with the parameter c. "
 "Extending this holomorphic motion, &xi;<sub>c</sub>(z) can be chosen such "
 "that its inverse depends holomorphically on c, and then h(c) will be "
 "quasiconformal in the exterior of M<sub>4</sub> . Mikhail Lyubich has "
 "<a href=\"http://arxiv.org/abs/math/9903201\">shown</a> that it is "
 "quasiconformal everywhere."
 "<p>"
 "<i>To change the parameter c, click into the left window, or move the "
 "cross with the Arrow keys. Watch the small Julia set and the fundamental "
 "annulus for parameters in M<sub>4</sub> , on its decorations "
 "M \\ M<sub>4</sub> , and in the exterior of M. When U<sub>c</sub>' "
 "becomes disconnected, g<sub>c</sub>(z) will no longer be "
 "quadratic-like.</i>"));
   if (backwards)
   {  dplane->stop(); pplane->stop(); delete dg; delete pg; pg = 0; dg = 0;
      pplane->setPlane(-0.1592, 1.0345, 0.037, 0);
      dplane->setPlane(-0.156520, 1.0322471, 0.0414, 0);
      pplane->draw(pf, 1, &mode);
      pplane->setPoint(-0.153940779149429, 1.03770019646018);
   }
   else dplane->setPoint(-0.157477872041, 1.034107764500);
   pplane->setActive(true);
}
if (page == 5)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Bodil Branner and Adrien Douady have "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:0668.58026\">constructed</a> a homeomorphism between the 1/2-limb "
 "and a subset of the 1/3-limb of the Mandelbrot set. In the images below, "
 "the parameter c is of period 6 in the 1/3-limb. The right image shows a "
 "subset of the filled Julia set K<sub>c</sub> of the quadratic polynomial "
 "f<sub>c</sub>(z) and the dynamic rays landing at the fixed point "
 "&alpha;<sub>c</sub> . The map g<sub>c</sub>(z) is defined piecewise "
 "such that it coincides with f<sub>c</sub> below &alpha;<sub>c</sub> . "
 "In the sector between the blue and cyan rays, g<sub>c</sub>(z) is the "
 "second iterate of f<sub>c</sub>(z), and the sector between the cyan and "
 "green rays is mapped quasiconformally to a narrow sector around the blue "
 "ray. In that sector, g<sub>c</sub>(z) is defined such that the second "
 "iterate of g<sub>c</sub>(z) equals the third iterate of "
 "f<sub>c</sub>(z). Therefore points z in this sector escape, and "
 "the distortion of g<sub>c</sub>(z) cannot grow indefinitely by orbits "
 "passing many times through the sectors. On a suitable domain, "
 "g<sub>c</sub>(z) will be a <a href=\"javascript:alert('A quasireguar "
 "map is like a quasiconformal map regarding its differentiability "
 "properties and the bounded distortion, but it need not be injective.'"
 ")\">quasiregular</a> quadratic-like map."
 "<p>"
 "The filled Julia set of g<sub>c</sub>(z) is a subset of the filled Julia "
 "set of f<sub>c</sub>(z) : the arm between the cyan and green rays consists "
 "of escaping points now, its preimage below is missing, as are the "
 "infinitely many preimages of that arm. The Straightening Theorem gives a "
 "hybrid-equivalence to a unique quadratic polynomial f<sub>d</sub>(z), and "
 "the new parameter d is of period 4 in the 1/2-limb of M. A map in "
 "parameter space is given by h(c) = d. The domain of this homeomorphism "
 "is not all of the 1/3-limb: infinitely many arms are cut off, because the "
 "critical orbit would escape under the iteration of g<sub>c</sub>(z). The "
 "left image shows this subset of the 1/3-limb. The inverse map is "
 "described on the following page."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to map z with "
 "g<sub>c</sub>(z), note that it is 4-periodic instead of 6-periodic. "
 "You can change the parameter c by clicking into the left window, or move "
 "it with the Arrow keys.</i><br>"
 "In the main program, this simulation is available as the family 8.3."
 "(The simulation iterates a piecewise defined map, which is "
 "discontinuous on certain lines. You will see some artefacts in the "
 "algorithms 0, 1, 2 of drawing.)"));
   dplane->stop(); pplane->stop(); delete dg; delete pg;
   pg = new mndsurge(3); dg = new mndsurge(3);
   mode = 3; dplane->setPlane(0, 0, 1.4506, 0);
   pplane->setPlane(-.113, .888, .343, 0);
   pplane->setPoint(-0.1134186559, 0.8605694725);
   pplane->draw(pg, 1, &mode); pplane->setActive(true);
}
if (page == 6)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "For the inverse construction, consider a parameter c of period 4 in the "
 "1/2-limb and the dynamic rays landing at the repelling fixed point "
 "&alpha;<sub>c</sub> of the quadratic polynomial f<sub>c</sub>(z). We shall "
 "need two narrow sectors around these rays, and the big sector between "
 "these rays containing the critcal value c. The new map "
 "g<sub>c</sub>(z) will be a conformal map &phi;<sub>c</sub>(z) from "
 "the big sector to the sector around the green ray, and in that sector "
 "it is given by g<sub>c</sub>(z) = "
 "f<sub>c</sub>(&phi;<sub>c</sub><sup>-1</sup>(z)). It equals "
 "f<sub>c</sub>(z) on the right-hand side of &alpha;<sub>c</sub> , and on "
 "the three intermediate sectors it is defined such that the third iterate "
 "of g<sub>c</sub>(z) equals the second iterate of f<sub>c</sub>(z) around "
 "the cyan ray. (Actually, this construction works only in sublimbs of the "
 "period-2 component, because the critical value sector must have the same "
 "<a href=\"http://www.zentralblatt-math.org/zmath/en/advanced/?"
 "q=an:1121.37314\">opening modulus</a> as the lower one.)"
 "<p>"
 "The filled Julia set of g<sub>c</sub>(z) has grown additional arms around "
 "the green ray and its preimages. The quasiregular quadratic-like map "
 "g<sub>c</sub>(z) is hybrid-equivalent to a unique "
 "quadratic polynomial f<sub>d</sub>(z), where d is of period 6 in the "
 "1/3-limb. The map in parameter space is inverse to that from the "
 "previous page, it is an embedding of the 1/2-limb into the 1/3-limb. In "
 "the left image, the simulation shows additional arms at the 1/2-limb. "
 "These would be mapped to the arms cut off from the 1/3-limb before (if "
 "the contruction of g<sub>c</sub>(z) was extended to this case)."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to map z with "
 "g<sub>c</sub>(z), note that it is 6-periodic instead of 4-periodic. "
 "You can change c by clicking into the left window, or move it with the "
 "Arrow keys.</i><br>"
 "In the main program, this simulation is available as the family 8.1."));
   dplane->stop(); pplane->stop(); delete dg; delete pg;
   pg = new mndsurge(1); dg = new mndsurge(1);
   mode = 3; dplane->setPlane(0, 0, 2.0, 0);
   pplane->setPlane(-1.4, 0, 0.7, 0);
   pplane->setPoint(-1.31070264, 0);
   pplane->draw(pg, 1, &mode); pplane->setActive(true);
}
if (page == 7)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "Bodil Branner and Nuria Fagella constructed homeomorphisms between "
 "limbs of the Mandelbrot set of equal denominators. The "
 "<a href=\"http://arXiv.org/abs/math/9505207\">first</a> construction "
 "employed a family of higher-degree polynomials, which is available in "
 "the main program as family 2. Suggested by Dierk Schleicher, their "
 "<a href=\"http://www.maia.ub.es/~fagella/Papers/0002fag_ext.pdf\">"
 "second</a> approach stayed within the quadratic family. Consider a "
 "parameter c of primitive period 4 in the 1/3-limb. The right image shows "
 "the filled Julia set and the dynamic rays landing at "
 "&plusmn;&alpha;<sub>c</sub> . The polynomial f<sub>c</sub>(z) has the "
 "rotation number 1/3 at the fixed point &alpha;<sub>c</sub> . We shall "
 "construct a map g<sub>c</sub>(z) with the rotation number 2/3: it "
 "equals the second iterate of f<sub>c</sub>(z) in the middle strip and in "
 "the top right sector. In the top left sector, it is given by a suitable "
 "branch of the inverse map f<sub>c</sub><sup>-1</sup>(z). This map "
 "would be discontinuous on the cyan and red rays, so it is modified in "
 "narrow sectors around these rays, such that it is quasiconformal there "
 "and the third iterate of g<sub>c</sub>(z) equals the third iterate of "
 "f<sub>c</sub>(z) around the red ray. On the lower sectors, "
 "g<sub>c</sub>(z) = g<sub>c</sub>(-z) is defined as an even map."
 "<p>"
 "Note that the filled Julia set of g<sub>c</sub>(z) is the same as that "
 "of f<sub>c</sub>(z). Again, the quasiregular quadratic-like map "
 "g<sub>c</sub>(z) is hybrid-equivalent to a unique quadratic polynomial "
 "f<sub>d</sub>(z). The new parameter d belongs to the 2/3-limb, and it "
 "has the period 5. The map h(c) = d is a homeomorphism from the "
 "1/3-limb onto the 2/3-limb of M. In contrast to the previous examples, "
 "no arms were cut off or addded in the dynamics or in the parameter plane. "
 "Thus the Julia sets K<sub>c</sub> and K<sub>d</sub> are mutually "
 "homeomorphic by the hybrid-equivalence, and the homeomorphism h(c) "
 "extends to a neighborhood of the limb. It is quasiconformal at least in "
 "the exterior. Composing h(c) with complex conjugation gives an "
 "orientation-reversing homeomorphism of the 1/3-limb onto itself."
 "<p>"
 "<i>Hit the Return key or push the Go-button repeatedly to map z with "
 "g<sub>c</sub>(z), note that it is 5-periodic instead of 4-periodic, and "
 "that the rotation number at &alpha;<sub>c</sub> is 2/3. You can change "
 "the parameter c by clicking into the left window, or move it with the "
 "Arrow keys.</i><br>"
 "This simulation is not available in the main program."));
   if (!backwards)
   { dplane->stop(); pplane->stop(); delete dg; delete pg; pg = 0; dg = 0; }
   dplane->setPlane(0, 0, 1.45, 0);
   pplane->setPlane(-.113, .888, .343, 0);
   pplane->draw(pf, 1, &mode); pplane->setActive(true);
   pplane->setPoint(-0.1565202, 1.0322471);
}
if (page == 8)
{  text->setHtml(QString(
 "<style type=\"text/css\">a:link { text-decoration: none; }</style>"
 "The final example was announced on page 5 of Chapter 6. Consider the "
 "parameter edge of order 4 in the 1/3-limb (see page 11 of Chapter 3). The "
 "homeomorphism h(c) will map this edge onto itself, fixing the "
 "endpoints. Therefore it has an infinite family of fundametal domains: the "
 "strips bounded by parameter rays in the left image mark subsets of M, each "
 "of which is mapped to the next one by h(c). All of them are mutually "
 "homeomorphic."
 "<p>"
 "The "
 "<a href=\"http://www.mndynamics.com/indexm.html#surge\">construction</a> "
 "of g<sub>c</sub>(z) from the quadratic polynomial "
 "f<sub>c</sub>(z) shall be explained in a few steps. The right image shows "
 "the central edge between the fixed point &alpha;<sub>c</sub> and its "
 "preimage -&alpha;<sub>c</sub> . The cyan rays land at a 3-periodic "
 "point, and the blue rays mark a preimage. The critical point z = 0 is in "
 "the middle strip. The strip between the red and blue rays is mapped to the "
 "strip between the red and cyan rays by the third iterate of "
 "f<sub>c</sub>(z), fixing &alpha;<sub>c</sub> . The strip between the "
 "magenta and cyan rays is mapped to that strip as well. So the map "
 "z &rarr; -f<sub>c</sub><sup>3</sup>(z) fixes -&alpha;<sub>c</sub> and "
 "sends the cyan rays to the blue ones. Its inverse is pasted together "
 "with the first map to yield a map j<sub>c</sub>(z) from the "
 "central edge to itself. It expands the strip between the red and blue "
 "rays to the strip between the red and cyan rays, and contracts the strip "
 "between the blue and magenta rays to the strip between the cyan and "
 "magenta rays."
 "<p>"
 "Now g<sub>c</sub>(z) shall equal f<sub>c</sub>(z) except on the dynamic "
 "edge of order 4, which contains the critical value z = c. That edge is "
 "mapped to the central edge by the third iterate of f<sub>c</sub>(z), "
 "stretched / contracted by j<sub>c</sub>(z), and mapped back by an inverse "
 "of the second iterate of f<sub>c</sub>(z). This g<sub>c</sub>(z) is a 2:1 "
 "map, but discontinuous on six dynamic rays. By a quasiconformal "
 "interpolation in sectors around these rays, it becomes a quasiregular "
 "quadratic-like map on a suitable domain."
 "<p>"
 "<i>To verify the claims from the second paragraph, choose z by clicking "
 "into the right window, or move the cross with the Arrow keys. Hit the "
 "Return key or push the Go-button to map it with f<sub>c</sub>(z).</i>"));
   if (!backwards) pplane->setPlane(-0.14216, 1.02128, 0.0726, 0);
   dplane->setPlane(0, 0, 1.05, 0); mode = 16; //0.7 to alpha
   pplane->setPoint(-0.156520166834, 1.032247108923);
   pplane->draw(pf, 1, &mode); dplane->setActive(true);
}
if (page == 9)
{  text->setHtml(QString(
 "By the Straightening Theorem, the quasiregular quadratic-like map "
 "g<sub>c</sub>(z) constructed on the previous page is hybrid-equivalent to "
 "a unique quadratic polynomial f<sub>d</sub>(z), and the map h(c) = d "
 "is a homeomorphism in parameter space. In the left image, the blue rays "
 "land at a root of period 4, and the magenta and red rays "
 "land at two roots of period 7. The right image shows the common filled "
 "Julia set K<sub>c</sub> of f<sub>c</sub>(z) and g<sub>c</sub>(z), and the "
 "six rays mark the strips where g<sub>c</sub>(z) is defined "
 "piecewise. The lower narrow strip is expanded and the upper wide strip is "
 "contracted by g<sub>c</sub>(z)."
 "<p>"
 "When the parameter c is the lower center of period 7, the critical orbit "
 "will have period 4. For the center c of period 4, the critical orbit under "
 "g<sub>c</sub>(z) is of period 7, and its combinatrics correspond to the "
 "upper center of period 7. The critical orbit of the latter is 10-periodic "
 "under g<sub>c</sub>(z) now. So h(c) is map the lower period 7 to "
 "period 4, then to the upper period 7, and to a period 10."
 "<p>"
 "<i>Click into the left image or move the cross with the Arrow keys to set "
 "the parameter c into a hyperbolic component of period 7, 4, 7. Map it with "
 "g<sub>c</sub>(z) by hitting the Return key or pushing the Go-button "
 "repeatedly. Verify that the period is 4, 7, 10 now.</i>"));
   if (backwards)
   {  points = 0; delete[] xp; delete[] yp; xp = 0; yp = 0;
      pplane->setPlane(-0.14216, 1.02128, 0.0726, 0);
   }
   dplane->setPlane(-0.12222, 0, 1.21, 0); mode = 16;
   pplane->setPoint(-0.12749997355, 0.98746090949);
   pplane->draw(pf, 1, &mode); pplane->setActive(true);
}
if (page == 10)
{  text->setHtml(QString(
 "The homeomorphism h(c) = d is quasiconformal in the exterior of the "
 "Mandelbrot set. The hybrid-equivalence from g<sub>c</sub>(z) to "
 "f<sub>d</sub>(z) maps the filled Julia sets "
 "K<sub>c</sub> &rarr; K<sub>d</sub> . The images below show two mutually "
 "homeomorphic subsets of M, which contain small Mandelbrot sets of "
 "periods 7 and 4, respectively. Afterwards you will see mutually "
 "homeomorphic parts of the filled Julia sets K<sub>c</sub> and "
 "K<sub>d</sub> around the critical values, with c of period 7*3 and d of "
 "period 4*3."
 "<p>"
 "As the parameter c moves from the lower center of period 7 to the "
 "center of period 4, the filled Julia set undergoes an infinite number of "
 "bifurcations. These combine such that the final Julia set is homeomorphic "
 "to the first one again. In the last animation, you will see the central "
 "part of the Julia sets, and the dynamic rays are (pre-)images of the "
 "dynamic rays landing at the edge of order 4."
 "<p>"
 "<i>Hit the Return key or push the Go-button to see parts of two "
 "homeomorphic Julia sets of periods 7 and 4, and to move the parameter c "
 "while observing the central part of K<sub>c</sub> .</i>"));
   pplane->setPlane(-0.1283077, 0.9877817, 0.0163366, 0);
   pplane->setPoint(-0.12720686, 0.98887027); mode = 16; n = -2;
   pf->setParameter(-0.12720686, 0.98887027); pplane->draw(pf, 1, &mode);
   dplane->setPlane(-0.1594566, 1.0343453, 0.0376579, 0);
   dplane->setPoint(-0.153940779, 1.037700196);
   df->setParameter(-0.153940779, 1.037700196); dplane->draw(df, 1, &mode);
   steps = 3; points = 19; xp = new mdouble[points]; yp = new mdouble[points];
   xp[18] = -0.127499973546363; yp[18] = 0.987460909489457;//7
   xp[17] = -0.132073186946809; yp[17] = 0.98997456087101;//7m
   xp[16] = -0.134583625703313; yp[16] = 0.993328741444919;//7m
   xp[15] = -0.134080545068242; yp[15] = 0.997203455824895;//13m
   xp[14] = -0.135191252755202; yp[14] = 0.999380418818262;//13m
   xp[13] = -0.134735329223102; yp[13] = 1.00230906593948;//10m
   xp[12] = -0.136421467222428; yp[12] = 1.00487952918666;//10
   xp[11] = -0.138051926065576; yp[11] = 1.00759684040941;//10m
   xp[10] = -0.137435834452226; yp[10] = 1.01013819947559;//13m
   xp[9] = -0.138172535258404; yp[9] = 1.01223218563766;//13m
   xp[8] = -0.137775189806989; yp[8] = 1.01477295349041;//4m
   xp[7] = -0.139784662970739; yp[7] = 1.01629567466499;//14
   xp[6] = -0.141251983404437; yp[6] = 1.01805107856856;//m
   xp[5] = -0.145432812114773; yp[5] = 1.02010845629261;//m
   xp[4] = -0.147076564687346; yp[4] = 1.02485009713839;//4m
   xp[3] = -0.150452379784434; yp[3] = 1.02787699514147;//4m
   xp[2] = -0.152836598245754; yp[2] = 1.02975804927887;//4m
   xp[1] = -0.154724581342867; yp[1] = 1.03104721517436;//4~r
   xp[0] = -0.156520166833755; yp[0] = 1.03224710892283;//4
}
} //showPage QC

void QmnDemoQC::go()
{  if (page == 10)
   {  if (n == -2)
      {  n = -1; pplane->setPlane(-0.127821984, 0.987771360, 0.013984,0);
         pplane->draw(pf, -1, &mode);
         dplane->setPlane(-0.15747787, 1.03410776, 0.0267456, 0);
         dplane->draw(df, -1, &mode); return;
      }
      if (n == -1)
      {  n = 0; pplane->setPlane(-0.14216, 1.02128, 0.0726, 0);
         dplane->setPlane(0, 0, 0.367, 0);
         pplane->setPoint(xp[18], yp[18]); //per 7
         pplane->draw(pf, 1, &mode); return;
      }
      polyline(); return;
   }
   mdouble x, y; dplane->getPoint(x, y); if (x*x + y*y > 100) return;
   if (page <= 4) for (int i = 1; i <= 4; i++) df->iterate(x, y); // >= 3
   else if (page <= 6) dg->iterate(x, y);
   else if (page == 7) //g BF
   {  mdouble x1, y1; pplane->getPoint(x1, y1); //alpha:
      mndynamics::root(0.25 - x1, -y1, x1, y1); x1 = 0.5 - x1; y1 = -y1;
      if (x > y) { x = -x; y = -y; }
      if (x - y < x1 - y1 && 2*x + y < 2*x1 + y1) df->iterate(x, y, -1);
      else { df->iterate(x, y); df->iterate(x, y); }
   }
   else if (page == 8) df->iterate(x, y);
   else //g edge
   {  int j; mdouble x1 = -0.138, y1 = 0.959, x2 = -0.147, y2 = 0.999,
         x3 = -0.157, y3 = 1.081;
      df->find(-1, 3, 1, x1, y1); df->find(-1, 4, 3, x2, y2);
      df->find(-1, 4, 1, x3, y3);
      if (x + y < x1 + y1 || x - y > x1 - y1 || y > y3) df->iterate(x, y);
      else if (y < y2)
      {  for (j = 0; j < 6; j++) df->iterate(x, y);
         df->iterate(x, y, -2); df->iterate(x, y, -1);
      }
      else
      {  for (j = 0; j < 3; j++) df->iterate(x, y);
         x = -x; y = -y; df->iterate(x, y, -1);
         df->iterate(x, y, -1); df->iterate(x, y, -2);
         df->iterate(x, y, -2); df->iterate(x, y, -1);
      }
   }
   dplane->setPoint(x, y);
} //go QC

void QmnDemoQC::drawRight()
{  if (page == 10 && n < 0) return;
   mdouble x, y; pplane->getPoint(x, y); df->setParameter(x, y);
   if (page == 1)
   {  dplane->draw(df, 0, &mode); int j; mdouble c = 1.0 - x*x - y*y;
      if (x < -1.001 || x > -0.999) for (j = 0; j <= 100; j++)
      {  dplane->drawLine(-2.2, (-2.2*y + j*c)/(1.0 + x),
                          2.2, (2.2*y + j*c)/(1.0 + x));
         dplane->drawLine(-2.2, (-2.2*y - j*c)/(1.0 + x),
            2.2, (2.2*y - j*c)/(1.0 + x));
      }
      if (x > 1.001 || x < 0.999) for (j = 0; j <= 100; j++)
      {  dplane->drawLine((-2.2*y + j*c)/(1.0 - x), -2.2,
                          (2.2*y + j*c)/(1.0 - x), 2.2);
         dplane->drawLine((-2.2*y - j*c)/(1.0 - x), -2.2,
                          (2.2*y - j*c)/(1.0 - x), 2.2);
      }
   }
   if (page == 2)
   {  dplane->draw(df, 0, &mode);
      mdouble t, u, x1 = x*x, y1 = y*y, x2, y2, R = 1 - x1 - y1;
      if (R > -0.001 && R < 0.001) return; R = 1/R;
      for (t = 0; t <= 6.30; t += 0.02)
      {  x2 = cos(t); y2 = sin(t);
         u = ((1 - x)*x2 - y*y2)*R; y2 = ((1 + x)*y2 - y*x2)*R; x2 = u;
         if (t > 0) dplane->drawLine(x1, y1, x2, y2);
         x1 = x2; y1 = y2;
      }
   }
   if (page == 5 || page == 6)
   { dg->setParameter(x, y); dplane->draw(dg, -1, &mode); }
   x = 0; y = 0;
   if (page == 3 || page == 4)
   {  df->iterate(x, y, -2); df->iterate(x, y, -1); df->iterate(x, y, -1);
      n &= 3;
   }
   dplane->setPoint(x, y);
   if (page == 4 || page == 10) dplane->draw(df,-1, &mode, 1);
   else if (page == 3 || page >= 7) dplane->draw(df, -1, &mode);
} //drawRight QC

void QmnDemoQC::pFinished()
{  if (page <= 4 && !(n & 2) )
   { n |= 2; uint n1 = 1028; pplane->draw(pf, 1, &n1, 2); }
   if (page == 8)
   {  mdouble x, y, u, v, w;
      for (w = 0; w <= 6; w += 1.0)
      {  u = 11.0/56.0 + pow(0.125, w)/1008.0;
         v = 15.0/56.0 - pow(0.125, w)/1008.0;
         pplane->traceRay(1, u, pf, x, y, 4);
         pplane->traceRay(1, v, pf, x, y, 4);
         u = 23.0/112.0 - pow(0.125, w)/1008.0;
         v = 29.0/112.0 + pow(0.125, w)/1008.0;
         pplane->traceRay(1, u, pf, x, y, 4);
         pplane->traceRay(1, v, pf, x, y, 4);
      }
   }
   if (page == 9 || (page == 10 && !n))
   {  mdouble x, y;
      pplane->traceRay(1, 25.0/127.0, pf, x, y, 5, Qt::magenta);
      pplane->traceRay(1, 34.0/127.0, pf, x, y, 5, Qt::magenta);
      pplane->traceRay(1, 3.0/15.0, pf, x, y, 5, Qt::blue);
      pplane->traceRay(1, 4.0/15.0, pf, x, y, 5, Qt::blue);
      pplane->traceRay(1, 26.0/127.0, pf, x, y, 5, Qt::red);
      pplane->traceRay(1, 33.0/127.0, pf, x, y, 5, Qt::red);
   }
   if (page == 10 && n < 0)
   {  mdouble x, y;
      pplane->traceRay(-2*n - 3, 25.0/127.0, pf, x, y, 10, Qt::magenta);
      pplane->traceRay(-2*n - 3, 34.0/127.0, pf, x, y, 10, Qt::magenta);
   }
} //pFinished QC

void QmnDemoQC::dFinished()
{  mdouble x, y;
   if (page <= 4 && !(n & 4) )
   {  dplane->traceRay(-1, 3223.0/16128.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 1615.0/8064.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 1643.0/8064.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 3293.0/16128.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 4231.0/16128.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 2119.0/8064.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 2147.0/8064.0, df, x, y, 4, Qt::darkCyan);
      dplane->traceRay(-1, 4301.0/16128.0, df, x, y, 4, Qt::darkCyan);
      x = -0.11866; y = 1.00292;
      dplane->green(df, -1, df->green(-1, x, y), 4, Qt::red);
      for (int j = 0; j < 4; j++) df->iterate(x, y, -1);
      dplane->green(df, -1, df->green(-1, x, y), 4, Qt::darkCyan);
      dplane->traceRay(-1, 103.0/504.0, df, x, y, 4, Qt::red);
      dplane->traceRay(-1, 131.0/504.0, df, x, y, 4, Qt::red);
      dplane->traceRay(-1, 199.0/1008.0, df, x, y, 4, Qt::red);
      dplane->traceRay(-1, 269.0/1008.0, df, x, y, 4, Qt::red);
      n |= 4; uint n1 = 1027; dplane->draw(df, -1, &n1, 2);
   }
   if (page == 5)
   {  dplane->traceRay(-1, 1.0/7.0, df, x, y, 2, Qt::blue);
      dplane->traceRay(-1, 2.0/7.0, df, x, y, 2, Qt::darkCyan);
      dplane->traceRay(-1, 4.0/7.0, df, x, y, 2, Qt::green);
   }
   if (page == 6)
   {  dplane->traceRay(-1, 1.0/3.0, df, x, y, 2, Qt::darkCyan);
      dplane->traceRay(-1, 2.0/3.0, df, x, y, 2, Qt::green);
   }
   if (page == 7)
   {  dplane->traceRay(-1, 1.0/7.0, df, x, y, 2, Qt::blue);
      dplane->traceRay(-1, 2.0/7.0, df, x, y, 2, Qt::darkCyan);
      dplane->traceRay(-1, 4.0/7.0, df, x, y, 2, Qt::red);
      dplane->traceRay(-1, 9.0/14.0, df, x, y, 2, Qt::blue);
      dplane->traceRay(-1, 11.0/14.0, df, x, y, 2, Qt::darkCyan);
      dplane->traceRay(-1, 1.0/14.0, df, x, y, 2, Qt::red);
   }
   if (page == 8)
   {  dplane->traceRay(-1, 1.0/7.0, df, x, y, 5, Qt::red);
      dplane->traceRay(-1, 4.0/7.0, df, x, y, 5, Qt::red);
      dplane->traceRay(-1, 9.0/14.0, df, x, y, 5, Qt::magenta);
      dplane->traceRay(-1, 1.0/14.0, df, x, y, 5, Qt::magenta);
      dplane->traceRay(-1, 5.0/63.0, df, x, y, 5, Qt::darkCyan);
      dplane->traceRay(-1, 40.0/63.0, df, x, y, 5, Qt::darkCyan);
      dplane->traceRay(-1, 17.0/126.0, df, x, y, 5, Qt::blue);
      dplane->traceRay(-1, 73.0/126.0, df, x, y, 5, Qt::blue);
   }
   if (page == 9)
   {  df->getParameter(x, y);
      dplane->backRay(11LL, 56LL,  x, y, 3);
      dplane->backRay(15LL, 56LL,  x, y, 3);
      dplane->backRay(199LL, 1008LL,  x, y, 3);
      dplane->backRay(269LL, 1008LL,  x, y, 3);
      dplane->backRay(23LL, 112LL,  x, y, 3);
      dplane->backRay(29LL, 112LL,  x, y, 3);
}
   if (page == 10 && n < 0)
   {  mdouble x, y;
      dplane->traceRay(-2*n - 3, 3.0/15.0, df, x, y, 7, Qt::blue);
      dplane->traceRay(-2*n - 3, 4.0/15.0, df, x, y, 7, Qt::blue);
   }
   if (page == 10 && n >= 0)
   {  mdouble x, y; df->getParameter(x, y);
      dplane->backRay(76LL, 127LL, x, y, 3, Qt::magenta);
      dplane->backRay(17LL, 127LL, x, y, 3, Qt::magenta);
      dplane->backRay(9LL, 15LL, x, y, 3, Qt::blue);
      dplane->backRay(2LL, 15LL, x, y, 3, Qt::blue);
      dplane->backRay(13LL, 127LL, x, y, 3, Qt::red);
      dplane->backRay(80LL, 127LL, x, y, 3, Qt::red);
   }
} //dFinished QC

////////////////////////////////////////////////////////////////////////

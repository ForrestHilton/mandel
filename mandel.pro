TEMPLATE = app
TARGET = mandel
INCLUDEPATH += .
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_FILE = qmnicons.rc
HEADERS += mndynamo.h qmnplane.h qmnshell.h
SOURCES += mndlbrot.cpp mndynamo.cpp mndcombi.cpp qmnplane.cpp qmnshell.cpp qmndemos.cpp qmndialg.cpp
TRANSLATIONS = mandel_de.ts mandel_pl.ts mandel_pt.ts

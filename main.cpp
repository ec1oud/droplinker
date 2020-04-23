/****************************************************************************
**
** Copyright (C) 2020 Shawn Rutledge
**
** This file is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** version 3 as published by the Free Software Foundation
** and appearing in the file LICENSE included in the packaging
** of this file.
**
** This code is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QLibraryInfo>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QLatin1String("Drop Linker"));
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main",
        "drag and drop files to create symlinks in a directory"));
    parser.addHelpOption();
    parser.addPositionalArgument("destination", QCoreApplication::translate("main",
        "Directory in which to create links."));
    parser.process(app);

#ifndef QT_NO_TRANSLATION
    //qDebug() << "looking for" << QLatin1String("droplinker_") + QLocale().name() << "in" <<     QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("droplinker"), QLatin1String("_")))
        QCoreApplication::installTranslator(&translator);
#endif

    MainWindow w(parser.positionalArguments().count() > 0 ?
                     parser.positionalArguments().first() : QString());
    w.show();
    return app.exec();
}

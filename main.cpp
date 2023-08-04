#define EXERCISE 2

#if EXERCISE == 1
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QWebEngineSettings>


#include "1/weathertrayicon.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    return app.exec();
}

#endif

#if EXERCISE == 2
/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include "1.2/browserwindow.h"
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QNetworkProxy>
#include <QPushButton>
#include <QVBoxLayout>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(app.translate("main", "Super-window"));
#ifdef Q_WS_MAC
        app.setCursorFlashTime(0);
#endif

    QString url("pl.wikipedia.org/wiki/Main_Page");
    if (argc > 1)
        url = argv[1];
    BrowserWindow *browser = new BrowserWindow(url);
    if (argc > 2) browser->showToolBar(false); // For testing; don't quote
    browser->show();
    return app.exec();
}



#endif

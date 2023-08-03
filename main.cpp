#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QWebEngineSettings>

#define EXERCISE 1

#if EXERCISE == 1

#include "1/weathertrayicon.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    return app.exec();
}

#endif

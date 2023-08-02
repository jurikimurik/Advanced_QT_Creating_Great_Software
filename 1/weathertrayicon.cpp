#include "weathertrayicon.h"

#include <QApplication>
#include <QActionGroup>
#include <QSettings>
#include <QTimer>
#include <QTextDocumentFragment>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

WeatherTrayIcon::WeatherTrayIcon() : QSystemTrayIcon(), retryDelaySec(1)
{
    setIcon(QIcon(":/1/rss.png"));
    createContextMenu();
    networkJSONAccess = new QNetworkAccessManager(this);
    connect(networkJSONAccess, &QNetworkAccessManager::finished, this, &WeatherTrayIcon::readJSON);
    QTimer::singleShot(0, this, &WeatherTrayIcon::requestJSON);
}

void WeatherTrayIcon::readJSON(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError) {
        setToolTip(tr("Błąd pobierania danych pogodowych:\n%1").arg(reply->errorString()));
        QTimer::singleShot(retryDelaySec * 1000, this, &WeatherTrayIcon::requestJSON);
        retryDelaySec <<= 1;
        if(retryDelaySec > 60*60)
            retryDelaySec = 1;
        return;
    }
    retryDelaySec = 1;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if(!document.isNull()) {
        populateToolTip(&document);
        updateIcon(&document);
    }
    QTimer::singleShot(60*60*1000, this, &WeatherTrayIcon::requestJSON);
}

void WeatherTrayIcon::updateIcon(QJsonDocument *document)
{
    QString weathercode = textForTag("weathercode", document);
    QString isDay = textForTag("is_day", document);
    if(weathercode.isEmpty() || isDay.isEmpty())
        return;

    setIcon(getWeatherIcon(isDay.toInt(), weathercode.toInt()));
}

void WeatherTrayIcon::setCity(QAction *action)
{
    cityData = action->data().toStringList();
    QSettings settings;
    settings.setValue("cityData", cityData);
    requestJSON();
}

QIcon WeatherTrayIcon::getWeatherIcon(bool isDay, int weatherCode)
{
    QString searchedName;
    if(isDay)
        searchedName += "day_";
    else
        searchedName += "night_";
    searchedName += QString::number(weatherCode) + ".png";

    {
        //Try to find icon
        QIcon newIcon(":/weather/" + searchedName);
        if(!newIcon.isNull())
            return newIcon;
    }


    {
        //If there is none, search with "none_"
        QIcon newIcon(":/weather/none_" + QString::number(weatherCode) + ".png");
        if(!newIcon.isNull())
            return newIcon;
    }

    // Default icon
    return QIcon(":/1/rss.png");
}

void WeatherTrayIcon::requestJSON()
{
    QString longitude = cityData.at(1);
    QString latitude = cityData.at(2);

    QString url = QString("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current_weather=true")
                      .arg(latitude, longitude);
    networkJSONAccess->get(QNetworkRequest(QUrl(url)));
}

void WeatherTrayIcon::createContextMenu()
{
    QList<QStringList> citiesData = readCities();

    QSettings settings;
    cityData = settings.value("cityData", QVariant(citiesData.value(0))).toStringList();
    QActionGroup *group = new QActionGroup(this);
    for (int i = 0; i < citiesData.size(); ++i) {
        const QString &anCity = citiesData.at(i).at(0);
        QAction *action = menu.addAction(anCity);
        group->addAction(action);
        action->setCheckable(true);
        action->setChecked(anCity == cityData.at(0));
        action->setData(citiesData.at(i));
    }
    connect(group, &QActionGroup::triggered, this, &WeatherTrayIcon::setCity);
    menu.addSeparator();
    menu.addAction(QIcon(":/1/exit.png"), tr("Wyjście"), qApp, &QApplication::exit);
        setContextMenu(&menu);
}

void WeatherTrayIcon::populateToolTip(QJsonDocument *document)
{
        QString toolTipText = tr("<font color=darkblue>%1</font><br>").arg(cityData.at(0));
        QString temperature = textForTag("temperature", document);
        if(!temperature.isEmpty())
        toolTipText += toolTipField(tr("Temperatura"), "green", temperature, false);

#ifndef Q_WS_X11
        toolTipText = QTextDocumentFragment::fromHtml(toolTipText).toPlainText();
#endif
        setToolTip(toolTipText);
}

QString WeatherTrayIcon::textForTag(const QString &tag, QJsonDocument *document)
{
    QJsonValue currentWeather = (*document)["current_weather"];
    if(currentWeather.isObject())
    {
        QJsonValue value = currentWeather[tag];
            if(!value.isNull()) {
            return value.toVariant().toString();
            }
    }
    return QString();
}

QString WeatherTrayIcon::toolTipField(const QString &name, const QString &htmlColor, const QString &value, bool appendBr)
{
    return QString("<i>%1:</i>&nbsp;<font color=\"%2\">%3</font>%4")
        .arg(name).arg(htmlColor).arg(value).arg(appendBr ? "<br>" : "");
}

QList<QStringList> WeatherTrayIcon::readCities(const QString &pathname)
{
    QList<QStringList> data;

    QFile datafile(pathname);
    if(!datafile.open(QIODevice::ReadOnly)) {
            qDebug() << tr("Nie mogę przeczytać pliku z danymi!");
            return data;
    }

    QTextStream stream(&datafile);
    QRegularExpression regex(stream.readLine().split("REGEX:").value(1));

    QString line;
    while(stream.readLineInto(&line))
    {
            QRegularExpressionMatch match(regex.match(line));
            if(match.hasMatch())
            {
                QStringList list(match.capturedTexts());
                    //Deleting basic captured group
                list.pop_front();
                    //Deleting whitespaces for name
                list.first() = list.first().simplified();
                    //Releasing memory
                for(QString &str : list)
                    str.shrink_to_fit();

                data.push_back(list);
            }
    }

    return data;
}

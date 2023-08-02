#include "weathertrayicon.h"

#include <QApplication>
#include <QActionGroup>
#include <QSettings>
#include <QTimer>

WeatherTrayIcon::WeatherTrayIcon() : QSystemTrayIcon(), retryDelaySec(1)
{
    setIcon(QIcon(":/1/rss.png"));
    createContextMenu();
    networkJSONAccess = new QNetworkAccessManager(this);
    networkIconAccess = new QNetworkAccessManager(this);
    connect(networkJSONAccess, &QNetworkAccessManager::finished, this, &WeatherTrayIcon::readJSON);
    connect(networkIconAccess, &QNetworkAccessManager::finished, this, &WeatherTrayIcon::readIcon);
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
    if(!document.isNull())
        populateToolTip(&document);
    QTimer::singleShot(60*60*1000, this, &WeatherTrayIcon::requestJSON);
}

void WeatherTrayIcon::setCity(QAction *action)
{

}

void WeatherTrayIcon::readIcon(QNetworkReply *reply)
{

}

void WeatherTrayIcon::requestJSON()
{
    QString coordinates;
    if(city == "Gdansk")
        coordinates = "54.22 18.38";
    else if(city == "Wejherowo")
        coordinates = "54.36 18.15";
    else if(city == "Gdynia")
        coordinates = "54.32 18.32";

    QString latitude = coordinates.split(" ").at(0);
    QString longitude = coordinates.split(" ").at(1);

    QString url = QString("https://api.open-meteo.com/v1/forecast?latitude=%1&longitude=%2&current_weather=true")
                      .arg(latitude, longitude);
    networkJSONAccess->get(QNetworkRequest(QUrl(url)));
}

void WeatherTrayIcon::createContextMenu()
{
    QStringList cities;
    cities << "Gdansk" << "Wejherowo" << "Gdynia";
    QStringList coordinates;
    coordinates << "54.22 18.38" << "54.36 18.15" << "54.32 18.32";

    QSettings settings;
    city = settings.value("city", QVariant(cities.at(0))).toString();
    QActionGroup *group = new QActionGroup(this);
    for (int i = 0; i < cities.size(); ++i) {
        const QString &anCity = cities.at(i);
        QAction *action = menu.addAction(anCity);
        group->addAction(action);
        action->setCheckable(true);
        action->setChecked(anCity == city);
        action->setData(coordinates.at(i));
    }
    connect(group, &QActionGroup::triggered, this, &WeatherTrayIcon::setCity);
    menu.addSeparator();
    menu.addAction(QIcon(":/1/exit.png"), tr("Wyjście"), qApp, &QApplication::exit);
        setContextMenu(&menu);
}

void WeatherTrayIcon::populateToolTip(QJsonDocument *document)
{

}

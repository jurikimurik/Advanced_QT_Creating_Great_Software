#ifndef WEATHERTRAYICON_H
#define WEATHERTRAYICON_H

#include <QAction>
#include <QCache>
#include <QIcon>
#include <QJsonDocument>
#include <QMenu>
#include <QNetworkReply>
#include <QSystemTrayIcon>

class WeatherTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit WeatherTrayIcon();
private slots:
    void requestJSON();
    void readJSON(QNetworkReply *reply);
    void setCity(QAction *action);

private:
    QIcon getWeatherIcon(bool isDay, int weatherCode);
    void updateIcon(QJsonDocument *document);

    void createContextMenu();
    void populateToolTip(QJsonDocument *document);
    QString textForTag(const QString &tag, QJsonDocument *document);
    QString toolTipField(const QString &name, const QString &htmlColor, const QString &value, bool appendBr = true);

    QList<QStringList> readCities(const QString &pathname = ":/1/cities.txt");

    QMenu menu;
    QNetworkAccessManager *networkJSONAccess;
    QStringList cityData;
    QCache<QUrl, QIcon> iconCache;
    int retryDelaySec;
};

#endif // WEATHERTRAYICON_H

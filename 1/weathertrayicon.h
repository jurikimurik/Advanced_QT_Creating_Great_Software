#ifndef WEATHERTRAYICON_H
#define WEATHERTRAYICON_H

#include <QAction>
#include <QCache>
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
    void readIcon(QNetworkReply *reply);
    void setCity(QAction *action);
private:
    void createContextMenu();
    void populateToolTip(QJsonDocument *document);

    QMenu menu;
    QNetworkAccessManager *networkJSONAccess;
    QNetworkAccessManager *networkIconAccess;
    QString city;
    QCache<QUrl, QIcon> iconCache;
    int retryDelaySec;
};

#endif // WEATHERTRAYICON_H

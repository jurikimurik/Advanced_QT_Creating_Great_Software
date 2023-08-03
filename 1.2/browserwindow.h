#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>
#include <QWebEnginePage>
#include <QWebEngineView>
#include <qprogressbar.h>

class BrowserWindow : public QFrame
{
    Q_OBJECT
public:
    explicit BrowserWindow(const QString &url = QString(),
                           QWebEnginePage *webPage = nullptr, QWidget *parent = nullptr,
                           Qt::WindowFlags flags = Qt::WindowFlags());

signals:
    void loadFinished(bool ok);
    void urlChanged(const QUrl &url);

public slots:
    void load(const QString &url);
    void setUrl();
    void urlChange(const QUrl &url);
    void loadFinish(bool ok);
    void setZoomFactor(int zoom);
    void zoomOut();
    void zoomIn();
    void enableActions(bool enable);
    void popUpHistoryMenu();

    void setHtml(const QString &html) { webView->setHtml(html); }
    void showToolBar(bool on) { toolBar->setVisible(on); }

protected:
    void focusInEvent(QFocusEvent *event) {webView->setFocus();}

private:
    void createActions();
    void createToolBar();
    void createLayout();
    void createConnections();

private:
    QWebEngineView *webView;
    QString html;

    QToolBar *toolBar;
    QSpinBox *zoomSpinBox;
    QProgressBar *progressBar;
    QLabel *progressLabel;

    QAction *zoomOutAction;
    QAction *zoomInAction;
    QAction *setUrlAction;
    QAction *historyAction;

    const int ZoomStepSize = 5;
    const int MaxHistoryMenuItems = 20;
    const int MaxMenuWidth = 300;
};

#endif // BROWSERWINDOW_H

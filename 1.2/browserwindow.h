#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QFrame>
#include <QWebEnginePage>
#include <QWebEngineView>

class BrowserWindow : public QFrame
{
    Q_OBJECT
public:
    explicit BrowserWindow(const QString &url = QString(),
                           QWebEnginePage *webPage = nullptr, QWidget *parent = nullptr,
                           Qt::WindowFlags flags = Qt::WindowFlags());

    void toHtmlInto(QString &where) const
    {
        webView->page()->toHtml([&where](const QString &result) {where = result;});
    }

    void toPlainText(QString &where) const
    {
        webView->page()->toPlainText([&where](const QString &result) {where = result;});
    }

signals:
    void loadFinished(bool ok);
    void urlChanged(const QUrl &url);

public slots:
    void load(const QString &url);
    void setHtml(const QString &html) { webView->setHtml(html); }
    void showToolBar(bool on) { toolBar->setVisible(on); }
    void enableActions(bool enable);

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

    QAction *zoomOutAction;
    QAction *zoomInAction;
    QAction *setUrlAction;
    QAction *historyAction;
};

#endif // BROWSERWINDOW_H

#include "browserwindow.h"

BrowserWindow::BrowserWindow(const QString &url, QWebEnginePage *webPage, QWidget *parent, Qt::WindowFlags flags)
    : QFrame(parent, flags)
{
    setFrameStyle(QFrame::Box|QFrame::Raised);

    webView = new QWebEngineView(this);
    if (webPage)
        webView->setPage(webPage);
    load(url);

    createActions();
    createToolBar();
    createLayout();
    createConnections();
}

void BrowserWindow::createActions()
{

}

void BrowserWindow::createToolBar()
{

}

void BrowserWindow::createLayout()
{

}

void BrowserWindow::createConnections()
{

}

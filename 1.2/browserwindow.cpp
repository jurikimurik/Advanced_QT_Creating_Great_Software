#include "browserwindow.h"

#include <QFontMetrics>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QProgressBar>
#include <QSet>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWebEngineHistoryItem>

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

    readSettings();
}

void BrowserWindow::load(const QString &url)
{
    if(url.isEmpty())
        return;
    QString theUrl = url;
    if(!theUrl.contains("://"))
        theUrl.prepend("https://");
    webView->load(theUrl);
}

void BrowserWindow::setUrl()
{
    load(QInputDialog::getText(this, tr("Ustaw URL"), tr("&URL:")));
}

void BrowserWindow::urlChange(const QUrl &url)
{
    emit urlChanged(url);
    progressLabel->setText(tr("Ładowanie"));
}

void BrowserWindow::loadFinish(bool ok)
{
    emit loadFinished(ok);
    progressLabel->setText(ok ? tr("Załadowano") : tr("Anulowano"));
}

void BrowserWindow::setZoomFactor(int zoom)
{
    webView->setZoomFactor(zoom / 100.0);
}

void BrowserWindow::zoomOut()
{
    zoomSpinBox->setValue(zoomSpinBox->value() - ZoomStepSize);
}

void BrowserWindow::zoomIn()
{
    zoomSpinBox->setValue(zoomSpinBox->value() + ZoomStepSize);
}

void BrowserWindow::enableActions(bool enable)
{
    for(QAction *action : webView->actions())
    {
        action->setEnabled(enable);
    }
    toolBar->setEnabled(enable);
    webView->setContextMenuPolicy(enable ? Qt::ActionsContextMenu : Qt::NoContextMenu);
}

void BrowserWindow::popUpHistoryMenu()
{
    QFontMetrics fontMetrics(font());
    QMenu menu;
    QSet<QUrl> uniqueUrls;
    QListIterator<QWebEngineHistoryItem> i(webView->history()->items());
    i.toBack();
    while(i.hasPrevious() && uniqueUrls.count() < MaxHistoryMenuItems) {
        const QWebEngineHistoryItem &item = i.previous();
        if(uniqueUrls.contains(item.url()))
            continue;
        uniqueUrls << item.url();
        QString title = fontMetrics.elidedText(item.title(), Qt::ElideRight, MaxMenuWidth);
        QAction *action = new QAction(title, &menu);
        action->setData(item.url());
        menu.addAction(action);
    }
    if(QAction *action = menu.exec(QCursor::pos()))
        webView->load(action->data().toUrl());
}

void BrowserWindow::createActions()
{
    zoomOutAction = new QAction(QIcon(":/icons/zoom_out.png"), tr("Powiększenie"), this);
                    zoomOutAction->setShortcuts(QKeySequence::ZoomOut);
    zoomInAction = new QAction(QIcon(":/icons/zoom_in.png"), tr("Zmniejszenie"), this);
                    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    setUrlAction = new QAction(QIcon(":/icons/set_url.png"), tr("Ustaw URL"), this);
                    historyAction = new QAction(QIcon(":/icons/history.png"), tr("Historia"), this);
    QAction* actionBack = webView->pageAction(QWebEnginePage::Back);
                    actionBack->setIcon(QIcon(":/icons/go_back.png"));
    QAction* actionForward = webView->pageAction(QWebEnginePage::Forward);
                    actionForward->setIcon(QIcon(":/icons/go_forward.png"));
    QAction* actionReload = webView->pageAction(QWebEnginePage::Reload);
                    actionReload->setIcon(QIcon(":/icons/reload.png"));
    QAction* actionStop = webView->pageAction(QWebEnginePage::Stop);
                    actionStop->setIcon(QIcon(":/icons/stop.png"));


    QList<QAction*> actions;
    actions << webView->pageAction(QWebEnginePage::Back)
            << webView->pageAction(QWebEnginePage::Forward)
            << webView->pageAction(QWebEnginePage::Reload)
            << webView->pageAction(QWebEnginePage::Stop)
            << zoomOutAction << zoomInAction << setUrlAction
            << historyAction;

#ifdef  DEBUG
    actions << webView->pageAction(QWebEnginePage::InspectElement);
#endif

    webView->addActions(actions);
    webView->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void BrowserWindow::createToolBar()
{
    toolBar = new QToolBar(tr("Nawigacja"), this);
#ifdef Q_WS_MAC
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
#endif
    toolBar->addAction(webView->pageAction(QWebEnginePage::Back));
    toolBar->addAction(webView->pageAction(QWebEnginePage::Forward));
    toolBar->addAction(webView->pageAction(QWebEnginePage::Reload));
    toolBar->addAction(webView->pageAction(QWebEnginePage::Stop));
    toolBar->addSeparator();
    toolBar->addAction(zoomOutAction);
    toolBar->addAction(zoomInAction);
    toolBar->addSeparator();
    zoomSpinBox = new QSpinBox;
    zoomSpinBox->setSingleStep(ZoomStepSize);
    zoomSpinBox->setRange(25, 250);
    zoomSpinBox->setValue(100);
    zoomSpinBox->setSuffix(tr("%"));
    zoomSpinBox->setFocusPolicy(Qt::NoFocus);
    QLabel *zoomLabel = new QLabel(tr("Powiększenie"));
                        zoomLabel->setMargin(4);
    progressBar = new QProgressBar;
    progressLabel = new QLabel(tr("Postęp"));
                    progressLabel->setMargin(4);
#ifndef Q_WS_MAC
    toolBar->addWidget(zoomLabel);
    toolBar->addWidget(zoomSpinBox);
    toolBar->addSeparator();
    toolBar->addAction(setUrlAction);
    toolBar->addAction(historyAction);
    toolBar->addWidget(progressLabel);
    toolBar->addWidget(progressBar);
#else
    QWidget *zoomWidget = new QWidget;
    QVBoxLayout *zoomLayout = new QVBoxLayout;
    zoomLayout->addWidget(zoomSpinBox);
    zoomLayout->addWidget(zoomLabel);
    zoomLayout->setAlignment(zoomLabel, Qt::AlignCenter);
    zoomWidget->setLayout(zoomLayout);
    toolBar->addWidget(zoomWidget);
    toolBar->addSeparator();
    toolBar->addAction(setUrlAction);
    toolBar->addAction(historyAction);
    QWidget *progressWidget = new QWidget;
    QVBoxLayout *progressLayout = new QVBoxLayout;
    progressLayout->setAlignment(Qt::AlignCenter);
    progressLayout->addWidget(progressBar);
    progressLayout->addWidget(progressLabel);
    progressLayout->setAlignment(progressLabel, Qt::AlignCenter);
    progressWidget->setLayout(progressLayout);
    toolBar->addWidget(progressWidget);
#endif
}

void BrowserWindow::createLayout()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(webView);
    layout->addWidget(toolBar);
    setLayout(layout);
}

void BrowserWindow::createConnections()
{
    connect(webView, &QWebEngineView::loadProgress, progressBar, &QProgressBar::setValue);
    connect(webView, &QWebEngineView::urlChanged, this, &BrowserWindow::urlChange);
    connect(webView, &QWebEngineView::loadFinished, this, &BrowserWindow::loadFinished);
    connect(setUrlAction, &QAction::triggered, this, &BrowserWindow::setUrl);
    connect(historyAction, &QAction::triggered, this, &BrowserWindow::popUpHistoryMenu);
    connect(zoomOutAction, &QAction::triggered, this, &BrowserWindow::zoomOut);
    connect(zoomInAction, &QAction::triggered, this, &BrowserWindow::zoomIn);
    connect(zoomSpinBox, &QSpinBox::valueChanged, this, &BrowserWindow::setZoomFactor);
}

void BrowserWindow::readSettings()
{
    QSettings settings;
    QByteArray geometry = settings.value("geometry").toByteArray();
    qDebug() << geometry.isEmpty();
    if(!geometry.isEmpty())
        restoreGeometry(geometry);
}

void BrowserWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QFrame::closeEvent(event);
}

//---------------------------------------------------------------------------------------------------

template<typename Arg, typename R, typename C>
struct InvokeWrapper {
    R *receiver;
    void (C::*memberFun)(Arg);
    void operator()(Arg result) {
        (receiver->*memberFun)(result);
    }
};

template<typename Arg, typename R, typename C>
InvokeWrapper<Arg, R, C> invoke(R *receiver, void (C::*memberFun)(Arg))
{
    InvokeWrapper<Arg, R, C> wrapper = {receiver, memberFun};
    return wrapper;
}

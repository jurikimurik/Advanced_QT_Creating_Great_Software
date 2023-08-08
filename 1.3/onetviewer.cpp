#include "onetviewer.h"
#include "ui_onetviewer.h"

#include <QTimer>

const QString OnetURL("https://wiadomosci.onet.pl");
OnetViewer::OnetViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OnetViewer)
{
    ui->setupUi(this);
    createWidgets();
    createLayout();

    issueLinkFetcher = new LinkFetcher(OnetURL, scriptPathAndName("fetch_issue_links.js"), this);
    articleLinkFetcher = new LinkFetcher(OnetURL, scriptPathAndName("fetch_article_links.js"), this);
    createConnections();
    //issueComboBox->setFocus();
    issueLinkFetcher->load(OnetURL);
    QTimer::singleShot(1000*60, this, &OnetViewer::networkTimeout);
}

OnetViewer::~OnetViewer()
{
    delete ui;
}

void OnetViewer::networkTimeout()
{

}

void OnetViewer::createWidgets()
{

}

void OnetViewer::createLayout()
{

}

void OnetViewer::createConnections()
{

}

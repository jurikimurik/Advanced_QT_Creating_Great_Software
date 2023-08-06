#include "onetviewer.h"
#include "ui_onetviewer.h"

OnetViewer::OnetViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OnetViewer)
{
    ui->setupUi(this);
}

OnetViewer::~OnetViewer()
{
    delete ui;
}

#ifndef ONETVIEWER_H
#define ONETVIEWER_H

#include <QMainWindow>

namespace Ui {
class OnetViewer;
}

class OnetViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit OnetViewer(QWidget *parent = nullptr);
    ~OnetViewer();

protected slots:
    void networkTimeout();

private:
    void createWidgets();
    void createLayout();
    void createConnections();

private:
    Ui::OnetViewer *ui;
    LinkFetcher *issueLinkFetcher;
    LinkFetcher *articleLinkFetcher;
};

#endif // ONETVIEWER_H

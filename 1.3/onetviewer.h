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

private:
    Ui::OnetViewer *ui;
};

#endif // ONETVIEWER_H

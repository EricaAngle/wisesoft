#ifndef MAINWIN_H
#define MAINWIN_H

#include <QMainWindow>
#include <memory>
namespace angle {


class MainWin : public QMainWindow
{
    Q_OBJECT
    struct PrivateMainWinData;

public:
    explicit MainWin(QWidget *parent = 0);

signals:

public slots:

private:
    void initialize();
    void iniNetWork();
private:
    std::shared_ptr<PrivateMainWinData> _d;

};

//endof namespace angle
}

#endif // MAINWIN_H

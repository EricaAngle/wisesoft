#include "mainwin.h"
#include <QtCharts>
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include "receivethread.h"
#include "udpreceiver.h"
#include "udpthreadsender.h"

namespace angle{

struct MainWin::PrivateMainWinData{
    ReceiveThread   receiveThread;
    UdpThreadSender sender;
};

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent),
    _d(new MainWin::PrivateMainWinData)
{
    initialize();
    iniNetWork();
}

void
MainWin::initialize(){
    using namespace QtCharts;
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);
    series->append("Protein 4.2%", 4.2);
    QPieSlice *slice = series->append("Fat 15.6%", 15.6);
    slice->setExploded();
    slice->setLabelVisible();
    series->append("Other 23.8%", 23.8);
    series->append("Carbs 56.4%", 56.4);

    QChartView *chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setTitle("Donut with a lemon glaze (100g)");
    chartView->chart()->addSeries(series);
    chartView->chart()->legend()->setAlignment(Qt::AlignBottom);
    chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);
    chartView->chart()->legend()->setFont(QFont("Arial", 7));
    setCentralWidget(chartView);
}

void
MainWin::iniNetWork(){
    _d->receiveThread.start();

    std::thread senderthread(_d->sender);
    //senderthread.join();
    senderthread.detach();

}

//endof namespace angle
}


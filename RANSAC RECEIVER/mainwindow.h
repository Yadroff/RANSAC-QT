#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QInputDialog>

#include "../QCustomPlot/qcustomplot.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void titleDoubleClick(QMouseEvent * event);

    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);

    void mousePress();

    void mouseWheel();

    void removeSelectedGraph();

    void removeAllGraphs();

    void contextMenuRequest(QPoint pos);

    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

    void drawLine();

    void readData();

private:
    Ui::MainWindow *ui;
    double k_, b_;
    QUdpSocket socket_;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QElapsedTimer>
#include <QUdpSocket>
#include <QInputDialog>

#include "../QCustomPlot/qcustomplot.h"

#include "calculatormanager.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private slots:
  void titleDoubleClick(QMouseEvent *event);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void mousePress();
  void mouseWheel();
  void removeSelectedGraph();
  void removeAllGraphs();
  void contextMenuRequest(QPoint pos);
  void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  void addPoint(QMouseEvent *);
  void plot();
  void plotWithoutLine();
  void on_buttonCalculate_clicked();
  void on_buttonSend_clicked();
  void drawLine();

  void on_buttonOpen_clicked();

  void on_buttonSave_clicked();

private:
  Ui::MainWindow *ui;
  QVector<double> x_, y_;
  QVector<bool> new_;
  Answer ans_;
  QUdpSocket socket_;
};

#endif // MAINWINDOW_H

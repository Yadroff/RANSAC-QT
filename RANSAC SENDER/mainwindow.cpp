#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  std::srand(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
  ui->setupUi(this);
  
  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables);
  ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
  ui->customPlot->addGraph();
  ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
  ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
  connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
  connect(ui->customPlot, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(addPoint(QMouseEvent *)));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}


void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

//void MainWindow::addRandomGraph()
//{
//  int n = 50; // number of points in graph
//  double xScale = (std::rand()/(double)RAND_MAX + 0.5)*2;
//  double yScale = (std::rand()/(double)RAND_MAX + 0.5)*2;
//  double xOffset = (std::rand()/(double)RAND_MAX - 0.5)*4;
//  double yOffset = (std::rand()/(double)RAND_MAX - 0.5)*10;
//  double r1 = (std::rand()/(double)RAND_MAX - 0.5)*2;
//  double r2 = (std::rand()/(double)RAND_MAX - 0.5)*2;
//  double r3 = (std::rand()/(double)RAND_MAX - 0.5)*2;
//  double r4 = (std::rand()/(double)RAND_MAX - 0.5)*2;
//  QVector<double> x(n), y(n);
//  for (int i=0; i<n; i++)
//  {
//    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
//    y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
//  }
  
//  ui->customPlot->addGraph();
//  ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
//  ui->customPlot->graph()->setData(x, y);
//  ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(std::rand()%5+1));
//  if (std::rand()%100 > 50)
//    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(std::rand()%14+1)));
//  QPen graphPen;
//  graphPen.setColor(QColor(std::rand()%245+10, std::rand()%245+10, std::rand()%245+10));
//  graphPen.setWidthF(std::rand()/(double)RAND_MAX*2+1);
//  ui->customPlot->graph()->setPen(graphPen);
//  ui->customPlot->replot();
//}

void MainWindow::removeSelectedGraph()
{
  while (ui->customPlot->selectedGraphs().size() > 0)
  {
    auto graph = ui->customPlot->selectedGraphs().first();
    double x = graph->data().get()->begin()->key, y = graph->data().get()->begin()->value;
    for (int i = 0; i < x_.size(); ++i){
        if (x == x_[i] and y == y_[i]){
            x_.remove(i);
            y_.remove(i);
        }
    }
    ui->customPlot->removeGraph(graph);
    ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
  x_.clear();
  y_.clear();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
//  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
//  {
//    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
//    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
//    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
//    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
//    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
//  } else  // general context menu on graphs requested
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  
  menu->popup(ui->customPlot->mapToGlobal(pos));
}

//void MainWindow::moveLegend()
//{
//  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
//  {
//    bool ok;
//    int dataInt = contextAction->data().toInt(&ok);
//    if (ok)
//    {
//      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
//      ui->customPlot->replot();
//    }
//  }
//}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}

void MainWindow::addPoint(QMouseEvent *event)
{
 auto pos = event->pos();
 x_.append(ui->customPlot->xAxis->pixelToCoord(pos.x()));
 y_.append(ui->customPlot->yAxis->pixelToCoord(pos.y()));
 new_.append(false);
// std::cout << x_.last() << " " << y_.last() << std::endl;
 plot();
}

void MainWindow::plot()
{
    if (ans_.max != -1){
        drawLine();
    } else{
        plotWithoutLine();
    }
}

void MainWindow::plotWithoutLine()
{
    ui->statusBar->showMessage("DRAW POINTS", 500);
    ui->customPlot->clearGraphs();
    for (int i = 0; i < x_.size(); ++i){
        ui->customPlot->addGraph();
        auto graph = ui->customPlot->graph(i);
        graph->setScatterStyle(QCPScatterStyle::ssCircle);
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setPen(QPen(Qt::gray));
        graph->addData(x_[i], y_[i]);
    }
    ui->customPlot->replot();
    ui->customPlot->update();
}





void MainWindow::on_buttonCalculate_clicked()
{
    ui->statusBar->showMessage("MAKE CALCULATIONS");
    CalculatorManager manager(x_, y_, ui->doubleSpinBox->value(), ui->spinBox->value(), ui->parallel->isChecked());
    QElapsedTimer timer;
    timer.start();
    manager.start();
    std::cout << "Calculation took " << timer.elapsed() << " milliseconds" << std::endl;
    ans_ = manager.answer();
    QString b = (ans_.b > 0) ? QString("+ %1").arg(ans_.b) : QString("- %1").arg(-ans_.b);
    QString res = QString("y = %1x %2").arg(ans_.k).arg(b);
    std::cout << res.toStdString() << std::endl;
    ui->statusBar->showMessage(res, 1000);
    drawLine();
}


void MainWindow::drawLine()
{
    int n = 100; ///< количество точек на прямой
    double k = ans_.k;
    double b = ans_.b;
    QVector<double> x(n), y(n);
    double x_min = INFINITY, x_max = -INFINITY;
    double y_min = INFINITY, y_max = -INFINITY;
    for (const auto& x_cord: x_){
        x_min = qMin(x_cord, x_min);
        x_max = qMax(x_cord, x_max);
    }
    for (int i = 0; i < n; ++i){
        x[i] = x_min + (x_max - x_min) / n * i;
        y[i] = k * x[i] + b;
        y_min = qMin(y_min, y[i]);
        y_max = qMax(y_max, y[i]);
    }
    ui->statusBar->showMessage("DRAW LINES");
    ui->customPlot->clearGraphs();
    for (int i = 0; i < x_.size(); ++i){
        bool isInline = qAbs(y_[i] - (k * x_[i] + b)) < ans_.eps;
        ui->customPlot->addGraph();
        auto graph = ui->customPlot->graph(i);
        graph->setScatterStyle(QCPScatterStyle::ssCircle);
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setPen(QPen(isInline ? Qt::blue : Qt::red));
        graph->addData(x_[i], y_[i]);
    }
    ui->customPlot->addGraph();
    auto graph = ui->customPlot->graph(ui->customPlot->graphCount() - 1);
    graph->setData(x, y);
    QPen graphPen;
    graphPen.setColor(Qt::blue);
    graphPen.setWidth(3);
    graph->setPen(graphPen);
    ui->customPlot->xAxis->setRange(x_min, x_max);
    ui->customPlot->yAxis->setRange(y_min, y_max);
    ui->customPlot->replot();
    ui->customPlot->update();
    ui->statusBar->clearMessage();
}


void MainWindow::on_buttonSend_clicked()
{
    ui->statusBar->showMessage("SENDING EQUATION");
   if (ans_.max == -1){
       QMessageBox::warning(this, tr("Error"), tr("Nothing to send!"));
       ui->statusBar->clearMessage();
       return;
   }
   bool ok;
   int port = QInputDialog::getInt(this, tr("Get port"), tr("Input port to send"), 7777, 1111, 9999, 1111, &ok);
   if (!ok){
       ui->statusBar->clearMessage();
       return;
   }
   socket_.writeDatagram(QString("%1 %2").arg(ans_.k).arg(ans_.b).toUtf8(), QHostAddress::LocalHost, port);
   ui->statusBar->clearMessage();
}


void MainWindow::on_buttonOpen_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File (*.txt)"));
    if (fileName == ""){
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Error"), tr("Can not open file"));
        return;
    }
    if (!file.isReadable()){
        QMessageBox::warning(this, tr("Error"), tr("The file is not readable"));
        return;
    }
    QTextStream stream(&file);
    QVector<double> new_x;
    QVector<double> new_y;
    QChar open = '(', sep = ',', close = ')';
    double x, y;
    QString str;
    while (!stream.atEnd()){
        str = stream.readLine();
        if (str[0] == open and str[str.length() - 1] == close){
            str = str.mid(1, str.length() - 2);
            auto x_y_ = str.split(sep);
            bool ok = false;
            x = x_y_[0].toDouble(&ok);
            std::cout << x_y_[0].toStdString() << std::endl << x_y_[1].toStdString() << std::endl;
            if (!x){
                QMessageBox::warning(this, tr("Error"), tr("Can not convert string %1 to double").arg(x_y_[0]));
                break;
            }
            y = x_y_[1].toDouble(&ok);
            if (!y){
                QMessageBox::warning(this, tr("Error"), tr("Can not convert string %1 to double").arg(x_y_[1]));
                break;
            }
            new_x.append(x);
            new_y.append(y);
        }
    }
    x_ = new_x;
    y_ = new_y;
    new_.resize(x_.size());
    new_.fill(false);
//    std::cout << "HERE" << std::endl;
    plot();
}

void MainWindow::on_buttonSave_clicked()
{
    bool hasNew = false;
    for (const auto& isNew: new_){
        hasNew = hasNew or isNew;
    }
    if (!hasNew){
        QMessageBox::warning(this, tr("Warning"), tr("Nothing to save"));
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),tr(""),tr("Text File (*.txt)"));
    if (fileName == ""){
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Error"), tr("Can not open file"));
        return;
    }
    QTextStream textStream(&file);
    for (int i = 0; i < new_.size(); ++i){
        if (new_[i]){
            QString str = QString("(%1, %2)").arg(x_[i]).arg(y_[i]);
            textStream << str;
        }
    }
    QMessageBox::information(this, "Save", "File saved successfully");
    file.close();
}


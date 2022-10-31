#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectPlottables);
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
//  connect(ui->customPlot, SIGNAL(mouseDoubleClick(QMouseEvent *)), this, SLOT(addPoint(QMouseEvent *)));
    bool ok = false;
    int port;
    while (!ok) {
        port = QInputDialog::getInt(this, "Port", "Input port", 7777, 1111, 9999, 1111, &ok);
    }
    socket_.bind(port);
    connect(&socket_, SIGNAL(readyRead()), this, SLOT(readData()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent *event) {
    Q_UNUSED(event)
    if (QCPTextElement *title = qobject_cast<QCPTextElement *>(sender())) {
        // Set the plot title by double clicking on it
        bool ok;
        QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal,
                                                 title->text(), &ok);
        if (ok) {
            title->setText(newTitle);
            ui->customPlot->replot();
        }
    }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part) {
    // Set an axis label by double clicking on it
    if (part ==
        QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
    {
        bool ok;
        QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal,
                                                 axis->label(), &ok);
        if (ok) {
            axis->setLabel(newLabel);
            ui->customPlot->replot();
        }
    }
}


void MainWindow::mousePress() {
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged

    if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
    else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
    else
        ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
}

void MainWindow::mouseWheel() {
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
    else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
    else
        ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
}


void MainWindow::removeSelectedGraph() {
    while (ui->customPlot->selectedGraphs().size() > 0) {
        auto graph = ui->customPlot->selectedGraphs().first();
        ui->customPlot->removeGraph(graph);
        ui->customPlot->replot();
    }
}

void MainWindow::removeAllGraphs() {
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos) {
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    if (ui->customPlot->selectedGraphs().size() > 0)
        menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
        menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));

    menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex) {
    // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
    // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
    double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
    QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(
            dataIndex).arg(dataValue);
    ui->statusBar->showMessage(message, 2500);
}

void MainWindow::drawLine() {
    int n = 100; ///< количество точек на прямой
    double k = k_;
    double b = b_;
    double y_min = INFINITY, y_max = -INFINITY, x_min = 0, x_max = 100;
    QVector<double> x(n), y(n);
    for (int i = 0; i < n; ++i) {
        x[i] = i;
        y[i] = k * x[i] + b;
        y_min = qMin(y_min, y[i]);
        y_max = qMax(y_max, y[i]);
    }
    ui->customPlot->clearGraphs();
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
}


void MainWindow::readData() {
    QHostAddress sender;
    quint16 senderPort;
    while (socket_.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket_.pendingDatagramSize());
        socket_.readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        std::cout << "NEW MESSAGE:\n" << datagram.toStdString() << "\n Sender:" << sender.toString().toStdString()
                  << " PORT:" << senderPort << std::endl;
        QMessageBox::information(this, tr("New message"),
                                 QString("NEW MESSAGE:\n %1\n Sender: %2 PORT: %3").arg(QString(datagram)).arg(
                                         sender.toString()).arg(senderPort));
        auto list = QString(datagram).split(' ');
        if (list.size() != 2) {
            QMessageBox::warning(this, tr("ERROR"), tr("WRONG SEND FORMAT"));
            return;
        }
        bool ok;
        double new_k = list[0].toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, tr("ERROR"), QString("Can not convert %1 to double").arg(list[0]));
            return;
        }
        double new_b = list[1].toDouble(&ok);
        if (!ok) {
            QMessageBox::warning(this, tr("ERROR"), QString("Can not convert %1 to double").arg(list[1]));
            return;
        }
        k_ = new_k;
        b_ = new_b;
        drawLine();
    }
}


#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QVector>
#include <QPointF>
#include <QObject>
#include <QRandomGenerator>
#include <QDateTime>
#include <QRunnable>


struct Answer {
    double k;
    double b;
    int max;
    double eps;

    Answer() : k(0), b(0), max(-1), eps(0) {}
};

class Calculator : public QObject, public QRunnable {
    Q_OBJECT
public:
    Calculator(const QVector<double> &x, const QVector<double> &y, const double &eps, const int &iterations = 1000);

    const Answer &answer() const;
signals:
    void finish();
private:
    QVector<double> x_;
    QVector<double> y_;
    double eps_;
    int iterations_;
    QRandomGenerator generator_;
    Answer ans_;

    // QRunnable interface
public:
    void run();
};

#endif // CALCULATOR_H

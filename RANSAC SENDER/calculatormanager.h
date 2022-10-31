#ifndef CALCULATORMANAGER_H
#define CALCULATORMANAGER_H

#include <QObject>
#include <QThreadPool>

#include "calculator.h"

class CalculatorManager : public QObject {
    Q_OBJECT
public:
    CalculatorManager(const QVector<double> &x, const QVector<double> &y, const double &eps, const int &iterations,
                      const bool &parallelFlag);

    const Answer &answer() const;

public slots:
    void start();
private:
    Answer ans_;
    Calculator *calc_;
    bool parallel_;
};

#endif // CALCULATORMANAGER_H

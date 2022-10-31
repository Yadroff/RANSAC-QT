#include "calculatormanager.h"
#include <iostream>

CalculatorManager::CalculatorManager(const QVector<double> &x, const QVector<double> &y, const double &eps,
                                     const int &iterations, const bool &parallelFlag) :
        ans_(), parallel_(parallelFlag) {
    calc_ = new Calculator(x, y, eps, iterations);
    calc_->setAutoDelete(false);
}

void CalculatorManager::start() {
    auto pool = new QThreadPool;
    if (!parallel_) {
        pool->setMaxThreadCount(1);
    } else {
        pool->setMaxThreadCount(QThread::idealThreadCount());
    }
    pool->start(calc_);
    pool->waitForDone();
    ans_ = calc_->answer();
    delete pool;
}

const Answer &CalculatorManager::answer() const {
    return ans_;
}

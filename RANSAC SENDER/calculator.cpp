#include "calculator.h"

#include <iostream>

Calculator::Calculator(const QVector<double> &x, const QVector<double> &y, const double &eps, const int &iterations):
    x_(x), y_(y), eps_(eps), iterations_(iterations), generator_(QTime::currentTime().msec()) {}

const Answer &Calculator::answer() const
{
    return ans_;
}

void Calculator::run()
{
    int n = x_.size();
    Answer ans;
    if (n < 2){
        ans_ = ans;
        return;
    }
    ans.eps = eps_;
    ans.max = -1;
    int cur = 0;
    double k_cur, b_cur;
    for (int iteration = 0; iteration < iterations_; ++iteration, cur = 0){
        QVector<bool> curInlines(n, false);
        int first = 0, second = 0;
        while (first == second or x_[first] - x_[second] == 0){
            first = generator_.bounded(n);
            second = generator_.bounded(n);
//            std::cout << first << " " << second << std::endl;
        }
        QPointF point1 = QPointF(x_[first], y_[first]);
        QPointF point2 = QPointF(x_[second], y_[second]);
        // строим уравнение потенциальной прямой
        k_cur = (point1.y() - point2.y()) / (point1.x() - point2.x());
        b_cur = point1.y() - k_cur * point1.x();
        for (int i = 0; i < n; ++i){
            double x = x_[i];
            double y = y_[i];
            if (qAbs(y - (k_cur * x + b_cur)) < eps_){
                ++cur;
                curInlines[i] = true;
            }
        }
        if (ans.max < cur){
            ans.b = b_cur;
            ans.k = k_cur;
            ans.max = cur;
        }
    }
    ans_ = ans;
    emit finish();
    std::cout << "FINISH EMITED" << std::endl;
}

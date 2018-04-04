#ifndef FASTFOURIERTRANSFORM_H
#define FASTFOURIERTRANSFORM_H
#include <QVector>
#include <complex>

class FastFourierTransform
{
public:
    FastFourierTransform();
    QVector<std::complex<double>>* CombineResults(int n, QVector<std::complex<double>>* b, QVector<std::complex<double>>* c);
    QVector<std::complex<double>>* BPF(QVector<std::complex<double>>* a, bool minus = false);
    void Draw(QVector<double>* y, int N);
};

#endif // FASTFOURIERTRANSFORM_H

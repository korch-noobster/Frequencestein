#include "fastfouriertransform.h"

FastFourierTransform::FastFourierTransform()
{

}

QVector<std::complex<double>>* FastFourierTransform::CombineResults(int n, QVector<std::complex<double>>* b, QVector<std::complex<double>>* c)
{
    QVector<std::complex<double>>* image = new QVector<std::complex<double>>(n);
    for (int  i = 0; i < n / 2; i++)
    {
        (*image)[2 * i] = (*b)[i];
        (*image)[2 * i + 1] = (*c)[i];
    }
    return image;
}

QVector<std::complex<double>>* FastFourierTransform::BPF(QVector<std::complex<double>>* a, bool minus){
    int n = (*a).size();
    if (n == 1) return a;

    QVector<std::complex<double>> c(n/2), b(n/2);
    std::complex<double> w = {1, 0};
    for(int j = 0; j < n/2; j++){
        b[j] = (*a)[j] + (*a)[j + n / 2];
        c[j] = ((*a)[j] - (*a)[j + n / 2]) * w;
        w *= std::complex<double> {cos(minus?2*M_PI/n:-2*M_PI/n), sin(minus?2*M_PI/n:-2*M_PI/n)};
    }
    return CombineResults(n, BPF(&b, minus), BPF(&c, minus));
}

void FastFourierTransform::Draw(QVector<double>* y, int N)
{
    QVector<double> X_f(N);
    QVector<double> Y_f(N);
    QVector<std::complex<double>> F(N);

    for(int i = 0; i < N; i++)
    {
        F[i] = std::complex<double>{(*y)[i], 0};//myEquationComplex(i*2*M_PI/(N-1));
    }
    F = *BPF(&F);
    for(int i = 0; i < N; i++)
    {
        X_f[i] = i;
        F[i] = std::complex<double> {F[i].real()/N, F[i].imag()/N};
        (*y)[i] = sqrt(pow(F[i].real(), 2) + pow(F[i].imag(), 2));
    }
}


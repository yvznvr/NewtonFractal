#ifndef BASICSYMBOLIC_H
#define BASICSYMBOLIC_H

#include <QString>
#include <QStringList>
#include <complex>
#include <QFile>
#include <QDir>
#include <dlfcn.h>
#include <QProcess>
#include <QDebug>


class BasicSymbolic
{
public:
    BasicSymbolic();
    ~BasicSymbolic();
    BasicSymbolic(QString func);
    void createFunc();
    std::complex<float> getFuncValue(std::complex<float> z);
    std::complex<float> getDerivativeValue(std::complex<float> z);
    int degree();
private:
    QStringList coefficients, powers, signs;
    QStringList coefficientsDerivative, powersDerivative;
    QString func;
    std::complex<float> (*FunctionValue)(std::complex<float>);
    std::complex<float> (*DerivativeValue)(std::complex<float>);
    void *handle;
    void parse();
    void derivative();

};

#endif // BASICSYMBOLIC_H

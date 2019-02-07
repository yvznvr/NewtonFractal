#include "basicsymbolic.h"

BasicSymbolic::BasicSymbolic()
{

}

BasicSymbolic::~BasicSymbolic()
{
    dlclose(handle);
}

BasicSymbolic::BasicSymbolic(QString func)
{
    this->func = func;
}

void BasicSymbolic::parse()
{
    QStringList separatedList;
    QStringList separatedByPlus = func.split('+');

    if((func.at(0)!='+') || (func.at(0)!='-')) signs.append("+");
    QStringList temp = func.split(QRegExp("[^+-]"));

    for(int i=0; i<temp.size(); i++)
    {
        if(temp.at(i)!="") signs.append(temp.at(i));
    }

    for(int i=0; i<separatedByPlus.size(); i++)
    {
        separatedList.append(separatedByPlus.at(i).split('-'));
    }

    for(int i=0; i<separatedList.size(); i++)
    {
        QStringList temp = separatedList.at(i).split(QRegExp("\\D"));
        coefficients.append(temp.at(0));

        if(temp.size()==3)
            powers.append(separatedList.at(i).split(QRegExp("\\D")).at(2));
        else if(temp.size()==2)
            powers.append("1");
        else
            powers.append("0");
    }
}

void BasicSymbolic::derivative()
{
    for(int i=0; i<coefficients.size(); i++)
    {
        int coefficientOfDerivative = coefficients.at(i).toInt()*powers.at(i).toInt(); // new coefficient
        coefficientsDerivative.append(QString::number(coefficientOfDerivative));
        powersDerivative.append(QString::number(powers.at(i).toInt()-1));
    }
}

void BasicSymbolic::createFunc()
{
    parse();
    derivative();

    // Create cpp file for function and derivative
    QString temp;
    for(int i=0; i<coefficients.size(); i++)
    {
        temp.append(signs.at(i));
        temp.append(QString("std::complex<float>(%1, 0)*").arg(coefficients.at(i)));
        temp.append(QString("pow(z, %1)").arg(powers.at(i)));
    }

    QString stringFunc = QString(
     "extern \"C\" std::complex<float> Function (std::complex<float> z)"
     "{"
     "return %1;"
     "}").arg(temp);

    temp.clear();
    for(int i=0; i<coefficients.size(); i++)
    {
        if(powersDerivative.at(i) != "-1")
        {
            temp.append(signs.at(i));
            temp.append(QString("std::complex<float>(%1, 0)*").arg(coefficientsDerivative.at(i)));
            temp.append(QString("pow(z, %1)").arg(powersDerivative.at(i)));
        }
    }

    QString stringFuncDerivative = QString(
     "extern \"C\" std::complex<float> Derivative (std::complex<float> z)"
     "{"
     "return %1;"
     "}").arg(temp);

    QString tempDir = QDir::tempPath() + QDir::separator();
    QFile file(tempDir + "sym.cpp");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "#include <complex>\n" << stringFunc << "\n" << stringFuncDerivative;
    file.close();

    // build file
    QProcess build;
    build.start(QString("gcc %1/sym.cpp -o %2/sym.so -shared -fPIC").arg
                  (QDir::tempPath(),QDir::tempPath()));

    if(!build.waitForFinished()) qDebug() << "Error";

    // include shared library and assign variable
    handle = dlopen((tempDir+"sym.so").toLocal8Bit().constData(), RTLD_LAZY);

    if(handle)
    {
        *(void **)(&FunctionValue) = dlsym(handle, "Function");
        *(void **)(&DerivativeValue) = dlsym(handle, "Derivative");
    }

    char* error;
    if ((error = dlerror()) != NULL)
           qDebug() << error;
}

std::complex<float> BasicSymbolic::getFuncValue(std::complex<float> z)
{
    z = (*FunctionValue)(z);
    return z;
}

std::complex<float> BasicSymbolic::getDerivativeValue(std::complex<float> z)
{
    return (*DerivativeValue)(z);
}

int BasicSymbolic::degree()
{
    int temp=0;
    for (int i=0; i < powers.size(); i++)
    {
        if(powers.at(i).toInt() > temp) temp = powers.at(i).toInt();
    }
    return temp;
}


#include <QCoreApplication>
#include <QDebug>
#include <QVector>
#include "ACFFactors.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<int> seq;
    int v[] = {-1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1};
//    int v[] = {1, -1, 1, 1, 1};
    for(int i = 0; i < 31; ++i) {
        seq.append(v[i]);
    }
    qDebug() << "SEQ SIZE =" << seq.size();

    qDebug() << "ACF Factors:";
    QVector<int> factors = ACFFactors::valueFactors(seq);
    for(int k = 0; k < factors.size(); ++k) {
        qDebug() << "ACF(" << k << ") =" << factors.at(k);
    }

    return a.exec();
}

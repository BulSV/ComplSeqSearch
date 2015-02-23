#include "ACFFactors.h"
#include <QDebug>

QVector<int> ACFFactors::valueFactors(const QVector<int> &seq)
{
    QVector<int> factors;
    int summa = 0;

    qDebug() << "k =" << seq.size() - 1;

    for(int k = seq.size() - 1; k >= 0; --k) {
        qDebug() << "K =" << k;
        for(int i = 0; i < seq.size() - k; ++i) {
            qDebug() << "seq[" << i << "] =" << seq.at(i) << "| seq[" << i + k << "] =" << seq.at(i + k);
            summa += seq.at(i) * seq.at(i + k);
            qDebug() << "summa =" << summa;
        }
        factors.append(summa);
        summa = 0;
    }

    return factors;
}

#include <QCoreApplication>
#include <QDebug>
#include <QVector>
#include "ACFFactors.h"
#include "IInput.h"
#include "FileInput.h"
#include "ComplGen.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    IInput *input = new FileInput("Test.txt");
    input->inputSeq();

    ComplGen *complGen = new ComplGen();
    complGen->setACFFactors(ACFFactors::valueFactors(input->seqs().at(0)));
    complGen->setFiltered(true);
    complGen->generate();
    qDebug() << "GENARATED SEQS:" << complGen->getSequences();

    return a.exec();
}

#ifndef FILEINPUT_H
#define FILEINPUT_H

#include "IInput.h"
#include <QString>
#include <QVector>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class FileInput : public IInput
{
public:
    explicit FileInput(QString fileName);
    virtual ~FileInput();
    virtual void inputSeq();
    virtual QVector<QVector<int> > seqs() const;
private:
    QFile m_File;
    QVector<QVector<int> > m_Seqs;

    bool sequenceReader(QVector<int> &vec, int &pos);
    bool sequenceReaderHEX(const QByteArray &ba, QVector<int> &vec, const int &size);
    bool sequenceReaderBIN(const QByteArray &ba, QVector<int> &vec);
    bool sequenceReaderDCB(const QByteArray &ba, QVector<int> &vec);
};

#endif // FILEINPUT_H

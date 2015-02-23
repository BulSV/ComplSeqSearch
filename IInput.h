#ifndef IINPUT_H
#define IINPUT_H

#include <QVector>

class IInput
{
public:
    virtual ~IInput();
    virtual void inputSeq() = 0;
    virtual QVector<QVector<int> > seqs() const = 0;
};

#endif // IINPUT_H

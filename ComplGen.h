#ifndef COMPLGEN_H
#define COMPLGEN_H

#include <QObject>
#include <QVector>
#include <QMutex>

class ComplGen : public QObject
{
    Q_OBJECT
public:
    explicit ComplGen(QObject *parent = 0);
    explicit ComplGen(const QVector<int> &acfFactors,
                       const int &terminalSideLobes = 0,
                       bool isFiltered = true,
                       const int &closeCentralSideLobes = 0,
                       QObject *parent = 0);
    virtual ~ComplGen();
    void setCloseCentralSideLobes(const int &closeCentralSideLobes);
    void setTerminalSideLobes(const int &terminalSideLobes);
    void setFiltered(const bool &isFiltered);
    void setACFFactors(const QVector<int> &acfFactors);
    bool isFiltered() const;
    int getCloseCentralSideLobes() const;
    int getTerminalSideLobes() const;
    QVector<QVector<int> > generate();
    QVector<QVector<int> > getSequences() const;
signals:
    void sequenceGenerated(QVector<int>);
private:
    int m_seqSize;
    int m_combSize;
    int m_combSizeSimplified;
    int m_closeCentralSideLobes;
    bool m_isFiltered;
    int m_terminalSideLobes;
    int *m_combs;
    int *m_combsSimplified;
    QVector<int> m_originACFFactors;
    QVector<QVector<int> > m_tempSequences;
    QVector<QVector<int> > m_sequences;
    QMutex m_mutex;

    void fillCombinations();
    void gen(int phase, QVector<int> &seq, bool isSimplified);
    int phaseLimit();
    bool isOddSeqSize();
    bool filter(const QVector<int> &seq);
};

#endif // COMPLGEN_H

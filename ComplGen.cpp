#include "ComplGen.h"
#include <QtMath>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#ifdef DEBUG
#include <QDebug>
#endif

ComplGen::ComplGen(QObject *parent) :
    QObject(parent)
  , m_seqSize(0)
  , m_closeCentralSideLobes(0)
  , m_isFiltered(false)
  , m_terminalSideLobes(0)
{
    fillCombinations();
}

ComplGen::ComplGen(const QVector<int> &acfFactors,
                     const int &terminalSideLobes,
                     bool isFiltered,
                     const int &closeCentralSideLobes,
                     QObject *parent) :
    QObject(parent)
  , m_isFiltered(isFiltered)
{
    setACFFactors(acfFactors);
    setCloseCentralSideLobes(closeCentralSideLobes);
    setTerminalSideLobes(terminalSideLobes);
    fillCombinations();
}

ComplGen::~ComplGen()
{
    delete[] m_combs;
    delete[] m_combsSimplified;
}

void ComplGen::setCloseCentralSideLobes(const int &closeCentralSideLobes)
{
    m_closeCentralSideLobes = qAbs(closeCentralSideLobes);
}

void ComplGen::setTerminalSideLobes(const int &terminalSideLobes)
{
    m_terminalSideLobes = terminalSideLobes;
}

void ComplGen::setFiltered(const bool &isFiltered)
{
    m_isFiltered = isFiltered;
}

void ComplGen::setACFFactors(const QVector<int> &acfFactors)
{
    m_originACFFactors = acfFactors;
//#ifdef DEBUG
    qDebug() << "m_originACFFactors:" << m_originACFFactors;
//#endif
    m_seqSize = acfFactors.size();
}

bool ComplGen::isFiltered() const
{
    return m_isFiltered;
}

int ComplGen::getCloseCentralSideLobes() const
{
    return m_closeCentralSideLobes;
}

int ComplGen::getTerminalSideLobes() const
{
    return m_terminalSideLobes;
}

QVector<QVector<int> > ComplGen::generate()
{
    int phase = m_seqSize - 1;
    QVector<int> sequence;
    QVector<QFuture<void> > fs;

    sequence.fill(0, m_seqSize);
//    for(int combIndex = 0; combIndex < m_combSizeSimplified; combIndex += 2) {
//        sequence[m_seqSize - phase - 1] = m_combsSimplified[combIndex];
//        sequence[phase] = m_combsSimplified[combIndex + 1];
//        m_tempSequences.append(sequence);
//    }

//    for(int i = 0; i < m_combSizeSimplified/2; ++i) {
//        fs.append(QtConcurrent::run(this, &ComplGen::gen, phase - 1, m_tempSequences[i], true));
//    }

//    for(int i = 0; i < m_combSizeSimplified/2; ++i) {
//        fs[i].waitForFinished();
//    }
//    for(int i = 0; i < m_combSizeSimplified/2; ++i) {
//        gen(phase - 1, m_tempSequences[i], true);
//    }

    gen(phase, sequence, false);
    return m_sequences;
}

void ComplGen::gen(int phase, QVector<int> &seq, bool isSimplified)
{
    int summa = 0;
#ifdef DEBUG
    qDebug() << "IN void ComplGen::gen()";
    qDebug() << "phase =" << phase;
    qDebug() << "phaseLimit() =" << phaseLimit();
#endif
    if(phase >= phaseLimit() && phase < m_seqSize/* - 1*/
            /*&& ((!isOddSeqSize() && phase < m_seqSize - 2)
                || (isOddSeqSize() && phase < m_seqSize - 1))*/) {
#ifdef DEBUG
        qDebug() << "phase >= phaseLimit()";
#endif
        if(isOddSeqSize() && (phase == phaseLimit())) {
            for(int combIndex = 0; combIndex < 2; ++combIndex) {
                seq[phase] = qPow(-1, combIndex + 1);

                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
                qDebug() << "m_originACFFactors.at(" << m_seqSize - phase - 1 << ") =" << m_originACFFactors.at(m_seqSize - phase - 1);
                qDebug() << "m_terminalSideLobes =" << m_terminalSideLobes;
#endif
                if(qAbs(summa + m_originACFFactors.at(m_seqSize - phase - 1)) <= qAbs(m_terminalSideLobes)) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    seq[phase] = 0;
                }
                summa = 0;
            }
        } else if(isSimplified && !((seq.at(m_seqSize - phase - 1))^(seq.at(phase)))) {
            for(int combIndex = 0; combIndex < m_combSizeSimplified; combIndex += 2) {
                seq[m_seqSize - phase - 1] = m_combsSimplified[combIndex];
                seq[phase] = m_combsSimplified[combIndex + 1];

                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
                qDebug() << "m_originACFFactors.at(" << m_seqSize - phase - 1 << ") =" << m_originACFFactors.at(m_seqSize - phase - 1);
                qDebug() << "m_terminalSideLobes =" << m_terminalSideLobes;
#endif
                if(qAbs(summa + m_originACFFactors.at(m_seqSize - phase - 1)) <= qAbs(m_terminalSideLobes)) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    seq[m_seqSize - phase - 1] = 0;
                    seq[phase] = 0;
                }
                summa = 0;
            }
        } else {
            isSimplified = false;
            for(int combIndex = 0; combIndex < m_combSize; combIndex += 2) {
                seq[m_seqSize - phase - 1] = m_combs[combIndex];
                seq[phase] = m_combs[combIndex + 1];

                for(int index = 0; index < m_seqSize - phase; ++index) {
                    summa += seq.at(index)*seq.at(index + phase);
                }
#ifdef DEBUG
                qDebug() << "summa =" << summa;
                qDebug() << "m_originACFFactors.at(" << m_seqSize - phase - 1 << ") =" << m_originACFFactors.at(m_seqSize - phase - 1);
                qDebug() << "m_terminalSideLobes =" << m_terminalSideLobes;
#endif
                if(qAbs(summa + m_originACFFactors.at(m_seqSize - phase - 1)) <= qAbs(m_terminalSideLobes)) {
                    gen(phase - 1, seq, isSimplified);
                } else {
                    seq[m_seqSize - phase - 1] = 0;
                    seq[phase] = 0;
                }
                summa = 0;
            }
        }
    } else if((m_isFiltered && filter(seq)) || !m_isFiltered) {
        QMutexLocker locker(&m_mutex); // ? need or not
        emit sequenceGenerated(seq);
        m_sequences.append(seq);
    }
}

int ComplGen::phaseLimit()
{
    return (isOddSeqSize() ? (m_seqSize - 1)/2 : m_seqSize/2);
}

bool ComplGen::isOddSeqSize()
{
    return (m_seqSize & 1);
}

bool ComplGen::filter(const QVector<int> &seq)
{
    int summa = 0;

    for(int phase = 1; phase < ((m_seqSize & 1) ? (m_seqSize - 1)/2 : m_seqSize/2); ++phase) {
        for(int index = 0; index < m_seqSize - phase; ++index) {
            summa += seq.at(index)*seq.at(index + phase);
        }
#ifdef DEBUG
        qDebug() << "[!]*****bool ComplGen::filter():" << (qAbs(summa + m_originACFFactors.at(m_seqSize - phase - 1)) <= qAbs(m_closeCentralSideLobes));
        qDebug() << "[!]*****summa =" << summa;
        qDebug() << "[!]*****bool ComplGen::filter() | m_originACFFactors.at(" << m_seqSize - phase - 1 << "):" << m_originACFFactors.at(m_seqSize - phase - 1);
        qDebug() << "[!]*****bool ComplGen::filter() | m_closeCentralSideLobes:" << m_closeCentralSideLobes;
#endif
        if(qAbs(summa + m_originACFFactors.at(m_seqSize - phase - 1)) > qAbs(m_closeCentralSideLobes)) {
            return false;
        }
        summa = 0;
    }

    return true;
}

QVector<QVector<int> > ComplGen::getSequences() const
{
    return m_sequences;
}

void ComplGen::fillCombinations()
{
    int v[] = {-1, -1, -1, 1, 1, -1, 1, 1};
    int vSimple[] = {-1, -1, -1, 1, 1, -1};
    m_combSize = 8;
    m_combSizeSimplified = 6;
    m_combs = new int[m_combSize];
    m_combsSimplified = new int[m_combSizeSimplified];

    for(int i = 0; i < m_combSize; ++i) {
        m_combs[i] = v[i];
    }

    for(int i = 0; i < m_combSizeSimplified; ++i) {
        m_combsSimplified[i] = vSimple[i];
    }
}

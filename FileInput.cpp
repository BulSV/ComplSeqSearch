#include "FileInput.h"

FileInput::FileInput(QString fileName)
{
    m_File.setFileName(fileName);
}

FileInput::~FileInput()
{
}

void FileInput::inputSeq()
{
    if(!m_File.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qErrnoWarning(QString("ERROR!\nCan't open file: \"" + m_File.fileName() + "\"").toStdString().c_str());
    }

    QVector<int> vec;
    int pos = 0;

    while(sequenceReader(vec, pos)) {
            if(!vec.isEmpty()) {
                m_Seqs.append(vec);
            }
            vec.clear();
    }

    m_File.close();
}

QVector<QVector<int> > FileInput::seqs() const
{
    return m_Seqs;
}

bool FileInput::sequenceReader(QVector<int> &vec, int &pos)
{
    QByteArray ba;

    if(!m_File.atEnd()) {
        m_File.seek(pos);
        ba = m_File.readLine();
        pos = m_File.pos();
    } else {
        return false;
    }

    // Without empty line at the end of file
    if(ba.at(ba.size() - 1) == '\n') {
        ba.truncate(ba.size() - 1);
    }
    // For independence from lower or upper cases
    ba = ba.toUpper();
    // Remove all spaces
    ba = ba.replace(" ", "");

    if(ba.contains("BIN")) {
        ba.remove(0, 3);
        sequenceReaderBIN(ba, vec);
    } else if(ba.contains("HEX")) {
        ba.remove(0, 3);
        QByteArray size;
        if(ba.contains("SIZE") && ba.indexOf("SIZE") + 4 < ba.size()) {
            size = ba.mid(ba.indexOf("SIZE") + 4);
            ba.remove(ba.indexOf("SIZE"), 4 + size.size());
            sequenceReaderHEX(ba, vec, size.toInt());
        } else {
            qDebug() << "\nError! The sequence" << ba << "has no size set!\n";
        }
    } else if(!ba.isEmpty()){
        qDebug() << "\nError! The sequence" << ba << "is not valid!";
        qDebug() << "For HEX-format sequence write in file at the begin of line: HEX";
        qDebug() << "For BIN-format sequence write in file at the begin of line: BIN\n";
    }

    return true;
}

bool FileInput::sequenceReaderHEX(const QByteArray &ba, QVector<int> &vec, const int &size)
{
    for(int i = 0; i < ba.size(); ++i) {
        if(ba.at(i) != '\n' && ((ba.at(i) >= '0' && ba.at(i) <= '9') || (ba.at(i) >= 'A' && ba.at(i) <= 'F'))) {
            switch(ba.at(i)) {
            case '0':
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(-1);
                break;
            case '1':
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(1);
                break;
            case '2':
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(-1);
                break;
            case '3':
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(1);
                break;
            case '4':
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(-1);
                break;
            case '5':
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(1);
                break;
            case '6':
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(-1);
                break;
            case '7':
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(1);
                break;
            case '8':
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(-1);
                break;
            case '9':
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(-1);
                vec.push_back(1);
                break;
            case 'A':
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(-1);
                break;
            case 'B':
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(1);
                vec.push_back(1);
                break;
            case 'C':
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(-1);
                break;
            case 'D':
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(-1);
                vec.push_back(1);
                break;
            case 'E':
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(-1);
                break;
            case 'F':
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(1);
                vec.push_back(1);
                break;
            default:
                qDebug() << "Error! Not HEX format!";
                return false;
            }
        } else {
            qDebug() << "\nError! The sequence" << ba << "has not HEX format!\n";
            return false;
        }
    }

    if(vec.size() < size || vec.size() - 3 > size) {
        qDebug() << "\nError! The sequence" << ba << "has incorrectly size set!\n";
        return false;
    } else if(vec.size() > size) {
        vec.remove(0, vec.size() - size);
    }

    return true;
}

bool FileInput::sequenceReaderBIN(const QByteArray &ba, QVector<int> &vec)
{
    for(int i = 0; i < ba.size(); ++i) {
        if(ba.at(i) != '\n' && (ba.at(i) == '0' || ba.at(i) == '1')) {
            if(ba.at(i) == '0') {
                vec.push_back(ba.at(i) - '0' - 1);
            } else {
                vec.push_back(ba.at(i) - '0');
            }
        } else {
            qDebug() << "\nError! The sequence" << ba << "has not BIN format!\n";
            return false;
        }
    }

    return true;
}

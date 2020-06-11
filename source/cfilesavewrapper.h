#ifndef CFILESAVEWRAPPER_H
#define CFILESAVEWRAPPER_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <vector>

#define MSG_ERR_WRITE_FILE QObject::tr("Error: Cannot write file\n")

class CFileSaveWrapper
{
public:
    CFileSaveWrapper();
    ~CFileSaveWrapper();
    void Open(const QString fileName, const QString fileType, QWidget *parent);
    void Close();

    void ElemStart(const QString Val);
    void ElemVal(const QString Val);
    void ElemEnd(const QString Val);


private:
    QWidget             *m_parent;
    QFile               *m_pQFile;
    QXmlStreamWriter    *m_pQXmlStreamWriter;
    QTextStream         *m_pQTextStream;
    QVector<QString>    m_tag;
};

#endif // CFILESAVEWRAPPER_H

#include "cfilesavewrapper.h"

#include <QMessageBox>

CFileSaveWrapper::CFileSaveWrapper()
    : m_parent(nullptr)
    , m_pQFile(nullptr)
    , m_pQXmlStreamWriter(nullptr)
    , m_pQTextStream(nullptr)
{
}

CFileSaveWrapper::~CFileSaveWrapper()
{
    Close();
}

void CFileSaveWrapper::Open(const QString fileName, const QString fileType, QWidget *parent)
{

    m_parent = parent;

    m_pQFile = new QFile(fileName);
    if (!m_pQFile->open(QFile::WriteOnly | QFile::Text))    //Error to write
    {
        QString mess(MSG_ERR_WRITE_FILE + fileName + "\n" +  m_pQFile->errorString());
        QMessageBox::critical(m_parent, QObject::tr("Save"), mess, QMessageBox::Ok);
        return;
    }

    if(fileType.startsWith("xml", Qt::CaseInsensitive))
    {
        m_pQXmlStreamWriter = new QXmlStreamWriter(m_pQFile);
        m_pQXmlStreamWriter->setAutoFormatting(true);
        m_pQXmlStreamWriter->writeStartDocument();
        m_pQXmlStreamWriter->writeStartElement("root");
    }
    else if(fileType.startsWith("csv", Qt::CaseInsensitive))
    {
        m_pQTextStream = new QTextStream(m_pQFile);
    }
}



void CFileSaveWrapper::Close()
{

    if(m_pQXmlStreamWriter != nullptr)
    {
        m_pQXmlStreamWriter->writeEndDocument();
        delete m_pQXmlStreamWriter;
        m_pQXmlStreamWriter = nullptr;
    }

    if(m_pQTextStream != nullptr)
    {
        m_pQTextStream->flush();
        delete m_pQTextStream;
        m_pQTextStream = nullptr;
    }

    if (m_pQFile != nullptr)
    {
        m_pQFile->close();
        if (m_pQFile->error())
        {
            QString mess(MSG_ERR_WRITE_FILE + m_pQFile->errorString());
            QMessageBox::critical(m_parent, QObject::tr("Save"), mess, QMessageBox::Ok);
        }
        delete m_pQFile;
        m_pQFile = nullptr;
    }
    m_tag.clear();
}

void CFileSaveWrapper::ElemStart(const QString Val)
{
    if( m_pQXmlStreamWriter)
        m_pQXmlStreamWriter->writeStartElement(Val);

    m_tag.push_back(Val);
}

void CFileSaveWrapper::ElemVal(const QString Val)
{
    if( m_pQXmlStreamWriter)
        m_pQXmlStreamWriter->writeCharacters(Val);
    else if(m_pQTextStream)
        *m_pQTextStream << Val;
}

void CFileSaveWrapper::ElemEnd(const QString Val)
{
    if( m_pQXmlStreamWriter)
        m_pQXmlStreamWriter->writeEndElement(/*Val*/);
    else if(m_pQTextStream)
        *m_pQTextStream << ((Val.compare("c") == 0) ? "\t" : "\n");

    if(m_tag.isEmpty() || m_tag.last().compare(Val) != 0)
        throw std::runtime_error("Error writeEndElement.");

    m_tag.pop_back();
}


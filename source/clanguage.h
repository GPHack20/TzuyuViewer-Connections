#ifndef CLANGUAGE_H
#define CLANGUAGE_H

#include <QStringList>
#include <QMap>
#include <QMainWindow>
#include <QCoreApplication>
#include <QTranslator>
#include <QDir>


class CLanguage
{
public:

    CLanguage();
    void SetLang(const QString &langName);
    void SetLangByLocale();
    bool SetLangByLocale(QString localeName);
    const QStringList GetListLangNames();
    void LoadTranslations(const QDir &dir);
    QString ExtractLanguageName(const QString &fileName);

private:

    QStringList            m_langNames;
    QMap<QString, QString> m_langList;
    QTranslator            m_translator;

};

#endif // CLANGUAGE_H

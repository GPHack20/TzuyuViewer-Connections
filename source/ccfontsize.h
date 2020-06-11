#ifndef CCFONTSIZE_H
#define CCFONTSIZE_H

#include <QSlider>
#include <QWidget>

class CCFontSize
{
public:
    CCFontSize();
    bool Init();
    void SetFontSize(int fontIndex);
    void ConfigureSlider(QSlider* slider, QWidget *pObj);
    void SaveConfig();

private:
    int                 m_fontSize;
    QWidget             *m_qwidget;
    QSlider             *m_slider;
    QList<int>          m_fontSizeList;

    int GetPointSize();
    QString GetConfigName();

};

#endif // CCFONTSIZE_H

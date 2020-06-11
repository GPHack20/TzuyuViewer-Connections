
#include "ccfontsize.h"

#include <QtGui>


CCFontSize::CCFontSize()
    : m_fontSize(-1)
    , m_qwidget(nullptr)
    , m_slider(nullptr)
{

}

void CCFontSize::ConfigureSlider(QSlider* slider, QWidget *pObj)
{
    m_qwidget = pObj;
    m_slider = slider;

    slider->setSingleStep(1);
    slider->setPageStep(1);

}

bool CCFontSize::Init()
{

    if(m_fontSize != -1)
        return false;

    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    auto initialFontSize = settings.value(GetConfigName(), m_qwidget->font().pointSize());
    auto fontSize = initialFontSize.toInt();

    m_fontSizeList = QFontDatabase::standardSizes();
    m_slider->setRange(0, m_fontSizeList.length() - 1);

    auto index = m_fontSizeList.indexOf(fontSize);
    if(index < 0)
    {
        for(QList<int>::iterator it(m_fontSizeList.begin()); it != m_fontSizeList.end(); ++it)
        {
            if(*it < fontSize)
                continue;
            m_fontSizeList.insert(it,fontSize);
            break;
        }
        index = m_fontSizeList.indexOf(fontSize);
    }

    if(index >= 0)
       m_slider->setValue(index);

    if(index >= 0)
        m_fontSize = fontSize;

    if(index >= 0)
        SetFontSize(index);

    return true;
}

void CCFontSize::SetFontSize(int fontIndex)
{
    if(m_fontSize < 0 || m_qwidget == nullptr)
        return;

    if( fontIndex >= m_fontSizeList.length() )
        fontIndex  = m_fontSizeList.length() - 1;

    m_fontSize = m_fontSizeList[fontIndex];

    auto font = m_qwidget->font();
    font.setPointSize(m_fontSize);
    m_qwidget->setFont(font);

}

int CCFontSize::GetPointSize()
{
    return m_fontSize;
}

QString CCFontSize::GetConfigName()
{
    return QString("base/fontSize");
}

void CCFontSize::SaveConfig()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue(GetConfigName(), GetPointSize());
}

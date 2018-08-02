
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QFont>
#include <QSize>
#include <QPoint>

class AppConfig : public QObject
{
    Q_OBJECT

    QFont m_editorFont;
    QSize m_mainWindowsize;
    QPoint m_mainWindowpoint;
    bool m_isAutoWrap;
    bool m_isToolBarVisible;
    bool m_isStatusBarVisible;
    bool m_isValid;
    bool m_StylsetVisible;

    bool restore();
public:
    explicit AppConfig(QObject *parent = nullptr);
    explicit AppConfig(QFont font,QPoint point,QSize size,bool isWrap,bool tbVisible,bool sbVisible,bool StylsetVisible,QObject *parent = nullptr);
    bool store();
    QFont editorFont();
    bool isAutoWrap();
    bool isToolBarVisible();
    bool isStatusBarVisible();
    QSize mainWindowsize();
    QPoint mainWindowpoint();
    bool isValid();
    bool StyleSet();
};

#endif

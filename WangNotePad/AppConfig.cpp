#include "AppConfig.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>
AppConfig::AppConfig(QObject *parent) : QObject(parent)  //这个构造函数提供读取状态的功能
{
    m_isValid = restore();
}

//这个构造函数提供保存状态的功能
AppConfig::AppConfig(QFont font,bool isWrap,bool tbVisible,bool sbVisible,QObject *parentr)
{
    m_editorFont = font;
    m_isAutoWrap = isWrap;
    m_isToolBarVisible = tbVisible;
    m_isStatusBarVisible = sbVisible;
    m_isValid = true;
}

bool AppConfig::restore()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath() + "/app.config");   //打开文件  文件名为app.config  路径和程序的路径想同；

    if(file.open(QIODevice::ReadOnly))    //只读入数据；
    {
        QDataStream in(&file);             //定义一个数据流

        in.setVersion(QDataStream::Qt_5_9);        //指定Qt版本

        in>>m_editorFont;                  //把每个变量的状态从配置文件中读出
        in>>m_isAutoWrap;
        in>>m_isToolBarVisible;
        in>>m_isStatusBarVisible;

        file.close();
    }
    else
    {
        ret = false;
    }
    return ret;
}

bool AppConfig::store()
{
    bool ret = true;
    QFile file(QApplication::applicationDirPath() + "/app.config");   //打开文件  文件名为app.config  路径和程序的路径想同；

    if(file.open(QIODevice::WriteOnly))    //只写入数据；
    {
        QDataStream out(&file);             //定义一个数据流

        out.setVersion(QDataStream::Qt_5_9);        //指定Qt版本

        out<<m_editorFont;                  //把每个变量的状态写入文件中保存
        out<<m_isAutoWrap;
        out<<m_isToolBarVisible;
        out<<m_isStatusBarVisible;

        file.close();
    }
    else
    {
        ret = false;
    }
    return ret;
}
QFont AppConfig::editorFont()
{
    return m_editorFont;
}
bool AppConfig::isAutoWrap()
{
    return m_isAutoWrap;
}
bool AppConfig::isToolBarVisible()
{
    return m_isToolBarVisible;
}
bool AppConfig::isStatusBarVisible()
{
    return m_isStatusBarVisible;
}

bool AppConfig::isValid()
{
    return m_isValid;
}







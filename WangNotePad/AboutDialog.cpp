#include "AboutDialog.h"
#include <QPixmap>
#include <QPalette>
AboutDialog::AboutDialog(QWidget* parent):QDialog(parent,Qt::WindowCloseButtonHint),m_logoLbl(this),m_infoEdit(this),m_closeBtn(this),m_qqnumber(this)
{

    QPixmap pm(":/res/pic/logo.png");
    m_logoLbl.setPixmap(pm);
    pm = pm.scaled(824,582,Qt::KeepAspectRatio);
    m_logoLbl.move(20,20);
    m_logoLbl.resize(824,582);

    QPalette p = m_infoEdit.palette();
    //在激活和非激活的状态下设置背景色 背景色直接采用对话框中的背景色
    p.setColor(QPalette::Active,QPalette::Base,palette().color(QPalette::Active,QPalette::Background));
    p.setColor(QPalette::Inactive,QPalette::Base,palette().color(QPalette::Inactive,QPalette::Background));

    m_infoEdit.move(850,30);
    m_infoEdit.resize(180,130);
    m_infoEdit.setPalette(p);       //设置信息框中的背景色
    m_infoEdit.setFrameStyle(QFrame::NoFrame);  //设置为无边框
    m_infoEdit.insertPlainText("Notepad project\n\nPlatform: Qt 5.9.0\n\nVersion: 1.0.0\n\nCopyright: Wanghuqinag");
    m_infoEdit.setReadOnly(true);

    m_qqnumber.move(850,170);
    m_qqnumber.setText("QQ Number: 1608751109");

    m_closeBtn.setText("Close");
    m_closeBtn.move(850,570);
    m_closeBtn.resize(100,30);

    setFixedSize(1050,620);
    setWindowTitle("About WangNotePad");

    connect(&m_closeBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

#include "FindDialog.h"
#include <QEvent>
#include <QTextCursor>
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    initControl();
    connetSlot();

    setLayout(&m_layout);
    setWindowTitle("Find");
//    setPlainTextEdit(pText);  //出现问题
}


void FindDialog::initControl()          //初始化查询对话框上的各种控制按钮
{
    m_findLbl.setText("Find What:");
    m_findBtn.setText("Find Next");
    m_closeBtn.setText("Close");
    m_matchChkBx.setText("Match Case");
    m_backwardBtn.setText("Backward");
    m_forwardBtn.setText("Forward");
    m_forwardBtn.setChecked(true);
    m_radioGrpBx.setTitle("Direction");

    m_hbLayout.addWidget(&m_forwardBtn);
    m_hbLayout.addWidget(&m_backwardBtn);

    m_radioGrpBx.setLayout(&m_hbLayout);

    m_layout.setSpacing(10);
    m_layout.addWidget(&m_findLbl, 0, 0);
    m_layout.addWidget(&m_findEdit, 0, 1);
    m_layout.addWidget(&m_findBtn, 0, 2);
    m_layout.addWidget(&m_matchChkBx, 1, 0);
    m_layout.addWidget(&m_radioGrpBx, 1, 1);
    m_layout.addWidget(&m_closeBtn, 1, 2);
}

bool FindDialog::event(QEvent* evt)    //自定义的事件处理函数
{
    if( evt->type() == QEvent::Close )
    {
        hide();

        return true;
    }

    return QDialog::event(evt);
}

void FindDialog::setPlainTextEdit(QPlainTextEdit* pText)
{
    m_pText = pText;        //将智能指针指向平面编辑
}
QPlainTextEdit* FindDialog::getPlainTextEdit()
{
    return m_pText;             //返回该智能指针
}

void FindDialog::connetSlot()     //在这里定义connet映射关系
{
    connect(&m_closeBtn,SIGNAL(clicked(bool)),this,SLOT(onCloseClickd()));
    connect(&m_findBtn,SIGNAL(clicked(bool)),this,SLOT(onFindClickd()));
}

void FindDialog::onFindClickd()
{
    QString target = m_findEdit.text();         //获得用户想要查找的字符串

    if(m_pText != NULL && target != "")
    {
        QString text = m_pText->toPlainText();       //把当前平面文本编辑器中的所有文本读出来；
        QTextCursor c = m_pText->textCursor();        //获得当前的光标位置
        int index = -1;

        if(m_forwardBtn.isChecked())   //检测是否是想前查找
        {
            index = text.indexOf(target,c.position(),m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
            //从光标位置开始查找，第三个参数表示是否区分大小写
            if(index>=0)
            {
                c.setPosition(index);     //设置光标的起始位置
                c.setPosition(index+target.length(),QTextCursor::KeepAnchor); //将查找到的地方设置为高亮；

                m_pText->setTextCursor(c);
            }
        }

        if(m_backwardBtn.isChecked())
        {     //从末尾开始查找
            index = text.lastIndexOf(target,c.position()-text.length()-1,m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);
            if(index>=0)
            {
                c.setPosition(index+target.length());
                c.setPosition(index,QTextCursor::KeepAnchor);

                m_pText->setTextCursor(c);
            }
        }

        if(index < 0 )
        {
            QMessageBox msg(this);
            msg.setWindowTitle("Find");
            msg.setText("Can't find \""+target+"\" ");
            msg.setIcon(QMessageBox::Information);
            msg.setStandardButtons(QMessageBox::Ok);

            msg.exec();
        }
    }
}
void FindDialog::onCloseClickd()
{
    close();
}








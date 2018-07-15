#include "ReplaceDialog.h"

ReplaceDialog::ReplaceDialog(QWidget* parent,QPlainTextEdit* pText):FindDialog(parent,pText)
{
    initControl();
    connetSlot();
    setWindowTitle("Replace");
}

void ReplaceDialog::initControl()  //初始化UI界面
{
    m_replaceLB.setText("Replace to:");
    m_replaceBtn.setText("Replace");
    m_replaceAllBtn.setText("Replace all");

    m_layout.removeWidget(&m_matchChkBx);
    m_layout.removeWidget(&m_radioGrpBx);
    m_layout.removeWidget(&m_closeBtn);

    m_layout.addWidget(&m_replaceLB,1,0);
    m_layout.addWidget(&m_replaceEdit,1,1);
    m_layout.addWidget(&m_replaceBtn,1,2);
    m_layout.addWidget(&m_matchChkBx,2,0);
    m_layout.addWidget(&m_radioGrpBx,2,1);
    m_layout.addWidget(&m_replaceAllBtn,2,2);
    m_layout.addWidget(&m_closeBtn,3,2);
}
void ReplaceDialog::connetSlot()
{
    connect(&m_replaceBtn,SIGNAL(clicked(bool)),this,SLOT(onReplaceCliked()));
    connect(&m_replaceAllBtn,SIGNAL(clicked(bool)),this,SLOT(onReplaceAllCliked()));
}

void ReplaceDialog::onReplaceCliked()   //单个替换
{
    QString target = m_findEdit.text();
    QString to = m_replaceEdit.text();

    if((m_pText != NULL) && (target != "")&&(to != ""))
    {
        QString selText = m_pText->textCursor().selectedText(); //获取当前光标标记的文本

        if(selText == target)
        {
            m_pText->insertPlainText(to);
        }

        onFindClickd();
    }
}
void ReplaceDialog::onReplaceAllCliked()  //替换所有
{
    QString target = m_findEdit.text();  //获取要查找的字符串
    QString to = m_replaceEdit.text();      //获取替换的文本

    if((m_pText != NULL) && (target != "")&&(to != ""))     //不为空
    {
        QString text = m_pText->toPlainText();      //获取所有的文本
        text.replace(target,to,m_matchChkBx.isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive); //在临时的text中把所有的符合要求的文本给替换掉

        m_pText->clear();  //将原来的编辑器中的文本给清空

        m_pText->insertPlainText(text);     //把替换后的所有文本放置到编辑器中
    }
}


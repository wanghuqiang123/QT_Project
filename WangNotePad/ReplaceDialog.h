#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H
#include "FindDialog.h"

class ReplaceDialog : public FindDialog
{
    Q_OBJECT
protected:
    QLabel m_replaceLB;
    QLineEdit m_replaceEdit;
    QPushButton m_replaceBtn;
    QPushButton m_replaceAllBtn;

    void initControl();
    void connetSlot();
protected slots:
    void onReplaceCliked();
    void onReplaceAllCliked();
public:
    explicit ReplaceDialog(QWidget* parent = 0,QPlainTextEdit* pText = 0);

};

#endif // REPLACEDIALOG_H

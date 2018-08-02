#ifndef _FINDDIALOG_H_
#define _FINDDIALOG_H_

#include <QWidget>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QPointer>

class FindDialog : public QDialog
{
    Q_OBJECT

protected:
    QGroupBox m_radioGrpBx;

    QGridLayout m_layout;
    QHBoxLayout m_hbLayout;

    QLabel m_findLbl;
    QLineEdit m_findEdit;
    QPushButton m_findBtn;
    QPushButton m_closeBtn;
    QCheckBox m_matchChkBx;
    QRadioButton m_forwardBtn;
    QRadioButton m_backwardBtn;

    QPointer<QPlainTextEdit>m_pText;   //指针所指向的实际对象不会被销毁

    void initControl();
    void connetSlot();
public:
    explicit FindDialog(QWidget* parent = 0);
    void setPlainTextEdit(QPlainTextEdit* pText);
    QPlainTextEdit* getPlainTextEdit();
    bool event(QEvent* evt);

protected slots:
    void onFindClickd();
    void onCloseClickd();

};

#endif // _FINDDIALOG_H_

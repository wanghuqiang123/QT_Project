#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel m_logoLbl;
    QPlainTextEdit m_infoEdit;
    QPushButton m_closeBtn;
    QLabel m_qqnumber;
public:
    explicit AboutDialog(QWidget* parent = 0);
};

#endif // ABOUTDIALOG_H

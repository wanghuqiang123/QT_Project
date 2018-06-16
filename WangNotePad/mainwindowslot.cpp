#include "mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

void MainWindow::showErroeMessage(const QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical); //critical :临界的 ，关键的 批评的
    msg.setStandardButtons(QMessageBox::Ok);
    msg.show();
}
QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog fd(this);
    QStringList filters;

    filters.append("Text File (*.txt)");  //append  :追加 附上；
    filters.append("C++ (*.cpp)");
    filters.append("C (*.c)");
    filters.append("All Files (*)");

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);
    fd.setNameFilters(filters);  //filter : 过滤

    if(mode == QFileDialog::AcceptOpen)
    {
        fd.setFileMode(QFileDialog::ExistingFile); //exist :存在
    }

    if(fd.exec() == QFileDialog::Accepted)
    {
        ret = fd.selectedFiles()[0];
    }

    return ret;
}
void MainWindow::onFileOpen()
{
    QString path = showFileDialog(QFileDialog::AcceptOpen,"open");

    if(path != "")
    {
        QFile file(path);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            mainedit.setPlainText(QString(file.readAll()));
            file.close();

            m_filepath = path;
            setWindowTitle("NotePad - ["+m_filepath+"]");
        }
        else
        {
            showErroeMessage(QString("open file error! \n\n") + QString("\"") + QString(path) + QString("\""));
        }
    }
}

void MainWindow::onFileSave()
{
    if(m_filepath == "")
    {
        m_filepath = showFileDialog(QFileDialog::AcceptSave,"Save");
    }
    if(m_filepath != "")
    {
        QFile file(m_filepath);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
             QTextStream out(&file);
             out<<mainedit.toPlainText();
             file.close();
             setWindowTitle("NotePad - ["+m_filepath+"]");
        }
        else
        {
             showErroeMessage(QString("Save file error! \n\n") + QString("\"") + QString(m_filepath) + QString("\""));
             m_filepath = "";
        }
    }
}

void MainWindow::onFileSaveAs()
{
    QString path = showFileDialog(QFileDialog::AcceptSave,"Save as");

    if(path != "")
    {
        QFile file(path);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out<<mainedit.toPlainText();
            file.close();
            m_filepath = path;
            setWindowTitle("NotePad - ["+m_filepath+"]");
        }
        else
        {
             showErroeMessage(QString("Save file error! \n\n") + QString("\"") + QString(m_filepath) + QString("\""));
             m_filepath = "";
        }
    }
}



















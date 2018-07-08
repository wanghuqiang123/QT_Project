#include "mainwindow.h"
#include "AboutDialog.h"
#include "AppConfig.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include <QCloseEvent>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QFileInfo>
#include <QObjectList>
#include <QMenuBar>
#include <QToolBar>
#include <QPrintDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QKeyEvent>
#include <QApplication>
#include <QInputDialog>
#include <QStatusBar>
#include <QToolBar>
#include <QFontDialog>
#include <QDesktopServices>

//错误消息提示对话框
void MainWindow::showErroeMessage(const QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical); //critical :临界的 ，关键的 批评的
    msg.setStandardButtons(QMessageBox::Ok);
    msg.show();
}
//询问对话框
 int MainWindow::showQueryMessage(QString Message)
 {
     QMessageBox msg(this);

     msg.setWindowTitle("Query");
     msg.setText(Message);
     msg.setIcon(QMessageBox::Question);
     msg.setStandardButtons(QMessageBox::Ok | QMessageBox::No | QMessageBox::Cancel);
     return  msg.exec();
 }
//此函数调用filedialog 选择为保存还是打开模式
QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog fd(this);       //在当前程序设置一个文件对话框
    QStringList filters;        //filters文件的格式列表
    QMap<QString,QString>map;
    const char* filterArray[][2] =
    {
      {"Text File (*.txt)",".txt"},
      {"All Files (*)","*"},
      {"C++ (*.cpp)",".cpp"},
      {"C (*.c)",".c"},
      {NULL,NULL}
    };                        //文件过滤器所有名称的数组

    for(int i = 0;filterArray[i][0] != NULL;i++)
    {
        filters.append(filterArray[i][0]);          //append  :追加 附上； filters保存所有的文件类型名称
        map.insert(filterArray[i][0],filterArray[i][1]);    //把不同的文件类型名称的后缀名对应上去
    }

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);       //文件的选择模式   保存还是打开
    fd.setNameFilters(filters);  //filter : 过滤  只查看filters中文件类型名称的文件

    if(mode == QFileDialog::AcceptOpen)          //如果为打开模式
    {
        fd.setFileMode(QFileDialog::ExistingFile); //exist :存在    文件需要存在
    }

    if(fd.exec() == QFileDialog::Accepted)   //用户选择了
    {
        ret = fd.selectedFiles()[0];   //把所选择的第一个文件路径返回

        if(mode == QFileDialog::AcceptSave)   //当用保存的模式来运行文件对话框的时候
        {
            QString posfix = map[fd.selectedNameFilter()];    //文件对话框中所要保存的文件类型名  posfix将其键对应的值保存  即后缀名

            if((posfix != "*") && !ret.endsWith(posfix))           //若用户没有自己添加后缀  而且保存文件类型也不是为All file类型
            {
                ret = ret+posfix;  //路径名加上后缀名；
            }
        }
    }

    return ret;
}

void MainWindow::preEditorChange()
{
    if(m_isTextChange)   //若当前路径中的文件没有保存
    {
        int r = showQueryMessage("Do you want to save the currrent changes to file?");
        switch (r)
        {
         case QMessageBox::Ok:
            saveCurrentData(m_filepath);  //按照文件路径保存文件
            break;
         case QMessageBox::No:
            m_isTextChange = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}

void MainWindow::onFileNew()  //建立一个新的文件
{
    preEditorChange(); //检测当前文件是否更改，若更改没保存 ，可以保存
    if(!m_isTextChange)
    {
        mainedit.clear();
        setWindowTitle("NotePad - [New]");
        m_filepath = "";
        m_isTextChange = false;
    }
}

void MainWindow::openFileToEditor(QString path) //打开文件内容到编辑器
{
    if(path != "")
    {
        QFile file(path);    //打开文件
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))   //用只读和文本的方式打开
        {
            mainedit.setPlainText(QString(file.readAll()));   //读取所有的内容到平面上
            file.close();

            m_filepath = path;                             //设置当前编辑文件的文件的路径为path
            m_isTextChange = false;                        //没改变  表示已经保存
            setWindowTitle("NotePad - ["+m_filepath+"]");
        }
        else
        {
            showErroeMessage(QString("open file error! \n\n") + QString("\"") + QString(path) + QString("\""));
        }
    }
}

//文件打开函数
void MainWindow::onFileOpen()
{
    preEditorChange();
    if(!m_isTextChange)   //如果当前路径的文件已经保存了或者未保存  判断过后  直接打开另外文件；
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen,"open"); //返回的是要打开的文件的路径

        openFileToEditor(path);
    }

}

void MainWindow::openFile(QString path)    //此函数功能用于在点击文本时默认打开文本编辑器
{
    preEditorChange();
    if(!m_isTextChange)
    {
        openFileToEditor(path);
    }
}

//保存当前的文本内容数据
QString MainWindow::saveCurrentData(QString path)
{
    QString ret = path;

    if(ret == "")  //当路径为空的时候  证明未保存  弹出对话框   选择保存路径
    {
        ret = showFileDialog(QFileDialog::AcceptSave,"Save");  //选择文件路径
    }
    if(ret != "")  //如果已经保存在路径下的时候，则不弹出对话框询问 ，直接保存更新的内容；
    {
        QFile file(ret);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) //以写入文本的方式打开
        {
             QTextStream out(&file);
             out<<mainedit.toPlainText();
             file.close();
             setWindowTitle("NotePad - ["+ret+"]");  //把程序的标题设置为路径名
             m_isTextChange = false;                    //是否有内容改变标记变量   现在保存后没有更新了就为false
        }
        else
        {
             showErroeMessage(QString("Save file error! \n\n") + QString("\"") + QString(ret) + QString("\""));
             ret = "";
        }
    }
    return ret;
}



//保存函数
void MainWindow::onFileSave()
{
    QString path = saveCurrentData(m_filepath);  //按照当前的路径保存；返回所保存文件的路径；
    if(path != "")
    {
        m_filepath = path;
    }
}
//另存为的函数
void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData();       //重新选择保存的文件路径
    if(path != "")
    {
        m_filepath = path;
    }
}
//当文本编辑器中的内容发生变化时触发此函数。
void MainWindow::onTextChanged()
{
    if(!m_isTextChange)
    {
        setWindowTitle("Editing..."+windowTitle());
    }
    m_isTextChange = true;
}

void MainWindow::closeEvent(QCloseEvent* e)  //关闭程序时若未保存则询问是否保存当前内容
{
    preEditorChange();
    if(!m_isTextChange)
    {
        QFont font = mainedit.font();
        bool isWrap = (mainedit.lineWrapMode() == QPlainTextEdit::WidgetWidth);
        bool tbVisible = toolbar()->isVisible();
        bool sb =   statusBar()->isVisible();
        AppConfig config(font,pos(),size(),isWrap,tbVisible,sb);        //获得当前程序中的个状态参数 存入到配置文件中

        config.store();

        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e) // 拖放事件处理；
{
    if(e->mimeData()->hasUrls())  //获取所拖动的数据文件的路径
    {
        e->acceptProposedAction();  //获得该事件的处理动作
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *e)  //这里的拖动的放下事件是对应的QWidget的  所以需要将QPlainText中的给关闭
{
    if(e->mimeData()->hasUrls())
    {
            QList<QUrl>list = e->mimeData()->urls();   //获得文件路径
            QString path = list[0].toLocalFile();
            QFileInfo fi(path);
            if(fi.isFile())    //是否是文件
            {
                preEditorChange();  //当前文件是否保存

                if(!m_isTextChange)   //如果已经保存
                {
                    openFileToEditor(path);  //将所拖动的文件路径传递给此函数
                }
            }
            else
            {
                showErroeMessage("Cannot open file");  //如果不是文件  则提示无法打开
            }
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::onCopyAvailable(bool available)
{
    findMenuBarAction("Copy")->setEnabled(available);   //若编辑器有文本输入  发生了变化 将action动作对应的槽函数设置为true; 触发后，传来的available为true;
    findMenuBarAction("Cut")->setEnabled(available);
    findToolBarAction("Copy")->setEnabled(available);
    findToolBarAction("Cut")->setEnabled(available);

}
void MainWindow::onRedoAvailable(bool available)
{
    findMenuBarAction("Redo")->setEnabled(available);
    findToolBarAction("Redo")->setEnabled(available);
}
void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction("Undo")->setEnabled(available);
    findToolBarAction("Undo")->setEnabled(available);
}

QAction* MainWindow::findMenuBarAction(QString text)
{
    QAction* ret = NULL;
    const QObjectList& list = menuBar()->children();

    for(int i = 0;i<list.count();i++)
    {
        QMenu* menu = dynamic_cast<QMenu*>(list[i]);

        if(menu != NULL)
        {
            QList<QAction*>actions = menu->actions();
            for(int j = 0;j<actions.count();j++)
            {
                if(actions[j]->text().startsWith(text))
                {
                    ret = actions[j];
                    break;
                }
            }
        }
    }
    return ret;
}
QAction* MainWindow::findToolBarAction(QString text)   //列出子类查找与text所对应的标签的action对象  然后返回其指针
{
    QAction* ret = NULL;
    QToolBar* toolBar = toolbar();
    QList<QAction*>actions = toolBar->actions();

    for(int j = 0;j<actions.count();j++)
    {
        if(actions[j]->toolTip().startsWith(text))
        {
            ret = actions[j];
            break;
        }
    }

    return ret;
}

void MainWindow::onFilePrint()
{
    QPrintDialog dlg(this);    //弹出打印对话框
    dlg.setWindowTitle("Print");    //对话框标题

    if(dlg.exec() == QPrintDialog::Accepted)        //等待按下是否 选择是打印
    {
        QPrinter* p =  dlg.printer();               //获取打印机对象

        mainedit.document()->print(p);              //将编辑器中的内容传给打印机打印。
    }
}

void MainWindow::onCursorPositiongChanged()         //在状态栏显示当前的光标位置
{
    int pos = mainedit.textCursor().position();  //在文本编辑框中光标所在的字符位置；
    QString text = mainedit.toPlainText();      //获取当前编辑器的文本内容；
    int col = 0;          //列
    int line = 1;         //行
    int flag = -1;

    for(int i = 0;i<pos;i++)
    {
        if(text[i] == '\n')
        {
            line++;
            flag = i;
        }
    }
    flag++;
    col = pos-flag;     //flag表示上一行的所有字符数目。

    statusLbl.setText("Ln: "+QString::number(line)+"   col: "+QString::number(col));

}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier);    //该事件模拟（同步）键盘上的delete键  当该事件发送时 文本编辑器上的相应内容被删除
    QKeyEvent keyRelease(QEvent::KeyRelease,Qt::Key_Delete,Qt::NoModifier);

    QApplication::sendEvent(&mainedit,&keyPress);      //把该事件发送给mainedit中
    QApplication::sendEvent(&mainedit,&keyRelease);


}

void MainWindow::onFileExit()
{
    close();
}
void MainWindow::onEditFind()
{
    m_pfindDialog->show();
}

void MainWindow::onReplace()
{
    m_pReplaceDlg->show();
}

void MainWindow::onEditGoto()
{
    bool ok = false;
    int ln = QInputDialog::getInt(this,"Goto","Line: ",1,1,mainedit.document()->lineCount(),1,&ok);
    //从输入对话框中输入获取用户输入的整形值，
    if(ok)
    {
        QString text = mainedit.toPlainText();  //以纯文本形式返回文本编辑的文本。
        QTextCursor c = mainedit.textCursor();
        int pos = 0;   //跳转到的位置；
        int next = -1;

        for(int i = 0;i<ln;i++)
        {
            pos = next+1;
            next = text.indexOf('\n',pos);  //当遇到换行符的时候跳转到下一行
        }

        c.setPosition(pos);
        mainedit.setTextCursor(c);   //在主窗口中设置光标
    }
}

void MainWindow::onViewToolbar()
{
    QToolBar* tb = toolbar();

    bool visible = tb->isVisible();   //取得工具栏的可见性状态

    tb->setVisible(!visible);          //取反

    findMenuBarAction("Tool Bar")->setChecked(!visible);  //在菜单栏中取反（打勾表示），

}
void MainWindow::onViewStatusbar()
{
     QStatusBar* sb = statusBar();
     bool visible = sb->isVisible();

     sb->setVisible(!visible);

     findMenuBarAction("Status Bar")->setCheckable(!visible);
     findToolBarAction("Status Bar")->setCheckable(!visible);
}

void MainWindow::onHelpAbout()
{
    AboutDialog(this).exec();
}

void MainWindow::onFormatFont()       //设置字体格式大小
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,mainedit.font(),this);

    if(ok)
    {
        mainedit.setFont(font);
    }
}

void MainWindow::onFormatWrap()   //换行转换
{
    QPlainTextEdit::LineWrapMode mode = mainedit.lineWrapMode();
    if(mode == QPlainTextEdit::NoWrap)
    {
        mainedit.setLineWrapMode(QPlainTextEdit::WidgetWidth); //当前输入的字符串长度与窗口长度相同  自动换行
        findMenuBarAction("Auto Warp(W)")->setChecked(true);
        findToolBarAction("Wrap")->setChecked(true);
    }
    else
    {
        mainedit.setLineWrapMode(QPlainTextEdit::NoWrap);

        findMenuBarAction("Auto Warp(W)")->setChecked(false);
        findToolBarAction("Wrap")->setChecked(false);
    }
}
void MainWindow::onHelpManual()
{
    QDesktopServices::openUrl(QUrl("https://github.com/wanghuqiang123/QT_Project/tree/master/WangNotePad"));
}

void MainWindow::onBackStyleSet()
{

}








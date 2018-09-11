
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
    QMessageBox msg(&m_tabwidget);

    msg.setWindowTitle("Error");
    msg.setText(message);
    msg.setIcon(QMessageBox::Critical); //critical :临界的 ，关键的 批评的
    msg.setStandardButtons(QMessageBox::Ok);
    msg.show();
}
//询问对话框
 int MainWindow::showQueryMessage(QString Message)
 {
     QMessageBox msg(&m_tabwidget);

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
    QFileDialog fd(&m_tabwidget);       //在当前程序设置一个文件对话框
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

void MainWindow::preEditorChange(int index)
{
    if(m_isTextChange[index])
    {
        int r = showQueryMessage("Do you want to save the currrent changes to file?");
        switch (r)
        {
         case QMessageBox::Ok:
            saveCurrentData(m_filepath);  //按照文件路径保存文件
            isClose = false; //可以关闭
            break;
         case QMessageBox::No:
            m_isTextChange[index] = false;
            isClose = false;
            break;
        case QMessageBox::Cancel:
            isClose = true;
            break;
        }
    }
}

void MainWindow::onFileNew()  //建立一个新的文件
{
        QString Tabnum = QString::number(v_mainedit.count()+1);
        openNewTab("New "+ Tabnum);
        setWindowTitle("NotePad - [New]");
        m_filepath = "";

}

void MainWindow::openFileToEditor(QString path) //打开文件内容到编辑器
{
    if(path != "")
    {
        QFile file(path);    //打开文件
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))   //用只读和文本的方式打开
        {
            openNewTab(file.fileName());

            v_mainedit[v_mainedit.size()-1]->setPlainText(QString(file.readAll()));   //读取所有的内容到平面上
            m_isTextChange[m_tabwidget.currentIndex()] = false; //此语句的作用是上句语句将文本打印在平面编辑器上时调用了onTextChange，将标志设置为了true;

            setSavename(v_mainedit.size()-1,file.fileName());

            file.close();

            m_filepath = path;                             //设置当前编辑文件的文件的路径为path
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
    QString path = showFileDialog(QFileDialog::AcceptOpen,"open"); //返回的是要打开的文件的路径

    openFileToEditor(path);

}

void MainWindow::openFile(QString path)    //此函数功能用于在点击文本时默认打开文本编辑器
{
      openFileToEditor(path);
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
             out<<v_mainedit[m_tabwidget.currentIndex()]->toPlainText();
             file.close();
             setWindowTitle("NotePad - ["+ret+"]");  //把程序的标题设置为路径名
            m_isTextChange[m_tabwidget.currentIndex()] = false;                    //是否有内容改变标记变量   现在保存后没有更新了就为false
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
        setSavename(m_tabwidget.currentIndex(),path);
    }
}
//另存为的函数
void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData();       //重新选择保存的文件路径
    if(path != "")
    {
        m_filepath = path;
        setSavename(m_tabwidget.currentIndex(),path);
    }
}
//当文本编辑器中的内容发生变化时触发此函数。
void MainWindow::onTextChanged()
{
    if(!m_isTextChange[m_tabwidget.currentIndex()])  //
    {
        setWindowTitle("Editing..."+windowTitle());
        m_isTextChange[m_tabwidget.currentIndex()] = true;
    }

}

void MainWindow::onCloseTab(int index)
{
        preEditorChange(index);
        if(m_tabwidget.count() == 1)
        {
            QFont font = v_mainedit[index]->font();
            bool isWrap = (v_mainedit[index]->lineWrapMode() == QPlainTextEdit::WidgetWidth);
            bool tbVisible = toolbar()->isVisible();
            bool sb =   statusBar()->isVisible();
            AppConfig config(font,pos(),size(),isWrap,tbVisible,sb,m_BackStyleSet);        //获得当前程序中的个状态参数 存入到配置文件中

            config.store();
        }
        if(!isClose)
        {
            m_tabwidget.removeTab(index);  //移除选择的标签  index表示被移除的标签的下标号  从0开始； 移除这个标签并不能销毁标签中的对象以及分配的内存
            removeEditPoint(index);    //这里是调用remove去掉QVector中的被删除的下标对应的元素   为指向编辑器的指针； 使其的下标号和TabWidget中的下标号和地址值相对应
        }
}

void MainWindow::closeEvent(QCloseEvent* e)  //关闭程序时若未保存则询问是否保存当前内容
{
    while(m_tabwidget.count() != 0 && isClose != true)
    {
        onCloseTab(m_tabwidget.count()-1);
    }

    if(!isClose)
    {
        QMainWindow::closeEvent(e);

    }
    else
    {
        e->ignore();
    }
    isClose = false;
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
                preEditorChange(m_tabwidget.currentIndex());  //当前文件是否保存

                if(!m_isTextChange[m_tabwidget.currentIndex()])   //如果已经保存
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
    QPrintDialog dlg(&m_tabwidget);    //弹出打印对话框
    dlg.setWindowTitle("Print");    //对话框标题

    if(dlg.exec() == QPrintDialog::Accepted)        //等待按下是否 选择是打印
    {
        QPrinter* p =  dlg.printer();               //获取打印机对象

        v_mainedit[m_tabwidget.currentIndex()]->document()->print(p);              //将编辑器中的内容传给打印机打印。
    }
}

void MainWindow::onCursorPositiongChanged()         //在状态栏显示当前的光标位置
{
    if(m_tabwidget.count() != 0)
    {
        int pos = v_mainedit[m_tabwidget.currentIndex()]->textCursor().position();  //在文本编辑框中光标所在的字符位置；
        QString text = v_mainedit[m_tabwidget.currentIndex()]->toPlainText();      //获取当前编辑器的文本内容；
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
}

void MainWindow::onEditDelete()
{
    QKeyEvent keyPress(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier);    //该事件模拟（同步）键盘上的delete键  当该事件发送时 文本编辑器上的相应内容被删除
    QKeyEvent keyRelease(QEvent::KeyRelease,Qt::Key_Delete,Qt::NoModifier);

    QApplication::sendEvent(v_mainedit[m_tabwidget.currentIndex()],&keyPress);      //把该事件发送给mainedit中
    QApplication::sendEvent(v_mainedit[m_tabwidget.currentIndex()],&keyRelease);


}

void MainWindow::onFileExit()
{
    close();
}
void MainWindow::onEditFind()
{
    if(m_tabwidget.count() != 0)
    {
        m_pfindDialog->setPlainTextEdit(v_mainedit[m_tabwidget.currentIndex()]);
        m_pfindDialog->show();
    }
}

void MainWindow::onReplace()
{
    if(m_tabwidget.count() != 0)
    {
         m_pReplaceDlg->setPlainTextEdit(v_mainedit[m_tabwidget.currentIndex()]);
         m_pReplaceDlg->show();
    }
}

void MainWindow::onEditGoto()
{
    bool ok = false;
    int ln = QInputDialog::getInt(&m_tabwidget,"Goto","Line: ",1,1,v_mainedit[m_tabwidget.currentIndex()]->document()->lineCount(),1,&ok);
    //从输入对话框中输入获取用户输入的整形值，
    if(ok)
    {
        QString text = v_mainedit[m_tabwidget.currentIndex()]->toPlainText();  //以纯文本形式返回文本编辑的文本。
        QTextCursor c = v_mainedit[m_tabwidget.currentIndex()]->textCursor();
        int pos = 0;   //跳转到的位置；
        int next = -1;

        for(int i = 0;i<ln;i++)
        {
            pos = next+1;
            next = text.indexOf('\n',pos);  //当遇到换行符的时候跳转到下一行
        }

        c.setPosition(pos);
        v_mainedit[m_tabwidget.currentIndex()]->setTextCursor(c);   //在主窗口中设置光标
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
    AboutDialog(&m_tabwidget).exec();
}

void MainWindow::onFormatFont()       //设置字体格式大小
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,v_mainedit[m_tabwidget.currentIndex()]->font(),&m_tabwidget);

    if(ok)
    {
        v_mainedit[m_tabwidget.currentIndex()]->setFont(font);
    }
}

void MainWindow::onFormatWrap()   //换行转换
{
    QPlainTextEdit::LineWrapMode mode = v_mainedit[m_tabwidget.currentIndex()]->lineWrapMode();
    if(mode == QPlainTextEdit::NoWrap)
    {
        v_mainedit[m_tabwidget.currentIndex()]->setLineWrapMode(QPlainTextEdit::WidgetWidth); //当前输入的字符串长度与窗口长度相同  自动换行
        findMenuBarAction("Auto Warp(W)")->setChecked(true);
        findToolBarAction("Wrap")->setChecked(true);
    }
    else
    {
        v_mainedit[m_tabwidget.currentIndex()]->setLineWrapMode(QPlainTextEdit::NoWrap);

        findMenuBarAction("Auto Warp(W)")->setChecked(false);
        findToolBarAction("Wrap")->setChecked(false);
    }
}
void MainWindow::onHelpManual()
{
    QDesktopServices::openUrl(QUrl("https://github.com/wanghuqiang123/QT_Text_editor"));
}

void MainWindow::onBackStyleSetNew()
{
    if(m_BackStyleSet)
    {
        m_BackStyleSet = false;
        onBackStyleSet();
    }
}
void MainWindow::onBackStyleSet()
{

    if(!m_BackStyleSet)    //如果背景标志为假的  则可以用户点击的时候背景变成灰色  并且将标志位设置为false
    {
        if(m_tabwidget.count() != 0)
        {
            for(int i = 0;i<m_tabwidget.count();i++)
            {
                QPalette p = v_mainedit[i]->palette();
                p.setColor(QPalette::Active,QPalette::Base,Qt::black);
                p.setColor(QPalette::Inactive,QPalette::Base,Qt::black);     //Base用于设置的是窗口小部件的背景色   在这里是高亮行的背景色
                p.setColor(QPalette::Active,QPalette::Background,Qt::black);
                p.setColor(QPalette::Inactive,QPalette::Background,Qt::black);
                p.setColor(QPalette::Active,QPalette::Text,Qt::gray);
                p.setColor(QPalette::Inactive,QPalette::Text,Qt::gray);


                v_mainedit[i]->setAutoFillBackground(true);
                v_mainedit[i]->setBackgroundVisible(true);
                v_mainedit[i]->setBackgroundRole(QPalette::Base);
                v_mainedit[i]->setBackgroundRole(QPalette::Background);
                v_mainedit[i]->setBackgroundRole(QPalette::Text);
                v_mainedit[i]->setPalette(p);
            }
            m_BackStyleSet = true;
        }
    }
    else
    {
        if(m_tabwidget.count() != 0)
        {
            for(int i = 0;i<m_tabwidget.count();i++)
            {
                QPalette p = v_mainedit[i]->palette();
                p.setColor(QPalette::Active,QPalette::Base,Qt::white);
                p.setColor(QPalette::Inactive,QPalette::Base,Qt::white);     //Base用于设置的是窗口小部件的背景色   在这里是高亮行的背景色
                p.setColor(QPalette::Active,QPalette::Background,Qt::white);
                p.setColor(QPalette::Inactive,QPalette::Background,Qt::white);
                p.setColor(QPalette::Active,QPalette::Text,Qt::black);
                p.setColor(QPalette::Inactive,QPalette::Text,Qt::black);


                v_mainedit[i]->setAutoFillBackground(true);
                v_mainedit[i]->setBackgroundVisible(true);
                v_mainedit[i]->setBackgroundRole(QPalette::Base);
                v_mainedit[i]->setBackgroundRole(QPalette::Background);
                v_mainedit[i]->setBackgroundRole(QPalette::Text);
                v_mainedit[i]->setPalette(p);
            }
            m_BackStyleSet = false;
        }
    }
}

QString MainWindow::getFilename(const QString path)
{
    QString ret = path;
    QStringList list = ret.split("/",QString::SkipEmptyParts);

    ret = list.back();
    return ret;
}


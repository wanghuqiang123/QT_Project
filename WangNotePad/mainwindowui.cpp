#include "mainwindow.h"
#include <QMenu>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QLabel>

MainWindow::MainWindow() : m_pfindDialog(new FindDialog(this,&mainedit))
{
    m_filepath = "";
    m_isTextChange = false;
    setWindowTitle("NotePad - [New]");
    setAcceptDrops(true);   //设置为可以放下app外的拖动事件
}

MainWindow* MainWindow::NewInstance()
{
    MainWindow* ret = new MainWindow();

    if( (ret == NULL) || !ret->construct() )
    {
        delete ret;
        ret = NULL;
    }

    return ret;
}

bool MainWindow::construct()
{
    bool ret = true;

    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEditor();
    return ret;
}

bool MainWindow::initMenuBar()
{
    bool ret = true;
    QMenuBar* mb = menuBar();

    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initSetMenu(mb);
    ret = ret && initHelpMenu(mb);

    return ret;
}

bool MainWindow::initToolBar()
{
    bool ret = true;
    QToolBar* tb = addToolBar("Tool bar");
    tb->setIconSize(QSize(16,16));
    ret = ret && initFileToolItem(tb);
    return ret;

}

bool MainWindow::initStatusBar()    //底部状态栏的设置
{
    bool ret = true;
    QStatusBar * sb = statusBar();
    QLabel* label = new QLabel("WangNotepd");
    if(label != NULL)
    {
        label->setMinimumWidth(200);
        label->setAlignment(Qt::AlignCenter);
        statusLbl.setMinimumWidth(200);
        statusLbl.setAlignment(Qt::AlignCenter);
        statusLbl.setText("ln: 1     col: 1");

        sb->addPermanentWidget(&statusLbl);
        sb->addPermanentWidget(label);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::initMainEditor()
{
    bool ret = true;
    mainedit.setParent(this);

    connect(&mainedit,SIGNAL(textChanged()),this,SLOT(onTextChanged()));  //当文本编辑器内容发生变化时  触发信号 对应到槽函数去

    connect(&mainedit,SIGNAL(copyAvailable(bool)),this,SLOT(onCopyAvailable(bool)));    //当前的文本内容中有复制信号的触发（当有文本被选择后产生触发），调用该程序中的槽函数。
    connect(&mainedit,SIGNAL(redoAvailable(bool)),this,SLOT(onRedoAvailable(bool)));    //当有文本输入时，即为可以重做，因此信号发出true，给槽函数设置为true;
    connect(&mainedit,SIGNAL(undoAvailable(bool)),this,SLOT(onUndoAvailable(bool)));

    connect(&mainedit,SIGNAL(cursorPositionChanged()),this,SLOT(onCursorPositiongChanged()));

    setCentralWidget(&mainedit);                    //设置在APP窗口中间；
    mainedit.setAcceptDrops(false); //Qplaintext不能放置拖动的放置事件  这样会把路径都输入
    return ret;
}
bool MainWindow::initFileMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("File(F)",mb);
    bool ret = (menu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        ret = ret && makeAction(action,menu, "New(N) ", Qt::CTRL + Qt::Key_N);

        if( ret )
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onFileNew()));
            menu->addAction(action);    // add Action item to Menu
        }

        ret = ret && makeAction(action,menu, "Open(O) ",Qt::CTRL + Qt::Key_O);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onFileOpen()));
            menu->addAction(action);
        }

       ret = ret && makeAction(action,menu, "Save(S) ",Qt::CTRL + Qt::Key_S);
       if(ret)
       {
           connect(action,SIGNAL(triggered()),this,SLOT(onFileSave())); //triggered:触发
           menu->addAction(action);
       }

       ret = ret && makeAction(action,menu, "Save As...",0);
       if(ret)
       {
           connect(action,SIGNAL(triggered()),this,SLOT(onFileSaveAs()));
            menu->addAction(action);
       }

       ret = ret && makeAction(action,menu, "Save All ",Qt::CTRL + Qt::SHIFT + Qt::Key_S);
       if(ret)
       {
           menu->addAction(action);
       }

       menu->addSeparator();

       ret = ret && makeAction(action,menu, "Print(P) ",Qt::CTRL + Qt::Key_P);
       if(ret)
       {
           connect(action,SIGNAL(triggered(bool)),this,SLOT(onFilePrint()));
           menu->addAction(action);
       }

       menu->addSeparator();

       ret = ret && makeAction(action,menu,  "Exit(X) ", Qt::CTRL + Qt::Key_X);
        if( ret )
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onFileExit()));
            menu->addAction(action);    // add Action item to Menu
        }
    }

    if( ret )
    {
        mb->addMenu(menu);    // add Menu add to application Menu Bar
    }
    else
    {
        delete menu;
    }


    return ret;
}

bool MainWindow::initEditMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("Edit(E)",mb);
    bool ret = (menu != NULL);

    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,menu, "Undo(U)",Qt::CTRL + Qt::Key_Z);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),&mainedit,SLOT(undo()));  //一旦该动作被触发  调用Qt中封装的undo（）函数即可。
            menu->addAction(action);
            action->setEnabled(false);   //默认状态下该动作不能使用
        }

        ret = ret && makeAction(action,menu, "Redo",Qt::CTRL + Qt::Key_Y);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),&mainedit,SLOT(redo()));
            action->setEnabled(false);
            menu->addAction(action);
        }
        menu->addSeparator();

        ret = ret && makeAction(action,menu, "Cut ",Qt::CTRL + Qt::Key_X);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),&mainedit,SLOT(cut()));
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Copy ",Qt::CTRL + Qt::Key_C);
        if(ret)
        {
             connect(action,SIGNAL(triggered()),&mainedit,SLOT(copy()));
             action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Paste ",Qt::CTRL + Qt::Key_V);
        if(ret)
        {
             connect(action,SIGNAL(triggered()),&mainedit,SLOT(paste()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Delete",Qt::Key_Delete);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onEditDelete()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,menu, "Find",Qt::CTRL + Qt::Key_F);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onEditFind()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Replace",Qt::CTRL + Qt::Key_H);
        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Goto",Qt::CTRL + Qt::Key_G);
        if(ret)
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,menu, "Select All",Qt::CTRL + Qt::Key_A);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),&mainedit,SLOT(selectAll()));
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}
bool MainWindow::initFormatMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("Format(M)",mb);
    bool ret = (menu != NULL);
    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,menu, "Auto Warp(W)",0);
        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Font",0);
        if(ret)
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}
bool MainWindow::initViewMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("View(V)",mb);
    bool ret = (menu != NULL);

    if(ret)
    {
        QAction* action = NULL;
        ret = ret && makeAction(action,menu, "Tool Bar",0);
        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Status Bar",0);
        if(ret)
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}
bool MainWindow::initHelpMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("Help(H)",mb);
    bool ret = (menu != NULL);

    if(ret)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action,menu, "User Manual",0);
        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "About NotePad",0);
        if(ret)
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::initSetMenu(QMenuBar *mb)
{
    QMenu* menu = new QMenu("Setting(S)",mb);
    bool ret = (menu != NULL);

    if(ret)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action,menu, "Language Format Set",0);
        if(ret)
        {
            menu->addAction(action);
        }
    }
    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

bool MainWindow::makeAction(QAction*& action,QWidget* parent, QString text, int key)
{
    bool ret = true;

    action = new QAction(text, parent);

    if( action != NULL )
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::makeAction(QAction *&action, QWidget* parent,QString tip, QString icon)
{
    bool ret = true;

    action = new QAction("", parent);

    if( action != NULL )
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::initFileToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;

    ret = ret && makeAction(action,tb,"new",":/res/new");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFileNew()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Open",":/res/open");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFileOpen()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Save",":/res/save");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFileSave()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Save As",":/res/saveas");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onFileSaveAs()));
        tb->addAction(action);
    }
    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Undo",":/res/undo");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),&mainedit,SLOT(undo()));    //工具栏中一旦该undo动作被触发，调用qt中封装好的undo（）槽函数。
        action->setEnabled(false);                                  //默认状态下该动作为false。
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Redo",":/res/redo");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),&mainedit,SLOT(redo()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    tb->addSeparator();
    ret = ret && makeAction(action,tb,"Copy",":/res/pic/copy");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),&mainedit,SLOT(copy()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Cut",":/res/pic/cut");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),&mainedit,SLOT(cut()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Paste",":/res/pic/paste");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),&mainedit,SLOT(paste()));
        tb->addAction(action);
    }

    tb->addSeparator();
    ret = ret && makeAction(action,tb,"Find",":/res/pic/find");
    if(ret)
    {
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onEditFind()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Replace",":/res/pic/replace");
    if(ret)
    {
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Tool",":/res/pic/tool");
    if(ret)
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Status",":/res/pic/status");
    if(ret)
    {
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Font",":/res/pic/font");
    if(ret)
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Goto",":/res/pic/goto");
    if(ret)
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Wrap",":/res/pic/wrap");
    if(ret)
    {
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Print",":/res/pic/print");
    if(ret)
    {
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onFilePrint()));
        tb->addAction(action);
    }


    return ret;
}

MainWindow::~MainWindow()
{

}

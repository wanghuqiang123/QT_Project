
#include "mainwindow.h"
#include "AppConfig.h"
#include <QMenu>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QLabel>
#include <QPalette>

MainWindow::MainWindow() : m_pfindDialog(new FindDialog(this,&mainedit)),m_pReplaceDlg(new ReplaceDialog(this,&mainedit))
{
    m_filepath = "";
    m_isTextChange = false;
    setWindowTitle("NotePad - [New]");
    setAcceptDrops(true);   //设置为可以放下app外的拖动事件
    num = 0;
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
    AppConfig config;
    ret = ret && initMenuBar();
//    ret = ret && initTabWidget();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
//    ret = ret && initMainEditor();

    if(config.isValid())
    {
        if(!config.isToolBarVisible())
        {
            toolbar()->setVisible(false);
            findMenuBarAction("Tool Bar")->setChecked(false);
        }

        if(!config.isStatusBarVisible())
        {
            statusBar()->setVisible(false);  //因为状态栏实在Qt中直接调用出来的
            findMenuBarAction("Status Bar")->setChecked(false);
            findToolBarAction("Status Bar")->setChecked(false);
        }
        if(!config.StyleSet())
        {
            findMenuBarAction("Dark style Set")->setChecked(false);
        }
        else
        {
            findMenuBarAction("Dark style Set")->setChecked(true);
            onBackStyleSet();
        }

        move(config.mainWindowpoint());
        resize(config.mainWindowsize());
    }
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

bool MainWindow::initTabWidget()
{
    bool ret = true;
    m_tabwidget.setParent(this);

    initMainEditor();  //set  QPlainTextEdit  and add m_tabwidget;

    setCentralWidget(&m_tabwidget);
    m_tabwidget.setTabPosition(QTabWidget::North);
    m_tabwidget.setTabsClosable(true);

    connect(&m_tabwidget,SIGNAL(tabCloseRequested(int)),this,SLOT(onCloseTab(int)));  //if close this tabwidget,call slot onclosetab() function;

    return ret;
}


bool MainWindow::initMainEditor(QString filename)
{
    bool ret = true;
    static int n = 1;     //this n record assigned how many QPlainTextEdit in Qvector
    if(v_mainedit.size() == 0)
    {
        v_mainedit.resize(n);
    }
    else
    {
        n++;
        v_mainedit.resize(n);
    }
//    v_mainedit[0] = &mainedit;  //make a point to maineit
    v_mainedit[n-1] = new QPlainTextEdit();

    QPalette p = v_mainedit[n-1]->palette();
    p.setColor(QPalette::Inactive,QPalette::Highlight,p.color(QPalette::Active,QPalette::Highlight)); //设置被选取的字符的颜色  第三个参数表示选取高亮行的被激活后的颜色
    p.setColor(QPalette::Inactive,QPalette::HighlightedText,p.color(QPalette::Active,QPalette::HighlightedText));

    v_mainedit[n-1]->setPalette(p);
    v_mainedit[n-1]->setParent(&m_tabwidget);

    connect(v_mainedit[n-1],SIGNAL(textChanged()),this,SLOT(onTextChanged()));  //当文本编辑器内容发生变化时  触发信号 对应到槽函数去
    connect(v_mainedit[n-1],SIGNAL(copyAvailable(bool)),this,SLOT(onCopyAvailable(bool)));    //当前的文本内容中有复制信号的触发（当有文本被选择后产生触发），调用该程序中的槽函数。
    connect(v_mainedit[n-1],SIGNAL(redoAvailable(bool)),this,SLOT(onRedoAvailable(bool)));    //当有文本输入时，即为可以重做，因此信号发出true，给槽函数设置为true;
    connect(v_mainedit[n-1],SIGNAL(undoAvailable(bool)),this,SLOT(onUndoAvailable(bool)));
    connect(v_mainedit[n-1],SIGNAL(cursorPositionChanged()),this,SLOT(onCursorPositiongChanged()));

   // setCentralWidget(&mainedit);                    //设置在APP窗口中间；
    v_mainedit[n-1]->setAcceptDrops(false); //Qplaintext不能放置拖动的放置事件  这样会把路径都输入

    AppConfig config;
    v_mainedit[n-1]->setFont(config.editorFont());  //设置字体
    if(!config.isAutoWrap())
    {
        v_mainedit[n-1]->setLineWrapMode(QPlainTextEdit::NoWrap);
        findMenuBarAction("Auto Warp(W)")->setChecked(false);
        findToolBarAction("Wrap")->setChecked(false);
    }

    m_tabwidget.addTab(v_mainedit[n-1],filename);   //let new QPlainTextEdit add m_tabwidget;


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
            connect(action,SIGNAL(triggered()),this,SLOT(onEditUndo()));  //一旦该动作被触发  调用Qt中封装的undo（）函数即可。
            menu->addAction(action);
            action->setEnabled(false);   //默认状态下该动作不能使用
        }

        ret = ret && makeAction(action,menu, "Redo",Qt::CTRL + Qt::Key_Y);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onEditRedo()));
            action->setEnabled(false);
            menu->addAction(action);
        }
        menu->addSeparator();

        ret = ret && makeAction(action,menu, "Cut ",Qt::CTRL + Qt::Key_X);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onEditCut()));
            action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Copy ",Qt::CTRL + Qt::Key_C);
        if(ret)
        {
             connect(action,SIGNAL(triggered()),this,SLOT(onEditCopy()));
             action->setEnabled(false);
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Paste ",Qt::CTRL + Qt::Key_V);
        if(ret)
        {
             connect(action,SIGNAL(triggered()),this,SLOT(onEditPaste()));
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
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onReplace()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Goto",Qt::CTRL + Qt::Key_G);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onEditGoto()));
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action,menu, "Select All",Qt::CTRL + Qt::Key_A);
        if(ret)
        {
            connect(action,SIGNAL(triggered()),this,SLOT(onEditSelectAll()));
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
            action->setCheckable(true);      //设置这个action对象性质是可选择的
            action->setChecked(false);  //默认是未选择的
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onFormatWrap()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Font",0);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onFormatFont()));
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

            action->setCheckable(true);
            action->setChecked(true);
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onViewToolbar()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "Status Bar",0);
        if(ret)
        {
            action->setCheckable(true);
            action->setChecked(true);
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onViewStatusbar()));
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
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onHelpManual()));
            menu->addAction(action);
        }

        ret = ret && makeAction(action,menu, "About NotePad",0);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onHelpAbout()));
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

        menu->addSeparator();
        ret = ret && makeAction(action,menu,"Dark style Set",0);
        if(ret)
        {
            connect(action,SIGNAL(triggered(bool)),this,SLOT(onBackStyleSet()));
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
        connect(action,SIGNAL(triggered()),this,SLOT(onEditUndo()));    //工具栏中一旦该undo动作被触发，调用qt中封装好的undo（）槽函数。
        action->setEnabled(false);                                  //默认状态下该动作为false。
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Redo",":/res/redo");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onEditRedo()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    tb->addSeparator();
    ret = ret && makeAction(action,tb,"Copy",":/res/pic/copy");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onEditCopy()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Cut",":/res/pic/cut");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onEditCut()));
        action->setEnabled(false);
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Paste",":/res/pic/paste");
    if(ret)
    {
        connect(action,SIGNAL(triggered()),this,SLOT(onEditPaste()));
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
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onReplace()));
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Status Bar",":/res/pic/status");
    if(ret)
    {
        action->setCheckable(true);
        action->setChecked(true);
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onViewStatusbar()));
        tb->addAction(action);
    }

    tb->addSeparator();

    ret = ret && makeAction(action,tb,"Font",":/res/pic/font");
    if(ret)
    {
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onFormatFont()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Goto",":/res/pic/goto");
    if(ret)
    {
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onEditGoto()));
        tb->addAction(action);
    }

    ret = ret && makeAction(action,tb,"Wrap",":/res/pic/wrap");
    if(ret)
    {
        action->setCheckable(true);      //设置这个action对象性质是可选择的
        action->setChecked(false);  //默认是未选择的
        connect(action,SIGNAL(triggered(bool)),this,SLOT(onFormatWrap()));
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



QToolBar* MainWindow::toolbar()  //查找主窗口中的工具栏
{
    QToolBar* ret = NULL;
    const QObjectList& list = children();  //查找主窗口中的子类
    for(int i = 0;i<list.count();i++)
    {
        QToolBar* tb = dynamic_cast<QToolBar*>(list[i]);  //将父类指针强制转换为子类指针  如果能转换 则指针不会为空
        if(tb != NULL)
        {
            ret = tb;
            break;
        }
    }

    return ret;
}


void MainWindow::openNewTab(QString filename)
{
    if(m_tabwidget.count() == 0)   //when user push "new" ,initial m_tabwidget and assigned a new QPlainTextEdit to m_tabwidget;
    {
        initTabWidget();
        m_tabwidget.setTabText(m_tabwidget.currentIndex(),filename);   //if open saved file,rename this Tab is filename;
    }
    else
    {
        initMainEditor(filename);
    }

}

void MainWindow::DeleteVector()  //删除QVector中的指向堆中分配的指针
{
    int size = v_mainedit.size();
    for(int i = 0;i<size;i++)
        delete v_mainedit[i];
}

MainWindow::~MainWindow()
{
    DeleteVector();
}










